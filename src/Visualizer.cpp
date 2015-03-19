/*
 * Visualizer.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: Tomasz Posłuszny
 *
 *      Copyright. All rights reserved.
 *
 */

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
