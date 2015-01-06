/*
 * ObjectsDesignating.hpp
 *
 *  Created on: Nov 11, 2014
 *      Author: alucard
 */

#ifndef OBJECTSDESIGNATING_HPP_
#define OBJECTSDESIGNATING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

class ObjectsDesignating : public ImageProcessor {
public:
	ObjectsDesignating();
	virtual ~ObjectsDesignating();

private:
	void process_implementation(Mat &a, void* data);

	bool is_border(const Mat& m, const int& x, const int& y);
};

#endif /* OBJECTSDESIGNATING_HPP_ */
