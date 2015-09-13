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

#ifndef LIBYAARG_CONFIG_PARSER_H
# define LIBYAARG_CONFIG_PARSER_H

# include "base.h"
# include "utils.h"

# include <map>
# include <deque>
# include <iostream>
# include <list>

class Command;
class Option;
class ConfigParser;

class CommandHolder {
 public:
  enum Flags {
    // Nothing special.
    Default = 0,

    // Has to have some options provided.
    RequiresOptions = BIT(0), // NOT IMPLEMENTED
    // Has to have some command provided.
    RequiresCommands = BIT(1), // NOT IMPLEMENTED
    // True if we want to allow the user to specify the same option from
    // the command line / configuration multiple times.
    AllowDuplicateOptions = BIT(2), // NOT IMPLEMENTED

    // Set to true if API allows the same command / option to be registered
    // multiple times.
    APIAllowsDuplicateOptions = BIT(3),
    APIAllowsDuplicateCommands = BIT(4)
  };

  explicit CommandHolder(int flags = Default);
  explicit CommandHolder(const char* name, int flags = Default);

  virtual ~CommandHolder() {}

  const char* GetName() const { return name_; }

  // Return false if an option by that name was already registered.
  bool RegisterOptionByLongName(const char* name, Option* option);
  bool RegisterOptionByShortName(const char* name, Option* option);
  bool RegisterCommand(const char* name, Command* command);
  void RegisterLeftoverParser(Option* option);

  // Users can specify commands to run. Each command holder will eventually
  // have one command. SetFoundCommand and GetFoundCommand allow to access
  // those commands, recursively.
  void SetFoundCommand(Command* command);
  Command* GetFoundCommand() const;

  // Each command can implement a Run method by using inheritance.
  // RunCommands() will invoke those run methods for you.
  void RunCommands();

  virtual ConfigParser* GetParser() = 0;

  typedef map<const char*, Command*, CStringCmpFunctor> CommandMap;
  typedef map<const char*, Option*, CStringCmpFunctor> OptionMap;
  typedef list<Option*> LeftoverParsers;

  // Each config parser has its own way to display options and commands.
  // For example, an argv parser can display options with -- and -, a
  // config file parser might generate a man page. Either we standardize
  // on a way so each command holder can call methods and functions to
  // create arbitrary help screens, or code to generate help screens
  // needs access to all the commands and options a holder knows about.
  // We decided to go this route here.
  const CommandMap& GetCommands() const { return commands_; }
  const OptionMap& GetShortOptions() const { return short_options_; }
  const OptionMap& GetLongOptions() const { return long_options_; }
  const LeftoverParsers& GetLeftoverParsers() const { return leftover_parsers_; }

 protected:
  virtual void AddError(const string& error) = 0;
  virtual void AddMessage(const string& message) = 0;

 private:
  const char* name_;
  int flags_;

  Command* found_command_;

  CommandMap commands_;
  OptionMap short_options_;
  OptionMap long_options_;

  LeftoverParsers leftover_parsers_;
};

class Command : public CommandHolder {
 public:
  Command(CommandHolder* holder, int flags,
	  const char* name, const char* description);

  const char* GetDescription() { return description_; }
  virtual ConfigParser* GetParser() { return holder_->GetParser(); }

  virtual void Run() {}

 protected:
  virtual void AddError(const string& error);
  virtual void AddMessage(const string& message);

 private:
  CommandHolder* holder_;
  const char* description_;
};

class ConfigParser : public CommandHolder {
 public:
  ConfigParser(int flags, const char* description);
  const char* GetDescription() const { return description_; }
  virtual ConfigParser* GetParser() { return this; }

  // FIXME: all need to be implemented
  enum DumpFlags {
    // Don't show options that have the same value as the default.
    NoShowDefaultOptions = BIT(0),
    // Don't show options that have no value.
    NoShowUnvaluedOptions = BIT(1), 
    // Don't show options that haven't been set from the command line.
    NoShowUnsetOptions = BIT(2)
  };

  bool HasErrors() const;
  bool HasMessages() const;

  bool PrintErrors(ostream* error) const;
  bool GetErrors(string* error) const;
  void FlushErrors();

  bool PrintMessages(ostream* output) const;
  bool GetMessages(string* output) const;
  void FlushMessages();

  virtual void DumpConfigs(int flags, ostream* stream) const = 0;
  virtual void PrintHelp(ostream* stream) const = 0;
  virtual void GetHelp(string* help) const = 0;

  virtual bool ShouldExit() const { return HasErrors(); }

  virtual void AddError(const string& error);
  virtual void AddMessage(const string& message);

 private:
  const char* description_;

  list<string> errors_;
  list<string> messages_;
};

class Option {
 public:
  Option(CommandHolder* parser, int flags, const char* lname,
	 const char* sname, const char* description);
  virtual ~Option() {}

  // FIXME: flags are not really supported.
  enum Flags {
    // Nothing special.
    Default = 0,
    // This flag is mandatory, has to be specified from the command line.
    // FIXME: is mandatory really that useful? Can't we specify constraints instead?
    Mandatory = BIT(0),
    // This flag controls the behavior of the binary? and should not be specified
    // in the configuration file, like --help or --dump.
    Dynamic = BIT(1),
    // Required value is not mandatory.
    ValueIsOptional = BIT(2)
  };

  // Simple accessors.
  
  // Return true if this option was ever set, regardless of the value.
  virtual bool HasBeenSet() const = 0;
  // Return true if this flag has the default value regardless if it's
  // been set by the user or not.
  virtual bool HasDefaultValue() const = 0;

  const char* GetShortName() const { return short_name_; }
  const char* GetLongName() const { return long_name_; }
  const char* GetDescription() const { return description_; }

  virtual const char* GetType() const = 0;
  virtual bool Parse(const char* value, deque<const char*>* argv,
		     ostream* error, ostream* output) = 0;

  // return false if there is no value to this flag. If it's not
  // the default, it's been passed from the command line.
  virtual bool GetAsString(string* value) const = 0;

  void AddTo(CommandHolder* holder);

 private:
  const char* long_name_;
  const char* short_name_;
  const char* description_;
};

#endif /* LIBYAARG_CONFIG_PARSER_H */
