/*
 * CudaMeanShift.hpp
 *
 *  Created on: Nov 7, 2014
 *      Author: alucard
 */

#ifndef CUDAMEANSHIFT_HPP_
#define CUDAMEANSHIFT_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"
#include "Visualizer.hpp"

class CudaMeanShift : public ImageProcessor {
public:
	CudaMeanShift();
	virtual ~CudaMeanShift();

private:
	void process_implementation(Mat &a, void* data);
	std::shared_ptr<Visualizer> m_visualizer;
};

#endif /* CUDAMEANSHIFT_HPP_ */
