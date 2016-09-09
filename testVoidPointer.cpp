
#include"stdio.h"
#include"stdlib.h"

int main()
{
   
	printf("test void pointer , written by xiedonghai, 2016.7.15 .... \n");
	
	void* pt = NULL;
	
	int* parray = (int*)malloc(10*sizeof(int));
	
	pt = (int*)(parray);
	
	for(int i=0; i<10; i++)
	{
		printf("%d %d \n", parray[i], *(pt+i) );
	}
	
	return 0;
}