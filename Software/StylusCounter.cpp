#include <opencv2/opencv.hpp>
#include <highgui.h>

#define KERNEL 16

using namespace cv;
using namespace std;

int main()
{
	//init
	VideoCapture cam1(1);				//retreive image from (non-default) camera
	if(!cam1.isOpened()) return 1;			//error if camera fails to load

	//variables
    	int erosion_sizeR=3;		                //erosion element size (square)
    	int erosion_sizeY=2;
    	int erosion_sizeB=3;
    	int dilation_sizeR0=2;  	                //dilation element size (square)
    	int dilation_sizeR=6;
    	int dilation_sizeY=6;
    	int dilation_sizeY0=1;
    	int dilation_sizeB=3;
    	Mat frame0;                     	        //original image
    	Mat frame1;                             	//median blur
    	Mat frame2;                             	//HSV image
    	Mat frame3;                             	//eroded image
    	Mat erosion_elementR = getStructuringElement(MORPH_RECT,Size(2*erosion_sizeR+1,2*erosion_sizeR+1),Point(erosion_sizeR,erosion_sizeR));
    	Mat erosion_elementY = getStructuringElement(MORPH_RECT,Size(2*erosion_sizeY+1,2*erosion_sizeY+1),Point(erosion_sizeY,erosion_sizeY));
    	Mat erosion_elementB = getStructuringElement(MORPH_RECT,Size(2*erosion_sizeB+1,2*erosion_sizeB+1),Point(erosion_sizeB,erosion_sizeB));
    	Mat dilation_elementR0 = getStructuringElement(MORPH_RECT,Size(2*dilation_sizeR0+1,2*dilation_sizeR0+1),Point(dilation_sizeR0,dilation_sizeR0));
    	Mat dilation_elementR = getStructuringElement(MORPH_RECT,Size(2*dilation_sizeR+1,2*dilation_sizeR+1),Point(dilation_sizeR,dilation_sizeR));
    	Mat dilation_elementY = getStructuringElement(MORPH_RECT,Size(2*dilation_sizeY+1,2*dilation_sizeY+1),Point(dilation_sizeY,dilation_sizeY));
    	Mat dilation_elementY0 = getStructuringElement(MORPH_RECT,Size(2*dilation_sizeY0+1,2*dilation_sizeY0+1),Point(dilation_sizeY0,dilation_sizeY0));
    	Mat dilation_elementB = getStructuringElement(MORPH_RECT,Size(2*dilation_sizeB+1,2*dilation_sizeB+1),Point(dilation_sizeB,dilation_sizeB));
    	Mat red;                                	//red pen threshold
    	Mat yellow;                             	//yellow pen threshold
    	Mat blue;                               	//blue pen threshold
    	Mat eroder;                             	//eroded red
    	Mat erodey;                             	//eroded yellow
    	Mat erodeb;                             	//eroded blue
    	Mat dilater0;                           	//dilated red
    	Mat dilater;
    	Mat dilatey;                            	//dilated yellow
    	Mat dilatey0;
    	Mat dilateb;                            	//dilated blue
    	Mat cloner;
    	Mat cloney;
    	Mat cloneb;
    	vector<vector<Point> > bcontours;       	//blue contours
    	vector<vector<Point> > ycontours;       	//yellow contours
    	vector<vector<Point> > rcontours;       	//red contours
	vector<Vec4i> heirarchy;
	
	while(1)
	{
	  	//mapping
	   	cam1 >> frame0;

		//median blur
		for (int i=1; i<KERNEL; i=i+2)		//move kernel two pixels at a time
	       	{
			medianBlur (frame0,frame1,i);
		}

	   	//convert to HSV
		cvtColor(frame1,frame2,CV_BGR2HSV,0);

	   	//Threshold shades of red
		inRange(frame2,Scalar(120,80,128),Scalar(179,255,255),red);

	   	//Threshold shades of blue
		inRange(frame2,Scalar(100,80,100),Scalar(130,255,255),blue);

	   	//Threshold shades of yellow
		inRange(frame2,Scalar(8,80,100),Scalar(64,255,220),yellow);

		//1st dilate
		dilate(red,dilater1,dilation_elementR0);
        	dilate(yellow,dilatey0,dilation_elementY0);

		//Erode
		erode(dilater0,eroder, erosion_elementR);
		erode(dilatey0,erodey, erosion_elementY);
		erode(blue,erodeb, erosion_elementB);

		//Dilate
		dilate(eroder,dilater,dilation_elementR);
		dilate(erodey,dilatey,dilation_elementY);
		dilate(erodeb,dilateb,dilation_elementB);

		cloner=dilater.clone();
		cloney=dilatey.clone();
		cloneb=dilateb.clone();

		//Blue contours
		findContours(cloneb,bcontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		//Yellow contours
		findContours(cloney,ycontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		//Red contours
		findContours(cloner,rcontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		//Displayed images are required for debugging purposes only
		imshow("Original",frame0);
		imshow("blur",frame1);
		imshow("HSV",frame2);
		imshow("Red",red);
		imshow("Yellow",yellow);
		imshow("DilateY0",dilatey0);
		imshow("Blue",blue);
		imshow("DilateR0",dilater0);
		imshow("ErodeR", eroder);
		imshow("ErodeY",erodey);
		imshow("ErodeB",erodeb);
		imshow("DilateR", dilater);
		imshow("DilateY",dilatey);
		imshow("DilateB",dilateb);

		//Output counts
		cout<<"Blue pens:"<<bcontours.size()/2<<"	"<<"Yellow pens:"<<ycontours.size()/2<<"	"<<"Red pens:"<<rcontours.size()/2<<endl;

		int c=cvWaitKey(1);
	}
	return 0;
}

