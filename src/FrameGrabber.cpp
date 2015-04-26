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

FrameGrabber::FrameGrabber() : m_frame_num(0)
{

}

FrameGrabber::~FrameGrabber()
{

}

std::shared_ptr<FrameGrabber> FrameGrabber::instantiate(int device)
{
	m_instance = std::make_shared<VideoFrameGrabber>(device);
	return m_instance;
}

std::shared_ptr<FrameGrabber> FrameGrabber::instantiate(const std::string& file)
{
	m_instance = std::make_shared<VideoFrameGrabber>(file);
	return m_instance;
}

std::shared_ptr<FrameGrabber> FrameGrabber::instantiate(const std::string& path, const std::string& prefix, const std::string& suffix)
{
	m_instance = std::make_shared<ImageFrameGrabber>(path, prefix, suffix);
	return m_instance;
}

std::shared_ptr<FrameGrabber> FrameGrabber::instance()
{
	if (!m_instance.get())
		FrameGrabber::instantiate(0); // initialize VideoCapture for camera device with id 0
	return m_instance;
}

VideoFrameGrabber::VideoFrameGrabber(int device)
{
	m_capture.open(device);
	if (!m_capture.isOpened())
	{
		std::cout << "Error: cannot open the video device " << device << std::endl;
	}
}

VideoFrameGrabber::VideoFrameGrabber(const std::string& file)
{
	m_capture.open(file);
	if (!m_capture.isOpened())
	{
		std::cout << "Error: cannot open the video file " << file << std::endl;
	}
}

VideoFrameGrabber::~VideoFrameGrabber()
{
	m_capture.release();
}

ImageFrameGrabber::ImageFrameGrabber(const std::string& path, const std::string& prefix, const std::string& suffix)
{

}

ImageFrameGrabber::~ImageFrameGrabber()
{

}

bool VideoFrameGrabber::read()
{
	bool ret = m_capture.read(m_current_frame);
	if (!ret) {
		std::cout << "Error: cannot read a frame" << std::endl;
		return false;
	}
	m_frame_num++;
	return true;
}
