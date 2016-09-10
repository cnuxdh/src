

#include"stdio.h"
#include"string.h"
#include"stdlib.h"

#include"panorama.hpp"


//opencv
#ifdef OPENCV_1X 
	#include "cv.h"
	#include "highgui.h"
	#include "cxcore.h"
#else
	#include "opencv2/core/core.hpp"
	#include "opencv2/highgui/highgui.hpp"
	#include "opencv2/calib3d/calib3d.hpp"
	using namespace cv;
#endif



#define PI 3.1415926


int main(int argc, char* argv[])
{
	printf("Panoram reprojection ..... \n");
	
	if(argc!=6)
	{
			printf("input error ! \n");
			printf("usage: *.exe vertical_fov  horizonal_fov  lat  lon  \n");
			return -1;
	}
	
	char   filename[256];	
	strcpy(filename, argv[1]);
	double vangle = atof(argv[2]); //fov vertical angle
	double hangle = atof(argv[3]); //fov horizonal angle
	double lat = atof(argv[4]) / 180.0 * PI; //from top to bottom: 0-180
	double lon = atof(argv[5]) / 180.0 * PI; //from left to right: 0-360
	
	
	//double widthAngle = PI*(1.0/3.0); // 60 degree
	
	printf("input image: %s \n", filename);
	printf("input angle: %lf %lf \n", vangle,hangle);
	
	//calculate the direction according to (lon,lat), must be the opposite
	double direction[3];
	direction[0] = -sin(lon)*sin(lat);  //x
	direction[1] = -cos(lon)*sin(lat);  //y
	direction[2] = -cos(lat);           //z
	
	printf("direction: %lf %lf %lf \n", direction[0], direction[1], direction[2]);
	char outfile[256];
	sprintf(outfile, "%d_%d.jpg", int(lat/PI*180), int(lon/PI*180));
	PanoToPlane(filename, outfile, vangle, hangle, direction, 1);
	
	
	
	/*
	IplImage* pImage = cvLoadImage(filename, 0);
	//cvNamedWindow("panoimage");
	//cvShowImage("panoimage", pImage);
	
	int ht = pImage->height;
	int wd = pImage->width;
	int scanWd = pImage->widthStep;
	double radius = double(wd)/(2*PI);
	printf("radius: %lf \n", radius);
	
	//calculate the projection plane size
	double radianAngle = 1.0 / 180.0 * PI;
	double projFocus = radius*0.8;//*sin(radianAngle*0.5);
	printf("proj focus: %lf \n", projFocus);
	
	int projHt = projFocus*tan(vangle*radianAngle*0.5)*2;
	int projWd = projFocus*tan(hangle*radianAngle*0.5)*2;
	
	printf("proj width:%d   height:%d \n", projWd, projHt);
	
	//image reprojection
	IplImage* planeImage = cvCreateImage( cvSize(projWd, projHt), 8, 1);
	int projScanWd = planeImage->widthStep;
	
	double gx[2],gy,gz;
	gx[0] = projFocus;  //*cos(radianAngle*0.5);
	gx[1] = -projFocus;
	
	for(int i=0; i<2; i++){
		printf("%d \n", i);
		for(int z=-projHt*0.5; z<projHt*0.5; z++){
		//printf(".");	
		for(int y=-projWd*0.5; y<projWd*0.5; y++){  
				
				int pj = projHt*0.5 - z;
				int pi = -y + projWd*0.5;
			  
			  gy = y;
			  gz = z;
			  
				double ix, iy;
				GrdToSphere( gx[i], gy, gz, radius, ix, iy);	
				//printf("%lf %lf \n", ix, iy);
				
				//ix = min(wd-1, ix);
				//iy = min(ht-1, iy);
				if(ix>=wd) ix=wd-1;
				if(iy>=ht) iy=ht-1;
				int nx = ix;
				int ny = iy;
				planeImage->imageData[pj*projScanWd + pi] = pImage->imageData[ny*scanWd+nx];
				
			}
		}
		
		char file[256];
		sprintf(file, "%s_proj_%d.jpg", filename, i);
		cvSaveImage(file, planeImage);
	}
	//cvNamedWindow("projimage");
	//cvShowImage("projimage", planeImage);
	
	
	//cvWaitKey(0);
	
	cvReleaseImage(&planeImage);
	cvReleaseImage(&pImage);
	*/
	
	
	
	
	printf("Finished! \n");
	
	
	return 0;
}