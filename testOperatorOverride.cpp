//
//  testOperatorOverride.cpp
//  
//
//  Created by 谢东海 on 16/7/31.
//
//

#include <stdio.h>


struct CostFunctor {
    template <typename T>
    bool operator()(const T* const x, T* residual) const {
        residual[0] = T(10.0) - x[0];
        return true;
    }
};


int main()
{
    
    printf("Operator Override.... \n");
    
    CostFunctor *cf = new CostFunctor();
    
    double x[1] = {1};
    double residual[1]={0};
    (*cf)(x,residual);
    
    printf("residual: %lf \n", residual[0]);
    
    return 0;
}