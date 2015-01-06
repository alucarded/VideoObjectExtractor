/*
 * ObjectsDesignating.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: alucard
 */

#include "FastScanning.hpp"

#include "ObjectsDesignating.hpp"

ObjectsDesignating::ObjectsDesignating() {
	// TODO Auto-generated constructor stub

}

ObjectsDesignating::~ObjectsDesignating() {
	// TODO Auto-generated destructor stub
}

void ObjectsDesignating::process_implementation(Mat &a, void* data)
{
	Mat tmp;
	int i, j, lab;
	vector<int> w(a.rows*a.cols, 0);
	std::shared_ptr<FastScanning> fs = std::make_shared<FastScanning>(100.0f, 0);
	// for debug
	//current_frame->copyTo(a);
	//m_visualizer->visualize(labels, a);

	current_frame->copyTo(tmp);
	fs->process(tmp, NULL);

	const vector<int>& vs = fs->get_sizes();
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			lab = tmp.at<int>(i, j);
			if (0 == a.at<uchar>(i, j))
				w[lab]++;
		}
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			lab = tmp.at<int>(i, j);
			if (w[lab] > (int)(0.2f*(double)vs[lab]))
				a.at<uchar>(i, j) = 0;
		}

	// for mask
	a.copyTo(tmp);
	current_frame->copyTo(a);
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			if (0 == tmp.at<uchar>(i, j))
				a.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}

}

bool ObjectsDesignating::is_border(const Mat& m, const int& x, const int& y)
{
	if (x <= 0 || y <= 0 || x >= m.rows - 1 || y >= m.cols - 1)
		return false;
	const uchar &v = m.at<uchar>(x, y), &l = m.at<uchar>(x-1, y), &r = m.at<uchar>(x+1, y),
			&t = m.at<uchar>(x, y-1), &b = m.at<uchar>(x, y+1);
	if (BINARY_ONE == v)
		return false;
	return l != v || r != v || t != v || b != v;
}
