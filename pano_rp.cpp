

#include"stdio.h"


//5point lib
#include"5point.h"


//opencv lib
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"




int main()
{
	
	printf("relative pose estimation ....  \n");
	
	printf("%d \n", argc);

	char leftImageFile[256];
	char rightImageFile[256];
	char mosaicFile[256];
	char outFile[256];
	//double focalLen = 0;
	bool bIsBundlerOut = false;
	

	strcpy(leftImageFile, "\0");
	strcpy(rightImageFile, "\0");


	//get the param
	for(int i=1; i<argc; i++)
	{
		//printf("%s \n", argv[i]);

		if( strcmp( argv[i], "-left" ) == 0 ) //left image
		{
			strcpy(leftImageFile, argv[i+1]);
			printf("%s \n", leftImageFile);
			i++;
		}
		else if( strcmp( argv[i], "-right" ) == 0 ) //right image
		{
			strcpy(rightImageFile, argv[i+1]);
			printf("%s \n", rightImageFile);
			i++;
		}
		else if( strcmp( argv[i], "-out" ) == 0 ) //output file
		{
			strcpy(outFile, argv[i+1]);
			printf("%s \n", outFile);
			i++;
		}
		else if( strcmp( argv[i], "-bundlerout" ) == 0 ) //output same as bundler
		{
			bIsBundlerOut = true;
			printf("output according to bundler's format! \n");
		}
		else if( strcmp( argv[i], "-mosaic" ) == 0 )
		{
			strcpy(mosaicFile, argv[i+1]);
			printf("mosaic file: %s \n", mosaicFile);
			i++;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if( strlen(leftImageFile)<3 || strlen(leftImageFile)<3)
	{
		printf("no files... \n");
		return -1;
	}


  IplImage* lImage = cvLoadImage(file1);
	IplImage* rImage = cvLoadImage(file2);
	
	printf("%s \n", file1);
	printf("%s \n", file2);

	//1. feature detection
	CFeatureBase* pFeatDetect = new CSIFTFloat(); 
	//CFeatureBase* pFeatDetect = new CSURF(); 
	ImgFeature lImageFeats;
	ImgFeature rImageFeats;
	pFeatDetect->Detect(file1, lImageFeats);
	pFeatDetect->Detect(file2, rImageFeats);
	
	//convert from spherical to real 3D
	int ht = lImage->height;
	int wd = lImage->width;

	
	//2. matching
	printf("Matching... \n");
	CMatchBase* pMatch = new CPanoMatch();
	vector<MatchPairIndex> mi;
	pMatch->Match(lImageFeats, rImageFeats, mi);
	printf("Match Pair: %d \n", mi.size());
	

	//3. pose estimation and compute residual error 
	double ratio = 0.6;
	vector<Point3DDouble> lptPano;     //3D spherical points
	vector<Point3DDouble> rptPano;
	vector<Point2DDouble> lptImagePts; //image points
	vector<Point2DDouble> rptImagePts;
	vector<int> matchIndex;
	double radius = (double)(wd) / (2*PI); 

	for(int i=0; i<mi.size(); i++)
	{
		int li = mi[i].l;
		int ri = mi[i].r;

		CvPoint pl;
		pl.x = lImageFeats.featPts[li].col;
		pl.y = lImageFeats.featPts[li].row;
		CvPoint pr;
		pr.x = rImageFeats.featPts[ri].col;
		pr.y = rImageFeats.featPts[ri].row;

		//remove the match without motion
		double dis = sqrt( (double)( (pl.x-pr.x)*(pl.x-pr.x) + (pl.y-pr.y)*(pl.y-pr.y)) );
		if(dis<5) continue;
		
		if( pl.y<ht*ratio && pr.y<ht*ratio )
		{
			Point2DDouble ip;
			ip.p[0] = pl.x;
			ip.p[1] = pl.y;
			lptImagePts.push_back(ip);
			ip.p[0] = pr.x;
			ip.p[1] = pr.y;
			rptImagePts.push_back(ip);

			Point3DDouble gp;					
			SphereTo3D(pl.x, pl.y, radius, gp.p[0], gp.p[1], gp.p[2]);
			lptPano.push_back(gp);
			SphereTo3D(pr.x, pr.y, radius, gp.p[0], gp.p[1], gp.p[2]);
			rptPano.push_back(gp);
		}
	}

	double R[9];
	double T[3];
	vector<double> residual;
	residual.resize(lptPano.size());
	EstimatePose5Point_Pano(lptPano, rptPano, radius, 500, 2.5, R, T, residual);

	//output the relative pose estimation result
	FILE* fp = fopen("pano_rt.txt", "w");		
	fprintf(fp, "%d %d \n", ht, wd);
	for(int j=0; j<3; j++)
	{
		for(int i=0; i<3; i++)
		{
			fprintf(fp, " %lf ", R[j*3+i]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, " %lf %lf %lf \n", T[0], T[1], T[2]);
	fclose(fp);
	
	double E[9];
	CalculateEssentialMatrix(R, T, E);
	fp = fopen("panoFM.txt", "w");
	for(int j=0; j<3; j++)
	{
		for(int i=0; i<3; i++)
		{
			fprintf(fp, "%lf ", E[j*3+i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	     

	//4. remove outliers and display
	srand(100);
	IplImage* pMosaic = VerticalMosaic(lImage, rImage);

	for(int i=0; i<lptImagePts.size(); i++)
	{				
		//if(residual[i]>2.5)
		//	continue;

		int r,g,b;
		if(residual[i]>2.5)
		{
			//continue;
			r = 0;
			g = 0;
			b = 0;

		}
		else
		{
			r = (double)(rand())/(double)(RAND_MAX)*255;
			g = (double)(rand())/(double)(RAND_MAX)*255;
			b = (double)(rand())/(double)(RAND_MAX)*255;

			CvPoint pl;
			pl.x = lptImagePts[i].p[0];
			pl.y = lptImagePts[i].p[1];
			CvPoint pr;
			pr.x = rptImagePts[i].p[0];
			pr.y = rptImagePts[i].p[1];	

			cvDrawCircle(lImage, pl, 2, CV_RGB(r,g,b),2);
			cvDrawCircle(rImage, pr, 2, CV_RGB(r,g,b),2);
			cvDrawLine(pMosaic, pl, cvPoint( pr.x, pr.y+ht ), CV_RGB(255,0,0));
		}				
	}

	/*
	cvNamedWindow("Match1");
	cvShowImage("Match1", lImage);
	cvNamedWindow("Match2");
	cvShowImage("Match2", rImage);
	*/

	cvSaveImage("left.jpg",  lImage);
	cvSaveImage("right.jpg", rImage);
	cvSaveImage("match.jpg", pMosaic);
	cvReleaseImage(&pMosaic);		


	//5. 3D reconstruction and output
	double dis = 4.94;     //the real distance of 
	double R0[9];
	double T0[3];
	memset(R0, 0, sizeof(double)*9);
	memset(T0, 0, sizeof(double)*3);
	R0[0] = R0[4] = R0[8] = 1;

	vector<Point3DDouble> vPts;
	vector<Point3DDouble> vColors;
	for(int i=0; i<lptPano.size(); i++)
	{
		Point3DDouble gp;
		if(residual[i]<3)
		{
			Point2DDouble lp,rp;
			//normalized coordinates
			lp.p[0] = lptPano[i].p[0] / lptPano[i].p[2];
			lp.p[1] = lptPano[i].p[1] / lptPano[i].p[2];
			rp.p[0] = rptPano[i].p[0] / rptPano[i].p[2];
			rp.p[1] = rptPano[i].p[1] / rptPano[i].p[2];
			double error = 0;
			Point3DDouble gp = TriangulatePt(lp, rp, R0, T0, R, T, &error);
			gp.p[0] *= dis;
			gp.p[1] *= dis;
			gp.p[2] *= dis;

			if( fabs(gp.p[0])>50 || fabs(gp.p[1])>50 || fabs(gp.p[2])>50 )
				continue;

			vPts.push_back(gp);
			
			//get color
			Point3DDouble color;
			int x = lptImagePts[i].p[0];
			int y = lImage->height - lptImagePts[i].p[1];
			int scanWd = lImage->widthStep;
			int r = (unsigned char)(lImage->imageData[ y*scanWd + x*3 ]);
			int g = (unsigned char)(lImage->imageData[ y*scanWd + x*3 + 1 ]);
			int b = (unsigned char)(lImage->imageData[ y*scanWd + x*3 + 2 ]);

			color.p[0] = r;
			color.p[1] = g;
			color.p[2] = b;
			vColors.push_back(color);
		}
	}

	CModelFileBase* pModel = new CPlyModel();
	pModel->Save("sphericalModel.ply", vPts, vColors);


	//projection from panorama to plane 
	int nAngleStep = 30;

	
	return 0;
}