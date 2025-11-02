#include "Keyboard.hpp"
#include "../EngineContext.hpp"

void Lua::Keyboard::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"isKeyPressed", isKeyPressedS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, kModuleName);
}

int Lua::Keyboard::isKeyPressedS(lua_State *L) noexcept {
    const char *key = luaL_checkstring(L, 1);
    bool key_state = EngineContext::core()->getKeyboardController().isKeyPressed(key);
    lua_pushboolean(L, static_cast<int>(key_state));
    return 1;
}
