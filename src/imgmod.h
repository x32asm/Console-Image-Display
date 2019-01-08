#ifndef GUARD_IMGMOD_H
#define GUARD_IMGMOD_H
#pragma once

#include <opencv2/opencv.hpp>


namespace imod {
	void ScaleDown(cv::Mat & resultImage, const cv::Mat & originalImage, int scaleFactor);
	cv::Mat ScaleDown(const cv::Mat & originalImage, int scaleFactor);

	void ScaleUp(cv::Mat & resultImage, const cv::Mat & originalImage, int scaleFactor);
	cv::Mat ScaleUp(const cv::Mat & originalImage, int scaleFactor);

	void KeepRows(cv::Mat & resultImage, const cv::Mat & originalImage, int ratio);
	cv::Mat KeepRows(const cv::Mat & originalImage, int ratio);

	void RemoveRows(cv::Mat & resultImage, const cv::Mat & originalImage, int ratio);
	cv::Mat RemoveRows(const cv::Mat & originalImage, int ratio);

	void CreateNoisyImage(cv::Mat & resultImage, int pixelSize = 1, int pixelGap = 0);
	cv::Mat CreateNoisyImage(int rows, int collumns, int type, int pixelSize = 1, int pixelGap = 0);
}


#endif // GUARD_IMGMOD_H