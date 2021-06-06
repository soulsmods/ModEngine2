#pragma once

#include "modengine/hook_set.h"
#include "gametypes/dantelion/dlstring.h"

#include <set>
#include <concurrent_vector.h>

namespace modengine::ext {

// String type used in DS2/DS3
typedef struct
{
    wchar_t* string;
    void* unk;
    UINT64 length;
    UINT64 capacity;
} dlstring_t;

// String type used in Sekiro
typedef struct
{
    void* unk;
    dlstring_t string;
} dlstring_sekiro_t;

typedef HANDLE(WINAPI* fpCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

void* __cdecl virtual_to_archive_path_ds3(DLString<modengine::GameType::DS3, wchar_t>* path, UINT64 p2, UINT64 p3, DLString<modengine::GameType::DS3, wchar_t>* p4, UINT64 p5, UINT64 p6);
void* __cdecl virtual_to_archive_path_ds2(LPVOID p1, dlstring_t* path);
void* __cdecl virtual_to_archive_path_sekiro(dlstring_sekiro_t* path, UINT64 p2, UINT64 p3, dlstring_sekiro_t* p4, UINT64 p5, UINT64 p6);

extern Hook<fpCreateFileW> hooked_CreateFileW;
extern Hook<decltype(&virtual_to_archive_path_ds3)> hooked_virtual_to_archive_path_ds3;
extern Hook<decltype(&virtual_to_archive_path_ds2)> hooked_virtual_to_archive_path_ds2;
extern Hook<decltype(&virtual_to_archive_path_sekiro)> hooked_virtual_to_archive_path_sekiro;
extern concurrency::concurrent_vector<std::wstring> hooked_file_roots;


HANDLE WINAPI tCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

}