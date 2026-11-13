#include "functions.h"

int luaPushWorldGetAllArray(lua_State* L, int* entities, int arraySize)
{
    lua_newtable(L);

    for (int i = 0; i < arraySize; i++)
    {
        lua_pushinteger(L, entities[i]);
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int print(lua_State* L)
{
    auto thread = getCurrentThread();

    if (thread)
    {
        printf("[%s] ", (*thread).name.c_str());
    }

    int n = lua_gettop(L);

    for (int i = 1; i <= n; i++)
    {
        if (i > 1) printf("\t");
        if (lua_isstring(L, i))
            printf("%s", lua_tostring(L, i));
        else if (lua_isnil(L, i))
            printf("%s", "nil");
        else if (lua_isboolean(L, i))
            printf("%s", lua_toboolean(L, i) ? "true" : "false");
        else
            printf("%s:%p", luaL_typename(L, i), lua_topointer(L, i));
    }

    printf("\n");

    return 0;
}

static int lua_wasKeyPressed(lua_State* L)
{
    LuaArgs args(L);
    auto key = args.getDword();
    args.pushBool(wasKeyPressed(key));
    return args.pushed;
}

static int lua_isKeyDown(lua_State* L)
{
    LuaArgs args(L);
    auto key = args.getDword();
    args.pushBool(isKeyDown(key));
    return args.pushed;
}

static int lua_wasKeyReleased(lua_State* L)
{
    LuaArgs args(L);
    auto key = args.getDword();
    args.pushBool(wasKeyReleased(key));
    return args.pushed;
}

static int lua_worldGetAllVehicles(lua_State* L)
{
    int vehicles[WORLD_GET_ALL_ARRAY_SIZE];
    return luaPushWorldGetAllArray(L, vehicles, worldGetAllVehicles(vehicles, WORLD_GET_ALL_ARRAY_SIZE));
}

static int lua_worldGetAllPeds(lua_State* L)
{
    int peds[WORLD_GET_ALL_ARRAY_SIZE];
    return luaPushWorldGetAllArray(L, peds, worldGetAllPeds(peds, WORLD_GET_ALL_ARRAY_SIZE));
}

static int lua_worldGetAllObjects(lua_State* L)
{
    int objects[WORLD_GET_ALL_ARRAY_SIZE];    
    return luaPushWorldGetAllArray(L, objects, worldGetAllObjects(objects, WORLD_GET_ALL_ARRAY_SIZE));
}

static int lua_worldGetAllPickups(lua_State* L)
{
    int pickups[WORLD_GET_ALL_ARRAY_SIZE];
    return luaPushWorldGetAllArray(L, pickups, worldGetAllPickups(pickups, WORLD_GET_ALL_ARRAY_SIZE));
}

static int lua_getGameVersion(lua_State* L)
{
    LuaArgs args(L);
    args.pushInt(getGameVersion());
    return args.pushed;
}

static int lua_getPluginVersion(lua_State* L)
{
    LuaArgs args(L);
    args.pushInt(PLUGIN_VERSION);
    return args.pushed;
}

static int lua_setUnloadHandler(lua_State* L)
{
    auto thread = getCurrentThread();

    if (lua_isfunction(L, 1))
    {
        auto ref = luaL_ref(L, LUA_REGISTRYINDEX);

        if (thread)
        {
            thread->unloadHandlerRef = ref;
        }
    }
    else if (lua_isnoneornil(L, 1))
    {
        if (thread->unloadHandlerRef != LUA_NOREF)
        {
            luaL_unref(thread->thread, LUA_REGISTRYINDEX, thread->unloadHandlerRef);
            thread->unloadHandlerRef = LUA_NOREF;
        }
    }

    return 0;
}

void luaRegisterFunctions(lua_State* L)
{
    lua_register(L, "print", print);
        
    lua_register(L, "wasKeyPressed", lua_wasKeyPressed);
    lua_register(L, "isKeyDown", lua_isKeyDown);
    lua_register(L, "wasKeyReleased", lua_wasKeyReleased);

    lua_register(L, "worldGetAllVehicles", lua_worldGetAllVehicles);
    lua_register(L, "worldGetAllPeds", lua_worldGetAllPeds);
    lua_register(L, "worldGetAllObjects", lua_worldGetAllObjects);
    lua_register(L, "worldGetAllPickups", lua_worldGetAllPickups);

    lua_register(L, "getGameVersion", lua_getGameVersion);
    lua_register(L, "getPluginVersion", lua_getPluginVersion);
    lua_register(L, "setUnloadHandler", lua_setUnloadHandler);
}