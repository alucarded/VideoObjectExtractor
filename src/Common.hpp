/*
 * Common.h
 *
 *  Created on: Dec 27, 2013
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <algorithm>
#include <cmath>
#include <climits>
#include <ctime>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <cassert>
#include <memory>
#include <fstream>
#include <utility>
#include <set>

// file operations
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>

#define SCREENSHOTS_DIR "../screenshots/"
#define TEMPLATE_IMAGE SCREENSHOTS_DIR"template.jpg"
#define RONDO_JAZDY "/home/alucard/fat_storage/Rondo_Jazdy_1.MXF"
#define STADION "/home/alucard/fat_storage/Stadion.MXF"

#define COUNT_PIXELS(a) (a).rows*(a).cols

enum Label
{
	BINARY_ZERO = 0,
	LABEL_A = 100,
	LABEL_B = 101,
	LABEL_C = 102,
	BINARY_HALF = 127,
	BINARY_ONE = 255
};

#define FOR_PIXELS(i, j, a) for (int i = 0; i < a.rows; i++) \
                           for (int j = 0; j < a.cols; j++)

// some hardcoded parameters for quick changes and testing
#define GC_KERNEL_SIZE 9

typedef std::pair<int, int> pii;

namespace voe {

template <typename T>
T distance(const cv::Vec<T, 2>& a, const cv::Vec<T, 2>& b);

// 1) dx and dy are absolute values
// 2) dx < dy
double norm_approximation(const double &dx, const double &dy);

}

#endif /* COMMON_H_ */
