/*
Luaclang is a module (loadable library) for the Lua programming language (www.lua.org) providing bindings to the LLVM/Clang APIs (www.llvm.org), in order to control LLVM/Clang from within Lua scripts.
Luaclang is built against the LLVM 2.6 stable release, for Lua 5.1.
Luaclang aims to closely follow the LLVM API, but making use of Lua idioms where possible. 

Luaclang is released under the MIT open source license
@see http://www.opensource.org/licenses/mit-license.php

Copyright (c) 2009 Graham Wakefield

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef LUA_GLUE_H
#define LUA_GLUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
}
#endif

namespace lua {

static void dump(lua_State * L, const char * msg) {
	printf("DUMP lua_State (%p) %s\n", L, msg);
	int top = lua_gettop(L);
	for (int i=1; i<=top; i++) {
		switch(lua_type(L, i)) {
			case LUA_TNIL:
				printf("%i (-%i): nil\n", i, top+1-i); break;
			case LUA_TBOOLEAN:
				printf("%i (-%i): boolean (%s)\n", i, top+1-i, lua_toboolean(L, i) ? "true" : "false"); break;
			case LUA_TLIGHTUSERDATA:
				printf("%i (-%i): lightuserdata (%p)\n", i, top+1-i, lua_topointer(L, i)); break;
			case LUA_TNUMBER:
				printf("%i (-%i): number (%f)\n", i, top+1-i, lua_tonumber(L, i)); break;
			case LUA_TSTRING:
				printf("%i (-%i): string (%s)\n", i, top+1-i, lua_tostring(L, i)); break;
			case LUA_TUSERDATA:
				//printf("%i (-%i): userdata (%p)\n", i, top+1-i, lua_topointer(L, i)); break;
				lua_getglobal(L, "tostring");
				lua_pushvalue(L, i);
				lua_call(L, 1, 1);
				printf("%i (-%i): %s\n", i, top+1-i, lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			default:{
				printf("%i (-%i): %s (%p)\n", i, top+1-i, lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
			}
		}
	}
}

} //lua::

/*
	Glue
	template annotation class to bind C++ types to Lua
	includes (single-)inheritance and many helper functions
*/
#pragma mark Glue
template <typename T>
class Glue {
public:
	/*	required hook to define metatable name
	*/
	static const char * usr_name();
	/*	optional hook to define metatable superclass name
	*/
	static const char * usr_supername();
	/*	optional hook to define a create function (default returns NULL)
		arguments at stack indices 1+
	*/
	static T * usr_new(lua_State * L);
	/*	optional hook to convert non-userdata value at stack index idx to a T */
	static T * usr_reinterpret(lua_State * L, int idx);
	/*	optional hook to add additional fields to metatable
		metatable is at stack index -1
	*/
	static void usr_mt(lua_State * L);
	/*	optional hook to specify __gc method (default is no action) */
	static void usr_gc(lua_State * L, T * u);
	/*	optional hook to override the default __tostring method */
	static int usr_tostring(lua_State * L, T * u);
	/*	optional hook to apply additional behavior when pushing a T type into Lua space 
		userdata is at stack index -1
	*/
	static void usr_push(lua_State * L, T * u);

	/*	create the metatable 
		if superclass != NULL, metatable will inherit from the superclass metatable 
			(which must already be published)
		call this e.g. in luaopen_xxx
	*/
	static void define(lua_State * L);
	
	/*	register either
			the constructor function (create; usr_new must be defined) 
			or the metatable itself 
		to the table at stack index -1, under the name usr_name
		call this e.g. in luaopen_xxx
	*/
	static void register_ctor(lua_State * L);
	static void register_table(lua_State * L);
	
	/*	Install additional methods to metatable via a luaL_Reg array */
	static void usr_lib(lua_State * L, const luaL_Reg * lib);
	/*	push a T pointer to the Lua space (also calls usr_push if defined) */
	static int push(lua_State * L, T * u);
	/*	if index idx is a T (checks metatable key), returns it, else return NULL */
	static T * to(lua_State * L, int idx = 1);
	/*	as above but throws error if not found */
	static T * checkto(lua_State * L, int idx = 1);
	/*	Lua bound constructor (usr_new must be defined) */
	static int create(lua_State * L);
	/*	zero the pointer in the userdata */
	static void erase(lua_State * L, int idx);

	// internal methods:
	static int gc(lua_State * L);
	static int tostring(lua_State * L);
	static const char * mt_name(lua_State * L);
};

/*
	Inline implementation
*/
#pragma mark Inline Implementation

template <typename T> const char * Glue<T> :: usr_supername() { return NULL; }
template <typename T> T * Glue<T> :: usr_new(lua_State * L) { return 0; }
template <typename T> T * Glue<T> :: usr_reinterpret(lua_State * L, int idx) { return 0; } // if idx isn't a userdata
template <typename T> void Glue<T> :: usr_mt(lua_State * L) {}
template <typename T> void Glue<T> :: usr_lib(lua_State * L, const luaL_Reg * lib) {
	luaL_getmetatable(L, mt_name(L));
	while (lib->name) {
		lua_pushcclosure(L, lib->func, 0);
		lua_setfield(L, -2, lib->name);
		lib++;
	}
	lua_pop(L, 1);
}
template <typename T> void Glue<T> :: usr_gc(lua_State * L, T * u) {}
template <typename T> int Glue<T> :: usr_tostring(lua_State * L, T * u) { lua_pushfstring(L, "%s: %p", Glue<T>::usr_name(), u); return 1; }
template <typename T> void Glue<T> :: usr_push(lua_State * L, T * u) {}

template <typename T> int Glue<T> :: push(lua_State * L, T * u) {
	if (u==0)
		return luaL_error(L, "Cannot create null %s", usr_name());
	T ** udata = (T**)lua_newuserdata(L, sizeof(T*));
	luaL_getmetatable(L, mt_name(L));
	lua_setmetatable(L, -2);
	*udata = u;
	usr_push(L, u);	
	return 1;
}
template <typename T> int Glue<T> :: create(lua_State * L) {
	T * u = usr_new(L);
	if (!u) 
		luaL_error(L, "Error creating %s", usr_name());
	push(L, u);
	return 1;
}
template <typename T> void Glue<T> :: erase(lua_State * L, int idx) {
	if (checkto(L, idx)) {
		T ** udata = (T **)lua_touserdata(L, idx);
		*udata = NULL;
	}
}
template <typename T> T * Glue<T> :: to(lua_State * L, int idx) {
	T * u = 0;
	lua_pushvalue(L, idx);
	if (lua_isuserdata(L, -1)) {
		if (lua_getmetatable(L, -1)) {
			lua_getfield(L, -1, Glue<T>::usr_name());
			if (!lua_isnoneornil(L, -1)) {
				u = *(T **)lua_touserdata(L, -3);
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	} else if (!lua_isnoneornil(L, idx)) {
		u = usr_reinterpret(L, lua_gettop(L));
	}
	lua_pop(L, 1);
	return u;
}
template <typename T> T * Glue<T> :: checkto(lua_State * L, int idx) {
	T * u = to(L, idx);
	if (u == 0) luaL_error(L, "%s not found (index %d)", usr_name(), idx);
	return u;
}
template <typename T> void Glue<T> :: define(lua_State * L) {
	luaL_newmetatable(L, mt_name(L));
	lua_pushstring(L, Glue<T>::usr_name());
	lua_pushboolean(L, true);
	lua_settable(L, -3);

	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	if (usr_supername() != NULL) {
		const char * supername = lua_pushfstring(L, "meta_%s", usr_supername()); lua_pop(L, 1);
		luaL_getmetatable(L, supername);
		lua_setmetatable(L, -2);
	}

	lua_pushcfunction(L, tostring);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, gc);
	lua_setfield(L, -2, "__gc");
	lua_pushcfunction(L, gc);
	lua_setfield(L, -2, "close");	/* equivalent to __gc but manually called */
	lua_pushcfunction(L, create);
	lua_setfield(L, -2, "create");

	int u_mt = lua_gettop(L);
	usr_mt(L);
	lua_settop(L, u_mt-1);
}

template <typename T> void Glue<T> :: register_table(lua_State * L) {
	luaL_getmetatable(L, mt_name(L));
	lua_setfield(L, -2, usr_name());
}
template <typename T> void Glue<T> :: register_ctor(lua_State * L) {
	lua_pushcfunction(L, Glue<T>::create);
	lua_setfield(L, -2, usr_name());
}
template <typename T> int Glue<T> :: gc(lua_State * L) { 
	T * u = to(L, 1); 
	if (u) { 
		Glue<T>::usr_gc(L, u); 
		lua_pushnil(L);
		lua_setmetatable(L, 1);
	} 
	return 0; 
}
template <typename T> int Glue<T> :: tostring(lua_State * L) {
	T * u = to(L, 1);
	if (u)
		Glue<T>::usr_tostring(L, u);
	else
		lua_pushfstring(L, "%s: nil", Glue<T>::usr_name());
	return 1;
}
template <typename T> const char * Glue<T> :: mt_name(lua_State * L) {
	const char * t = lua_pushfstring(L, "meta_%s", usr_name()); lua_pop(L, 1); return t;
}

#endif
