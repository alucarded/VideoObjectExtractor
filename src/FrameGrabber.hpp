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
	virtual ~FrameGrabber();

	static FrameGrabber *instance();

	bool initialize(int device);
	bool initialize(const std::string& file);
	bool read();

	inline cv::Mat getCurrentFrame() const { return m_current_frame; }
	inline cv::Mat& getCurrentFrame() { return m_current_frame; }
	inline int getFrameNum() const { return m_frame_num; }

private:
	FrameGrabber();
	static FrameGrabber *m_instance;

	cv::VideoCapture m_capture;
	cv::Mat m_current_frame;
	int m_frame_num;
};

#endif /* SRC_FRAMEGRABBER_HPP_ */
