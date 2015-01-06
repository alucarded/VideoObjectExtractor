/*
 * mmd.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: alucard
 */

#include "ImageProcessor.hpp"
#include "MovingObjectExtracting.hpp"
#include "Utility.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	VideoCapture cap;
	Mat img;
	bool go = true, ret;
	int no = 0;
	double t;
	void *data = reinterpret_cast<void*>(new Mat);
	std::string aname = "mog2";
	// for file operations
	std::string fn, fname;
	// hardcoded parameters
	String file = "/home/alucarded/my_video-1.mkv";
	int test_frame = 250;

	// open a video stream for reading
	cap = VideoCapture(1);

	//get video stream parameters
	// cout << "Frame size : " << cap.get(CAP_PROP_FRAME_WIDTH) << " x " << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
	// cout << "Speed:" << cap.get(CAP_PROP_ISO_SPEED)
	//				<< ", frame count: " << cap.get(CAP_PROP_FRAME_COUNT)
	//				<< ", pos avi ratio: " << cap.get(CAP_PROP_POS_AVI_RATIO) << endl;
	//cap.set(CAP_PROP_POS_AVI_RATIO, 0.01);
	// cap.set(CAP_PROP_FRAME_COUNT, 1);
	//cap.set(CAP_PROP_ISO_SPEED, 100);
	//cap.set(CAP_PROP_FORMAT, CV_8UC1); // gray image grabbing this way doesn't work

	if (!cap.isOpened())
	{
		cap.open(0);
		if (!cap.isOpened())
		{
			cout << "Cannot open the video file" << endl;
			return -1;
		}
	}

	ImageProcessor::current_frame = std::make_shared<Mat>(cap.get(CAP_PROP_FRAME_WIDTH),
			cap.get(CAP_PROP_FRAME_HEIGHT),
			cap.get(CAP_PROP_FORMAT));

	namedWindow( "input", WINDOW_AUTOSIZE );

	std::shared_ptr<MovingObjectExtracting> moe = std::make_shared<GrabCutMovingObjectExtracting>("input");

	// main loop
	while (go)
	{
		usleep(30000);

		ret = cap.read(*ImageProcessor::current_frame);
		if (!ret)
		{
			cout << "Cannot read a frame" << endl;
			break;
		}
		/*ImageProcessor::last_frames.push_back(*ImageProcessor::current_frame);
		cout << "lol " << ImageProcessor::last_frames.size() << endl;
		if (ImageProcessor::last_frames.size() > 3) {
			cout << "lol";
			ImageProcessor::last_frames.erase(ImageProcessor::last_frames.begin());
			fastNlMeansDenoisingColoredMulti(ImageProcessor::last_frames, *ImageProcessor::current_frame,
					1, 3);
		} else
			continue;*/

		ImageProcessor::frame_num++;
		ImageProcessor::current_frame->copyTo(img);

		// processing
		t = (double)getTickCount();
		moe->extract(img, data);
		t = (double)getTickCount() - t;
		ImageProcessor::exec_time = t*1000./getTickFrequency();

		cout << "execution time = " << ImageProcessor::exec_time << " ms" << endl;
		cout << ImageProcessor::frame_num << endl;

		imshow("input", *ImageProcessor::current_frame);
		imshow("output", img);
		moveWindow("input", 0, 0);
		moveWindow("output", 800, 0);

		if (!Utility::actionSwitch(img, aname, static_cast<char>(waitKey(1))))
			go = 0;

		// test an algorithm
		if (ImageProcessor::frame_num == test_frame)
			Utility::captureFrame(img, aname);
	}
	return 0;
}
