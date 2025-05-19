#pragma once

#include <dlfcn.h>
#include <link.h>
#include <scotland2/shared/modloader.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <scotland2/shared/loader.hpp>
#include <string>
#include <string_view>
#include <type_traits>

#include "logging.hpp"

namespace AudicaHook::Utils::FileSystem {
    using namespace AudicaHook::Logging;

    template <typename T = void>
    requires(std::is_void<T>::value)
    static inline int makePath(std::string_view file_path) {
        return system((std::string("mkdir -p -m +rw ") + file_path.data()).c_str());
    }

    template <typename T = void>
    requires(std::is_void<T>::value)
    inline static bool fileExists(std::string_view filename) {
        return access(filename.data(), W_OK | R_OK) != -1;
    }

    template <typename T = void>
    requires(std::is_void<T>::value)
    static inline bool directoryExists(std::string const& path) {
        struct stat info;

        // Check if the path exists and is a directory
        if (stat(path.c_str(), &info) != 0) {
            return false;  // Path does not exist
        }
        return (info.st_mode & S_IFDIR) != 0;  // Check if it's a directory
    }

    template <typename T = void>
    requires(std::is_void<T>::value)
    inline static std::string getDataDir(char const* mod_id) {
        std::string dataDir = fmt::format("/storage/emulated/0/ModData/{}/Mods/{}/", modloader_get_application_id(), mod_id);
        Logger.info("Data directory: {}", dataDir);
        auto exists = directoryExists(dataDir);

        if (exists) {
            Logger.info("Data directory exists: {}", dataDir);
        }

        if (!exists) {
            Logger.info("Creating data directory: {}", dataDir.c_str());
            if (!makePath(dataDir)) {
                Logger.error("Failed to create data directory: {}", dataDir.c_str());
            }
        }

        return dataDir;
    }
}  // namespace AudicaHook::Utils::FileSystem
