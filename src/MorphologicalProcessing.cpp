// This file is part of VideoObjectExtractor.
//
// Copyright 2014-2015 Tomasz Posłuszny
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights  to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

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
