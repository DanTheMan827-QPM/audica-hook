// thx https://github.com/jbro129/Unity-Substrate-Hook-Android

#include "utils/utils.hpp"

#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include "utils/export.hpp"

using namespace std;

namespace AudicaHook::Utils {
    HOOK_EXPORT long long baseAddr(char const* soname
    )  // credits to https://github.com/ikoz/AndroidSubstrate_hookingC_examples/blob/master/nativeHook3/jni/nativeHook3.cy.cpp
    {
        void* imagehandle = dlopen(soname, RTLD_LOCAL | RTLD_LAZY);
        if (soname == NULL) {
            return (long long) NULL;
        }
        if (imagehandle == NULL) {
            return (long long) NULL;
        }
        uintptr_t* irc = NULL;
        FILE* f = NULL;
        char line[200] = {0};
        char* state = NULL;
        char* tok = NULL;
        char* baseAddr = NULL;
        if ((f = fopen("/proc/self/maps", "r")) == NULL) {
            return (long long) NULL;
        }
        while (fgets(line, 199, f) != NULL) {
            tok = strtok_r(line, "-", &state);
            baseAddr = tok;
            tok = strtok_r(NULL, "\t ", &state);
            tok = strtok_r(NULL, "\t ", &state);  // "r-xp" field
            tok = strtok_r(NULL, "\t ", &state);  // "0000000" field
            tok = strtok_r(NULL, "\t ", &state);  // "01:02" field
            tok = strtok_r(NULL, "\t ", &state);  // "133224" field
            tok = strtok_r(NULL, "\t ", &state);  // path field

            if (tok != NULL) {
                int i;
                for (i = (int) strlen(tok) - 1; i >= 0; --i) {
                    if (!(tok[i] == ' ' || tok[i] == '\r' || tok[i] == '\n' || tok[i] == '\t')) {
                        break;
                    }
                    tok[i] = 0;
                }
                {
                    size_t toklen = strlen(tok);
                    size_t solen = strlen(soname);
                    if (toklen > 0) {
                        if (toklen >= solen && strcmp(tok + (toklen - solen), soname) == 0) {
                            fclose(f);
                            return (long long) strtoll(baseAddr, NULL, 16);
                        }
                    }
                }
            }
        }
        fclose(f);
        return (long long) NULL;
    }

    HOOK_EXPORT long long getRealOffset(void* offset)  // calculate dump.cs address + lib.so base address.
    {
        static long long location = 0;

        if (location == 0) {
            // arm
            location = baseAddr(IL2CPP_SO_PATH);  // replace the com.package.name with the package name of the app you are modding.
        }
        return location + (long long) offset;
    }

    HOOK_EXPORT u16string_view csstrtostr(Il2CppString* in) {
        return {in->chars, static_cast<uint32_t>(in->length)};
    }

    HOOK_EXPORT void setcsstr(Il2CppString* in, u16string_view str) {
        in->length = str.length();
        for (int i = 0; i < in->length; i++) {
            // Can assume that each char is only a single char (a single word --> double word)
            in->chars[i] = str[i];
        }
    }

    HOOK_EXPORT string to_utf8(u16string_view view) {
        char dat[view.length() + 1];
        transform(view.data(), view.data() + view.size(), dat, [](auto utf16_char) {
            return static_cast<char>(utf16_char);
        });
        dat[view.length()] = '\0';
        return {dat};
    }

    HOOK_EXPORT u16string to_utf16(string_view view) {
        char16_t dat[view.length() + 1];
        transform(view.data(), view.data() + view.size(), dat, [](auto standardChar) {
            return static_cast<char16_t>(standardChar);
        });
        dat[view.length()] = '\0';
        return {dat};
    }

    HOOK_EXPORT void dump(int before, int after, void* ptr) {
        // Logger.debug("Dumping Immediate Pointer: {}: {}", ptr, (uintptr_t) ptr);
        auto begin = static_cast<int*>(ptr) - before;
        auto end = static_cast<int*>(ptr) + after;
        for (auto cur = begin; cur != end; ++cur) {
            // Logger.debug("{}: {}", (void*) cur, *cur);
        }
    }

}  // namespace AudicaHook::Utils
