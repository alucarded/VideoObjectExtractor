// This file is part of VideoObjectExtractor.
//
// Copyright 2014-2015 Tomasz Posłuszny
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights  to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "FrameGrabber.hpp"
#include "Utility.hpp"

Utility::Utility() {
	// TODO Auto-generated constructor stub

}

Utility::~Utility() {
	// TODO Auto-generated destructor stub
}

const std::string toFrameNumber(const uint idx) {
   std::ostringstream oss;
   oss << std::setfill('0') << std::setw(6) << idx;
   return oss.str();
}

bool Utility::actionSwitch(const Mat& img, const std::string& path, char key)
{
	switch (static_cast<char>(waitKey(1)))
	{
	case 's':
		return captureFrame(img, path);
		break;
	case 27:
		std::cout << "esc key is pressed by user" << std::endl;
		return false;
	default:
		return true;
		break;
	}
}

bool Utility::captureFrame(const Mat& img, const std::string& path)
{
	// TODO: make it platform independent or implement for different operating systems
#ifdef __linux__
	bool ret;
	std::string fn;
	struct stat st = {0};
	int n = 1;
	//if (stat(SCREENSHOTS_DIR, &st) == -1) {
	//	mkdir(SCREENSHOTS_DIR, 0700);
	//}
	fn = path + "bin" + toFrameNumber(FrameGrabber::instance()->getFrameNum()) + ".jpg";
	while (stat(fn.c_str(), &st) != -1) {
		fn = path + "bin" + toFrameNumber(FrameGrabber::instance()->getFrameNum()) + "_" + std::to_string(n) + ".jpg";
		n++;
	}
	ret = imwrite(fn, img);
	//fn = SCREENSHOTS_DIR + aname + "_input.jpg";
	//ret = imwrite(fn, FrameGrabber::instance()->getCurrentFrame());
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
	precision = (float)tp / (float)(tp + fp);
	std::cout << "TP: " << tp << ", TN: " << tn << ", FP:" << fp << ", FN: " << fn << std::endl;
	std::cout << "Recall: " << recall << std::endl;
	std::cout << "Precision: " << precision << std::endl;

}
