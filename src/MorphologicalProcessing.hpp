/*
 * MorphologicalProcessing.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: alucard
 */

#ifndef MORPHOLOGICALPROCESSING_HPP_
#define MORPHOLOGICALPROCESSING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

class MorphologicalProcessing : public ImageProcessor {
public:
	MorphologicalProcessing();
	virtual ~MorphologicalProcessing();

private:
	void process_implementation(Mat &a, void* data);
};

#endif /* MORPHOLOGICALPROCESSING_HPP_ */
