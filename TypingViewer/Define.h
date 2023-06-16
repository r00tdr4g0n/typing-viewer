#pragma once

#define TOOLBAR_BTN_CNT 6
#define TOOLBAR_FOLDER_PATH L"res\\save.bmp"
#define TOOLBAR_FONT_PATH L"res\\font.bmp"
#define TOOLBAR_MONITOR_OFF_PATH L"res\\switch-off.bmp"
#define TOOLBAR_MONITOR_ON_PATH L"res\\switch-on.bmp"
#define TOOLBAR_INFO_PATH L"res\\info.bmp"
#define TOOLBAR_SETTING_PATH L"res\\gear.bmp"
#define TOOLBAR_MASK_COLOR RGB(255,255,255)

#define OUTPUT_FILE_PATH L"content.txt"

#define UM_CHANGE_ALPHA	(WM_USER + 1)
#define UM_DRAW_BORDER (WM_USER + 2)
#define UM_SET_SPACE_UP (WM_USER + 3)
#define UM_SET_SPACE_DOWN (WM_USER + 4)
#define UM_SET_SPACE_LEFT (WM_USER + 5)
#define UM_SET_SPACE_RIGHT (WM_USER + 6)
#define UM_SET_LINE (WM_USER + 7)
#define UM_SET_WORD_CNT (WM_USER + 8)
#define UM_SET_SUBTITLE_UP (WM_USER + 9)
#define UM_SET_SUBTITLE_DOWN (WM_USER + 10)
#define UM_SET_SUBTITLE (WM_USER + 11)
#define UM_MAIN_DESTROY (WM_USER + 12)
#define UM_SET_SPACE	(WM_USER + 13)

typedef struct SubtitleSpace {
	UINT left;
	UINT right;
	UINT top;
	UINT bottom;
}SubtitleSpace;

void LoadBmp(CBitmap& a_bmp, LPCTSTR a_strBmpPath);