#include <Windows.h>
#include "shv/main.h"
#include "luajit/lua.hpp"
#include "source/lua.h"
#include "source/keyboard.h"
#include "source/console.h"

void scriptInitialize()
{
    luaInitialize();
    luaSearchForScripts();
}

void scriptMain()
{
    scriptInitialize();

    while (true)
    {
        WAIT(0);

        // CTRL + Backspace
        if (isKeyDown(17) && wasKeyPressed(8))
        {
            luaTerminate();
            luaInitialize();
            luaSearchForScripts();
            updateKeyboard();
        }

        luaTick();

        // CTRL + Delete
        if (isKeyDown(17) && wasKeyPressed(46))
        {
            toggleConsole();
            updateKeyboard();
        }

        updateKeyboard();
    }
}

void scriptTerminate()
{
    luaTerminate();
    consoleTerminate();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        scriptRegister(hModule, scriptMain);
        keyboardHandlerRegister(onKeyboardMessage);
        break;
    case DLL_PROCESS_DETACH:
        scriptTerminate();
        scriptUnregister(hModule);
        keyboardHandlerUnregister(onKeyboardMessage);
        break;
    }

    return TRUE;
}