#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    cout<<getenv("TESTENV")<<endl;
    while(1) {
        printf("hello world\n");
        //cout<<"hello world"<<endl;
        sleep(1);
    }
    return 0;
}
