
#include"stdio.h"
#include"stdlib.h"
#include"string.h"


//cvlib
#include"cvInvoke.hpp"

//corelib
#include"commonfile.h"
#include"commonfuncs.h"


 
//executing program 
int main(int argc, char* argv[])
{
	printf("feature detecting....  \n");
	
	char imageListFile[512];
	char outpath[512];
	int  dstHt; 
	
	
	//get the params
	for(int i=1; i<argc; i++)
	{
		//printf("%s \n", argv[i]);
		if( strcmp( argv[i], "-list" ) == 0 ) //left image
		{
			strcpy(imageListFile, argv[i+1]);
			printf("%s \n", imageListFile);
			i++;
		}
		else if( strcmp( argv[i], "-outpath" ) == 0 ) //left image
		{
			strcpy(outpath, argv[i+1]);
			printf("%s \n", outpath);
			i++;
		}
		else if( strcmp( argv[i], "-height" ) == 0 ) //image height to be resized
		{
			dstHt = atoi(argv[i+1]);
			printf("%d \n", dstHt);
			i++;
		}
	}
	
	
	//reading image files
	int nFileRows = GetFileRows(imageListFile);
	printf("%d \n", nFileRows);
	char** filenames = NULL;
	int n=0,nfile=0;
	filenames = f2c(nFileRows, 512);
	
	FILE* fp = fopen(imageListFile, "r");
	for(int i=0; i<nFileRows; i++)
	{
		fscanf(fp,"%s", filenames[i]);
		
	}
	fclose(fp);
	
	
	//invoke the core function
	DetectFileFeaturePts(filenames, nFileRows, outpath);
	
	
	
	
	return 0;
}





