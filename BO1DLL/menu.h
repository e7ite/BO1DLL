#pragma once

#include "structures.h"

enum ScreenAlignment
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

enum Fonts
{
	smallFont = 3,
	normalFont = 1,
	consoleFont = 5,
	objectiveFont = 6,
};

void DrawFillRect(float x, float y, float width, float height,
	const float *color, float rotation, int type = 0);
void DrawEmptyRect(float x, float y, float width, float height, float size,
	const float *color, int type = 0);
void RenderGameText(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *color, const char *fontName,
	float rotation = 0.0f, int fontType = 0,
	float *wOut = nullptr, float *hOut = nullptr);
float RenderGameTextWithBackground(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *borderColor, const float *textColor,
	const char *fontName, int fontType = 0, 
	float *wOut = nullptr, float *hOut = nullptr);
float RenderUIText(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *color,
	Fonts fontName, int fontType = 0,
	float *wOut = nullptr, float *hOut = nullptr);
float RenderUITextWithBackground(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *borderColor, const float *textColor,
	Fonts fontName, int fontType = 0,
	float *wOut = nullptr, float *hOut = nullptr);
