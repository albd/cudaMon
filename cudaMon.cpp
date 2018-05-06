#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <mutex>

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
    pthread_t monitor;
    int priority;
    int partition;
    string path;
    public:
    int pid;
    task(int pr, string pa) {
        pipe(childpipe);
        partition = 100;
        priority = pr;
        path = pa;
        launch();
    }
    void launch()
    {
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
    void halve()
    {
        partition = partition/2;
        relaunch();
    }
    void inc()
    {
        partition = min(100, partition+1);
        relaunch();
    }
    void relaunch()
    {

    }
    void print()
    {
        cout<<priority<<'\t'<<pid<<'\t'<<path<<'\t'<<partition<<endl;
    }
    bool operator<(const task& other) const
    {
        return priority < other.priority;
    }
};

class reservation {
    private:
    vector<task> tasks;
    mutex lock;

    public:
    void insert(task newTask)
    {
        lock_guard<mutex> guard(lock);
        tasks.push_back(newTask);
        sort(tasks.begin(), tasks.end());
    }
    void print()
    {
        for(auto task: tasks) {
            task.print();
        }
    }
    void update(int pid)
    {
        lock_guard<mutex> guard(lock);
        bool found = false;
        for(auto task: tasks) {
            if (!found) {
                if(task.pid == pid) {
                    found = true;
                }
            } else {
                task.halve();
            }
            task.print();
        }

    }

} reserv;

int main()
{
    while(1) {
        string com, path;
        int priority, pid;
        cin>>com;
        if (com == "exit") {
            return 0;
        } else if (com == "ls") {
            reserv.print();
        } else if (com == "launch") {
            cin>>priority>>path;
            task newTask(priority, path);
            reserv.insert(newTask);
        }

    }
}

