#ifndef THFONT_H
#define THFONT_H

#include <Common\THCommon.h>
#include <map>
#include <Math\THVector.h>

namespace THEngine
{
	class Texture;

	class Font : public Object
	{
	private:
		Ptr<Texture> texture;
		std::map<char, Rect> charset;
		//HashMap<char, Rect> charset;

	public:
		Font();
		virtual ~Font();
		
		static Ptr<Font> CreateFontFromFile(const String& ImagePath, const String& txtPath);

		void DrawString(const String& text,float x,float y);
	};
}

#endif