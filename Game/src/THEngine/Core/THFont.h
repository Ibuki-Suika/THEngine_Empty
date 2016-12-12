#ifndef THFONT_H
#define THFONT_H

#include "THSprite.h"
#include <map>

namespace THEngine
{
	class Font : public Object
	{
	private:
		Texture* texture;
		std::map<char, Rect> charset;
		//HashMap<char, Rect> charset;

	public:
		Font();
		
		static Font* CreateFontFromFile(String ImagePath,String txtPath);

		void DrawString(String text,float x,float y);
	};
}

#endif