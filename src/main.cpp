/*
 * mmd.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#include "FrameGrabber.hpp"
#include "ProcessingChain.hpp"
#include "Utility.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	std::shared_ptr<FrameGrabber> grabber = VideoFrameGrabber::instantiate(0);
	Mat img;
	bool go = true;
	double t;
	std::string aname = "mog2";
	// for file operations
	std::string fn, fname;
	// hardcoded parameters
	// input file
	String file = "/home/alucarded/my_video-1.mkv";
	// frame number to save results
	int test_frame = 250;

	// open a video stream for reading
	// from device with id as in argument
	//if (!grabber->initialize(1))
	//	if (!grabber->initialize(0))
	//		return -1;

	namedWindow( "input", WINDOW_AUTOSIZE );
	namedWindow( "output", WINDOW_AUTOSIZE );
	moveWindow("input", 0, 0);
	moveWindow("output", 800, 0);

	// processing chain, here change between different available chains
	std::shared_ptr<ProcessingChain> moe = std::make_shared<GrabCutMovingObjectExtracting>("input");

	// main loop
	while (go)
	{
		// usleep(30000);

		if (!grabber->read())
			break;

		img = grabber->getCurrentFrame();

		// processing
		t = (double)getTickCount();
		moe->process(img);
		t = (double)getTickCount() - t;
		t = t*1000./getTickFrequency();

		cout << "execution time = " << t << " ms" << endl;
		cout << grabber->getFrameNum() << endl;

		imshow("input", grabber->getCurrentFrame());
		imshow("output", img);

		if (!Utility::actionSwitch(img, aname, static_cast<char>(waitKey(1))))
			go = 0;

		// test
		if (grabber->getFrameNum() == test_frame)
			Utility::captureFrame(img, aname);
	}
	return 0;
}
