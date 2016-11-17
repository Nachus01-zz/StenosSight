#include <opencv2/opencv.hpp>
#include <highgui.h>

#define KERNEL 16

using namespace cv;
using namespace std;

int main()
{
	//init
	VideoCapture cam1(1);				//retreive image from (non-default) camera
	if(!cam0.isOpened()) return 1;			//error if camera fails to load

	//variables
	int erosion_size=3;			//erosion element size (square)
	int dilation_size=3;			//dilation element size (square
	Mat frame0;				//original image
	Mat frame1;				//median blur
	Mat frame2;				//HSV image
	Mat frame3;				//eroded image
	Mat erosion_element = getStructuringElement(MORPH_RECT,Size(2*erosion_size+1,2*erosion_size+1),Point(erosion_size,erosion_size));
	Mat dilation_element = getStructuringElement(MORPH_RECT,Size(2*dilation_size+1,2*dilation_size+1),Point(dilation_size,dilation_size));
	Mat red;         			//red pen threshold
	Mat yellow;	     			//yellow pen threshold
	Mat blue;   	  			//blue pen threshold
	Mat eroder;				//eroded red
	Mat erodey;				//eroded yellow
	Mat erodeb;				//eroded blue
	Mat dilater;				//dilated red
	Mat dilatey;				//dilated yellow
	Mat dilateb;				//dilated blue
	vector<vector<Point> > bcontours;	//blue contours
	vector<vector<Point> > ycontours;	//yellow contours
	vector<vector<Point> > rcontours;	//red contours
	vector<Vec4i> heirarchy;
	
	while(1)
	{
	      	//mapping
	      	cam1 >> frame0;

		//median blur
		for ( int i = 1; i < KERNEL; i = i + 2 )
        	{
			medianBlur (frame0,frame1 , i );
		}

	      	//convert to HSV
		cvtColor(frame1,frame2,CV_BGR2HSV,0);

	      	//Threshold shades of red
		inRange(frame1,Scalar(120,80,128),Scalar(179,255,255),red);

	      	//Threshold shades of blue
		inRange(frame1,Scalar(100,80,128),Scalar(130,255,255),blue);

	      	//Threshold shades of yellow
		inRange(frame1,Scalar(8,100,128),Scalar(70,255,255),yellow);

		//Erode

		erode(red,eroder, erosion_element);
//		erode(yellow,erodey, erosion_element);
		erode(blue,erodeb, erosion_element);



		//Dilate

		dilate(eroder,dilater,dilation_element);
		dilate(yellow,dilatey,dilation_element);
		dilate(erodeb,dilateb,dilation_element);

		//Blue contours
		findContours(dilateb,bcontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		//Yellow contours
		findContours(dilatey,ycontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		//Red contours
		findContours(dilater,rcontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		//Display test images
		imshow("Original",frame0);
		imshow("HSV",frame2);
		imshow("Red",dilater);
		imshow("Yellow",dilatey);
		imshow("Blues",dilateb);

		//Output counts
		cout<<"Blue pens:"<<bcontours.size()/2<<"	"<<"Yellow pens:"<<ycontours.size()/2<<"	"<<"Red pens:"<<rcontours.size()/2<<endl;

		int c=cvWaitKey(1);
	}
	return 0;
}

