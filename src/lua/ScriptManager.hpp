#pragma once

#include <string>
#include <lua.hpp>
#include "../util/Vector.hpp"

class ScriptManager {
public:
    class FileNotFoundError : public std::exception {
    public:
        FileNotFoundError(const std::string &filename);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    class ScriptError : public std::exception {
    public:
        ScriptError(std::string &&error);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    class WrapperError : public std::exception {
    public:
        WrapperError(std::string &&error);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    ScriptManager();
    ~ScriptManager();

    ScriptManager(const ScriptManager &) = delete;
    ScriptManager& operator=(const ScriptManager &) = delete;

    void registerImpl(); // platform-specific function overrides - implemented in ../platform/...
    void loadMainScript();

    void initApiCall();
    void timeStepApiCall(double time);
    void keyPressedApiCall(const std::string &key);
    void keyReleasedApiCall(const std::string &key);
    void pointerDownApiCall(Vector2f pos, int pointer_id);
    void pointerUpApiCall(Vector2f pos, int pointer_id);
    void pointerMoveApiCall(Vector2f pos, int pointer_id);
    void pointerCancelApiCall();
    void mouseButtonPressedApiCall(const char *button, Vector2d pos);
    void mouseButtonReleasedApiCall(const char *button, Vector2d pos);
private:
    void checkStack(int &top);
    bool getCallback(const char *module, const char *name);
    bool getCallback(const char *name);
    void throwLuaError();
    void registerEmptyTable(const char *name);
    void doFileImpl(const std::string &filename); // implemented in ../platform/...

    static constexpr const char *kEngineTableName = "Engine";
    static constexpr const char *kTouchscreenModuleName = "Touchscreen";
    static constexpr const char *kMouseModuleName = "Mouse";

    lua_State *L_;
};
