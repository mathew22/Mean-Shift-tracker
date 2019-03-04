#ifndef WEIGHTEDHISTOGRAM_H
#define WEIGHTEDHISTOGRAM_H

#include<iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



#include"elps.h"
using namespace std;
using namespace cv;

class WeightedHistogram
{
    public:
        //no of bins in a single channel
        int m_NoOfBinsPerChannel;

        //inverse of  bin size
        float m_InvBinSize;

        // No of channel in the image
        int m_NoOfChannels;

        // dimension of the histogram
        int m_HistogramSize;

        // Histogram values
        float* m_HistogramData;

    public:
        WeightedHistogram();

        // DESTRUCTOR
        ~WeightedHistogram()
        {
            cout<<"DESTRUCTOR FOR HISTOGRAM\n";
            m_NoOfBinsPerChannel = -1;
            m_InvBinSize         = -1.0;
            m_NoOfChannels       = -1;
            m_HistogramSize      = -1;
           delete[] m_HistogramData;
        }


        WeightedHistogram( int noOfBinsPerChannel, int noOfChannels);
        int displayHistogramContents();
        int computeWeightedHistogram ( Mat& img, EllipseMask& elpsMask, int centerY, int centerX );
        //int computeBhattacharyaCoeff( WeightedHistogram histogram1, WeightedHistogram histogram2 , float *BhattacharyaCoeff );

};


#endif // WEIGHTEDHISTOGRAM_H
