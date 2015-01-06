/*
 * MovingObjectExtracting.hpp
 *
 *  Created on: Jan 6, 2015
 *      Author: alucarded
 */

#ifndef THE_EXTRACTOR
#define THE_EXTRACTOR

#include "ImageProcessor.hpp"

class MovingObjectExtracting {
public:

	MovingObjectExtracting(const std::string& win_name);
	virtual ~MovingObjectExtracting();

	void extract(Mat &a, void* data);
protected:
	virtual void init() = 0;
	std::string m_win_name;
	std::shared_ptr<ImageProcessor> m_processor;
};

class RegionMovingObjectExtracting : public MovingObjectExtracting {
public:
	RegionMovingObjectExtracting(const std::string& win_name);
	virtual ~RegionMovingObjectExtracting();
private:
	void init();
};

class GrabCutMovingObjectExtracting : public MovingObjectExtracting {
public:
	GrabCutMovingObjectExtracting(const std::string& win_name);
	virtual ~GrabCutMovingObjectExtracting();
private:
	void init();

	static void onTrackbarChange(int value, void* data);

	int m_kernel_size;
};

#endif /* THE_EXTRACTOR */
