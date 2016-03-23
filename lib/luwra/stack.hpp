/* Luwra
 * Minimal-overhead Lua wrapper for C++
 *
 * Copyright (C) 2015, Ole Krüger <ole@vprsm.de>
 */

#ifndef LUWRA_STACK_H_
#define LUWRA_STACK_H_

#include "common.hpp"
#include "types.hpp"

#include <cassert>
#include <utility>
#include <functional>
#include <string>

LUWRA_NS_BEGIN

namespace internal {
template <typename T> struct Layout {
  static_assert(sizeof(T) == -1,
                "Parameter to Layout is not a valid signature");
};

template <typename R> struct Layout<R()> {
  using ReturnType = R;

  template <typename F, typename... A>
  static inline R direct(State *, int, F &&hook, A &&... args) {
    return hook(std::forward<A>(args)...);
  }
};

template <typename R, typename T> struct Layout<R(T)> {
  using ReturnType = R;

  template <typename F, typename... A>
  static inline R direct(State *state, int n, F &&hook, A &&... args) {
    return hook(std::forward<A>(args)..., Value<T>::read(state, n));
  }
};

template <typename R, typename T1, typename... TR> struct Layout<R(T1, TR...)> {
  using ReturnType = R;

  template <typename F, typename... A>
  static inline R direct(State *state, int n, F &&hook, A &&... args) {
    return Layout<R(TR...)>::direct(state, n + 1, std::forward<F>(hook),
                                    std::forward<A>(args)...,
                                    Value<T1>::read(state, n));
  }
};
}

/**
 * Retrieve values from the stack and invoke a `Callable` with them.
 *
 * \tparam S Signature in the form of `R(A...)` where `A` is a sequence of
 * types, which shall be
 *           retrieved from the stack, and `R` the return type of `hook`
 * \tparam F An instance of `Callable` which accepts parameters `X..., A...` and
 * returns `R`
 *           (this parameter should be inferable and can be omitted)
 * \tparam X Extra argument types
 *
 * \param state Lua state instance
 * \param pos   Index of the first value
 * \param hook  Callable value
 * \param args  Extra arguments which shall be be passed to `hook` before the
 * stack values
 *
 * \returns Result of calling `hook`
 */
template <typename S, typename F, typename... X>
static inline typename internal::Layout<S>::ReturnType
direct(State *state, int pos, F &&hook, X &&... args) {
  return internal::Layout<S>::direct(state, pos, std::forward<F>(hook),
                                     std::forward<X>(args)...);
}

/**
 * Same as `direct(state, 1, hook)`.
 */
template <typename S, typename F, typename... A>
static inline typename internal::Layout<S>::ReturnType
direct(State *state, F &&hook, A &&... args) {
  return internal::Layout<S>::direct(state, 1, std::forward<F>(hook),
                                     std::forward<A>(args)...);
}

/**
 * Synonym for [direct](@ref direct) with a function pointer which lets you omit
 * all template parameters.
 * The stack layout will be inferred using the signature of the given function
 * pointer.
 */
template <typename R, typename... A>
static inline R apply(State *state, int pos, R (*fun)(A...)) {
  return direct<R(A...)>(state, pos, fun);
}

/**
 * Same as `apply(state, 1, fun)`.
 */
template <typename R, typename... A>
static inline R apply(State *state, R (*fun)(A...)) {
  return apply(state, 1, fun);
}

/**
 * Synonym for [direct](@ref direct) with a function object which lets you omit
 * all template parameters.
 * The stack layout will be inferred using the template parameter to your
 * `std::function` object.
 */
template <typename R, typename... A>
static inline R apply(State *state, int pos,
                      const std::function<R(A...)> &fun) {
  return internal::Layout<R(A...)>::direct(state, pos, fun);
}

/**
 * Same as `apply(state, 1, fun)`.
 */
template <typename R, typename... A>
static inline R apply(State *state, const std::function<R(A...)> &fun) {
  return apply(state, 1, fun);
}

namespace internal {
template <typename T> struct LayoutMapper {
  static_assert(sizeof(T) == -1,
                "Parameter to LayoutMapper is not a valid signature");
};

template <typename... A> struct LayoutMapper<void(A...)> {
  template <typename F, typename... X>
  static inline size_t map(State *state, int n, F &&hook, X &&... args) {
    direct<void(A...)>(state, n, std::forward<F>(hook),
                       std::forward<X>(args)...);
    return 0;
  }
};

template <typename R, typename... A> struct LayoutMapper<R(A...)> {
  template <typename F, typename... X>
  static inline size_t map(State *state, int n, F &&hook, X &&... args) {
    return push(state, direct<R(A...)>(state, n, std::forward<F>(hook),
                                       std::forward<X>(args)...));
  }
};
}

/**
 * Similiar to [direct](@ref direct) but pushes the result of the given
 * `Callable` onto the stack.
 * \returns Number of values pushed
 */
template <typename S, typename F, typename... A>
static inline size_t map(State *state, int pos, F &&hook, A &&... args) {
  return internal::LayoutMapper<S>::map(state, pos, std::forward<F>(hook),
                                        std::forward<A>(args)...);
}

/**
 * Same as `map(state, 1, hook)`.
 */
template <typename S, typename F, typename... A>
static inline size_t map(State *state, F &&hook, A &&... args) {
  return internal::LayoutMapper<S>::map(state, 1, std::forward<F>(hook),
                                        std::forward<A>(args)...);
}

LUWRA_NS_END

#endif
