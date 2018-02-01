#ifndef THSTRING_H
#define THSTRING_H

#include <Windows.h>
#include <string>

namespace THEngine
{
	class String
	{
	private:
		TCHAR* buffer = nullptr;
		int length = 0;
	public:
		String();
		String(const TCHAR* str);
		String(const char* str);
		String(const String& str);
		~String();

		inline const TCHAR* GetBuffer() const
		{
			return buffer;
		}

		inline int GetLength() const
		{
			return length;
		}

		std::string ToStdString() const;

		int LastIndexOf(TCHAR ch) const;
		String SubString(int start, int end) const;

		String operator +(const String& str) const;
		String operator +(int number) const;
		void operator =(const String& str);
		const TCHAR& operator[](int number) const;
		bool operator ==(const String& str) const;
	};
}

#endif