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

#include "config-parser.h"

CommandHolder::CommandHolder(int flags)
    : name_("main"), flags_(flags), found_command_(NULL) {
}

CommandHolder::CommandHolder(const char* name, int flags)
    : name_(name), flags_(flags), found_command_(NULL) {
}

bool CommandHolder::RegisterOptionByLongName(const char* name, Option* option) {
  if (long_options_.insert(make_pair(name, option)).second)
    return true;

  if (!(flags_ & APIAllowsDuplicateOptions)) {
    string error("SOFTWARE BUG: long option '");
    error.append(name);
    error.append("' was already registered, duplicate.");
    AddError(error);
  }

  return false;
}

void CommandHolder::RegisterLeftoverParser(Option* option) {
  leftover_parsers_.push_back(option);
}

bool CommandHolder::RegisterOptionByShortName(const char* name, Option* option) {
  if (short_options_.insert(make_pair(name, option)).second)
    return true;

  if (!(flags_ & APIAllowsDuplicateOptions)) {
    string error("SOFTWARE BUG: short option '");
    error.append(name);
    error.append("' was already registered, duplicate.");
    AddError(error);
  }

  return false;
}

bool CommandHolder::RegisterCommand(const char* name, Command* command) {
  if (commands_.insert(make_pair(name, command)).second)
    return true;

  if (!(flags_ & APIAllowsDuplicateCommands)) {
    string error("SOFTWARE BUG: command '");
    error.append(name);
    error.append("' was already registered, duplicate.");
    AddError(error);
  }

  return false;
}

void CommandHolder::AddMessage(const string& message) {
  GetParser()->AddMessage(message);
}

void CommandHolder::SetFoundCommand(Command* command) {
  found_command_ = command;
}

Command* CommandHolder::GetFoundCommand() const {
  return found_command_;
}

void CommandHolder::RunCommands() {
  Command* command = found_command_;
  while (command) {
    command->Run();
    command = command->GetFoundCommand();
  }
}

void Command::AddError(const string& error) {
  GetParser()->AddError(error);
}

void Command::AddMessage(const string& error) {
  GetParser()->AddMessage(error);
}

ConfigParser::ConfigParser(int flags, const char* description)
    : description_(description) {
}

bool ConfigParser::HasErrors() const {
  bool retval = !errors_.empty();
  return retval;
}

bool ConfigParser::HasMessages() const {
  return !messages_.empty();
}

bool ConfigParser::PrintErrors(ostream* estr) const {
  if (!HasErrors())
    return false;

  for (list<string>::const_iterator it = errors_.begin();
       it != errors_.end(); ++it) {
    (*estr) << *it << '\n';
  }

  return true;
}

bool ConfigParser::GetErrors(string* output) const {
  if (!HasErrors())
    return false;

  for (list<string>::const_iterator it = errors_.begin();
       it != errors_.end(); ++it) {
    output->append(*it);
    output->append("\n");
  }

  return true;
}

void ConfigParser::FlushErrors() {
  errors_.clear();
}

bool ConfigParser::PrintMessages(ostream* estr) const {
  if (!HasMessages())
    return false;

  for (list<string>::const_iterator it = messages_.begin();
       it != messages_.end(); ++it) {
    (*estr) << *it << '\n';
  }

  return true;
}

bool ConfigParser::GetMessages(string* output) const {
  if (!HasMessages())
    return false;

  for (list<string>::const_iterator it = messages_.begin();
       it != messages_.end(); ++it) {
    output->append(*it);
    output->append("\n");
  }

  return true;
}

void ConfigParser::FlushMessages() {
  messages_.clear();
}

void ConfigParser::AddError(const string& error) {
  errors_.push_back(error);
}

void ConfigParser::AddMessage(const string& error) {
  messages_.push_back(error);
}

Option::Option(
    CommandHolder* parser, int flags, const char* lname,
    const char* sname, const char* description)
    : long_name_(lname), short_name_(sname), description_(description) {
  AddTo(parser);
}

void Option::AddTo(CommandHolder* holder) {
  if (!holder)
    return;

  if (long_name_)
    holder->RegisterOptionByLongName(long_name_, this);
  if (short_name_)
    holder->RegisterOptionByShortName(short_name_, this);
  if (!long_name_ && !short_name_)
    holder->RegisterLeftoverParser(this);
}

Command::Command(
    CommandHolder* holder, int flags, const char* name,
    const char* description)
    : CommandHolder(name), holder_(holder), description_(description) {
  if (holder)
    holder->RegisterCommand(name, this);
}
