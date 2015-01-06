/*
 * MovingObjectExtracting.cpp
 *
 *  initd on: Jan 6, 2015
 *      Author: alucarded
 */

#include "FastScanning.hpp"
#include "GaussianMixture.hpp"
#include "GrabCutExtracting.hpp"
#include "MorphologicalProcessing.hpp"
#include "MovingObjectExtracting.hpp"
#include "ObjectsDesignating.hpp"
#include "RegionSizeFiltering.hpp"
#include "SigmaDeltaFiltering.hpp"

#include "MovingObjectExtracting.hpp"

// class MovingObjectExtracting

MovingObjectExtracting::MovingObjectExtracting(const std::string& win_name)
	: m_win_name(win_name)
{

}

MovingObjectExtracting::~MovingObjectExtracting()
{

}

void MovingObjectExtracting::extract(Mat &a, void* data)
{
	m_processor->process(a, data);
}

// class RegionMovingObjectExtracting

RegionMovingObjectExtracting::RegionMovingObjectExtracting(const std::string& win_name)
	: MovingObjectExtracting(win_name)
{
	init();
}

RegionMovingObjectExtracting::~RegionMovingObjectExtracting()
{

}

void RegionMovingObjectExtracting::init()
{
	m_processor = std::make_shared<GaussianMixture>();
	std::shared_ptr<ImageProcessor> mp = std::make_shared<MorphologicalProcessing>();
	std::shared_ptr<ImageProcessor> rsf = std::make_shared<RegionSizeFiltering>(1000, 3000);
	std::shared_ptr<ImageProcessor> od = std::make_shared<ObjectsDesignating>();

	m_processor->set_next_processor(mp);
	mp->set_next_processor(rsf);
	rsf->set_next_processor(od);
}

// class GrabCutMovingObjectExtracting

GrabCutMovingObjectExtracting::GrabCutMovingObjectExtracting(const std::string& win_name)
	: MovingObjectExtracting(win_name), m_kernel_size(15)
{
	init();
}

GrabCutMovingObjectExtracting::~GrabCutMovingObjectExtracting()
{

}

void GrabCutMovingObjectExtracting::init()
{
	m_processor = std::make_shared<GaussianMixture>();
	ImageProcessor *gce = new GrabCutExtracting();

	m_processor->set_next_processor(gce);

	createTrackbar( "Kernel size", m_win_name, &m_kernel_size, 30, onTrackbarChange, gce);
}

void GrabCutMovingObjectExtracting::onTrackbarChange(int value, void* data)
{
	GrabCutExtracting *gce = reinterpret_cast<GrabCutExtracting*>(data);
	gce->setKernelSize(value);
}
