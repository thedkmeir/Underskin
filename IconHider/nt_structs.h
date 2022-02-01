#pragma once
//#include <winternl.h>
//#include <iostream>
//#include <windows.h>
//#include <tlhelp32.h>
//#include <ntdef.h>

typedef struct _IO_STATUS_BLOCK
{
	NTSTATUS Status;
	ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;

typedef enum _FILE_INFORMATION_CLASS
{
	FileDirectoryInformation,
	FileFullDirectoryInformation,
	FileIdFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileIdBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileObjectIdInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileAttributeTagInformation,
	FileTrackingInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef VOID(NTAPI *PIO_APC_ROUTINE)(IN PVOID ApcContext, IN PIO_STATUS_BLOCK   IoStatusBlock, IN ULONG Reserved);

//typedef struct _FILE_BOTH_DIR_INFORMATION
//{
//	ULONG         NextEntryOffset;
//	ULONG         FileIndex;
//	LARGE_INTEGER CreationTime;
//	LARGE_INTEGER LastAccessTime;
//	LARGE_INTEGER LastWriteTime;
//	LARGE_INTEGER ChangeTime;
//	LARGE_INTEGER EndOfFile;
//	LARGE_INTEGER AllocationSize;
//	ULONG         FileAttributes;
//	ULONG         FileNameLength;
//	ULONG         EaSize;
//	UCHAR         ShortNameLength;
//	WCHAR         ShortName[12];
//	WCHAR         FileName[1];
//} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

//typedef struct _FILE_DIRECTORY_INFORMATION
//{
//	ULONG         NextEntryOffset;
//	ULONG         FileIndex;
//	LARGE_INTEGER CreationTime;
//	LARGE_INTEGER LastAccessTime;
//	LARGE_INTEGER LastWriteTime;
//	LARGE_INTEGER ChangeTime;
//	LARGE_INTEGER EndOfFile;
//	LARGE_INTEGER AllocationSize;
//	ULONG         FileAttributes;
//	ULONG         FileNameLength;
//	WCHAR         FileName[1];
//} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

//
typedef struct _FILE_FULL_DIR_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	WCHAR         FileName[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

//typedef struct _FILE_ID_FULL_DIR_INFORMATION
//{
//	ULONG         NextEntryOffset;
//	ULONG         FileIndex;
//	LARGE_INTEGER CreationTime;
//	LARGE_INTEGER LastAccessTime;
//	LARGE_INTEGER LastWriteTime;
//	LARGE_INTEGER ChangeTime;
//	LARGE_INTEGER EndOfFile;
//	LARGE_INTEGER AllocationSize;
//	ULONG         FileAttributes;
//	ULONG         FileNameLength;
//	ULONG         EaSize;
//	LARGE_INTEGER FileId;
//	WCHAR         FileName[1];
//} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;

//
typedef struct _FILE_ID_BOTH_DIR_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	LARGE_INTEGER FileId;
	WCHAR         FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

//typedef struct _FILE_NAMES_INFORMATION
//{
//	ULONG NextEntryOffset;
//	ULONG FileIndex;
//	ULONG FileNameLength;
//	WCHAR FileName[1];
//} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;