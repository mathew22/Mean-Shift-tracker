#include<iostream>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include"../include/MeanShiftTracker.h"



MeanShiftTracker:: MeanShiftTracker()
{

}
// Parameterised Constructor
MeanShiftTracker::MeanShiftTracker( int verticalAxis , int horizontalAxis, int noOfBinsPerChannel, int initCenterY, int initCenterX , int noOfChannels)
    :m_EllipseMAsk( verticalAxis , horizontalAxis),m_NoOfBinsPerChannel(noOfBinsPerChannel), m_MSTHistogramSize(pow( noOfBinsPerChannel,3) ),
      m_TargetHistogram(  noOfBinsPerChannel, noOfChannels),m_PrevPosHistogram(  noOfBinsPerChannel,  noOfChannels ), m_CurrPosHistogram(  noOfBinsPerChannel,  noOfChannels ),
      m_PrevCenterY( initCenterY), m_PrevCenterX( initCenterX), m_CurrCenterY(initCenterY),m_CurrCenterX( initCenterX),
      m_PrevPosBhattacharyaCoeff( -1 ), m_CurrPosBhattacharyaCoeff( -1 )
{
    cout<<"Creating Mean Shift Tracker\n";

}


int MeanShiftTracker::setEllipseMaskWeight()
{
    m_EllipseMAsk.setWeightArrayAndIndexArray();
    return 0;
}

// Compute and set the target histogram in the tracker
int MeanShiftTracker::setTargetHistogram(Mat& img)
{
    //    // return if the argument is NULL
    //    if(targetHistogram.m_HistogramSize != this->m_MSTHistogramSize    )
    //        return( 1 );
    //    m_TargetHistogram.m_HistogramSize       = targetHistogram.m_HistogramSize;
    //    m_TargetHistogram.m_InvBinSize          = targetHistogram.m_InvBinSize;
    //    m_TargetHistogram.m_NoOfBinsPerChannel  = targetHistogram.m_NoOfBinsPerChannel;
    //    m_TargetHistogram.m_NoOfChannels        = targetHistogram.m_NoOfChannels;

    //    for ( int index = 0 ; index < m_MSTHistogramSize    ; index++ )
    //        m_TargetHistogram.m_HistogramData[ index ] = targetHistogram.m_HistogramData[index];
    m_TargetHistogram.computeWeightedHistogram( img , m_EllipseMAsk , m_PrevCenterY, m_PrevCenterX);
    return( 0 );


}
// Compute and set the previous position histogram in the tracker
int MeanShiftTracker::setPrevPostHistogram(Mat& img )
{

    m_PrevPosHistogram.computeWeightedHistogram( img , m_EllipseMAsk , m_PrevCenterY, m_PrevCenterX );
    return( 0 );

}

// Compute and set the current position histogram in the tracker
int  MeanShiftTracker::setCurrPosHistogram(Mat &img )
{
    m_CurrPosHistogram.computeWeightedHistogram( img , m_EllipseMAsk , m_CurrCenterY, m_CurrCenterX );
    return( 0 );


}

int MeanShiftTracker::setPrevCenterY() { m_PrevCenterY = m_CurrCenterY; return(0); }
int MeanShiftTracker::setPrevCenterX() { m_PrevCenterX = m_CurrCenterX; return(0); }

//    int MeanShiftTracker::setCurrCenterY( int currY ){  m_CurrCenterY = currY; return(0); }
//    int MeanShiftTracker::setCurrCenterX( int currX ){  m_CurrCenterX = currX; return( 0);}

int MeanShiftTracker::setCurrentPosition( Mat &img )
{
    //        float tmpHistogram[m_MSTHistogramSize];

    //        for( int index =0 ; index <m_MSTHistogramSize ; index++ )
    //        {
    //            tmpHistogram[ index ] = 0;
    //        }

    float invBinSize = ( (float) m_NoOfBinsPerChannel ) / 256.0;

    float newCenterY = 0.0;
    float newCenterX = 0.0;

    float sumY =0.0;
    float sumX =0.0;

    cout<<"\n NEWPosition computation\n";

    float weightNormalizingConstant = 0 ;
    for ( int dim = 0 ; dim < m_EllipseMAsk.m_ArraySize ; dim++ )
    {
        if(m_EllipseMAsk.m_WeightArray[ dim ]!=0 )
        {
            int currentYIndx = m_EllipseMAsk.m_RelYIndexArray[ dim ] + m_PrevCenterY; ;
            int currentXIndx = m_EllipseMAsk.m_RelXIndexArray[ dim ] +  m_PrevCenterX ;

            int blue = img.at<Vec3b>( currentYIndx , currentXIndx ).val[0];
            int green = img.at<Vec3b>( currentYIndx , currentXIndx ).val[1];
            int red = img.at<Vec3b>( currentYIndx , currentXIndx ).val[2];

            int blueIndx = floor (  blue * invBinSize );
            int greenIndx = floor ( green * invBinSize );
            int redIndx = floor (  red * invBinSize );
            // Computing the histogram index corresponding to the pixel value
            int colorHistogramIndx = redIndx * ( m_NoOfBinsPerChannel  )* ( m_NoOfBinsPerChannel  ) + greenIndx * (m_NoOfBinsPerChannel )+ blueIndx ;
            //            cout<<"color index ="<<colorHistogramIndx<< " | ";
            //            if ( colorHistogramIndx<0 || colorHistogramIndx >4095)
            //                cout<<"ERROR in color index value\n"<<endl;

            if ( m_PrevPosHistogram.m_HistogramData[ colorHistogramIndx ] != 0 )
            {
                float weight = sqrt( m_TargetHistogram.m_HistogramData[ colorHistogramIndx ] / m_PrevPosHistogram.m_HistogramData[ colorHistogramIndx]);
                weightNormalizingConstant = weightNormalizingConstant + weight;
                //                    cout<<"|"<<colorHistogramIndx<<"|"<<"weight ="<< weight<<" | Y Index ="<<m_EllipseMAsk.m_RelYIndexArray[ dim ]<<" | X INdex ="<<m_EllipseMAsk.m_RelXIndexArray[ dim ] <<"||"<<endl;

                sumY = sumY + m_EllipseMAsk.m_RelYIndexArray[ dim ];
                sumX = sumX + m_EllipseMAsk.m_RelXIndexArray[ dim ];

                newCenterY = newCenterY + weight * currentYIndx;
                newCenterX = newCenterX + weight * currentXIndx;

            }
        }
    }
    //cout<<"\n total weight ="<<weightNormalizingConstant<<endl;

    m_CurrCenterY = (int)round( newCenterY / weightNormalizingConstant ) ;
    m_CurrCenterX = (int)round( newCenterX / weightNormalizingConstant ) ;

    cout<<"\n INside function:: newCenterY ="<<newCenterY/ weightNormalizingConstant <<" | newCenterX = "<<newCenterX/ weightNormalizingConstant <<" || ";
    cout<<"\n sum Y= "<< sumY<<" sumX = "<< sumX<<endl;
    return(0);
}


// Compute and set the previous position Bhattacharya Coefficient
int MeanShiftTracker::setPrevPosBhattacharyaCoeff(  )
{
    m_PrevPosBhattacharyaCoeff = 0;

    for( int index = 0; index < m_MSTHistogramSize; index++ )
    {
        m_PrevPosBhattacharyaCoeff = m_PrevPosBhattacharyaCoeff + sqrt( m_PrevPosHistogram.m_HistogramData[ index ] * m_TargetHistogram.m_HistogramData[ index ]);
    }
    return(0);
}

// Compute and set the current position Bhattacharya Coefficient
int MeanShiftTracker::setCurrPosBhattacharyaCoeff( )
{
    m_CurrPosBhattacharyaCoeff = 0;

    for( int index = 0; index < m_MSTHistogramSize; index++ )
    {
        m_CurrPosBhattacharyaCoeff = m_CurrPosBhattacharyaCoeff + sqrt( m_CurrPosHistogram.m_HistogramData[ index ] * m_TargetHistogram.m_HistogramData[ index ] );
    }
    return(0);
}


// Displays the tracker contents
int MeanShiftTracker::displayTrackerInfo()
{
    cout<<"*************TRACKER PARAMETERS**************"<<endl;
    m_EllipseMAsk.displayEllipseContents();
    cout<<"Target Histogram:\n";
    m_TargetHistogram.displayHistogramContents();
    cout<<"Previous Position Histogram:\n";
    m_PrevPosHistogram.displayHistogramContents();
    cout<<"Current Position Histogram:\n";
    m_CurrPosHistogram.displayHistogramContents();

    cout<<"HistogramSIze = "<<m_MSTHistogramSize<<"\n";
    cout<<"PrevCenter y = "<< m_PrevCenterY<<"| PrevCenter x = "<<m_PrevCenterX<<"\n";
    cout<<"CurrCenter y = "<< m_CurrCenterY<<"| CurrCenter x = "<<m_CurrCenterX<<endl;
    cout<<"Prev BC = "<<m_PrevPosBhattacharyaCoeff<<" | Curr BC = "<<m_CurrPosBhattacharyaCoeff<<"\n";
    //cout<<"Target Histogram Address = "<<m_PrevPosHistogram<<" | Prev Pos Histogram Address = "<<m_CurrPosHistogram<<" | Curr Pos Histogram Address ="<<m_CurrPosHistogram<<"\n";
    return( 0 );

}


