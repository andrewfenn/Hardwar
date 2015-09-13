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

#include "config-parser-options.h"

#include <string.h>

ActionOption::ActionOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    const char* description)
    : Option(holder, flags, lname, sname, description),
      has_been_set_(false),
      holder_(holder) {
}

HelpOption::HelpOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname)
    : ActionOption(holder, flags, lname, sname, "Shows this help screen.") {
}

bool HelpOption::Parse(
    const char* value, deque<const char*>* argv,
    ostream* error, ostream* output) {
  if (value) {
    (*error) << "option requires no arguments";
    return false;
  }

  has_been_set_ = true;
  holder_->GetParser()->PrintHelp(output);
  return false;
}

DumpOption::DumpOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname)
    : ActionOption(holder, flags, lname, sname,
	     "Dumps all the options that have been provided to this binary.") {
}

bool DumpOption::Parse(
    const char* value, deque<const char*>* argv,
    ostream* error, ostream* output) {
  if (value) {
    (*error) << "option requires no arguments";
    return false;
  }

  has_been_set_ = true;
  // FIXME: flags are always set to 0, which seems wrong.
  holder_->GetParser()->DumpConfigs(0, output);
  return true;
}

bool StringOption::GetAsString(string* value) const {
  if (value)
    value->assign(Get());
  return true;
}

bool StringOption::Parse(
    const char* value, deque<const char*>* argv,
    ostream* error, ostream* output) {
  if (!value && argv->empty()) {
    (*error) << "option requires a string as a value";
    return false;
  }

  if (!value) {
    value = (*argv)[0];
    argv->pop_front();
  }

  Set(value);
  return true;
}

OneOfOption::OneOfOption(
    CommandHolder* holder, int flags, const char* lname, const char* sname,
    const vector<const char*>& possible, int def, const char* description)
    : TypedOption<int>(holder, flags, lname, sname, "enum", def, description),
      possible_(possible) {
}

bool OneOfOption::GetAsString(string* value) const {
  if (value)
    value->assign(possible_[Get()]);

  return true;
}

bool OneOfOption::Parse(const char* value, deque<const char*>* argv,
			ostream* error, ostream* output) {
  if (!value && argv->empty()) {
    (*error) << "option requires one of " << GetType() << " as a value";
    return false;
  }
 
  if (!value) {
    value = (*argv)[0];
    argv->pop_front();
  }

  for (unsigned int i = 0; i < possible_.size(); ++i) {
    if (!strcmp(possible_[i], value)) {
      Set(i);
      return true;
    }
  }

  // If we get here, it means that we didn't find a valid value.
  (*error) << "\"" << value << "\" is invalid, must be one of " << GetType();
  return false;
}

const char* OneOfOption::GetType() const {
  if (value_.empty()) {
    for (unsigned int i = 0; i < possible_.size(); ++i) {
      if (i != 0) value_.append(" or ");
      value_.append("\"");
      value_.append(possible_[i]);
      value_.append("\"");
    }
  }

  return value_.c_str();
}

bool BoolOption::GetAsString(string* value) const {
  if (!value)
    return true;

  if (Get())
    value->assign("true");
  else
    value->assign("false");

  return true;
}

bool BoolOption::Parse(const char* value, deque<const char*>* argv,
			ostream* error, ostream* output) {
  if (!value && argv->empty()) {
    (*error) << "option requires " << GetType() << " as a value";
    return false;
  }
 
  if (!value) {
    value = (*argv)[0];
    argv->pop_front();
  }

  if (!strcasecmp(value, "true")) {
    Set(true);
    return true;
  }

  if (!strcasecmp(value, "false")) {
    Set(false);
    return true;
  }

  // If we get here, it means that we didn't find a valid value.
  (*error) << "\"" << value << "\" is invalid, must be one of " << GetType();
  return false;
}
