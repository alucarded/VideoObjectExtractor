/*
 * GrabCutExtracting.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#include "FrameGrabber.hpp"
#include "GrabCutExtracting.hpp"
#include "MorphologicalProcessing.hpp"
#include "RegionSizeFiltering.hpp"

GrabCutExtracting::GrabCutExtracting() : m_kernel_size(9)
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
	Mat kernel = getStructuringElement(MORPH_RECT, Size(m_kernel_size, m_kernel_size));
	Mat gc_mask(a.rows, a.cols, CV_8UC1);
	int fgd = 0, bgd = 0;
	int i, j;

	// TODO: think more how to do it to get best results
	erode(a, gc_fgd, kernel);
	// everything beyond eroded mask is assumed to be background
	// imshow("Eroded mask", gc_fgd);
	dilate(a, a, kernel);
	// everything inside dilated mask is assumed to be foreground
	// imshow("Dilated mask", a);

	// grabcut input mask visualization
	Mat mvis(a.rows, a.cols, CV_8UC1);
	// create grabcut mask
	FOR_PIXELS(i, j, a)
		if (BINARY_ONE == gc_fgd.at<uchar>(i, j)) {
			gc_mask.at<uchar>(i, j) = GC_FGD;
			fgd++;
			mvis.at<uchar>(i, j) = BINARY_ONE;
		}
		else if (BINARY_ZERO == a.at<uchar>(i, j)) {
			gc_mask.at<uchar>(i, j) = GC_BGD;
			bgd++;
			mvis.at<uchar>(i, j) = BINARY_ZERO;
		} else {
			gc_mask.at<uchar>(i, j) = GC_PR_FGD;
			fgd++;
			mvis.at<uchar>(i, j) = BINARY_HALF;
		}

	// if foreground or background mask is very small, then return original image
	if (bgd < 1000 || fgd < 1000) {
		//current_frame.copyTo(a);
		a = Mat::zeros(a.rows, a.cols, CV_8UC1);
		return;
	}

	// execute grabcut
	grabCut(current_frame, gc_mask, dummy_rect, bgd_model, fgd_model, 1, GC_INIT_WITH_MASK);

	// show cut out image (silhouette)
	get_bin_mask(gc_mask, bmask);
	// current_frame.copyTo(a, bmask);

	// gc_mask = gc_mask + 1;
	// applyColorMap(gc_mask, mvis, COLORMAP_JET);
	imshow("GrabCut mask", mvis);
/*
	for (i = 1; i < a.rows; i++)
		for (j = 1; j < a.cols; j++) {
			if (Vec3b(0, 0, 0) == a.at<Vec3b>(i, j)) {
				a.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}*/

	// make foreground pixels white
	FOR_PIXELS(i,j,bmask) {
		if (bmask.at<uchar>(i, j) > 0)
			bmask.at<uchar>(i, j) = 255;
	}
	// result: foreground binary mask
	bmask.copyTo(a);
}

void GrabCutExtracting::get_bin_mask(const Mat& com_mask, Mat& bin_mask)
{
    if( com_mask.empty() || com_mask.type()!=CV_8UC1 )
        CV_Error( Error::StsBadArg, "com_mask is empty or has incorrect type (not CV_8UC1)" );
    if( bin_mask.empty() || bin_mask.rows!=com_mask.rows || bin_mask.cols!=com_mask.cols )
        bin_mask.create( com_mask.size(), CV_8UC1 );
    // GC_FGD and GC_PR_FGD become 1, GC_BGD and GC_PR_BGD become 0
    bin_mask = com_mask & 1;
}
