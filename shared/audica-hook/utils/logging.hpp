#pragma once

#include <paper2_scotland2/shared/logger.hpp>

namespace AudicaHook::Logging {
    /// @brief A logger, useful for printing debug messages
    /// @return
    inline constexpr auto Logger = Paper::ConstLoggerContext(MOD_ID "_" VERSION);
}  // namespace AudicaHook::Logging
