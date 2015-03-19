/*
 * ImageProcessor.hpp
 *
 *  Created on: Sep 30, 2013
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

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
