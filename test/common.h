#ifndef TEST_COMMON
#define TEST_COMMON

#include <queue>
#include <string>
#include <functional>

using namespace std;

class TestCase
{
public:
  TestCase(string title, function<void()> func);
private:
};


class TestScenario
{
public:
  TestScenario();
  inline ~TestScenario(){ queue<TestCase>().swap(this->cases); };

  TestScenario& operator>>(const TestCase &c);

protected:
  queue<TestCase> cases;

private:

};



#endif