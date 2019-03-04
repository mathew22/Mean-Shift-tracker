#include<iostream>
#include<math.h>

#include "../include/weightedhistogram.h"
using namespace std;

// Default Constructor
WeightedHistogram::WeightedHistogram()
{
    m_NoOfBinsPerChannel = -1;
    m_InvBinSize         = -1.0;
    m_NoOfChannels       = -1;
    m_HistogramSize      = -1;
    m_HistogramData      = NULL;

}

// Constructor with parameter
WeightedHistogram::WeightedHistogram(int noOfBinsPerChannel, int noOfChannels)
{


    m_NoOfBinsPerChannel = noOfBinsPerChannel;
    m_InvBinSize         =  (float)noOfBinsPerChannel/(float)256;
    m_NoOfChannels       = noOfChannels;
    m_HistogramSize      = pow(m_NoOfBinsPerChannel,noOfChannels);
    m_HistogramData      = new float[m_HistogramSize];
}

int WeightedHistogram::displayHistogramContents()
{
    cout<<"*********HISTOGRAm PARAMETERS***********\n";
    cout<<"noOfBinsPerChannel"  << m_NoOfBinsPerChannel<<endl;
    cout<<"InvBinSize = "       <<m_InvBinSize<<endl;
    cout<<"NoOfChannels = "     <<m_NoOfChannels<<endl;
    cout<<"HistogramSize = "       <<m_HistogramSize<<endl;
    cout<<"HistogramData Address ="<<m_HistogramData<<endl;
    return(0);
}

    // Computing the weightedHistogram
    int WeightedHistogram:: computeWeightedHistogram ( Mat& img, EllipseMask& elpsMask, int centerY, int centerX )
    {

        // initialize the contents to zero
        for ( int dim = 0 ; dim < m_HistogramSize ; dim++ )
        {
            m_HistogramData[ dim] = 0;
        }

        // looping through the pixels inside the elliptical region
        for ( int dim = 0 ; dim < elpsMask.m_ArraySize ; dim++ )
            {
                 if(elpsMask.m_WeightArray[ dim ]!=0 )// pixels with non zero weights only belongs to the ellipse
                 {
                    int currentYIndx = elpsMask.m_RelYIndexArray[ dim ] + centerY ;
                    int currentXIndx = elpsMask.m_RelXIndexArray[ dim ] + centerX ;
                    // Accessing the pixel color values
                    int blue = img.at<Vec3b>( currentYIndx , currentXIndx ).val[0];
                    int green = img.at<Vec3b>( currentYIndx , currentXIndx ).val[1];
                    int red = img.at<Vec3b>( currentYIndx , currentXIndx ).val[2];

                    // Histogram bin corresponding to each channel
                    int blueIndx = floor (  blue * m_InvBinSize );
                    int greenIndx = floor (  green * m_InvBinSize );
                    int redIndx = floor (  red * m_InvBinSize);
                   // printf(" dim = %d current index = %d || blueIndx = %d | greenIndx = %d | redindx = %d \n", dim , currentIndx, blueIndx, greenIndx, redIndx );
                    // Find the histogram index corresponding to the color values of the pixel
                    int colorHistogramIndx = redIndx * ( m_NoOfBinsPerChannel  )* ( m_NoOfBinsPerChannel  ) + greenIndx * (m_NoOfBinsPerChannel )+ blueIndx ;

                    // Check for out of bounds value
                    // if ( colorHistogramIndx<0 || colorHistogramIndx >4095)
                    //        cout<<"ERROR in color index value\n"<<endl;


                    // adding weight values to the corresponding histogram
                    m_HistogramData[ colorHistogramIndx ] = m_HistogramData[ colorHistogramIndx ] + elpsMask.m_WeightArray[ dim ];
                   // printf(" histogram index =%d and value =%f \n", colorHistogramIndx , elpsMask->m_WeightArray[ dim ]);
                 }
            }

            // Finding the number of non-zero values in the histogram and the normalizing constant
            float sum =0;
            int count = 0;
            for( int dim = 0 ; dim <  m_HistogramSize ;dim ++)
            {

                if ( m_HistogramData[ dim ] != 0 )
                {
                    count =count + 1;

                    sum = sum  +  m_HistogramData[ dim ];

                }
            }

            // Normalizing the histogram array
            for( int dim = 0 ; dim < m_HistogramSize ;dim ++)
            {
                m_HistogramData[ dim ] = m_HistogramData[ dim ] /sum;
                // Check for invalid values
                if ( m_HistogramData[ dim ] < 0 || m_HistogramData[ dim ] > 1)
                    cout<<"ERROR in HISTOGRAM DATA\n"<<endl;

//                if ( m_HistogramData[ dim ]!=0)
                //cout<<"dim = "<< dim <<" | value = "<<m_HistogramData[ dim ]<<"  ||  ";
            }

            return( 0 );


    }


    // Computing the Bhattacharya Coefficient
//    int  WeightedHistogram::computeBhattacharyaCoeff( WeightedHistogram histogram1, WeightedHistogram histogram2 , float *BhattacharyaCoeff )
//    {
//        if ( histogram1.m_HistogramSize != histogram2.m_HistogramSize )
//            return(1);

//        *BhattacharyaCoeff = 0;
//        for ( int index = 0 ; index< histogram1.m_HistogramSize ; index++)
//        {
//            *BhattacharyaCoeff  = *BhattacharyaCoeff  + sqrt(histogram1.m_HistogramData[ index ] * histogram2.m_HistogramData[ index ]);
//        }
//        return( 0 );
//    }


