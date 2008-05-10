// Thread++ Library
// Copyright (c) 2008 Urs C. Hanselmann
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef THREADPP_PLATFORM_H
#define THREADPP_PLATFORM_H

#define THREADPP_PLATFORM_UNIX 1
#define THREADPP_PLATFORM_WINDOWS 2

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(_WINDOWS)
#	define THREADPP_PLATFORM THREADPP_PLATFORM_WINDOWS
#else
#	define THREADPP_PLATFORM THREADPP_PLATFORM_UNIX
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define THREADPP_INLINE __attribute__((always_inline)) inline
#else
#    define THREADPP_INLINE inline
#endif

#endif
