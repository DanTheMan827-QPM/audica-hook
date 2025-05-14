#pragma once

#define HOOK_EXPORT __attribute__((visibility("default")))
#define HOOK_EXPORT_C extern "C" HOOK_EXPORT
