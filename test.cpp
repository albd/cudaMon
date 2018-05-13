#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    cout<<"CUDA_MPS_ACTIVE_THREAD_PERCENTAGE="<<getenv("CUDA_MPS_ACTIVE_THREAD_PERCENTAGE")<<endl;
    while(1) {
        //printf("hello world\n");
        //cout<<"hello world"<<endl;
        sleep(1);
        if(atoi(getenv("CUDA_MPS_ACTIVE_THREAD_PERCENTAGE")) > 30)
            cerr<<"missed"<<endl;

    }
    return 0;
}
