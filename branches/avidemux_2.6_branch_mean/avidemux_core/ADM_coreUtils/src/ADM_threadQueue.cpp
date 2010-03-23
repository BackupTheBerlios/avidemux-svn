/***************************************************************************
            \file ADM_threadQueue.cpp
            \brief Create a thread that fills a queue from another part
            \author  (c) 2010 Mean , fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "ADM_default.h"
#include "ADM_threadQueue.h"
#include <math.h>

static void *boomerang(void *x)
{
    ADM_threadQueue *a=(ADM_threadQueue *)x;
    a->run();
}

/**
    \fn ADM_audioAccess_thread
    \brief
*/
ADM_threadQueue::ADM_threadQueue(void)
{
    mutex=new admMutex("audioAccess");
    cond=new admCond(mutex);
    threadState=RunStateIdle;
    started=false;   
    cond=new admCond(mutex);
}
/**
    \fn ~ADM_threadQueue
    \brief
*/

ADM_threadQueue::~ADM_threadQueue()
{
    ADM_info("Killing audio thread and son\n");
    // ask the thread to stop
    
    if(started)
    {
        mutex->lock();
        if(threadState==RunStateRunning)
        {   
            ADM_info("Asking the thread to stop\n");
            threadState=RunStateStopOrder;
            if(cond->iswaiting())
            {
                cond->wakeup();
            }
            mutex->unlock();
            int count=100;
            while(count)
            {
                if(threadState==RunStateStopped) break;
                ADM_usleep(1000*100); // Slep 100 ms
            }
        }else mutex->unlock();
    }
    if(cond) delete cond;
    if(mutex) delete mutex;
    cond=NULL;
    mutex=NULL;
}

/**
    \fn run
    \brief entry point for thread
*/
void ADM_threadQueue::run(void)
{
    
    threadState=RunStateRunning;
    runAction();
    threadState=RunStateStopped;
}
/**
    \fn startThread
*/
bool ADM_threadQueue::startThread(void)
{
      ADM_info("Starting thread...\n");
        if(pthread_create(&myThread,NULL, boomerang, this))
        {
            ADM_error("ERROR CREATING THREAD\n");
            ADM_assert(0);
        }
        while(threadState==RunStateIdle)
        {
            ADM_usleep(10000);
        }
        ADM_info("Thread created and started\n");
        started=true;
        return true;
}

/**
    \fn stopThread
*/
bool ADM_threadQueue::stopThread(void)
{
        ADM_info("Destroying threadQueue\n");
        int clockDown=10;
        mutex->lock();
        if(threadState==RunStateRunning)
        {
            threadState=RunStateStopOrder;
            mutex->unlock();
            while(threadState!=RunStateStopped && clockDown)
            {
                
                ADM_usleep(50*1000);
                clockDown--;
            };
            ADM_info("Thread stopped, continuing dtor\n");
        }else   
        {
                mutex->unlock();
        }
        return true;
}
// EOF
