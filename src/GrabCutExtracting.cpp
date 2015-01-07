/*
 * GrabCutExtracting.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: alucarded
 */

#include "FrameGrabber.hpp"
#include "GrabCutExtracting.hpp"
#include "MorphologicalProcessing.hpp"
#include "RegionSizeFiltering.hpp"

GrabCutExtracting::GrabCutExtracting() : m_kernel_size(15)
{

}

GrabCutExtracting::~GrabCutExtracting()
{

}

void GrabCutExtracting::process_implementation(Mat &a, void* data)
{
	const Mat& current_frame = FrameGrabber::instance()->getCurrentFrame();
	Mat gc_fgd;
	Rect dummy_rect;
	Mat bmask;
	Mat bgd_model;
	Mat fgd_model;
	Mat kernel = Mat::ones(m_kernel_size, m_kernel_size, CV_8UC1);
	Mat gc_mask(a.rows, a.cols, CV_8UC1);
	int fgd = 0, bgd = 0;
	int i, j;

	// create grabcut mask
	erode(a, gc_fgd, kernel);
	imshow("Foreground", gc_fgd);
	FOR_PIXELS(i, j, a)
		if (BINARY_ONE == gc_fgd.at<uchar>(i, j)) {
			gc_mask.at<uchar>(i, j) = GC_FGD;
			fgd++;
		}
		else if (BINARY_ZERO == a.at<uchar>(i, j)) {
			gc_mask.at<uchar>(i, j) = GC_BGD;
			bgd++;
		} else {
			gc_mask.at<uchar>(i, j) = GC_PR_BGD;
			bgd++;
		}

	if (bgd < 1000 || fgd < 1000) {
		current_frame.copyTo(a);
		return;
	}

	// execute grabcut
	grabCut(current_frame, gc_mask, dummy_rect, bgd_model, fgd_model, 1, GC_INIT_WITH_MASK);

	// show cut image
	get_bin_mask(gc_mask, bmask);
	current_frame.copyTo(a, bmask);
	/*
	for (i = 1; i < a.rows; i++)
		for (j = 1; j < a.cols; j++) {
			if (Vec3b(0, 0, 0) == a.at<Vec3b>(i, j)) {
				a.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}*/
	//gc_fgd.copyTo(a);
}

void GrabCutExtracting::get_bin_mask(const Mat& com_mask, Mat& bin_mask)
{
    if( com_mask.empty() || com_mask.type()!=CV_8UC1 )
        CV_Error( Error::StsBadArg, "com_mask is empty or has incorrect type (not CV_8UC1)" );
    if( bin_mask.empty() || bin_mask.rows!=com_mask.rows || bin_mask.cols!=com_mask.cols )
        bin_mask.create( com_mask.size(), CV_8UC1 );
    bin_mask = com_mask & 1;
}

