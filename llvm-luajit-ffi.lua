local ffi = require "ffi"
local llvm = require "ffi.llvm"

function FunctionType(ret, ...)
	local numargs = select("#", ...)
	local argsTypes = ffi.new("LLVMTypeRef[?]", numargs, ...)
	return llvm.FunctionType(ret, argsTypes, numargs, false)
end

function Values(...)
	local numargs = select("#", ...)
	return ffi.new("LLVMValueRef[?]", numargs, ...)
end

-- TEST --
function test(debug)
	local C = llvm.GetGlobalContext()
	
	-- cache common type definitions:
	local doubleTy = C:DoubleTypeInContext()
	local voidTy = C:VoidTypeInContext()
	local int32Ty = C:Int32TypeInContext()
	local luaStateTy = C:StructCreateNamed("lua_State")
	local luaStatePtrTy = luaStateTy:PointerType(0)
	
	-- create a module:
	local M = llvm.Module("test")
	
	-- create an instruction builder:
	local B = llvm.Builder()
	
	local lua_tonumber = M:AddFunction("lua_tonumber", FunctionType(doubleTy, luaStatePtrTy, int32Ty))
	local lua_pushnumber = M:AddFunction("lua_pushnumber", FunctionType(voidTy, luaStatePtrTy, doubleTy))
	
	-- create FunctionType *
	local FT = FunctionType(int32Ty, luaStatePtrTy)
	--llvm.FunctionType(int32Ty, argsTypes, numargs, false)
	
	-- create Function *
	local F = M:AddFunction("foo", FT)
	F:SetLinkage(llvm.ExternalLinkage)
	
	-- get arguments as LLVMValueRef:
	local L0 = F:GetParam(0)
	L0:SetValueName("L")
	
	-- create body entry BasicBlock:
	local entryBB = F:AppendBasicBlock("entry")
	-- add to BB using the builder:
	B:PositionBuilderAtEnd(entryBB)
	
	local args = Values(L0, int32Ty:ConstInt(1, true))
	local l1 = B:BuildCall(lua_tonumber, args, 2, "l1")
	-- multiply by 2:
	local v0 = doubleTy:ConstReal(2)
	local v1 = B:BuildFMul(l1, v0, "v1")
	-- push result
	local args = Values(L0, v1)
	local l1 = B:BuildCall(lua_pushnumber, args, 2, "l2")
	B:BuildRet(int32Ty:ConstInt(1, true))
	
	-- wrap module with JIT engine:
	local EE = llvm.ExecutionEngine(M)
	
	-- allocate a lua_CFunction ** in C:
	local bf = clang.make_function_box()
	-- copy in the ffi'd function:
	ffi.cast("void **", bf)[0] = EE:GetPointerToGlobal(F)
	
	-- dump contents:
	if debug then 
		M:DumpModule()
	else
		coroutine.yield()
	end
	-- use C to push this as a lua_CFunction:
	-- stash the EE as an upvalue to prevent it being collected 
	return clang.unbox_function_box(bf, EE)
	
	--[[
	-- calling functions through LLVM GenericValue is more expensive
	local args = ffi.new("LLVMGenericValueRef[?]", 1)
	args[0] = doubleTy:CreateGenericValueOfFloat(10)
	local gv = EE:RunFunction(F, 1, args)
	if debug then
		print("result", doubleTy:GenericValueToFloat(gv))
	end
	--]]
end

function measure()
	local uv = require "uv"
	local t0 = uv.hrtime()
	local runs = 1000
	local tests = {}
	for i = 1, runs do
		tests[i] = coroutine.wrap(test)
	end
	for j = 1, 2 do
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
print(f, f(123))
-- around 1ms for a lua_CFunction to multiply a number by 2.
-- (but no optimizer, almost no assertions)
measure()			


return llvm