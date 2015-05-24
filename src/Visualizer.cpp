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

#include "Visualizer.hpp"

Visualizer::Visualizer()// : m_channels(cn)
{
	// TODO Auto-generated constructor stub

}

Visualizer::~Visualizer()
{
	// TODO Auto-generated destructor stub
}

Vec3b Visualizer::_label_to_color(const int& label)
{
	typedef long long int lli;
	//printf("max: %d\n", m_max_label);
	Vec3b ret;
	lli v = ((lli)label*256L*256L*256L)/(lli)2000LL;
	ret.val[0] = static_cast<uchar>(v % 256L);
	v /= 256L;
	ret.val[1] = static_cast<uchar>(v % 256L);
	v /= 256L;
	ret.val[2] = static_cast<uchar>(v % 256L);
	//printf(" %u %u %u % d %d\n", ret.val[0], ret.val[1], ret.val[2], label, m_max_label);
	return ret;
}
