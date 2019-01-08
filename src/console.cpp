#include "console.h"
#include "strfunc.h"


int ScrollByAbsoluteCoord(HANDLE hStdout, int iRows) {
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	SMALL_RECT srctWindow;

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) {
		std::string szBuffer(33, '\0');
		sprintf_s(&szBuffer[0], 33, "GetConsoleScreenBufferInfo (%d)\n", GetLastError());
		MessageBox(NULL, s2ws(szBuffer).c_str(), L"Error", MB_ICONERROR);
		return 0;
	}

	srctWindow = csbiInfo.srWindow;

	if (srctWindow.Top >= iRows) {
		srctWindow.Top -= (SHORT)iRows;
		srctWindow.Bottom -= (SHORT)iRows;

		if (!SetConsoleWindowInfo(hStdout, TRUE, &srctWindow)) {
			std::printf("SetConsoleWindowInfo (%d)\n", GetLastError());
			return 0;
		}
		return iRows;
	}
	else {
		std::printf("\nCannot scroll; the window is too close to the top.\n");
		return 0;
	}
}



BOOL GetConsoleHandle(HANDLE & hConsole, LPCSTR szTitle) {
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hConsole == INVALID_HANDLE_VALUE) return FALSE;

	if (strcmp(szTitle, "")) if (!SetConsoleTitleA(szTitle)) goto Error;
	if (!SetConsoleActiveScreenBuffer(hConsole)) goto Error;

	return TRUE;

Error:
	CloseHandle(hConsole);
	return FALSE;
}

BOOL SetConsoleFont(HANDLE & hConsole, SHORT nFontSize, const CHAR * szFontName) {
	CONSOLE_FONT_INFOEX cfie;
	ZeroMemory(&cfie, sizeof(cfie));
	cfie.cbSize = sizeof(cfie);
	cfie.dwFontSize.Y = nFontSize;
	lstrcpyW(cfie.FaceName, (LPCWSTR)szFontName);

	if (SetCurrentConsoleFontEx(hConsole, TRUE, &cfie)) return TRUE;

	CloseHandle(hConsole);
	return FALSE;
}

BOOL SetConsoleSize(HANDLE & hConsole, SHORT nScreenWidth, SHORT nScreenHeight, SMALL_RECT * psrBufferRect) {
	COORD largest, bufferSize;
	SMALL_RECT srBufferRect;

	srBufferRect = { 0, 0, 1, 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &srBufferRect)) goto Error;

	largest = GetLargestConsoleWindowSize(hConsole);
	bufferSize = { min(largest.X, nScreenWidth), min(largest.Y, nScreenHeight) };
	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) goto Error;

	srBufferRect = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &srBufferRect)) goto Error;

	ScrollByAbsoluteCoord(hConsole, -1);
	if (psrBufferRect != NULL) *psrBufferRect = srBufferRect;

	return TRUE;
Error:
	*psrBufferRect = srBufferRect;
	CloseHandle(hConsole);
	return FALSE;
}

BOOL SetConsoleColorTable(HANDLE & hConsole, const COLORREF crpColorTable[16]) {
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	if (!GetConsoleScreenBufferInfoEx(hConsole, &csbi)) goto Error;

	for (SIZE_T i = 0; i < 16; ++i) {
		csbi.ColorTable[i] = crpColorTable[i];
	}

	if (!SetConsoleScreenBufferInfoEx(hConsole, &csbi)) goto Error;

	return TRUE;
Error:
	CloseHandle(hConsole);
	return FALSE;
}



void Close(
	const std::string & szExitStr,
	HANDLE hConsole,
	std::vector<CHAR_INFO> * prgScreenBuffer,
	COORD * pcBufferSize,
	SMALL_RECT * psrBufferRect) {

	std::string szErrorMessage;
	if (hConsole != NULL && hConsole != INVALID_HANDLE_VALUE) {
		if (!SetConsoleFont(hConsole)) {
			szErrorMessage = "SetConsoleFont() -- " + std::to_string(GetLastError());
			goto Error;
		}

		if (prgScreenBuffer != NULL && pcBufferSize != NULL && psrBufferRect != NULL) {
			for (size_t i = 0; i < prgScreenBuffer->size(); ++i) {
				(*prgScreenBuffer)[i].Char.AsciiChar = ' ';
				(*prgScreenBuffer)[i].Attributes = 0;
			}
			if (!WriteConsoleOutputA(hConsole, &(*prgScreenBuffer)[0], *pcBufferSize, { 0, 0 }, psrBufferRect)) {
				szErrorMessage = "WriteConsoleOutputA() -- " + std::to_string(GetLastError());
				goto Error;
			}
		}

		if (!SetConsoleSize(hConsole)) {
			szErrorMessage = "SetConsoleSize() -- " + std::to_string(GetLastError());
			goto Error;
		}

		if (!SetConsoleColorTable(hConsole)) {
			szErrorMessage = "SetConsoleColorTable() -- " + std::to_string(GetLastError());
			goto Error;
		}

		CloseHandle(hConsole);
	}
	else if (szExitStr.empty()) {
		szErrorMessage = "Invalid handle value.";
		goto Error;
	}

	if (!szExitStr.empty()) MessageBox(NULL, (LPCWSTR)s2ws(szExitStr).c_str(), (LPCWSTR)L"Terminated", MB_ICONASTERISK);
	exit(0);

Error:
	MessageBox(NULL, s2ws(szErrorMessage).c_str(), L"Error", MB_ICONASTERISK);
	exit(1);
}