#pragma once

#include <string>
#include <lua.hpp>

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

    void loadMainScript();

    void initApiCall();
    void timeStepApiCall(double time);
    void keyPressedApiCall(const std::string &key);
    void keyReleasedApiCall(const std::string &key);
private:
    void checkStack(int &top);
    bool getCallback(const char *module, const char *name);
    bool getCallback(const char *name);
    void throwLuaError();
    void doFileImpl(const std::string &filename); // implemented in ../platform/...

    lua_State *L_;
};
