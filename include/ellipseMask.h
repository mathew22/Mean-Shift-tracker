/******************************************************************************

   PROJECT:       CVDRIK
   FILE:          CVDRIKEllipseMask.h
   AUTHOR:
   DATE:
   DESCRIPTION:   Header File for the mask values in an ellipse

*****************************************************************************/
#ifndef ELLIPSEMASK_H
#define ELLIPSEMASK_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <ctype.h>


/*********************************************************************************

            DATA STRUCTURE FOR ELLIPSE MASK
 
 ********************************************************************************/

struct CELpsMask2D
{
    // vertical axis of the ellipse
    int m_aY;

    // Horizontal axis of the ellipse
    int m_aX;

    // Pointer to Relative index in the vertical direction
    int* m_RelYIndexArray;

    // Poiner to Relative index of a pixel in the horizontal direction wrto the center
    int* m_RelXIndexArray;

    // Pointer  to Relative index of the pixel
    int* m_RelIndexArray;

    // Pointer to the weight value m_RelXIndexArray
    float* m_WeightArray;

    // size of the weight m_RelXIndexArray
    int m_ArraySize;
	
};

// Function for Initializing a CELpsMask2D 
// Inputs :-
//			(CELpsMask2D*) elpsMask: Pointer to the CELpsMask2D structure
// Outputs :-
//			None
// Invoked As : initialize_CVDRIKCELpsMask2D(( &elpsMask );
void initialize_CVDRIKCELpsMask2D( CELpsMask2D* elpsMask );

// Function for allocating memory to CELpsMask2D 
// Inputs :-
//			(CELpsMask2D*) elpsMask: Pointer to the CELpsMask2D structure
//			( int ) aY : vertical axis of the ellipse 
//			( int ) aX : horizontal axis of the ellipse 
//			( int ) widthStepOfImage : widthstep of the image under consideration
//			( int ) channelsInImage : no of channels inthe image
// Outputs :-
//			( unsigned char ) errCode: error code value
// Invoked As : allocate_CVDRIKCELpsMask2D(( &elpsMask , aY ,  aX ,  widthStepOfImage ,  channelsInImage );
unsigned char allocate_CVDRIKCELpsMask2D( CELpsMask2D* elpsMask , int aY , int aX , int widthStepOfImage , int channelsInImage );


// Function for dallocating memory of CELpsMask2D 
// Inputs :-
//			(CELpsMask2D*) elpsMask: Pointer to the CELpsMask2D structure
// Outputs :-
//			( unsigned char ) errCode: error code value
// Invoked As : deallocate_CVDRIKCELpsMask2D(( &elpsMask);
unsigned char deallocate_CVDRIKCELpsMask2D( CELpsMask2D* elpsMask );

// Function for reallocating memory a CELpsMask2D 
// Inputs :-
//			(CELpsMask2D*) elpsMask: Pointer to the CELpsMask2D structure
//			( int ) aY : vertical axis of the ellipse 
//			( int ) aX : horizontal axis of the ellipse 
//			( int ) widthStepOfImage : widthstep of the image under consideration
//			( int ) channelsInImage : no of channels inthe image
// Outputs :-
//			( unsigned char ) errCode: error code value
// Invoked As : reallocate_CVDRIKCELpsMask2D(( &elpsMask , aY ,  aX ,  widthStepOfImage ,  channelsInImage );
unsigned char reallocate_CVDRIKCELpsMask2D( CELpsMask2D* elpsMask , int aY , int aX , int widthStepOfImage , int 
nChannels );

#endif  //ELLIPSEMASK
