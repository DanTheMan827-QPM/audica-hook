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
#include "../audica.h"
#include "filesystem.hpp"
#include "il2cpp-utils.hpp"
#include "strings.hpp"
#include "typedefs.h"
#include "utils-macros.hpp"
#include "utils-methods.hpp"
