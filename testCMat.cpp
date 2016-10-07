
#include"stdio.h"


#include"dataBase.hpp"

#include<iostream>

using namespace std;


class example
{
public:
    int a;
    int b;
    example& operator+(const example& obj);
    example& operator=(const example& obj);
};

example& example::operator=(const example& obj)
{
		printf("operator= .... \n");
		
    (*this).a = obj.b;
    (*this).b = obj.a;


    return *this;
}

example& example::operator+(const example& obj2)
{
	  printf("operator+ .... \n");
	  
    example tmp_obj = *this;
    tmp_obj.a = tmp_obj.a + obj2.a;
    tmp_obj.b = tmp_obj.b + obj2.b;
    return tmp_obj;
}

int fun1(CMat& srcImage)
{
	
	CMat tm;
	tm = srcImage;
	
	CMat tm1;
	tm1 = tm;
	
	
	printf("fun1... \n");
	
	return 0;
}



int main(int argc, char* argv[])
{

  
	printf("test CMat ... \n");
	
	char* filepath = "/Users/xiedonghai/Programs/images/lena.jpg";
	
	
	CMat image = ImageRead(filepath, 0);
	
	
	//printf("ht: %d   wd: %d \n", image.GetRows(), image.GetCols());
	
	
	//blow is impossible ???
	//CMat dst = image;
	
	//CMat dst;
	//dst = image;
	
	fun1(image);
	
	ImageWrite("out.jpg", image );

	
	/*
	example a;
	a.a = 1;
	a.b = 2;
	
	example b = a;
	
	example c ;
	c = a;
	
	std::cout<<c.a<<"  "<<c.b<<"\n";
	*/
	 	
	cout<<"finished!"<<endl;
	
	
	return 0;
}