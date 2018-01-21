#include "THFont.h"
#include "THSprite.h"
#include "THGame.h"
#include <Platform\THDevice.h>
#include "../Asset/THAssetManager.h"
#include <fstream>
#include <sstream>

using namespace THEngine;

Font::Font()
{
}

Font::~Font()
{
	int test = 1;
}

Ptr<Font> Font::CreateFontFromFile(const String& imagePath, const String& txtPath)
{
	std::string s = txtPath.ToStdString();

	Ptr<Font> font = Ptr<Font>::New();
	font->texture = AssetManager::GetInstance()->CreateTextureFromFile(imagePath);

	if (font->texture == nullptr)
	{
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

		font->charset.insert(std::pair<char, Rect>(c, rc));
	}
	in.close();
	return font;
}

void Font::DrawString(const String& text, float x, float y)
{
	Ptr<Sprite> sprite = Ptr<Sprite>::New();

	//setup transform matrices
	auto game = Game::GetInstance();
	auto device = Device::GetInstance();
	Matrix proj, view;
	Matrix::Ortho(&proj, 0, game->GetWidth(), 0, game->GetHeight(), 0, TH_MAX_Z);
	Matrix::Identity(&view);
	device->SetProjectionMatrix(proj);
	device->SetViewMatrix(view);

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
}