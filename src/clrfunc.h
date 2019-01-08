#ifndef GUARD_CLRFUNC_H
#define GUARD_CLRFUNC_H
#pragma once

#include <opencv2/opencv.hpp>
#include <windows.h>

double ColorDist(const cv::Vec3b & c1, const cv::Vec3b & c2);
double ColorDist(int c1, const cv::Vec3b & c2);
double ColorDist(int c1, int c2);

bool Vec3bContains(
	const std::vector<cv::Vec3b> & vv3Vec,
	const cv::Vec3b & v3El,
	int nAllowance = 0);

void GetUniqueColors(const std::vector<cv::Vec3b> & vVec, std::vector<cv::Vec3b> & v3Un);
void AveragePairs(std::vector<cv::Vec3b> & vv3Colors);
int AverageColor(const cv::Mat & mImage);
void CommonColors3C(const cv::Mat & mImage, COLORREF * crpColorTable, bool bBW, bool bAscii);

#endif // GUARD_CLRFUNC_H