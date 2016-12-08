#ifndef TEST_COMMON
#define TEST_COMMON

#include <queue>
#include <string>
#include <functional>

using namespace std;

class TestCase
{
public:
  TestCase(string title, function<bool()> func);

  virtual bool pass() const;

protected:
  string title;
  function<bool()> func;
};


class TestScenario
{
public:
  TestScenario();
  ~TestScenario();

  void addCase(const TestCase &c);

protected:
  queue<TestCase> cases;

private:

};


TestScenario& operator>>(TestScenario& t, const TestCase &c);

#endif