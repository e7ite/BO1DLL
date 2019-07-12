#include "menu.h"

std::vector<std::vector<Option>> Menu::options;
int Menu::open;
int Menu::toggled;
int Menu::currentSub;
int Menu::timer;

int Variables::enemyESP;
int Variables::friendlyESP;
int Variables::enableAimbot;
int Variables::aimKey;

void Menu::Build()
{
	using namespace Variables;

	for (int sub = MAIN_MENU; sub <= ESP_MENU; ++sub)
		Menu::options.push_back(std::vector<Option>());

	Menu::Insert(MAIN_MENU, "Aimbot Menu", nullptr,
		TYPE_SUB, std::bind(Menu::LoadSub, AIMBOT_MENU));
	Menu::Insert(MAIN_MENU, "ESP Menu", nullptr,
		TYPE_SUB, std::bind(Menu::LoadSub, ESP_MENU));
	Menu::Insert(MAIN_MENU, "Misc Menu", nullptr,
		TYPE_SUB, std::bind(Menu::LoadSub, MISC_MENU));

	Menu::Insert(ESP_MENU, "Enemy ESP", &enemyESP,
		TYPE_BOOL, std::bind(Menu::BoolModify, &enemyESP));
	Menu::Insert(ESP_MENU, "Friendly ESP", &friendlyESP,
		TYPE_BOOL, std::bind(Menu::BoolModify, &friendlyESP));

	Menu::Insert(AIMBOT_MENU, "Enable Aimbot", &enableAimbot,
		TYPE_BOOL, std::bind(Menu::BoolModify, &enableAimbot));
	Menu::Insert(AIMBOT_MENU, "Aim Key", &aimKey, TYPE_INT,
		std::bind(Menu::IntModify, &aimKey, &Menu::toggled, 0, 2));
}

void Menu::Insert(int sub, const char *option, int *data,
	OptionType type, std::function<void()> callback)
{
	Menu::options.at(sub).push_back(Option(option, data, type, callback));
}

void Menu::LoadSub(int sub)
{
	Menu::currentSub = sub;
}

void Menu::BoolModify(int *var)
{
	*var = !var;
}

bool Menu::Ready()
{
	return Sys_Milliseconds() > Menu::timer;
}

void Menu::Wait(int ms)
{
	Menu::timer = Sys_Milliseconds() + ms;
}

void Menu::Execute()
{
	float menuCenterX = dc->screenDimensions[0] / 2
		/ scrPlace->scaleVirtualToFull[0];
	float menuCenterY = dc->screenDimensions[1] / 2
		/ scrPlace->scaleVirtualToFull[1];
	float optionY = menuCenterY - 100;

	float textWidth, textHeight;
	Font_s *fontPointer;
	float xAligned = AlignText("BO1 DLL By E7ite", normalFont, 0.3f,
		menuCenterX, ALIGN_CENTER, 1, 1, &fontPointer, &textWidth, &textHeight);
	optionY += RenderUITextWithBackground("BO1 DLL By E7ite",
		xAligned, optionY, textWidth, textHeight, Colors::blue,
		Colors::white, fontPointer, 0.3f, true);

	float borderX = menuCenterX - 140, borderY = menuCenterY - 100,
		borderW = menuCenterX - 20,
		borderH = textHeight * Menu::options[Menu::currentSub].size();
	UI_FillRect(scrPlace, borderX, borderY, borderW, borderH, 0, 0,
		Colors::transparentBlack);
	UI_DrawRect(scrPlace, borderX, borderY, borderW, borderH, 0, 0, 2,
		Colors::blue);

	int index = 0;
	for (const auto &i : Menu::options[Menu::currentSub])
	{
		Colors::Color color = Colors::white;

		optionY += RenderUIText(i.option, borderX,
			optionY, 0.3f, Colors::blue, fontPointer);

		if (MonitorMouse(i, index++, borderX, optionY, borderW, borderH))
			color = Colors::blue;

		switch (i.type)
		{
		case TYPE_BOOL:
			UI_FillRect(scrPlace, borderX, optionY, 15, 15,
				0, 0, *i.data ? Colors::blue : Colors::transparentBlack);
			break;
		case TYPE_INT:
			std::string data = std::to_string(*i.data);
			RenderUIText(data.data(), AlignText(data.data(), normalFont,
				0.3f, menuCenterX + 20, ALIGN_RIGHT, 1, 0),
				optionY, 0.3f, color, fontPointer);
			break;
		}
	}
}

void Menu::IntModify(int *var, int *increment, int min, int max)
{
	if (*increment)
		++(*var);
	else
		--(*var);

	if (*var > max)
		*var = min;
	if (*var < min)
		*var = max;
}

bool Menu::MonitorMouse(const Option &opt, int optPos, float optionX, float optionY,
	float optionW, float optionH)
{
	if (dc->cursorPos[0] > optionX && dc->cursorPos[0] < optionW
		&& dc->cursorPos[1] > optionY - (optionH - optionY)
		&& dc->cursorPos[1] < optionY)
	{

		if (Menu::Ready())
			if (GetAsyncKeyState(VK_LBUTTON) & 0x10000)
			{
				opt.callback();
				Menu::Wait(200);
			}
			else if (GetAsyncKeyState(VK_RBUTTON) & 0x10000)
			{
				opt.callback();
				Menu::Wait(200);
			}

		return true;
	}

	return false;
}

void RenderShader(float x, float y, float width, float height, float angle,
	const float *color, const char *material, int type)
{
	CG_DrawRotatedPicPhysical(scrPlace, x, y, width, height, angle, color,
		Material_RegisterHandle(material, type));
}

void DrawFillRect(float x, float y, float width, float height,
	const float *color, float rotation, int type)
{
	RenderShader(x, y, width, height, rotation, color, "white", type);
}

void DrawEmptyRect(float x, float y, float width, float height, float size,
	const float *color, int type)
{
	RenderShader(x, y, width, size, 0, color, "white", type); //up
	RenderShader(x, y + height, width + (size - 1), size, 0,  //down
		color, "white", type);
	RenderShader(x, y, size, height + (size - 1), 0, color,   //left
		"white", type);
	RenderShader(x + width, y, size, height + size, 0, color, //right
		"white", type);
}

float AlignText(const char *text, const Fonts &font, float scale, float initX,
	ScreenAlignment align, bool ui, bool bg, Font_s **fOut,
	float *wOut, float *hOut)
{
	Font_s *fontPointer;
	float width, height;
	float xAligned;

	if (bg && ui)
	{
		fontPointer = UI_GetFontHandle(scrPlace, font.index, scale);
		width = (float)UI_TextWidth(text, INT_MAX, fontPointer, scale) + 12.0f;
		height = (float)UI_TextHeight(fontPointer, scale) + 2.0f;
	}
	else if (!bg && ui)
	{
		fontPointer = UI_GetFontHandle(scrPlace, font.index, scale);
		width = (float)UI_TextWidth(text, INT_MAX, fontPointer, scale);
		height = (float)UI_TextHeight(fontPointer, scale) + 2.0f;
	}
	else if (bg && !ui)
	{
		fontPointer = CL_RegisterFont(font.dir, 0);
		width = (float)R_TextWidth(text, INT_MAX, fontPointer) * scale + 14.0f;
		height = (float)R_TextHeight(fontPointer) * scale + 2.0f;
	}
	else
	{
		fontPointer = CL_RegisterFont(font.dir, 0);
		width = (float)R_TextWidth(text, INT_MAX, fontPointer);
		height = (float)R_TextHeight(fontPointer) * scale + 2.0f;
	}

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = initX;
		break;
	case ALIGN_CENTER:
		xAligned = initX - width / 2;
		break;
	case ALIGN_RIGHT:
		xAligned = initX - width;
		break;
	default:
		xAligned = initX;
		break;
	}

	if (wOut)
		*wOut = width;
	if (hOut)
		*hOut = height;
	if (fOut)
		*fOut = fontPointer;

	return xAligned;
}

float RenderGameText(const char *text, float x, float y, float scale,
	const float *color, Font_s *font, float rotation)
{
	R_AddCmdDrawText(text, INT_MAX, font, x, y,
		scale, scale, rotation, color, 0);

	return R_TextHeight(font) * scale;
}

float RenderGameTextWithBackground(const char *text, float x, float y,
	float w, float h, const float *borderColor, const float *textColor,
	Font_s *font, float scale)
{
	DrawFillRect(x, y - h, w - 2, h + 2,
		Colors::transparentBlack, 0);
	DrawEmptyRect(x, y - h, w - 2, h + 2, 2,
		borderColor);
	RenderGameText(text, x + 6, y, scale, textColor, font, 0);

	return h + 2;
}

float RenderUIText(const char *text, float x, float y, float scale,
	const float *color, Font_s *font, bool glow, const float *glowColor)
{
	if (glow)
		UI_DrawTextWithGlow(scrPlace, text, INT_MAX, font, x, y,
			0, 0, scale, color, 0, glowColor, 0, 0);
	else
		UI_DrawText(scrPlace, text, INT_MAX, font, x, y,
			0, 0, scale, color, 0);

	return UI_TextHeight(font, scale) + 2.0f;
}

float RenderUITextWithBackground(const char *text, float x, float y,
	float w, float h, const float *borderColor, const float *textColor,
	Font_s *font, float scale, bool glow)
{
	UI_FillRect(scrPlace, x, y - h, w - 2, h + 2,
		0, 0, Colors::transparentBlack);
	UI_DrawRect(scrPlace, x, y - h, w - 2, h + 2,
		0, 0, 2, borderColor);
	RenderUIText(text, x + 6, y, scale, textColor,
		font, glow, borderColor);

	return h + 2;
}

void WriteBytes(DWORD addr, const char *bytes, size_t len)
{
	DWORD curProtection;
	VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)addr, bytes, len, 0);

	VirtualProtect((LPVOID)addr, len, curProtection, nullptr);
}