
require "ast"

local typedef = ast.typedef
local number = ast.number -- an pre-defined type
-- number "x" -> { typedef = number, name = "x" }

-- define some new types:
local vec = typedef { number "x", number "y", number "z" } "vec"
-- recursive definition:
--local quat = typedef { number "w", vec "v" } "quat"





-- generate C code:
local vec_src = ast.generate(ast.vec)
--local quat_src = ast.generate(ast.quat)
local src = ast.header .. vec_src --.. quat_src
print(src)



-- compile it:
local m = assert(clang.compile(src))
m:optimize()
--m:dump()



-- runtime tests:
local ee = clang.ExecutionEngine
local function call(...)
	return ee.call(m, ...)
end

local v = call("vec_new", 1, 2, 3)
call("vec_dump", v)
print(v)


for k, v in pairs(m:functions()) do
	print("function", k)
end

--[[
local q = call("quat_new", 4, v)
call("quat_dump", q)
print(q)
--]]