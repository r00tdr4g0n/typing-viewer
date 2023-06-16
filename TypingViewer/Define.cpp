#include "pch.h"
#include "Define.h"

void LoadBmp(CBitmap& a_bmp, LPCTSTR a_strBmpPath)
{
	CImage img;
	img.Load(a_strBmpPath);
	a_bmp.Attach(img.Detach());
}