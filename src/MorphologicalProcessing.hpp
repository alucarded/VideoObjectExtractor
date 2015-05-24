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
