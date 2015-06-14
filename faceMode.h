/*
	Cartoon Mode
	Sketch Mode
	Evil Mode

	Created by: Sambit Sarkar
	Created on: 12 June 2015

*/

#ifndef FACEMODES_H
#define FACEMODES_H

class faceModes{
private:
	int MEDIAN_BLUR_FILTER_SIZE;
	int LAPLACIAN_FILTER_SIZE;
	int EDGES_THRESHOLD;
	int repetitions;
	int EVIL_EDGE_THRESHOLD;

public:
	faceModes();
	cv::Mat cartoonMode(cv::Mat);
	cv::Mat sketchMode(cv::Mat);
	cv::Mat evilMode(cv::Mat);
	~faceModes();
};

#endif

