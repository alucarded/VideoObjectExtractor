/*
 * RegionSizeFiltering.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: alucard
 */

#include "RegionSizeFiltering.hpp"

RegionSizeFiltering::RegionSizeFiltering(int min_white, int min_black) :
	m_min_white(min_white), m_min_black(min_black)
{

}

RegionSizeFiltering::~RegionSizeFiltering()
{

}

void RegionSizeFiltering::process_implementation(Mat &a, void* data)
{
	int i, j, lab, tl, ll, b;
	uchar t, l, v;
	// pixel labels, 0 label means no label (pixel doesn't belong to mask)
	Mat labels = Mat::zeros(a.rows, a.cols, CV_32SC1);
	Mat tmp; a.copyTo(tmp);

	m_merge_proxy.assign(a.rows*a.cols, -1);
	m_sizes.assign(a.rows*a.cols, 0);
	// m_boundary_regions.clear();

	lab = 1;
	labels.at<int>(0, 0) = lab;
	m_sizes[lab]++;

	for (i = 1; i < a.rows; i++) {
		if (a.at<uchar>(i-1, 0) != a.at<uchar>(i, 0))
			lab++;
		labels.at<int>(i, 0) = lab;
		m_sizes[lab]++;
	}
	for (j = 1; j < a.cols && a.at<uchar>(0, j-1) == a.at<uchar>(0, j); j++) {
		labels.at<int>(0, j) = 1;
		m_sizes[1]++;
	}
	for (; j < a.cols; j++) {
		if (a.at<uchar>(0, j-1) != a.at<uchar>(0, j))
			lab++;
		labels.at<int>(0, j) = lab;
		m_sizes[lab]++;
  }
	for (i = 1; i < a.rows; i++)
		for (j = 1; j < a.cols; j++) {
			t = a.at<uchar>(i-1, j); tl = unify_label(labels.at<int>(i-1, j));
			l = a.at<uchar>(i, j-1); ll = unify_label(labels.at<int>(i, j-1));
			v = a.at<uchar>(i, j);
			if (l == v && t != v) {
				labels.at<int>(i, j) = ll;
				m_sizes[ll]++;
			} else if (l != v && t == v) {
				labels.at<int>(i, j) = tl;
				m_sizes[tl]++;
			} else if (l != v && t != v) {
				lab++;
				labels.at<int>(i, j) = lab;
				m_sizes[lab]++;
			} else { // if (l == v && t == v)
				labels.at<int>(i, j) = ll;
				if (tl != ll) {
					m_merge_proxy[tl] = ll;
					m_sizes[ll] += m_sizes[tl];
					m_sizes[tl] = 0;
				}
				m_sizes[ll]++;
			}
		}

	// TODO: should create region neighborhood graph to do it properly
	Neighborhood_Graph ng(lab+1);
	std::vector<bool> rv(lab+1); // white or black
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			int &l = labels.at<int>(i, j);
			l = unify_label(labels.at<int>(i, j));
			if (BINARY_ONE == tmp.at<uchar>(i, j))
				rv[l] = 1;
			else
				rv[l] = 0;
			b = get_neighbor(tmp, labels, i, j);
			if (b && !already_neighbor(ng, l, b))
				ng[l].push_back(b);
		}
	for (i = 0; i <= lab; i++) {
		if (rv[i]) {
			for (j = 0; j < ng[i].size(); j++) {
				const int& k = ng[i][j];
				if (m_sizes[k] < m_min_black) {
					m_sizes[i] += m_sizes[k];
					m_sizes[k] = 0;
					m_merge_proxy[k] = i;
				}
			}
		}
	}
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			int &l = labels.at<int>(i, j);
			l = unify_label(labels.at<int>(i, j));
			if (rv[l] && m_sizes[l] < m_min_white)
				a.at<uchar>(i, j) = BINARY_ZERO;
			else if (rv[l])
				a.at<uchar>(i, j) = BINARY_ONE;
		}
	/*
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			int &l = labels.at<int>(i, j);
			l = unify_label(labels.at<int>(i, j));
			// remove small white regions
			if (255 == tmp.at<uchar>(i, j)) {
				//printf("%d\n", m_sizes[labels.at<int>(i, j)]);
				// TODO: update regions
				if (m_sizes[l] < m_min_white)
					a.at<uchar>(i, j) = 0;
			}
		}

	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			// get small black regions inside white regions
			if ((0 == i || 0 == j || a.rows-1 == i || a.cols-1 == j) && 0 == tmp.at<uchar>(i, j))
				m_boundary_regions.insert(l);
		}

	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			int &l = labels.at<int>(i, j);
			// fill small black regions inside white regions
			if (0 == tmp.at<uchar>(i, j) && m_boundary_regions.end() == m_boundary_regions.find(l)
					&& m_sizes[l] < m_min_black)
				// TODO: update regions
				a.at<uchar>(i, j) = 255;
		}
*/
/*
	// for mask
	a.copyTo(tmp);
	current_frame->copyTo(a);
	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++) {
			if (0 == tmp.at<uchar>(i, j))
				a.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}*/
	labels.copyTo(*reinterpret_cast<Mat*>(data));
}

int RegionSizeFiltering::unify_label(const int& l)
{
	int ret = l;
	while (m_merge_proxy[ret] != -1) {
		ret = m_merge_proxy[ret];
		if (ret == m_merge_proxy[m_merge_proxy[ret]]) {
			printf("Looper Error!\n");
			break;
		}
	}
	if (ret != l)
		m_merge_proxy[l] = ret;
	return ret;
}

bool RegionSizeFiltering::is_border(const Mat& m, const int& x, const int& y)
{
	if (x <= 0 || y <= 0 || x >= m.rows - 1 || y >= m.cols - 1)
		return false;
	const uchar &v = m.at<uchar>(x, y), &l = m.at<uchar>(x-1, y), &r = m.at<uchar>(x+1, y),
			&t = m.at<uchar>(x, y-1), &b = m.at<uchar>(x, y+1);
	if (BINARY_ONE == v)
		return false;
	return l != v || r != v || t != v || b != v;
}

int RegionSizeFiltering::get_neighbor(const Mat& m, const Mat& lab, const int& x, const int& y)
{
	if (x <= 0 || y <= 0 || x >= m.rows - 1 || y >= m.cols - 1)
		return 0;
	const uchar &v = m.at<uchar>(x, y), &l = m.at<uchar>(x-1, y), &r = m.at<uchar>(x+1, y),
			&t = m.at<uchar>(x, y-1), &b = m.at<uchar>(x, y+1);
	if (l != v)
		return lab.at<int>(x-1, y);
	if (r != v)
		return lab.at<int>(x+1, y);
	if (t != v)
		return lab.at<int>(x, y-1);
	if (b != v)
		return lab.at<int>(x, y+1);
	return 0;
}

bool RegionSizeFiltering::already_neighbor(const Neighborhood_Graph& g, const int& a, const int& b)
{
	assert(a < g.size());
	for (int i = 0; i < g[a].size(); i++) {
		if (g[a][i] == b)
			return true;
	}
	return false;
}
