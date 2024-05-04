#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>

void error(lua_State *L, const char *fmt)
{
  fprintf(stderr, "%s: %s\n", fmt, lua_tostring(L, -1));
}

int main_(void)
{
  lua_State *L;

  L = luaL_newstate();

  luaL_openlibs(L); /* Load Lua libraries */

  if (luaL_loadfile(L, "assets/scripts/script_1.lua") || lua_pcall(L, 0, 0, 0))
  {
    fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    exit(1);
  }

  lua_getglobal(L, "test"); 
  
  if (lua_pcall(L, 0, 0, 0) != 0)
    error(L, "error running function `f'");


  if (luaL_loadfile(L, "assets/scripts/script_2.lua") || lua_pcall(L, 0, 0, 0))
  {
    fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    exit(1);
  }

  lua_getglobal(L, "test"); 
  
  if (lua_pcall(L, 0, 0, 0) != 0)
    error(L, "error running function `f'");

  lua_close(L);

  return 0;
}
