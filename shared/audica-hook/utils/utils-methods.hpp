#pragma once

#include <scotland2/shared/modloader.h>

#include "../audica.h"

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

    template <typename T = System_String_o*>
    requires std::same_as<T, System_String_o*>
    static inline u16string_view csstrtostr(T in) {
        return csstrtostr(reinterpret_cast<Il2CppString*>(in));
    }

    /**
     * @brief Sets the value of an Il2CppString from a UTF-16 string view.
     *
     * @param in The Il2CppString to modify.
     * @param str The UTF-16 string view to set as the value.
     */
    void setcsstr(Il2CppString* in, u16string_view str);

    template <typename T = System_String_o*>
    requires std::same_as<T, System_String_o*>
    static inline void setcsstr(T in, u16string_view str) {
        return setcsstr(reinterpret_cast<Il2CppString*>(in), str);
    }

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
