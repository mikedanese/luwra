/* Luwra
 * Minimal-overhead Lua wrapper for C++
 *
 * Copyright (C) 2015, Ole Krüger <ole@vprsm.de>
 */

#ifndef LUWRA_COMMON_H_
#define LUWRA_COMMON_H_

// Check C++ version
#if !defined(__cplusplus) || __cplusplus < 201103L
#error You need a C++11 compliant compiler
#endif

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

// Check for proper Lua version
#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM < 501 || LUA_VERSION_NUM >= 600
#error Luwra has not been tested against your installed version of Lua
#endif

// LUA_OK does not exist in Lua 5.1 and earlier
#ifndef LUA_OK
#define LUA_OK 0
#endif

// Namespaces
#define LUWRA_NS_BEGIN namespace luwra {
#define LUWRA_NS_END }

// Version MAJOR.MINOR.PATCH
#define LUWRA_VERSION_MAJOR 0
#define LUWRA_VERSION_MINOR 2
#define LUWRA_VERSION_PATCH 0

#endif
