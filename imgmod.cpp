#include "imgmod.h"


void imod::QuickScaleDown(cv::Mat & resultImage, const cv::Mat & originalImage, int scaleFactor) {
	cv::Scalar scalar((originalImage.channels() == 3) ? 0, 0, 0 : (originalImage.channels() == 2) ? 0, 0 : 0);
	resultImage = cv::Mat(originalImage.rows / scaleFactor, originalImage.cols / scaleFactor, originalImage.type(), scalar);

	int nChannels = originalImage.channels();
	for (int y = 0, x; y < resultImage.rows; ++y) {
		for (x = 0; x < resultImage.cols; ++x) {

			if (y * scaleFactor < originalImage.rows && x * scaleFactor < originalImage.cols) {
				for (int channel = 0; channel < nChannels; ++channel) {
					if (nChannels == 3) resultImage.at<cv::Vec3b>(y, x)[channel] = originalImage.at<cv::Vec3b>(y * scaleFactor, x * scaleFactor)[channel];
					else if (nChannels == 2) resultImage.at<cv::Vec2b>(y, x)[channel] = originalImage.at<cv::Vec2b>(y * scaleFactor, x * scaleFactor)[channel];
					else resultImage.at<uint8_t>(y, x) = originalImage.at<uint8_t>(y * scaleFactor, x * scaleFactor);
				}
			}

		}
	}
}
cv::Mat imod::QuickScaleDown(const cv::Mat & originalImage, int scaleFactor) {
	cv::Mat ret; QuickScaleDown(ret, originalImage, scaleFactor);
	return ret;
}


void imod::QuickScaleUp(cv::Mat & resultImage, const cv::Mat & originalImage, int scaleFactor) {
	cv::Scalar scalar((originalImage.channels() == 3) ? 0, 0, 0 : (originalImage.channels() == 2) ? 0, 0 : 0);
	resultImage = cv::Mat(originalImage.rows * scaleFactor, originalImage.cols * scaleFactor, originalImage.type(), scalar);

	int nChannels = originalImage.channels();
	uint8_t * pixel = new uint8_t[nChannels];
	for (int y = 0, x, r, c, channel; y < originalImage.rows; ++y) {
		for (x = 0; x < originalImage.cols; ++x) {
			for (channel = 0; channel < nChannels; ++channel) {
				if (nChannels == 3) pixel[channel] = originalImage.at<cv::Vec3b>(y, x)[channel];
				else if (nChannels == 2) pixel[channel] = originalImage.at<cv::Vec2b>(y, x)[channel];
				else pixel[channel] = originalImage.at<uint8_t>(y, x);
			}

			for (r = 0; r < scaleFactor; ++r) {
				for (c = 0; c < scaleFactor; ++c) {

					if (y * scaleFactor + r < resultImage.rows && x * scaleFactor + c < resultImage.cols) {
						for (channel = 0; channel < originalImage.channels(); ++channel) {
							if (nChannels == 3) resultImage.at<cv::Vec3b>(y * scaleFactor + r, x * scaleFactor + c)[channel] = pixel[channel];
							else if (nChannels == 2) resultImage.at<cv::Vec2b>(y * scaleFactor + r, x * scaleFactor + c)[channel] = pixel[channel];
							else resultImage.at<uint8_t>(y * scaleFactor + r, x * scaleFactor + c) = pixel[channel];
						}
					}

				}
			}

		}
	}

	delete[] pixel;
}
cv::Mat imod::QuickScaleUp(const cv::Mat & originalImage, int scaleFactor) {
	cv::Mat ret; QuickScaleUp(ret, originalImage, scaleFactor);
	return ret;
}

void imod::KeepRows(cv::Mat & resultImage, const cv::Mat & originalImage, int ratio) {
	cv::Scalar scalar((originalImage.channels() == 3) ? 0, 0, 0 : (originalImage.channels() == 2) ? 0, 0 : 0);
	resultImage = cv::Mat(originalImage.rows / ratio, originalImage.cols, originalImage.type(), scalar);

	int nChannels = originalImage.channels();
	uint8_t * pixel = new uint8_t[nChannels];
	for (int y = 0, x, channel; y < originalImage.rows; y += ratio) {
		for (x = 0; x < originalImage.cols; ++x) {
			if (y / ratio >= resultImage.rows) break;
			for (channel = 0; channel < nChannels; ++channel) {
				if (nChannels == 3) pixel[channel] = originalImage.at<cv::Vec3b>(y, x)[channel];
				else if (nChannels == 2) pixel[channel] = originalImage.at<cv::Vec2b>(y, x)[channel];
				else pixel[channel] = originalImage.at<uint8_t>(y, x);
			}

			for (channel = 0; channel < originalImage.channels(); ++channel) {
				if (nChannels == 3) resultImage.at<cv::Vec3b>(y / ratio, x)[channel] = pixel[channel];
				else if (nChannels == 2) resultImage.at<cv::Vec2b>(y / ratio, x)[channel] = pixel[channel];
				else resultImage.at<uint8_t>(y / ratio, x) = pixel[channel];
			}
		}
	}

	delete[] pixel;
}
cv::Mat imod::KeepRows(const cv::Mat & originalImage, int ratio) {
	cv::Mat ret; KeepRows(ret, originalImage, ratio);
	return ret;
}


void imod::RemoveRows(cv::Mat & resultImage, const cv::Mat & originalImage, int ratio) {
	cv::Scalar scalar((originalImage.channels() == 3) ? 0, 0, 0 : (originalImage.channels() == 2) ? 0, 0 : 0);
	resultImage = cv::Mat(originalImage.rows - originalImage.rows / ratio, originalImage.cols, originalImage.type(), scalar);

	int nChannels = originalImage.channels();
	uint8_t * pixel = new uint8_t[nChannels];
	int ry = 0;
	for (int y = 0, x, channel; y < originalImage.rows; ++y) {
		if (y % ratio == 0) {
			continue;
		}
		++ry;
		for (x = 0; x < originalImage.cols; ++x) {
			if (ry >= resultImage.rows) break;
			for (channel = 0; channel < nChannels; ++channel) {
				if (nChannels == 3) pixel[channel] = originalImage.at<cv::Vec3b>(y, x)[channel];
				else if (nChannels == 2) pixel[channel] = originalImage.at<cv::Vec2b>(y, x)[channel];
				else pixel[channel] = originalImage.at<uint8_t>(y, x);
			}

			for (channel = 0; channel < originalImage.channels(); ++channel) {
				if (nChannels == 3) resultImage.at<cv::Vec3b>(ry, x)[channel] = pixel[channel];
				else if (nChannels == 2) resultImage.at<cv::Vec2b>(ry, x)[channel] = pixel[channel];
				else resultImage.at<uint8_t>(ry, x) = pixel[channel];
			}
		}
	}

	delete[] pixel;
}
cv::Mat imod::RemoveRows(const cv::Mat & originalImage, int ratio) {
	cv::Mat ret; RemoveRows(ret, originalImage, ratio);
	return ret;
}


void imod::CreateNoisyImage(cv::Mat & resultImage, int pixelSize, int pixelGap) {
	srand((unsigned)time(0));

	int nChannels = resultImage.channels();
	uint8_t * randPixel = new uint8_t[nChannels];
	for (int y = 0, x, r, c; y < resultImage.rows;) {
		for (x = 0; x < resultImage.cols;) {
			for (int channel = 0; channel < nChannels; ++channel) {
				randPixel[channel] = (uchar)(rand() % 256);
			}

			for (r = 0; r < pixelSize; ++r) {
				for (c = 0; c < pixelSize; ++c) {

					if (r + y < resultImage.rows && c + x < resultImage.cols) {
						for (int channel = 0; channel < nChannels; ++channel) {
							if (nChannels == 3) resultImage.at<cv::Vec3b>(r + y, c + x)[channel] = randPixel[channel];
							else if (nChannels == 2) resultImage.at<cv::Vec2b>(r + y, c + x)[channel] = randPixel[channel];
							else resultImage.at<uint8_t>(r + y, c + x) = randPixel[channel];
						}
					}

				}
			}

			x += pixelSize + pixelGap;
		}
		y += pixelSize + pixelGap;
	}

	delete[] randPixel;
}
cv::Mat imod::CreateNoisyImage(int rows, int collumns, int type, int pixelSize, int pixelGap) {
	cv::Mat ret(rows, collumns, type); CreateNoisyImage(ret, pixelSize, pixelGap);
	return ret;
}