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
