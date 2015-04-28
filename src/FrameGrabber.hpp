/*
 * FrameGrabber.hpp
 *
 *  Created on: Jan 7, 2015
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#ifndef SRC_FRAMEGRABBER_HPP_
#define SRC_FRAMEGRABBER_HPP_

#include <string>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class FrameGrabber {
public:
	virtual ~FrameGrabber();
	/**
	 * @param video device to read from.
	 */
	FrameGrabber(int device);
	/**
	 * @param video file or image sequence to read from.
	 */
	FrameGrabber(const std::string& file);
	virtual bool read();
	inline cv::Mat getCurrentFrame() const { return m_current_frame; }
	inline cv::Mat& getCurrentFrame() { return m_current_frame; }
	inline int getFrameNum() const { return m_frame_num; }

	static std::shared_ptr<FrameGrabber> instance();
	static std::shared_ptr<FrameGrabber> instantiate(int device);
	static std::shared_ptr<FrameGrabber> instantiate(const std::string& file);

private:
	static std::shared_ptr<FrameGrabber> m_instance;

	cv::Mat m_current_frame;
	int m_frame_num;

	cv::VideoCapture m_capture;
};

#endif /* SRC_FRAMEGRABBER_HPP_ */
