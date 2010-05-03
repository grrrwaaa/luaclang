require "clang"
for k, v in pairs(clang) do print(string.format("clang.%s = %s", k, tostring(v))) end

local cc = clang.Compiler()

local app = (require"app")
cc:include((require"app").apppath .. "/Headers")

cc:define("SCALAR=7")

assert(cc:compile([=[

#include "lua.h"
#include "lauxlib.h"

int test(lua_State * L) {
	double x = luaL_optnumber(L, 1, 0);
	lua_pushnumber(L, x*5);
	return 1;
}

]=]))

--cc:dump()
cc:optimize()
--cc:dump()

print(cc:datalayout())
print(cc:targettriple())
print(clang.lua_pointer())

print(unpack(cc:functions()))

print(cc:getfunction("test"))
local f = cc:getluafunction("test")

for i = 1, 10 do
	print(i, f(i))
end



require "clang.llvm"

local m = cc:module()
local F = clang.llvm.Function(m, "test")
print(F)

