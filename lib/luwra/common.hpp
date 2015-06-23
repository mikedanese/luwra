/* Luwra
 * Minimal-overhead Lua wrapper for C++
 *
 * Copyright (C) 2015, Ole Krüger <ole@vprsm.de>
 */

#ifndef LUWRA_COMMON_H_
#define LUWRA_COMMON_H_

// Check C++ version
#if !defined(__cplusplus) || __cplusplus < 201402L
	#error You need a C++14 compliant compiler
#endif

#include <lua.hpp>

// Check for proper Lua version
#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM < 501 || LUA_VERSION_NUM >= 600
	#error Luwra has not been tested against your installed version of Lua
#endif

// Namespaces
#define LUWRA_NS_BEGIN namespace luwra {
#define LUWRA_NS_END }

// Version MAJOR.MINOR.PATH
#define LUWRA_VERSION_MAJOR 0
#define LUWRA_VERSION_MINOR 0
#define LUWRA_VERSION_PATCH 0

#endif
