/*
 * FrameGrabber.hpp
 *
 *  Created on: Jan 7, 2015
 *      Author: alucarded
 */

#ifndef SRC_FRAMEGRABBER_HPP_
#define SRC_FRAMEGRABBER_HPP_

#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class FrameGrabber {
public:
	FrameGrabber();
	virtual ~FrameGrabber();

	bool initialize(int device);
	bool initialize(const std::string& file);
	bool read();

	static cv::Mat current_frame;
	static int frame_num;
private:
	cv::VideoCapture m_capture;
};

#endif /* SRC_FRAMEGRABBER_HPP_ */
