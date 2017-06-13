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
		Texture* texture;
		std::map<char, Rect> charset;
		//HashMap<char, Rect> charset;

	public:
		Font();
		virtual ~Font();
		
		static Font* CreateFontFromFile(String ImagePath,String txtPath);

		void DrawString(String text,float x,float y);
	};
}

#endif