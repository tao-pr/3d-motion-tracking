#ifndef TEST_COMMON
#define TEST_COMMON

#include <functional>

class TestScenario
{
public:
  TestScenario();
  ~TestScenario();

  virtual void setup();
  virtual void tearDown();


private:
} 


#endif