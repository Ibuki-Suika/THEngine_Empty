#ifndef THIMAGE_H
#define THIMAGE_H

#include <Common\THCommon.h>

namespace THEngine
{
	class Image : public Object
	{
	protected:
		int width, height;
		unsigned char* data;

	private:
		bool LoadJPG(const char* filePath);
		bool LoadPNG(const char* filePath);

	public:
		Image();
		Image(int width, int height);
		virtual ~Image();

		static Ptr<Image> Load(const String& filePath);
		
		inline int GetWidth() { return width; }
		inline int GetHeight() { return height; }
		inline unsigned char* GetData() { return data; }
	};
}

#endif