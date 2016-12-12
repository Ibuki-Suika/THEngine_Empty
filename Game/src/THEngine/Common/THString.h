#ifndef THSTRING_H
#define THSTRING_H

#include <Windows.h>
#include <string>

namespace THEngine
{
	class String
	{
	private:
		TCHAR* buffer;
		int length;
	public:
		String();
		String(const TCHAR* str);
		String(const char* str);
		String(const String& str);
		~String();

		inline TCHAR* GetBuffer()
		{
			return buffer;
		}

		inline int GetLength()
		{
			return length;
		}

		std::string ToStdString();

		int LastIndexOf(TCHAR ch);
		String SubString(int start, int end);

		String operator +(const String& str);
		String operator +(int number);
		void operator =(const String& str);
		TCHAR& operator[](int number);
		bool operator ==(const String& str);
	};
}

#endif