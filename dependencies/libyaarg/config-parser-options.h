// Copyright (c) 2008,2009,2010,2011 Carlo Contavalli (ccontavalli@gmail.com).
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY Carlo Contavalli ''AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL Carlo Contavalli OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are
// those of the authors and should not be interpreted as representing official
// policies, either expressed or implied, of Carlo Contavalli.

#ifndef LIBYAARG_CONFIG_PARSER_OPTIONS_H
# define LIBYAARG_CONFIG_PARSER_OPTIONS_H

# include "config-parser.h"
# include <vector>
# include <sstream>

class CallbackCommand : public Command {
 public:
  typedef function<void ()> callback_t;
  CallbackCommand(
      CommandHolder* holder, int flags, const char* name,
      const char* description, callback_t callback)
      : Command(holder, flags, name, description),
        callback_(callback) {}

  void Run() { callback_(); }

 private:
  callback_t callback_;
};

class ActionOption : public Option {
 public:
  ActionOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    const char* description);

  const char* GetType() const { return NULL; }
  bool GetAsString(string* /* value */) const { return false; }
  bool HasBeenSet() const { return has_been_set_; }
  bool HasDefaultValue() const { return false; } 

 protected:
  bool has_been_set_;
  CommandHolder* holder_;
};

class DumpOption : public ActionOption {
 public:
  DumpOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname);

  virtual bool Parse(
      const char* value, deque<const char*>* argv,
      ostream* error, ostream* output);
};

class HelpOption : public ActionOption {
 public:
  HelpOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname);

  virtual bool Parse(
      const char* value, deque<const char*>* argv,
      ostream* error, ostream* output);
};

template<typename TYPE>
class TypedOption : public Option {
 public:
  TypedOption(CommandHolder* holder, int flags, const char* lname, const char* sname,
	      const char* expected, const TYPE& fallback, const char* description)
      : Option(holder, flags, lname, sname, description),
        has_been_set_(false),
	has_default_value_(false),
	expected_value_(expected),
        fallback_(fallback),
        value_(fallback) {
  }

  const TYPE& Get() const { return value_; }
  void Set(const TYPE& value) {
    if (value_ == fallback_)
      has_default_value_ = true;
    else
      has_default_value_ = false;

    has_been_set_ = true;
    value_ = value;
  }

  bool HasBeenSet() const { return has_been_set_; }
  bool HasDefaultValue() const { return has_default_value_; } 

  virtual const char* GetType() const { return expected_value_; }

 private:
  bool has_been_set_;
  bool has_default_value_;

  const char* expected_value_;
  TYPE fallback_;
  TYPE value_;
};

class StringOption : public TypedOption<string> {
 public:
  StringOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    const string& fallback, const char* description)
    : TypedOption<string>(
        holder, flags, lname, sname, "string", fallback, description) {}
  
  bool GetAsString(string* value) const;
  bool Parse(const char* value, deque<const char*>* argv,
             ostream* error, ostream* output);
};

class OneOfOption : public TypedOption<int> {
 public:
  OneOfOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    const vector<const char*>& possible, int def, const char* description);
  
  bool GetAsString(string* value) const;
  bool Parse(const char* value, deque<const char*>* argv,
             ostream* error, ostream* output);

  virtual const char* GetType() const;

 private:
  // Marked as mutable as it's basically a cache.
  mutable string value_;
  const vector<const char*> possible_;
};

class BoolOption : public TypedOption<bool> {
 public:
  BoolOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    bool fallback, const char* description)
    : TypedOption<bool>(
        holder, flags, lname, sname, "true or false", fallback, description) {}
 
  bool GetAsString(string* value) const;
  bool Parse(const char* value, deque<const char*>* argv,
             ostream* error, ostream* output);
};

template<typename IntTypeT>
class IntOption : public TypedOption<IntTypeT> {
 public:
  IntOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    IntTypeT fallback, const char* description)
    : TypedOption<IntTypeT>(
        holder, flags, lname, sname, "integer", fallback, description) {}
 
  bool GetAsString(string* value) const {
    if (!value)
      return true;
  
    if (this->Get())
      value->assign("true");
    else
      value->assign("false");
  
    return true;
  }

  bool Parse(const char* value, deque<const char*>* argv,
             ostream* error, ostream* output) {
    if (!value && argv->empty()) {
      (*error) << "option requires " << this->GetType() << " as a value";
      return false;
    }
   
    if (!value) {
      value = (*argv)[0];
      argv->pop_front();
    }
  
    IntTypeT number;
    if (istringstream(value) >> number)
      return true;
  
    // If we get here, it means that we didn't find a valid value.
    (*error) << "\"" << value << "\" is invalid, must be one of " << this->GetType();
    return false;
  }
};

class StandardOptions {
 public:
  StandardOptions(CommandHolder* holder)
      : dump_(holder, Option::Default, "dump-configs", NULL),
        help_(holder, Option::Default, "help", NULL) {
  }

 private:
  DumpOption dump_;
  HelpOption help_;
};

#endif /* LIBYAARG_CONFIG_PARSER_OPTIONS_H */
