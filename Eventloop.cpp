#include "Eventloop.h"

using std::cout;
using std::endl;
using std::lock_guard;

Eventloop::Eventloop()
    :   looping_(false),
        eventHandling(false),
        quit_(false),
        doingPendingFunctor(false),
        poller_(new Poller(this))
{
/*debug*/
 poller_->getchannels()[123] = nullptr;
    cout << "eventloop create" << endl;
}

Eventloop::~Eventloop() {}

void Eventloop::loop()
{
    looping_ = true;
    quit_ = true;

    while (quit_) {
        // poll
        activeChannels_.clear();  
        poller_->poll(&activeChannels_);

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

void Eventloop::quit() {
    quit_ = false;
}

void Eventloop::addInPendingFunctors(Functor cb) {
    lock_guard<std::mutex> lock(mutex_);
    pendingFunctors_.push_back(std::move(cb));
}

void Eventloop::dopendingFunctor() {
    std::vector<Functor> functors;
    
    {
        lock_guard<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(const Functor& functor : functors) {
        functor();
    }
}

void Eventloop::updateChannel(Channel *channel) {
/* debug */
cout<<"loop update..." <<endl;
    poller_->updateChannel(channel);
/* debug */
    cout<<"has updated " << endl;
}

void Eventloop::removeChannel(Channel *channel) {
    poller_->removeChannel(channel);
}

bool Eventloop::isLooping() {
    return looping_;
}