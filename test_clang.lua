
-- quick'n'dirty way to get current path:
function cmd(str)
	local f = assert(io.popen(str, "r"))
	local res = f:read()
	f:close()
	return res and res:gsub("%c", "")	-- remove control characters
end

local separator = package.config:sub( 1, 1 )
local pwd = assert(cmd"pwd") .. separator
local script = pwd .. (arg and arg[0] or "")
local path = {separator}
for w in string.gmatch(script, string.format("[^%s]+%s", separator, separator)) do
	table.insert(path, w)
end
path = table.concat(path)
print(path)

require "clang"
for k, v in pairs(clang) do 
	print(string.format("clang.%s = %s", k, tostring(v))) 
end

local cc = clang.Compiler()

print()

--local app = (require"app")
cc:include(path .. "headers")

if not clang.hosttriple:find("apple") then
	-- problematic on OSX
	cc:include("/usr/include")
	cc:include("/usr/include/i386-linux-gnu")
	cc:include("/usr/include/x86_64-linux-gnu")
end

cc:define("SCALAR=7")

assert(cc:compile([=[





extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
}

extern "C" int test(lua_State * L) {
	double x = luaL_optnumber(L, 1, 0);
	lua_pushnumber(L, x*5);
	return 1;
}

]=]))

--cc:dump()
cc:optimize()
cc:dump()

print(cc:datalayout())
print(cc:targettriple())
print(clang.lua_pointer())

local jit = cc:jit()

print(jit:getfunctionptr("test"))
local f = assert(jit:pushcfunction("test"), "failed to find function")

for i = 1, 10 do
	print(i, f(i))
end


