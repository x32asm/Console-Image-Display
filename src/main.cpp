#include <thread>
#include <chrono>
#include <shlobj.h>
#include <conio.h>

#include "gstochar.h"
#include "imgmod.h"
#include "strfunc.h"
#include "console.h"
#include "clrfunc.h"


#define Sleep(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
bool bBW = false, bAscii = false;


int main(int argc, char** argv) {
	auto tstart = std::chrono::system_clock::now();
	auto tstop = std::chrono::system_clock::now();
	std::string szExitStr;
	int nLen;

	PWSTR pwszPath = NULL;
	std::wstring wszFolderPath(1024, 32);
	std::string szDesktopPath;
	std::string szImagePath;

	SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &pwszPath);
	int nCharsWritten = swprintf_s(&wszFolderPath[0], wszFolderPath.size(), L"%s", pwszPath);
	wszFolderPath.resize(nCharsWritten);
	CoTaskMemFree(pwszPath);
	szDesktopPath = ws2s(wszFolderPath);

	int nFontSize = 1;

	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			if (argv[i][0] == '-') {
				for (size_t j = 0; j < strlen(argv[i]); ++j) {
					if (argv[i][j] == 'g') bBW = true;
					else if (argv[i][j] == 'a') bAscii = true;
				}
			}
			else {
				if (szImagePath.compare("")) nFontSize = std::max(1, std::stoi(argv[i]));
				else szImagePath = argv[i];
			}
		}
	}
	if (!trim(szImagePath).compare("")) {
		szExitStr.resize(1024);
		nLen = sprintf_s(&szExitStr[0], 1024,
			"Usage:\n    options - g, greyscale mode ; a, ascii mode\n\n    *.exe <image path> [options]");
		szExitStr.resize(nLen);
		Close(szExitStr);
	}

Load:
	cv::Mat mImage = cv::imread(szImagePath);
	if (mImage.empty()) {
		if (szImagePath.find(szDesktopPath) == -1) {
			std::string szNewImagePath(1024, 0);
			nLen = sprintf_s(&szNewImagePath[0], 1024, "%s\\%s", szDesktopPath.c_str(), szImagePath.c_str());
			szNewImagePath.resize(nLen);
			szImagePath = szNewImagePath;
			goto Load;
		}
		Close("Could not find or load image");
	}

	COORD cImageDimensions;
	const CHAR * szTitle = "Console Image";

	cImageDimensions = { (SHORT)mImage.cols, (SHORT)mImage.rows };


	HANDLE hConsole;
	SMALL_RECT srBufferRect;
	COLORREF pColorTable[16];

	if (!GetConsoleHandle(hConsole, (LPCSTR)szTitle)) {
		szExitStr.resize(1024);
		nLen = sprintf_s(&szExitStr[0], 1024, "GetConsoleHandle(): -- 0x%X", GetLastError());
		szExitStr.resize(nLen);
		Close(szExitStr, hConsole);
	}
	if (!SetConsoleFont(hConsole, nFontSize, "Consolas")) {
		szExitStr.resize(1024);
		nLen = sprintf_s(&szExitStr[0], 1024, "SetConsoleFont(): -- 0x%X", GetLastError());
		szExitStr.resize(nLen);
		Close(szExitStr, hConsole);
	}


	tstart = std::chrono::system_clock::now();
	cv::Mat grey;
	mImage = imod::RemoveRows(mImage, ((nFontSize >= 3 && nFontSize <= 5) || (nFontSize >= 10 && nFontSize <= 12) || (nFontSize >= 16 && nFontSize <= 17)) ? 2 : 3);
	if (nFontSize >= 13 && nFontSize <= 15) mImage = imod::RemoveRows(mImage, 5);
	if (nFontSize >= 19) mImage = imod::RemoveRows(mImage, 6);
	if (nFontSize == 1) mImage = imod::RemoveRows(mImage, 2);
	CommonColors3C(mImage, pColorTable, bBW, bAscii);
	COORD cMaxSize = GetLargestConsoleWindowSize(hConsole);
	COORD cScaleDimensions;
	SHORT nBiggestImageDimension;

	auto lfQComp = [mImage, cMaxSize]() { return (float)mImage.cols / cMaxSize.X > (float)mImage.rows / cMaxSize.Y; };
	auto lfSetDimensions = [lfQComp, mImage, cMaxSize](SHORT * nBig) {
		*nBig = (lfQComp()) ? (SHORT)mImage.cols : (SHORT)mImage.rows;
		if (lfQComp()) return COORD{ *nBig, cMaxSize.X };
		else return COORD{ *nBig, cMaxSize.Y };
	};

	if (mImage.cols > cMaxSize.X || mImage.rows > cMaxSize.Y) {
		cScaleDimensions = lfSetDimensions(&nBiggestImageDimension);
		mImage = imod::ScaleDown(mImage, (int)ceil((float)cScaleDimensions.X / cScaleDimensions.Y));
	}
	else {
		cScaleDimensions = lfSetDimensions(&nBiggestImageDimension);
		mImage = imod::ScaleUp(mImage, (int)((float)cScaleDimensions.Y / cScaleDimensions.X));
	}

	cImageDimensions = { (SHORT)mImage.cols, (SHORT)mImage.rows };

	cvtColor(mImage, grey, CV_BGR2GRAY);

	if (!SetConsoleSize(hConsole, cImageDimensions.X, cImageDimensions.Y, &srBufferRect)) {
		szExitStr.resize(1024);
		nLen = sprintf_s(&szExitStr[0], 1024, "GetConsoleHandle(): -- 0x%X", GetLastError());
		szExitStr.resize(nLen);
		Close(szExitStr, hConsole);
	}

	if (!SetConsoleColorTable(hConsole, pColorTable)) {
		szExitStr.resize(1024);
		nLen = sprintf_s(&szExitStr[0], 1024, "SetConsoleColorTable(): -- 0x%X", GetLastError());
		szExitStr.resize(nLen);
		Close(szExitStr, hConsole);
	}
	std::vector<CHAR_INFO> rgScreenBuffer(cImageDimensions.X * cImageDimensions.Y);


	auto lfNearestColor = [pColorTable, mImage](int x, int y) {
		uint8_t nClosestIndex = 0;
		cv::Vec3b v3CurrentColor;
		for (uint8_t i = 1; i < 16; ++i) {
			v3CurrentColor = mImage.at<cv::Vec3b>(y, x);
			if (ColorDist(pColorTable[i], v3CurrentColor) < ColorDist(pColorTable[nClosestIndex], v3CurrentColor))
				nClosestIndex = i;
		}
		return nClosestIndex;
	};

	for (int y = 0, x; y < cImageDimensions.Y; ++y) {
		for (x = 0; x < cImageDimensions.X; ++x) {
			uint8_t nClosestIndex = 0;
			int nAvg;

			nClosestIndex = lfNearestColor(x, y);
			if (bBW || bAscii) nAvg = (GetRValue(pColorTable[nClosestIndex]) + GetGValue(pColorTable[nClosestIndex]) + GetBValue(pColorTable[nClosestIndex])) / 3;

			if (!bAscii) rgScreenBuffer[x + y * cImageDimensions.X].Char.AsciiChar = (uchar)219;
			else rgScreenBuffer[x + y * cImageDimensions.X].Char.AsciiChar = gtc::chGSRamp[(int)(nAvg / gtc::fRampConversion)];

			if (bAscii && bBW) rgScreenBuffer[x + y * cImageDimensions.X].Attributes = 15;
			else rgScreenBuffer[x + y * cImageDimensions.X].Attributes = nClosestIndex;
		}
	}

	while (!_kbhit()) { 
		if (!WriteConsoleOutputA(hConsole, &rgScreenBuffer[0], cImageDimensions, { 0, 0 }, &srBufferRect)) {
			std::printf("WriteConsoleOutputA(): -- 0x%X\n", GetLastError());
		}

		Sleep(100);
	}

	tstop = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> diff = tstop - tstart;

	szExitStr = "Time: " + std::to_string(diff.count()) + "ms";
	Close("", hConsole, &rgScreenBuffer, &cImageDimensions, &srBufferRect);
	return 0;
}
