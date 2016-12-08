#include "common.h"

TestCase::TestCase(string title, function<bool()> func)
{
  this->title = title;
  this->func = func;
}

bool TestCase::pass() const
{
  return this->func();
}

TestScenario::~TestScenario()
{ 
  queue<TestCase>().swap(this->cases); 
}


void TestScenario::addCase(const TestCase &c)
{
  this->cases.push(c);
}

TestScenario& operator>>(TestScenario& t, const TestCase &c)
{
  t.addCase(c);
  return t;
}


