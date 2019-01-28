#pragma once

extern "C" {
  int l_custom_print(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs != 1) {
      std::cerr << "Called lua print, expected 1 argument, got "
                << nargs << std::endl;
    }
    if (!lua_isstring(L, 1)) {
      std::cerr << "Called lua print, but argument is not a string"
                << std::endl;
    }
    const auto val = lua_tostring(L, 1);
    std::cout << "LUA CUSTOM PRINT: " << val << std::endl;
    return 0;
  }
}

const luaL_Reg printlib [] = {
  {"print", l_custom_print},
  {NULL, NULL} /* end of array */
};

void do_script_test() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  // Load custom print
  lua_getglobal(L, "_G");
  luaL_register(L, NULL, printlib); // for Lua versions < 5.2
  lua_pop(L, 1);

  if (luaL_dofile(L, "assets/scripts/test.lua")) {
    std::cerr << "Error executing lua script:" << std::endl;
    std::cerr << lua_tostring(L, -1) << std::endl;
  }
}
