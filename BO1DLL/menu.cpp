#include "menu.h"

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

void RenderGameText(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *color, const char *fontName,
	float rotation, int fontType, float *wOut, float *hOut)
{
	Font_s *fontPointer = CL_RegisterFont(fontName, fontType);
	float width = (float)R_TextWidth(text, INT_MAX, fontPointer);
	float xAligned = .0f;

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = x;
		break;
	case ALIGN_CENTER:
		xAligned = x - width / 2 * scale;
		break;
	case ALIGN_RIGHT:
		xAligned = x - width * scale;
		break;
	default:
		xAligned = x;
		break;
	}

	R_AddCmdDrawText(text, INT_MAX, fontPointer, xAligned, y,
		scale, scale, rotation, color, fontType);
}

float RenderGameTextWithBackground(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *borderColor, const float *textColor,
	const char *fontName, int fontType, float *wOut, float *hOut)
{
	Font_s *fontPointer = CL_RegisterFont(fontName, fontType);
	float width = R_TextWidth(text, INT_MAX, fontPointer) * scale + 14.0f;
	float height = R_TextHeight(fontPointer) * scale + 2.0f;
	float xAligned = 0.0f;

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = x;
		break;
	case ALIGN_CENTER:
		xAligned = x - width / 2;
		break;
	case ALIGN_RIGHT:
		xAligned = x - width;
		break;
	default:
		xAligned = x;
		break;
	}

	DrawFillRect(xAligned, y - height, width - 2, height + 2, 
		Colors::transparentBlack, 0, fontType);
	DrawEmptyRect(xAligned, y - height, width - 2, height + 2, 2,
		borderColor, fontType);
	RenderGameText(text, xAligned + 6, y, scale, ALIGN_LEFT, textColor, 
		fontName);

	return y + height + 2;
}

float RenderUIText(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *color,
	Fonts fontName, int fontType, float *wOut, float *hOut)
{
	Font_s *font = UI_GetFontHandle(scrPlace, fontName, scale);
	float textWidth = (float)UI_TextWidth(text, INT_MAX, font, scale);
	float textHeight = (float)UI_TextHeight(font, scale) + 2.0f;
	float xAligned;

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = x;
		break;
	case ALIGN_CENTER:
		xAligned = x - textWidth / 2;
		break;
	case ALIGN_RIGHT:
		xAligned = x - textWidth;
		break;
	default:
		xAligned = x;
		break;
	}

	if (wOut != nullptr)
		*wOut = xAligned;
	if (hOut != nullptr)
		*hOut = textHeight;

	UI_DrawText(scrPlace, text, INT_MAX, font, xAligned, y,
		0, 0, scale, color, fontType);

	return textHeight;
}

float RenderUITextWithBackground(const char *text, float x, float y, float scale,
	ScreenAlignment align, const float *borderColor, const float *textColor,
	Fonts fontName, int fontType, float *wOut, float *hOut)
{
	Font_s *font = UI_GetFontHandle(scrPlace, fontName, scale);
	float textWidth = (float)UI_TextWidth(text, INT_MAX, font, scale) + 16.0f;
	float textHeight = (float)UI_TextHeight(font, scale) + 2.0f;
	float xAligned;

	switch (align)
	{
	case ALIGN_LEFT:
		xAligned = x;
		break;
	case ALIGN_CENTER:
		xAligned = x - textWidth / 2;
		break;
	case ALIGN_RIGHT:
		xAligned = x - textWidth;
		break;
	default:
		xAligned = x;
		break;
	}

	if (wOut != nullptr)
		*wOut = xAligned;
	if (hOut != nullptr)
		*hOut = textHeight;

	UI_FillRect(scrPlace, xAligned, y - textHeight, textWidth - 2, textHeight + 2,
		0, 0, Colors::transparentBlack);
	UI_DrawRect(scrPlace, xAligned, y - textHeight, textWidth - 2, textHeight + 2,
		0, 0, 2, borderColor);
	RenderUIText(text, xAligned + 6, y, scale, ALIGN_LEFT, textColor,
		fontName, fontType);

	return y + textHeight + 2;
}

void WriteBytes(DWORD addr, const char *bytes, size_t len)
{
	DWORD curProtection;
	VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &curProtection);

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)addr, bytes, len, 0);

	VirtualProtect((LPVOID)addr, len, curProtection, nullptr);
}