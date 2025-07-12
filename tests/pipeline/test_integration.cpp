//
// Created by home on 05/07/25.
//

#include "pipeline.h++"
#include "gtest/gtest.h"


TEST(Integration, SimpleCode) {
    auto code = "var x = [[4]]; x[10 - 10][0] = 10; println(x[0][0]);" ;
    runVm(code);
}