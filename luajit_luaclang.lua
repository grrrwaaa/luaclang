local compiler = require "ffi.compiler"
--require "ffi.llvm"

local code = [[

extern "C" double test(double x) {
	return x * 2.;
}

]]

local cc = compiler()
print(cc)
cc:compile(code)
cc:optimize()
cc:dump()

local jit = cc:jit()
print(jit)
local test = jit:getfunctionptr("test")
jit = nil	-- don't use 'test' after the next compiler.sweep!

collectgarbage()
compiler.sweep()
collectgarbage()
compiler.sweep()