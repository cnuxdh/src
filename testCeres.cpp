//
//  testOperatorOverride.cpp
//  
//
//  Created by 谢东海 on 16/7/31.
//
//

#include <stdio.h>


#include "ceres/ceres.h"
#include "ceres/rotation.h"



int main()
{
    
    printf(" Ceres functions.... \n");
    
    double p[3];
    ceres::AngleAxisRotatePoint(camera, point, p);
    
    
    
    return 0;
}