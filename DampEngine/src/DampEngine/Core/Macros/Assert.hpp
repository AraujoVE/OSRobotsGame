#pragma once

#include <DampEngine/Core/Base.hpp>
#include <DampEngine/Core/Log.hpp>

#define DE_INTERNAL_STRIP_PARENTHESIS(...) __VA_ARGS__

#define DE_ASSERT_INTERNAL_TYPE_APP _
#define DE_ASSERT_INTERNAL_TYPE_ENGINE _ENGINE_ 

#define DE_GET_MACRO_NAME_3(arg1, arg2, arg3, macro, ...) macro
//Usage DE_ASSERT(check_expression, message_string?, (placeholder_val1, val2, ...)? )
#define DE_ASSERT(...) DE_GET_MACRO_NAME_3(__VA_ARGS__,DE_ASSERT_3,DE_ASSERT_2,DE_ASSERT_1)(DE_ASSERT_INTERNAL_TYPE_APP, __VA_ARGS__)
#define DE_ENGINE_ASSERT(...) DE_GET_MACRO_NAME_3(__VA_ARGS__,DE_ASSERT_3,DE_ASSERT_2,DE_ASSERT_1)(DE_ASSERT_INTERNAL_TYPE_ENGINE, __VA_ARGS__)

#define DE_ASSERT_3(type, check, message, param) { if (!(check))  CAT_EVAL(CAT_EVAL(DE, _), ERROR) (message, DE_INTERNAL_STRIP_PARENTHESIS param); }
#define DE_ASSERT_2(type, check, message) { if (!(check))  CAT_EVAL(CAT_EVAL(DE, _), ERROR)(message); }
#define DE_ASSERT_1(type, check) DE_ASSERT_3(type, check, "Assertion failed: '{0}'", (#check) )

