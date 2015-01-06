/*
 * Utility.hpp
 *
 *  Created on: Nov 11, 2014
 *      Author: alucard
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include "Common.hpp"

using namespace cv;

class Utility {
public:
	Utility();
	virtual ~Utility();

	static bool actionSwitch(const Mat& img, const std::string& aname, char key);
	static bool captureFrame(const Mat& img, const std::string& aname);
};

#endif /* UTILITY_HPP_ */
