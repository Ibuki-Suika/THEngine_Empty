#include "THCoreDumper.h"
#include <DbgHelp.h>
#include <strsafe.h>
#include <Core\THGame.h>

namespace THEngine
{
	CoreDumper* CoreDumper::instance = nullptr;

	CoreDumper::CoreDumper()
	{
		ASSERT(instance == nullptr);
		SetUnhandledExceptionFilter(UnhandledExceptionFilter);
		instance = this;
	}

	CoreDumper::~CoreDumper()
	{
		instance = nullptr;
	}

	LONG WINAPI CoreDumper::UnhandledExceptionFilter(_EXCEPTION_POINTERS* exception)
	{
		if (instance)
		{
			WCHAR fileName[MAX_PATH];
			if (instance->Dump(exception, fileName))
			{
				THMessageBox((String)"啊哦，程序崩溃了>_<\n崩溃信息已写入程序所在目录dump文件夹下的" + (String)fileName
					+ "文件中。请将该文件发送至SZ_Silence06@foxmail.com");
			}
			else
			{
				THMessageBox("啊哦，程序崩溃了>_<");
			}
			return EXCEPTION_EXECUTE_HANDLER;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	bool CoreDumper::Dump(_EXCEPTION_POINTERS* exception, WCHAR* fileName)
	{
		SYSTEMTIME localTime;

		GetLocalTime(&localTime);

		String dir = "dump";

		WIN32_FIND_DATAW wfd;
		HANDLE handle = FindFirstFile(dir.GetBuffer(), &wfd);
		if (handle == INVALID_HANDLE_VALUE || ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
		{
			//dump目录不存在,创建之
			CreateDirectory(dir.GetBuffer(), NULL);
		}

		WCHAR path[MAX_PATH];
		StringCchPrintf(fileName, MAX_PATH, L"%s-%04d%02d%02d-%02d%02d%02d.dmp",
			Game::GetInstance()->GetTitle().GetBuffer(),
			localTime.wYear, localTime.wMonth, localTime.wDay,
			localTime.wHour, localTime.wMinute, localTime.wSecond);

		StringCchPrintf(path, MAX_PATH, L"%s\\%s", dir.GetBuffer(), fileName);

		HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		//Now the dmp file has been opened, set informations.
		MINIDUMP_EXCEPTION_INFORMATION information;
		information.ThreadId = GetCurrentThreadId();
		information.ExceptionPointers = exception;
		information.ClientPointers = TRUE;

		TH_LOCK(section)
		{
			if (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &information, nullptr, nullptr))
			{
				return true;
			}
		}

		return false;
	}
}