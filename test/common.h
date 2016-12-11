#ifndef TEST_COMMON
#define TEST_COMMON

#include <queue>
#include <string>
#include <functional>
#include <iostream>
#include "coloriser.h"

using namespace std;

class TestCase
{
public:
  TestCase(string title, function<bool()> func);

  virtual bool pass() const;
  inline string getTitle() const { return this->title; }

protected:
  string title;
  function<bool()> func;
};


class TestScenario
{
public:
  TestScenario(string title);
  ~TestScenario();

  void addCase(const TestCase &c);
  bool runAll();

  friend TestScenario& operator>>(TestScenario& t, const TestCase &c);

protected:
  string title;
  queue<TestCase> cases;

private:

};


TestScenario& operator>>(TestScenario& t, const TestCase &c);

#endif