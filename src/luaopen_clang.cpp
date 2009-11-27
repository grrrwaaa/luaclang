/*
Luaclang is a module (loadable library) for the Lua programming language (www.lua.org) providing bindings to the LLVM/Clang APIs (www.llvm.org), in order to control LLVM/Clang from within Lua scripts.
Luaclang is built against the LLVM 2.6 stable release, for Lua 5.1.
Luaclang aims to closely follow the LLVM API, but making use of Lua idioms where possible. 

Luaclang is released under the MIT open source license
@see http://www.opensource.org/licenses/mit-license.php

Copyright (c) 2009 Graham Wakefield
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
#include "llvm/Linker.h"
#include "llvm/Module.h"
#include "llvm/ModuleProvider.h"
#include "llvm/Target/Targetdata.h"
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

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include <list>
#include <set>

#include "luaglue.h"
#include "luaopen_clang.h"

//#define ddebug(...) 
#define ddebug(...) printf(__VA_ARGS__)

extern llvm::ExecutionEngine * getEE();
extern void registerWithJIT(lua_State * L, llvm::Module * module);

// used by almost all LLVM types:
template<typename T>
int llvm_print(lua_State * L, T * u) {
	std::stringstream s;
	u->print(s);
	lua_pushfstring(L, "%s", s.str().c_str());
	return 1;
}

using namespace llvm;
struct ForceJITLinking forcejitlinking;

/* 
	Module
*/
#pragma mark Module
template<> int llvm_print<llvm::Module>(lua_State * L, Module * u) {
	std::stringstream s;
	u->print(s, 0);
	lua_pushfstring(L, "%s", s.str().c_str());
	return 1;
}

template<> const char * Glue<Module>::usr_name() { return "Module"; }
template<> int Glue<Module>::usr_tostring(lua_State * L, Module * u) {
	lua_pushfstring(L, "%s: %s(%p)", usr_name(), u->getModuleIdentifier().c_str(), u);
	return 1;
}
static int module_linkto(lua_State * L) {
	std::string err;
	Module * self = Glue<Module>::checkto(L, 1);
	Module * mod = Glue<Module>::checkto(L, 2);
	llvm::Linker::LinkModules(self, mod, &err);
	if (err.length())
		luaL_error(L, err.c_str());
	return 0;
}

int module_writeBitcodeFile(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	std::ofstream ofile(luaL_checkstring(L, 2), std::ios_base::out | std::ios_base::trunc);
	WriteBitcodeToFile(m, ofile);
	ofile.close();
	return 0;
}

static int module_dump(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	m->dump();
	return 0;
}

int module_addTypeName(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	const Type * ty = Glue<Type>::checkto(L, 2);
	const char * name = luaL_checkstring(L, 3);
	lua_pushboolean(L, m->addTypeName(name, ty));
	return 1;
}
int module_getTypeName(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	const Type * ty = Glue<Type>::checkto(L, 2);
	lua_pushstring(L, m->getTypeName(ty).c_str());
	return 1;
}
int module_getTypeByName(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	const char * name = luaL_checkstring(L, 2);
	const Type * t = m->getTypeByName(name);
	if (t == 0)
		return luaL_error(L, "Type %s not found", name);
	return Glue<Type>::push(L, (Type *)t);
}
// A utility function that adds a pass to the pass manager but will also add
// a verifier pass after if we're supposed to verify.
static inline void addPass(PassManager &PM, Pass *P) {
	// Add the pass to the pass manager...
	PM.add(P);
//	// If we are verifying all of the intermediate steps, add the verifier...
//	if (VerifyEach)
//		PM.add(createVerifierPass());
}

int module_optimize(lua_State * L) {
	PassManager Passes;
	Module * M = Glue<Module>::checkto(L, 1);
	addPass(Passes, new TargetData(M));
	
	bool DisableOptimizations = lua_toboolean(L, 2);
	bool DisableInline = lua_toboolean(L, 3);;


	// @see https://llvm.org/svn/llvm-project/llvm/branches/non-call-eh/tools/llvm-ld/Optimize.cpp
	if (!DisableOptimizations) {
//		// Now that composite has been compiled, scan through the module, looking
//		// for a main function.  If main is defined, mark all other functions
//		// internal.
//		if (!DisableInternalize)
//			addPass(Passes, createInternalizePass(true));

		// Propagate constants at call sites into the functions they call.  This
		// opens opportunities for globalopt (and inlining) by substituting function
		// pointers passed as arguments to direct uses of functions.  
		addPass(Passes, createIPSCCPPass());

		// Now that we internalized some globals, see if we can hack on them!
		addPass(Passes, createGlobalOptimizerPass());

		// Linking modules together can lead to duplicated global constants, only
		// keep one copy of each constant...
		addPass(Passes, createConstantMergePass());

		// Remove unused arguments from functions...
		addPass(Passes, createDeadArgEliminationPass());

		// Reduce the code after globalopt and ipsccp.  Both can open up significant
		// simplification opportunities, and both can propagate functions through
		// function pointers.  When this happens, we often have to resolve varargs
		// calls, etc, so let instcombine do this.
		addPass(Passes, createInstructionCombiningPass());

		if (!DisableInline)
			addPass(Passes, createFunctionInliningPass()); // Inline small functions

		addPass(Passes, createPruneEHPass());            // Remove dead EH info
		addPass(Passes, createGlobalOptimizerPass());    // Optimize globals again.
		addPass(Passes, createGlobalDCEPass());          // Remove dead functions

		// If we didn't decide to inline a function, check to see if we can
		// transform it to pass arguments by value instead of by reference.
		addPass(Passes, createArgumentPromotionPass());

		// The IPO passes may leave cruft around.  Clean up after them.
		addPass(Passes, createInstructionCombiningPass());
		addPass(Passes, createJumpThreadingPass());        // Thread jumps.
		addPass(Passes, createScalarReplAggregatesPass()); // Break up allocas

		// Run a few AA driven optimizations here and now, to cleanup the code.
		//addPass(Passes, createGlobalsModRefPass());      // IP alias analysis

		addPass(Passes, createLICMPass());               // Hoist loop invariants
		addPass(Passes, createGVNPass());                  // Remove redundancies
		addPass(Passes, createMemCpyOptPass());          // Remove dead memcpy's
		addPass(Passes, createDeadStoreEliminationPass()); // Nuke dead stores

		// Cleanup and simplify the code after the scalar optimizations.
		addPass(Passes, createInstructionCombiningPass());

		addPass(Passes, createJumpThreadingPass());        // Thread jumps.
		addPass(Passes, createPromoteMemoryToRegisterPass()); // Cleanup jumpthread.

		// Delete basic blocks, which optimization passes may have killed...
		addPass(Passes, createCFGSimplificationPass());

		// Now that we have optimized the program, discard unreachable functions...
		addPass(Passes, createGlobalDCEPass());
	}

//	// If the -s or -S command line options were specified, strip the symbols out
//	// of the resulting program to make it smaller.  -s and -S are GNU ld options
//	// that we are supporting; they alias -strip-all and -strip-debug.
//	if (Strip || StripDebug)
//		addPass(Passes, createStripSymbolsPass(StripDebug && !Strip));
//
//	// Create a new optimization pass for each one specified on the command line
//	std::auto_ptr<TargetMachine> target;
//	for (unsigned i = 0; i < OptimizationList.size(); ++i) {
//		const PassInfo *Opt = OptimizationList[i];
//		if (Opt->getNormalCtor())
//			addPass(Passes, Opt->getNormalCtor()());
//		else
//			luaL_error(L, "llvm-ld: cannot create pass: %s", Opt->getPassName());
//	}

	// The user's passes may leave cruft around. Clean up after them them but
	// only if we haven't got DisableOptimizations set
	if (!DisableOptimizations) {
		addPass(Passes, createInstructionCombiningPass());
		addPass(Passes, createCFGSimplificationPass());
		addPass(Passes, createAggressiveDCEPass());
		addPass(Passes, createGlobalDCEPass());
	}

	// Make sure everything is still good.
	Passes.add(createVerifierPass());
	
	Passes.run(*M);
	return 0;
}
template<> Module * Glue<Module>::usr_new(lua_State * L) {
	const char * modulename = luaL_checkstring(L, 1);
	Module * M = new Module(modulename, getGlobalContext()); 
	return M;
}
template<> void Glue<Module>::usr_push(lua_State * L, Module * u) {
	registerWithJIT(L, u);
}
template<> void Glue<Module> :: usr_mt(lua_State * L) {
	lua_pushcfunction(L, module_linkto); lua_setfield(L, -2, "linkto");
	lua_pushcfunction(L, module_optimize); lua_setfield(L, -2, "optimize");
	lua_pushcfunction(L, module_dump); lua_setfield(L, -2, "dump");
	lua_pushcfunction(L, module_addTypeName); lua_setfield(L, -2, "addTypeName");
	lua_pushcfunction(L, module_getTypeName); lua_setfield(L, -2, "getTypeName");
	lua_pushcfunction(L, module_getTypeByName); lua_setfield(L, -2, "getTypeByName");
	lua_pushcfunction(L, module_writeBitcodeFile); lua_setfield(L, -2, "writeBitcodeFile");
}

/* 
	ModuleProvider
*/
#pragma mark ModuleProvider
template<> const char * Glue<ModuleProvider>::usr_name() { return "ModuleProvider"; }
template<> int Glue<ModuleProvider>::usr_tostring(lua_State * L, ModuleProvider * u) {
	lua_pushfstring(L, "%s: %s(%p)", usr_name(), u->getModule()->getModuleIdentifier().c_str(), u);
	return 1;
}
template<> ModuleProvider * Glue<ModuleProvider>::usr_new(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	return new ExistingModuleProvider(m);
}

/*
	Execution Engine
*/
#pragma mark EE
static ExecutionEngine * EE = 0;
ExecutionEngine * getEE() {
	return EE;
}

void registerWithJIT(lua_State * L, Module * module) {
	ExistingModuleProvider * emp = new ExistingModuleProvider(module);

	// register with JIT (create if necessary)
	if (EE == 0) {
		std::string err;
		EE = ExecutionEngine::createJIT(emp, &err, 0, CodeGenOpt::Default, false);
		if (EE == 0) {
			luaL_error(L, "Failed to create Execution Engine %p: %s\n", EE, err.c_str());
		}
	} else {
		EE->addModuleProvider(emp);
	}
}




static void Lua2GV(lua_State * L, int idx, GenericValue & v, const Type * t) {
	switch(t->getTypeID()) {
		case Type::VoidTyID:
			break;
		case Type::FloatTyID:
			v.FloatVal = luaL_optnumber(L, idx, 0);
			break;
		case Type::DoubleTyID:
			v.DoubleVal = luaL_optnumber(L, idx, 0);
			break;
		case Type::IntegerTyID:
			int i = luaL_optinteger(L, idx, 0);
			v.IntVal = APInt(((IntegerType *)t)->getBitWidth(), i);
			break;
		case Type::PointerTyID:
			v = PTOGV(lua_touserdata(L, idx));
			break;
		case Type::OpaqueTyID:
		case Type::FunctionTyID:
		case Type::StructTyID:
		case Type::ArrayTyID:
		case Type::VectorTyID:
			v.PointerVal = lua_touserdata(L, idx);
			break;
	}
}
static int GV2Lua(lua_State * L, GenericValue & v, const Type * t) {
	switch(t->getTypeID()) {
		case Type::VoidTyID:
			lua_pushnil(L);
			return 1;
		case Type::FloatTyID:
			lua_pushnumber(L, v.FloatVal);
			return 1;
		case Type::DoubleTyID:
			lua_pushnumber(L, v.DoubleVal);
			return 1;
		case Type::IntegerTyID:
			if (v.IntVal.getBitWidth() == 1) {
				lua_pushnumber(L, v.IntVal.getBoolValue());
				return 1;
			}
			lua_pushinteger(L, v.IntVal.getSExtValue());
			return 1;
		case Type::PointerTyID:
		case Type::OpaqueTyID:
		case Type::FunctionTyID:
		case Type::StructTyID:
		case Type::ArrayTyID:
		case Type::VectorTyID:
			lua_pushlightuserdata(L, v.PointerVal);
			return 1;
	}
	return 0;
}

static int ee_getTargetData(lua_State * L) {
	ExecutionEngine * ee = getEE();
	const TargetData * td = ee->getTargetData();

	lua_newtable(L);
	// LLVM string representation of TargetData
	lua_pushstring(L, td->getStringRepresentation().c_str());
	lua_setfield(L, -2, "representation");
	// endianness:
	lua_pushboolean(L, td->isLittleEndian());
	lua_setfield(L, -2, "litteEndian");
	// pointer alignment
	lua_pushinteger(L, td->getPointerABIAlignment());
	lua_setfield(L, -2, "pointerABIAlignment");
	lua_pushinteger(L, td->getPointerPrefAlignment());
	lua_setfield(L, -2, "pointerPrefAlignment");
	lua_pushinteger(L, td->getPointerSize());
	lua_setfield(L, -2, "pointerSize");
	lua_pushinteger(L, td->getPointerSizeInBits());
	lua_setfield(L, -2, "pointerSizeInBits");
	// int type of pointer
	Glue<Type>::push(L, (Type *)td->getIntPtrType(getGlobalContext()));
	lua_setfield(L, -2, "intPtrType");

	return 1;
}

static int ee_offsetOf(lua_State * L) {
	ExecutionEngine * ee = getEE();
	const TargetData * td = ee->getTargetData();
	const Type * t = Glue<Type>::checkto(L, 1);
	unsigned nindices = lua_gettop(L) - 1;
	Value * indices[nindices];
	for (int i=0; i<nindices; i++) {
		indices[i] = Glue<Value>::checkto(L, i+2);
	}
	lua_pushinteger(L, td->getIndexedOffset(t, indices, nindices));
	return 1;
}

static int ee_call(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	if (getEE() == 0)
		return luaL_error(L, "no execution engine");
	Function * f;
	const char * name;
	if (lua_isstring(L, 2)) {
		name = lua_tostring(L, 2);
		f = m->getFunction(name);
	} else {
		f = Glue<Function>::checkto(L, 2);
		name = f->getNameStr().c_str();
	}
	if (f == 0)
		return luaL_error(L, "function %s not found", name);

	const FunctionType * ft = f->getFunctionType();

	// get args:
	std::vector<GenericValue> gvs(ft->getNumParams());
	for (int i=0; i<ft->getNumParams(); i++) {
		Lua2GV(L, i+3, gvs[i], ft->getParamType(i));
	}
	GenericValue res = EE->runFunction(f, gvs);
	return GV2Lua(L, res, ft->getReturnType());
}


//template <typename A1, void (*F)(A1)>
//void call1(A1 a1) {
//	F(a1);
//}
//
//int callLua(lua_State * L) {
//	ExecutionEngine * ee = getEE();
//	Module * m = getLModule(L)->getModule();
//
//	Function * F;
//	const char * name;
//	if (lua_isstring(L, 1)) {
//		name = lua_tostring(L, 1);
//		F = m->getFunction(name);
//		if (F == 0)
//			return luaL_error(L, "Function %s not found\n", name);
//	} else {
//		F = Glue<Function>::checkto(L, 1);
//		if (F == 0)
//			return luaL_error(L, "Not a function name or Function\n");
//		name = F->getNameStr().c_str();
//	}
////	if (F->getFunctionType() != luaFunctionTy) {
////		llvm_print(L, F->getFunctionType());
////		return luaL_error(L, "Function is not a lua_CFunction");
////	}
//	// get rid of the function to align the stack:
//	lua_remove(L, 1);
//	// here we can test to ensure that it has the right function type:
//	lua_CFunction lf = (lua_CFunction)ee->getPointerToFunction(F);
//	return lf(L);
//}
//
//int call(lua_State * L) {
//	Function * f = Glue<Function>::checkto(L, 1);
//	ExecutionEngine * ee = getEE();
//	if (ee == 0)
//		return luaL_error(L, "Execution Engine not found");
//	void * fptr = LModule::getEE()->getPointerToFunction(f);
//	if (fptr == 0)
//		return luaL_error(L, "Function pointer not found");
//	// now need to get types & check/cast args accordingly:
//	const FunctionType * ft = f->getFunctionType();
//	int nparams = ft->getNumParams();
//	if (lua_gettop(L)-1 != nparams)
//		return luaL_error(L, "Expected %d params, received %d", nparams, lua_gettop(L)-1);
//	return 0;
//}

static int ee_pushluafunction(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	Function * F;
	const char * name;
	if (lua_isstring(L, 2)) {
		name = lua_tostring(L, 2);
		F = m->getFunction(name);
		if (F == 0)
			return luaL_error(L, "Function %s not found\n", name);
	} else {
		F = Glue<Function>::checkto(L, 2);
		if (F == 0)
			return luaL_error(L, "Not a function name or Function\n");
		name = F->getNameStr().c_str();
	}
	// TODO verify functiontype
	lua_pushcfunction(L, (lua_CFunction)(getEE()->getPointerToFunction(F)));
	return 1;
}

template<> const char * Glue<ExecutionEngine>::usr_name() { return "ExecutionEngine"; }
template<> void Glue<ExecutionEngine>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, ee_call);	lua_setfield(L, -2, "call");
	lua_pushcfunction(L, ee_getTargetData);	lua_setfield(L, -2, "getTargetData");
	lua_pushcfunction(L, ee_offsetOf);	lua_setfield(L, -2, "offsetOf");
	lua_pushcfunction(L, ee_pushluafunction); lua_setfield(L, -2, "pushluafunction");
	
}

/*
	Type
*/
#pragma mark Type
template<> const char * Glue<Type>::usr_name() { return "Type"; }
template<> int Glue<Type>::usr_tostring(lua_State * L, Type * t) { return llvm_print(L, t); }

// type call with no argument is a constant constructor
// type call with an argument is a cast operator
//static int type_call(lua_State * L) {
//	const Type * t = Glue<Type>::checkto(L, 1);
//	if (!lua_isuserdata(L, 2)) {
//		// constant initializer:
//		switch (t->getTypeID()) {
//			case Type::VoidTyID:
//				return Glue<Constant>::push(L, ConstantPointerNull::get((const PointerType *)Type::getVoidTy(getGlobalContext())));
//			case Type::FloatTyID:
//			case Type::DoubleTyID:
//				return Glue<Constant>::push(L, ConstantFP::get(t, luaL_optnumber(L, 2, 0.)));
//			case Type::IntegerTyID:
//				return Glue<Constant>::push(L, ConstantInt::get(t, luaL_optinteger(L, 2, 0)));
//		}
//		return luaL_error(L, "Type cannot be constructed as a constant");
//	} else {
//		// cast:
//		Value * v = Glue<Value>::checkto(L, 2);
//		const Type * t2 = v->getType();
//		IRBuilder<> * b = getLModule(L)->getBuilder();
//
//		// can't cast void:
//		if (t->getTypeID() == Type::VoidTyID || t2->getTypeID() == Type::VoidTyID)
//			return luaL_error(L, "Cannot cast to/from Void");
//
//		// ptr to ptr
//		if (t->getTypeID() == Type::PointerTyID && t2->getTypeID() == Type::PointerTyID) {
//			return Glue<Value>::push(L, b->CreateBitCast(v, t, "cast"));
//		}
//
//		// int to float:
//		if (t->isInteger() && t2->isFloatingPoint())
//			return Glue<Value>::push(L, b->CreateFPToSI(v, t, "intcast"));
//		// float to int
//		if (t->isFloatingPoint() && t2->isInteger())
//			return Glue<Value>::push(L, b->CreateSIToFP(v, t, "floatcast"));
//
//		// int to int
//		if (t->isInteger() == t2->isInteger()) {
//			const IntegerType * it = (IntegerType *)t;
//			const IntegerType * it2 = (IntegerType *)t2;
//			if (it->getBitWidth() > it2->getBitWidth()) {
//				return Glue<Value>::push(L, b->CreateZExt(v, it, "trunc"));
//			} else if (it->getBitWidth() < it2->getBitWidth()) {
//				return Glue<Value>::push(L, b->CreateTrunc(v, it, "trunc"));
//			} else {
//				return 1; // no cast required
//			}
//		}
//
//		return luaL_error(L, "unrecognized cast");
//	}
//}

int type_id(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	lua_pushinteger(L, t->getTypeID());
	return 1;
}
int type_isinteger(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	lua_pushboolean(L, t->isInteger());
	return 1;
}
int type_isfloatingpoint(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	lua_pushboolean(L, t->isFloatingPoint());
	return 1;
}
int type_isabstract(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	lua_pushboolean(L, t->isAbstract());
	return 1;
}
int type_issized(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	lua_pushboolean(L, t->isSized());
	return 1;
}
int type_isValidReturnType(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L);
	lua_pushboolean(L, FunctionType::isValidReturnType(t));
	return 1;
}
//int type_sizeABI(lua_State * L) {
//	const Type * t = Glue<Type>::checkto(L, 1);
//	ExecutionEngine * ee = LModule::getEE();
//	const TargetData * td = ee->getTargetData();
//	/// getTypeSizeInBits - Return the number of bits necessary to hold the
//	/// specified type.  For example, returns 36 for i36 and 80 for x86_fp80.
//	lua_pushinteger(L, td->getABITypeSize(t));
//	return 1;
//}
int type_sizeinbits(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	const TargetData * td = getEE()->getTargetData();
	/// getTypeSizeInBits - Return the number of bits necessary to hold the
	/// specified type.  For example, returns 36 for i36 and 80 for x86_fp80.
	lua_pushinteger(L, td->getTypeSizeInBits(t));
	return 1;
}
static int type_pointer(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	int addressSpace = luaL_optinteger(L, 2, 0);
	if (t->getTypeID() == Type::VoidTyID) {
		// special case for void *:
		return Glue<PointerType>::push(L, PointerType::get(Type::getInt8Ty(getGlobalContext()), addressSpace));
	}
	return Glue<PointerType>::push(L, PointerType::get(t, addressSpace));
}
static int type_eq(lua_State * L) {
	//lua::dump(L, "eq");
	const Type * a = Glue<Type>::checkto(L, 1);
	const Type * b = Glue<Type>::checkto(L, 2);
	//ddebug("%p %p\n", a, b);
	lua_pushboolean(L, a == b);
	return 1;
}
int type_modname(lua_State * L) {
	Module * M = Glue<Module>::checkto(L, 1);
	const Type * t = Glue<Type>::checkto(L, 2);
	lua_pushstring(L, M->getTypeName(t).c_str());
	return 1;
}
template<> void Glue<Type>::usr_mt(lua_State * L) {
	//lua_pushcfunction(L, type_call); lua_setfield(L, -2, "__call");
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, type_pointer); lua_setfield(L, -2, "pointer");
	lua_pushcfunction(L, type_pointer); lua_setfield(L, -2, "ptr");
	lua_pushcfunction(L, type_modname); lua_setfield(L, -2, "name");
	lua_pushcfunction(L, type_isinteger); lua_setfield(L, -2, "isinteger");
	lua_pushcfunction(L, type_isfloatingpoint); lua_setfield(L, -2, "isfloatingpoint");
	lua_pushcfunction(L, type_isabstract); lua_setfield(L, -2, "isabstract");
	lua_pushcfunction(L, type_issized); lua_setfield(L, -2, "issized");
	lua_pushcfunction(L, type_sizeinbits); lua_setfield(L, -2, "sizeinbits");
	lua_pushcfunction(L, type_isValidReturnType); lua_setfield(L, -2, "isValidReturnType");
	//lua_pushcfunction(L, type_sizeABI); lua_setfield(L, -2, "size");
	lua_pushcfunction(L, type_id); lua_setfield(L, -2, "id");
	
	Glue<Type>::push(L, (Type *)Type::getVoidTy(getGlobalContext())); lua_setfield(L, -2, "Void");
	Glue<Type>::push(L, (Type *)Type::getLabelTy(getGlobalContext())); lua_setfield(L, -2, "Label");
	Glue<Type>::push(L, (Type *)Type::getFloatTy(getGlobalContext())); lua_setfield(L, -2, "Float");
	Glue<Type>::push(L, (Type *)Type::getDoubleTy(getGlobalContext())); lua_setfield(L, -2, "Double");
	Glue<Type>::push(L, (Type *)Type::getInt1Ty(getGlobalContext())); lua_setfield(L, -2, "Int1");
	Glue<Type>::push(L, (Type *)Type::getInt8Ty(getGlobalContext())); lua_setfield(L, -2, "Int8");
	Glue<Type>::push(L, (Type *)Type::getInt16Ty(getGlobalContext())); lua_setfield(L, -2, "Int16");
	Glue<Type>::push(L, (Type *)Type::getInt32Ty(getGlobalContext())); lua_setfield(L, -2, "Int32");
	Glue<Type>::push(L, (Type *)Type::getInt64Ty(getGlobalContext())); lua_setfield(L, -2, "Int64");
}

/*
	StructType : CompositeType : DerivedType : Type
*/
#pragma mark StructType
template<> const char * Glue<StructType>::usr_name() { return "StructType"; }
template<> const char * Glue<StructType>::usr_supername() { return "Type"; }
template<> int Glue<StructType>::usr_tostring(lua_State * L, StructType * t) { return llvm_print<StructType>(L, t); }
template<> StructType * Glue<StructType>::usr_new(lua_State * L) {
	std::vector<const Type *> types;
	if (lua_type(L, 1) == LUA_TTABLE) {
		int ntypes = lua_objlen(L, 1);
		for (int i=1; i<= ntypes; i++) {
			lua_rawgeti(L, 1, i);
			types.push_back(Glue<Type>::checkto(L, -1));
			lua_pop(L, 1);
		}
	}
	bool isPacked = false; // this is true for unions, I think??
	return StructType::get((getGlobalContext()), types, isPacked);
}
static int structtype_len(lua_State * L) {
	StructType * u = Glue<StructType>::checkto(L, 1);
	lua_pushinteger(L, u->getNumElements());
	return 1;
}
static int structtype_getelementtype(lua_State * L) {
	StructType * u = Glue<StructType>::checkto(L, 1);
	int i = luaL_checkinteger(L, 2);
	if (i >= u->getNumElements())
		return luaL_error(L, "StructType has only %d elements (requested %d)", u->getNumElements(), i);
	return Glue<Type>::push(L, (Type *)u->getElementType(i));
}
static int structtype_gettypes(lua_State * L) {
	StructType * u = Glue<StructType>::checkto(L, 1);
	for (int i=0; i< u->getNumElements(); i++) {
		Glue<Type>::push(L, (Type *)u->getElementType(i));
	}
	return u->getNumElements();
}
template<> void Glue<StructType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, structtype_len); lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, structtype_getelementtype); lua_setfield(L, -2, "type");
	lua_pushcfunction(L, structtype_gettypes); lua_setfield(L, -2, "types");
}

/*
	SequentialType : CompositeType : DerivedType : Type
*/
#pragma mark SequentialType
template<> const char * Glue<SequentialType>::usr_name() { return "SequentialType"; }
template<> const char * Glue<SequentialType>::usr_supername() { return "Type"; }
template<> int Glue<SequentialType>::usr_tostring(lua_State * L, SequentialType * t) { return llvm_print<SequentialType>(L, t); }
static int sequentialtype_getelementtype(lua_State * L) {
	SequentialType * u = Glue<SequentialType>::checkto(L, 1);
	return Glue<Type>::push(L, (Type *)u->getElementType());
}
template<> void Glue<SequentialType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, sequentialtype_getelementtype); lua_setfield(L, -2, "type");
}

/*
	PointerType : SequentialType
*/
#pragma mark PointerType
template<> const char * Glue<PointerType>::usr_name() { return "PointerType"; }
template<> const char * Glue<PointerType>::usr_supername() { return "SequentialType"; }
template<> int Glue<PointerType>::usr_tostring(lua_State * L, PointerType * t) { return llvm_print<PointerType>(L, t); }
template<> PointerType * Glue<PointerType>::usr_new(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	int addressSpace = luaL_optinteger(L, 2, 0);
	return PointerType::get(t, addressSpace);
}
template<> void Glue<PointerType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
}

/*
	ArrayType : SequentialType
*/
#pragma mark ArrayType
template<> const char * Glue<ArrayType>::usr_name() { return "ArrayType"; }
template<> const char * Glue<ArrayType>::usr_supername() { return "SequentialType"; }
template<> int Glue<ArrayType>::usr_tostring(lua_State * L, ArrayType * t) { return llvm_print<ArrayType>(L, t); }
template<> ArrayType * Glue<ArrayType>::usr_new(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	int len = luaL_optinteger(L, 2, 1);
	return ArrayType::get(t, len);
}
static int arraytype_len(lua_State * L) {
	ArrayType * u = Glue<ArrayType>::checkto(L, 1);
	lua_pushinteger(L, u->getNumElements());
	return 1;
}
template<> void Glue<ArrayType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, arraytype_len); lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
}

/*
	VectorType : SequentialType
*/
#pragma mark VectorType
template<> const char * Glue<VectorType>::usr_name() { return "VectorType"; }
template<> const char * Glue<VectorType>::usr_supername() { return "SequentialType"; }
template<> int Glue<VectorType>::usr_tostring(lua_State * L, VectorType * t) { return llvm_print<VectorType>(L, t); }
template<> VectorType * Glue<VectorType>::usr_new(lua_State * L) {
	const Type * t = Glue<Type>::checkto(L, 1);
	int len = luaL_optinteger(L, 2, 1);
	return VectorType::get(t, len);
}
static int vectortype_len(lua_State * L) {
	VectorType * u = Glue<VectorType>::checkto(L, 1);
	lua_pushinteger(L, u->getNumElements());
	return 1;
}
template<> void Glue<VectorType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, vectortype_len); lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
}

/*
	OpaqueType : DerivedType : Type
*/
#pragma mark OpaqueType
template<> const char * Glue<OpaqueType>::usr_name() { return "OpaqueType"; }
template<> const char * Glue<OpaqueType>::usr_supername() { return "Type"; }
template<> int Glue<OpaqueType>::usr_tostring(lua_State * L, OpaqueType * t) { return llvm_print<OpaqueType>(L, t); }
template<> OpaqueType * Glue<OpaqueType>::usr_new(lua_State * L) {
	return OpaqueType::get(getGlobalContext());
}
template<> void Glue<OpaqueType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
}

/*
	FunctionType : DerivedType : Type
*/
#pragma mark FunctionType
template<> const char * Glue<FunctionType>::usr_name() { return "FunctionType"; }
template<> const char * Glue<FunctionType>::usr_supername() { return "Type"; }
template<> int Glue<FunctionType>::usr_tostring(lua_State * L, FunctionType * t) { return llvm_print<FunctionType>(L, t); }
template<> FunctionType * Glue<FunctionType>::usr_new(lua_State * L) {
	const Type * ret = Glue<Type>::checkto(L, 1);
	std::vector<const Type *> types;
	int ntypes = lua_gettop(L) - 1;
	for (int i=0; i<ntypes; i++)
		types.push_back(Glue<Type>::checkto(L, i+2));
	bool isVarArg = false; // this is true for unions, I think??
	return FunctionType::get(ret, types, isVarArg);
}
static int functiontype_isvararg(lua_State * L) {
	FunctionType * f = Glue<FunctionType>::checkto(L, 1);
	lua_pushinteger(L, f->isVarArg());
	return 1;
}
static int functiontype_getparamtype(lua_State * L) {
	FunctionType * u = Glue<FunctionType>::checkto(L, 1);
	int i = luaL_checkinteger(L, 2);
	if (i >= u->getNumParams())
		return luaL_error(L, "FunctionType has only %d params (requested %d)", u->getNumParams(), i);
	return Glue<Type>::push(L, (Type *)u->getParamType(i));
}
static int functiontype_getreturntype(lua_State * L) {
	FunctionType * u = Glue<FunctionType>::checkto(L, 1);
	return Glue<Type>::push(L, (Type *)u->getReturnType());
}
static int functiontype_len(lua_State * L) {
	FunctionType * u = Glue<FunctionType>::checkto(L, 1);
	lua_pushinteger(L, u->getNumParams());
	return 1;
}
template<> void Glue<FunctionType>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, type_eq); lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, functiontype_len); lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, functiontype_isvararg); lua_setfield(L, -2, "isvararg");
	lua_pushcfunction(L, functiontype_getparamtype); lua_setfield(L, -2, "param");
	lua_pushcfunction(L, functiontype_getreturntype); lua_setfield(L, -2, "ret");
	lua_pushcfunction(L, Glue<Function>::create); lua_setfield(L, -2, "__call");
}

/*
	Value
*/
#pragma mark Value
template<> const char * Glue<Value>::usr_name() { return "Value"; }
template<> int Glue<Value>::usr_tostring(lua_State * L, Value * u) { return llvm_print<Value>(L, u); }
template<> Value * Glue<Value>::usr_reinterpret(lua_State * L, int idx) {
	if (lua_isnoneornil(L, idx))
		return 0;
	double n;
	switch (lua_type(L, idx)) {
		case LUA_TBOOLEAN:
			// equiv: const bool;
			return ConstantInt::get((getGlobalContext()), APInt(1, lua_toboolean(L, idx)));
		case LUA_TNUMBER:
			n = lua_tonumber(L, idx);
			if (fmod(n, 1.0) == 0.)
				// equiv: const sint32_t;
				return ConstantInt::get((getGlobalContext()), APInt(32, lua_tonumber(L, idx)));
			else	
				// equiv: const double;
				return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), lua_tonumber(L, idx));
		case LUA_TSTRING:
			// equiv: const char * (null terminated)
			return ConstantArray::get((getGlobalContext()), std::string(lua_tostring(L, idx)), true); // true == null terminate
		case LUA_TUSERDATA:	{	 
			// makes sense only if it is a Value:
			Value * u = Glue<Value>::checkto(L, idx);
			return u;
		}
		case LUA_TLIGHTUSERDATA: // pointers can't be typeless constants, so exchange for null
		case LUA_TNIL:
		default:				// thread, function & table make no sense for an LLVM Value *
			luaL_error(L, "cannot interpret value of type %s\n", lua_typename(L, lua_type(L, idx)));
			break;
	}
	return 0;
}
static int value_type(lua_State * L) {
	Value * u = Glue<Value>::checkto(L);
	return Glue<Type>::push(L, (Type *)u->getType());
}
static int value_name(lua_State * L) {
	Value * u = Glue<Value>::checkto(L);
	if (lua_isstring(L, 2)) {
		const char * name = lua_tostring(L, 2);
		u->setName(name);
		lua_pushvalue(L, 1);
		return 1;
	}
	lua_pushstring(L, u->getNameStr().c_str());
	return 1;
}
static int value_replace(lua_State * L) {
	Value * u = Glue<Value>::checkto(L, 1);
	Value * v = Glue<Value>::checkto(L, 2);
	u->replaceAllUsesWith(v);
	return 0;
}
static int value_uses(lua_State * L) {
	Value * u = Glue<Value>::checkto(L, 1);
	lua_pushinteger(L, u->getNumUses());
	return 1;
}
template<> void Glue<Value>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, value_type); lua_setfield(L, -2, "type");
	lua_pushcfunction(L, value_name); lua_setfield(L, -2, "name");
	lua_pushcfunction(L, value_replace); lua_setfield(L, -2, "replace");
	lua_pushcfunction(L, value_uses); lua_setfield(L, -2, "uses");
}

/*
	Argument : Value
*/
#pragma mark Argument
template<> const char * Glue<Argument>::usr_name() { return "Argument"; }
template<> const char * Glue<Argument>::usr_supername() { return "Value"; }
template<> int Glue<Argument>::usr_tostring(lua_State * L, Argument * u) { return llvm_print<Argument>(L, u); }
static int argument_parent(lua_State * L) {
	Argument * u = Glue<Argument>::checkto(L, 1);
	return Glue<Function>::push(L, u->getParent());
}
static int argument_argno(lua_State * L) {
	Argument * u = Glue<Argument>::checkto(L, 1);
	lua_pushinteger(L, u->getArgNo());
	return 1;
}
static int argument_byval(lua_State * L) {
	Argument * u = Glue<Argument>::checkto(L, 1);
	lua_pushboolean(L, u->hasByValAttr());
	return 1;
}
template<> void Glue<Argument>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, argument_parent); lua_setfield(L, -2, "parent");
	lua_pushcfunction(L, argument_argno); lua_setfield(L, -2, "argno");
	lua_pushcfunction(L, argument_byval); lua_setfield(L, -2, "byval");
}

/*
	Instruction : User : Value
*/
#pragma mark Instruction
template<> const char * Glue<Instruction>::usr_name() { return "Instruction"; }
template<> const char * Glue<Instruction>::usr_supername() { return "Value"; }
template<> int Glue<Instruction>::usr_tostring(lua_State * L, Instruction * u) { return llvm_print<Instruction>(L, u); }
static int inst_parent(lua_State * L) {
	Instruction * u = Glue<Instruction>::checkto(L, 1);
	return Glue<BasicBlock>::push(L, u->getParent());
}
static int inst_erase(lua_State * L) {
	Instruction * f = Glue<Instruction>::checkto(L, 1);
	f->eraseFromParent();
	Glue<Instruction>::erase(L, 1);
	return 0;
}
static int inst_opcode(lua_State * L) {
	Instruction * u = Glue<Instruction>::checkto(L, 1);
	lua_pushstring(L, u->getOpcodeName());
	return 1;
}
template<> void Glue<Instruction>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, inst_parent); lua_setfield(L, -2, "parent");
	lua_pushcfunction(L, inst_erase); lua_setfield(L, -2, "erase");
	lua_pushcfunction(L, inst_opcode); lua_setfield(L, -2, "opcode");
}

/*
	PHINode : Instruction
*/
#pragma mark PHINode
template<> const char * Glue<PHINode>::usr_name() { return "PHINode"; }
template<> const char * Glue<PHINode>::usr_supername() { return "Instruction"; }
template<> int Glue<PHINode>::usr_tostring(lua_State * L, PHINode * u) { return llvm_print<PHINode>(L, u); }
static int phi_addincoming(lua_State * L) {
	PHINode * u = Glue<PHINode>::checkto(L, 1);
	Value * v = Glue<Value>::checkto(L, 2);
	BasicBlock * block = Glue<BasicBlock>::checkto(L, 3);
	u->addIncoming(v, block);
	return 0;
}
template<> void Glue<PHINode>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, phi_addincoming); lua_setfield(L, -2, "addincoming");
}


/*
	BasicBlock : Value
*/
#pragma mark BasicBlock
template<> const char * Glue<BasicBlock>::usr_name() { return "BasicBlock"; }
template<> const char * Glue<BasicBlock>::usr_supername() { return "Value"; }
template<> int Glue<BasicBlock>::usr_tostring(lua_State * L, BasicBlock * u) { return llvm_print<BasicBlock>(L, u); }
template<> BasicBlock * Glue<BasicBlock>::usr_new(lua_State * L) {
	const char * name = luaL_optstring(L, 1, "noname");
	Function * parent = Glue<Function>::to(L, 2);
	BasicBlock * insertbefore = Glue<BasicBlock>::to(L, 3);
	return BasicBlock::Create((getGlobalContext()), name, parent, insertbefore);
}
static int block_parent(lua_State * L) {
	BasicBlock * u = Glue<BasicBlock>::checkto(L, 1);
	return Glue<Function>::push(L, u->getParent());
}
static int block_erase(lua_State * L) {
	BasicBlock * f = Glue<BasicBlock>::checkto(L, 1);
	f->eraseFromParent();
	Glue<BasicBlock>::erase(L, 1);
	return 0;
}
static int block_terminator(lua_State * L) {
	BasicBlock * f = Glue<BasicBlock>::checkto(L, 1);
	return Glue<Instruction>::push(L, f->getTerminator());
}
static int block_instruction(lua_State * L) {
	BasicBlock * f = Glue<BasicBlock>::checkto(L, 1);
	int i = luaL_checkinteger(L, 2);
	if (i >= f->size())
		return luaL_error(L, "Function has only %d arguments (requested %d)", f->size(), i);
	BasicBlock::iterator it = f->begin();
	while (i--) it++;
	return Glue<Instruction>::push(L, it);
}
static int block_front(lua_State * L) {
	BasicBlock * f = Glue<BasicBlock>::checkto(L, 1);
	return Glue<Instruction>::push(L, &f->front());
}
static int block_back(lua_State * L) {
	BasicBlock * f = Glue<BasicBlock>::checkto(L, 1);
	return Glue<Instruction>::push(L, &f->back());
}
static int block_len(lua_State * L) {
	BasicBlock * f = Glue<BasicBlock>::checkto(L, 1);
	lua_pushinteger(L, f->size());
	return 1;
}
//static int block_setinsertpoint(lua_State * L) {
//	BasicBlock * block = Glue<BasicBlock>::checkto(L, 1);
//	getLModule(L)->getBuilder()->SetInsertPoint(block);
//	return 0;
//}
//static int block_ret(lua_State * L) {
//	IRBuilder<> * b = getLModule(L)->getBuilder();
//	BasicBlock * block = Glue<BasicBlock>::checkto(L, 1);
//	Value * v = Glue<Value>::to(L, 2);
//	Function * f = block->getParent();
//	if (f) {
//		// check types:
//		const Type * retType = f->getFunctionType()->getReturnType();
//		if (retType->getTypeID() == Type::VoidTyID) {
//			if (v)
//				return luaL_error(L, "current function returns void");
//			return Glue<Instruction>::push(L, b->CreateRetVoid());
//		}
//		if (retType != v->getType())
//			luaL_error(L, "return type mismatch");
//		return Glue<Instruction>::push(L, b->CreateRet(v));
//	}
//	if (v)
//		return Glue<Instruction>::push(L, b->CreateRet(v));
//	return Glue<Instruction>::push(L, b->CreateRetVoid());
//}
template<> void Glue<BasicBlock>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, block_len); lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, block_instruction); lua_setfield(L, -2, "instruction");
	lua_pushcfunction(L, block_parent); lua_setfield(L, -2, "parent");
	lua_pushcfunction(L, block_front); lua_setfield(L, -2, "front");
	lua_pushcfunction(L, block_back); lua_setfield(L, -2, "back");
	lua_pushcfunction(L, block_erase); lua_setfield(L, -2, "erase");
	lua_pushcfunction(L, block_terminator); lua_setfield(L, -2, "terminator");
//	lua_pushcfunction(L, block_setinsertpoint); lua_setfield(L, -2, "setinsertpoint");
//	lua_pushcfunction(L, block_ret); lua_setfield(L, -2, "ret");
}

/*
	Constant : User : Value
*/
#pragma mark Constant
template<> const char * Glue<Constant>::usr_name() { return "Constant"; }
template<> const char * Glue<Constant>::usr_supername() { return "Value"; }
template<> int Glue<Constant>::usr_tostring(lua_State * L, Constant * u) { return llvm_print<Constant>(L, u); }
template<> Constant * Glue<Constant>::usr_reinterpret(lua_State * L, int idx) {
	return (Constant *)Glue<Value>::usr_reinterpret(L, idx);
}
// destroyConstant... todo: call this on all constants registered so far, when the module closes

/*
	GlobalValue : Constant
*/
#pragma mark GlobalValue
template<> const char * Glue<GlobalValue>::usr_name() { return "GlobalValue"; }
template<> const char * Glue<GlobalValue>::usr_supername() { return "Constant"; }
template<> int Glue<GlobalValue>::usr_tostring(lua_State * L, GlobalValue * u) { return llvm_print<GlobalValue>(L, u); }
static int global_linkage(lua_State * L) {
	GlobalValue * u = Glue<GlobalValue>::checkto(L, 1);
	if (lua_isnumber(L, 2)) {
		u->setLinkage((GlobalValue::LinkageTypes)lua_tointeger(L, 2));
	}
	lua_pushinteger(L, u->getLinkage());
	return 1;
}
static int global_isdeclaration(lua_State * L) {
	GlobalValue * f = Glue<GlobalValue>::checkto(L, 1);
	lua_pushboolean(L, f->isDeclaration());
	return 1;
}
template<> void Glue<GlobalValue>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, global_linkage); lua_setfield(L, -2, "linkage");
	lua_pushcfunction(L, global_isdeclaration); lua_setfield(L, -2, "isdeclaration");
}
// likely methods:	getParent() (returns a Module *)


/*
	GlobalVariable : GlobalValue
*/
#pragma mark GlobalVariable
template<> const char * Glue<GlobalVariable>::usr_name() { return "GlobalVariable"; }
template<> const char * Glue<GlobalVariable>::usr_supername() { return "GlobalValue"; }
template<> int Glue<GlobalVariable>::usr_tostring(lua_State * L, GlobalVariable * u) { return llvm_print<GlobalVariable>(L, u); }
template<> GlobalVariable * Glue<GlobalVariable>::usr_new(lua_State * L) {
	Module * m = Glue<Module>::checkto(L, 1);
	const Type * t = Glue<Type>::checkto(L, 2);
	const char * name  = luaL_checkstring(L, 3);
	bool isConstant = lua_toboolean(L, 4);
	Constant * initializer = 0;
	if (lua_isuserdata(L, 5)) {
		initializer = Glue<Constant>::to(L, 5);
		if (initializer->getType() != t) {
			luaL_error(L, "initializer must match type");
			return 0;
		}
	}
	GlobalValue::LinkageTypes lt = (GlobalValue::LinkageTypes)luaL_optinteger(L, 5, GlobalValue::ExternalLinkage);
	return new GlobalVariable((getGlobalContext()), t, isConstant, lt, initializer, name, m);
}
static int globalvar_erase(lua_State * L) {
	GlobalVariable * u = Glue<GlobalVariable>::checkto(L, 1);
	u->eraseFromParent();
	Glue<GlobalVariable>::erase(L, 1);
	return 0;
}
static int globalvar_isconstant(lua_State * L) {
	GlobalVariable * u = Glue<GlobalVariable>::checkto(L, 1);
	lua_pushboolean(L, u->isConstant());
	return 1;
}
static int globalvar_initializer(lua_State * L) {
	GlobalVariable * u = Glue<GlobalVariable>::checkto(L, 1);
	if (u->hasInitializer())
		return Glue<Constant>::push(L, u->getInitializer());
	return 0;
}
template<> void Glue<GlobalVariable>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, globalvar_erase); lua_setfield(L, -2, "erase");
	lua_pushcfunction(L, globalvar_initializer); lua_setfield(L, -2, "initializer");
	lua_pushcfunction(L, globalvar_isconstant); lua_setfield(L, -2, "isconstant");
}

/*
	Function : GlobalValue
*/
#pragma mark Function
template<> const char * Glue<Function>::usr_name() { return "Function"; }
template<> const char * Glue<Function>::usr_supername() { return "GlobalValue"; }
template<> int Glue<Function>::usr_tostring(lua_State * L, Function * u) { return llvm_print<Function>(L, u); }
template<> Function * Glue<Function>::usr_new(lua_State * L) {
	Module * M = Glue<Module>::checkto(L, 1);
	// if argument is a string, then search for a pre-existing function of that name
	if (lua_isstring(L, 2))
		return M->getFunction(lua_tostring(L, 2));
	// else generate a new function:
	const FunctionType * ft = Glue<FunctionType>::checkto(L, 2);
	std::string name  = luaL_checkstring(L, 3);
	GlobalValue::LinkageTypes lt = (GlobalValue::LinkageTypes)luaL_optinteger(L, 4, GlobalValue::ExternalLinkage);
	bool noAutoName = (bool)lua_isboolean(L, 4) && (bool)lua_toboolean(L, 5); // default false
	Function * F = Function::Create(ft, lt, name, M);
	if (noAutoName && F->getName() != name) {
		F->eraseFromParent();
		luaL_error(L, "Function %s already exists", name.c_str());
		return 0;
	}
	int i=0;
	Function::arg_iterator AI = F->arg_begin();
	for (; i < F->getFunctionType()->getNumParams(); ++AI, ++i) {
		char argname[16];
		sprintf(argname, "arg%i", i);
		AI->setName(argname);
	}
	return F;
}
static int function_intrinsic(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	lua_pushinteger(L, f->getIntrinsicID());
	return 1;
}
static int function_deletebody(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	f->deleteBody();
	return 0;
}
static int function_erase(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	f->eraseFromParent();
	Glue<Function>::erase(L, 1);
	return 0;
}
static int function_callingconv(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	if (lua_isnumber(L, 2)) {
		unsigned cc = lua_tonumber(L, 2);
		f->setCallingConv(cc);
	}
	lua_pushinteger(L, f->getCallingConv());
	return 1;
}
static int function_argument(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	int i = luaL_checkinteger(L, 2);
	if (i >= f->getFunctionType()->getNumParams())
		return luaL_error(L, "Function has only %d arguments (requested %d)", f->getFunctionType()->getNumParams(), i);
	Function::arg_iterator it = f->arg_begin();
	while (i--) it++;
	return Glue<Argument>::push(L, it);
}
static int function_len(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	lua_pushinteger(L, f->size());
	return 1;
}
static int function_block(lua_State * L) {
	Function * f = Glue<Function>::checkto(L, 1);
	int i = luaL_checkinteger(L, 2);
	if (i >= f->size())
		return luaL_error(L, "Function has only %d blocks (requested %d)", f->size(), i);
	Function::iterator it = f->begin();
	while (i--) it++;
	return Glue<BasicBlock>::push(L, it);
}
static int function_verify(lua_State * L) {
	Function * u = Glue<Function>::to(L, 1);
	lua_pushboolean(L, u && (verifyFunction(*u, ReturnStatusAction) == false));
	return 1;
}
// trick here is knowing what the right
static int function_getptr(lua_State * L) {
	Function * u = Glue<Function>::to(L, 1);
	void * f = getEE()->getPointerToFunction(u);
	lua_pushlightuserdata(L, f);
	return 1;
}
//static int function_optimize(lua_State * L) {
//	Function * f = Glue<Function>::checkto(L, 1);
//	lua_pushboolean(L, getLModule(L)->optimize(f));
//	return 1;
//}
static int function_pushcfunction(lua_State * L) {
	Function * u = Glue<Function>::checkto(L, 1);
	void * f = getEE()->getPointerToFunction(u);
	if(f) { // && u->getFunctionType() != luaFunctionTy) {
		lua_pushcfunction(L, (lua_CFunction)f);
		return 1;
	}
	else {
		return 0;
	}
}
template<> void Glue<Function>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, function_len); lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, function_intrinsic); lua_setfield(L, -2, "intrinsic");
	lua_pushcfunction(L, function_deletebody); lua_setfield(L, -2, "deletebody");
	lua_pushcfunction(L, function_erase); lua_setfield(L, -2, "erase");
	lua_pushcfunction(L, function_callingconv); lua_setfield(L, -2, "callingconv");
	lua_pushcfunction(L, function_argument); lua_setfield(L, -2, "argument");
	lua_pushcfunction(L, function_block); lua_setfield(L, -2, "block");
	lua_pushcfunction(L, function_verify); lua_setfield(L, -2, "verify");
	lua_pushcfunction(L, function_getptr); lua_setfield(L, -2, "getptr");
	//lua_pushcfunction(L, function_optimize); lua_setfield(L, -2, "optimize");
	lua_pushcfunction(L, function_pushcfunction); lua_setfield(L, -2, "pushcfunction");
}

/*
	IRBuilder
*/
#pragma mark IRBuilder
template<> const char * Glue<IRBuilder<> >::usr_name() { return "IRBuilder"; }
template<> IRBuilder<> * Glue<IRBuilder<> >::usr_new(lua_State * L) {
	return new IRBuilder<>(getGlobalContext());
}

static int createPHI(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Type * ty = Glue<Type>::checkto(L, 2);
	const char * name = luaL_optstring(L, 3, "phi");
	return Glue<PHINode>::push(L, b->CreatePHI(ty, name));
}
static int createBr(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	BasicBlock * branch = Glue<BasicBlock>::checkto(L, 2);
	return Glue<Instruction>::push(L, b->CreateBr(branch));
}
static int createCondBr(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * condition = Glue<Value>::checkto(L, 2);
	if (condition->getType() != Type::getInt1Ty(getGlobalContext()))
		return luaL_error(L, "condition must be of Int1 type");
	BasicBlock * iftrue = Glue<BasicBlock>::checkto(L, 3);
	BasicBlock * iffalse = Glue<BasicBlock>::checkto(L, 4);
	return Glue<Instruction>::push(L, b->CreateCondBr(condition, iftrue, iffalse));
}
static int createRet(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	BasicBlock * block = b->GetInsertBlock();
	if (block == 0)
		return luaL_error(L, "not currently building a block");
	Value * v = Glue<Value>::to(L, 2);
	Function * f = block->getParent();
	if (f) {
		// check types:
		const Type * retType = f->getFunctionType()->getReturnType();
		if (retType->getTypeID() == Type::VoidTyID) {
			return Glue<Instruction>::push(L, b->CreateRetVoid());
		}
		if (v == NULL)
			return luaL_error(L, "function requires a return value");
		if (retType->getTypeID() != v->getType()->getTypeID()) {
			//ddebug("%i %i\n", retType->getTypeID(), v->getType()->getTypeID());
			luaL_error(L, "return type mismatch");
		}
		return Glue<Instruction>::push(L, b->CreateRet(v));
	}
	if (v)
		return Glue<Instruction>::push(L, b->CreateRet(v));
	return Glue<Instruction>::push(L, b->CreateRetVoid());
}
static int createAdd(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	return Glue<Value>::push(L, b->CreateAdd(x, y, "add"));
}
static int createSub(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	return Glue<Value>::push(L, b->CreateSub(x, y, "sub"));
}
static int createMul(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	return Glue<Value>::push(L, b->CreateMul(x, y, "mul"));
}
static int createDiv(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		// todo check for unsignedness
		return Glue<Value>::push(L, b->CreateSDiv(x, y, "sdiv"));
	}
	return Glue<Value>::push(L, b->CreateFDiv(x, y, "fdiv"));
}
static int createRem(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		// todo check for unsignedness
		return Glue<Value>::push(L, b->CreateSRem(x, y, "srem"));
	}
	return Glue<Value>::push(L, b->CreateFRem(x, y, "frem"));
}
static int createNeg(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	return Glue<Value>::push(L, b->CreateNeg(Glue<Value>::checkto(L, 2), "neg"));
}
static int createNot(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	return Glue<Value>::push(L, b->CreateNot(Glue<Value>::checkto(L, 2), "not"));
}
static int createCmpEQ(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	// for int: option indicates unsigned; for float: fail if either is nan;
	bool option = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		return Glue<Value>::push(L, b->CreateICmpEQ(x, y, "icmp_eq"));
	}
	if (!option)
		return Glue<Value>::push(L, b->CreateFCmpOEQ(x, y, "fcmp_oeq"));
	return Glue<Value>::push(L, b->CreateFCmpUEQ(x, y, "fcmp_ueq"));
}
static int createCmpNE(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	// for int: option indicates unsigned; for float: fail if either is nan;
	bool option = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		return Glue<Value>::push(L, b->CreateICmpNE(x, y, "icmp_ne"));
	}
	if (!option)
		return Glue<Value>::push(L, b->CreateFCmpONE(x, y, "fcmp_one"));
	return Glue<Value>::push(L, b->CreateFCmpUNE(x, y, "fcmp_une"));
}
static int createCmpGT(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	// for int: option indicates unsigned; for float: fail if either is nan;
	bool option = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		if (option)
			return Glue<Value>::push(L, b->CreateICmpUGT(x, y, "icmp_ugt"));
		return Glue<Value>::push(L, b->CreateICmpSGT(x, y, "icmp_sgt"));
	}
	if (!option)
		return Glue<Value>::push(L, b->CreateFCmpOGT(x, y, "fcmp_ogt"));
	return Glue<Value>::push(L, b->CreateFCmpUGT(x, y, "fcmp_ugt"));
}
static int createCmpLT(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	// for int: option indicates unsigned; for float: fail if either is nan;
	bool option = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		if (option)
			return Glue<Value>::push(L, b->CreateICmpULT(x, y, "icmp_ult"));
		return Glue<Value>::push(L, b->CreateICmpSLT(x, y, "icmp_slt"));
	}
	if (!option)
		return Glue<Value>::push(L, b->CreateFCmpOLT(x, y, "fcmp_olt"));
	return Glue<Value>::push(L, b->CreateFCmpULT(x, y, "fcmp_ult"));
}
static int createCmpGE(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	// for int: option indicates unsigned; for float: fail if either is nan;
	bool option = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		if (option)
			return Glue<Value>::push(L, b->CreateICmpUGE(x, y, "icmp_uge"));
		return Glue<Value>::push(L, b->CreateICmpSGE(x, y, "icmp_sge"));
	}
	if (!option)
		return Glue<Value>::push(L, b->CreateFCmpOGE(x, y, "fcmp_oge"));
	return Glue<Value>::push(L, b->CreateFCmpUGE(x, y, "fcmp_uge"));
}
static int createCmpLE(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	Value * y = Glue<Value>::checkto(L, 3);
	// for int: option indicates unsigned; for float: fail if either is nan;
	bool option = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	bool ix = x->getType()->isInteger();
	bool iy = y->getType()->isInteger();
	if (ix && iy) {
		if (option)
			return Glue<Value>::push(L, b->CreateICmpULE(x, y, "icmp_ule"));
		return Glue<Value>::push(L, b->CreateICmpSLE(x, y, "icmp_sle"));
	}
	if (!option)
		return Glue<Value>::push(L, b->CreateFCmpOLE(x, y, "fcmp_ole"));
	return Glue<Value>::push(L, b->CreateFCmpULE(x, y, "fcmp_ule"));
}

static int createFToI(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	// option indicates unsigned
	bool option = lua_isboolean(L, 3) && lua_toboolean(L, 3);
	if (!x->getType()->isFloatingPoint())
		luaL_error(L, "attempt to cast from non-floating point type");
	if (!option)
		return Glue<Value>::push(L, b->CreateFPToSI(x, Type::getInt32Ty(getGlobalContext()), "fptosi"));
	return Glue<Value>::push(L, b->CreateFPToUI(x, Type::getInt32Ty(getGlobalContext()), "fptoui"));
}
static int createIToF(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * x = Glue<Value>::checkto(L, 2);
	// option indicates unsigned
	bool option = lua_isboolean(L, 3) && lua_toboolean(L, 3);
	if (!x->getType()->isInteger())
		luaL_error(L, "attempt to cast from non-integer type");
	if (!option)
		return Glue<Value>::push(L, b->CreateSIToFP(x, Type::getDoubleTy(getGlobalContext()), "sitofp"));
	return Glue<Value>::push(L, b->CreateUIToFP(x, Type::getDoubleTy(getGlobalContext()), "uitofp"));
}

static int createCall(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Module * m = Glue<Module>::checkto(L, 2);	
	// get args:
	std::vector<Value *> args;
	for (int i=4; i<=lua_gettop(L); i++) {
		args.push_back(Glue<Value>::checkto(L, i));
	}
	// function may be by Function or by name:
	if (lua_isstring(L, 3)) {
		const char * fname = lua_tostring(L, 3);
		Function * f = m->getFunction(fname);
		if (f == 0)
			return luaL_error(L, "function %s not found", lua_tostring(L, 3));
		if (args.size() < f->getFunctionType()->getNumParams())
			return luaL_error(L, "insufficient arguments");

		// ok it's a function. what return type?
		if (f->getReturnType() == Type::getVoidTy(getGlobalContext())) {
			// functions returning void shouldn't be named:
			b->CreateCall(f, args.begin(), args.end());
			return 0;
		}
		return Glue<Value>::push(L, b->CreateCall(f, args.begin(), args.end(), f->getNameStr().c_str()));
	}
	Function * f = Glue<Function>::checkto(L, 3);
	FunctionType * ft = (FunctionType *)f->getFunctionType();
	if (args.size() < ft->getNumParams())
		return luaL_error(L, "insufficient arguments");
	if (f->getReturnType() == Type::getVoidTy(getGlobalContext())) {
		// functions returning void shouldn't be named:
		b->CreateCall(f, args.begin(), args.end());
		return 0;
	}
	return Glue<Value>::push(L, b->CreateCall(f, args.begin(), args.end(), "call"));
}
static int createExtractValue(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * agg = Glue<Value>::checkto(L, 2);
	if (!agg->getType()->isAggregateType())
		return luaL_error(L, "not an aggregate type");
	unsigned idx = luaL_optinteger(L, 3, 0);
	// todo: verify that agg.indices > idx
	return Glue<Value>::push(L, b->CreateExtractValue(agg, idx, "extract"));
}
static int createInsertValue(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * agg = Glue<Value>::checkto(L, 2);
	if (!agg->getType()->isAggregateType())
		return luaL_error(L, "not an aggregate type");
	unsigned idx = luaL_optinteger(L, 3, 0);
	// todo: verify that agg.indices > idx
	Value * val = Glue<Value>::checkto(L, 4);
	// verify that types match
	return Glue<Value>::push(L, b->CreateInsertValue(agg, val, idx, "insert"));
}

Value * toInteger(lua_State * L, int idx = 1, int def = 0) {
	if (lua_isnumber(L, idx))
		return ConstantInt::get((getGlobalContext()), APInt(32, lua_tointeger(L, idx)));
	Value * res = Glue<Value>::to(L, idx);
	if (res == 0)
		return ConstantInt::get((getGlobalContext()), APInt(32, def));
	return res;
}

static int createMalloc(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	const Type * t = Glue<Type>::checkto(L, 2);
	if (!t->isSized())
		return luaL_error(L, "type is not sized");
	Value * sz = toInteger(L, 3, 1);
	if (!sz->getType()->isInteger())
		return luaL_error(L, "size is not integer");
	return Glue<Instruction>::push(L, b->CreateMalloc(t, sz, "mem"));
}
static int createAlloca(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	// todo: get the current function, and insert this instruction in its entry block
	// (necessary for mem2reg optimizations to work)
	/*
	IRBuilder TmpB(&TheFunction->getEntryBlock(),
                 TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::DoubleTy, 0, VarName.c_str());
	*/
	const Type * t = Glue<Type>::checkto(L, 2);
	if (!t->isSized())
		return luaL_error(L, "type is not sized");
	Value * sz = toInteger(L, 3, 1);
	if (!sz->getType()->isInteger())
		return luaL_error(L, "size is not integer");
	return Glue<Instruction>::push(L, b->CreateAlloca(t, sz, "mem"));
}
static int createFree(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * p = Glue<Value>::checkto(L, 2);
	if (p->getType()->getTypeID() != Type::PointerTyID)
		return luaL_error(L, "not a pointer type");
	return Glue<Instruction>::push(L, b->CreateFree(p));
}
static int createLoad(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * p = Glue<Value>::checkto(L, 2);
	if (p->getType()->getTypeID() != Type::PointerTyID)
		return luaL_error(L, "not a pointer type");
	bool isVolatile = lua_isboolean(L, 3) && lua_toboolean(L, 3);
	return Glue<Instruction>::push(L, b->CreateLoad(p, isVolatile, "load"));
}
static int createStore(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * p = Glue<Value>::checkto(L, 2);
	Value * v = Glue<Value>::checkto(L, 3);
	if (p->getType()->getTypeID() != Type::PointerTyID)
		return luaL_error(L, "not a pointer type");
	if (p->getType()->getTypeID() != PointerType::get(v->getType(), 0)->getTypeID())
		return luaL_error(L, "mismatch type");
	bool isVolatile = lua_isboolean(L, 4) && lua_toboolean(L, 4);
	return Glue<Instruction>::push(L, b->CreateStore(v, p, isVolatile));
}
static int createGEP(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * p = Glue<Value>::checkto(L, 2);
	if (p->getType()->getTypeID() != Type::PointerTyID)
		return luaL_error(L, "not a pointer type");
	std::vector<Value *> indices;
	for (int i=3; i<=lua_gettop(L); i++) {
		// todo: it's easy to index out of range here, and this causes a crash
		// but how to avoid this is hard...
		// if value is a constant, then we can try to calculate, but too much is runtime dependent
		Value * idx = toInteger(L, i, 0);
		if (!idx->getType()->isInteger())
			return luaL_error(L, "indices must be integers");
		indices.push_back(idx);
	}
	return Glue<Value>::push(L, b->CreateGEP(p, indices.begin(), indices.end(), "ptr"));
}
static int createBitCast(lua_State * L) {
	IRBuilder<> * b = Glue<IRBuilder<> >::checkto(L, 1);
	Value * v = Glue<Value>::checkto(L, 2);
	const Type * vty = v->getType();
	if (vty->isAggregateType())
		return luaL_error(L, "cannot cast aggregate type");
	const Type * ty = Glue<Type>::checkto(L, 3);
	if (ty->isAggregateType())
		return luaL_error(L, "cannot cast to aggregate type");
	if (vty->getTypeID() == Type::PointerTyID && ty->getTypeID() != Type::PointerTyID)
		return luaL_error(L, "pointers can only be cast to pointer types");
	return Glue<Value>::push(L, b->CreateBitCast(v, ty, "cast"));
}
template<> void Glue<IRBuilder<> >::usr_mt(lua_State * L) {
	lua_pushcfunction(L, createPHI); lua_setfield(L, -2, "PHI");
	lua_pushcfunction(L, createBr); lua_setfield(L, -2, "Br");
	lua_pushcfunction(L, createCondBr); lua_setfield(L, -2, "CondBr");
	lua_pushcfunction(L, createRet); lua_setfield(L, -2, "Ret");
	lua_pushcfunction(L, createAdd); lua_setfield(L, -2, "Add");
	lua_pushcfunction(L, createSub); lua_setfield(L, -2, "Sub");
	lua_pushcfunction(L, createDiv); lua_setfield(L, -2, "Div");
	lua_pushcfunction(L, createMul); lua_setfield(L, -2, "Mul");
	lua_pushcfunction(L, createRem); lua_setfield(L, -2, "Rem");
	lua_pushcfunction(L, createNeg); lua_setfield(L, -2, "Neg");
	lua_pushcfunction(L, createNot); lua_setfield(L, -2, "Not");
	lua_pushcfunction(L, createCmpEQ); lua_setfield(L, -2, "CmpEQ");
	lua_pushcfunction(L, createCmpNE); lua_setfield(L, -2, "CmpNE");
	lua_pushcfunction(L, createCmpLE); lua_setfield(L, -2, "CmpLE");
	lua_pushcfunction(L, createCmpLT); lua_setfield(L, -2, "CmpLT");
	lua_pushcfunction(L, createCmpGE); lua_setfield(L, -2, "CmpGE");
	lua_pushcfunction(L, createCmpGT); lua_setfield(L, -2, "CmpGT");
	lua_pushcfunction(L, createFToI); lua_setfield(L, -2, "FToI");
	lua_pushcfunction(L, createIToF); lua_setfield(L, -2, "IToF");
	lua_pushcfunction(L, createBitCast); lua_setfield(L, -2, "BitCast");
	lua_pushcfunction(L, createCall); lua_setfield(L, -2, "Call");
	lua_pushcfunction(L, createExtractValue); lua_setfield(L, -2, "ExtractValue");
	lua_pushcfunction(L, createInsertValue); lua_setfield(L, -2, "InsertValue");
	lua_pushcfunction(L, createMalloc); lua_setfield(L, -2, "Malloc");
	lua_pushcfunction(L, createAlloca); lua_setfield(L, -2, "Alloca");
	lua_pushcfunction(L, createFree); lua_setfield(L, -2, "Free");
	lua_pushcfunction(L, createLoad); lua_setfield(L, -2, "Load");
	lua_pushcfunction(L, createStore); lua_setfield(L, -2, "Store");
	lua_pushcfunction(L, createGEP); lua_setfield(L, -2, "GEP");
}

/*
	DynamicLibrary:
*/
#pragma mark DynamicLibrary
int LoadLibraryPermanently(lua_State * L) {
	const char * filename = luaL_checkstring(L, 1);
	std::string ErrMsg;
	bool res = llvm::sys::DynamicLibrary::LoadLibraryPermanently(filename, &ErrMsg);
	lua_pushboolean(L, res);
	lua_pushstring(L, ErrMsg.c_str());
	return 2;
}

int SearchForAddressOfSymbol(lua_State * L) {
	const char * symbolName = luaL_checkstring(L, 1);
	lua_pushlightuserdata(L, llvm::sys::DynamicLibrary::SearchForAddressOfSymbol(symbolName));
	return 1;
} 

int AddSymbol(lua_State * L) {
	const char * symbolName = luaL_checkstring(L, 1);
	void * symbolValue = lua_touserdata(L, 2);
	llvm::sys::DynamicLibrary::AddSymbol(symbolName, symbolValue);
	return 0;
}


/*
	Other
*/
#pragma mark Other
int readBitcodeFile(lua_State * L) {
	std::string filename = lua_tostring(L, 1);
	
	std::string err;
	MemoryBuffer * buffer = MemoryBuffer::getFile(filename.c_str(), &err);
	if (err.size()) {
		luaL_error(L, "%s: %s", filename.c_str(), err.c_str());
	}

	Module * bitcodemodule = ParseBitcodeFile(buffer, (getGlobalContext()), &err);
	if (err.size()) {
		luaL_error(L, "%s: %s", filename.c_str(), err.c_str());
	}

	Glue<Module>::push(L, bitcodemodule);
	return 1;
}
/*
static unsigned GetFID(const FIDMap& FIDs, const SourceManager &SM, SourceLocation L)
{
  FileID FID = SM.getFileID(SM.getInstantiationLoc(L));
  FIDMap::const_iterator I = FIDs.find(FID);
  assert(I != FIDs.end());
  return I->second;
}
*/


#pragma mark Clang
using namespace clang;
using namespace clang::driver;

class LuaDiagnosticPrinter : public DiagnosticClient {
	lua_State * L;
public:
	LuaDiagnosticPrinter(lua_State * L)
    : L(L) {}
	virtual ~LuaDiagnosticPrinter() {}
	
	virtual void HandleDiagnostic(Diagnostic::Level Level,
                                const DiagnosticInfo &Info) {
		std::ostringstream OS;

		OS << "clang: ";

		switch (Level) {
			case Diagnostic::Ignored: assert(0 && "Invalid diagnostic type");
			case Diagnostic::Note:    OS << "note: "; break;
			case Diagnostic::Warning: OS << "warning: "; break;
			case Diagnostic::Error:   OS << "error: "; break;
			case Diagnostic::Fatal:   OS << "fatal error: "; break;
		}

		const FullSourceLoc& SourceLoc = Info.getLocation();
		int LineNum = SourceLoc.getInstantiationLineNumber();
		int ColNum = SourceLoc.getInstantiationColumnNumber();
		int FileOffset = SourceLoc.getManager().getFileOffset(SourceLoc);
		
		std::pair< const char *, const char * > LocBD = SourceLoc.getBufferData();
		OS << SourceLoc.getManager().getBufferName(SourceLoc) << " " << FileOffset << " " << LineNum << ":" << ColNum << "\n";
		
		char errstr[128];
		strncpy(errstr, LocBD.first+FileOffset, 127);
//		ddebug("%s\n", errstr);
		OS << errstr;

		llvm::SmallString<100> OutStr;
		Info.FormatDiagnostic(OutStr);
		OS.write(OutStr.begin(), OutStr.size());
		
		const CodeModificationHint * hint = Info.getCodeModificationHints();
		if (hint) {
			OS << hint->CodeToInsert;
		}
		
		OS << "\n\n";
		
		lua_pushfstring(L, OS.str().c_str());
	}
};

static std::vector<std::string> default_headers;

int addSearchPath(lua_State * L) {
	std::string path = luaL_checkstring(L, 1);
	// TODO: avoid double insertions
	default_headers.push_back(path);
	return 0;
}


class Compiler {
public:
	Compiler() {}
	~Compiler() {ddebug("~Compiler\n");}
	
	char Oflag;						// -O0 .. -O4 
	std::set<std::string> iflags;	// differentiate -i or -I ? and -F ?
//	std::set<std::string> lflags;	// does a linker make sense for JIT ?
	std::set<std::string> dflags;	// -D
	std::set<std::string> wflags;	// -W
//	std::set<std::string> mflags;	// -m
//	std::set<std::string> fflags;	// -f
//	// other flags: -x (language), -std (language standard), etc., -arch, -g (debug info), 
	
	static int compile(lua_State *L) {
		Compiler *self = Glue<Compiler>::checkto(L, 1);
		if(self) {
			std::string csource = luaL_checkstring(L, 2);
			std::string srcname = luaL_optstring(L, 3, "untitled");
			std::string predefines = luaL_optstring(L, 4, "");
			std::string isysroot = luaL_optstring(L, 5, "/Developer/SDKs/MacOSX10.4u.sdk");
			
			
			// todo: set include search paths
			lua_settop(L, 0);
			
			// Souce to compile
			MemoryBuffer *buffer = MemoryBuffer::getMemBufferCopy(csource.c_str(), csource.c_str() + csource.size(), srcname.c_str());
			if(!buffer) {
				luaL_error(L, "couldn't load %s\n", srcname.c_str());
				return 0;
			}
			
			// Diagnostics (warning/error handling)
			LuaDiagnosticPrinter client(L);
			Diagnostic diags(&client);
			
			
		//------------------------------------------------------
		// Platform info
			TargetInfo *target = TargetInfo::CreateTargetInfo(llvm::sys::getHostTriple());
			
			LangOptions lang;

			// from clang-cc:684
			// Allow the target to set the default the langauge options as it sees fit.
			target->getDefaultLangOptions(lang);
			
			lang.C99 = 1;
			lang.HexFloats = 1;
			lang.BCPLComment = 1;  // Only for C99/C++.
			lang.Digraphs = 1;     // C94, C99, C++.
			// GNUMode - Set if we're in gnu99, gnu89, gnucxx98, etc.
			lang.GNUMode = 1;
			lang.ImplicitInt = 0;
			lang.DollarIdents = 1;
			lang.WritableStrings = 0;
			lang.Exceptions = 0;
			lang.Rtti = 0;
			lang.Bool = 0;
			lang.MathErrno = 0;
			lang.InstantiationDepth = 99;
			lang.OptimizeSize = 0;
			lang.PICLevel = 1;
			lang.GNUInline = 0;
			lang.NoInline = 1;
			lang.Static = 0;

		//------------------------------------------------------
		// Search paths
			FileManager fm;
			HeaderSearch headers(fm);
			InitHeaderSearch initHeaders(headers, true, isysroot);
			for(std::set<std::string>::iterator it = self->iflags.begin(); it != self->iflags.end(); ++it) {
				initHeaders.AddPath(*it, InitHeaderSearch::Angled, false, true, false);
			}
		
				
			// Add system search paths
			initHeaders.AddDefaultSystemIncludePaths(lang);
			initHeaders.Realize();
			
			
		//------------------------------------------------------
		// Preprocessor 

			SourceManager sm;
			Preprocessor pp(diags, lang, *target, sm, headers);
			pp.setPredefines(predefines);
			

		//------------------------------------------------------
		// Source compilation
			sm.createMainFileIDForMemBuffer(buffer);
			
			IdentifierTable idents(lang);
			SelectorTable selects;
			Builtin::Context builtin(*target);
			builtin.InitializeBuiltins(idents);
			ASTContext context(lang, sm, *target, idents, selects, builtin);
			CompileOptions copts; // e.g. optimizations
			CodeGenerator * codegen = CreateLLVMCodeGen(diags, srcname, copts, getGlobalContext());
			
			ParseAST(pp, codegen, context, false); // last flag is verbose statist
			Module * cmodule = codegen->ReleaseModule(); // or GetModule() if we want to reuse it?
			if (cmodule) {
				// link with other module? JIT?
				lua_pushboolean(L, true);
				Glue<Module>::push(L, cmodule);
				
				delete codegen;
				delete target;
			
				return 2;
			} else {
				lua_pushboolean(L, false);
				lua_insert(L, 1);
				// diagnose?
				unsigned count = diags.getNumDiagnostics();
				
				delete codegen;
				delete target;
				
				return count+1;
			}
		}
		else {
			luaL_error(L, "Compiler.compile: invalid object or arguments");
		}
		return 0;
	}
	
	static int include(lua_State * L) {	
		Compiler * self = Glue<Compiler>::checkto(L, 1);
		std::string str = luaL_checkstring(L, 2);
		self->iflags.insert(str);
		return 0;
	}
	
	static int define(lua_State * L) {	
		Compiler * self = Glue<Compiler>::checkto(L, 1);
		std::string str = luaL_checkstring(L, 2);
		self->dflags.insert(str);
		return 0;
	}
	
	static int warning(lua_State * L) {	
		Compiler * self = Glue<Compiler>::checkto(L, 1);
		std::string str = luaL_checkstring(L, 2);
		self->wflags.insert(str);
		return 0;
	}
};

template <> const char * Glue<Compiler>::usr_name() { return "Compiler"; }

template <> Compiler * Glue<Compiler>::usr_new(lua_State * L) {
	return new Compiler();
}
template <> void Glue<Compiler>::usr_gc(lua_State * L, Compiler * self) {
	delete self;
}
template <> void Glue<Compiler>::usr_mt(lua_State * L) {
	lua_pushcfunction(L, Compiler::compile);		lua_setfield(L, -2, "compile");
	lua_pushcfunction(L, Compiler::include);		lua_setfield(L, -2, "include");
	lua_pushcfunction(L, Compiler::warning);		lua_setfield(L, -2, "warning");
	lua_pushcfunction(L, Compiler::define);			lua_setfield(L, -2, "define");
}


int compile(lua_State * L) {

	std::string csource = luaL_checkstring(L, 1);
	std::string srcname = luaL_optstring(L, 2, "untitled");
	std::string predefines = luaL_optstring(L, 3, "");
	std::string isysroot = luaL_optstring(L, 4, "/Developer/SDKs/MacOSX10.4u.sdk");
	
	
	// todo: set include search paths
	lua_settop(L, 0);
	
	// Souce to compile
	MemoryBuffer *buffer = MemoryBuffer::getMemBufferCopy(csource.c_str(), csource.c_str() + csource.size(), srcname.c_str());
	if(!buffer) {
		luaL_error(L, "couldn't load %s\n", srcname.c_str());
		return 0;
	}
	
	// Diagnostics (warning/error handling)
	LuaDiagnosticPrinter client(L);
	Diagnostic diags(&client);
	
	
//------------------------------------------------------
// Platform info
	TargetInfo *target = TargetInfo::CreateTargetInfo(llvm::sys::getHostTriple());

	LangOptions lang;

	// from clang-cc:684
	// Allow the target to set the default the langauge options as it sees fit.
	target->getDefaultLangOptions(lang);
	lang.C99 = 1;
    lang.HexFloats = 1;
	lang.BCPLComment = 1;  // Only for C99/C++.
	lang.Digraphs = 1;     // C94, C99, C++.
	// GNUMode - Set if we're in gnu99, gnu89, gnucxx98, etc.
	lang.GNUMode = 1;
	lang.ImplicitInt = 0;
	lang.DollarIdents = 1;
	lang.WritableStrings = 0;
	lang.Exceptions = 0;
	lang.Rtti = 0;
	lang.Bool = 0;
	lang.MathErrno = 0;
	lang.InstantiationDepth = 99;
	lang.OptimizeSize = 0;
	lang.PICLevel = 1;
	lang.GNUInline = 0;
	lang.NoInline = 1;
	lang.Static = 0;

//------------------------------------------------------
// Search paths
	FileManager fm;
	HeaderSearch headers(fm);
	InitHeaderSearch initHeaders(headers, true, isysroot);
	
	for(int i=0; i < default_headers.size(); ++i) {
		initHeaders.AddPath(default_headers[i], InitHeaderSearch::Angled, false, true, false);
	}
	
	/// CLANG HEADERS:
//	Init.AddPath(MainExecutablePath.c_str(), InitHeaderSearch::System,
//				false, false, false, true /*ignore sysroot*/);
	
	// Add system search paths
	initHeaders.AddDefaultSystemIncludePaths(lang);
	initHeaders.Realize();
	
	
//------------------------------------------------------
// Preprocessor 

	SourceManager sm;
	Preprocessor pp(diags, lang, *target, sm, headers);
	pp.setPredefines(predefines);
	

//------------------------------------------------------
// Source compilation
	sm.createMainFileIDForMemBuffer(buffer);
	
	IdentifierTable idents(lang);
	SelectorTable selects;
	Builtin::Context builtin(*target);
	builtin.InitializeBuiltins(idents);
	ASTContext context(lang, sm, *target, idents, selects, builtin);
	CompileOptions copts; // e.g. optimizations
	CodeGenerator * codegen = CreateLLVMCodeGen(diags, srcname, copts, getGlobalContext());
	
	ParseAST(pp, codegen, context, false); // last flag is verbose statist
	Module * cmodule = codegen->ReleaseModule(); // or GetModule() if we want to reuse it?
	if (cmodule) {
		// link with other module? JIT?
		//lua_pushboolean(L, true);
		Glue<Module>::push(L, cmodule);
	} else {
		lua_pushboolean(L, false);
		lua_insert(L, 1);
		// diagnose?
		unsigned count = diags.getNumDiagnostics();
		return count+1;
	}

	delete codegen;
	delete target;
	return 1;
}

/*
	Lua interactions
*/
#pragma mark Lua
int getLuaState(lua_State * L) {
	lua_pushlightuserdata(L, L);
	return 1;
}


extern CodeGenerator * clang_cc_main(int argc, char **argv, const char *srcname, const char *csource);

int lua_clang_cc(lua_State *L) {
	luaL_argcheck(L, lua_type(L,1)==LUA_TTABLE, 1, "compiler flags table");
	luaL_argcheck(L, lua_type(L,2)==LUA_TSTRING, 2, "source string");	
		
	std::vector<std::string> args;
	int len = lua_objlen(L, 1);
	for(int i=1; i <= len; i++) {
		lua_rawgeti(L, 1, i);
		const char *s = lua_tostring(L, -1);
		args.push_back(std::string(s));
		lua_pop(L, 1);
	}
	
	const char *argv[256];
	for(int i=0; i < args.size(); i++) {
		argv[i] = args[i].c_str();
			ddebug("argv[%d]: %s\n", i, argv[i]);
	}
	
	std::string csource = lua_tostring(L, 2);
	
	// Diagnostics (warning/error handling)
//		LuaDiagnosticPrinter client(L);
//		Diagnostic diags(&client);
	std::string srcname = luaL_optstring(L, 3, "untitled");
	
//		CompileOptions copts; // e.g. optimizations
//		CodeGenerator * codegen = CreateLLVMCodeGen(diags, srcname, copts, getGlobalContext());
	
	CodeGenerator * codegen = clang_cc_main(args.size(), (char **)argv, srcname.data(), csource.data());
	
	Module * cmodule = codegen->ReleaseModule(); // or GetModule() if we want to reuse it?
	if(cmodule) {
//			ddebug("Print functions\n");
		Module::FunctionListType &fl = cmodule->getFunctionList();
		for(Module::FunctionListType::iterator it = fl.begin(); it != fl.end(); ++it) {
//				ddebug("F: %s\n", it->getName().data());
		}
	
		// link with other module? JIT?
		//lua_pushboolean(L, true);
		Glue<Module>::push(L, cmodule);
	} else {
		lua_pushboolean(L, false);
//			lua_insert(L, 1);
		// diagnose?
//			unsigned count = diags.getNumDiagnostics();
//			return count+1;
		return 1;
	}
	
	delete codegen;
	return 1;
}


int luaopen_clang(lua_State * L) {

	// too damn useful to not have around:
	if (llvm::InitializeNativeTarget()) 
		luaL_error(L, "InitializeNativeTarget failure");
		
	llvm::sys::DynamicLibrary::AddSymbol("printf", (void *)printf);

	const char * libname = lua_tostring(L, -1);
	struct luaL_reg lib[] = {
		{"LoadLibraryPermanently", LoadLibraryPermanently },
		{"SearchForAddressOfSymbol", SearchForAddressOfSymbol },
		{"AddSymbol", AddSymbol },
		
		{"readBitcodeFile", readBitcodeFile },
		
		{"addSearchPath", addSearchPath },
		{"compile", compile},
		
		{"getLuaState", getLuaState},
		
		{"cc", lua_clang_cc},
		{NULL, NULL},
	};
	luaL_register(L, libname, lib);
	
	//lua::dump(L, "luaopen_clang lib");
	
	Glue<llvm::Module>::define(L);			Glue<llvm::Module>::register_ctor(L);
	Glue<llvm::ModuleProvider>::define(L);	Glue<llvm::ModuleProvider>::register_ctor(L);
	
	Glue<llvm::Type>::define(L);			Glue<llvm::Type>::register_table(L);
	Glue<llvm::StructType>::define(L);		Glue<llvm::StructType>::register_ctor(L);
	Glue<llvm::SequentialType>::define(L);
	Glue<llvm::PointerType>::define(L);		Glue<llvm::PointerType>::register_ctor(L);
	Glue<llvm::ArrayType>::define(L);		Glue<llvm::ArrayType>::register_ctor(L);
	Glue<llvm::VectorType>::define(L);		Glue<llvm::VectorType>::register_ctor(L);
	Glue<llvm::OpaqueType>::define(L);		Glue<llvm::OpaqueType>::register_ctor(L);
	Glue<llvm::FunctionType>::define(L);	Glue<llvm::FunctionType>::register_ctor(L);
		
	Glue<llvm::Value>::define(L);
	Glue<llvm::Argument>::define(L);
	Glue<llvm::Instruction>::define(L); 
	Glue<llvm::PHINode>::define(L); 
	Glue<llvm::BasicBlock>::define(L);		Glue<llvm::BasicBlock>::register_ctor(L);
	Glue<llvm::Constant>::define(L); 
	Glue<llvm::GlobalValue>::define(L); 
	Glue<llvm::Function>::define(L);		Glue<llvm::Function>::register_ctor(L);
	Glue<llvm::GlobalVariable>::define(L);	Glue<llvm::GlobalVariable>::register_ctor(L);
	
	Glue<IRBuilder<> >::define(L);			Glue<llvm::GlobalVariable>::register_ctor(L);
	Glue<ExecutionEngine>::define(L);		Glue<llvm::ExecutionEngine>::register_table(L);
	
	Glue<Compiler>::define(L);				Glue<Compiler>::register_ctor(L);
	
	//lua::dump(L, "luaopen_clang defines");
	
	lua_newtable(L);
	lua_pushinteger(L, GlobalValue::ExternalLinkage); lua_setfield(L, -2, "External");
	lua_pushinteger(L, GlobalValue::LinkOnceAnyLinkage); lua_setfield(L, -2, "LinkOnce");
	lua_pushinteger(L, GlobalValue::WeakAnyLinkage); lua_setfield(L, -2, "Weak");
	lua_pushinteger(L, GlobalValue::AppendingLinkage); lua_setfield(L, -2, "Appending");
	lua_pushinteger(L, GlobalValue::InternalLinkage); lua_setfield(L, -2, "Internal");
	lua_pushinteger(L, GlobalValue::DLLImportLinkage); lua_setfield(L, -2, "DLLImport");
	lua_pushinteger(L, GlobalValue::DLLExportLinkage); lua_setfield(L, -2, "DLLExport");
	lua_pushinteger(L, GlobalValue::ExternalWeakLinkage); lua_setfield(L, -2, "ExternalWeak");
	lua_pushinteger(L, GlobalValue::GhostLinkage); lua_setfield(L, -2, "Ghost");
	lua_pushinteger(L, GlobalValue::CommonLinkage); lua_setfield(L, -2, "Common");
	lua_setfield(L, -2, "Linkage");
	
	//lua::dump(L, "luaopen_clang constants");
	
	// create a default module (+ ensures that EE exists)
	lua_pushstring(L, "main");
	lua_insert(L, 1);
	Glue<llvm::Module>::create(L);
	lua_setfield(L, -2, "main");
	
	return 1;
}