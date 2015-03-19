/*
 * MorphologicalProcessing.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */
#ifndef MORPHOLOGICALPROCESSING_HPP_
#define MORPHOLOGICALPROCESSING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

/*
 * INPUT: binary image
 * OUTPUT: binary image after morphological processing
 */
class MorphologicalProcessing : public ImageProcessor {
public:
	enum Type {
		DILATE,
		ERODE,
		CLOSE
		// add more
	};
	MorphologicalProcessing(Type type, int size);
	virtual ~MorphologicalProcessing();

private:
	void process_implementation(Mat &a, void* data);

	Type m_type;
	int m_size;
};

#endif /* MORPHOLOGICALPROCESSING_HPP_ */
