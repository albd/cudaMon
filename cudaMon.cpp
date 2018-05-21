#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <mutex>
#include <sys/types.h>
#include <signal.h>
#include "cudaMon.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <chrono>
#include <thread>

#define READ 0
#define WRITE 1

using namespace std;



class task {
    private:
    int childpipe[2];
    pthread_t monitor;
    int partition;
    string path;
    time_t lastHalve = 0;
    time_t lastInc = 0;
    time_t firstLaunch = 0;
    bool halvePending = false;
    friend void* childMonitor(void *);
    public:
    int pid;
    int priority;
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
            pthread_create(&monitor, NULL, childMonitor, this);
            time(&firstLaunch);
        } else { //child process
            close(childpipe[READ]);
            dup2(childpipe[WRITE], 2);
            setenv("CUDA_MPS_ACTIVE_THREAD_PERCENTAGE", "100", 1);
            cout<<"launching"<<path<<endl;
            execve(path.c_str(), NULL, environ);
        }
    }
    void halve()
    {
	//if ((time(NULL) - firstLaunch) < 5) {
	//	return;
	//}
	if ((time(NULL) - lastHalve) < 1) {
		//halvePending = true;
		return;
	}
        cout<<"halving"<<endl;
        partition = max(0, partition/2);
	time(&lastHalve);
        relaunch();
	halvePending = false;
	print();
    }
    void inc()
    {
	if ((time(NULL) - lastInc) < 1)
		return;
	if (halvePending) {
		halve();
		return;
	}
	cout<<"incing"<<endl;
        partition = min(100, partition+1);
	time(&lastInc);
        relaunch();
	print();
    }
    void relaunch()
    {
	int status;
        cout<<"sent kill to process "<<pid<<endl;
        kill(pid, 2);
	waitpid(pid, &status, 0);
        cout<<pid<<" killed"<<endl;

        if (pid = fork()) { //parent
		this_thread::sleep_for(chrono::milliseconds(50));
        } else { //child process
            close(childpipe[READ]);
            dup2(childpipe[WRITE], 2);
            setenv("CUDA_MPS_ACTIVE_THREAD_PERCENTAGE", to_string(partition).c_str(), 1);
            cout<<"launching"<<path<<endl;
            execve(path.c_str(), NULL, environ);
        }
    }
    void print()
    {
        cout<<time(NULL)<<'\t'<<priority<<"\t\t"<<pid<<'\t'<<partition<<'\t'<<path<<endl;
    }
    bool operator<(const task& other) const
    {
        return priority < other.priority;
    }
};

class reservation {
    private:
    vector<task *> tasks;
    mutex lock;

    public:
    void insert(task *newTask)
    {
        lock_guard<mutex> guard(lock);
        tasks.push_back(newTask);
        //sort(tasks.begin(), tasks.end());
    }
    void print()
    {
	cout<<"time\t\tpriority\tpid\tpartition\tpath"<<endl;
        for(auto task: tasks) {
            task->print();
        }
    }
    void missed(int pid)
    {
        //cout<<"looking for"<<pid<<endl;
        lock_guard<mutex> guard(lock);
        bool found = false;
        int i=0;
        for(auto task: tasks) {
            //cout<<i++;
            //task->print();
            if (!found) {
                if(task->pid == pid) {
                    //cout<<"found"<<endl;
                    found = true;
                }
            } else if (task->priority > 3) {
                //cout<<"halving"<<endl;
                task->halve();
            }
        }
	//print();

    }
    void inc(int pid)
    {
        //cout<<"looking for"<<pid<<endl;
        lock_guard<mutex> guard(lock);
        bool found = false;
        int i=0;
        for(auto task: tasks) {
            //cout<<i++;
            //task->print();
            if (!found) {
                if(task->pid == pid) {
                    //cout<<"found"<<endl;
                    found = true;
                }
            } else if (task->priority > 3) {
                //cout<<"incing"<<endl;
                task->inc();
            }
        }
	//print();

    }

} reserv;

void* childMonitor(void *arg)
{
    //cout<<"launched thread"<<endl;
    task *thisTask = (task *)arg;
    int fd = thisTask->childpipe[READ];
    //cout<<"got fd"<<fd<<endl;
    FILE *fp = fdopen(fd, "r");
    char buffer[1024];
    while(1) {
        //cout<<"before wait"<<endl;
	fseek(fp, 0, SEEK_END);
        fgets(buffer, 1024, fp);
        //cout<<buffer<<endl;
        //cout<<"monitor"<<thisTask<<" "<<thisTask->pid<<"sleeping"<<endl;
        //sleep(10);
        //cout<<"monitor"<<thisTask<<" "<<thisTask->pid<<"awoke"<<endl;
	if(!strncmp(buffer, "miss", 4))
		reserv.missed(thisTask->pid);
	else if(!strncmp(buffer, "pass", 4))
		reserv.inc(thisTask->pid);
    }
    return NULL;
}

void help() {
    cout<<"launch priority path \t launch a task"<<endl;
    cout<<"exit \t\t\t exit cudaMon"<<endl;
    cout<<"ls \t\t\t list all tasks launched"<<endl;
    cout<<"help \t\t\t display this help text"<<endl;
}

int main()
{
    while(1) {
        string com, path;
        int priority, pid;
        cout<<">> ";
        cin>>com;
        if (com == "exit") {
            return 0;
        } else if (com == "help") {
            help();
        } else if (com == "ls") {
            reserv.print();
        } else if (com == "launch") {
            cin>>priority>>path;
            task *newTask = new task(priority, path);
            reserv.insert(newTask);
        }

    }
}

