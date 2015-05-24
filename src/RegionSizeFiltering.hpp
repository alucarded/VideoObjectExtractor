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

#ifndef REGIONSIZEFILTERING_HPP_
#define REGIONSIZEFILTERING_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

/*
 * INPUT: raw moving are mask
 * OUTPUT: moving area mask with filtered small regions
 */
class RegionSizeFiltering : public ImageProcessor {
public:
	RegionSizeFiltering(int min_white, int min_black);
	virtual ~RegionSizeFiltering();

	typedef std::vector< std::vector<int> > Neighborhood_Graph;
private:
	void process_implementation(Mat &a, void* data);
	int unify_label(const int& l);
	bool is_border(const Mat& m, const int& x, const int& y);
	int get_neighbor(const Mat& m, const Mat& lab, const int& x, const int& y);
	int get_neighbor(const Mat& lab, const int& x, const int& y);
	bool already_neighbor(const Neighborhood_Graph& g, const int& a, const int& b);

	int m_min_white;
	int m_min_black;
	std::vector<int> m_merge_proxy;
	std::vector<int> m_sizes;
	// TODO: replace with vector for time optimization
	// std::set<int> m_boundary_regions;
};

#endif /* REGIONSIZEFILTERING_HPP_ */
