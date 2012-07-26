local clang = require "clang"
local ffi = require "ffi"
local lib = ffi.C

local header = [[

void LLVMInitializeX86TargetInfo(void);
void LLVMInitializeX86Target(void);
void LLVMInitializeX86TargetMC(void);

typedef struct Compiler * LCCompilerRef;
typedef struct JIT * LCJITRef;

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
]]
ffi.cdef(header)


-- initialize native target:
lib.LLVMInitializeX86TargetInfo()
lib.LLVMInitializeX86Target()
lib.LLVMInitializeX86TargetMC()

local Compiler = {
	cpp = lib.lc_compiler_cpp,
	define = lib.lc_compiler_define,
	include = lib.lc_compiler_include,
	dump = lib.lc_compiler_dump,
}
Compiler.__index = Compiler

function Compiler:compile(code, name)
	assert(type(code) == "string")
	name = name or "anon"
	return lib.lc_compiler_compile(self, code, name)
end
function Compiler:optimize(o)
	lib.lc_compiler_optimize(self, o or "O2")
end

function Compiler:jit()
	local jit = lib.lc_compiler_jit(self)
	if jit ~= ffi.NULL then
		ffi.gc(jit, lib.lc_jit_release)
		return jit
	end
end

ffi.metatype("struct Compiler", Compiler)

local JIT = {
	retain = lib.lc_jit_retain,
	release = lib.lc_jit_release,
	refs = lib.lc_jit_refs,
	getfunctionptr = lib.lc_jit_getfunctionptr,
}	
JIT.__index = JIT

ffi.metatype("struct JIT", JIT)


return setmetatable({
	sweep = lib.lc_sweep,
	findsymbol = lib.lc_findsymbol,
}, {
	__call = function()
		return ffi.gc(lib.lc_compiler_create(), lib.lc_compiler_destroy)
	end,
})