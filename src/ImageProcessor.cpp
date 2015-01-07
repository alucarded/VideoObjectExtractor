/*
 * ImageProcessor.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: alucard
 */

#include <cassert>

#include "ImageProcessor.hpp"

void ImageProcessor::process(Mat &a, void* data)
{
  process_implementation(a, data);
  if (m_next_processor.get() != 0)
    {
      m_next_processor->process(a, data);
    }
}

void ImageProcessor::set_next_processor(ImageProcessor *p)
{
  assert(p != 0);
  m_next_processor = std::shared_ptr<ImageProcessor>(p);
}

void ImageProcessor::set_next_processor(std::shared_ptr<ImageProcessor> p)
{
  assert(p != 0);
  m_next_processor = p;
}
