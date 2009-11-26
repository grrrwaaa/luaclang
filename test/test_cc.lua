
local src = [[

double foo(double v) { return v * 3; }

]]
local flags = {
	
	"-march=i386"
}

print(clang.cc(flags, src))