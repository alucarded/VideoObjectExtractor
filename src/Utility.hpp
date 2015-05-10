/*
 * Utility.hpp
 *
 *  Created on: Nov 11, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include "Common.hpp"

using namespace cv;

/**
* An utility class.
*/
class Utility {
public:
	Utility();
	virtual ~Utility();

	/**
	* Action decision on user key press.
	* @param current frame.
	* @param name of process.
	* @param key pressed.
	* @return true on action, false on exit key pressed
	*/
	static bool actionSwitch(const Mat& img, const std::string& path, char key);
	/**
	* Save input and output images.
	* @param current frame.
	* @param name of process.
	* @return true on success, false on failure
	*/
	static bool captureFrame(const Mat& img, const std::string& path);

	/**
	* Compare result to ground truth. Extract statistical data.
	* @param result image.
	* @param path to ground truth image.
	*/
	static void test(const Mat& img, const std::string& reference);
};

#endif /* UTILITY_HPP_ */
