/*
 * SigmaDeltaFiltering.hpp
 *
 *  Created on: Nov 11, 2014
 *      Author: alucard
 */

#ifndef SIGMADELTAFILTERING_HPP_
#define SIGMADELTAFILTERING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

class SigmaDeltaFiltering : public ImageProcessor {
public:
	SigmaDeltaFiltering();
	virtual ~SigmaDeltaFiltering();

private:
	void process_implementation(Mat &a, void* data);
};

#endif /* SIGMADELTAFILTERING_HPP_ */
