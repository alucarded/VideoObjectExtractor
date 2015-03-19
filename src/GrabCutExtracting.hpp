/*
 * GrabCutExtracting.hpp
 *
 *  Created on: Nov 16, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#ifndef SRC_GRABCUTEXTRACTING_HPP_
#define SRC_GRABCUTEXTRACTING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

class GrabCutExtracting : public ImageProcessor {
public:
	GrabCutExtracting();
	virtual ~GrabCutExtracting();

	void setKernelSize(int ksize) { m_kernel_size = ksize; }
private:
	void process_implementation(Mat &a, void* data);

	void get_bin_mask(const Mat& comMask, Mat& binMask);

	int m_kernel_size;
};

#endif /* SRC_GRABCUTEXTRACTING_HPP_ */
