/*
 * Utility.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: alucard
 */

#include "ImageProcessor.hpp"
#include "Utility.hpp"

Utility::Utility() {
	// TODO Auto-generated constructor stub

}

Utility::~Utility() {
	// TODO Auto-generated destructor stub
}

bool Utility::actionSwitch(const Mat& img, const std::string& aname, char key)
{
	std::string fn, fname;
	struct stat st = {0};
	int no = 0;
	switch (static_cast<char>(waitKey(1)))
	{
	case 's':
		captureFrame(img, aname);
		break;
	case 27:
		cout << "esc key is pressed by user" << endl;
		return false;
	default:
		break;
	}
	return true;
}

bool Utility::captureFrame(const Mat& img, const std::string& aname)
{
	bool ret;
	std::string fn;
	struct stat st = {0};
	int n = 1;
	if (stat(SCREENSHOTS_DIR, &st) == -1) {
		mkdir(SCREENSHOTS_DIR, 0700);
	}
	fn = SCREENSHOTS_DIR + aname + "_" + std::to_string(ImageProcessor::frame_num) + ".jpg";
	while (stat(fn.c_str(), &st) != -1) {
		fn = SCREENSHOTS_DIR + aname + "_" + std::to_string(ImageProcessor::frame_num) + "_" + std::to_string(n) + ".jpg";
		n++;
	}
	imwrite(fn, img);
	fn = SCREENSHOTS_DIR + aname + "_input.jpg";
	imwrite(fn, *ImageProcessor::current_frame);
	return ret;
}
