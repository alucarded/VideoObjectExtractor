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

#ifndef IMAGEPROCESSOR_HPP_
#define IMAGEPROCESSOR_HPP_

#include <memory>
#include <queue>
#include <string>

#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class ImageProcessor
{
public:
  ImageProcessor() : m_next_processor(0), m_name("") { }
  virtual ~ImageProcessor() { }

  void process(Mat &a, void* data);
  void set_next_processor(ImageProcessor *p);
  void set_next_processor(std::shared_ptr<ImageProcessor> p);

protected:
  virtual void process_implementation(Mat &a, void* data) = 0;
  std::string m_name;
private:
  std::shared_ptr<ImageProcessor> m_next_processor;

};

class NullProcessor : public ImageProcessor
{
public:
  NullProcessor() { }
  ~NullProcessor() { }

private:
  virtual void process_implementation(Mat &a, void* data) { };

};


#endif /* IMAGEPROCESSOR_HPP_ */
