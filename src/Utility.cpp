/*
 * Utility.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */
#include "FrameGrabber.hpp"
#include "Utility.hpp"

Utility::Utility() {
	// TODO Auto-generated constructor stub

}

Utility::~Utility() {
	// TODO Auto-generated destructor stub
}

bool Utility::actionSwitch(const Mat& img, const std::string& aname, char key)
{
	switch (static_cast<char>(waitKey(1)))
	{
	case 's':
		return captureFrame(img, aname);
		break;
	case 27:
		std::cout << "esc key is pressed by user" << std::endl;
		return false;
	default:
		return true;
		break;
	}
}

bool Utility::captureFrame(const Mat& img, const std::string& aname)
{
	// TODO: make it platform independent or implement for different operating systems
#ifdef __linux__
	bool ret;
	std::string fn;
	struct stat st = {0};
	int n = 1;
	if (stat(SCREENSHOTS_DIR, &st) == -1) {
		mkdir(SCREENSHOTS_DIR, 0700);
	}
	fn = SCREENSHOTS_DIR + aname + "_" + std::to_string(FrameGrabber::instance()->getFrameNum()) + ".jpg";
	while (stat(fn.c_str(), &st) != -1) {
		fn = SCREENSHOTS_DIR + aname + "_" + std::to_string(FrameGrabber::instance()->getFrameNum()) + "_" + std::to_string(n) + ".jpg";
		n++;
	}
	imwrite(fn, img);
	fn = SCREENSHOTS_DIR + aname + "_input.jpg";
	ret = imwrite(fn, FrameGrabber::instance()->getCurrentFrame());
	return ret;
#endif
}

void Utility::test(const Mat& img, const std::string& reference)
{
	Mat ref;
	ref = imread(reference, IMREAD_GRAYSCALE);
	int tp = 0, tn = 0, fp = 0, fn = 0;
	float precision, recall;
	FOR_PIXELS(i, j, img) {
		const uchar &a = img.at<uchar>(i, j);
		const uchar &b = ref.at<uchar>(i, j);
		//std::cout << (int)a << " " << (int)b << std::endl;
		if (a > 0) {
			if (b > 0)
				tp++;
			else
				fp++;
		} else {
			if (b > 0)
				fn++;
			else
				tn++;
		}
	}
	recall = (float)tp / (float)(tp + fn);
	precision = (float)fp / (float)(fp + tn);
	std::cout << "TP: " << tp << ", TN: " << tn << ", FP:" << fp << ", FN: " << fn << std::endl;
	std::cout << "Recall: " << recall << std::endl;
	std::cout << "Precision: " << precision << std::endl;

}
