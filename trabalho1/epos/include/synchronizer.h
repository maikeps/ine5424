// EPOS Synchronizer Abstractions Common Package

#ifndef __synchronizer_h
#define __synchronizer_h

#include <cpu.h>
#include <thread.h>
#include <utility/queue.h>

#include <utility/ostream.h>

__BEGIN_SYS

class Synchronizer_Common
{
protected:
    Synchronizer_Common() {}

    Ordered_Queue<Thread *, Thread::Priority, List_Elements::Doubly_Linked_Ordered<Thread *, Thread::Priority> > suspended_threads;

    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }

    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    void sleep() { 

        cout << "sadsad: Lantes";
        Thread * thread = Thread::self();
        thread->suspend();
        unsigned int priority = thread->priority();
        List_Elements::Doubly_Linked_Ordered<Thread *, Thread::Priority> dll = List_Elements::Doubly_Linked_Ordered<Thread *, Thread::Priority>(&thread, priority); 
        suspended_threads.insert(&dll);

        cout << "sadsad: L";

        end_atomic();
    } // implicit unlock()
    void wakeup() { 
        Thread* thread = *(suspended_threads.remove()->object());
        thread->resume();
        end_atomic(); 
    }
    void wakeup_all() { 
        while(suspended_threads.size() > 0) {
            Thread* thread = *(suspended_threads.remove()->object());
            thread->resume();
        }
        end_atomic(); 
    }
};

__END_SYS

#endif

