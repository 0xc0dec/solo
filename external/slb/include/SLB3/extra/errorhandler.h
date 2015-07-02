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
#ifndef __SLB3_EXTRA_ERROR__
#define __SLB3_EXTRA_ERROR__

#include "../config.h"

namespace SLB3 {
namespace extra {

  class ErrorHandler {
  public:
    enum ErrorType {
      kRuntime_Error,
      kSyntax_Error,
      kOutOfMemory_Error,
      kFileNotFound_Error,
    };

    // performs the lua_pcall using this errorHandler
    int call(lua_State *L, int nargs, int nresults);

    // Returns the last formated error as string.
    virtual const char *last_error() const {
      assert(!"This method must be reimplemented!!");
      return NULL;
    };

    // You can call this method to register an error before any code is
    // executed, for example due to syntax errors, out of memory errors,
    // file not found, and so on. Normally you won't call this method directlly.
    void internal_launchError(const ErrorType type, const char *msg);

    // In case of error, you can manually call to this method before the
    // stack is unwinded (only on lua error handlers, and lua_resume). It
    // Expects a string at the top with the current error.
    // Normally you won't this method directlly.
    void internal_processLuaStackAfterError(lua_State *L);

  protected:
    ErrorHandler() {}
    virtual ~ErrorHandler() {}

    /// first function to be called with the main error.
    virtual void begin(const ErrorType /*type*/, const char* /*error*/) {}

    /// Last function that will be called at the end, once
    /// the error stack has been processed.
    virtual void end() {};

    /// Called on each stack element, here you can call SE_*
    /// functions. 
    virtual void stackElement(int /*level*/) {}

    /// StackElement function
    /// returns the possible name of the function (can be null)
    const char *SE_name() const;

    /// StackElement function
    /// returns the "global", "local", "method", "field", "upvalue", or ""
    const char *SE_nameWhat() const;

    /// StackElement function
    /// returns "Lua", "C", "main", "tail"
    const char *SE_what() const;

    /// StackElement function
    /// If the function was defined in a string, returns that string. If the
    /// function was defined in a file the source starts with '@' followed by
    /// the name of the file.
    const char *SE_source() const;

    /// StackElement function
    /// shortened version of source
    const char *SE_shortSource() const; 

    /// StackElement function
    /// number of line, or -1 if there is no info available
    int SE_currentLine() const;

    /// StackElement function
    /// number of upvalues of the function
    int SE_numberOfUpvalues() const;

    /// StackElement function
    /// the line number where the definition of the funciton starts
    int SE_lineDefined() const;

    /// StackElement function
    /// the last line where the definition of the function ends
    int SE_lastLineDefined() const;

    /// StackElement function
    /// returns if the current stack element is a tail call
    bool SE_isTailCall() const;

  private:
    ErrorHandler(const ErrorHandler&);
    ErrorHandler& operator=(const ErrorHandler&);
    lua_Debug debug_;
    lua_State *lua_state_;
    static int InternalStackHandler(lua_State*);
  };

  // A basic implementation of a complete and functional error handler that
  // uses the given buffer to store the error (in case there is an error).
  // Probably you would rather use DefaultErrorHandler instead of this class.
  class BufferErrorHandler : public ErrorHandler {
  public:
    BufferErrorHandler(char *buffer, size_t buffer_size);
    virtual ~BufferErrorHandler() {}
    virtual const char *last_error() const { return (buffer_pos_)?buffer_:"";}
  protected:
    virtual void begin(const ErrorType type, const char *error);
    virtual void end();
    virtual void stackElement(int level);
    BufferErrorHandler& append(const char *str);
    BufferErrorHandler& append(int);
  private:
    char * const buffer_;
    size_t const buffer_size_;
    size_t buffer_pos_;
  };

  // DefaultErrorHandler stores a 1k buffer statically to handle possible errors
  // It can be created at the stack before calling a lua method.
  class DefaultErrorHandler : public BufferErrorHandler {
  public:
    DefaultErrorHandler() : BufferErrorHandler(buffer_, 1024) {}
    virtual ~DefaultErrorHandler() {}
  private:
    char buffer_[1024];
  };

} // extra
} // SLB

#endif // __SLB3_EXTRA_ERROR__
