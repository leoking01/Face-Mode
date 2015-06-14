#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "faceModes.h"

using namespace std;
using namespace cv;

// Constructor
	faceModes::faceModes(){
		this->MEDIAN_BLUR_FILTER_SIZE = 7;
		this->LAPLACIAN_FILTER_SIZE = 5;
		this->EDGES_THRESHOLD = 80;
		this->repetitions = 7;
		this->EVIL_EDGE_THRESHOLD = 12;
	}

/* 	This function converts the given image to its Cartoon form.
	First convert the image to gray sclae as median works only with them.
	Then it uses MEDIANBLUR to remove random noise without 
	compromising the edges. After that it uses LAPLACIAN to 
	find the edges. Then it removes unwanted edges using a 
	threshold.
*/
	Mat faceModes::sketchMode(Mat src){
		
	// Convert image to its gray scale
	// Syntax: cvtColor(input, output, change);
		cvtColor(src, src, CV_BGR2GRAY);

	// Use medianBlur to remove unwanted noise. MEDIAN_BLUR_FILTER_SIZE is just to change the amount
	// of blur. Higher the number, more the blur. But it has to be odd and greater than 1.
	// Syntax: medianBlur(input, output, ksize);
		medianBlur(src, src, this->MEDIAN_BLUR_FILTER_SIZE);

	// Use Laplacian to detect edges. More the ksize, better the detection including noise.
	// Ksize has to be odd and more than 1
	//Syntax: Laplacian(input, output, CV_8U, ksize);
		Laplacian(src, src, CV_8U, this->LAPLACIAN_FILTER_SIZE = 5);

	// Now using a threshold to segment the image. i.e. to remove the unwanted edges
	// Syntax: threshold(input, output, lowerLimit, HigherLimit, type); In this case, type is Binary Inverted
		threshold(src, src, this->EDGES_THRESHOLD, 255,THRESH_BINARY_INV);

		return src;

	}

/*
	This function converts the given image to its Sketch form.
	First convert the image to gray sclae as median works only with them.
	Then it uses MEDIANBLUR to remove random noise without 
	compromising the edges. After that it uses LAPLACIAN to 
	find the edges. Then it removes unwanted edges using a 
	threshold. Upto this a sketch of the image is created. Now we add 
	color to it. Basically we use bilateral filter to create a color mask 
	and then place the sketch over it.
*/

	Mat faceModes::cartoonMode(Mat src){

		Mat srcCopy;

	// Convert image to its gray scale
	// Syntax: cvtColor(input, output, change);
		cvtColor(src, srcCopy, CV_BGR2GRAY);

	// Use medianBlur to remove unwanted noise. MEDIAN_BLUR_FILTER_SIZE is just to change the amount
	// of blur. Higher the number, more the blur. But it has to be odd and greater than 1.
	// Syntax: medianBlur(input, output, ksize);
		medianBlur(srcCopy, srcCopy, this->MEDIAN_BLUR_FILTER_SIZE);

	// Use Laplacian to detect edges. More the ksize, better the detection including noise.
	// Ksize has to be odd and more than 1
	//Syntax: Laplacian(input, output, CV_8U, ksize);
		Laplacian(srcCopy, srcCopy, CV_8U, this->LAPLACIAN_FILTER_SIZE);

	// Now using a threshold to segment the image. i.e. to remove the unwanted edges
	// Syntax: threshold(input, output, lowerLimit, HigherLimit, type); In this case, type is Binary Inverted
		threshold(srcCopy, srcCopy, this->EDGES_THRESHOLD, 255, THRESH_BINARY_INV);

	// Bilateral filter takes some to process high res pictures and hence we will resize the image to 1/2 of its size
		Size size = src.size();
		Size smallSize;
		smallSize.width = size.width/2;
		smallSize.height = size.height/2;

	// Using the resize() we will resize the image into a new image.
	// Syntax: resize(input, output, newSize, 0, 0, INTER_LINEAR); The last attribute can change. In this case we are 
	// using bilinear interpolation
		Mat smallImg;
		resize(src, smallImg, smallSize, 0, 0, INTER_LINEAR);

	// Next we are setting up a Temp img element to do bilateral filter. The we apply the filter many times to make 
	// the cartoon effect better.
	// Now we use the bilateral filter to get the painting effect
	// Syntax: bilateralFilter(input, output, ksize, sigmaColor, sigmaSpace);
	// ksize - Diameter of each pixel neighborhood that is used during filtering. It affects the speed of processing
	// sigmaColor - Double, Filter sigma in the color space. 
	// sigmaSpace - Double, Filter sigma in the coordinate space. It affects the speed of processing.
	// We use bilateralFilter 2 because it cannot over-wright its input
		Mat temp = Mat(smallSize, CV_8UC3);
		for(int i = 0; i < this->repetitions; i++){
			bilateralFilter(smallImg, temp, 9, 7, 7);
			bilateralFilter(temp, smallImg, 9, 7, 7);
		}

	// Now we need to resize the image to its original size
		Mat bigImg, dst;
		resize(smallImg, bigImg, size, 0, 0, INTER_LINEAR);

	// Now in order to create the cartoon effect, we will copy the pixels of the bilateral filter output and put them in 
	// a black image. Then we will copy the pixels from the sketch into it.
		dst.setTo(0); // sets the pixels of dst to 0 i.e. black
		bigImg.copyTo(dst, srcCopy); // Syntax: inputImageToCopyFrom.copyTo(outputImage, maskToCopyFrom);

		return dst;
	}

/*
	This function converts the image into a very evil picture. First it converts it to gray scale.
	Then applies median blur to it and then creates a Scharr filter. Simultaneously it makes a 
	painting of the given image and then masks the painting with the output of the Charr filter.
*/

	Mat faceModes::evilMode(Mat src){

	// Creating a painting of the given image
		Size size = src.size();
		Size smallSize;
		smallSize.width = size.width/2;
		smallSize.height = size.height/2;
		Mat smallImg;
		resize(src, smallImg, smallSize, 0, 0, INTER_LINEAR);
		Mat temp = Mat(smallSize, CV_8UC3);
		for(int i = 0; i < this->repetitions; i++){
			bilateralFilter(smallImg, temp, 9, 7, 7);
			bilateralFilter(temp, smallImg, 9, 7, 7);
		}
		Mat bigImg, dst;
		resize(smallImg, bigImg, size, 0, 0, INTER_LINEAR);
		dst.setTo(0);

	// Creating the Scharr filter
		cvtColor(src, src, CV_BGR2GRAY);
		medianBlur(src, src, this->MEDIAN_BLUR_FILTER_SIZE);
		Mat edges, edges2;
		Scharr(src, edges, CV_8U, 1, 0);
		Scharr(src, edges2, CV_8U, 1, 0, -1);
		edges += edges2;
		threshold(edges, edges, this->EVIL_EDGE_THRESHOLD, 255, THRESH_BINARY_INV);
		medianBlur(edges, edges, 3);

	// Copy the pixels to dst
		bigImg.copyTo(dst, edges);

		return dst;
	}
