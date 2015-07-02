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
#ifndef __SLB3_EXTRA_SCRIPT__
#define __SLB3_EXTRA_SCRIPT__

#include "../config.h" 

namespace SLB3  {
namespace extra {

  class ErrorHandler; // #include <SLB3/extra/errorhandler.h> 
  
  class Script {  
  public:
    // Creates a non-initialized luaVM manager, call Init or InitWithAllocator
    // before using the script
    Script();

    ~Script();

    // Initializes the internal lua_State with luaL_newstat
    void init();

    // Initializes the internal lua_State with the given allocator
    void initWithAllocator(lua_Alloc func, void *alloc_user_data);

    // closes the lua_State
    void finish();

    // closes the current state, and re-initializes the luaVM
    void reset();

    // Returns the current lua_state
    lua_State* lua_state() { return lua_state_; }

    // Script can be used with any function that requires a valid lua_state
    operator lua_State*() { return lua_state_; }

    // Changes if the luaVM loads the default lua libraries when the the state
    // is initialized. Changes only take effect on next call of init after
    // calling this method.
    void set_load_default_libs(bool b) { load_default_libs_ = b; }
    bool load_default_libs() const { return load_default_libs_; }

    // Executes the content of the given file, returns true if the call
    // was successful, false otherwise. If error_handler is null the default
    // error handler is used.
    bool doFile(const char* filename, ErrorHandler* error_handler = NULL);

    // Executes the given code chunk and returns true if successful, false
    // otherwise. If error_handler is null the default error handler is used
    bool doString(
      const char *codeChunk,
      const char *where_hint ="[SLB3]",
      ErrorHandler *error_handler = NULL);

    /* ***************************** WARNING ******************************* *
     * Sometines you need to manually call Garbage Collector(GC), to be sure *
     * that all objects are destroyed.                                       *
     * GC operations are really expensive, avoid calling GC too frequently.  *
     * ***************************** WARNING ******************************* */
    void callGC();

    // Returns the number of Kb of memory used by the script
    size_t memUsage();

    typedef void (*PrintCallback)(
        Script *,     // Script that produced the print call
        const char *, // string to print
        size_t        // size of the string to print
    );

    // Changes the print callback. This callback will be called by the internal
    // print function from the scripts in order generate output.
    void set_print_callback( PrintCallback pc ) { print_callback_ = pc; }
    PrintCallback print_callback() const { return print_callback_; }

    enum Event {
      kEvent_Init,
      kEvent_Finish,
      kEvent_Error,
    };

    typedef void (*EventCallback)(
        Script *,     // Script that produced the event
        const Event,  // type of event
        const ErrorHandler*  // if event == Error,the ErrorHandler used
        );

    // Changes the event callback. This callback will be called each time the
    // Script performs one of this actions:
    //  * kEvent_Init   --> called after the luaVM has ben initialized
    //  * kEvent_Finish --> called before the luaVM is closed
    //  * kEvent_Error  --> called after the script execution ended as error
    // If you want to catch initialization of the script the first time, set
    // the event callback before calling init();
    void set_event_callback( EventCallback ev) { event_callback_ = ev; }
    EventCallback event_callback() const { return event_callback_; }

  private:
    Script(const Script &s);
    Script& operator=(const Script&);

    static int PrintHook(lua_State *);
    void doInitialization();
    bool doExecution(int top, int load_result, ErrorHandler *err_handler);

    lua_State* lua_state_;
    lua_Alloc  allocator_;
    void*      allocator_ud_;
    bool       load_default_libs_;
    PrintCallback print_callback_;
    EventCallback event_callback_;
  };

}} /* SLB3::extra */

#endif
