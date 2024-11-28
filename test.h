// test.h
#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>

struct PerformanceResult {
    int data_size;
    std::string generator_name;
    std::string sorter_name;
    double average_time;
};

void run_tests();

#endif // TEST_H
