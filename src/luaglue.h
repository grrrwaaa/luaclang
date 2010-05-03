/*
 *  AlloSphere Research Group / Media Arts & Technology, UCSB, 2009
 */

/*
	Copyright (C) 2006-2008. The Regents of the University of California (REGENTS). 
	All Rights Reserved.

	Permission to use, copy, modify, distribute, and distribute modified versions
	of this software and its documentation without fee and without a signed
	licensing agreement, is hereby granted, provided that the above copyright
	notice, the list of contributors, this paragraph and the following two paragraphs 
	appear in all copies, modifications, and distributions.

	IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
	SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
	OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
	BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
	HEREUNDER IS PROVIDED "AS IS". REGENTS HAS  NO OBLIGATION TO PROVIDE
	MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#ifndef LUA_GLUE_H
#define LUA_GLUE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
}
#endif

/*
	Glue
	template annotation class to bind C++ types to Lua (via boxed pointer)
	includes (single-)inheritance and many helper functions
	
	NB: Glue does not preserve identity between userdata and their objects.
		That is, pushing pointer x twice results in two distinct userdata.	
		For this reason, the default behavior for __gc is to take no action.
		The usr_push and usr_gc methods can be used to trigger reqeust/release 
			behaviors for reference-counting, if desired.
		The usr_eq method can be used to query identity of objects in Lua.
*/
#pragma mark Glue

template <typename T>
class Glue {
public:
	/*	
		required hook to define metatable name
	*/
	static const char * usr_name();
	/*	
		optional hook to define metatable superclass name
	*/
	static const char * usr_supername();
	/*	
		optional hook to define a create function (default returns NULL)
		arguments at stack indices 1+
	*/
	static T * usr_new(lua_State * L);
	/*	
		optional hook to specify __gc method (default is no action) 
		NB: multiple calls to push() the same object will result in an equal 
			number of calls to gc() the same object (i.e. some form of reference 
			counting may be appropriate)
	*/
	static void usr_gc(lua_State * L, T * u);
	
	/*	
		optional hook to apply additional behavior when pushing a T into Lua 
			(e.g. reference count increment, create userdata environment, etc.)
		userdata is at stack index -1
	*/
	static void usr_push(lua_State * L, T * u);
	/*	
		optional hook to specify __newindex method (default is to signal an error) 
		key will be at stack index 2, value will be at stack index 3
		(userdata itself is at index 1)
	*/
	static void usr_newindex(lua_State * L, T * u);
	/*	
		optional hook to specify __index method (default is to index metatable, then superclass(es)) 
		key will be at stack index 2
		(userdata itself is at index 1)
	*/
	static int usr_index(lua_State * L, T * u);
	/*	
		optional hook to specify __eq method 
			(default tests equality of pointers a and b)
		NB: Will only apply for objects of the same T (not superclasses)
			The Lua manual states that the __eq metamethod "only is selected 
			when both objects being compared have the same type and the same 
			metamethod for the selected operation."
	*/
	static bool usr_eq(lua_State * L, T * a, T * b);
	/*	
		optional hook to convert non-userdata value at stack index idx to a T 
		e.g. convert a number into a T object...
	*/
	static T * usr_reinterpret(lua_State * L, int idx);
	/*	
		optional hook to override the default __tostring method 
	*/
	static int usr_tostring(lua_State * L, T * u);
	
	/*	
		optional hook to add additional fields to metatable
		metatable is at stack index mt
	*/
	static void usr_mt(lua_State * L, int mt);

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
	
	/*	
		push a T pointer to the Lua space (also calls usr_push if defined) 
		NB: pushing the same object will create a new userdatum for each push
	*/
	static int push(lua_State * L, T * u);
	/*	if index idx is a T (checks metatable key), returns it, else return NULL */
	static T * to(lua_State * L, int idx = 1);
	/*	as above but throws error if not found */
	static T * checkto(lua_State * L, int idx = 1);
	/*	Lua bound constructor (usr_new must be defined) */
	static int create(lua_State * L);
	/*	zero the pointer in the userdata */
	static void erase(lua_State * L, int idx);

private:
	// internal methods:
	static int gc(lua_State * L);
	static int tostring(lua_State * L);
	static int index(lua_State * L);
	static int newindex(lua_State * L);
	static int eq(lua_State * L);
	static const char * mt_name(lua_State * L);
	// object wrapper:
	struct Box { T * ptr; };
};

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
//				printf("%i (-%i): userdata (%p)\n", i, top+1-i, lua_topointer(L, i)); break;
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

/*
	Inline implementation
*/
#pragma mark Inline Implementation

template <typename T> const char * Glue<T> :: usr_supername() { return NULL; }
template <typename T> T * Glue<T> :: usr_new(lua_State * L) { return 0; }
template <typename T> T * Glue<T> :: usr_reinterpret(lua_State * L, int idx) { return 0; } // if idx isn't a userdata
template <typename T> void Glue<T> :: usr_mt(lua_State * L, int mt) {}
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
template <typename T> void Glue<T> :: usr_newindex(lua_State * L, T * u) {
	luaL_error(L, "cannot assign keys/values to %s", Glue<T>::usr_name());
}
template <typename T> int Glue<T> :: usr_index(lua_State * L, T * u) {
	// check mt:
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1); // nil
		//lua_getfield(L, <#int idx#>, <#const char * k#>)
	}

	luaL_error(L, "cannot assign keys/values to %s", Glue<T>::usr_name());
}
template <typename T> int Glue<T> :: usr_tostring(lua_State * L, T * u) { 
	lua_pushfstring(L, "%s: %p", Glue<T>::usr_name(), u); 
	return 1; 
}
template <typename T> bool Glue<T> :: usr_eq(lua_State * L, T * a, T * b) {
	return a == b;
}
template <typename T> void Glue<T> :: usr_push(lua_State * L, T * u) {}

template <typename T> int Glue<T> :: push(lua_State * L, T * u) {
	if (u==0)
		return luaL_error(L, "Cannot create null %s", usr_name());
	Box * udata = (Box *)lua_newuserdata(L, sizeof(Box));
	luaL_getmetatable(L, mt_name(L));
	lua_setmetatable(L, -2);
	udata->ptr = u;
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
		Box * udata = (Box *)lua_touserdata(L, idx);
		udata->ptr = NULL;
	}
}
template <typename T> T * Glue<T> :: to(lua_State * L, int idx) {
	T * u = 0;
	lua_pushvalue(L, idx);
	if (lua_isuserdata(L, -1)) {
		if (lua_getmetatable(L, -1)) {
			lua_getfield(L, -1, Glue<T>::usr_name());
			if (!lua_isnoneornil(L, -1)) {
				u = ((Box *)lua_touserdata(L, -3))->ptr;
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
	int u_mt = lua_gettop(L);
	
	// mt[usr_name] = true
	lua_pushstring(L, Glue<T>::usr_name());
	lua_pushboolean(L, true);
	lua_settable(L, u_mt);

	lua_pushcfunction(L, tostring);
	lua_setfield(L, u_mt, "__tostring");
	lua_pushcfunction(L, gc);
	lua_setfield(L, u_mt, "__gc");
	lua_pushcfunction(L, gc);
	lua_setfield(L, u_mt, "close");	/* equivalent to __gc but manually called */
	lua_pushcfunction(L, create);
	lua_setfield(L, u_mt, "create");
	lua_pushcfunction(L, newindex);
	lua_setfield(L, u_mt, "__newindex");
	lua_pushvalue(L, u_mt);
	//lua_pushcfunction(L, index);
	lua_setfield(L, u_mt, "__index");

	usr_mt(L, u_mt);
	
	if (usr_supername() != NULL) {
		// setmetatable(mt, super.mt)
		const char * supername = lua_pushfstring(L, "glue_meta_%s", usr_supername()); 
		lua_pop(L, 1);
		
		lua_newtable(L);
		lua_pushstring(L, "__index");
		luaL_getmetatable(L, supername);
		lua_rawset(L, -3);
		//lua_setfield(L, u_mt, "__super");
		lua_setmetatable(L, u_mt);
	}
	
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
template <typename T> int Glue<T> :: index(lua_State * L) { 
	T * u = to(L, 1); 
	if (u) { 
		return Glue<T>::usr_index(L, u); 
	} 
	return 0; 
}
template <typename T> int Glue<T> :: newindex(lua_State * L) { 
	T * u = to(L, 1); 
	if (u) { 
		Glue<T>::usr_newindex(L, u); 
	} 
	return 0; 
}
template <typename T> int Glue<T> :: eq(lua_State * L) {
	T * a = to(L, 1);
	T * b = to(L, 2);
	lua_pushboolean(L, usr_eq(L, a, b) == true);
	return 1;
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
	const char * t = lua_pushfstring(L, "glue_meta_%s", usr_name()); lua_pop(L, 1); return t;
}

#endif /* include guard */

