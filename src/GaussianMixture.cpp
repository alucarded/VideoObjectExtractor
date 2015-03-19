/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

// taken from OpenCV library and modified by Tomasz Posłuszny

#include "FrameGrabber.hpp"
#include "GaussianMixture.hpp"

/*
void parallel_for_(const cv::Range& range, const cv::ParallelLoopBody& body, double nstripes)
{
#ifdef CV_PARALLEL_FRAMEWORK

    if(numThreads != 0)
    {
        ProxyLoopBody pbody(body, range, nstripes);
        cv::Range stripeRange = pbody.stripeRange();
        if( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            return;
        }

#if defined HAVE_TBB

        tbb::parallel_for(tbb::blocked_range<int>(stripeRange.start, stripeRange.end), pbody);

#elif defined HAVE_CSTRIPES

        parallel(MAX(0, numThreads))
        {
            int offset = stripeRange.start;
            int len = stripeRange.end - offset;
            Range r(offset + CPX_RANGE_START(len), offset + CPX_RANGE_END(len));
            pbody(r);
            barrier();
        }

#elif defined HAVE_OPENMP

        #pragma omp parallel for schedule(dynamic)
        for (int i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#elif defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#elif defined HAVE_CONCURRENCY

        if(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            Concurrency::parallel_for(stripeRange.start, stripeRange.end, pbody);
        }
        else
        {
            pplScheduler->Attach();
            Concurrency::parallel_for(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#else

#error You have hacked and compiling with unsupported parallel framework

#endif

    }
    else

#endif // CV_PARALLEL_FRAMEWORK
    {
        (void)nstripes;
        body(range);
    }
}
*/
struct GMM
{
    float weight;
    float variance;
};

// taken from opencv/modules/video/src/bgfg_gaussmix2.cpp
// shadow detection performed per pixel
// should work for rgb data, could be usefull for gray scale and depth data as well
// See: Prati,Mikic,Trivedi,Cucchiarra,"Detecting Moving Shadows...",IEEE PAMI,2003.
CV_INLINE bool
detectShadowGMM(const float* data, int nchannels, int nmodes,
                const GMM* gmm, const float* mean,
                float Tb, float TB, float tau)
{
    float tWeight = 0;

    // check all the components  marked as background:
    for( int mode = 0; mode < nmodes; mode++, mean += nchannels )
    {
        GMM g = gmm[mode];

        float numerator = 0.0f;
        float denominator = 0.0f;
        for( int c = 0; c < nchannels; c++ )
        {
            numerator   += data[c] * mean[c];
            denominator += mean[c] * mean[c];
        }

        // no division by zero allowed
        if( denominator == 0 )
            return false;

        // if tau < a < 1 then also check the color distortion
        if( numerator <= denominator && numerator >= tau*denominator )
        {
            float a = numerator / denominator;
            float dist2a = 0.0f;

            for( int c = 0; c < nchannels; c++ )
            {
                float dD= a*mean[c] - data[c];
                dist2a += dD*dD;
            }

            if (dist2a < Tb*g.variance*a*a)
                return true;
        };

        tWeight += g.weight;
        if( tWeight > TB )
            return false;
    };
    return false;
}

// from opencv/modules/video/src/bgfg_gaussmix2.cpp
class MOG2Invoker : public ParallelLoopBody
{
public:
    MOG2Invoker(const Mat& _src, Mat& _dst,
                GMM* _gmm, float* _mean,
                uchar* _modesUsed,
                int _nmixtures, float _alphaT,
                float _Tb, float _TB, float _Tg,
                float _varInit, float _varMin, float _varMax,
                float _prune, float _tau, bool _detectShadows,
                uchar _shadowVal)
    {
        src = &_src;
        dst = &_dst;
        gmm0 = _gmm;
        mean0 = _mean;
        modesUsed0 = _modesUsed;
        nmixtures = _nmixtures;
        alphaT = _alphaT;
        Tb = _Tb;
        TB = _TB;
        Tg = _Tg;
        varInit = _varInit;
        varMin = MIN(_varMin, _varMax);
        varMax = MAX(_varMin, _varMax);
        prune = _prune;
        tau = _tau;
        detectShadows = _detectShadows;
        shadowVal = _shadowVal;
    }

    void operator()(const Range& range) const
    {
        int y0 = range.start, y1 = range.end;
        int ncols = src->cols, nchannels = src->channels();
        AutoBuffer<float> buf(src->cols*nchannels);
        float alpha1 = 1.f - alphaT;
        float dData[CV_CN_MAX];

        for( int y = y0; y < y1; y++ )
        {
            const float* data = buf;
            if( src->depth() != CV_32F )
                src->row(y).convertTo(Mat(1, ncols, CV_32FC(nchannels), (void*)data), CV_32F);
            else
                data = src->ptr<float>(y);

            float* mean = mean0 + ncols*nmixtures*nchannels*y;
            GMM* gmm = gmm0 + ncols*nmixtures*y;
            uchar* modesUsed = modesUsed0 + ncols*y;
            uchar* mask = dst->ptr(y);

            for( int x = 0; x < ncols; x++, data += nchannels, gmm += nmixtures, mean += nmixtures*nchannels )
            {
                //calculate distances to the modes (+ sort)
                //here we need to go in descending order!!!
                bool background = false;//return value -> true - the pixel classified as background

                //internal:
                bool fitsPDF = false;//if it remains zero a new GMM mode will be added
                int nmodes = modesUsed[x], nNewModes = nmodes;//current number of modes in GMM
                float totalWeight = 0.f;

                float* mean_m = mean;

                //////
                //go through all modes
                for( int mode = 0; mode < nmodes; mode++, mean_m += nchannels )
                {
                    float weight = alpha1*gmm[mode].weight + prune;//need only weight if fit is found
                    int swap_count = 0;
                    ////
                    //fit not found yet
                    if( !fitsPDF )
                    {
                        //check if it belongs to some of the remaining modes
                        float var = gmm[mode].variance;

                        //calculate difference and distance
                        float dist2;

                        if( nchannels == 3 )
                        {
                            dData[0] = mean_m[0] - data[0];
                            dData[1] = mean_m[1] - data[1];
                            dData[2] = mean_m[2] - data[2];
                            dist2 = dData[0]*dData[0] + dData[1]*dData[1] + dData[2]*dData[2];
                        }
                        else
                        {
                            dist2 = 0.f;
                            for( int c = 0; c < nchannels; c++ )
                            {
                                dData[c] = mean_m[c] - data[c];
                                dist2 += dData[c]*dData[c];
                            }
                        }

                        //background? - Tb - usually larger than Tg
                        if( totalWeight < TB && dist2 < Tb*var )
                            background = true;

                        //check fit
                        if( dist2 < Tg*var )
                        {
                            /////
                            //belongs to the mode
                            fitsPDF = true;

                            //update distribution

                            //update weight
                            weight += alphaT;
                            float k = alphaT/weight;

                            //update mean
                            for( int c = 0; c < nchannels; c++ )
                                mean_m[c] -= k*dData[c];

                            //update variance
                            float varnew = var + k*(dist2-var);
                            //limit the variance
                            varnew = MAX(varnew, varMin);
                            varnew = MIN(varnew, varMax);
                            gmm[mode].variance = varnew;

                            //sort
                            //all other weights are at the same place and
                            //only the matched (iModes) is higher -> just find the new place for it
                            for( int i = mode; i > 0; i-- )
                            {
                                //check one up
                                if( weight < gmm[i-1].weight )
                                    break;

                                swap_count++;
                                //swap one up
                                std::swap(gmm[i], gmm[i-1]);
                                for( int c = 0; c < nchannels; c++ )
                                    std::swap(mean[i*nchannels + c], mean[(i-1)*nchannels + c]);
                            }
                            //belongs to the mode - bFitsPDF becomes 1
                            /////
                        }
                    }//!bFitsPDF)

                    //check prune
                    if( weight < -prune )
                    {
                        weight = 0.0;
                        nmodes--;
                    }

                    gmm[mode-swap_count].weight = weight;//update weight by the calculated value
                    totalWeight += weight;
                }
                //go through all modes
                //////

                //renormalize weights
                totalWeight = 1.f/totalWeight;
                for( int mode = 0; mode < nmodes; mode++ )
                {
                    gmm[mode].weight *= totalWeight;
                }

                nmodes = nNewModes;

                //make new mode if needed and exit
                if( !fitsPDF && alphaT > 0.f)
                {
                	// TODO: check how behaves double, is for example 0.00000001 == true ?
                    // replace the weakest or add a new one
                    int mode = nmodes == nmixtures ? nmixtures-1 : nmodes++;

                    if (nmodes==1)
                        gmm[mode].weight = 1.f;
                    else
                    {
                        gmm[mode].weight = alphaT;

                        // renormalize all other weights
                        for( int i = 0; i < nmodes-1; i++ )
                            gmm[i].weight *= alpha1;
                    }

                    // init
                    for( int c = 0; c < nchannels; c++ )
                        mean[mode*nchannels + c] = data[c];

                    gmm[mode].variance = varInit;

                    //sort
                    //find the new place for it
                    for( int i = nmodes - 1; i > 0; i-- )
                    {
                        // check one up
                        if( alphaT < gmm[i-1].weight )
                            break;

                        // swap one up
                        std::swap(gmm[i], gmm[i-1]);
                        for( int c = 0; c < nchannels; c++ )
                            std::swap(mean[i*nchannels + c], mean[(i-1)*nchannels + c]);
                    }
                }

                //set the number of modes
                modesUsed[x] = uchar(nmodes);
                mask[x] = background ? 0 :
                    detectShadows && detectShadowGMM(data, nchannels, nmodes, gmm, mean, Tb, TB, tau) ?
                    shadowVal : 255;
            }
        }
    }

    const Mat* src;
    Mat* dst;
    GMM* gmm0;
    float* mean0;
    uchar* modesUsed0;

    int nmixtures;
    float alphaT, Tb, TB, Tg;
    float varInit, varMin, varMax, prune, tau;

    bool detectShadows;
    uchar shadowVal;
};

void GaussianMixture::process_implementation(Mat &a, void* data)
{
	Mat fg;
	// Gaussian blur enlarges the region, careful
	// GaussianBlur( a, a, Size( 5, 5 ), 0, 0 );
	if (FrameGrabber::instance()->getFrameNum() < history)
		apply(a, fg);
	else
		apply(a, fg, m_final_lr);
	fg.copyTo(a);

	imshow("mog2", a);
}

void GaussianMixture::apply(InputArray _image, OutputArray _fgmask, double learningRate)
{
    bool needToInitialize = nframes == 0 || learningRate >= 1 || _image.size() != frameSize || _image.type() != frameType;

    if( needToInitialize )
        initialize(_image.size(), _image.type());

    /*
    if (opencl_ON)
    {
        CV_OCL_RUN(opencl_ON, ocl_apply(_image, _fgmask, learningRate))

        opencl_ON = false;
        initialize(_image.size(), _image.type());
    }
*/
    Mat image = _image.getMat();
    _fgmask.create( image.size(), CV_8U );
    Mat fgmask = _fgmask.getMat();

    ++nframes;
    learningRate = learningRate >= 0 && nframes > 1 ? learningRate : 1./std::min( 2*nframes, history );
    //learningRate = nframes < 100 ? 1./std::min( 2*nframes, history ) : 0;
    CV_Assert(learningRate >= 0);

    parallel_for_(Range(0, image.rows),
                  MOG2Invoker(image, fgmask,
                              bgmodel.ptr<GMM>(),
                              (float*)(bgmodel.ptr() + sizeof(GMM)*nmixtures*image.rows*image.cols),
                              bgmodelUsedModes.ptr(), nmixtures, (float)learningRate,
                              (float)varThreshold,
                              backgroundRatio, varThresholdGen,
                              fVarInit, fVarMin, fVarMax, float(-learningRate*fCT), fTau,
                              bShadowDetection, nShadowDetection),
                              image.total()/(double)(1 << 16));
/*    MOG2Invoker(image, fgmask,
                bgmodel.ptr<GMM>(),
                (float*)(bgmodel.ptr() + sizeof(GMM)*nmixtures*image.rows*image.cols),
                bgmodelUsedModes.ptr(), nmixtures, (float)learningRate,
                (float)varThreshold,
                backgroundRatio, varThresholdGen,
                fVarInit, fVarMin, fVarMax, float(-learningRate*fCT), fTau,
                bShadowDetection, nShadowDetection)(Range(0, image.rows));*/
}
