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

#include <iostream>

#include "FrameGrabber.hpp"

std::shared_ptr<FrameGrabber> FrameGrabber::m_instance = std::shared_ptr<FrameGrabber>(nullptr);

FrameGrabber::~FrameGrabber()
{
	m_capture.release();
}

std::shared_ptr<FrameGrabber> FrameGrabber::instantiate(int device)
{
	m_instance = std::make_shared<FrameGrabber>(device);
	return m_instance;
}

std::shared_ptr<FrameGrabber> FrameGrabber::instantiate(const std::string& file)
{
	m_instance = std::make_shared<FrameGrabber>(file);
	return m_instance;
}

std::shared_ptr<FrameGrabber> FrameGrabber::instance()
{
	if (!m_instance.get())
		FrameGrabber::instantiate(0); // initialize VideoCapture for camera device with id 0
	return m_instance;
}

FrameGrabber::FrameGrabber(int device) : m_frame_num(0)
{
	m_capture.open(device);
	if (!m_capture.isOpened())
	{
		std::cout << "Error: cannot open the video device " << device << std::endl;
	}
}

FrameGrabber::FrameGrabber(const std::string& file) : m_frame_num(0)
{
	m_capture.open(file);
	if (!m_capture.isOpened())
	{
		std::cout << "Error: cannot open the video file " << file << std::endl;
	}
}

bool FrameGrabber::read()
{
	bool ret = m_capture.read(m_current_frame);
	if (!ret) {
		std::cout << "Error: cannot read a frame" << std::endl;
		return false;
	}
	m_frame_num++;
	return true;
}
