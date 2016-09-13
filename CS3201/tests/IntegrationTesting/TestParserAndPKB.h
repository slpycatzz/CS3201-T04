#pragma once

#include <string>

class TestParserAndPKB {
  public:
  TestParserAndPKB();
  ~TestParserAndPKB();

  // method for parsing the SIMPLE source
  void parse(std::string filename);
};

