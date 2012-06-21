local ffi = require "ffi"
local clang = require "clang"
local llvm = require "ffi.llvm"

print(jit.version)

local format = string.format
--[[

around 1ms for a lua_CFunction to multiply a number by 2.
around + 0.25ms for lua.h
goes up to 3.5ms for 100 multiplies
optimizer adds about 1ms more

for realistic code, we might expect around 10ms

however...
the llvm version comes out around 4.6ms
the clang version comes out around 4.9ms

Seems like the difference isn't worth it.
	
--]]

-- olevel is an integer to specify -O0, -O1, -O2, -O3
-- returns 0 if no changes made
function optimize(M, olevel)
	local pm = llvm.PassManager()
	local pmb = llvm.PassManagerBuilder()
	pmb:SetOptLevel(olevel or 2)
	pmb:PopulateModulePassManager(pm)
	return pm:RunPassManager(M)
end

function ctest(debug)
	local code = {[[
	extern "C" {
		#include <lua.h>
	}
	]]}
	
	local F = { "extern \"C\" int foo(lua_State * L) {" }
	F[#F+1] = "double v = lua_tonumber(L, 1);"
	F[#F+1] = "double two = 2.;"
	for i = 1, 100 do
		F[#F+1] = "v = v * two;"
	end
	F[#F+1] = "lua_pushnumber(L, v);"
	F[#F+1] = "return 1;"
	F[#F+1] = "}"
	code[#code + 1] = table.concat(F, "\n\t")
	code = table.concat(code, "\n")
	
	cc = clang.Compiler()
	cc:include("headers")
	cc:compile(code)
	cc:optimize("O2")
	if debug then
		cc:dump()
	end
	jit = cc:jit()
	return jit:pushcfunction("foo")
end

-- TEST --
function test(debug)
	
	-- create a module:
	local M = llvm.Module("test")
	local C = M:GetModuleContext()
		
	-- a bit of the lua.h API:
	local C = M:GetModuleContext()
	local doubleTy = C:DoubleTypeInContext()
	local voidTy = C:VoidTypeInContext()
	local voidPtrTy = voidTy:PointerType(0)
	local int32Ty = C:Int32TypeInContext()
	local int32PtrTy = int32Ty:PointerType(0)
	local int8Ty = C:Int8TypeInContext()
	local strTy = int8Ty:PointerType(0)
	local sizeTy = int32Ty
	local luaStateTy = C:StructCreateNamed("lua_State")
	local luaStatePtrTy = luaStateTy:PointerType(0)
	local luaCFunctionTy = llvm.FunctionType(int32Ty, {luaStatePtrTy})
	
	local lua_close = M:AddFunction("lua_close", llvm.FunctionType(voidTy, {luaStatePtrTy}))
	local lua_newthread = M:AddFunction("lua_newthread", llvm.FunctionType(luaStatePtrTy, {luaStatePtrTy}))
	
	local lua_gettop = M:AddFunction("lua_gettop", llvm.FunctionType(int32Ty, {luaStatePtrTy}))
	local lua_settop = M:AddFunction("lua_settop", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_pushvalue = M:AddFunction("lua_pushvalue", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_remove = M:AddFunction("lua_remove", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_insert = M:AddFunction("lua_insert", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_replace = M:AddFunction("lua_replace", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_checkstack = M:AddFunction("lua_checkstack", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_xmove = M:AddFunction("lua_xmove", llvm.FunctionType(voidTy, {luaStatePtrTy, luaStatePtrTy, int32Ty}))

	local lua_isnumber = M:AddFunction("lua_isnumber", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_isstring = M:AddFunction("lua_isstring", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_iscfunction = M:AddFunction("lua_iscfunction", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_isuserdata = M:AddFunction("lua_isuserdata", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_type = M:AddFunction("lua_type", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_typename = M:AddFunction("lua_typename", llvm.FunctionType(strTy, {luaStatePtrTy, int32Ty}))
	local lua_equal = M:AddFunction("lua_equal", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_rawequal = M:AddFunction("lua_rawequal", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_lessthan = M:AddFunction("lua_lessthan", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_tonumber = M:AddFunction("lua_tonumber", llvm.FunctionType(doubleTy, {luaStatePtrTy, int32Ty}))
	local lua_tointeger = M:AddFunction("lua_tointeger", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_toboolean = M:AddFunction("lua_toboolean", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_tolstring = M:AddFunction("lua_tolstring", llvm.FunctionType(strTy, {luaStatePtrTy, int32Ty, int32PtrTy}))
	local lua_objlen = M:AddFunction("lua_objlen", llvm.FunctionType(sizeTy, {luaStatePtrTy, int32Ty}))
	local lua_tocfunction = M:AddFunction("lua_tocfunction", llvm.FunctionType(luaCFunctionTy, {luaStatePtrTy, int32Ty}))
	local lua_touserdata = M:AddFunction("lua_touserdata", llvm.FunctionType(voidPtrTy, {luaStatePtrTy, int32Ty}))
	local lua_tothread = M:AddFunction("lua_tothread", llvm.FunctionType(luaStatePtrTy, {luaStatePtrTy, int32Ty}))
	local lua_topointer = M:AddFunction("lua_topointer", llvm.FunctionType(voidPtrTy, {luaStatePtrTy, int32Ty}))
	
	local lua_pushnil = M:AddFunction("lua_pushnil", llvm.FunctionType(voidTy, {luaStatePtrTy}))
	local lua_pushnumber = M:AddFunction("lua_pushnumber", llvm.FunctionType(voidTy, {luaStatePtrTy, doubleTy}))
	local lua_pushinteger = M:AddFunction("lua_pushinteger", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_pushlstring = M:AddFunction("lua_pushlstring", llvm.FunctionType(voidTy, {luaStatePtrTy, strTy, int32Ty}))
	local lua_pushstring = M:AddFunction("lua_pushstring", llvm.FunctionType(voidTy, {luaStatePtrTy, strTy}))
	local lua_pushcclosure = M:AddFunction("lua_pushcclosure", llvm.FunctionType(voidTy, {luaStatePtrTy, luaCFunctionTy, int32Ty}))
	local lua_pushboolean = M:AddFunction("lua_pushboolean", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_pushlightuserdata = M:AddFunction("lua_pushlightuserdata", llvm.FunctionType(voidTy, {luaStatePtrTy, voidPtrTy}))
	local lua_pushthread = M:AddFunction("lua_pushthread", llvm.FunctionType(int32Ty, {luaStatePtrTy}))
	
	local lua_gettable = M:AddFunction("lua_gettable", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_getfield = M:AddFunction("lua_getfield", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty, strTy}))
	local lua_rawget = M:AddFunction("lua_rawget", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_rawgeti = M:AddFunction("lua_rawgeti", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_createtable = M:AddFunction("lua_createtable", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_newuserdata = M:AddFunction("lua_newuserdata", llvm.FunctionType(voidPtrTy, {luaStatePtrTy, int32Ty}))
	local lua_getmetatable = M:AddFunction("lua_getmetatable", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_getfenv = M:AddFunction("lua_getfenv", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_settable = M:AddFunction("lua_settable", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_setfield = M:AddFunction("lua_setfield", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty, strTy}))
	local lua_rawset = M:AddFunction("lua_rawset", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty}))
	local lua_rawseti = M:AddFunction("lua_rawseti", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_setmetatable = M:AddFunction("lua_setmetatable", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_setfenv = M:AddFunction("lua_setfenv", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	
	local lua_call = M:AddFunction("lua_call", llvm.FunctionType(voidTy, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_pcall = M:AddFunction("lua_pcall", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty, int32Ty, int32Ty}))
	local lua_cpcall = M:AddFunction("lua_cpcall", llvm.FunctionType(int32Ty, {luaStatePtrTy, luaCFunctionTy, voidPtrTy}))
	local lua_yield = M:AddFunction("lua_yield", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_resume = M:AddFunction("lua_resume", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	local lua_status = M:AddFunction("lua_status", llvm.FunctionType(int32Ty, {luaStatePtrTy}))
	local lua_gc = M:AddFunction("lua_gc", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty, int32Ty}))
	local lua_error = M:AddFunction("lua_error", llvm.FunctionType(int32Ty, {luaStatePtrTy}))
	local lua_next = M:AddFunction("lua_next", llvm.FunctionType(int32Ty, {luaStatePtrTy, int32Ty}))
	
	
	-- create an instruction builder:
	local B = llvm.Builder()
	
	-- create Function *
	local F = M:AddFunction("foo", luaCFunctionTy)
	F:SetLinkage(llvm.ExternalLinkage)
	
	-- get arguments as LLVMValueRef:
	local L0 = F:GetParam(0)
	L0:SetValueName("L")
	
	-- create body entry BasicBlock:
	local entryBB = F:AppendBasicBlock("entry")
	-- add to BB using the builder:
	B:PositionBuilderAtEnd(entryBB)
	
	-- get from stack
	local l1 = B:Call(lua_tonumber, "l1", L0, int32Ty:ConstInt(1, true))
	-- multiply by 2:
	local v0 = doubleTy:ConstReal(2)
	for i = 1, 200 do
		 v0 = B:BuildFMul(l1, v0, format("v%d", i))
	end
	-- push result
	local l1 = B:Call(lua_pushnumber, "l2", L0, v0) 
	B:BuildRet(int32Ty:ConstInt(1, true))
	
	optimize(M, 2)
	
	-- dump contents:
	if debug then 
		M:DumpModule()
	end
	
	-- wrap module with JIT engine:
	local EE = llvm.ExecutionEngine(M)
		
	return EE:GetLuaFunction(F)
end

function measure(f)
	local uv = require "uv"
	local t0 = uv.hrtime()
	local runs = 1000
	local tests = {}
	for i = 1, runs do
		tests[i] = coroutine.wrap(f)
	end
	for j = 1, 1 do
		for i = 1, runs do
			tests[i]()
		end
	end
	local us2ms = 0.000001
	local total = (uv.hrtime() - t0) * us2ms
	print(string.format("avg over %d runs = %.3f (ms)", runs, total/runs))
end

local f = test(true)
collectgarbage()
print(f, f(0.9))

local f = ctest(true)
collectgarbage()
print(f, f(0.9))


measure(test)			
measure(ctest)

return llvm