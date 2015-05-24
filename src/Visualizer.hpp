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

#ifndef VISUALIZER_HPP_
#define VISUALIZER_HPP_

#include "Common.hpp"

using namespace cv;

class Visualizer {
public:
	Visualizer();
	virtual ~Visualizer();

	template <typename Tl, typename Ta>
	void visualize(const Mat& l, Mat& a);

	// TODO:
	template <typename T>
	static void distribute_values_evenly(Mat &a, T max_input_val, T max_output_val);
private:
	template <typename T>
	bool _is_border(const Mat& l, const int& x, const int& y);
	Vec3b _label_to_color(const int& label);
	template <typename T>
	bool compare(const T& a, const T& b);
	// int m_channels;
};

template <typename Tl, typename Ta>
void Visualizer::visualize(const Mat& l, Mat& a)
{
	int r, g, b, v;
	for (int i = 1; i < a.rows - 1; i++)
		for (int j = 1; j < a.cols - 1; j++) {
			if (_is_border<Tl>(l, i, j)) {
				//a.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
				Ta& vec = a.at<Ta>(i, j);
				for (int k = 0; k < vec.channels; k++)
					vec.val[k] = 255;
			//a.at<Vec3b>(i, j) = _label_to_color(l.at<int>(i, j));
			}
		}
}

template <typename T>
bool Visualizer::_is_border(const Mat& l, const int& x, const int& y)
{
	assert(x > 0 && y > 0 && x < l.rows - 1 && y < l.cols - 1);
	T v = l.at<T>(x, y);
	// printf("%d %d %d %d\n", v.val[0], v.val[1], v.val[2], v.val[3]);
	return !this->compare<T>(l.at<T>(x + 1, y), v) || !this->compare<T>(l.at<T>(x - 1, y), v)
			|| !this->compare<T>(l.at<T>(x, y + 1), v) || !this->compare<T>(l.at<T>(x, y - 1), v);
}

template <typename T>
bool Visualizer::compare(const T& a, const T& b)
{
	for (int i = 0; i < a.channels; i++) {
		if (a.val[i] != b.val[i])
			return false;
	}
	return true;
}

#endif /* VISUALIZER_HPP_ */
