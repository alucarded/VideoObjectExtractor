/*
 * FrameGrabber.cpp
 *
 *  Created on: Jan 7, 2015
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#include <iostream>

#include "FrameGrabber.hpp"

FrameGrabber *FrameGrabber::m_instance = NULL;

FrameGrabber::FrameGrabber() : m_frame_num(0)
{

}

FrameGrabber::~FrameGrabber()
{
	m_capture.release();
}

FrameGrabber *FrameGrabber::instance()
{
	if (!m_instance)
		m_instance = new FrameGrabber();
	return m_instance;
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
	bool ret = m_capture.read(m_current_frame);
	if (!ret) {
		std::cout << "Error: cannot read a frame" << std::endl;
		return false;
	}
	m_frame_num++;
	return true;
}
