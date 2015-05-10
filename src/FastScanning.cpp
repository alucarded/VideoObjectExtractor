/*
 * FastScanning.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

#include "FastScanning.hpp"
#include "FrameGrabber.hpp"

#define MAX_DISTANCE 195075.0f // 3*255*255

typedef Vec<int, 1> Vec1d;

FastScanning::FastScanning(double threshold, int min_size) :
	m_threshold(threshold), m_min_size(min_size), m_max_label(0)
{

}

FastScanning::~FastScanning()
{

}

void FastScanning::initialize(VideoCapture& cap)
{

	bool ret = cap.read(FrameGrabber::instance()->getCurrentFrame());
	if (!ret) {
		printf("Your camera is not responding :(");
		return;
	}
	// we prepare memory for pessimistic case - every pixel is a region (not memory efficient)
	// need to change that - make some restrictions for minimum number of pixels in a region
	int pxs = FrameGrabber::instance()->getCurrentFrame().rows
			*FrameGrabber::instance()->getCurrentFrame().cols;
	m_means.resize(pxs);
	m_sizes.resize(pxs);
}

void FastScanning::process_implementation(Mat &a, void* data)
{
	int i, j, l, t, v, max_label;
	Vec3d val, dvala, dvalb;
	double dista, distb;

	// clear data
	m_labels.deallocate();
	m_means.clear();
	m_sizes.clear();
	m_merge_proxy.clear();
	m_adj.clear();

	// initialize
	// labels initialization, 0-label means no label assignment
	m_labels = Mat::zeros(a.rows, a.cols, CV_32SC1);
	// we prepare memory for pessimistic case - every pixel is a region
	int pxs = FrameGrabber::instance()->getCurrentFrame().rows
			*FrameGrabber::instance()->getCurrentFrame().cols;
	m_means.assign(pxs, 0);
	m_sizes.assign(pxs, 0);
	m_merge_proxy.assign(pxs, -1);
	m_adj.assign(pxs, set<int>());
	l = 0;
	m_labels.at<int>(0, 0) = l;
	m_means[0] = static_cast<Vec3d>(a.at<Vec3b>(0, 0));
	m_sizes[0] = 1;

	// fast scanning
	for (j = 1; j < a.cols; j++) { // i = 0
		val = static_cast<Vec3d>(a.at<Vec3b>(0, j));
		dvala = val - m_means[l];
		// Mahalanobis distance with identity covariance matrix = euclidean distance
		dista = dvala.val[0]*dvala.val[0] + dvala.val[1]*dvala.val[1] + dvala.val[2]*dvala.val[2];
		if (dista <= m_threshold) {
			m_labels.at<int>(0, j) = l;
			m_sizes[l]++;
		} else {
			m_labels.at<int>(0, j) = ++l;
			m_means[l] = val;
			m_sizes[l] = 1;
		}
	}
	max_label = l;
	t = m_labels.at<int>(0, 0);
	for (i = 1; i < a.rows; i++) { // j = 0
		val = static_cast<Vec3d>(a.at<Vec3b>(i, 0));
		dvalb = val - m_means[t];
		distb = dvalb.val[0]*dvalb.val[0] + dvalb.val[1]*dvalb.val[1] + dvalb.val[2]*dvalb.val[2];
		if (distb <= m_threshold) {
			m_labels.at<int>(i, 0) = t;
			m_sizes[t]++;
		} else {
			m_labels.at<int>(i, 0) = t = ++max_label;
			m_means[l] = val;
			m_sizes[l] = 1;
		}
	}
	for (i = 1; i < a.rows; i++)
		for (j = 1; j < a.cols; j++) {
			l = unify_label(m_labels.at<int>(i, j-1));
			t = unify_label(m_labels.at<int>(i-1, j));
			val = static_cast<Vec3d>(a.at<Vec3b>(i, j));
			dvala = val - m_means[l];
			dvalb = val - m_means[t];
			dista = dvala.val[0]*dvala.val[0] + dvala.val[1]*dvala.val[1] + dvala.val[2]*dvala.val[2];
			distb = dvalb.val[0]*dvalb.val[0] + dvalb.val[1]*dvalb.val[1] + dvalb.val[2]*dvalb.val[2];
			if (dista <= m_threshold && distb > m_threshold) {
				m_labels.at<int>(i, j) = l;
				m_sizes[l]++;
			} else if (dista > m_threshold && distb <= m_threshold) {
				m_labels.at<int>(i, j) = t;
				m_sizes[t]++;
			} else if (dista > m_threshold && distb > m_threshold) {
				m_labels.at<int>(i, j) = ++max_label;
				m_means[max_label] = val;
				m_sizes[max_label] = 1;
			} else if (dista <= m_threshold && distb <= m_threshold) {
				if (l == t) {
					m_labels.at<int>(i, j) = l;
					m_sizes[l]++;
				} else {
					m_labels.at<int>(i, j) = l;
					m_means[l] = (m_means[l] + m_means[t])/2;
					m_sizes[l] += m_sizes[t] + 1;
					m_merge_proxy[t] = l;
					m_sizes[t] = 0;
				}
			}

		}
	m_max_label = l; // keep maximum label id

	// update regions' labels, create neighborhood graph
	for (i = 0; i < a.rows; i++) {
		m_labels.at<int>(i, 0) = unify_label(m_labels.at<int>(i, 0));
	}
	for (j = 0; j < a.rows; j++) {
		m_labels.at<int>(0, j) = unify_label(m_labels.at<int>(0, j));
	}
	for (i = 1; i < a.rows; i++) {
		for (j = 1; j < a.cols; j++) {
			v = m_labels.at<int>(i, j) = unify_label(m_labels.at<int>(i, j));
			l = m_labels.at<int>(i-1, j);
			t = m_labels.at<int>(i, j-1);
			if (v != l) {
				m_adj[v].insert(l);
				m_adj[l].insert(v);
			}
			if (v != t) {
				m_adj[v].insert(t);
				m_adj[t].insert(v);
			}
		}
	}

	m_merge_proxy.assign(pxs, -1);
	// merge small regions
	double eval, p;
	set<int>::iterator it;
	for (i = 0; i < m_adj.size(); i++) {
		// TODO: optimize it
		if (!m_adj[i].empty() && m_sizes[i] < m_min_size) {
			eval = MAX_DISTANCE;
			v = -1;
			for (it = m_adj[i].begin(); it != m_adj[i].end(); it++) {
				l = unify_label(*it);
				if (l != i) {
					val = m_means[l]-m_means[i];
					// here we include area size,
					// so that large background areas don't eat up objects with similar color
					// and small areas stick together
					p = val.val[0]*val.val[0] + val.val[1]*val.val[1] + val.val[2]*val.val[2] + m_sizes[l];
					if (eval > p) {
						eval = p;
						v = l;
					}
				}
			}
			if (v != -1) {
				m_merge_proxy[i] = v;
				m_means[v] = (m_means[v]*m_sizes[v] + m_means[i]*m_sizes[i])/(m_sizes[v]+m_sizes[i]);
				m_sizes[v] += m_sizes[i];
				m_sizes[i] = 0;
				m_adj[v].erase(i);
				m_adj[i].erase(v);
				m_adj[v].insert(m_adj[i].begin(), m_adj[i].end());
			}
		}
	}
	// update regions' labels
	for (i = 0; i < a.rows; i++) {
		for (j = 0; j < a.cols; j++) {
			m_labels.at<int>(i, j) = unify_label(m_labels.at<int>(i, j));
		}
	}
	//m_visualizer->visualize<Vec1d, Vec3b>(m_labels, a);
	m_labels.copyTo(a);

}

int FastScanning::unify_label(const int& l)
{
	int ret = l;
	while (m_merge_proxy[ret] != -1) {
		ret = m_merge_proxy[ret];
		/*if (ret == m_merge_proxy[m_merge_proxy[ret]]) {
			printf("Looper Error!\n");
			break;
		}*/
	}
	if (ret != l)
		m_merge_proxy[l] = ret;
	return ret;
}
