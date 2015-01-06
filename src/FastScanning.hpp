/*
 * FastScanning.hpp
 *
 *  Created on: Oct 6, 2014
 *      Author: alucard
 */

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
