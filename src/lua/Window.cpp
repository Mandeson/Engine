#include "Window.hpp"
#include "../EngineContext.hpp"

void Lua::Window::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"getSize", getSizeS},
        //{"getDefault", getDefaultS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "Window");
}

int Lua::Window::getSizeS(lua_State *L) noexcept {
    auto window_size = EngineContext::core()->getWindowSize();
    lua_pushinteger(L, window_size.x);
    lua_pushinteger(L, window_size.y);
    return 2;
}
