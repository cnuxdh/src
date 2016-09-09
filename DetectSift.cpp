//
//  DetectSift.cpp
//  
//
//  Created by 谢东海 on 16/8/26.
//
//

#include <stdio.h>


//opencv
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"


//siftlib





int main(int argc, char* argv[])
{
    printf("detect sift feature.... \n");
    
    printf("the parameter number: %d \n", argc);
    
    
    if(argc<2)
    {
        printf("no parameters ! \n");
        return 1;
    }
    
    char   filename[256];
    strcpy(filename, argv[1]);
    
    IplImage* pImage = cvLoadImage(filename, 0);
    IplImage* pColor = cvLoadImage(filename, 1);
	  //cvNamedWindow("image");
		//cvShowImage("image", pImage);
	   
	   
		int ht = pImage->height;
		int wd = pImage->width;
		int scanWd = pImage->widthStep;
    
    float* fImage;
		int ht,wd;
		IplImageToFloatImage(pImage, &fImage, &ht, &wd);

		printf("Sift Feature detection ... \n");
		int keynumber = 0;
		Key_Point* featPts = SiftFeaturesFloat(fImage, wd, ht, keynumber);

		printf("Feature Number: %d \n", keynumber);

		for(int i=0; i<keynumber; i++)
		{
			CvScalar color = CV_RGB(255,0,0);
			CvPoint tp;
			tp.x = featPts[i].key_column;
			tp.y = featPts[i].key_row;
			cvLine( pColor, cvPoint( tp.x-5,tp.y), cvPoint( tp.x+5,tp.y),CV_RGB(255,0,255),1,8,0);
			cvLine( pColor, cvPoint( tp.x,tp.y-5), cvPoint( tp.x,tp.y+5),CV_RGB(255,0,255),1,8,0);		
		}
    
    cvNamedWindow("image");
		cvShowImage("image", pColor);
    
    cvWaitKey(0);
    
    cvReleaseImage(&pImage);
    cvReleaseImage(&pColor);
    free(fImage);
    free(featPts);
    
    return 0;
}
