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

#ifndef INCLUDE_LUACLANG_COMPILER_H
#define INCLUDE_LUACLANG_COMPILER_H 1

// for reference (OpenCL)
//llvm::Module *clang(options, source, log) { 
//  // 1. Set up diagnostics 
//  // 2. Initialize language options for OpenCL. 
//  // 3. Create a Preprocessor, parsing -I and -D from ‘options’ 
//  // 4. Create a CodeGenerator and ASTContext. 
//  // 5. Invoke ParseAST() to run the CodeGen on ‘source’. 
//  // 6. Return the Module and any Diagnostics in ‘log’. 
//} 
//
//new ASTContext(PP->getLangOptions(), 
//               PP->getSourceManager(), 
//               PP->getTargetInfo(), 
//               PP->getIdentifierTable(), 
//               PP->getSelectorTable(), 
//DO THIS -----> /* FreeMemory = */ false, 
//               /* size_reserve = */0, 
//               /* InitializeBuiltins = */ !usepch);
//


#include "luaopen_clang.h"

class Compiler {
public:
	Compiler() {}
	~Compiler() {}
	
	char Oflag;						// -O0 .. -O4 
	std::set<std::string> iflags;	// differentiate -i or -I ? and -F ?
//	std::set<std::string> lflags;	// does a linker make sense for JIT ?
	std::set<std::string> dflags;	// -D
	std::set<std::string> wflags;	// -W
//	std::set<std::string> mflags;	// -m
//	std::set<std::string> fflags;	// -f
//	// other flags: -x (language), -std (language standard), etc., -arch, -g (debug info), 
	
	static int compile(lua_State *L) {
		using namespace llvm;
		using namespace clang;
	
		Compiler * self = Glue<Compiler>::checkto(L, 1);

		std::string csource = luaL_checkstring(L, 2);
		std::string srcname = luaL_optstring(L, 3, "untitled");
		//std::string isysroot = luaL_optstring(L, 4, "/Developer/SDKs/MacOSX10.4u.sdk");
		std::string isysroot = luaL_optstring(L, 4, "/");
		
		
		// todo: set include search paths
		lua_settop(L, 0);
		
		// Souce to compile
		MemoryBuffer *buffer = MemoryBuffer::getMemBufferCopy(csource.c_str(), csource.c_str() + csource.size(), srcname.c_str());
		if(!buffer) {
			luaL_error(L, "couldn't load %s\n", srcname.c_str());
			return 0;
		}
		
		// Diagnostics (warning/error handling)
		TextDiagnosticBuffer client;
		Diagnostic diags(&client);
		
		
	//------------------------------------------------------
	// Platform info
		TargetInfo *target = TargetInfo::CreateTargetInfo(llvm::sys::getHostTriple());
		
		
		llvm::StringMap<bool> Features;
		//target->getDefaultFeatures(std::string("yonah"), Features); // detects SSE and other processor-specific settings
		target->getDefaultFeatures(std::string(""), Features); // detects SSE and other processor-specific settings
		target->HandleTargetFeatures(Features);	// sets SSELevel for x86 targets
		
		
		uint64_t width = target->getPointerWidth(0);	// 32
//		printf("getPointerWidth: %
		printf("3DNOW: %s\n", Features.lookup(std::string("3dnow")) ? "TRUE" : "FALSE");
		printf("3DNOWA: %s\n", Features.lookup(std::string("3dnowa")) ? "TRUE" : "FALSE");
		printf("MMX: %s\n", Features.lookup(std::string("mmx")) ? "TRUE" : "FALSE");
		printf("SSE: %s\n", Features.lookup(std::string("sse")) ? "TRUE" : "FALSE");
		printf("SSE2: %s\n", Features.lookup(std::string("sse2")) ? "TRUE" : "FALSE");
		printf("SSE3: %s\n", Features.lookup(std::string("sse3")) ? "TRUE" : "FALSE");
		printf("SSSE3: %s\n", Features.lookup(std::string("ssse3")) ? "TRUE" : "FALSE");
		printf("SSSE41: %s\n", Features.lookup(std::string("ssse41")) ? "TRUE" : "FALSE");
		printf("SSSE42: %s\n", Features.lookup(std::string("ssse42")) ? "TRUE" : "FALSE");
		

		LangOptions lang;
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
		FileManager fm;
		HeaderSearch headers(fm);

		bool verbose_headers = false;
	#ifdef LUA_CLANG_OSX
		InitHeaderSearch initHeaders(headers, verbose_headers, isysroot);
	#else
		InitHeaderSearch initHeaders(headers, verbose_headers);
	#endif

//		for(unsigned int i=0; i < self->iflags.size(); ++i) {
		for(std::set<std::string>::iterator it  = self->iflags.begin();
			it != self->iflags.end();
			++it) 
		{
			std::string path = *it; 
			//printf("include: %s\n", path.data());
			initHeaders.AddPath(path, 
								InitHeaderSearch::Angled, 
								false, 
								true, 
								false);
		}
		
		initHeaders.AddDefaultEnvVarPaths(lang);
		
		// Add system search paths
		initHeaders.AddDefaultSystemIncludePaths(lang);
		initHeaders.Realize();
		
	//------------------------------------------------------
	// Preprocessor 

		SourceManager sm;
		Preprocessor pp(diags, lang, *target, sm, headers);
		
		// TODO: verify correctness of this
		std::string predefines;
		for(std::set<std::string>::iterator it  = self->dflags.begin();
			it != self->dflags.end();
			++it) 
		{
			predefines.append("\n");
			predefines.append(*it);
		}
//		pp.setPredefines(predefines);
		
		PreprocessorInitOptions InitOpts;
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

		ASTContext context(pp.getLangOptions(), sm, pp.getTargetInfo(), pp.getIdentifierTable(), pp.getSelectorTable(), pp.getBuiltinInfo());
		CompileOptions copts; // e.g. optimizations
		CodeGenerator * codegen = CreateLLVMCodeGen(diags, srcname, copts, getGlobalContext());
		
		ParseAST(pp, codegen, context, false); // last flag is verbose statistics
		Module * cmodule = codegen->ReleaseModule(); // or GetModule() if we want to reuse it?
		if (cmodule) {
			//lua_pushboolean(L, true);
			Glue<ModuleProvider>::push(L, createModuleProviderFromJIT(L, cmodule));
		} else {
			lua_pushboolean(L, false);
			lua_insert(L, 1);
			// diagnose?
	//		unsigned count = diags.getNumDiagnostics();
	//		return count+1;
			unsigned count = 0;
			for(TextDiagnosticBuffer::const_iterator it = client.err_begin();
				it != client.err_end();
				++it)
			{
				FullSourceLoc SourceLoc = FullSourceLoc(it->first, sm);;
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

				lua_pushfstring(L, "Error: %s %d:%d\n'%s'\n%s\n", 
									SourceLoc.getManager().getBufferName(SourceLoc),
									LineNum, ColNum, 
									errstr,
									it->second.data());
				count++;
				
				if(count > 250) break;
			}

			for(TextDiagnosticBuffer::const_iterator it = client.warn_begin();
				it != client.warn_end();
				++it)
			{
				FullSourceLoc SourceLoc = FullSourceLoc(it->first, sm);;
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

				lua_pushfstring(L, "Warning: %s %d:%d\n'%s'\n%s\n",
									SourceLoc.getManager().getBufferName(SourceLoc),
									LineNum, ColNum, 
									errstr,
									it->second.data());
				count++;
				
				if(count > 250) break;
			}

			return count+1;
		}

		delete codegen;
		delete target;
		return 1;
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

#endif /* include guard */