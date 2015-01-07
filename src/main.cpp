/*
 * mmd.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: alucard
 */

#include "FrameGrabber.hpp"
#include "ProcessingChain.hpp"
#include "Utility.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	FrameGrabber grabber;
	Mat img;
	bool go = true;
	double t;
	void *data = reinterpret_cast<void*>(new Mat);
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
	if (!grabber.initialize(1))
		if (!grabber.initialize(0))
			return -1;

	/*FrameGrabber::current_frame = std::make_shared<Mat>(cap.get(CAP_PROP_FRAME_WIDTH),
			cap.get(CAP_PROP_FRAME_HEIGHT),
			cap.get(CAP_PROP_FORMAT));*/

	namedWindow( "input", WINDOW_AUTOSIZE );
	namedWindow( "output", WINDOW_AUTOSIZE );
	moveWindow("input", 0, 0);
	moveWindow("output", 800, 0);

	// processing chain
	std::shared_ptr<ProcessingChain> moe = std::make_shared<GrabCutMovingObjectExtracting>("input");

	// main loop
	while (go)
	{
		// usleep(30000);

		if (!grabber.read())
			break;

		FrameGrabber::frame_num++;
		FrameGrabber::current_frame.copyTo(img);

		// processing
		t = (double)getTickCount();
		moe->process(img, data);
		t = (double)getTickCount() - t;
		t = t*1000./getTickFrequency();

		cout << "execution time = " << t << " ms" << endl;
		cout << FrameGrabber::frame_num << endl;

		imshow("input", FrameGrabber::current_frame);
		imshow("output", img);

		if (!Utility::actionSwitch(img, aname, static_cast<char>(waitKey(1))))
			go = 0;

		// test
		if (FrameGrabber::frame_num == test_frame)
			Utility::captureFrame(img, aname);
	}
	return 0;
}
