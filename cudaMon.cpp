#include <stdio.h>
#include <iostream>
#include <unistd.h>

#define READ 0
#define WRITE 1

using namespace std;

int main()
{
    int pid, childpipe[2];
    char buffer[1024];
    pipe(childpipe);
    if (pid = fork()) { //parent
        cout<<"forked child with pid "<<pid<<endl;
        FILE *fp = fdopen(childpipe[READ], "r");
        while(1) {
            fgets(buffer, 1024, fp);
            cout<<buffer<<endl;
        }
    } else { //child process
        close(childpipe[READ]);
        dup2(childpipe[WRITE], WRITE);
        execl("./test.out", NULL, NULL);
    }
}
