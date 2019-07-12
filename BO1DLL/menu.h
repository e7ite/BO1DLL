#pragma once

#include "structures.h"

enum ScreenAlignment
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

enum Submenus
{
	MAIN_MENU,
	MISC_MENU,
	AIMBOT_MENU,
	ESP_MENU,
};

enum OptionType
{
	TYPE_SUB,
	TYPE_INT,
	TYPE_BOOL,
	TYPE_ARRAY,
};

struct Option
{
	const char *option;
	int *data;
	OptionType type;
	std::function<void()> callback;

	Option(const char *option, int *data,
		OptionType type, std::function<void()> callback)
		: option(option), data(data), type(type), callback(callback) {}
};

namespace Menu
{
	extern std::vector<std::vector<Option>> options;
	extern int open;
	extern int toggled;
	extern int currentSub;
	extern int timer;

	void Build();
	void Execute();
	void Insert(int sub, const char *option, int *data,
		OptionType type, std::function<void()> func);
	void LoadSub(int sub);
	void BoolModify(int *var);
	void IntModify(int *var, int *increment, int min, int max);
	bool Ready();
	void Wait(int ms);
	bool MonitorMouse(const Option &opt, int optPos, 
		float optionX, float optionY, float optionW, float optionH);
};

namespace Variables
{
	extern int enemyESP;
	extern int friendlyESP;
	extern int enableAimbot;
	extern int aimKey;
}

void DrawFillRect(float x, float y, float width, float height,
	const float *color, float rotation, int type = 0);
void DrawEmptyRect(float x, float y, float width, float height, float size,
	const float *color, int type = 0);
float AlignText(const char *text, const Fonts &font, float scale, float initX,
	ScreenAlignment align, bool ui, bool bg, Font_s **fOut = nullptr,
	float *wOut = nullptr, float *hOut = nullptr);
float RenderGameText(const char *text, float x, float y, float scale,
	const float *color, Font_s *font,
	float rotation);
float RenderGameTextWithBackground(const char *text, float x, float y,
	float w, float h, const float *borderColor, const float *textColor,
	Font_s *font, float scale);
float RenderUIText(const char *text, float x, float y, float scale,
	const float *color, Font_s *font,
	bool glow = false, const float *glowColor = nullptr);
float RenderUITextWithBackground(const char *text, float x, float y,
	float w, float h, const float *borderColor, const float *textColor,
	Font_s *font, float scale, bool glow);
void WriteBytes(DWORD addr, const char *bytes, size_t len);
