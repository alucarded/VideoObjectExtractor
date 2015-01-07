/*
 * Utility.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: alucard
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
}
