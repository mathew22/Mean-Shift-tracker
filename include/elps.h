#ifndef ELPS_H
#define ELPS_H

#include<iostream>


using namespace std;


/*********************************************************************************

            CLASS ELLIPSE MASK

 ********************************************************************************/

class EllipseMask
{
    public:
        // vertical axis of the ellipse
        int m_VerticalAxis;

        // Horizontal axis of the ellipse
        int m_HorizontalAxis;

        // Pointer to Relative index in the vertical direction considering the center pixels index as (0,0)
        int* m_RelYIndexArray;

        // Poiner to Relative index of a pixel in the horizontal  considering the center pixels index as (0,0)
        int* m_RelXIndexArray;


        // Pointer  to Relative index of the pixel
        //int* m_RelIndexArray;

        // Pointer to the weight value m_RelXIndexArray( Farther trh point from the center higher the weight value)
        float* m_WeightArray;

        // size of the weight m_RelXIndexArray
        int m_ArraySize;

    public:

        // Default constructor
        EllipseMask()
        {
            m_VerticalAxis    = -1;
            m_HorizontalAxis  = -1;
            m_RelYIndexArray  = NULL;
            m_RelXIndexArray  = NULL;
            //m_RelIndexArray   = NULL;
            m_WeightArray     = NULL;
            m_ArraySize       =-1;


        }

         // Destructor
        ~EllipseMask()
        {
            cout<<"DESTRUCTOR FOR ELLIPSEMASK\n";
            m_VerticalAxis    = -1;
            m_HorizontalAxis  = -1;
            m_ArraySize       = -1;

            delete[] m_RelYIndexArray;
            delete[] m_RelXIndexArray;
            //delete[] m_RelIndexArray;
            delete[] m_WeightArray;

        }

        // GETTERS

        int getVertAxis(){ return m_VerticalAxis;}
        int getHoriAxis(){return m_HorizontalAxis; }
        int getEllipseSize(){ return m_ArraySize;}

        int* getRelYIndex(){ return m_RelYIndexArray;}
        int* getRelXIndex(){ return m_RelYIndexArray;}
        float* getElipseWeightArray(){return  m_WeightArray;}

        // Display the contents
       int displayEllipseContents()
       {
           cout<<"************ELLIPSE PARAMETERS***********\n";
           cout<<"Vertcal Axis = "<<m_VerticalAxis<<" | Horizontal Axis = "<<m_HorizontalAxis<<"\n";
           cout<<"Ellipse Size = "<< m_ArraySize<<endl;
           cout<<"RelYIndx Address = "<<m_RelYIndexArray<<" | RelXIndx In Address = "<<m_RelXIndexArray<<" | WeightArray Address ="<<m_WeightArray<<"\n";
           return(0);

       }


        // Constructor with parameters
        EllipseMask( int verticalAxis , int horizontalAxis);

        int setWeightArrayAndIndexArray();

};


#endif // ELPS_H
