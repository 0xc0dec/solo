// Copyright (c) 2013, Jose L. Hidalgo Valiño <pplux@pplux.com>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __SLB3_EXTRA_COROUTINEMANAGER__
#define __SLB3_EXTRA_COROUTINEMANAGER__

#include "../config.h" 
#include "../value.h" 

namespace SLB3  {
namespace extra {

  class CoroutineManager {  
  public:
    // Registers async() and waitFor() methods in the lua_State.
    // - async(function [, arguments]) :
    //     Creates a new indpendent coroutine that will be started with @arguments
    // - waitFor(args):
    //     Holds the coroutine and calls the internal implementation of waitfor
    // async and waitFor names can be changed by overriding init_implementation
    void init(lua_State *s);

    // Aborts all coroutines and removes waitFor and async methods from main
    // lua_State.
    void finish();

    // Tries to execute active coroutines, if any.
    void update();

    // Returns the current lua_state
    lua_State* lua_state() { return lua_state_; }

    // Total number of coroutines suspended or active
    unsigned total_coroutines()  const;

    // Total number of coroutines ready to be executed
    unsigned active_coroutines() const;

    // A coroutine thread is just a lua_State* that lives inside another
    // lua_State (the main thread).
    typedef lua_State* Thread;

  protected:
    // you must subclass CoroutineManager in order to implement its waitfor
    // function (which is a pure virtual function)
    CoroutineManager();
    ~CoroutineManager();

    // -- Initialization of the manager ------------------------------------

    // Method called by init once the lua_state is set. You can override this
    // to define your own waitfor and async functions. 
    virtual void init_implementation() {
      lua_State *L = lua_state();
      pushLua_waitFor(L, 0);
      lua_setglobal(L, "waitFor");
      pushLua_async(L);
      lua_setglobal(L, "async");
    }

    // Called by finish, before aborting pending coroutines.
    // use this if you wan't to remove registered functions
    virtual void finish_implementation() {}

    // Pushes a "waitfor" function to the given stack, id is used to
    // identify the waitfor called, thus you can register several waitFor
    // functions (e.g. waitForTime, waitForSignal, waitForFile...) This
    // method is meant to be called from @init_implemenatation.
    //
    // L* must be related to the main thread (the one used at initialization)
    void pushLua_waitFor(lua_State *L, int id = 0);

    // Pushes the async method to the given stack. Async method will 
    // create a coroutine with the given function and arguments. This method
    // is meant to be called from @init_implementation.
    //
    // L* must be related to the main thread (the one used at initialization)
    void pushLua_async(lua_State *L);

    // -- WaitFor implementation -------------------------------------------
    enum WaitForResult {
      // The execution continues. The waitfor Function returns ALL values from
      // the current stack. You must take care of consuming all input arguments
      // before return, and leve at the stack only the results.
      kW_ContinueExecution,

      // The coroutine is suspendend, and the stack is left untouched. You must
      // take care of the stack before leaving waitFor_implementation
      kW_WaitForSignal,

      // The coroutine is suspended. The next resume will return from the wait
      // with all elements left at the stack after waitFor_implementation
      kW_WaitUntilNextUpdate,

      // The coroutine will never resturn, the stack is wiped to 0, but it
      // doesn't matter. It will be collected sooner or later.
      kW_AbortCoroutine,

      // the pushed string on the top of the stack is taken as error
      kW_Error,
    };

    // Method called once a thread executes a waitfor function, @id arg can be
    // used to identify the waitfor issued by the coroutine. WaitFor can only
    // be called from coroutines.
    // * WARNING *
    //   The stack must be wiped by hand, most of the time elements left in 
    //   the stack will be used as return value when exiting the wait call.
    virtual WaitForResult waitFor_implementation(Thread t_stack, int id) = 0;

    // Pushes some value to the thread stack, once the coroutine is signaled the
    // result of waitFor will receive all pushed values as a result.
    template<class T>
    void pushResultToCoroutineStack(Thread thread, T value) {
      Push<T>(thread, value);
    }

    // -- Update implementation -------------------------------------------

    // * Warning * This method MUST NOT be called directly, it is automatically
    // called from update();
    // 
    // This can be replaced to implement a proper scheduler if needed. It
    // can also be used to filter which active coroutines are updated.
    // By default executes all active coroutines available.
    virtual void update_implementation() {
      while(update_pullNextActiveCoroutine()) {
        update_resumeCoroutine();
      }
    }

    // *Warning* This method must be called only from update_implementation
    //           otherwise the result is undefined;
    // 
    // It returns the next available Thread from the ready-list.
    Thread update_pullNextActiveCoroutine();

    // *Warning* This method must be called only from update_implementation
    //           otherwise the result is undefined;
    //
    // Resumes the current coroutine, this call blocks the caller until the
    // coroutine executes wait, or finishes. 
    void update_resumeCoroutine();

    // -- Coroutine handling ----------------------------------------------

    // Signals a coroutine that was waitingForSignal, to continue its
    // execution on next update. It returns true if the coroutine was indeed in
    // that state and was successfully moved to active.
    bool signalCoroutine(const Thread);

    // Aborts the given coroutine, it will return true if the coroutine was
    // indeed a valid coroutine. False if you called this over a coroutine that
    // no longer existed.
    bool abortCoroutine(const Thread);


    enum Event {
      // A new call to async was made and a new coroutine is created:
      kEvent_CoroutineCreated = 1 << 0,
      // The given coroutine is suspended, waiting for a signal
      kEvent_CoroutineSuspended  = 1 << 1,
      // A coroutine is signaled and will be marked as active
      kEvent_CoroutineSignaled = 1 << 2,
      // A coroutine exists without errors
      kEvent_CoroutineFinishes = 1 << 3,
      // A coroutine is aborted
      kEvent_CoroutineAbort = 1 << 4,
      // Error inside a coroutine
      kEvent_CoroutineError = 1 << 5,
      // Event produced if the lua_State is closed before CoroutineManager
      // calls finish. This means, all Thread can not be reached
      // and will be INVALID. 
      kEvent_PrematureCloseOfLuaState = 1 << 8,
    };
    // No mask
    static const unsigned kEvent_None = 0;
    // All signals
    static const unsigned kEvent_All = 0xFFFF;

    virtual void event_implementation(
        const Event event, // event
        const Thread id,   // Coroutine that produced the event
        const char *error  // Error message if event == kEvent_CoroutineError
        ) {
      // Do nothing on incoming event, by default
      (void)event; (void)id; (void)error;
    }

    // Defines for which events event_implementation will be called
    // by default is kEvent_None
    void set_event_mask(unsigned event_mask);


    // Creates a coroutine with the given function at index. Returns the
    // new created thread, or NULL if the given index is not a function.
    Thread createCoroutine(lua_State *L, int index);

  private:
    CoroutineManager(const CoroutineManager &s);
    CoroutineManager& operator=(const CoroutineManager&);

    void dispatchEvent(Event type, Thread id, const char *error = NULL);
    void notifyAbortAllCoroutines(int table_ref);

    static int lua_async_call(lua_State*);
    static int lua_waitfor_call(lua_State*);
    static int lua_detect_GC(lua_State*);

    struct Data; //< Internal data, stored in the lua_State

    lua_State* lua_state_;
    Data *data_;
    unsigned event_mask_;
    struct {
      int table;
      Thread last_thread;
      int last_thread_status;
    } update_tmp_;
  };

}} /* SLB3::extra */

#endif
