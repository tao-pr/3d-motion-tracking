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
  cout << GREEN << "[Test] " << this->title << endl << RESET;
  while (!this->cases.empty())
  {
    auto nextCase = this->cases.front();
    this->cases.pop();

    cout << CYAN << "...case# " << nextCase.getTitle() << endl << RESET;
    auto rc = nextCase.pass();
    if (rc)
      cout << RED << "...[FAILED]" << endl << RESET;
    else
      cout << GREEN << "...[PASSED]" << endl << RESET;
    br &= rc;
  }

  return br;
}

TestScenario& operator>>(TestScenario& t, const TestCase &c)
{
  t.addCase(c);
  return t;
}


