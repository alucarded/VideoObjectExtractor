/*
 * CudaMeanShift.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: alucard
 */

#include <opencv2/cudaimgproc.hpp>

#include "CudaMeanShift.hpp"

CudaMeanShift::CudaMeanShift() : m_visualizer(new Visualizer) {
	// TODO Auto-generated constructor stub

}

CudaMeanShift::~CudaMeanShift() {
	// TODO Auto-generated destructor stub
}

void CudaMeanShift::process_implementation(Mat &a, void* data)
{
	// Mat res;
	Mat tmp;
	a.copyTo(tmp);
	cuda::GpuMat cuda_img;
	cuda_img.upload(tmp);
	cuda::meanShiftSegmentation(cuda_img, tmp, 6, 12, 5000);
	m_visualizer->visualize<Vec4b, Vec4b>(tmp, a);
	//cuda_img.download(a);
}
