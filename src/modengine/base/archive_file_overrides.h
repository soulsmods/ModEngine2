#pragma once

#include "modengine/hook_set.h"

namespace modengine::base {

// String type used in DS2/DS3
typedef struct
{
	wchar_t *string;
	void *unk;
	UINT64 length;
	UINT64 capacity;
} dlstring_t;

// String type used in Sekiro
typedef struct
{
	void *unk;
	dlstring_t string;
} dlstring_sekiro_t;

typedef HANDLE(WINAPI *fpCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef void*(*fpVirtualToArchivePathDS3)(dlstring_t*, UINT64, UINT64, dlstring_t*, UINT64, UINT64);
typedef void* (*fpVirtualToArchivePathDS2)(LPVOID, dlstring_t*);
typedef void*(*fpVirtualToArchivePathSekiro)(dlstring_sekiro_t*, UINT64, UINT64, dlstring_sekiro_t*, UINT64, UINT64);

extern std::shared_ptr<Hook<fpCreateFileW>> hooked_CreateFileW;
extern std::shared_ptr<Hook<fpVirtualToArchivePathDS3>> hooked_virtual_to_archive_path_ds3;
extern std::shared_ptr<Hook<fpVirtualToArchivePathDS2>> hooked_virtual_to_archive_path_ds2;
extern std::shared_ptr<Hook<fpVirtualToArchivePathSekiro>> hooked_virtual_to_archive_path_sekiro;

HANDLE WINAPI tCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);
void* __cdecl virtual_to_archive_path_ds3(dlstring_t *path, UINT64 p2, UINT64 p3, dlstring_t *p4, UINT64 p5, UINT64 p6);
void* __cdecl virtual_to_archive_path_ds2(LPVOID p1, dlstring_t* path);
void* __cdecl virtual_to_archive_path_sekiro(dlstring_sekiro_t *path, UINT64 p2, UINT64 p3, dlstring_sekiro_t *p4, UINT64 p5, UINT64 p6);

}