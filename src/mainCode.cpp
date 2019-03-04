
// Standard include files
#include <iostream>

// include files for OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv/cvaux.h>
//#include <opencv/cxcore.h>

// Include files for the GSl
#include <gsl/gsl_matrix.h>

// Include files defined by the user
#include"../include/elps.h"
#include"../include/weightedhistogram.h"
#include"../include/MeanShiftTracker.h"


// Namespaces used
using namespace cv;
using namespace std;

static void help()
{
    cout<<"\n**********************************************"<<endl;
    cout<<" MEAN SHIFT TRACKING"<<endl;
    cout<<"Draw a bounding box and press 'ESC' to start tracking."<<endl;
    cout<<"\n**********************************************"<<endl;
}

// Mouse callback function which handles the mouse events
void my_mouse_callback( int event, int x, int y, int flags, void* param );

bool destroy=false;
Rect box;
bool drawing_box = false;

// Draw a bounding box  on the given image
int draw_box(Mat * img, Rect rect)
{
    rectangle(*img, Point(box.x, box.y), Point(box.x+box.width,box.y+box.height),Scalar(255,255,255) ,1);

    //Rect rect2=Rect(box.x,box.y,box.width,box.height);
    return( 0 );
}

// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param )
{
    Mat* frame = (Mat*) param;

    switch( event )
    {
    case CV_EVENT_MOUSEMOVE:
    {
        if( drawing_box )
        {
            box.width = x-box.x;
            box.height = y-box.y;
        }
    }
        break;

    case CV_EVENT_LBUTTONDOWN:
    {
        drawing_box = true;
        box = Rect( x, y, 0, 0 );
    }
        break;

    case CV_EVENT_LBUTTONUP:
    {   drawing_box = false;
        if( box.width < 0 )
        {   box.x += box.width;
            box.width *= -1;
        }

        if( box.height < 0 )
        {   box.y += box.height;
            box.height *= -1;
        }
        draw_box(frame, box);

    }
        break;

    default:
        break;
    }
}

// main code
int main( int argc , char** argv )
{
    if( argc != 2 )
    {
        // Show Purpose
        printf( "\n\n Purpose : meanshift tracking in a sequence of images \n\n" );

        // Show Usage
        printf( "\n\n Usage As : meanShiftTracker < Parameter File Name >  \n\n" );

        // Show Sample Command Line
        printf( "\n\n Sample Command Line : ./meanShiftTracker meanShiftTracker-Params.txt \n\n" );

        return( 1 );
    }

    help();
    //cout<<"Main Code Execution started\n";

    // Opening the Parameter File for Reading
    FILE* fp;
    fp = fopen( argv[1] , "r" );
    if (!fp)
    {
        cout<<"ERROR : Unable to open parameter file\n";
        return(2);
    }

    // Reading the mean shift parameters
    char paramNameString[100] , eqString[3] , headerString[100];
    fscanf( fp , "%s" , headerString );
    float convergenceThreshold;
    fscanf( fp , "%s%s%f" , paramNameString , eqString , &convergenceThreshold );
    int maxNoOfIterations;
    fscanf( fp , "%s%s%d" , paramNameString , eqString , &maxNoOfIterations );
    int binsPerChannel;
    fscanf( fp , "%s%s%d" , paramNameString , eqString , &binsPerChannel );
    int subItrNumber;
    fscanf( fp , "%s%s%d" , paramNameString , eqString , &subItrNumber );
	fscanf( fp , "%s" , headerString );
	int imgSeqOrVideoFlag;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &imgSeqOrVideoFlag );
	char videoPath[200];
	fscanf( fp , "%s%s%s" ,paramNameString , eqString , videoPath );
	char imageSeqPath[100];
	fscanf( fp , "%s%s%s" ,paramNameString , eqString , imageSeqPath );
	char imageSeqNamesFile[100];
	fscanf( fp , "%s%s%s" ,paramNameString , eqString , imageSeqNamesFile );
	int initialBboxHardCodedFlag; 
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &initialBboxHardCodedFlag );
	int boxInitiallizationX;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &boxInitiallizationX );
	int boxInitiallizationY;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &boxInitiallizationY );
	int boxInitiallizationWIdth;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &boxInitiallizationWIdth );
	int boxInitiallizationHeight;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &boxInitiallizationHeight );
	int writeVideoFlag;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &writeVideoFlag );
	char opVideoFile[100];
	fscanf( fp , "%s%s%s" , paramNameString , eqString , opVideoFile );
	int groundTruthMarkingEnabled;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &groundTruthMarkingEnabled );	
	char groundTruthFile [100];
	fscanf( fp , "%s%s%s" , paramNameString , eqString , groundTruthFile );
    // Closing Parameter File;
    fclose( fp );
	
	// video capture object
	VideoCapture capture;
	
	
	
	// Starting and ending frame number for iterations
	double startingFrameNo ;// first frame 
	double endingFrameNo; // last frame
	
	int boxGTLeftX , boxGTLeftY , boxGTRightX , boxGTRightY, frameNoGT;
	
	FILE* fGTMark;
	if ( groundTruthMarkingEnabled ==1 ){
		fGTMark = fopen( groundTruthFile ,"r");
		
		
// 		if( fGTMark
		
		fscanf( fGTMark,"%d %d %d %d %d", &frameNoGT , &boxGTLeftX , &boxGTLeftY , &boxGTRightX , &boxGTRightY);
	}
		
	
	char imgFileName[100], imgFullPath[100];
	// image variable;
	Mat image;
	if ( imgSeqOrVideoFlag == 1)
	{
		// Read the path of the video file
		string video = string( videoPath );
		
				
		// reading a video
		capture.open(video); 
		
		//Check whether video is read
		if (!capture.isOpened())
		{
			cout << "specified video failed to open!" << endl;
			return 1;
		}
		
		// count the number of frames in the video
		double count = capture.get( CV_CAP_PROP_FRAME_COUNT ); //get the frame count
		
		// start and stop frame numbers
		startingFrameNo  = count - count ; // first frame 
		endingFrameNo    =  count; // last frame
		
		// Write the frame data from the image to the Mat container
		capture>>image;
		
	}
	else{
		int count = 0;
		FILE* fCount;
		fCount = fopen( imageSeqNamesFile,"r");

		for( char c = getc( fCount ); c != EOF ; c= getc( fCount ) ){
			if ( c == '\n')
				count ++;
		}
		// Close the file
		fclose( fCount );
		
		startingFrameNo =0;
		endingFrameNo  = count +1 ; // last line in the file wont have \n
		
		// Read the name of the first image
		fp = fopen( imageSeqNamesFile ,"r");
		fscanf( fp ,"%s", imgFileName );
		cout<<" file name " << imgFileName <<endl;
		
		// create the full path of the image sequence
		sprintf( imgFullPath ,"%s%s", imageSeqPath , imgFileName );
		image = imread( imgFullPath );
		// No of images specified in the file
		
		
	}
	
    if (!image.data)
    {
        cout<<"ERROR : cannot read image"<<endl;
        return( 1 );        // use echo $? at the terminal right after the execution of the main to know what value the main has returned

    }
    
    VideoWriter opVideo;
	
	if ( writeVideoFlag == 1){
		// reading a video
		opVideo.open(opVideoFile, CV_FOURCC('P','I','M','1'),20 , Size(image.cols, image.rows) ,true); 
		
		if (!opVideo.isOpened())
		{
			cout  << "Could not open the output video for write: " <<  endl;
			return -1;
		}
	}
	

	Mat first;
	
	first= image.clone();
	// Checking for valid image
    if (!first.data)
    {
        cout<<"ERROR : cannot read image"<<endl;
        return( 1 );        // use echo $? at the terminal right after the execution of the main to know what value the main has returned

    }
    
    // Window for displaying the image
    namedWindow("image");
	moveWindow("image", 200 , 200 );
    
    if( initialBboxHardCodedFlag == 0 )
	{
		
	
	
		// Associate a mouse callback with the particular window
		setMouseCallback("image", my_mouse_callback, &image);

		// Note: Only the final box drawn in the image is considered
		while( 1 )
		{
			Mat temp = image.clone();
			if (drawing_box)
				draw_box(&temp, box);


			imshow("image", temp);
			if (waitKey(15) == 27)
				break;
		}
	
		destroyWindow("image");
		//releaseImage(image);
	}
	else{
		
		// Assigining the bounding box parameters
		box.x = boxInitiallizationX;
		box.y = boxInitiallizationY;
		box.width = boxInitiallizationWIdth;
		box.height = boxInitiallizationHeight;
		
		rectangle( image , Point( boxInitiallizationX , boxInitiallizationY),Point(  
		boxInitiallizationX + boxInitiallizationWIdth-1, boxInitiallizationY + boxInitiallizationHeight -1 ), 
Scalar(255,255,255),1);
            		
			imshow("image", image);
			waitKey ( 0);
			opVideo.write( image);
			destroyWindow("image");
			//releaseImage(image);
		
	}
	
	// Mark the GT in first frame
	if( groundTruthMarkingEnabled == 1)
	{
		rectangle( image , Point( boxGTLeftX , boxGTLeftY),Point(boxGTRightY, boxGTRightY ), Scalar(0,255,0),1);
	}

    // Check whether bounding box is drawn or not
    if( box.width ==0 || box.height == 0)
    {
        cout<<"EXIT PROGRAM : Object Not Marked"<<endl;
        return(0);
    }

    // bounding Box width and height
    int bBoxWidth = box.width;
    int bBoxHeight = box.height;

    // Making the bounnding box size odd so that it will have a proper center pixel;
    if ( (bBoxWidth % 2) == 0 )
        bBoxWidth = bBoxWidth +1;

    if ( (bBoxHeight % 2) == 0 )
        bBoxHeight = bBoxHeight +1;

    // Major and minor axis of the ellipse contained in the bounding box
    int ellipseVAxis = ( bBoxHeight -1 )* 0.5;
    int ellipseHAxis = ( bBoxWidth -1 )* 0.5;;


    // initial center co-ordinates
    int initCenterY = box.y + ellipseVAxis ;
    int initCenterX = box.x + ellipseHAxis;

    // updating the box parameters
    box.height = bBoxHeight;
    box.width = bBoxWidth;

    //    elpsMask.setWeightArrayAndIndexArray();

    //    int size = elpsMAsk.getEllipseSize();
    //    float*weight = elpsMAsk.getElipseWeightArray();

    //    for ( int i =0; i< size ; i++)
    //    {
    //        cout<<" i = "<< i<<" | weight = "<<weight[i]<<"\n"<<endl;
    //    }

    //    WeightedHistogram wHistogram( binsPerChannel, 3);
    //    wHistogram.displayHistogramContents();

    cout<<"************BOUNDING BOX PARAMETERS***********\n";
    cout<<"y="<<box.y<<" | x="<<box.x<<" | width ="<<box.width <<" | height ="<<box.height<<" | center y= " << initCenterY<< " |  cente x =" << initCenterX<<endl;

    // variable which holds the error code returned from th different functions.
    int errCode;

    // Constructor for mean shift tracker
    MeanShiftTracker mst(  ellipseVAxis ,  ellipseHAxis,  binsPerChannel,  initCenterY,  initCenterX , 3);

    // Setting the weightArray of the ellipse in the tracker
    mst.setEllipseMaskWeight();

    /*//FOR DEBUGGING
    float ellipseYIndexSum =0;
    float ellipseXIndexSum =0;
    float ellipseWeightSum = 0;
    // Only pixels with non-zero weight falls inside the elliptical region
    for( int index =0 ; index < mst.m_EllipseMAsk.m_ArraySize ; index++)
    {
//        cout<<"index ="<< index<<" | Rel Y ="<<mst.m_EllipseMAsk.m_RelYIndexArray[ index]<<" | Rel X ="<< mst.m_EllipseMAsk.m_RelXIndexArray[ index] <<" weight ="<< mst.m_EllipseMAsk.m_WeightArray[ index ]<<endl;
        //if ( mst.m_EllipseMAsk.m_WeightArray[ index ]!=0 )
        {
            cout<<"index ="<< index<<" | Rel Y ="<<mst.m_EllipseMAsk.m_RelYIndexArray[ index]<<" | Rel X ="<< mst.m_EllipseMAsk.m_RelXIndexArray[ index] <<" weight ="<< mst.m_EllipseMAsk.m_WeightArray[ index ]<<endl;
            ellipseYIndexSum = ellipseYIndexSum + mst.m_EllipseMAsk.m_RelYIndexArray[ index] ;
            ellipseXIndexSum = ellipseXIndexSum + mst.m_EllipseMAsk.m_RelXIndexArray[ index];
            ellipseWeightSum = ellipseWeightSum + mst.m_EllipseMAsk.m_WeightArray[ index ];
        }
    }

    cout<<" ellipse:: YIndex Sum =" <<ellipseYIndexSum<<" | XIndex sum = "<<ellipseXIndexSum<<" | weight sum = "<< 
ellipseWeightSum<<endl; */
    //errCode = mst.displayTrackerInfo();

    cout<<"\nTARGET HISTOGEM VALUES:"<<endl;
    // Compute and set the target histogram in the tracker
    errCode =  mst.setTargetHistogram( first );
    if ( errCode !=0 )
    {
        cout<<"ERROR : error in setting the target histogram in the tracker"<<endl;
        return(1);
    }


    /*// FOR DEBUGGING
    float sum =0;
    cout<<"\nTARGET HISTOGRAM INTRACKER\n";
    for( int index =0 ; index < mst.m_TargetHistogram.m_HistogramSize ; index ++)
    {
        if( mst.m_TargetHistogram.m_HistogramData[index]!=0)
        {
            cout<<"dim = "<< index <<" | value ="<< mst.m_TargetHistogram.m_HistogramData[ index ]<<" ||";
            sum = sum + mst.m_TargetHistogram.m_HistogramData[ index ];
        }

    }
    cout<<" Sum ="<<sum<<endl;*/


//     // Variable which holds the name of the image along with its path
//     char imgName[100];

    // Create a new window for showing the frame by frame images
    namedWindow("CurrentFrame");
    moveWindow("CurrentFrame",200,200);

    cout<<"\n";
    cout<<" initial center Y ="<<initCenterY<<" | initial center X ="<< initCenterX<<endl;

    // File for writing the tracking coordinates in each frame
    FILE* fw;
    fw =fopen("test/trackingInfo.txt","w" );
	
	//image which holds the frames
	Mat frameData;
	
	// Writing the first frame to the output video
	if ( writeVideoFlag == 1){
		// reading a video
		opVideo.write( image );
	}
	
    // iterating through the frames i the sequence
	for( int frameCount = startingFrameNo+1; frameCount < endingFrameNo; frameCount++ )
    {
		
//         // Computing the image name
//         if ( frameCount>=0 && frameCount<=9 )
//         {
//             sprintf(imgName,"/home/mathew/Documents/DATASET/PETS2000/Images/test000%d.bmp", frameCount);
//             //cout<<imgName<<endl;
//         }
//         else if ( frameCount>=10 && frameCount<=99 )
//             sprintf(imgName,"/home/mathew/Documents/DATASET/PETS2000/Images/test00%d.bmp",frameCount);
//         else if ( frameCount>=100 && frameCount<=999 )
//             sprintf(imgName,"/home/mathew/Documents/DATASET/PETS2000/Images/test0%d.bmp",frameCount);
//         else if ( frameCount>=1000 && frameCount<=1400 )
//             sprintf(imgName,"/home/mathew/Documents/DATASET/PETS2000/Images/test%d.bmp",frameCount);



                // Computing the image name
//                if ( frameCount>=0 && frameCount<=9 )
//                {
//                    sprintf(imgName,"/home/mathew/Documents/DATASET/PETS 2009/Crowd_PETS09/S2/L1/Time_12-34/View_001/frame_000%d.jpg", frameCount);
//                    //cout<<imgName<<endl;
//                }
//                else if ( frameCount>=10 && frameCount<=99 )
//                    sprintf(imgName,"/home/mathew/Documents/DATASET/PETS 2009/Crowd_PETS09/S2/L1/Time_12-34/View_001/frame_00%d.jpg",frameCount);
//                else if ( frameCount>=100 && frameCount<=999 )
//                    sprintf(imgName,"/home/mathew/Documents/DATASET/PETS 2009/Crowd_PETS09/S2/L1/Time_12-34/View_001/frame_0%d.jpg",frameCount);
//                else if ( frameCount>=1000 && frameCount<=1400 )
//                    sprintf(imgName,"/home/mathew/Documents/DATASET/PETS 2009/Crowd_PETS09/S2/L1/Time_12-34/View_001/frame_%d.jpg",frameCount);

		if ( imgSeqOrVideoFlag == 0 ){
			
		    fscanf( fp ,"%s", imgFileName );
			
			// create the full path of the image sequence
			sprintf( imgFullPath ,"%s%s", imageSeqPath , imgFileName );
								
			// Reading the current frame
			frameData = imread( imgFullPath );
		}
		else{
			capture>>frameData;
		}

		cout<< " PROCESSING FRAME NO : " << frameCount << " at " <<imgFullPath<< endl;
        cout<< "\nPRE HISTOGEM VALUES:" << endl;
		
    int itr = 0;
    // MeanShift Iterations
    while ( itr <= maxNoOfIterations )
    {
        // Computing and setting the previous position histogram from the current frame
        mst.setPrevPostHistogram( frameData );


        /*// FOR DEBUGGING
        float sum =0;
        cout<<"\nPREV HISTOGRAM INTRACKER\n";
        for( int index =0 ; index < mst.m_PrevPosHistogram.m_HistogramSize ; index ++)
        {
            if( mst.m_PrevPosHistogram.m_HistogramData[ index ]!=0)
            {
                cout<<"dim = "<< index <<" | value ="<< mst.m_PrevPosHistogram.m_HistogramData[ index ]<<" ||";
                sum = sum + mst.m_PrevPosHistogram.m_HistogramData[ index ];
            }

        }
        cout<<" Sum ="<<sum<<endl;*/



        // Computing the bhattacharya coefficient between the target and the previous position histogram
        mst.setPrevPosBhattacharyaCoeff();
        cout<<"\n"<<" Pre BC ="<< mst.m_PrevPosBhattacharyaCoeff;


        cout<< "\n Mean Shift Iteration No : " << itr <<endl;

        // Compute and set the new center coordinate using mean shift procedure starting from the previous position
        mst.setCurrentPosition( frameData );
            cout<<"\n"<<"new Y= "<<mst.m_CurrCenterY<< "| new X ="<< mst.m_CurrCenterX<<endl;

            cout<<"\nCURR HISTOGEM VALUES:"<<endl;
            // Compute and set the current position histogram in the tracker
            mst.setCurrPosHistogram( frameData );

		/*// FOR DEBUGGING
				float sum =0;
				cout<<"\nCURR HISTOGRAM INTRACKER\n";
				for( int index =0 ; index < mst.m_CurrPosHistogram.m_HistogramSize ; index ++)
				{
					if( mst.m_CurrPosHistogram.m_HistogramData[index]!=0)
					{
					cout<<"dim = "<< index <<" | value ="<< mst.m_CurrPosHistogram.m_HistogramData[ index ]<<" ||";
					spinvum = sum + mst.m_CurrPosHistogram.m_HistogramData[ index ];
					}

				}
				cout<<" Sum ="<<sum<<endl;*/



            // Computing the bhattacharya coefficient between the target and the current position histogram
            mst.setCurrPosBhattacharyaCoeff();
            cout<< "\n" << "Curr BC =" << mst.m_CurrPosBhattacharyaCoeff;

            // Line search
            int subItrcCount = 0 ;
            while ( mst.m_CurrPosBhattacharyaCoeff  <= mst.m_PrevPosBhattacharyaCoeff )
            {
                // Update the current position value
                mst.m_CurrCenterY = ( mst.m_CurrCenterY + mst.m_PrevCenterY ) * 0.5;
                mst.m_CurrCenterX = ( mst.m_CurrCenterX + mst.m_PrevCenterX ) * 0.5;

                // Compute and set the current position histogram in the tracker
                mst.setCurrPosHistogram( frameData );



		/*// FOR DEBUGGING
		float sum =0;
		cout<<"\nCURR HISTOGRAM INTRACKER\n";
		for( int index =0 ; index < mst.m_CurrPosHistogram.m_HistogramSize ; index ++)
		{
			if( mst.m_CurrPosHistogram.m_HistogramData[index]!=0)
			{
			cout<<"dim = "<< index <<" | value ="<< mst.m_CurrPosHistogram.m_HistogramData[ index ]<<" ||";
			sum = sum + mst.m_CurrPosHistogram.m_HistogramData[ index ];
			}

		}
		cout<<" Sum ="<<sum<<endl;*/

                // Computing the bhattacharya coefficient between the target and the current position histogram
                mst.setCurrPosBhattacharyaCoeff();


                subItrcCount = subItrcCount + 1;
                cout<< "\nSub  Iteration No : " << subItrcCount <<endl;

                // Subiteration exit criteria check
                if ( subItrcCount == subItrNumber )
                    break;

            }





            // Convergence criteria check
            if( (  (mst.m_CurrCenterY - mst.m_PrevCenterY ) * (mst.m_CurrCenterY - mst.m_PrevCenterY ) +  ( mst.m_CurrCenterX - mst.m_PrevCenterX ) * ( mst.m_CurrCenterX - mst.m_PrevCenterX )  ) <= convergenceThreshold )
            {
                // Update previous center y coordinate as the current  center y coordinate for the next frame when the covergence is satisfied
                mst.setPrevCenterY();

                // Update previous center x coordinate as the current  center x coordinate
                mst.setPrevCenterX();

                // setting the previous position histogrm
                 //mst.setPrevPostHistogram(frameData );

                break;
            }
            else
            {
                // Update previous center y coordinate as the current  center y coordinate for the next iteration as well as for the next frame whwn the covergence is satisfied
                mst.setPrevCenterY();

                // Update previous center x coordinate as the current  center x coordinate
                mst.setPrevCenterX();

                // setting the previous position histogrm
                // mst.setPrevPostHistogram( frameData );

                // mst.setPrevPosBhattacharyaCoeff();
            }

            itr = itr +1;
        }

//        // Update previous center y coordinate as the current  center y coordinate for processing the next frame
//        mst.setPrevCenterY();

//        // Update previous center x coordinate as the current  center x coordinate for processing the next frame
//        mst.setPrevCenterX();


        cout<<"\n For next frame Prev = > CenterY = "<<mst.m_CurrCenterY<<" center X = "<<mst.m_CurrCenterX<<endl;

        // Displaying the currentr frame in the sequence after marking the bounding box and writing the tracking coordinates to a file.
        if( ( ( mst.m_CurrCenterY - ellipseVAxis ) >= 0 && ( mst.m_CurrCenterY + ellipseVAxis )  <= frameData.rows )  && ( ( mst.m_CurrCenterX - ellipseHAxis ) >= 0 && ( mst.m_CurrCenterX + ellipseHAxis )  <= frameData.cols ) )
        {
			// Draw the tracker box
            rectangle( frameData , Point( mst.m_CurrCenterX - ellipseHAxis , mst.m_CurrCenterY - ellipseVAxis ),Point(  
mst.m_CurrCenterX + ellipseHAxis , mst.m_CurrCenterY + ellipseVAxis), Scalar(255,0,0),1);
            fprintf( fw ,"%d %d %d ", frameCount, mst.m_CurrCenterY , mst.m_CurrCenterX);

            fprintf(fw, "\n");
			// Marking the ground truth
			if ( groundTruthMarkingEnabled ==1 ){
				fscanf( fGTMark,"%d %d %d %d %d", &frameNoGT , &boxGTLeftX , &boxGTLeftY , &boxGTRightX , &boxGTRightY);
				rectangle( frameData , Point( boxGTLeftX , boxGTLeftY),Point(boxGTRightY, boxGTRightY ), 
Scalar(0,255,0),1);
			}
			
			// Writing the output as video
			if ( writeVideoFlag == 1){
				// reading a video
				opVideo.write( frameData );
			}
            imshow("CurrentFrame", frameData);
            waitKey( 0 );

            // Marking the motion of object in image
//            first.at<cv::Vec3b>( mst.m_CurrCenterY , mst.m_CurrCenterX )[0] = 255;
//            first.at<cv::Vec3b>( mst.m_CurrCenterY , mst.m_CurrCenterX )[1] = 0;
//            first.at<cv::Vec3b>( mst.m_CurrCenterY , mst.m_CurrCenterX )[2] = 0;
            // Draw circle in trajectory
            circle (first , Point( mst.m_CurrCenterX  ,mst.m_CurrCenterY ),  2, Scalar( 255 , 0 ,0),1, 8, 0 );
        }
        else
        {
            cout<<"\nSTOPPED : Object BBox outside the image Boundary"<<endl;
           imshow( "CurrentFrame", first);
            waitKey(0);
            destroyWindow("CurrentFrame");
            return(1);
        }

    }

	// Close file pointers
	fclose( fp );
    fclose(fw);
	if ( groundTruthMarkingEnabled ==1 )
		fclose(fGTMark);
        return (0);


}
