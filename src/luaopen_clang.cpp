/*
Luaclang is a module (loadable library) for the Lua programming language (www.lua.org) providing bindings to the LLVM/Clang APIs (www.llvm.org), in order to control LLVM/Clang from within Lua scripts.
Luaclang is built against the LLVM 2.6 stable release, for Lua 5.1.
Luaclang aims to closely follow the LLVM API, but making use of Lua idioms where possible. 

Luaclang is released under the MIT open source license
@see http://www.opensource.org/licenses/mit-license.php

Copyright (c) 2009 Graham Wakefield & Wesley Smith
http://code.google.com/p/luaclang/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include <list>
#include <set>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"

	extern int luaopen_clang(lua_State * L);
	extern int luaopen_clang_llvm(lua_State * L);
}

#include "clang/Analysis/PathDiagnostic.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/FileManager.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "clang/Driver/Driver.h"
#include "clang/Frontend/CompileOptions.h"
#include "clang/Frontend/InitHeaderSearch.h"
#include "clang/Frontend/InitPreprocessor.h"
#include "clang/Frontend/TextDiagnosticBuffer.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/ParseAST.h"

#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Config/config.h"
#include "llvm/DerivedTypes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/Linker.h"
#include "llvm/Module.h"
#include "llvm/ModuleProvider.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Type.h"
#include "llvm/PassManager.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PassNameParser.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/System/DynamicLibrary.h"
#include "llvm/System/Host.h"
#include "llvm/System/Path.h"
#include "llvm/System/Signals.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ValueSymbolTable.h"

#include "luaglue.h"

#if defined(WIN32) || defined(__WINDOWS_MM__)
	#define LUA_CLANG_WIN32 1
	#ifdef LUA_CLANG_EXPORTS
		#define LUA_CLANG_API __declspec(dllexport)
	#else
		#define LUA_CLANG_API __declspec(dllimport)
	#endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
	#define LUA_CLANG_OSX 1
	#define LUA_CLANG_API extern
#else
	#define LUA_CLANG_LINUX 1
	#define LUA_CLANG_API extern
#endif

#define ddebug(...) 
//#define ddebug(...) printf(__VA_ARGS__)

// used by almost all LLVM types:
template<typename T>
int llvm_print(lua_State * L, T * u) {
	std::stringstream s;
	u->print(s);
	lua_pushfstring(L, "%s", s.str().c_str());
	return 1;
}

void llvmErrorHandler(void * userdata, const std::string &Message) {
	std::cerr << "llvm error:" << Message;
}

using namespace llvm;

class Compiler;

/* 
	Statics
*/
#pragma mark Statics

struct ForceJITLinking forcejitlinking;
static ExecutionEngine * EE = 0;
static std::list<std::string> global_includes;

/* 
	LLVM bindings
*/
#pragma mark LLVM bindings

#include "luaopen_clang_llvm.cpp"

/*
	Execution Engine
*/
#pragma mark Execution Engine
void eeRegisterModule(lua_State * L, Compiler * c, Module * module);

/*
	Class to snoop on what the JIT is doing:
*/
class LuaclangJITEventListener : public JITEventListener {
public:
	LuaclangJITEventListener() : JITEventListener() {}
	virtual ~LuaclangJITEventListener() {}

	virtual void NotifyFunctionEmitted(const Function &F,
                                     void *Code, size_t Size,
                                     const JITEvent_EmittedFunctionDetails &Details) {
		printf("JIT emitted Function %s at %p, size %d\n", 
			F.getName().data(), Code, (int)Size);
	}

	virtual void NotifyFreeingMachineCode(const Function &F, void *OldPtr) {
		printf("JIT freed Function %s at %p\n", 
			F.getName().data(), OldPtr);
	}
};
static LuaclangJITEventListener gLuaclangJITEventListener;

/*
	Clang compilation wrapper
*/
#pragma mark Compiler
class Compiler {
public:
	std::string		name;
	Module *	module;
	ModuleProvider * emp;
	std::list<std::string>	iflags;
	std::list<std::string>	dflags;
	std::list<std::string>	wflags;
	bool freeMachineCodeForFunction;
	
	
	Compiler(lua_State * L) : module(NULL) {
		name = luaL_optstring(L, 1, "untitled");
		// add standard headers?
		// add standard defines (e.g. platform)?
		freeMachineCodeForFunction = false;
	}
	~Compiler() {
		std::string err;
		if (module) {
			EE->runStaticConstructorsDestructors(module, true);
			EE->clearGlobalMappingsFromModule(module);
			
			if (freeMachineCodeForFunction) {
				// iterate functions and remove from EE:
				for (Module::iterator i = module->begin(), e = module->end(); i != e; ++i) {
					EE->freeMachineCodeForFunction(i);
				}
			}
			
			module->dropAllReferences();
			
			printf("clang removed module %s %s\n", module->getModuleIdentifier().data(), err.data());
		}
		// this isn't safe, since we may be using things created in m:
		//ee->deleteModuleProvider(mp); EE->removeModuleProvider(emp, &err);
	}
	
	/*
		Note: after this call, src is no longer a valid module
		(it has been absorbed into module instead)
	*/
	bool link(lua_State * L, Module * src) {
		std::string err;
		if (module) {
			Linker::LinkModules(module, src, &err);
			if (err.length()) {
				luaL_error(L, "link error: %s", err.data());
				return false;
			}
		} else {
			module = src;
		}
		return true;
	}
};

template <> const char * Glue<Compiler>::usr_name() { return "Compiler"; }

template <> Compiler * Glue<Compiler>::usr_new(lua_State * L) {
	return new Compiler(L);
}
template <> void Glue<Compiler>::usr_gc(lua_State * L, Compiler * self) {
	delete self;
}
static int compiler_compile(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	std::string unitsrc = luaL_checkstring(L, 2);
	std::string unitname = luaL_optstring(L, 3, "untitled");
	
	// Souce to compile
	llvm::MemoryBuffer *buffer = llvm::MemoryBuffer::getMemBufferCopy(unitsrc.c_str(), unitsrc.c_str() + unitsrc.size(), unitname.c_str());
	if(!buffer) {
		luaL_error(L, "couldn't load %s", unitsrc.c_str());
	}
	
	// Diagnostics (warning/error handling)
	clang::TextDiagnosticBuffer client;
	clang::Diagnostic diags(&client);
	
	//------------------------------------------------------
	// Platform info
	clang::TargetInfo *target = clang::TargetInfo::CreateTargetInfo(llvm::sys::getHostTriple());
	
	
	llvm::StringMap<bool> Features;
	//target->getDefaultFeatures(std::string("yonah"), Features); // detects SSE and other processor-specific settings
	target->getDefaultFeatures(std::string(""), Features); // detects SSE and other processor-specific settings
	target->HandleTargetFeatures(Features);	// sets SSELevel for x86 targets
	
//	uint64_t width = target->getPointerWidth(0);	// 32
//	printf("3DNOW: %s\n", Features.lookup(std::string("3dnow")) ? "TRUE" : "FALSE");
//	printf("3DNOWA: %s\n", Features.lookup(std::string("3dnowa")) ? "TRUE" : "FALSE");
//	printf("MMX: %s\n", Features.lookup(std::string("mmx")) ? "TRUE" : "FALSE");
//	printf("SSE: %s\n", Features.lookup(std::string("sse")) ? "TRUE" : "FALSE");
//	printf("SSE2: %s\n", Features.lookup(std::string("sse2")) ? "TRUE" : "FALSE");
//	printf("SSE3: %s\n", Features.lookup(std::string("sse3")) ? "TRUE" : "FALSE");
//	printf("SSSE3: %s\n", Features.lookup(std::string("ssse3")) ? "TRUE" : "FALSE");
//	printf("SSSE41: %s\n", Features.lookup(std::string("ssse41")) ? "TRUE" : "FALSE");
//	printf("SSSE42: %s\n", Features.lookup(std::string("ssse42")) ? "TRUE" : "FALSE");
	

	clang::LangOptions lang;
	client.setLangOptions(&lang);

	// from clang-cc:684
	// Allow the target to set the default the langauge options as it sees fit.
	target->getDefaultLangOptions(lang);
	lang.C99 = 1;
	lang.HexFloats = 1;
	lang.BCPLComment = 1;  // Only for C99/C++.
	lang.Digraphs = 1;     // C94, C99, C++.
	lang.Trigraphs = 0;	// UPDATE
	// GNUMode - Set if we're in gnu99, gnu89, gnucxx98, etc.
	lang.GNUMode = 1;
	lang.ImplicitInt = 0;
	lang.DollarIdents = 1;
	lang.LaxVectorConversions = 1;	// UPDATE
	lang.WritableStrings = 0;
	lang.Exceptions = 0;
	lang.Rtti = 0;
//	lang.Rtti = 1;	// UPDATE
	lang.NoBuiltin = 0;	// UPDATE
	
	lang.Bool = 0;
	lang.MathErrno = 0;
//	lang.MathErrno = 1;	// UPDATE
	lang.InstantiationDepth = 99;
	lang.OptimizeSize = 0;
	lang.PICLevel = 1;
//	lang.PICLevel = 0;	// UPDATE
	lang.GNUInline = 0;
	lang.NoInline = 1;
	lang.Static = 0;

	//------------------------------------------------------
	// Search paths
	clang::FileManager fm;
	clang::HeaderSearch headers(fm);

	bool verbose_headers = false;
	clang::InitHeaderSearch initHeaders(headers, verbose_headers);
	for (std::list<std::string>::iterator it = c->iflags.begin(); it != c->iflags.end(); it++) {
		initHeaders.AddPath(*it, 
							clang::InitHeaderSearch::Angled, 
							false, 
							true, 
							false);
	}
	for (std::list<std::string>::iterator it = global_includes.begin(); it != global_includes.end(); it++) {
		initHeaders.AddPath(*it, 
							clang::InitHeaderSearch::Angled, 
							false, 
							true, 
							false);
	}
	// Add system search paths
	initHeaders.AddDefaultSystemIncludePaths(lang);
	initHeaders.AddDefaultEnvVarPaths(lang);
	initHeaders.Realize();
	
	//------------------------------------------------------
	// Preprocessor 

	clang::SourceManager sm;
	clang::Preprocessor pp(diags, lang, *target, sm, headers);
	
	// TODO: verify correctness of this
	std::string predefines;
	for (std::list<std::string>::iterator it = c->dflags.begin(); it != c->dflags.end(); it++) {
		predefines.append("\n");
		predefines.append(*it);
	}
	pp.setPredefines(predefines);
	
	clang::PreprocessorInitOptions InitOpts;
	if(InitializePreprocessor(pp, InitOpts)) {
		printf("ERROR initializing preprocessor\n");
	}
	
	/*
	// NECESSARY???
	pp->getBuiltinInfo().InitializeBuiltins(pp->getIdentifierTable(),
										  pp->getLangOptions().NoBuiltin);
	*/
	
	//------------------------------------------------------
	// Source compilation
	sm.createMainFileIDForMemBuffer(buffer);

	clang::ASTContext context(pp.getLangOptions(), sm, pp.getTargetInfo(), pp.getIdentifierTable(), pp.getSelectorTable(), pp.getBuiltinInfo());
	clang::CompileOptions copts; // e.g. optimizations
	clang::CodeGenerator * codegen = CreateLLVMCodeGen(diags, unitname, copts, llvm::getGlobalContext());
	
	clang::ParseAST(pp, codegen, context, false); // last flag is verbose statistics
	llvm::Module * cmodule = codegen->ReleaseModule(); // or GetModule() if we want to reuse it?
	if (cmodule) 
	{
		//------------------------------------------------------
		// Successful compilation
		
		// link module:
		eeRegisterModule(L, c, cmodule);
		
		lua_pushboolean(L, true);
		lua_pushfstring(L, "successfully compiled %s.%s", c->name.data(), unitname.data());
	} else {
		//------------------------------------------------------
		// Failed compilation
		
		unsigned ecount = 0, wcount = 0;
		for(clang::TextDiagnosticBuffer::const_iterator it = client.err_begin();
			it != client.err_end();
			++it)
		{
			clang::FullSourceLoc SourceLoc = clang::FullSourceLoc(it->first, sm);;
			int LineNum = SourceLoc.getInstantiationLineNumber();
			int ColNum = SourceLoc.getInstantiationColumnNumber();
			int FileOffset = SourceLoc.getManager().getFileOffset(SourceLoc);

			std::pair< const char *, const char * > LocBD = SourceLoc.getBufferData();
			char errstr[128];
			int start = (FileOffset < 10) ? 0 : FileOffset-10;
			strncpy(errstr, LocBD.first+start, 10);
			errstr[10] = '^';
			strncpy(errstr+11, LocBD.first+start+10, 10);
			errstr[21] = '\0';			

			printf("%s %d:%d\n'%s'\n%s\n", 
								SourceLoc.getManager().getBufferName(SourceLoc),
								LineNum, ColNum, 
								errstr,
								it->second.data());
			ecount++;
			
			if(ecount > 250) break;
		}

//		for(clang::TextDiagnosticBuffer::const_iterator it = client.warn_begin();
//			it != client.warn_end();
//			++it)
//		{
//			clang::FullSourceLoc SourceLoc = clang::FullSourceLoc(it->first, sm);;
//			int LineNum = SourceLoc.getInstantiationLineNumber();
//			int ColNum = SourceLoc.getInstantiationColumnNumber();
//			int FileOffset = SourceLoc.getManager().getFileOffset(SourceLoc);
//
//			std::pair< const char *, const char * > LocBD = SourceLoc.getBufferData();
//			char errstr[128];
//			int start = (FileOffset < 10) ? 0 : FileOffset-10;
//			strncpy(errstr, LocBD.first+start, 10);
//			errstr[10] = '^';
//			strncpy(errstr+11, LocBD.first+start+10, 10);
//			errstr[21] = '\0';
//
//			printf("%s %d:%d\n'%s'\n%s\n",
//								SourceLoc.getManager().getBufferName(SourceLoc),
//								LineNum, ColNum, 
//								errstr,
//								it->second.data());
//			wcount++;
//			
//			if(wcount > 250) break;
//		}
		
		lua_pushboolean(L, false);
		lua_pushfstring(L, "compile failed: %d errors, %d warnings", ecount, wcount);
	}
	delete codegen;
	delete target;
	
	return 2;
}
static int compiler_include(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (lua_isstring(L, 2) )
		c->iflags.push_back(lua_tostring(L, 2)); 
	return 0;
}
static int compiler_define(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (lua_isstring(L, 2) )
		c->dflags.push_back(lua_tostring(L, 2)); 
	return 0;
}
static int compiler_warn(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (lua_isstring(L, 2) )
		c->wflags.push_back(lua_tostring(L, 2)); 
	return 0;
}
static int compiler_dump(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (c->module) c->module->dump();
	return 0;
}
static int compiler_functions(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	lua_newtable(L);
	int n = 1;
	for (Module::iterator i = c->module->begin(), e = c->module->end(); i != e; ++i) {
		lua_pushstring(L, i->getName().data());
		lua_rawseti(L, -2, n++);
	}
	return 1;
}
static int compiler_writebitcode(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	const char * filename = luaL_checkstring(L, 2);
	std::ofstream ofile(filename, std::ios_base::out | std::ios_base::trunc);
	llvm::WriteBitcodeToFile(c->module, ofile);
	ofile.close();
	return 0;
}

static int compiler_readbitcode(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	const char * filename = luaL_checkstring(L, 2);
	std::string err;
	llvm::MemoryBuffer * buffer = llvm::MemoryBuffer::getFile(filename, &err);
	if (err.size()) {
		luaL_error(L, "%s: %s", filename, err.data());
	}
	llvm::Module * bitcodemodule = llvm::ParseBitcodeFile(buffer, (llvm::getGlobalContext()), &err);
	delete buffer;
	if (err.size()) {
		luaL_error(L, "%s: %s", filename, err.data());
	}
	eeRegisterModule(L, c, bitcodemodule);
	return 0;
}

static int compiler_optimize(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	std::string olevel = luaL_optstring(L, 2, "02");
	if (c->module == 0) { return 0; }
	
	llvm::TargetData * targetdata = new llvm::TargetData(c->module);
	llvm::PassManager pm;
	pm.add(targetdata);
	
	if (olevel == std::string("01")) {
		pm.add(llvm::createPromoteMemoryToRegisterPass());
		pm.add(llvm::createInstructionCombiningPass());
		pm.add(llvm::createCFGSimplificationPass());
		pm.add(llvm::createVerifierPass(llvm::PrintMessageAction));
	} else if (olevel == std::string("03")) {
		pm.add(llvm::createCFGSimplificationPass());
		pm.add(llvm::createScalarReplAggregatesPass());
		pm.add(llvm::createInstructionCombiningPass());
		pm.add(llvm::createRaiseAllocationsPass());   // call %malloc -> malloc inst
		pm.add(llvm::createCFGSimplificationPass());       // Clean up disgusting code
		pm.add(llvm::createPromoteMemoryToRegisterPass()); // Kill useless allocas
		pm.add(llvm::createGlobalOptimizerPass());       // OptLevel out global vars
		pm.add(llvm::createGlobalDCEPass());          // Remove unused fns and globs
		pm.add(llvm::createIPConstantPropagationPass()); // IP Constant Propagation
		pm.add(llvm::createDeadArgEliminationPass());   // Dead argument elimination
		pm.add(llvm::createInstructionCombiningPass());   // Clean up after IPCP & DAE
		pm.add(llvm::createCFGSimplificationPass());      // Clean up after IPCP & DAE
		pm.add(llvm::createPruneEHPass());               // Remove dead EH info
		pm.add(llvm::createFunctionAttrsPass());         // Deduce function attrs
		pm.add(llvm::createFunctionInliningPass());      // Inline small functions
		pm.add(llvm::createArgumentPromotionPass());  // Scalarize uninlined fn args
		pm.add(llvm::createSimplifyLibCallsPass());    // Library Call Optimizations
		pm.add(llvm::createInstructionCombiningPass());  // Cleanup for scalarrepl.
		pm.add(llvm::createJumpThreadingPass());         // Thread jumps.
		pm.add(llvm::createCFGSimplificationPass());     // Merge & remove BBs
		pm.add(llvm::createScalarReplAggregatesPass());  // Break up aggregate allocas
		pm.add(llvm::createInstructionCombiningPass());  // Combine silly seq's
		pm.add(llvm::createCondPropagationPass());       // Propagate conditionals
		pm.add(llvm::createTailCallEliminationPass());   // Eliminate tail calls
		pm.add(llvm::createCFGSimplificationPass());     // Merge & remove BBs
		pm.add(llvm::createReassociatePass());           // Reassociate expressions
		pm.add(llvm::createLoopRotatePass());            // Rotate Loop
		pm.add(llvm::createLICMPass());                  // Hoist loop invariants
		pm.add(llvm::createLoopUnswitchPass());
		pm.add(llvm::createLoopIndexSplitPass());        // Split loop index
		pm.add(llvm::createInstructionCombiningPass());
		pm.add(llvm::createIndVarSimplifyPass());        // Canonicalize indvars
		pm.add(llvm::createLoopDeletionPass());          // Delete dead loops
		pm.add(llvm::createLoopUnrollPass());          // Unroll small loops
		pm.add(llvm::createInstructionCombiningPass()); // Clean up after the unroller
		pm.add(llvm::createGVNPass());                   // Remove redundancies
		pm.add(llvm::createMemCpyOptPass());            // Remove memcpy / form memset
		pm.add(llvm::createSCCPPass());                  // Constant prop with SCCP
		pm.add(llvm::createInstructionCombiningPass());
		pm.add(llvm::createCondPropagationPass());       // Propagate conditionals
		pm.add(llvm::createDeadStoreEliminationPass());  // Delete dead stores
		pm.add(llvm::createAggressiveDCEPass());   // Delete dead instructions
		pm.add(llvm::createCFGSimplificationPass());     // Merge & remove BBs
		pm.add(llvm::createStripDeadPrototypesPass()); // Get rid of dead prototypes
		pm.add(llvm::createDeadTypeEliminationPass());   // Eliminate dead types
		pm.add(llvm::createConstantMergePass());       // Merge dup global constants
		pm.add(llvm::createVerifierPass(llvm::PrintMessageAction));
	} else {
		pm.add(llvm::createCFGSimplificationPass());
		pm.add(llvm::createFunctionInliningPass());
		pm.add(llvm::createJumpThreadingPass());
		pm.add(llvm::createPromoteMemoryToRegisterPass());
		pm.add(llvm::createInstructionCombiningPass());
		pm.add(llvm::createCFGSimplificationPass());
		pm.add(llvm::createScalarReplAggregatesPass());
		pm.add(llvm::createLICMPass());
		pm.add(llvm::createCondPropagationPass());
		pm.add(llvm::createGVNPass());
		pm.add(llvm::createSCCPPass());
		pm.add(llvm::createAggressiveDCEPass());
		pm.add(llvm::createCFGSimplificationPass());
		pm.add(llvm::createVerifierPass(llvm::PrintMessageAction));
	}
	
	pm.run(*c->module);
	lua_pushfstring(L, "optimized module %s", c->name.data());
	return 1;
}

static int compiler_getfunction(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	const char * funcname = luaL_checkstring(L, 2);
	if (EE == 0) {
		luaL_error(L, "no execution engine");
	}
	llvm::Function * f = c->module->getFunction(funcname);
	if (f == 0) {
		luaL_error(L, "function %s not found", funcname);
	}
	lua_pushlightuserdata(L, EE->getPointerToFunction(f));
	return 1;
}

static int compiler_getluafunction(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	const char * funcname = luaL_checkstring(L, 2);
	if (EE == 0) {
		luaL_error(L, "no execution engine");
	}
	llvm::Function * f = c->module->getFunction(funcname);
	if (f == 0) {
		luaL_error(L, "function %s not found", funcname);
	}
	lua_pushcfunction(L, (lua_CFunction)EE->getPointerToFunction(f));
	return 1;
}

static int compiler_datalayout(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (c->module) {
		lua_pushstring(L, c->module->getDataLayout().data());
		return 1;
	}
	return 0;
}

static int compiler_targettriple(lua_State * L) {
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (c->module) {
		lua_pushstring(L, c->module->getTargetTriple().data());
		return 1;
	}
	return 0;
}

static int compiler_module(lua_State * L) {
	luaL_dostring(L, "require 'clang.llvm'");
	Compiler * c = Glue<Compiler>::checkto(L, 1);
	if (c->module && c->emp) {
		Glue<ModuleProvider>::push(L, c->emp);
		return 1;
	}
	return 0;
}

template <> void Glue<Compiler>::usr_mt(lua_State * L, int mt) {
	lua_pushcfunction(L, compiler_compile);			lua_setfield(L, mt, "compile");
	lua_pushcfunction(L, compiler_include);			lua_setfield(L, mt, "include");
	lua_pushcfunction(L, compiler_warn);			lua_setfield(L, mt, "warn");
	lua_pushcfunction(L, compiler_define);			lua_setfield(L, mt, "define");
	lua_pushcfunction(L, compiler_dump);			lua_setfield(L, mt, "dump");
	lua_pushcfunction(L, compiler_functions);		lua_setfield(L, mt, "functions");
	lua_pushcfunction(L, compiler_writebitcode);	lua_setfield(L, mt, "writebitcode");
	lua_pushcfunction(L, compiler_readbitcode);		lua_setfield(L, mt, "readbitcode");
	lua_pushcfunction(L, compiler_optimize);		lua_setfield(L, mt, "optimize");
	lua_pushcfunction(L, compiler_getfunction);		lua_setfield(L, mt, "getfunction");
	lua_pushcfunction(L, compiler_getluafunction);	lua_setfield(L, mt, "getluafunction");
	lua_pushcfunction(L, compiler_datalayout);		lua_setfield(L, mt, "datalayout");
	lua_pushcfunction(L, compiler_targettriple);	lua_setfield(L, mt, "targettriple");
	lua_pushcfunction(L, compiler_module);			lua_setfield(L, mt, "module");
}

/*
	EE should be created on first use (first created module)
	Subsequent modules should re-use the same EE:
*/
void eeRegisterModule(lua_State * L, Compiler * c, Module * module) {
	
	std::string err;
	llvm::ExistingModuleProvider * emp = new llvm::ExistingModuleProvider(module);
	
	// register with JIT (create if necessary)
	if (EE == 0) {
		
		// EE does not exist; create it (using this module as a basis):
		EE = llvm::ExecutionEngine::createJIT(
			emp,	// module provider
			&err,	// error string
			0,		// JITMemoryManager
			llvm::CodeGenOpt::Default,	// JIT slowly (None, Default, Aggressive)
			false	// allocate GlobalVariables separately from code
		);
		
		if (EE == 0) {
			luaL_error(L, "Failed to create Execution Engine: %s", err.data());
			return;
		} else {
			printf("created Execution Engine %p\n", EE);
		}
		
		// turn this off when not debugging:
		EE->RegisterJITEventListener(&gLuaclangJITEventListener);
		
		//EE->InstallLazyFunctionCreator(lazyfunctioncreator);
		EE->DisableLazyCompilation(true);
		//EE->DisableGVCompilation();
		
		// When we ask to JIT a function, we should also JIT other
		// functions that function depends on.  This would let us JIT in a
		// background thread to avoid blocking the main thread during
		// codegen.
		//EE->DisableLazyCompilation();
		
		c->emp = emp;
	}
	
	if (c->module == NULL) {
		// assign new module:
		c->module = module;
		c->emp = emp;
		EE->addModuleProvider(emp);
	
		// I guess we should do this:
		module->setTargetTriple(llvm::sys::getHostTriple());
		
		// unladen swallow also does this:
		module->setDataLayout(EE->getTargetData()->getStringRepresentation());
		
	} else {
		// c has a module already, so just link to it:
		c->link(L, module);
	}
	
//	// Fill the ExecutionEngine with the addresses of known global variables.
//	for (Module::global_iterator it = module->global_begin();
//		it != module->global_end(); ++it) {
//		EE->getOrEmitGlobalVariable(it);
//	}
	
	// run all static constructors:
	EE->runStaticConstructorsDestructors(c->module, false);
}

//int addSearchPath(lua_State * L) {
//	std::string path = luaL_checkstring(L, 1);
//	// TODO: avoid double insertions
//	global_includes.push_back(path);
//	return 0;
//}

/*
	Lua interactions
*/
#pragma mark Lua
int lua_pointer(lua_State * L) {
	if (lua_toboolean(L, 1)) {
		lua_pushvalue(L, lua_upvalueindex(1));
	} else {
		lua_pushlightuserdata(L, L);
	}
	return 1;
}

/* 
	It's not safe to do anything with EE here, in case any other independent lua_State is also using the singleton EE pointer. 
	The question is, how can we be sure that the EE has released all memory associated with this script?
*/
int luaclose_clang(lua_State * L) {
	if (EE) {
		// TODO: enable some of these?
//		EE->clearAllGlobalMappings();
//		EE->UnregisterJITEventListener(&gLuaclangJITEventListener);
//		delete EE;
//		EE = 0;
	}
	return 0;
}

/*
	get a callback when the value at stack index idx is collected:
*/
static void gc_sentinel(lua_State * L, int idx, lua_CFunction callback) {

	lua_pushvalue(L, idx); // value @idx
	lua_newuserdata(L, sizeof(void *)); // sentinel userdata
		lua_newtable(L);	// userdata metatable with __gc = callback
		lua_pushcfunction(L, callback);

		lua_setfield(L, -2, "__gc");
		lua_setmetatable(L, -2);

	/* check for (weak-valued) sentinel table; create if needed */
	lua_getfield(L, LUA_REGISTRYINDEX, "module_gc_sentinels");
	if (lua_isnoneornil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);

		// make weak-keyed
		lua_pushstring(L, "v");
		lua_setfield(L, -2, "__mode");
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -2);
		lua_pushvalue(L, -1);
		lua_setfield(L, LUA_REGISTRYINDEX, "module_gc_sentinels");
	}

	lua_insert(L, -3);
	lua_insert(L, -2);
	lua_settable(L, -3); // lua::sentinel[value @idx] = sentinel userdata
	lua_pop(L, 1); // lua::sentinel
}

static int clanglib_initialized = 0;

int luaopen_clang(lua_State * L) {

	if (clanglib_initialized == 0) {
		clanglib_initialized = 1;
		// Set an error handler, so that any LLVM backend diagnostics go through our
		// error handler.
		llvm::llvm_install_error_handler(llvmErrorHandler, NULL);

		llvm::InitializeAllTargetInfos();
		llvm::InitializeAllTargets();
		llvm::InitializeAllAsmPrinters();
		if (llvm::InitializeNativeTarget()) 
			luaL_error(L, "InitializeNativeTarget failure");
	}
		
	const char * libname = "clang";
	struct luaL_reg lib[] = {
		//{"lua_pointer", lua_pointer},
		{NULL, NULL},
	};
	luaL_register(L, libname, lib);
	
	lua_pushlightuserdata(L, L);
	lua_pushcclosure(L, lua_pointer, 1);
	lua_setfield(L, -2, "lua_pointer");
	
	Glue<Compiler>::define(L);				
	Glue<Compiler>::register_ctor(L);
	
	// unload handler
	//gc_sentinel(L, lua_gettop(L), luaclose_clang); 
	
	return 1;
}