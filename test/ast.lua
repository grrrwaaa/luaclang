-- use cosmo to produce source code:
require "cosmo"

module(..., package.seeall)

-- cache defined types in module itself:
local typedefs = getfenv()



-- Field metatable
--[[
	A field indicates some sub-element of a type; 
		it has a name, a type, and an offset (index) within the type
--]]
local field_meta = {}
function field_meta:__tostring()
	return string.format("field %s (%s)", self._name, tostring(self._typedef))
end

function field_create(typedef, name, parent, index)
	return setmetatable({ 
		_name = name, 
		_typedef = typedef,
		_offset = index,
		_parent = parent,
	}, field_meta)
end

--[[
	A type defines a particular abstract collection of named fields 
		(and possibly also methods, constants etc.)
--]]
-- Type metatable
local typedef_meta = {}
function typedef_meta:__tostring()
	return string.format("typedef %s", self._name)
end
-- type call creates a new field
function typedef_meta:__call(name, ...)
	return { name = name, typedef = self, ... }
end
-- type index returns a type-field or indexes the metatable:
function typedef_meta:__index(k)
	return typedef_meta[k] or self._fields[k]
end
-- create a pointer type
function typedef_meta:ptr()
	
end

local
function typedef_create(name, def)
	assert(name and type(name) == "string", "typedef name (string) required (argument 1)")
	assert(not typedefs[name], string.format("typedef %s already defined", name))
	def = def or {}
	
	local t = setmetatable({ 
		_name = name 
	}, typedef_meta)
	-- type isa type
	t._def = t
	
	-- a list of fields
	local fields = {}
	for i, v in ipairs (def) do
		assert(type(v) == "table" and type(v.name) == "string", string.format("field %d not understood", i))
		assert(not fields[v.name], string.format("duplicate field name %s", v.name))
		-- field list can be indexed numeric or name key
		local f = field_create(v.typedef, v.name, t, i)
		fields[f._name] = f
		fields[i] = f
	end
	t._fields = fields
	
	typedefs[name] = t
	return t
end

-- create a new typedef
-- e.g. ast.typedef { field list } "name"
function typedef(def) 
	assert(def and type(def) == "table", "definition (table) required (argument 1)")
	
	-- typedef actually returns a type constructor:
	return function(name)
		return typedef_create(name, def)
	end
end

-- some basic types:
typedefs.number = typedef_create("number")

-- some basic template macros for types:
typedef_meta._macros = {}
typedef_meta._macros.do_fields = function(args)
	local def = args[1]
	local prefix = args[2] or ""
	for i, v in ipairs(def._fields) do
		-- unpack aggregate types:
		if #v._typedef._fields > 0 then
			local prefix = string.format("%s%s_", prefix, v._name)
			typedef_meta._macros.do_fields{ v._typedef, prefix }
		else
		
		local comma = ","
		if prefix == "" and i == #def._fields then comma = "" end
		
		cosmo.yield{ 
			i=i, v=v, 
			comma = comma,
			prefix = prefix,
		}
		
		end
	end
end

header = header_prefix .. [==[
typedef double number;

void number_to(lua_State * L, int idx, number * v) { 
	*v = lua_tonumber(L, idx); 
}
int number_is(lua_State * L, int idx) {
	return lua_isnumber(L, idx);
}
void number_check(lua_State * L, int idx, number * v) { 
	if (!number_is(L, idx)) luaL_error(L, "argument %d not a number", idx);
	number_to(L, idx, v);
}
void number_opt(lua_State * L, int idx, number * v, number def) { 
	if (number_is(L, idx)) {
		number_to(L, idx, v);
	} else {
		*v = def;
	}
}
int number_push(lua_State * L, number v) { 
	lua_pushnumber(L, v);
	return 1;
}

]==]

-- generate C code:
local struct_template = [==[

/* todo: nested type needs to be defined first */

/* type $_name */
typedef struct _$_name {
$_macros|do_fields{$_def}[=[	$v|_typedef|_name $v|_name;
]=]} $_name;	

/* 
	problem: aggregate types should be passed as pointers... 
		or via type:copy?
*/
$_name * $_name{}_new($_macros|do_fields{$_def}[=[ $v|_typedef|_name $prefix$v|_name$comma]=] ) {
	$_name * self = ($_name *)malloc(sizeof(struct _$_name));
	$_macros|do_fields{$_def}[=[self->$prefix$v|_name = $v|_name;
	]=]return self;
}

/*	and here is even more complicated... 
		should nested objects be pointers, or somehow print a string...? 
		or should the macros recurse for aggregate types?
	OR-- should the unpacking of aggregates be done at AST level, before code generation?
*/
void $_name{}_dump( $_name * self ) {
	printf("$_name %p { $_macros|do_fields{$_def}[=[$prefix$v|_name %f ]=]} \n", 
		self, 
		$_macros|do_fields{$_def}[=[self->$prefix$v|_name$comma ]=]);
}

void $_name{}_free( $_name * self ) {
	free(self);
}

]==]

local test_template = [==[


]==]

function generate(t)
	--local ctx = setmetatable({ def = t }, type_macros)
	--return cosmo.fill(test_template, t)
	return cosmo.fill(struct_template, t)
end






