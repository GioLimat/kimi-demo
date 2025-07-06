//
// Created by home on 05/07/25.
//

#include "pipeline.h++"
#include "gtest/gtest.h"


TEST(Integration, SimpleCode) {
    auto code = "var x = 4; println(x);";
    runVm(code);
}