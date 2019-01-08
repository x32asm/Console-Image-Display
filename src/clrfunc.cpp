#include "clrfunc.h"


double ColorDist(const cv::Vec3b & c1, const cv::Vec3b & c2) {
	return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2));
}
double ColorDist(int c1, const cv::Vec3b & c2) {
	return sqrt(pow(GetRValue(c1) - c2[2], 2) + pow(GetGValue(c1) - c2[1], 2) + pow(GetBValue(c1) - c2[0], 2));
}
double ColorDist(int c1, int c2) {
	return sqrt(pow(GetRValue(c1) - GetRValue(c2), 2) + pow(GetGValue(c1) - GetGValue(c2), 2) + pow(GetBValue(c1) - GetBValue(c2), 2));
}


bool Vec3bContains(const std::vector<cv::Vec3b> & vv3Vec, const cv::Vec3b & v3El, int nAllowance) {
	for (size_t i = 0; i < vv3Vec.size(); ++i)
		if (ColorDist(vv3Vec[i], v3El) <= nAllowance) return true;
	return false;
}

void GetUniqueColors(const std::vector<cv::Vec3b> & vVec, std::vector<cv::Vec3b> & v3Un) {
	size_t nUnCounter = 0, nVecs = 48;
	v3Un.resize(nVecs);
	int nAllowance = 200;
	while (nUnCounter < nVecs - 1) {
		for (size_t i = 0; i < vVec.size() - 1; ++i) {
			if (!Vec3bContains(v3Un, vVec[i], nAllowance)) {
				v3Un[nUnCounter] = vVec[i];
				if (nUnCounter++ >= nVecs - 1) return;
			}
		}
		nAllowance -= 10;
	}
}


void AveragePairs(std::vector<cv::Vec3b> & vv3Colors) {
	size_t nVecEnd = vv3Colors.size(), i, nClosestIndex;
	int j;
	while (nVecEnd > 16) {
		for (i = 0; nVecEnd > 16 && i < nVecEnd - 2; ++i) {

			nClosestIndex = i + 1;
			for (j = (int)i + 2; nVecEnd > 16 && j < nVecEnd && i < nVecEnd; ++j) {
				if (ColorDist(vv3Colors[j], vv3Colors[i]) < ColorDist(vv3Colors[nClosestIndex], vv3Colors[i]))
					nClosestIndex = j;
			}

			for (j = 0; j < 3; ++j) vv3Colors[i][j] = (vv3Colors[i][j] + vv3Colors[nClosestIndex][j]) / 2;
			vv3Colors[nClosestIndex] = vv3Colors[--nVecEnd];

		}
	}
	vv3Colors.resize(nVecEnd);
}


int AverageColor(const cv::Mat & mImage) {
	int rgAvg[3];
	for (int y = 0, x, i; y < mImage.rows; ++y) {
		for (x = 0; x < mImage.cols; ++x) {
			for (i = 0; i < 3; ++i) rgAvg[i] += mImage.at<cv::Vec3b>(y, x)[i];
		}
	}
	for (int i = 0; i < 3; ++i) rgAvg[i] /= mImage.rows * mImage.cols;
	return RGB(rgAvg[2], rgAvg[1], rgAvg[0]);
}

int GetGrayscale(const cv::Vec3b v3Pixel) {
	int nColor = (v3Pixel[0] + v3Pixel[1] + v3Pixel[2]) / 3;
	return RGB(nColor, nColor, nColor);
}


void CommonColors3C(const cv::Mat & mImage, COLORREF * crpColorTable, bool bBW, bool bAscii) {
	std::vector<cv::Vec3b> vv3Colors;
	int rgColor[3];
	int nSamples = 32;
	int nDWidth = mImage.cols / nSamples, nDHeight = mImage.rows / nSamples;
	int nDResolution = nDWidth * nDHeight;
	for (int qr = 0; qr < nSamples; ++qr) {
		for (int qc = 0; qc < nSamples; ++qc) {
			for (int r = 0; r < nDHeight; ++r) {
				for (int c = 0; c < nDWidth; ++c) {
					rgColor[0] += mImage.at<cv::Vec3b>(r + qr * nDHeight, c + qc * nDWidth)[0];
					rgColor[1] += mImage.at<cv::Vec3b>(r + qr * nDHeight, c + qc * nDWidth)[1];
					rgColor[2] += mImage.at<cv::Vec3b>(r + qr * nDHeight, c + qc * nDWidth)[2];
				}
			}
			rgColor[0] /= nDResolution;
			rgColor[1] /= nDResolution;
			rgColor[2] /= nDResolution;
			vv3Colors.push_back(cv::Vec3b(rgColor[0], rgColor[1], rgColor[2]));
		}
	}

	std::vector<cv::Vec3b> vv3MostUnique;
	GetUniqueColors(vv3Colors, vv3MostUnique);
	AveragePairs(vv3MostUnique);


	int nAvg = 0;
	for (int i = 0; i < 16; ++i) {
		if (!bBW) crpColorTable[i] = RGB(vv3MostUnique[i][2], vv3MostUnique[i][1], vv3MostUnique[i][0]);
		else crpColorTable[i] = RGB((nAvg = (vv3MostUnique[i][2] + vv3MostUnique[i][1] + vv3MostUnique[i][0]) / 3), nAvg, nAvg);
	}
	if (bAscii)	crpColorTable[0] = 0;
	if (bBW && bAscii)	crpColorTable[15] = 0x00ffffff;
}