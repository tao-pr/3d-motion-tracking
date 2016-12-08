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

TestScenario::TestScenario(string title)
{
  this->title = title;
}

TestScenario::~TestScenario()
{ 
  queue<TestCase>().swap(this->cases); 
}


void TestScenario::addCase(const TestCase &c)
{
  this->cases.push(c);
}

bool TestScenario::runAll()
{
  bool br = true;
  cout << "[Test] " << this->title << endl;
  while (!this->cases.empty())
  {
    auto nextCase = this->cases.front();
    this->cases.pop();

    cout << "...case# " << nextCase.getTitle() << endl;
    br &= nextCase.pass();
  }

  return br;
}

TestScenario& operator>>(TestScenario& t, const TestCase &c)
{
  t.addCase(c);
  return t;
}


