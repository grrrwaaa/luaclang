

for k, v in pairs(clang) do print("clang", k, v) end


-- Module:
local m = clang.Module("module1")
print(m)

-- Type:
for k, v in pairs(clang.Type) do print("Type", k, v) end



local src = [==[

double foo(int x) {
	printf("foo %i\n", x);
	return x*2;
}

int main(int ac, char ** av) {
	
	return (int)foo(4);
}

]==]

local mod = clang.compile(src, "src")

print(clang.ExecutionEngine.call(mod, "foo", 3))

local src2 = [==[

double zap(double x) {
	printf("zap %f\n", x);
	return foo((int)x);
}	

]==]

local mod2 = clang.compile(src2, "src2")
mod2:linkto(mod)

print(mod2)

-- creates an internal function with constants & tail call:
print(clang.ExecutionEngine.call(mod2, "zap", 3))

mod2:dump()


--[[
-- bitcode write/read example:
local mod = clang.compile([==[
	double bitcode_example(double x) {
		printf("bitcode_example %f\n", x);
		return x;
	}
]==], "bitcode_example")
mod:optimize()
mod:writeBitcodeFile("bitcode_example.bc")
local mod2 = clang.readBitcodeFile("bitcode_example.bc")
mod2:dump()
--]]


