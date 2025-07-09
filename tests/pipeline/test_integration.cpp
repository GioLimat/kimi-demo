//
// Created by home on 05/07/25.
//

#include "pipeline.h++"
#include "gtest/gtest.h"


TEST(Integration, SimpleCode) {
    auto code = "val x = [[10,20], [30, 40]];  for (var i = 0; i < 2; i++) { for (var j = 0; j < 2; j++) { println(x[i][j]); } }" ;
    runVm(code);
}