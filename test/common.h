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

  bool pass() const;

protected:
  string title;
  function<bool()> func;
};


class TestScenario
{
public:
  TestScenario();
  inline ~TestScenario(){ queue<TestCase>().swap(this->cases); };

  void addCase(const TestCase &c);

protected:
  queue<TestCase> cases;

private:

};


TestScenario& operator>>(TestScenario& t, const TestCase &c)
{
  t.addCase(c);
  return t;
}


#endif