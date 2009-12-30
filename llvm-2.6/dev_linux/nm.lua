	---------------------------------------------------------------
-- Bootstrapping functions required to coalesce paths
local
function exec(cmd, echo)
	echo = echo or true
	if(echo) then
		print(cmd)
		print("")
	end
	local res = io.popen(cmd):read("*a")
	return res:sub(1, res:len()-1)
end

local
function stripfilename(filename)
	return string.match(filename, "(.+)/[^/]*%.%w+$")
end

local
function addmodulepath(path)
	-- add to package paths (if not already present)
	if not string.find(package.path, path, 0, true) then
		package.path = string.format("%s/?.lua;%s", path, package.path)
		package.path = string.format("%s/?/init.lua;%s", path, package.path)
		package.cpath = string.format("%s/?.so;%s", path, package.cpath)
	end
end

local
function setup_buildtools_modulepath()

	local pwd = exec("pwd")
	local root = arg[0]
	print(pwd)
	if(root and stripfilename(root)) then 
		root = stripfilename(root) .. "/" 
	else 
		root = "" 
	end
	
	local path = string.format("%s/%s%s", pwd, root, "modules")
	--print("module path", path)
	addmodulepath(path)
end

local
function svnroot()
	local shellpath = exec("pwd")
	local repopath = arg[1]

	local one = repopath:sub(1, 1)
	local two = repopath:sub(2, 2)
	if(one == "/") then
		-- nothing for absolute
	elseif(one == "." and two == ".") then
		-- concatenate for relative
		repopath = shellpath .. "/" .. repopath
	elseif(repopath == ".") then
		repopath = shellpath .. "/"
	else
		error("repo root folder path is invalid, must be either a relative or absolute path")
	end
	
	-- remove trailing '/'
	if(repopath:sub(repopath:len()) == "/") then
		repopath = repopath:sub(1, repopath:len()-1)
	end
	return repopath
end

function getextension(filename)
	return filename:match(".+%.(%w+)$")
end

local format = string.format
function findsym(file, sym)
	local cmd = format("nm %s | grep %s", file, sym)
	return exec(cmd)
end


local PWD = exec("pwd")
addmodulepath(PWD)

local lfs = require("lfs")

local data = {}
local dir = arg[2] or PWD
for f in lfs.dir(dir) do
	if(f:sub(1, 1) ~= '.' and getextension(f) == "a") then
		local lib = dir.."/"..f
--		print(lib)
		local res = findsym(lib, arg[1])
		if(res:len() > 0) then
			data[#data+1] = {
				f, res
			}
		end
	end
end


for i, d in ipairs(data) do
	print("")
	print("")
	print("------------------")
	print(d[1])
	print("------------------")	
	print(d[2])
	print("------------------------------------------------------------------------")
end
