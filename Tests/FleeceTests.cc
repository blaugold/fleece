//
// FleeceTests.cc
//
// Copyright (c) 2015 Couchbase, Inc All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "FleeceTests.hh"
#include "slice.hh"
#include <fcntl.h>

#ifndef _MSC_VER
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define O_BINARY 0
#else
#include <io.h>
#include <windows.h>
#define ssize_t int
#endif


namespace fleece_test {

    std::string sliceToHex(slice result) {
        std::string hex;
        for (size_t i = 0; i < result.size; i++) {
            char str[4];
            sprintf(str, "%02X", result[i]);
            hex.append(str);
            if (i % 2 && i != result.size-1)
                hex.append(" ");
        }
        return hex;
    }


    std::string sliceToHexDump(slice result, size_t width) {
        std::string hex;
        for (size_t row = 0; row < result.size; row += width) {
            size_t end = std::min(row + width, result.size);
            for (size_t i = row; i < end; ++i) {
                char str[4];
                sprintf(str, "%02X", result[i]);
                hex.append(str);
                if (i % 2 && i != result.size-1)
                    hex.append(" ");
            }
            hex.append("    ");
            for (size_t i = row; i < end; ++i) {
                char str[2] = {(char)result[i], 0};
                if (result[i] < 32 || result[i] >= 127)
                    str[0] = '.';
                hex.append(str);
            }
            hex.append("\n");
        }
        return hex;
    }

    std::ostream& dumpSlice(std::ostream& o, slice s) {
        o << "slice[";
        if (s.buf == nullptr)
            return o << "null]";
        auto buf = (const uint8_t*)s.buf;
        for (size_t i = 0; i < s.size; i++) {
            if (buf[i] < 32 || buf[i] > 126)
                return o << sliceToHex(s) << "]";
        }
        return o << "\"" << std::string((char*)s.buf, s.size) << "\"]";
    }

}
