#pragma once

#define FIRST_(a, ...) a
#define SECOND_(a, b, ...) b

#define FIRST(...) FIRST_(__VA_ARGS__,)
#define SECOND(...) SECOND_(__VA_ARGS__,)

#define EMPTY()

#define EVAL(...) EVAL12(__VA_ARGS__)
#define EVAL12(...) EVAL11(EVAL11(__VA_ARGS__))
#define EVAL11(...) EVAL10(EVAL10(__VA_ARGS__))
#define EVAL10(...) EVAL9(EVAL9(__VA_ARGS__))
#define EVAL9(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL7(EVAL7(__VA_ARGS__))
#define EVAL7(...) EVAL6(EVAL6(__VA_ARGS__))
#define EVAL6(...) EVAL5(EVAL5(__VA_ARGS__))
#define EVAL5(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL3(EVAL3(__VA_ARGS__))
#define EVAL3(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

#define DEFER1(m) m EMPTY()
#define DEFER2(m) m EMPTY EMPTY()()

#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1

#define CAT(a,...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a,...) a ## __VA_ARGS__


#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()

#define BOOL(x) NOT(NOT(x))

#define IF_ELSE(condition) _IF_ELSE(BOOL(condition))
#define _IF_ELSE(condition) CAT(_IF_, condition)

#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define HAS_ARGS(...) BOOL(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _END_OF_ARGUMENTS_() 0

#define STRINGFY(x) #x
