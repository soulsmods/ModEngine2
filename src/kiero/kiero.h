/*Copyright (c) 2014-2020 Rebzzel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#ifndef __KIERO_H__
#define __KIERO_H__

#include <stdint.h>

#define KIERO_VERSION "1.2.10"

#ifndef KIERO_INCLUDE_D3D9
# define KIERO_INCLUDE_D3D9   0 // 1 if you need D3D9 hook
#endif

#ifndef KIERO_INCLUDE_D3D10
# define KIERO_INCLUDE_D3D10  0 // 1 if you need D3D10 hook
#endif

#ifndef KIERO_INCLUDE_D3D11
# define KIERO_INCLUDE_D3D11  0 // 1 if you need D3D11 hook
#endif

#ifndef KIERO_INCLUDE_D3D12
# define KIERO_INCLUDE_D3D12  0 // 1 if you need D3D12 hook
#endif

#ifndef KIERO_INCLUDE_OPENGL
# define KIERO_INCLUDE_OPENGL 0 // 1 if you need OpenGL hook
#endif

#ifndef KIERO_INCLUDE_VULKAN
# define KIERO_INCLUDE_VULKAN 0 // 1 if you need Vulkan hook
#endif

#ifndef KIERO_USE_MINHOOK
# define KIERO_USE_MINHOOK    0 // 1 if you will use kiero::bind function with MinHook
#endif

#ifndef KIERO_USE_DETOURS
# define KIERO_USE_DETOURS    0 // 1 if you will use kiero::bind function with Microsoft Detours
#endif

#define KIERO_ARCH_X64 0
#define KIERO_ARCH_X86 0

#if defined(_M_X64)
# undef  KIERO_ARCH_X64
# define KIERO_ARCH_X64 1
#else
# undef  KIERO_ARCH_X86
# define KIERO_ARCH_X86 1
#endif

#if KIERO_ARCH_X64
typedef uint64_t uint150_t;
#else
typedef uint32_t uint150_t;
#endif

namespace kiero
{
struct Status
{
    enum Enum
    {
        UnknownError = -1,
        NotSupportedError = -2,
        ModuleNotFoundError = -3,

        AlreadyInitializedError = -4,
        NotInitializedError = -5,

        Success = 0,
    };
};

struct RenderType
{
    enum Enum
    {
        None,

        D3D9,
        D3D10,
        D3D11,
        D3D12,

        OpenGL,
        Vulkan,

        Auto
    };
};

Status::Enum init(RenderType::Enum renderType);
void shutdown();

Status::Enum bind(uint16_t index, void** original, void* function);
void unbind(uint16_t index);

RenderType::Enum getRenderType();
uint150_t* getMethodsTable();
}

#endif // __KIERO_H__