#ifndef MEANSHIFTTRACKER_H
#define MEANSHIFTTRACKER_H

// Standard header files
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// User Defined Header fles
#include"elps.h"
#include"weightedhistogram.h"


using namespace std;


class MeanShiftTracker
{

public:

    // Mask which holds the weight corresponding to pixels within the ellipse.(the ellipse is drawn inside th object bounding box)
    EllipseMask m_EllipseMAsk;

    // No of bins considered for each channel of the image for computing the histogram
    int m_NoOfBinsPerChannel;

    // Size of the histogram
    int m_MSTHistogramSize ;

    // Target Histogram
    WeightedHistogram m_TargetHistogram;

    // Histogram calculated in the current frame with the center co-ordinates from the previous frame
    WeightedHistogram m_PrevPosHistogram;

    // Histogram computed with respect to the estimated new position
    WeightedHistogram m_CurrPosHistogram;

    // Center Co-ordinates of the object bounding box in hte previous frame
    int m_PrevCenterY;
    int m_PrevCenterX;

    // Center Co-ordinates of the  object bounding box in the current frame
    int m_CurrCenterY;
    int m_CurrCenterX;

    // Current Position and previous position Bahttacharysa Coefficients
    float m_PrevPosBhattacharyaCoeff;
    float m_CurrPosBhattacharyaCoeff;

    // Default Constructor
    MeanShiftTracker();


public:

    // Constructor with parameters
    MeanShiftTracker( int verticalAxis , int horizontalAxis, int noOfBinsPerChannel, int initCenterY, int initCenterX , int noOfChannels);


    // Destructor
//    ~MeanShiftTracker() //No element is dynamically allocated in the mst so no need of destructor
//    {
//        std::cout << "DESTRUCTOR FOR MeanShiftTracker" << std::endl;

//        m_NoOfBinsPerChannel   = -1;
//        m_MSTHistogramSize        = -1;

////        m_EllipseMAsk.~EllipseMask();

////        m_TargetHistogram.~WeightedHistogram();
////        m_PrevPosHistogram.~WeightedHistogram();
////        m_CurrPosHistogram.~WeightedHistogram();


//        m_PrevCenterY =-1;
//        m_PrevCenterX =-1;

//        m_CurrCenterY = -1;
//        m_CurrCenterX = -1;

//        m_PrevPosBhattacharyaCoeff = -1;
//        m_CurrPosBhattacharyaCoeff = -1;
//    }

    // Getters

    int getNoOfBinsPerChannel( ){return m_NoOfBinsPerChannel; }
    int getHistogramSize() { return m_MSTHistogramSize   ; }

    EllipseMask getEllipseMask(){ return m_EllipseMAsk; }
    WeightedHistogram getTargetHistogram() { return m_TargetHistogram; }
    WeightedHistogram getPrevPostHistogram() { return m_PrevPosHistogram; }
    WeightedHistogram getCurrPosHistogram() { return m_CurrPosHistogram; }

    int getPrevCenterY() { return m_PrevCenterY; }
    int getPrevCenterX() { return m_PrevCenterX; }

    int getCurrCenterY(){ return m_CurrCenterY; }
    int getCurrCenterX(){ return m_CurrCenterX; }

    float getPrevPosBhattacharyaCoeff(){ return m_PrevPosBhattacharyaCoeff; }
    float getCurrPosBhattacharyaCoeff(){ return m_CurrPosBhattacharyaCoeff; }


    // SETTERS

    int setEllipseMaskWeight();
    int setTargetHistogram(Mat& img);
    int setPrevPostHistogram(Mat &img );
    int setCurrPosHistogram( Mat& img);
    int setPrevCenterY(  ) ;
    int setPrevCenterX();
//    int setCurrCenterY( int currY );
//    int setCurrCenterX( int currX );
    int setCurrentPosition(Mat &img);
    int setPrevPosBhattacharyaCoeff( );
    int setCurrPosBhattacharyaCoeff( );

    // Display the contents of the tracker
    int displayTrackerInfo();

};




#endif // MEANSHIFTTRACKER_H

