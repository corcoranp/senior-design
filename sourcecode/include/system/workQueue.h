/*
   wqueue.h

   Worker thread queue based on the Standard C++ library list
   template class.

   ------------------------------------------

   Copyright @ 2013 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef __workqueue_h__
#define __workqueue_h__

#include <pthread.h>
#include <list>

using namespace std;

template <typename T> class workqueue
{
    list<T>          m_queue;
    pthread_mutex_t  m_mutex;
    pthread_cond_t   m_condv;

  public:
    workqueue() {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_condv, NULL);
    }
    ~workqueue() {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condv);
    }
    void add(T item) {
        pthread_mutex_lock(&m_mutex);
        m_queue.push_back(item);
        pthread_cond_signal(&m_condv);
        pthread_mutex_unlock(&m_mutex);
    }
    T remove() {
        pthread_mutex_lock(&m_mutex);
        while (m_queue.size() == 0) {
            pthread_cond_wait(&m_condv, &m_mutex);
        }
        T item = m_queue.front();
        m_queue.pop_front();
        pthread_mutex_unlock(&m_mutex);
        return item;
    }
    int size() {
        pthread_mutex_lock(&m_mutex);
        int size = m_queue.size();
        pthread_mutex_unlock(&m_mutex);
        return size;
    }
};

#endif


/**
 * Example Usage
 *
 *
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "thread.h"
#include "wqueue.h"

 //basically the message sent
class WorkItem
{
    string m_message;
    int    m_number;

  public:
    WorkItem(const char* message, int number)
          : m_message(message), m_number(number) {}
    ~WorkItem() {}

    const char* getMessage() { return m_message.c_str(); }
    int getNumber() { return m_number; }
};


//The thread to consume message
class ConsumerThread : public Thread
{
    wqueue<WorkItem*>& m_queue;

  public:
    ConsumerThread(wqueue<WorkItem*>& queue) : m_queue(queue) {}

    void* run() {
        // Remove 1 item at a time and process it. Blocks if no items are
        // available to process.
        for (int i = 0;; i++) {
            printf("thread %lu, loop %d - waiting for item...\n",
                  (long unsigned int)self(), i);
            WorkItem* item = m_queue.remove();
            printf("thread %lu, loop %d - got one item\n",
                  (long unsigned int)self(), i);
            printf("thread %lu, loop %d - item: message - %s, number - %d\n",
                  (long unsigned int)self(), i, item->getMessage(),
                   item->getNumber());
            delete item;
        }
        return NULL;
    }
};



int main(int argc, char** argv)
{
    // Process command line arguments
    if ( argc != 2 ) {
        printf("usage: %s <iterations>\n", argv[0]);
        exit(-1);
    }
    int iterations = atoi(argv[1]);

    // Create the queue and consumer (worker) threads
    wqueue<WorkItem*>  queue;
    ConsumerThread* thread1 = new ConsumerThread(queue);
    ConsumerThread* thread2 = new ConsumerThread(queue);
    thread1->start();
    thread2->start();

    // Add items to the queue
    WorkItem* item;
    for (int i = 0; i < iterations; i++) {
        item = new WorkItem("abc", 123);
        queue.add(item);
        item = new WorkItem("def", 456);
        queue.add(item);
        item = new WorkItem("ghi", 789);
        queue.add(item);
        sleep(1);
    }

    // Wait for the queue to be empty
    while (queue.size() < 0);
    printf("done\n");
    exit(0);
}
 */
