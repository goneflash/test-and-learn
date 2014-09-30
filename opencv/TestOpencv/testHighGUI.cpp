#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;	
using namespace cv;
 
int main( ){
	// Create black empty images
	srand(time(NULL));

	Mat image = Mat::zeros( 400, 400, CV_32FC3 );
	cout << "Row: " << image.rows << " Col: " << image.cols << endl;
	cout << "Step: " << image.step << " Dim: " << image.dims << endl;
	cout << "ElemSize: " << image.elemSize() << " Depth: " << image.depth() << endl;
	cout << "Channels: " << image.channels() << endl;
	// unsigned char *imgMat = (unsigned char*)(image.data);
	for (unsigned int i = 0; i < image.rows; i++)
		for (unsigned int j = 0; j < image.cols; j++){
			image.at<Vec3f>(i, j)[0] = rand() / (float)RAND_MAX;// * 255;
			image.at<Vec3f>(i, j)[1] = rand() / (float)RAND_MAX;// * 255;
			image.at<Vec3f>(i, j)[2] = rand() / (float)RAND_MAX;// * 255;
			// image.data[image.step * i + j + 0] = 1; //rand() / (float)RAND_MAX * 255;
			// image.data[image.step * i + j + 1] = 1; //rand() / (float)RAND_MAX * 255;
			// image.data[image.step * i + j + 2] = 1; //rand() / (float)RAND_MAX * 255; 
		} 
  	// Draw a circle 
  	circle( image, Point( 200, 200 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
  	imshow("Image",image);
 
  	waitKey( 0 );
  	return(0);
}