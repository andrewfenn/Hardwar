// Copyright (c) 2008,2009,2010,2011 Carlo Contavalli (ccontavalli@gmail.com).
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY Carlo Contavalli ''AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL Carlo Contavalli OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are
// those of the authors and should not be interpreted as representing official
// policies, either expressed or implied, of Carlo Contavalli.

#ifndef LIBYAARG_BASE_H
# define LIBYAARG_BASE_H

# include <stdint.h>

using namespace std;

namespace std {
namespace tr1 {
}
}
using namespace std::tr1;

// Helpers to deal with the c++0x transition, some things are in std or tr1
// depending on compiler flags or version.
#ifdef __GXX_EXPERIMENTAL_CXX0X__
# include <functional>
# include <unordered_map>
# include <unordered_set>
#else
# include <tr1/functional>
# include <tr1/unordered_map>
# include <tr1/unordered_set>
#endif

#ifndef BIT
# define BIT(bit) (1<<(bit))
#endif

// This is roughly equivalent to strnlen on GNU systems.
static inline long int strmaxlen(const char* str, int len) {
  const char* end = str + len;
  for (; *str && str < end; ++str)
    ;
  return end - str;
}
#endif /* LIBYAARG_BASE_H */
