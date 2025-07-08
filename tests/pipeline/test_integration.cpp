//
// Created by home on 05/07/25.
//

#include "pipeline.h++"
#include "gtest/gtest.h"


TEST(Integration, SimpleCode) {
    auto code = "println([1, 4]);" ;
    runVm(code);
}