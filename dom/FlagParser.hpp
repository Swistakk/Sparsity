#pragma once

#include "Headers.hpp"

struct FlagParser {
  map<string, string> flags;
  set<string> asked; 
  void ParseFlags(int argc, char** argv);
  string GetFlag(string s, bool required);
  void Close();
};