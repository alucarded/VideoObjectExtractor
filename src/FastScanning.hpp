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

#ifndef FASTSCANNING_HPP_
#define FASTSCANNING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"
#include "Visualizer.hpp"

class FastScanning : public ImageProcessor {
public:
	FastScanning(double threshold, int min_size = 100);
	virtual ~FastScanning();

	void initialize(VideoCapture& cap);

	const vector<int>& get_sizes() { return m_sizes; }
	int get_max_label() { return m_max_label; }

private:
	void process_implementation(Mat &a, void* data);
	int unify_label(const int& l);

	double m_threshold;
	int m_min_size;
	Mat m_labels;
	vector<Vec3d> m_means;
	vector<int> m_sizes;
	vector<int> m_merge_proxy;
	// TODO: using set may not be optimal (though convenient)
	vector< set<int> > m_adj;
	std::shared_ptr<Visualizer> m_visualizer;
	int m_max_label;
};

#endif /* FASTSCANNING_HPP_ */
