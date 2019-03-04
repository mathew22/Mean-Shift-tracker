#include "../include/elps.h"


// Constructor with parameters
EllipseMask::EllipseMask( int verticalAxis , int horizontalAxis)
{
    cout<<"Constructing the ellipse mask\n";


    m_VerticalAxis    = verticalAxis;
    m_HorizontalAxis  = horizontalAxis;

    m_ArraySize     = (2*m_VerticalAxis + 1) * (2*m_HorizontalAxis+1 );

    m_RelYIndexArray  = new int[m_ArraySize];
    m_RelXIndexArray  = new int[ m_ArraySize];
    //m_RelIndexArray   = new int[m_ArraySize];
    m_WeightArray     = new float[m_ArraySize];


    // Initializing the weight array to 0 vales
    for( int index = 0 ; index < m_ArraySize ; index++)
    {
        m_WeightArray[ index ]=0;
    }



}

int  EllipseMask::setWeightArrayAndIndexArray()
{

    float invVaxis = 1.0 / m_VerticalAxis;
    float invHaxis = 1.0 /m_HorizontalAxis;


    int count =0;
    // Computing the dimension of the mask( ie tthe no of pixels which lies inside the  ellipse )
    for ( int y = -m_VerticalAxis; y <= m_VerticalAxis ; y++ )
    {
        for (int x = -m_HorizontalAxis ; x <= m_HorizontalAxis ; x++ )
        {


            float weightValue = 1 - ( y * y * invVaxis * invVaxis ) - ( x * x * invHaxis * invHaxis );
            if ( weightValue >0 )
            {
//                m_RelYIndexArray[count] = y;
//                m_RelXIndexArray[count] = x;
                //m_WeightArray[count]= weightValue;
               // normConstant = normConstant + weightValue;
                count = count +1 ;

            }


        }
    }

    // Update the array size
    m_ArraySize = count;

    //delete and reallocate memory
    delete[] m_RelYIndexArray;
    delete[] m_RelXIndexArray;
    delete[] m_WeightArray;

    // allocate new memory
    m_RelYIndexArray = new int[ m_ArraySize ];
    m_RelXIndexArray = new int[ m_ArraySize ];
    m_WeightArray = new float [m_ArraySize ];

    count = 0 ;
    float normConstant = 0.0;
    // Writing values to the mask
    for ( int y = -m_VerticalAxis; y <= m_VerticalAxis ; y++ )
    {
        for (int x = -m_HorizontalAxis ; x <= m_HorizontalAxis ; x++ )
        {


            float weightValue = 1 - ( y * y * invVaxis * invVaxis ) - ( x * x * invHaxis * invHaxis );
            if ( weightValue >0 )
            {
                m_RelYIndexArray[count] = y;
                m_RelXIndexArray[count] = x;
                m_WeightArray[count]= weightValue;
               normConstant = normConstant + weightValue;
               count = count +1 ;

            }


        }
    }


    // Normalizing the weight values
    for( int index =0 ; index < m_ArraySize ; index++)
    {
        m_WeightArray[index] = m_WeightArray[index]/normConstant;

    }

    return( 0 );

}
