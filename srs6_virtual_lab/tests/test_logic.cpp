#include <cstdio>
#include "../code/final/logic.h"

static int failed = 0;

static void check(bool ok, const char* name) {
  printf("%s %s\n", ok ? "PASS" : "FAIL", name);
  if (!ok) failed++;
}

int main() {
  check(classify(true, 10) == ZONE_RED, "10 cm -> RED");
  check(classify(true, 19.99f) == ZONE_RED, "19.99 cm -> RED");
  check(classify(true, 20) == ZONE_YELLOW, "20 cm -> YELLOW");
  check(classify(true, 35) == ZONE_YELLOW, "35 cm -> YELLOW");
  check(classify(true, 50) == ZONE_YELLOW, "50 cm -> YELLOW");
  check(classify(true, 50.01f) == ZONE_GREEN, "50.01 cm -> GREEN");
  check(classify(true, 70) == ZONE_GREEN, "70 cm -> GREEN");
  check(classify(false, 0) == ZONE_NONE, "invalid -> NONE (LED off)");
  check(classify(false, 999) == ZONE_NONE, "invalid 999 is not free path");

  check(!makeSample(0).valid, "pulseIn=0 -> invalid, not 0 cm");
  Sample s = makeSample(2041);
  check(s.valid && s.cm > 34.9f && s.cm < 35.1f, "2041 us -> ~35 cm");

  float m = 0;
  Sample spike[5] = {{true, 35}, {true, 36}, {true, 300}, {true, 34}, {true, 35}};
  check(medianOfValid(spike, 5, &m) && m == 35, "median rejects 300 cm spike");

  Sample twoLost[5] = {{true, 35}, {false, 0}, {true, 36}, {false, 0}, {true, 34}};
  check(medianOfValid(twoLost, 5, &m) && m == 35, "3 of 5 valid -> valid median");

  Sample threeLost[5] = {{true, 35}, {false, 0}, {false, 0}, {false, 0}, {true, 34}};
  check(!medianOfValid(threeLost, 5, &m), "2 of 5 valid -> invalid (safe)");

  printf("%s: %d failed\n", failed ? "FAILED" : "OK", failed);
  return failed ? 1 : 0;
}
