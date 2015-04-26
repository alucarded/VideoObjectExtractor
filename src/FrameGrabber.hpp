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
	enum {
		CAMERA,
		VIDEO,
		IMAGES
	};
	virtual ~FrameGrabber();
	virtual bool read();
	inline cv::Mat getCurrentFrame() const { return m_current_frame; }
	inline cv::Mat& getCurrentFrame() { return m_current_frame; }
	inline int getFrameNum() const { return m_frame_num; }

	static std::shared_ptr<FrameGrabber> instance();
	static std::shared_ptr<FrameGrabber> instantiate(int device);
	static std::shared_ptr<FrameGrabber> instantiate(const std::string& file);
	static std::shared_ptr<FrameGrabber> instantiate(const std::string& path, const std::string& prefix, const std::string& suffix);

protected:
	FrameGrabber();
	static std::shared_ptr<FrameGrabber> m_instance;

	cv::Mat m_current_frame;
	int m_frame_num;
};

class ImageFrameGrabber : public FrameGrabber {
public:
	virtual ~ImageFrameGrabber();
	ImageFrameGrabber(const std::string& path, const std::string& prefix, const std::string& suffix);

	virtual bool read();
private:

};

class VideoFrameGrabber : public FrameGrabber {
public:
	/**
	 * @param video device to read from.
	 */
	VideoFrameGrabber(int device);
	/**
	 * @param video file to read from.
	 */
	VideoFrameGrabber(const std::string& file);
	virtual ~VideoFrameGrabber();

	virtual bool read();

private:

	cv::VideoCapture m_capture;
};

#endif /* SRC_FRAMEGRABBER_HPP_ */
