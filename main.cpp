#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "faceModes.h"

using namespace std;
using namespace cv;

int main(){

	Mat src;
	namedWindow("Press esc to take a Pic", WINDOW_AUTOSIZE);
	VideoCapture cam(0);
	if(!cam.isOpened()){
		cout<<"Camera is not open. \n";
		exit(0);
	}

	while(1){
		Mat frame;
		cam>>frame;
		imshow("Press esc to take a Pic", frame);
		if(waitKey(30) == 27){
			src = frame;
			break;
		}
	}
	destroyWindow("Press esc to take a Pic"); 

	faceModes fm;
	cout<<"\n1: Sketch \n2: Cartoon \n3: Evil. \nEnter Your Choice: ";
	int ch;
	cin>>ch;
	if(ch == 1){
		Mat tmp = fm.sketchMode(src);
		imshow("Sketch", tmp);
		waitKey(0);
		imwrite("/home/ssarkar/Pictures/sketchMode.jpg", tmp);
	}

	if(ch == 2){
		Mat tmp = fm.cartoonMode(src);
		imshow("Cartoon", tmp);
		waitKey(0);
		imwrite("/home/ssarkar/Pictures/cartoonMode.jpg", tmp);
	}

	if( ch == 3){
		Mat tmp = fm.evilMode(src);
		imshow("Evil", tmp);
		waitKey(0);
		imwrite("/home/ssarkar/Pictures/evilMode.jpg", tmp);
	}

	return 0;
}

