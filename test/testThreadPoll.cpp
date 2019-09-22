#include <thread>
#include <iostream>
#include "../ThreadPoll.h"
#include <unistd.h>

using namespace std;

static int id;

void fun() {
    cout << "test" << id << endl;
}

void addfun(ThreadPoll &poll) {
    while(1) {
        sleep(1);
        poll.addTask(bind(&fun));
    }
}

int main(void) {
    ThreadPoll poll(10);
    poll.addTask(bind(&addfun, poll));
    poll.start();
}