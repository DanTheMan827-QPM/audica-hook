#pragma once

// Location of libil2cpp.so

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <string_view>

#include "../inline-hook/And64InlineHook.hpp"
#include "scotland2/shared/modloader.h"
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

#define MAKE_HOOK(name, addr, retval, ...) \
    void* addr_##name = (void*) addr;      \
    retval (*name)(__VA_ARGS__) = NULL;    \
    retval hook_##name(__VA_ARGS__)

#define MAKE_HOOK_OFFSETLESS(name, retval, ...) \
    retval (*name)(__VA_ARGS__) = NULL;         \
    retval hook_##name(__VA_ARGS__)

#ifdef __aarch64__

// ARM64 HOOKS

#define INSTALL_HOOK(name)                \
    log(INFO, "Installing 64 bit hook!"); \
    A64HookFunction((void*) ::AudicaHook::Utils::getRealOffset(addr_##name), (void*) hook_##name, (void**) &name);

#define INSTALL_HOOK_OFFSETLESS(name, methodInfo)    \
    log(INFO, "Installing 64 bit offsetless hook!"); \
    A64HookFunction((void*) methodInfo->methodPointer, (void*) hook_##name, (void**) &name);

#define INSTALL_HOOK_NATIVE(name)                \
    log(INFO, "Installing 64 bit native hook!"); \
    A64HookFunction((void*) (addr_##name), (void*) hook_##name, (void**) &name);

#define INSTALL_HOOK_DIRECT(name, addr)          \
    log(INFO, "Installing 64 bit direct hook!"); \
    A64HookFunction((void*) addr, (void*) hook_##name, (void**) &name);

#else

// ARM32 HOOKS

#define INSTALL_HOOK(name)                                                                                                      \
    log(INFO, "Installing 32 bit hook!");                                                                                       \
    registerInlineHook((uint32_t) ::AudicaHook::Utils::getRealOffset(addr_##name), (uint32_t) hook_##name, (uint32_t**) &name); \
    inlineHook((uint32_t) ::AudicaHook::Utils::getRealOffset(addr_##name));

#define INSTALL_HOOK_OFFSETLESS(name, methodInfo)                                                         \
    log(INFO, "Installing 32 bit offsetless hook!");                                                      \
    registerInlineHook((uint32_t) methodInfo->methodPointer, (uint32_t) hook_##name, (uint32_t**) &name); \
    inlineHook((uint32_t) methodInfo->methodPointer);

#define INSTALL_HOOK_NAT(name)                                                                \
    log(INFO, "Installing 32 bit native hook!");                                              \
    registerInlineHook((uint32_t) (addr_##name), (uint32_t) hook_##name, (uint32_t**) &name); \
    inlineHook((uint32_t) (addr_##name));

#define INSTALL_HOOK_DIRECT(name, addr)                                              \
    log(INFO, "Installing 32 bit offsetless hook!");                                 \
    registerInlineHook((uint32_t) addr, (uint32_t) hook_##name, (uint32_t**) &name); \
    inlineHook((uint32_t) addr);

#endif /* __aarch64__ */
