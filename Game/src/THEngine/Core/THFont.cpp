#include "THFont.h"
#include "THSprite.h"
#include "../Asset/THAssetManager.h"
#include <fstream>
#include <sstream>

using namespace THEngine;

Font::Font()
{

}

Font::~Font()
{
	TH_SAFE_RELEASE(texture);
}

Font* Font::CreateFontFromFile(String imagePath, String txtPath)
{
	std::string s = txtPath.ToStdString();

	Font* font = new Font();
	font->texture = AssetManager::GetInstance()->CreateTextureFromFile(imagePath);
	font->texture->Retain();

	if (font->texture == nullptr)
	{
		delete font;
		return nullptr;
	}

	std::ifstream in;
	in.open(s.c_str());
	while (in)
	{
		char buffer[200];
		in.getline(buffer, sizeof(buffer));

		std::stringstream ss;
		ss << buffer;

		char c;
		int left, top, width, height;
		ss >> c >> left >> top >> width >> height;

		Rect rc;
		rc.left = left;
		rc.right = left + width;
		rc.top = top;
		rc.bottom = top + height;

		font->charset.insert(std::pair<char,Rect>(c,rc));
	}
	in.close();
	return font;
}

void Font::DrawString(String text,float x,float y)
{
	Sprite* sprite = new Sprite();
	for (int i = 0; i < text.GetLength(); i++)
	{
		TCHAR c = text[i];
		char ascii[3];
		WideCharToMultiByte(CP_ACP, 0, &c, 1, ascii, sizeof(ascii), nullptr, nullptr);
		auto iter = charset.find(ascii[0]);
		if (iter == charset.end()) continue;
		Rect rc = iter->second;

		sprite->SetTexture(texture);
		sprite->SetTexRect(rc);
		sprite->SetPosition(Vector3f(x, y, 0));
		sprite->Draw();
		x += rc.Width();
	}
	delete sprite;
}