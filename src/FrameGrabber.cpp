/*
 * FrameGrabber.cpp
 *
 *  Created on: Jan 7, 2015
 *      Author: alucarded
 */

#include <iostream>

#include "FrameGrabber.hpp"

cv::Mat FrameGrabber::current_frame;
int FrameGrabber::frame_num = 0;

FrameGrabber::FrameGrabber() {
	// m_capture = cv::VideoCapture();
}

FrameGrabber::~FrameGrabber() {
	m_capture.release();
}

bool FrameGrabber::initialize(int device)
{
	m_capture.open(device);
	if (!m_capture.isOpened())
	{
		std::cout << "Error: cannot open the video device " << device  << std::endl;
		return false;
	}
	return true;
}

bool FrameGrabber::initialize(const std::string& file)
{
	m_capture.open(file);
	if (!m_capture.isOpened())
	{
		std::cout << "Error: cannot open the video file " << file  << std::endl;
		return false;
	}
	return true;
}

bool FrameGrabber::read()
{
	bool ret = m_capture.read(current_frame);
	if (!ret)
	{
		std::cout << "Error: cannot read a frame" << std::endl;
		return false;
	}
	return true;
}
