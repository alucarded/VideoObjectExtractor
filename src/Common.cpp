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

#include "Common.hpp"

template <typename T>
T distance(const cv::Vec<T, 2>& a, const cv::Vec<T, 2>& b)
{
  T dx, dy, dist;
  dx = a[0] - b[0];
  dy = a[1] - b[1];
  dist = (T)(sqrt((double)(dx*dx + dy*dy)));
  return dist;
}

double norm_approximation(const double &dx, const double &dy)
{
	return 0.41*dx + 0.941246*dy;
}
