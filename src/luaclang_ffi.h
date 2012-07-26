
#ifndef LUA_CLANG_FFI
#define LUA_CLANG_FFI

#ifdef __cplusplus 
	#include "lua_compiler.hpp"
	typedef struct al::Compiler * LCCompilerRef;
	typedef struct al::JIT * LCJITRef;
	
	extern "C" {
	
	
#else
	typedef struct Compiler * LCCompilerRef;
	typedef struct JIT * LCJITRef;

#endif

LCCompilerRef	lc_compiler_create();
void	lc_compiler_destroy(LCCompilerRef C);
void	lc_compiler_cpp(LCCompilerRef C, int cpp);
int		lc_compiler_compile(LCCompilerRef C, const char * code, const char * name);
void	lc_compiler_define(LCCompilerRef C, const char * opt);
void	lc_compiler_include(LCCompilerRef C, const char * path);
void	lc_compiler_dump(LCCompilerRef C);
void	lc_compiler_optimize(LCCompilerRef C, const char * olevel);
LCJITRef lc_compiler_jit(LCCompilerRef C);

int		lc_jit_retain(LCJITRef jit);
int		lc_jit_release(LCJITRef jit);
int		lc_jit_refs(LCJITRef jit);
void *	lc_jit_getfunctionptr(LCJITRef jit, const char * name);

void	lc_sweep();
void *	lc_findsymbol(const char * symbolName);


#ifdef __cplusplus 
	}	// extern "C"
#endif

#endif	// LUA_CLANG_FFI