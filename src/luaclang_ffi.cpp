
/*
Luaclang is a module (loadable library) for the Lua programming language (www.lua.org) providing bindings to the LLVM/Clang APIs (www.llvm.org), in order to control LLVM/Clang from within Lua scripts.
Luaclang is built against the LLVM 3.0 stable release, for Lua 5.1.
Luaclang aims to closely follow the LLVM API, but making use of Lua idioms where possible. 

Luaclang is released under the MIT open source license
@see http://www.opensource.org/licenses/mit-license.php

Copyright (c) 2009-2012 Graham Wakefield & Wesley Smith
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

#include "luaclang_ffi.h"

using al::Compiler;
using al::JIT;

LCCompilerRef	lc_compiler_create() {
	return new Compiler();
}

void	lc_compiler_destroy(LCCompilerRef C) {
	delete C;
}

void lc_compiler_cpp(LCCompilerRef C, int cpp) {
	C->options.CPlusPlus = cpp;
}

int lc_compiler_compile(LCCompilerRef C, const char * code, const char * name) {
	return C->compile(code, name);
}	

void lc_compiler_define(LCCompilerRef C, const char * opt) {
	C->options.defines.push_back(opt);
}	

void lc_compiler_include(LCCompilerRef C, const char * path) {
	C->options.system_includes.push_back(path);
}	

void lc_compiler_dump(LCCompilerRef C) {
	C->dump();
}	

void lc_compiler_optimize(LCCompilerRef C, const char * olevel) {
	C->optimize(olevel);
}	

LCJITRef lc_compiler_jit(LCCompilerRef C) {
	JIT * jit = C->jit();
	if (jit) jit->retain();
	return jit;
}	

int lc_jit_retain(LCJITRef jit) {
	jit->retain();
	return jit->refs();
}	

int lc_jit_release(LCJITRef jit) {
	jit->release();
	return jit->refs();
}

int lc_jit_refs(LCJITRef jit) {
	return jit->refs();
}

void * lc_jit_getfunctionptr(LCJITRef jit, const char * name) {
	return jit->getfunctionptr(name);
}

void lc_sweep() {
	JIT::sweep();
}

void * lc_findsymbol(const char * symbolName) {
	return Compiler::findSymbol(symbolName);
}
