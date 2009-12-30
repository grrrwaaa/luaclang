-- let Lua also search relative to this script:
local path = string.match(arg[0], ".+/")
package.path = string.format("%s?.lua;%s?/init.lua;%s", path, path, package.path)
package.cpath = string.format("%s?.so;%s", path, package.cpath)

require "clang"

for k, v in pairs(clang) do print("clang", k, v) end

-- we need some C headers:
clang.addSearchPath("/Developer/SDKs/MacOSX10.4u.sdk/usr/include/")
clang.addSearchPath("/Developer/SDKs/MacOSX10.4u.sdk/usr/lib/gcc/i686-apple-darwin9/4.0.1/include")
clang.addSearchPath("/usr/local/include/")

header_prefix = [==[
// TODO: derive this automatically:
typedef char* __builtin_va_list;
#define __GNUC__ 1
#define __APPLE__ 1
#define __i386__ 1
#define __LITTLE_ENDIAN__ 1
#define __FLT_MIN__	0.000000
#define __DBL_MIN__ 0.000000
#define __LDBL_MIN__ -0.00000

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
]==]

-- chose test to run:
--dofile(path .. "test_clang1.lua")
--dofile(path .. "test_vec.lua")
dofile(path .. "test_ast.lua")
--dofile(path .. "test_cc.lua")