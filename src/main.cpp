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
	// hardcoded parameters
	// input file
	String file = "/home/alucarded/Videos/changedetection.net/dataset/dynamicBackground/overpass/input/in%06d.jpg";
	// frame number to save results
	int test_frame = 1850;
	std::string path = "/home/alucarded/Videos/changedetection.net/results/dynamicBackground/overpass/";
	std::string reference = "/home/alucarded/Videos/background_subtraction_test_dataset/Camouflage/hand_segmented_01850.BMP";
	std::shared_ptr<FrameGrabber> grabber = FrameGrabber::instantiate(file);
	Mat img;
	bool go = true;
	double t;
	// for file operations
	std::string fn, fname;

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
		if (grabber->getFrameNum() >= 0) {
		// processing
		t = (double)getTickCount();
		moe->process(img);
		t = (double)getTickCount() - t;
		t = t*1000./getTickFrequency();
		}
		cout << "execution time = " << t << " ms" << endl;
		cout << grabber->getFrameNum() << endl;

		imshow("input", grabber->getCurrentFrame());
		imshow("output", img);

		if (!Utility::actionSwitch(img, path, static_cast<char>(waitKey(1))))
			go = 0;

		// test
		//if (grabber->getFrameNum() >= 470 && grabber->getFrameNum() <= 1700) {
			Utility::captureFrame(img, path);
			//Utility::test(img, reference);
			//sleep(100);
			//break;
		//}
	}
	return 0;
}
