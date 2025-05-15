#pragma once

// Location of libil2cpp.so

#include <dlfcn.h>
#include <fmt/format.h>
#include <scotland2/shared/modloader.h>
#include <stdio.h>
#include <stdlib.h>

#include <paper2_scotland2/shared/logger.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "../../inline-hook/And64InlineHook.hpp"
#include "../../libil2cpp/il2cpp-api-types.h"
#include "il2cpp-utils.hpp"
#include "typedefs.h"

namespace AudicaHook::Utils {
    static inline char const* IL2CPP_SO_PATH = modloader_get_libil2cpp_path();

    using namespace std;
    /**
     * @brief Retrieves the base address of a shared object (library) in memory.
     *
     * @param soname The name of the shared object (e.g., "libil2cpp.so").
     * @return The base address of the shared object as a long long, or NULL if not found.
     */
    long long baseAddr(char const* soname);

    /**
     * @brief Calculates the real memory offset by adding the base address of the shared object to the given offset.
     *
     * @param offset The offset to calculate the real address for.
     * @return The real memory address as a long long.
     */
    long long getRealOffset(void* offset);

    /**
     * @brief Converts an Il2CppString to a UTF-16 string view.
     *
     * @param in The Il2CppString to convert.
     * @return A UTF-16 string view representing the input string.
     */
    u16string_view csstrtostr(Il2CppString* in);

    /**
     * @brief Sets the value of an Il2CppString from a UTF-16 string view.
     *
     * @param in The Il2CppString to modify.
     * @param str The UTF-16 string view to set as the value.
     */
    void setcsstr(Il2CppString* in, u16string_view str);

    /**
     * @brief Converts a UTF-16 string view to a UTF-8 string.
     *
     * @param view The UTF-16 string view to convert.
     * @return A UTF-8 string representing the input string.
     */
    string to_utf8(u16string_view view);

    /**
     * @brief Converts a UTF-8 string view to a UTF-16 string.
     *
     * @param view The UTF-8 string view to convert.
     * @return A UTF-16 string representing the input string.
     */
    u16string to_utf16(string_view view);

    /**
     * @brief Dumps the memory contents around a given pointer for debugging purposes.
     *
     * @param before The number of integers to dump before the pointer.
     * @param after The number of integers to dump after the pointer.
     * @param ptr The pointer to dump memory around.
     */
    void dump(int before, int after, void* ptr);
}  // namespace AudicaHook::Utils

// clang-format off
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(dummy, ##__VA_ARGS__, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
// clang-format on
