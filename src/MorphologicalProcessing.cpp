/*
 * MorphologicalProcessing.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */
#include "MorphologicalProcessing.hpp"

MorphologicalProcessing::MorphologicalProcessing(Type type, int size)
	: m_type(type), m_size(size)
{

}

MorphologicalProcessing::~MorphologicalProcessing() {

}

void MorphologicalProcessing::process_implementation(Mat &a, void* data)
{
	Mat kernel = getStructuringElement(MORPH_RECT, Size(m_size, m_size));
	dilate(a, a, kernel);
	switch (m_type) {
	case DILATE:
		dilate(a, a, kernel);
		break;
	case ERODE:
		erode(a, a, kernel);
		break;
	case CLOSE:
		morphologyEx(a, a, MORPH_CLOSE, kernel);
		break;
	default:
		break;
	}
}
