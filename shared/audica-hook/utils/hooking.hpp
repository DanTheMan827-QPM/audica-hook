#pragma once

// Location of libil2cpp.so

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <paper2_scotland2/shared/logger.hpp>
#include <vector>

#include "../../inline-hook/And64InlineHook.hpp"
#include "utils.hpp"

namespace AudicaHook::Utils::Hooking {
    /**
     * @brief The DeferredHooks class manages the registration and installation of
     * hook functions.
     *
     * This class provides a mechanism to register hook installation functions that
     * can be called later to install hooks. It maintains a list of installation
     * functions and provides methods to add to this list and to call all registered
     * functions at a later time.
     */
    class DeferredHooks {
       public:
        /// @brief The HookType enum class represents the type of hook.
        enum class HookType { Early, Late };

       private:
        /// @brief Returns the list of hook installation functions for the specified type.
        /// @param type The type of hooks to get the installation functions for.
        /// @return A reference to the list of installation functions.
        inline static std::vector<void (*)()>& getInstallFuncs(HookType type) {
            switch (type) {
                case ::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Early:
                    static std::vector<void (*)()> earlyInstallFuncs;
                    return earlyInstallFuncs;

                case ::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Late:
                    static std::vector<void (*)()> lateInstallFuncs;
                    return lateInstallFuncs;
            }
        }

       public:
        /// @brief A logger context for the DeferredHooks class.
        inline static constexpr auto logger = Paper::ConstLoggerContext(MOD_ID);

        /// @brief Adds a hook installation function to the specified list.
        /// @param type The type of hook to add the installation function to.
        /// @param installFunc The installation function to add.
        inline static void addInstallFunc(HookType type, void (*installFunc)()) {
            getInstallFuncs(type).push_back(installFunc);
        }

        /// @brief Calls all installation functions added via addInstallFunc and
        /// then clears the list.
        /// @param type The type of hooks to install.
        inline static void installHooks(HookType type) {
            auto installFuncs = getInstallFuncs(type);

            for (auto& func : installFuncs) {
                func();
            }

            // Why would we need to keep these around?
            installFuncs.clear();
        }

        /// @brief Returns the number of hook installation functions that have
        /// been registered for the specified type.
        inline static int getHookCount(HookType type) {
            return getInstallFuncs(type).size();
        }
    };

    enum class HookType { Offset, Offsetless, Native };

    template <typename T>
    concept is_hook = requires {
        // Must have a type
        { T::type() } -> std::same_as<::AudicaHook::Utils::Hooking::HookType>;
        // Must have a name
        { T::name() } -> std::same_as<char const*>;
        // Must have a trampoline that returns the funcType
        { T::trampoline() } -> std::same_as<typename T::funcType*>;
        // Must have a trampoline that returns the funcType
        { T::addr() } -> std::same_as<void*>;
        // Must have a hook that returns the funcType
        { T::hook() } -> std::same_as<typename T::funcType>;
    };

    template <typename L>
    concept is_logger = requires(L& l) {
        l.info("");
        l.debug("");
        l.error("");
        l.warn("");
        l.critical("");
    };

    template <typename T, typename L>
    requires(is_hook<T> && is_logger<L>)
    inline void InstallHook(L& logger) {
#ifndef SUPPRESS_MACRO_LOGS
        logger.info("Installing hook: {} to offset: {}", T::name(), fmt::ptr(T::addr()));
#endif

#ifdef __aarch64__
        // ARM64 Hook Installation
        switch (T::type()) {
            case HookType::Offset:
                logger.info("Installing 64-bit offset hook!");
                A64HookFunction((void*) ::AudicaHook::Utils::getRealOffset(T::addr()), (void*) T::hook(), (void**) T::trampoline());
                break;
            case HookType::Offsetless:
                logger.info("Installing 64-bit offsetless hook!");
                A64HookFunction((void*) T::addr(), (void*) T::hook(), (void**) T::trampoline());
                break;
            case HookType::Native:
                logger.info("Installing 64-bit native hook!");
                A64HookFunction((void*) T::addr(), (void*) T::hook(), (void**) T::trampoline());
                break;
        }
#else
        // ARM32 Hook Installation
        switch (T::type()) {
            case HookType::Offset:
                logger.info("Installing 32-bit offset hook!");
                registerInlineHook((uint32_t) ::AudicaHook::Utils::getRealOffset(T::addr()), (uint32_t) T::hook(), (uint32_t**) T::trampoline());
                inlineHook((uint32_t) ::AudicaHook::Utils::getRealOffset(T::addr()));
                break;
            case HookType::Offsetless:
                logger.info("Installing 32-bit offsetless hook!");
                registerInlineHook((uint32_t) T::addr(), (uint32_t) T::hook(), (uint32_t**) T::trampoline());
                inlineHook((uint32_t) T::addr());
                break;
            case HookType::Native:
                logger.info("Installing 32-bit native hook!");
                registerInlineHook((uint32_t) T::addr(), (uint32_t) T::hook(), (uint32_t**) T::trampoline());
                inlineHook((uint32_t) T::addr());
                break;
        }
#endif
    }
}  // namespace AudicaHook::Utils::Hooking

/// @brief Macro to install a hook.
#define INSTALL_HOOK(name_, logger_) ::AudicaHook::Utils::Hooking::InstallHook<Hook_##name_>(logger_)

/// @brief Macro to install all registered early hooks.
#define INSTALL_EARLY_HOOKS() ::AudicaHook::Utils::Hooking::DeferredHooks::installHooks(::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Early)

/// @brief Macro to install all registered late hooks.
#define INSTALL_LATE_HOOKS() ::AudicaHook::Utils::Hooking::DeferredHooks::installHooks(::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Late)

/// @brief Returns the number of early hook installation functions that have
/// been registered.
#define EARLY_HOOK_COUNT ::AudicaHook::Utils::Hooking::DeferredHooks::getHookCount(::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Early)

/// @brief Returns the number of late hook installation functions that have been
/// registered.
#define LATE_HOOK_COUNT ::AudicaHook::Utils::Hooking::DeferredHooks::getHookCount(::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Late)

/// @brief Macro to automatically register a deferred early hook installation
/// function.
/// @param name_ The name of the hook to be installed.
#define MAKE_EARLY_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                                                                                   \
    __attribute((constructor)) void Hook_##name_##_Auto_Register() {                                                                     \
        ::AudicaHook::Utils::Hooking::DeferredHooks::addInstallFunc(::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Early, []() { \
            INSTALL_HOOK(name_, ::AudicaHook::Utils::Hooking::DeferredHooks::logger);                                                    \
        });                                                                                                                              \
    }

/// @brief Macro to automatically register a deferred early hook installation
/// function.
/// @param name_ The name of the hook to be installed.
#define MAKE_LATE_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                                                                                   \
    __attribute((constructor)) void Hook_##name_##_Auto_Register() {                                                                    \
        ::AudicaHook::Utils::Hooking::DeferredHooks::addInstallFunc(::AudicaHook::Utils::Hooking::DeferredHooks::HookType::Late, []() { \
            INSTALL_HOOK(name_, ::AudicaHook::Utils::Hooking::DeferredHooks::logger);                                                   \
        });                                                                                                                             \
    }

#define MAKE_HOOK_OFFSET(name_, addr, retval, ...)                 \
    struct Hook_##name_ {                                          \
        static ::AudicaHook::Utils::Hooking::HookType type() {     \
            return ::AudicaHook::Utils::Hooking::HookType::Offset; \
        }                                                          \
        using funcType = retval (*)(__VA_ARGS__);                  \
        constexpr static const char* name() {                      \
            return #name_;                                         \
        }                                                          \
        static void* addr() {                                      \
            return (void*) addr;                                   \
        }                                                          \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;      \
        static retval hook_##name_(__VA_ARGS__);                   \
        static funcType* trampoline() {                            \
            return &name_;                                         \
        }                                                          \
        static funcType hook() {                                   \
            return &hook_##name_;                                  \
        }                                                          \
    };                                                             \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_HOOK_OFFSETLESS(name_, methodInfo, retval, ...)           \
    struct Hook_##name_ {                                              \
        static ::AudicaHook::Utils::Hooking::HookType type() {         \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless; \
        }                                                              \
        using funcType = retval (*)(__VA_ARGS__);                      \
        constexpr static const char* name() {                          \
            return #name_;                                             \
        }                                                              \
        static void* addr() {                                          \
            return (void*) methodInfo->methodPointer;                  \
        }                                                              \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;          \
        static retval hook_##name_(__VA_ARGS__);                       \
        static funcType hook() {                                       \
            static funcType* trampoline() {                            \
                return &name_;                                         \
            }                                                          \
            return &hook_##name_;                                      \
        }                                                              \
    };                                                                 \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_HOOK_NATIVE(name_, addr, retval, ...)                 \
    struct Hook_##name_ {                                          \
        static ::AudicaHook::Utils::Hooking::HookType type() {     \
            return ::AudicaHook::Utils::Hooking::HookType::Native; \
        }                                                          \
        using funcType = retval (*)(__VA_ARGS__);                  \
        constexpr static const char* name() {                      \
            return #name_;                                         \
        }                                                          \
        static void* addr() {                                      \
            return (void*) addr;                                   \
        }                                                          \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;      \
        static retval hook_##name_(__VA_ARGS__);                   \
        static funcType* trampoline() {                            \
            return &name_;                                         \
        }                                                          \
        static funcType hook() {                                   \
            return &hook_##name_;                                  \
        }                                                          \
    };                                                             \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_HOOK_FIND(name_, namespace_, class_, method_, retval, ...)                                                    \
    struct Hook_##name_ {                                                                                                  \
        static ::AudicaHook::Utils::Hooking::HookType type() {                                                             \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless;                                                     \
        }                                                                                                                  \
        using funcType = retval (*)(__VA_ARGS__);                                                                          \
        constexpr static const char* name() {                                                                              \
            return #name_;                                                                                                 \
        }                                                                                                                  \
        static void* addr() {                                                                                              \
            return (void*) ::il2cpp_utils::GetMethod(namespace_, class_, method_, COUNT_ARGS(__VA_ARGS__))->methodPointer; \
        }                                                                                                                  \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                              \
        static retval hook_##name_(__VA_ARGS__);                                                                           \
        static funcType* trampoline() {                                                                                    \
            return &name_;                                                                                                 \
        }                                                                                                                  \
        static funcType hook() {                                                                                           \
            return &hook_##name_;                                                                                          \
        }                                                                                                                  \
    };                                                                                                                     \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_HOOK_FIND_INSTANCE(name_, namespace_, class_, method_, retval, ...)                                                 \
    struct Hook_##name_ {                                                                                                        \
        static ::AudicaHook::Utils::Hooking::HookType type() {                                                                   \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless;                                                           \
        }                                                                                                                        \
        using funcType = retval (*)(__VA_ARGS__);                                                                                \
        constexpr static const char* name() {                                                                                    \
            return #name_;                                                                                                       \
        }                                                                                                                        \
        static void* addr() {                                                                                                    \
            return (void*) ::il2cpp_utils::GetMethod(namespace_, class_, method_, (COUNT_ARGS(__VA_ARGS__) - 1))->methodPointer; \
        }                                                                                                                        \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                                    \
        static retval hook_##name_(__VA_ARGS__);                                                                                 \
        static funcType* trampoline() {                                                                                          \
            return &name_;                                                                                                       \
        }                                                                                                                        \
        static funcType hook() {                                                                                                 \
            return &hook_##name_;                                                                                                \
        }                                                                                                                        \
    };                                                                                                                           \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Auto Hook Macros
#define MAKE_EARLY_HOOK_OFFSET(name_, addr, retval, ...)           \
    struct Hook_##name_ {                                          \
        static ::AudicaHook::Utils::Hooking::HookType type() {     \
            return ::AudicaHook::Utils::Hooking::HookType::Offset; \
        }                                                          \
        using funcType = retval (*)(__VA_ARGS__);                  \
        constexpr static const char* name() {                      \
            return #name_;                                         \
        }                                                          \
        static void* addr() {                                      \
            return (void*) addr;                                   \
        }                                                          \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;      \
        static retval hook_##name_(__VA_ARGS__);                   \
        static funcType* trampoline() {                            \
            return &name_;                                         \
        }                                                          \
        static funcType hook() {                                   \
            return &hook_##name_;                                  \
        }                                                          \
    };                                                             \
    MAKE_EARLY_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                 \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_EARLY_HOOK_OFFSETLESS(name_, methodInfo, retval, ...)     \
    struct Hook_##name_ {                                              \
        static ::AudicaHook::Utils::Hooking::HookType type() {         \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless; \
        }                                                              \
        using funcType = retval (*)(__VA_ARGS__);                      \
        constexpr static const char* name() {                          \
            return #name_;                                             \
        }                                                              \
        static void* addr() {                                          \
            return (void*) methodInfo->methodPointer;                  \
        }                                                              \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;          \
        static retval hook_##name_(__VA_ARGS__);                       \
        static funcType hook() {                                       \
            static funcType* trampoline() {                            \
                return &name_;                                         \
            }                                                          \
            return &hook_##name_;                                      \
        }                                                              \
    };                                                                 \
    MAKE_EARLY_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                     \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_EARLY_HOOK_NATIVE(name_, addr, retval, ...)           \
    struct Hook_##name_ {                                          \
        static ::AudicaHook::Utils::Hooking::HookType type() {     \
            return ::AudicaHook::Utils::Hooking::HookType::Native; \
        }                                                          \
        using funcType = retval (*)(__VA_ARGS__);                  \
        constexpr static const char* name() {                      \
            return #name_;                                         \
        }                                                          \
        static void* addr() {                                      \
            return (void*) addr;                                   \
        }                                                          \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;      \
        static retval hook_##name_(__VA_ARGS__);                   \
        static funcType* trampoline() {                            \
            return &name_;                                         \
        }                                                          \
        static funcType hook() {                                   \
            return &hook_##name_;                                  \
        }                                                          \
    };                                                             \
    MAKE_EARLY_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                 \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_EARLY_HOOK_FIND(name_, namespace_, class_, method_, retval, ...)                                              \
    struct Hook_##name_ {                                                                                                  \
        static ::AudicaHook::Utils::Hooking::HookType type() {                                                             \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless;                                                     \
        }                                                                                                                  \
        using funcType = retval (*)(__VA_ARGS__);                                                                          \
        constexpr static const char* name() {                                                                              \
            return #name_;                                                                                                 \
        }                                                                                                                  \
        static void* addr() {                                                                                              \
            return (void*) ::il2cpp_utils::GetMethod(namespace_, class_, method_, COUNT_ARGS(__VA_ARGS__))->methodPointer; \
        }                                                                                                                  \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                              \
        static retval hook_##name_(__VA_ARGS__);                                                                           \
        static funcType* trampoline() {                                                                                    \
            return &name_;                                                                                                 \
        }                                                                                                                  \
        static funcType hook() {                                                                                           \
            return &hook_##name_;                                                                                          \
        }                                                                                                                  \
    };                                                                                                                     \
    MAKE_EARLY_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                                                                         \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_EARLY_HOOK_FIND_INSTANCE(name_, namespace_, class_, method_, retval, ...)                                           \
    struct Hook_##name_ {                                                                                                        \
        static ::AudicaHook::Utils::Hooking::HookType type() {                                                                   \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless;                                                           \
        }                                                                                                                        \
        using funcType = retval (*)(__VA_ARGS__);                                                                                \
        constexpr static const char* name() {                                                                                    \
            return #name_;                                                                                                       \
        }                                                                                                                        \
        static void* addr() {                                                                                                    \
            return (void*) ::il2cpp_utils::GetMethod(namespace_, class_, method_, (COUNT_ARGS(__VA_ARGS__) - 1))->methodPointer; \
        }                                                                                                                        \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                                    \
        static retval hook_##name_(__VA_ARGS__);                                                                                 \
        static funcType* trampoline() {                                                                                          \
            return &name_;                                                                                                       \
        }                                                                                                                        \
        static funcType hook() {                                                                                                 \
            return &hook_##name_;                                                                                                \
        }                                                                                                                        \
    };                                                                                                                           \
    MAKE_EARLY_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                                                                               \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_LATE_HOOK_OFFSET(name_, addr, retval, ...)            \
    struct Hook_##name_ {                                          \
        static ::AudicaHook::Utils::Hooking::HookType type() {     \
            return ::AudicaHook::Utils::Hooking::HookType::Offset; \
        }                                                          \
        using funcType = retval (*)(__VA_ARGS__);                  \
        constexpr static const char* name() {                      \
            return #name_;                                         \
        }                                                          \
        static void* addr() {                                      \
            return (void*) addr;                                   \
        }                                                          \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;      \
        static retval hook_##name_(__VA_ARGS__);                   \
        static funcType* trampoline() {                            \
            return &name_;                                         \
        }                                                          \
        static funcType hook() {                                   \
            return &hook_##name_;                                  \
        }                                                          \
    };                                                             \
    MAKE_LATE_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                  \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_LATE_HOOK_OFFSETLESS(name_, methodInfo, retval, ...)      \
    struct Hook_##name_ {                                              \
        static ::AudicaHook::Utils::Hooking::HookType type() {         \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless; \
        }                                                              \
        using funcType = retval (*)(__VA_ARGS__);                      \
        constexpr static const char* name() {                          \
            return #name_;                                             \
        }                                                              \
        static void* addr() {                                          \
            return (void*) methodInfo->methodPointer;                  \
        }                                                              \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;          \
        static retval hook_##name_(__VA_ARGS__);                       \
        static funcType hook() {                                       \
            static funcType* trampoline() {                            \
                return &name_;                                         \
            }                                                          \
            return &hook_##name_;                                      \
        }                                                              \
    };                                                                 \
    MAKE_LATE_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                      \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_LATE_HOOK_NATIVE(name_, addr, retval, ...)            \
    struct Hook_##name_ {                                          \
        static ::AudicaHook::Utils::Hooking::HookType type() {     \
            return ::AudicaHook::Utils::Hooking::HookType::Native; \
        }                                                          \
        using funcType = retval (*)(__VA_ARGS__);                  \
        constexpr static const char* name() {                      \
            return #name_;                                         \
        }                                                          \
        static void* addr() {                                      \
            return (void*) addr;                                   \
        }                                                          \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;      \
        static retval hook_##name_(__VA_ARGS__);                   \
        static funcType* trampoline() {                            \
            return &name_;                                         \
        }                                                          \
        static funcType hook() {                                   \
            return &hook_##name_;                                  \
        }                                                          \
    };                                                             \
    MAKE_LATE_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                  \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_LATE_HOOK_FIND(name_, namespace_, class_, method_, retval, ...)                                               \
    struct Hook_##name_ {                                                                                                  \
        static ::AudicaHook::Utils::Hooking::HookType type() {                                                             \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless;                                                     \
        }                                                                                                                  \
        using funcType = retval (*)(__VA_ARGS__);                                                                          \
        constexpr static const char* name() {                                                                              \
            return #name_;                                                                                                 \
        }                                                                                                                  \
        static void* addr() {                                                                                              \
            return (void*) ::il2cpp_utils::GetMethod(namespace_, class_, method_, COUNT_ARGS(__VA_ARGS__))->methodPointer; \
        }                                                                                                                  \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                              \
        static retval hook_##name_(__VA_ARGS__);                                                                           \
        static funcType* trampoline() {                                                                                    \
            return &name_;                                                                                                 \
        }                                                                                                                  \
        static funcType hook() {                                                                                           \
            return &hook_##name_;                                                                                          \
        }                                                                                                                  \
    };                                                                                                                     \
    MAKE_LATE_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                                                                          \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_LATE_HOOK_FIND_INSTANCE(name_, namespace_, class_, method_, retval, ...)                                            \
    struct Hook_##name_ {                                                                                                        \
        static ::AudicaHook::Utils::Hooking::HookType type() {                                                                   \
            return ::AudicaHook::Utils::Hooking::HookType::Offsetless;                                                           \
        }                                                                                                                        \
        using funcType = retval (*)(__VA_ARGS__);                                                                                \
        constexpr static const char* name() {                                                                                    \
            return #name_;                                                                                                       \
        }                                                                                                                        \
        static void* addr() {                                                                                                    \
            return (void*) ::il2cpp_utils::GetMethod(namespace_, class_, method_, (COUNT_ARGS(__VA_ARGS__) - 1))->methodPointer; \
        }                                                                                                                        \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                                    \
        static retval hook_##name_(__VA_ARGS__);                                                                                 \
        static funcType* trampoline() {                                                                                          \
            return &name_;                                                                                                       \
        }                                                                                                                        \
        static funcType hook() {                                                                                                 \
            return &hook_##name_;                                                                                                \
        }                                                                                                                        \
    };                                                                                                                           \
    MAKE_LATE_HOOK_INSTALL_WITH_AUTOLOGGER(name_)                                                                                \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)
