#pragma once

#include <string>
#include <type_traits>

#include "../audica.h"
#include "il2cpp-utils.hpp"
#include "utils-methods.hpp"

namespace AudicaHook::Utils::Strings {
    template <typename T>
    requires(std::is_same_v<T, Il2CppString*> || std::is_same_v<T, System_String_o*>)
    static inline std::string il2cpp_to_std(T il2cpp_str) {
        return ::AudicaHook::Utils::to_utf8(::AudicaHook::Utils::csstrtostr((T) (il2cpp_str)));
    }

    template <typename T = Il2CppString*>
    requires(std::is_same_v<T, Il2CppString*> || std::is_same_v<T, System_String_o*>)
    static inline T std_to_il2cpp(std::string const& std_str) {
        return (T) il2cpp_utils::createcsstr((std_str));
    }
}  // namespace AudicaHook::Utils::Strings
