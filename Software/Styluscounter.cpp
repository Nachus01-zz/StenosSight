#include <opencv2/opencv.hpp>
#include <highgui.h>
using namespace cv;
using namespace std;

int main()
{
  //init
	VideoCapture cam0(1);
	if(!cam0.isOpened()) return 1;

	while(1)
	{
    //frames
		Mat frame0;   //original image
		Mat frame1;		//HSV image
		Mat red;      //red pen threshold
		Mat yellow;	  //yellow pen threshold
		Mat blue;   	//blue pen threshold
		vector<vector<Point> > bcontours;
		vector<vector<Point> > ycontours;
		vector<vector<Point> > rcontours;
		vector<Vec4i> heirarchy;
    
    //mapping
    cam0 >> frame0;

    //convert to HSV
		cvtColor(frame0,frame1,CV_BGR2HSV,0);

    //Threshold shades of red
    inRange(frame1,Scalar(120,128,128),Scalar(179,255,255),red);
    
    //Threshold shades of blue
		inRange(frame1,Scalar(100,128,128),Scalar(130,255,255),blue);

    //Threshold shades of yellow
		inRange(frame1,Scalar(0,128,128),Scalar(58,255,255),yellow);

    //Blue contours
    findContours(blue,bcontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

    //Yellow contours
    findContours(yellow,ycontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

    //Red contours
    findContours(red,rcontours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
    
    //Display each filtered image
    imshow("HSV",frame1);
    imshow("Original",frame0);
		imshow("reds",red);
		imshow("blues",blue);
		imshow("yellows",yellow);
    
    //Print a table with the number of pens in each color
		cout<<"Blue pens:"<<bcontours.size()<<"	"<<"Yellow pens:"<<ycontours.size()<<"	"<<"Red pens:"<<rcontours.size()<<endl;
    
    //Delay to prevent data overrun.
		int c=cvWaitKey(1);
	}
	return 0;
}
