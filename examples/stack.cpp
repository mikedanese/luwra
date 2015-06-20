#include <lua.hpp>
#include <luwra.hpp>

#include <iostream>

using namespace luwra;

static
double sum3(int a, int b, double c) {
	return (a + b) * c;
}

int main() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);

	// Build stack
	lua_pushinteger(state, 13);
	lua_pushinteger(state, 37);
	lua_pushnumber(state, 42);

	// Each value can be retrieved individually.
	std::cout << "a = " << Value<int>::read(state, 1) << std::endl;
	std::cout << "b = " << Value<int>::read(state, 2) << std::endl;
	std::cout << "c = " << Value<double>::read(state, 3) << std::endl;

	// ... which is a little cumbersome. Instead we might apply a fitting function to our stack.
	std::cout << "(a + b) * c = "
	          << apply(state, sum3)
	          << std::endl;

	lua_close(state);
	return 0;
}