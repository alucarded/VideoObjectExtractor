/*
 * GaussianMixture.hpp
 *
 *  Created on: Sep 24, 2014
 *      Author: alucard
 */

#ifndef GAUSSIANMIXTURE_HPP_
#define GAUSSIANMIXTURE_HPP_

#include "Common.hpp"
#include "ImageProcessor.hpp"

// default parameters of gaussian background detection algorithm
static const int defaultHistory2 = 50; // Learning rate; alpha = 1/defaultHistory2
static const float defaultVarThreshold2 = 4.0f*4.0f;
static const int defaultNMixtures2 = 5; // maximal number of Gaussians in mixture
static const float defaultBackgroundRatio2 = 0.9f; // threshold sum of weights for background test
static const float defaultVarThresholdGen2 = 3.0f*3.0f;
static const float defaultVarInit2 = 15.0f; // initial variance for new components
static const float defaultVarMax2 = 5*defaultVarInit2;
static const float defaultVarMin2 = 4.0f;

// additional parameters
static const float defaultfCT2 = 0.05f; // complexity reduction prior constant 0 - no reduction of number of components
static const unsigned char defaultnShadowDetection2 = (unsigned char)0; // value to use in the segmentation mask for shadows, set 0 not to do shadow detection
static const float defaultfTau = 0.1f; // Tau - shadow threshold, see the paper for explanation

class GaussianMixture : public ImageProcessor{
public:
	virtual ~GaussianMixture() { };

    //! the default constructor
	GaussianMixture()
    {
        frameSize = Size(0,0);
        frameType = 0;

        nframes = 0;
        history = defaultHistory2;
        varThreshold = defaultVarThreshold2;
        bShadowDetection = 1; // default no shadow detection

        nmixtures = defaultNMixtures2;
        backgroundRatio = defaultBackgroundRatio2;
        fVarInit = defaultVarInit2;
        fVarMax  = defaultVarMax2;
        fVarMin = defaultVarMin2;

        varThresholdGen = defaultVarThresholdGen2;
        fCT = defaultfCT2;
        nShadowDetection = defaultnShadowDetection2;
        fTau = defaultfTau;

        m_final_lr = 0.0f;

        //opencl_ON = true;
    }
    //! the full constructor that takes the length of the history,
    // the number of gaussian mixtures, the background ratio parameter and the noise strength
	GaussianMixture(int _history,  float _varThreshold, bool _bShadowDetection=true)
    {
        frameSize = Size(0,0);
        frameType = 0;

        nframes = 0;
        history = _history > 0 ? _history : defaultHistory2;
        varThreshold = (_varThreshold>0)? _varThreshold : defaultVarThreshold2;
        bShadowDetection = _bShadowDetection;

        nmixtures = defaultNMixtures2;
        backgroundRatio = defaultBackgroundRatio2;
        fVarInit = defaultVarInit2;
        fVarMax  = defaultVarMax2;
        fVarMin = defaultVarMin2;

        varThresholdGen = defaultVarThresholdGen2;
        fCT = defaultfCT2;
        nShadowDetection =  defaultnShadowDetection2;
        fTau = defaultfTau;
        name_ = "BackgroundSubtractor.MOG2";

        m_final_lr = 0.0f;

        //opencl_ON = true;
    }

    //! computes a background image which are the mean of all background gaussians
    virtual void getBackgroundImage(OutputArray backgroundImage) const { }

    //! re-initiaization method
    void initialize(Size _frameSize, int _frameType)
    {


        frameSize = _frameSize;
        frameType = _frameType;
        nframes = 0;

        int nchannels = CV_MAT_CN(frameType);
        CV_Assert( nchannels <= CV_CN_MAX );
/*
        if (ocl::useOpenCL() && opencl_ON)
        {
            kernel_apply.create("mog2_kernel", ocl::video::bgfg_mog2_oclsrc, format("-D CN=%d -D NMIXTURES=%d", nchannels, nmixtures));
            kernel_getBg.create("getBackgroundImage2_kernel", ocl::video::bgfg_mog2_oclsrc, format( "-D CN=%d -D NMIXTURES=%d", nchannels, nmixtures));

            if (kernel_apply.empty() || kernel_getBg.empty())
                opencl_ON = false;
        }
        else opencl_ON = false;

        if (opencl_ON)
        {
            u_weight.create(frameSize.height * nmixtures, frameSize.width, CV_32FC1);
            u_weight.setTo(Scalar::all(0));

            u_variance.create(frameSize.height * nmixtures, frameSize.width, CV_32FC1);
            u_variance.setTo(Scalar::all(0));

            if (nchannels==3)
                nchannels=4;
            u_mean.create(frameSize.height * nmixtures, frameSize.width, CV_32FC(nchannels)); //4 channels
            u_mean.setTo(Scalar::all(0));

            //make the array for keeping track of the used modes per pixel - all zeros at start
            u_bgmodelUsedModes.create(frameSize, CV_32FC1);
            u_bgmodelUsedModes.setTo(cv::Scalar::all(0));
        }
        else*/
        {
            // for each gaussian mixture of each pixel bg model we store ...
            // the mixture weight (w),
            // the mean (nchannels values) and
            // the covariance
            bgmodel.create( 1, frameSize.height*frameSize.width*nmixtures*(2 + nchannels), CV_32F );
            //make the array for keeping track of the used modes per pixel - all zeros at start
            bgmodelUsedModes.create(frameSize,CV_8U);
            bgmodelUsedModes = Scalar::all(0);
        }
    }

    virtual AlgorithmInfo* info() const { return 0; }

    virtual int getHistory() const { return history; }
    virtual void setHistory(int _nframes) { history = _nframes; }

    virtual int getNMixtures() const { return nmixtures; }
    virtual void setNMixtures(int nmix) { nmixtures = nmix; }

    virtual double getBackgroundRatio() const { return backgroundRatio; }
    virtual void setBackgroundRatio(double _backgroundRatio) { backgroundRatio = (float)_backgroundRatio; }

    virtual double getVarThreshold() const { return varThreshold; }
    virtual void setVarThreshold(double _varThreshold) { varThreshold = _varThreshold; }

    virtual double getVarThresholdGen() const { return varThresholdGen; }
    virtual void setVarThresholdGen(double _varThresholdGen) { varThresholdGen = (float)_varThresholdGen; }

    virtual double getVarInit() const { return fVarInit; }
    virtual void setVarInit(double varInit) { fVarInit = (float)varInit; }

    virtual double getVarMin() const { return fVarMin; }
    virtual void setVarMin(double varMin) { fVarMin = (float)varMin; }

    virtual double getVarMax() const { return fVarMax; }
    virtual void setVarMax(double varMax) { fVarMax = (float)varMax; }

    virtual double getComplexityReductionThreshold() const { return fCT; }
    virtual void setComplexityReductionThreshold(double ct) { fCT = (float)ct; }

    virtual bool getDetectShadows() const { return bShadowDetection; }
    virtual void setDetectShadows(bool detectshadows) { bShadowDetection = detectshadows; }

    virtual int getShadowValue() const { return nShadowDetection; }
    virtual void setShadowValue(int value) { nShadowDetection = (uchar)value; }

    virtual double getShadowThreshold() const { return fTau; }
    virtual void setShadowThreshold(double value) { fTau = (float)value; }

    virtual void write(FileStorage& fs) const
    {
        fs << "name" << name_
        << "history" << history
        << "nmixtures" << nmixtures
        << "backgroundRatio" << backgroundRatio
        << "varThreshold" << varThreshold
        << "varThresholdGen" << varThresholdGen
        << "varInit" << fVarInit
        << "varMin" << fVarMin
        << "varMax" << fVarMax
        << "complexityReductionThreshold" << fCT
        << "detectShadows" << (int)bShadowDetection
        << "shadowValue" << (int)nShadowDetection
        << "shadowThreshold" << fTau;
    }

    virtual void read(const FileNode& fn)
    {
        CV_Assert( (String)fn["name"] == name_ );
        history = (int)fn["history"];
        nmixtures = (int)fn["nmixtures"];
        backgroundRatio = (float)fn["backgroundRatio"];
        varThreshold = (double)fn["varThreshold"];
        varThresholdGen = (float)fn["varThresholdGen"];
        fVarInit = (float)fn["varInit"];
        fVarMin = (float)fn["varMin"];
        fVarMax = (float)fn["varMax"];
        fCT = (float)fn["complexityReductionThreshold"];
        bShadowDetection = (int)fn["detectShadows"] != 0;
        nShadowDetection = saturate_cast<uchar>((int)fn["shadowValue"]);
        fTau = (float)fn["shadowThreshold"];
    }

private:
	void process_implementation(Mat &a, void* data);
	void apply(InputArray _image, OutputArray _fgmask, double learningRate = -1.0);

    Size frameSize;
    int frameType;
    Mat bgmodel;
    Mat bgmodelUsedModes;//keep track of number of modes per pixel

    int nframes;
    int history;
    int nmixtures;
    //! here it is the maximum allowed number of mixture components.
    //! Actual number is determined dynamically per pixel
    double varThreshold;
    // threshold on the squared Mahalanobis distance to decide if it is well described
    // by the background model or not. Related to Cthr from the paper.
    // This does not influence the update of the background. A typical value could be 4 sigma
    // and that is varThreshold=4*4=16; Corresponds to Tb in the paper.

    /////////////////////////
    // less important parameters - things you might change but be carefull
    ////////////////////////
    float backgroundRatio;
    // corresponds to fTB=1-cf from the paper
    // TB - threshold when the component becomes significant enough to be included into
    // the background model. It is the TB=1-cf from the paper. So I use cf=0.1 => TB=0.
    // For alpha=0.001 it means that the mode should exist for approximately 105 frames before
    // it is considered foreground
    // float noiseSigma;
    float varThresholdGen;
    //correspondts to Tg - threshold on the squared Mahalan. dist. to decide
    //when a sample is close to the existing components. If it is not close
    //to any a new component will be generated. I use 3 sigma => Tg=3*3=9.
    //Smaller Tg leads to more generated components and higher Tg might make
    //lead to small number of components but they can grow too large
    float fVarInit;
    float fVarMin;
    float fVarMax;
    //initial variance  for the newly generated components.
    //It will will influence the speed of adaptation. A good guess should be made.
    //A simple way is to estimate the typical standard deviation from the images.
    //I used here 10 as a reasonable value
    // min and max can be used to further control the variance
    float fCT;//CT - complexity reduction prior
    //this is related to the number of samples needed to accept that a component
    //actually exists. We use CT=0.05 of all the samples. By setting CT=0 you get
    //the standard Stauffer&Grimson algorithm (maybe not exact but very similar)

    //shadow detection parameters
    bool bShadowDetection;//default 0 - don't do shadow detection
    unsigned char nShadowDetection;//do shadow detection - insert this value as the detection result - 127 default value
    float fTau;

    double m_final_lr;

    std::string name_;
};

#endif /* GAUSSIANMIXTURE_HPP_ */
