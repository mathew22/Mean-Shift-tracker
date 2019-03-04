
// include header files
#include "ellipseMask.h"

#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

	struct CHistogram
	{
		//no of bins in a single channel
		int m_NoOfBinsPerChannel;
		
		//inverse of  bin size
		float m_InvBinSize;
		
		// No of channel in the image 
		int m_NoOfChannels;
		
		// dimension of the histogram
		int m_HistogramDimension;
		
		// Histogram values
		float* m_HistogramDataArray;
	};

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for Initializing a CHistogram 
	// Inputs :-
	//			(CHistogram*) hist: Pointer to the CHistogram structure
	// Outputs :-
	//			None
	// Invoked As : initialize_CVDRIKCHistogram(&hist );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void initialize_CVDRIKCHistogram( CHistogram* hist );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for allocating memory a CHistogram 
	// Inputs :-
	//			(CHistogram*) elpsMask: Pointer to the CHistogram structure
	//			( int ) binsPerChannel : bins per channel for computing the histogram 
	//			( int ) noOfChannels : no of channels in the image 
	// Outputs :-
	//			( unsigned char ) errCode: error code value
	// Invoked As : allocate_CVDRIKCHistogram( &hist , binsPerChannel ,  noOfChannels );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned char allocate_CVDRIKCHistogram( CHistogram* hist  , int binsPerChannel, int noOfChannels);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for deallocating memory of a CHistogram 
	// Inputs :-
	//			(CHistogram*) elpsMask: Pointer to the CHistogram structure
	// Outputs :-
	//			( unsigned char ) errCode: error code value
	// Invoked As : deallocate_CVDRIKCHistogram( &hist );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned char deallocate_CVDRIKCHistogram( CHistogram* hist );

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for copying one histogram to another
	// Inputs :-
	//			(CHistogram*) sourceHistogram: Pointer to the CHistogram structcure source histogram
	//			(CHistogram*) destHistogram: Pointer to the CHistogram structcure destination histogram
	// Outputs :-
	//			( unsigned char ) errCode: error code value
	// Invoked As : copy_CVDRIKCHistogram( &sourceHistogram, &destHistogram );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned char copy_CVDRIKCHistogram( CHistogram* sourceHistogram ,  CHistogram* destHistogram );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function  computing the color histogram from the intensity values
	// Inputs :-
	//			(CHistogram*) hist: Pointer to the CHistogram structcure 
	//			(IplImage*) rgbImg: Input image whose intensity values are considered for computing histogram
	//			(CELpsMask2D) elpsMask : Weight value corresponding to each pixel location
	//			( int ) centerY : vertical coordinate of the centre of the ellipsoidal redgion under consideration
	//			( int ) centerX : horizontal coordinate of the centre of the ellipsoidal redgion under consideration
	// Outputs :-
	//			( unsigned char ) errCode: error code value
	// Invoked As : computeWeightedRGBHistogram_CVDRIKCHistogram( &hist , rgbImg , elpsMask , centerY , centerX );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned char computeWeightedRGBHistogram_CVDRIKCHistogram( CHistogram* hist, IplImage* rgbImg , CELpsMask2D* elpsMask, int centerY, int centerX );

/**********************************
 * 
 * 
 * 				CVDRIKHISTOGRAM
 * *******************************/
	struct CVDRIKHistogram
	{	
		// size of the histogram
		int m_HistogramSize;
		
		// histogram Data
		float* m_HistogramData;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for initializing CVDRIKHistogram structure
	// Inputs :-
	//			(CVDRIKHistogram*) vHist: Pointer to the CVDRIKHistogram structure
	// Outputs :-
	//			( unsigned char ) errCode: 0 if no error
	// Invoked As : initialize_CVDRIKHistogram( &cvHist );
	void initialize_CVDRIKHistogram( CVDRIKHistogram * cvHist );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for allocating CVDRIKHistogram structure
	// Inputs :-
	//			( CVDRIKHistogram* ) cvHist: Pointer to the CVDRIKHistogram structure
	//			( int ) histogramSize : length of the histogram array
	// Outputs :-
	//			( unsigned char ) errCode: 0 if no error
	// Invoked As : allocate_CVDRIKHistogram( &cvHist , histogramSize  );
	unsigned char allocate_CVDRIKHistogram( CVDRIKHistogram* cvHist  , int histogramSize );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for dealocating memory for CVDRIKHistogram structure
	// Inputs :-
	//			( CVDRIKHistogram* ) cvHist: Pointer to the CVDRIKHistogram structure
	// Outputs :-
	//			( unsigned char ) errCode: 0 if no error
	// Invoked As : deallocate_CVDRIKHistogram( &cvHist );
	/////////////////////////////////////////////////////////////////////////////////////////
	unsigned char deallocate_CVDRIKHistogram( CVDRIKHistogram* cvHist );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for copying CVDRIKHistogram structure from one to other
	// Inputs :-
	//			( CVDRIKHistogram* ) cvHistogramSource: Pointer to the CVDRIKHistogram structure source histogram
	//			( CVDRIKHistogram* ) cvHistogramDest: Pointer to the CVDRIKHistogram structure Destination histogram
	//			( int ) histogramSize : length of the histogram array
	// Outputs :-
	//			( unsigned char ) errCode: 0 if no error
	// Invoked As : copy_CVDRIKHistogram( &cvHistogramSource , &cvHistogramDest  );
	////////////////////////////////////////////////////////////////////////////
	unsigned char copy_CVDRIKHistogram( CVDRIKHistogram* cvHistogramSource ,  CVDRIKHistogram* cvHistogramDest );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for computing the color histogram
	// Inputs :-
	//			( CVDRIKHistogram* ) cvHist: Pointer to the CVDRIKHistogram structure 
	//			( IplImage*) rgbImg : Pointer to the input image
	//			( CELpsMask2D* ) elpsMask : Pointer to the elliptical mask
	//			( int ) centerY : Center Y coordinate of the mask
	//			( int ) centerX : Center X  coordinate of hte mask
	//			( int ) binsPerChannel : Number of bins for computing the color histogram
	// Outputs :-
	//			( unsigned char ) errCode: 0 if no error
	// Invoked As : computeWeightedRGBHistogram_CVDRIKHistogram( &cvHist, &rgbImg , &epsMask , centerY, centerX ,binsPerChannel );
	/////////////////////////////////////////////////////////////////////////////////
	unsigned char computeWeightedRGBHistogram_CVDRIKHistogram( CVDRIKHistogram* cvHist, IplImage* rgbImg , CELpsMask2D* 
	elpsMask, int centerY, int centerX , int binsPerChannel );
	
	
	
/***************************************************
 * 
 * 
 * 				COMPRESSED HISTOGRAM STRUCTURE
 * 
 * 
 * ************************************************/
 
	struct CVDRIKCompressedHistogram
	{
		// No of non zero entries in the histogram
		int m_NoOfNonZeroElements;
		
		// index of the non zero entries
		int* m_NonZeroHistogramValueIndexArray;
		
		// Histogram values
		float* m_NonZeroHistogramValueArray;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for initializing CVDRIKCompressedHistogram structure
	// Inputs :-
	//			( CVDRIKComressedHistogram* ) compressedHist: Pointer to the CVDRIKCompressedHistogram structure
	// Outputs :-
	//			none
	// Invoked As : initialize_CVDRIKCompressedHistogram( &compressedHist );
	void initialize_CVDRIKCompressedHistogram( CVDRIKCompressedHistogram* compressedHist );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for allocating memory for CVDRIKCompressedHistogram structure
	// Inputs :-
	//			( CVDRIKComressedHistogram* ) compressedHist: Pointer to the CVDRIKCompressedHistogram structure
	//			( int ) noOfElements : no of non zero elements( size) of the histogram
	// Outputs :-
	//			(unsigned char ) errCode : 0 if no error
	// Invoked As : allocate_CVDRIKCompressedHistogram( &compressedHist );
	unsigned char allocate_CVDRIKCompressedHistogram( CVDRIKCompressedHistogram* compressedHist , int noOfElements );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for reallocating memory for CVDRIKCompressedHistogram structure
	// Inputs :-
	//			( CVDRIKComressedHistogram* ) compressedHist: Pointer to the CVDRIKCompressedHistogram structure
	//			( int ) noOfElements : no of non zero elements( size) of the histogram
	// Outputs :-
	//			(unsigned char ) errCode : 0 if no error
	// Invoked As : reallocate_CVDRIKCompressedHistogram( &compressedHist );
	unsigned char reallocate_CVDRIKCompressesdHistogram( CVDRIKCompressedHistogram* compressedHist , int noOfElements );
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for deallocating memory for CVDRIKCompressedHistogram structure
	// Inputs :-
	//			( CVDRIKComressedHistogram* ) compressedHist: Pointer to the CVDRIKCompressedHistogram structure
	// Outputs :-
	//			(unsigned char ) errCode : 0 if no error
	// Invoked As : deallocate_CVDRIKCompressedHistogram( &compressedHist );
	////////////////////////////////////////////////////////////////////////////
	unsigned char deallocate_CVDRIKCompressedHistogram( CVDRIKCompressedHistogram* compressedHist ) ;
	
	/******************************
	 * 
	 * 
	 * 
	 * 			BHATTACHARYA COEFFICIENT
	 * 
	 * 
	 * *****************************************/
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for computing bhattacharya coefficient of two histograms
	// Inputs :-
	//			( CHistogram* ) histogram1histogram2 : Pointer to the first histogram
	//			( CHistogram* ) cvHist2: Pointer t othe second histogram 
	//			(float ) similarityValue : Bhattacharya Coefficient value
	// Outputs :-
	//			(unsigned char ) errCode : 0 if no error
	// Invoked As : computeBhattacharyaCoeff_CVDRIKHistogram( &histogram1 , &histogram2 , similarityValue );
	unsigned char compute_CVDRIKBhattacharyaCoefficient( CHistogram* histogram1, CHistogram* histogram2 , float 
	similarityValue );
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for computing bhattacharya coefficient of two histograms
	// Inputs :-
	//			( CVDRIKHistogram* ) cvHIst1 : Pointer to the first histogram
	//			( CVDRIkHistogram* ) cvHist2: Pointer t othe second histogram 
	//			(float ) bhattacharyaCoefficient : Bhattacharya Coefficient value
	// Outputs :-
	//			(unsigned char ) errCode : 0 if no error
	// Invoked As : computeBhattacharyaCoeff_CVDRIKHistogram( &cvHist1 , & cvHist2 , bhattacharyaCoefficient );
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned char computeBhattacharyaCoeff_CVDRIKHistogram( CVDRIKHistogram* cvHist1, CVDRIKHistogram* cvHist2 , float 
bhattacharyaCoefficient );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function for computing bhattacharya coefficient of two histograms
	// Inputs :-
	//			( CVDRIKCompressedHistogram* ) compressedHIst1 : Pointer to the first histogram
	//			( CVDRIKCompressedHistogram* ) compressedvHist2: Pointer t othe second histogram 
	//			(float ) bhattacharyaCoefficient : Bhattacharya Coefficient value
	// Outputs :-
	//			(unsigned char ) errCode : 0 if no error
	// Invoked As : computeBhattacharyaCoeff_CVDRIKCompressedHistogram( &compressedHist1 , & compressedHist2 , bhattacharyaCoefficient );
	////////////////////////////////////////////////////////////////////////////////////
	unsigned char computeBhattacharyaCoeff_CVDRIKCompressedHistogram( CVDRIKCompressedHistogram* compressedHist1 , 
CVDRIKCompressedHistogram* compressedHist2 , float bhattacharyaCoefficient );
#endif
