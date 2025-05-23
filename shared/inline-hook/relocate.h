#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __aarch64__
#include <stdio.h>

    void relocateInstruction(
        uint32_t target_addr,
        void* orig_instructions,
        int length,
        void* trampoline_instructions,
        int* orig_boundaries,
        int* trampoline_boundaries,
        int* count
    );

#endif

#ifdef __cplusplus
}
#endif
