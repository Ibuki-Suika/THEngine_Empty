#include "THString.h"
#include "THCommon.h"
#include <string>
#include <strstream>

using namespace THEngine;

String::String()
{

}

String::String(const TCHAR* str)
{
	length = lstrlen(str);
	if (buffer)
	{
		delete[] buffer;
	}
	buffer = new TCHAR[length + 1];
	lstrcpy(buffer, str);
}

String::String(const char* str)
{
	length = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0) - 1;
	if (buffer)
	{
		delete[] buffer;
	}
	buffer = new TCHAR[length + 1];	
	MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, length + 1);
}

String::String(const String& str)
{
	length = str.length;
	if (buffer)
	{
		delete[] buffer;
	}
	buffer = new TCHAR[length + 1];
	lstrcpy(buffer, str.buffer);
}

String::~String()
{
	if (buffer)
	{
		delete[] buffer;
	}
}

std::string String::ToStdString() const
{
	int requiredSize = WideCharToMultiByte(CP_ACP, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
	char* outBuffer = new char[requiredSize + 1];
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, outBuffer, requiredSize + 1, nullptr, nullptr);

	std::string ret = outBuffer;
	delete[] outBuffer;
	return ret;
}

int String::LastIndexOf(TCHAR ch) const
{
	int i;
	for (i = length - 1; i >= 0; i--)
	{
		if (buffer[i] == ch)
		{
			break;
		}
	}
	return i;
}

String String::SubString(int start, int end) const
{
	ASSERT(start >= 0 && end <= length && start < end);
	String ret;
	ret.length = end - start;
	ret.buffer = new TCHAR[length + 1];
	for (int i = 0; i < ret.length; i++)
	{
		ret.buffer[i] = buffer[start + i];
	}
	ret.buffer[ret.length] = TCHAR('\0');
	return ret;
}

String String::operator + (const String& str)
{
	int newLength = this->length + str.length;
	TCHAR* newBuffer = new TCHAR[newLength + 1];
	lstrcpy(newBuffer, buffer);
	lstrcat(newBuffer, str.buffer);
	
	String ret;
	ret.buffer = newBuffer;
	ret.length = newLength;
	return ret;
}

String String::operator + (int number)
{
	std::string s;
	std::strstream buf;
	buf << number;
	buf >> s;
	std::string ws;
	ws = s;
	return (*this) + ws.c_str();
}

void String::operator = (const String& str)
{
	if (buffer)
	{
		delete[] buffer;
	}
	length = str.length;
	buffer = new TCHAR[length + 1];
	lstrcpy(buffer, str.buffer);
}

TCHAR& String::operator[](int number)
{
	ASSERT(number < length && number >= 0);
	return buffer[number];
}

bool String::operator ==(const String& str)
{
	if (length != str.length)
	{
		return false;
	}
	for (int i = 0; i < length; i++)
	{
		if (buffer[i] != str.buffer[i])
		{
			return false;
		}
	}
	return true;
}