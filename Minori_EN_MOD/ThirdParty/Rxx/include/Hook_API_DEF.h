﻿#pragma once
#include <Windows.h>


typedef INT(WINAPI* pMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

typedef HFONT(WINAPI* pCreateFontA)(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);

typedef HFONT(WINAPI* pCreateFontIndirectA)(CONST LOGFONTA* lplf);

typedef HWND(WINAPI* pCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, INT X, INT Y, INT nWidth, INT nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

typedef DWORD(WINAPI* pGetGlyphOutlineA)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2);