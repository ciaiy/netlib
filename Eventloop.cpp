#include "Eventloop.h"

using std::cout;
using std::endl;
using std::lock_guard;

Eventloop::Eventloop()
    :   looping_(false),
        eventHandling(false),
        quit_(false),
        doingPendingFunctor(false),
        poller_(Poller(this))
{
    cout << "eventloop create" << endl;
}

Eventloop::~Eventloop() {}

Eventloop::loop()
{
    looping_ = true;
    quit_ = false;

    while (quit_) {
        // poll
        activeChannels_.clear();  
        Poller.poll(&activeChannel);

        // event hand
        eventHandling = true;
        for(Channel *channel : activeChannels_) {
            channel->handleEvent(); 
        }
        eventHandling = false;

        // do pending functor
        doingPendingFunctor = true;
        dopendingFunctor();
        doingPendingFunctor = false;
    }
}

Eventloop::quit() {
    quit = false;
}

Eventloop::addInPendingFunctors(Functor cb) {
    lock_guard lock;
    pendingFunctors_.push_back(std::move(cb));
}

Eventloop::dopendingFunctor() {
    std::vector<Functor> functors;
    
    {
        lock_guard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(const Functor& functor : functors) {
        functor();
    }
}

Eventloop::updateChannel(Channel *channel) {
    poller_.updateChannel(channel);
}

Eventloop::removeChannel(Channel *channel) {
    poller_.removeChannel(channel);
}