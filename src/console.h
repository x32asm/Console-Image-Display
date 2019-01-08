#ifndef GUARD_CONSOLE_H
#define GUARD_CONSOLE_H
#pragma once

#include <windows.h>
#include <string>
#include <vector>


const COLORREF rgDefaultColorTable[16] = {
		RGB(0, 0, 0), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
		RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
		RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
		RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255) };

int ScrollByAbsoluteCoord(HANDLE hStdout, int iRows);
BOOL GetConsoleHandle(HANDLE & hConsole, LPCSTR szTitle = "");
BOOL SetConsoleFont(HANDLE & hConsole, SHORT nFontSize = 14, const CHAR * szFontName = "Consolas");
BOOL SetConsoleSize(HANDLE & hConsole, SHORT nScreenWidth = 100, SHORT nScreenHeight = 30, SMALL_RECT * psrBufferRect = NULL);
BOOL SetConsoleColorTable(HANDLE & hConsole, const COLORREF crpColorTable[16] = rgDefaultColorTable);

void Close(
	const std::string & szExitStr = "",
	HANDLE hConsole = NULL,
	std::vector<CHAR_INFO> * prgScreenBuffer = NULL,
	COORD * pcBufferSize = NULL,
	SMALL_RECT * psrBufferRect = NULL);


#endif // GUARD_CONSOLE_H