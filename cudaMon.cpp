#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>

#define READ 0
#define WRITE 1

using namespace std;


void* childMonitor(void *arg)
{
    cout<<"launched thread"<<endl;
    int fd = *(int *)arg;
    cout<<"got fd"<<fd<<endl;
    FILE *fp = fdopen(fd, "r");
    char buffer[1024];
    while(1) {
        cout<<"before wait"<<endl;
        fgets(buffer, 1024, fp);
        cout<<buffer<<endl;
    }
    return NULL;
}

class task {
    private:
    int childpipe[2];
    int pid;
    pthread_t monitor;
    int priority;
    int partition;
    string path;
    public:
    task(int pr, string pa) {
        pipe(childpipe);
        partition = 100;
        priority = pr;
        path = pa;
        if (pid = fork()) { //parent
            pthread_create(&monitor, NULL, childMonitor, &(childpipe[READ]));
        } else { //child process
            close(childpipe[READ]);
            dup2(childpipe[WRITE], WRITE);
            setenv("TESTENV", "parental control", 1);
            //execve("../cudaKernel/cuda_kernel", NULL, environ);
            cout<<"launching"<<path<<endl;
            execve(path.c_str(), NULL, environ);
        }
    }
};

//class reservation {
//    private:
//    vector<task> tasks;
//    public:
//    void insert(task newTask) {
//
//    }
//
//}

int main()
{
    while(1) {
        vector<task> reservation;
        string command, path;
        int priority, pid;
        cin>>command;
        if (command == "exit")
            return 0;
        cin>>priority>>path;
        task newTask(priority, path);
    }
}

