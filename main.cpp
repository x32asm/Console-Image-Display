#include <opencv2/opencv.hpp>
#include "gstochar.h"
#include "imgmod.h"

#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <chrono>


SHORT nScreenWidth, nScreenHeight;
const CHAR * szTitle = "Console Image";


const COLORREF rgDefaultColorTable[16] = {
		RGB(0, 0, 0), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
		RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
		RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
		RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255) };

BOOL GetConsoleHandle(HANDLE & hConsole, SMALL_RECT & srBufferRect, CONSOLE_SCREEN_BUFFER_INFOEX & csbi, const COLORREF pColorTable[16] = rgDefaultColorTable) {
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hConsole == INVALID_HANDLE_VALUE) return FALSE;

	srBufferRect = { 0, 0, nScreenWidth - 1, nScreenHeight - 1 };

	SetConsoleScreenBufferSize(hConsole, { nScreenWidth, nScreenHeight });
	SetConsoleWindowInfo(hConsole, TRUE, &srBufferRect);
	if (!GetConsoleScreenBufferInfoEx(hConsole, &csbi)) goto Error;

	for (SIZE_T i = 0; i < 16; ++i) {
		csbi.ColorTable[i] = pColorTable[i];
	}

	csbi.dwSize = { nScreenWidth, nScreenHeight };
	csbi.srWindow = { csbi.srWindow.Left, csbi.srWindow.Top, nScreenWidth, nScreenHeight };
	csbi.dwMaximumWindowSize = { nScreenWidth, nScreenHeight };

	if (!SetConsoleScreenBufferInfoEx(hConsole, &csbi)) goto Error;

	if (!SetConsoleTitleA((LPCSTR)szTitle)) goto Error;
	if (!SetConsoleActiveScreenBuffer(hConsole)) goto Error;

	return TRUE;

Error:
	CloseHandle(hConsole);
	return FALSE;
}


void CommonColors(const cv::Mat & image, COLORREF * pColorTable) {

}


int main(int argc, char** argv) {
	cv::Mat image = cv::imread("c:/users/kanan/desktop/bathtub.png");
	if (image.empty()) {
		std::printf("Could not find or load image\n");
		return 1;
	}

	cv::Mat grey;
	cvtColor(image, grey, CV_BGR2GRAY);
	grey = imod::RemoveRows(grey, 4);
	grey = imod::QuickScaleDown(grey, 7);
//	grey = imod::QuickScaleUp(grey, 2);

	nScreenWidth = grey.cols; nScreenHeight = grey.rows;


	HANDLE hConsole;
	SMALL_RECT srBufferRect;
	CONSOLE_SCREEN_BUFFER_INFOEX csbi; csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	if (!GetConsoleHandle(hConsole, srBufferRect, csbi)) {
		std::printf("GetConsoleHandle(): -- %X\n", GetLastError());
		return 0;
	}
	std::vector<CHAR_INFO> rgScreenBuffer(nScreenWidth * nScreenHeight);


	cv::String windowName = szTitle;
//	image = imod::KeepRows(image, 3);
//	image = imod::QuickScaleUp(image, 4);
	image = imod::QuickScaleDown(image, 2);
	cv::namedWindow(windowName);
	cv::imshow(windowName, image);


	while (1) {
		for (std::size_t i = 0; i < rgScreenBuffer.size(); ++i) {
			rgScreenBuffer[i].Char.AsciiChar = ' ';
			rgScreenBuffer[i].Attributes = 0;
		}

		for (int y = 0, x; y < nScreenHeight; ++y) {
			for (x = 0; x < nScreenWidth; ++x) {
				rgScreenBuffer[x + y * nScreenWidth].Char.AsciiChar = gtc::chGSRamp[(int)(grey.at<uint8_t>(y, x) / gtc::fRampConversion)];
				rgScreenBuffer[x + y * nScreenWidth].Attributes = (int)(grey.at<uint8_t>(y, x) / (255. / 16));
			}
		}

		if (!WriteConsoleOutputA(hConsole, &rgScreenBuffer[0], { nScreenWidth, nScreenHeight }, { 0, 0 }, &srBufferRect)) {
			std::printf("WriteConsoleOutputA(): -- %X\n", GetLastError());
		}

		cv::waitKey(1000);
	}


	cv::destroyWindow(windowName);

	return 0;
}