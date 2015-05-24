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
