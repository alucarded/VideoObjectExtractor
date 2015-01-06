/*
 * MorphologicalProcessing.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: alucard
 */

#include "MorphologicalProcessing.hpp"

MorphologicalProcessing::MorphologicalProcessing() {
	// TODO Auto-generated constructor stub

}

MorphologicalProcessing::~MorphologicalProcessing() {
	// TODO Auto-generated destructor stub
}

void MorphologicalProcessing::process_implementation(Mat &a, void* data)
{
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(a, a, MORPH_CLOSE, kernel);
	dilate(a, a, kernel);
}
