#include "logger.h"


int main() {
  INFO("Hi this is info");
  INFO("My age is ", "hi", "good morning", "good", "boy");
  ERROR("Hi this is error");
  WARN("Hi this is warning");
}
