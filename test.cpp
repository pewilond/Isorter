#include "test.h"
#include "display_charts.h"
#include <iostream>
#include <cassert>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "DataStructures/Sequence.h"
#include "DataStructures/DynamicArraySmart.h"
#include "DataStructures/LinkedListSmart.h"
#include "ISorter.h"
#include "HeapSorter.h"
#include "MergeSorter.h"

int CompareInt(int a, int b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

template <typename T>
bool is_sorted(Sequence<T>* seq, int (*cmp)(T, T)) {
    for (int i = 1; i < seq->GetLength(); ++i) {
        if (cmp(seq->Get(i - 1), seq->Get(i)) > 0) {
            return false;
        }
    }
    return true;
}

enum class DataType {
    RANDOM,
    REVERSE,
    PARTIAL_SORTED,
    ALREADY_SORTED,
    EMPTY
};

std::vector<int> generate_data(int size, DataType type, int sorted_elements = 0) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    switch (type) {
        case DataType::RANDOM:
            for (int& val : data) {
                val = dis(gen);
            }
            break;
        case DataType::REVERSE:
            for (int i = 0; i < size; ++i) {
                data[i] = size - i;
            }
            break;
        case DataType::PARTIAL_SORTED:
            for (int i = 0; i < sorted_elements && i < size; ++i) {
                data[i] = i;
            }
            for (int i = sorted_elements; i < size; ++i) {
                data[i] = dis(gen);
            }
            break;
        case DataType::ALREADY_SORTED:
            for (int i = 0; i < size; ++i) {
                data[i] = i;
            }
            break;
        case DataType::EMPTY:
            data.clear();
            break;
    }

    return data;
}

void run_functional_tests() {
    std::cout << "Running functional tests...\n";

    struct TestScenario {
        std::string description;
        std::vector<int> data;
    };

    std::vector<TestScenario> test_scenarios = {
            {"Random data", {5, 3, 8, 1, 9, 2}},
            {"Reverse sorted data", {9, 8, 7, 6, 5, 4, 3, 2, 1}},
            {"Partially sorted data (1 element sorted)", {1, 3, 8, 6, 5, 2}},
            {"Partially sorted data (2 elements sorted)", {1, 2, 8, 6, 5, 3}},
            {"Partially sorted data (3 elements sorted)", {1, 2, 3, 6, 5, 4}},
            {"Already sorted data", {1, 2, 3, 4, 5, 6}},
            {"Empty data", {}}
    };

    struct SorterInfo {
        std::string name;
        ISorter<int>* sorter;
    };

    std::vector<SorterInfo> sorters = {
            {"HeapSorter", new HeapSorter<int>()},
            {"MergeSorter", new MergeSorter<int>()}
    };

    struct SequenceInfo {
        std::string name;
        Sequence<int>* (*create_sequence)(const int*, int);
    };

    std::vector<SequenceInfo> sequences = {
            {"DynamicArraySmart", [](const int* data, int size) -> Sequence<int>* { return new DynamicArraySmart<int>(data, size); }},
            {"LinkedListSmart", [](const int* data, int size) -> Sequence<int>* { return new LinkedListSmart<int>(data, size); }}
    };

    for (const auto& scenario : test_scenarios) {
        std::cout << "Testing: " << scenario.description << "\n";

        for (const auto& seq_info : sequences) {
            for (const auto& sorter_info : sorters) {
                Sequence<int>* seq = nullptr;
                if (scenario.data.empty()) {
                    seq = seq_info.create_sequence(nullptr, 0);
                } else {
                    seq = seq_info.create_sequence(scenario.data.data(), scenario.data.size());
                }

                sorter_info.sorter->Sort(seq, CompareInt);

                if (scenario.data.empty()) {
                    assert(seq->GetLength() == 0 && "Sorted empty sequence should have length 0.");
                } else {
                    assert(is_sorted(seq, CompareInt) && "Sequence is not sorted correctly.");
                }

                delete seq;
            }
        }
    }

    for (auto& sorter_info : sorters) {
        delete sorter_info.sorter;
    }

    std::cout << "Functional tests passed.\n";
}

std::vector<PerformanceResult> run_performance_tests() {
    std::cout << "Running performance tests...\n";
    const std::vector<int> sizes = {10000, 50000, 100000};
    struct GeneratorInfo {
        std::string name;
        DataType type;
        int sorted_elements;
    };
    std::vector<GeneratorInfo> generators = {
            {"Random", DataType::RANDOM, 0},
            {"Reverse Sorted", DataType::REVERSE, 0},
            {"Partially Sorted (1 sorted)", DataType::PARTIAL_SORTED, 1},
            {"Partially Sorted (2 sorted)", DataType::PARTIAL_SORTED, 2},
            {"Partially Sorted (3 sorted)", DataType::PARTIAL_SORTED, 3},
            {"Already Sorted", DataType::ALREADY_SORTED, 0},
            {"Empty", DataType::EMPTY, 0}
    };
    struct SorterInfo {
        std::string name;
        ISorter<int>* sorter;
    };
    std::vector<SorterInfo> sorters = {
            {"HeapSorter", new HeapSorter<int>()},
            {"MergeSorter", new MergeSorter<int>()}
    };
    struct SequenceInfo {
        std::string name;
        Sequence<int>* (*create_sequence)(const int*, int);
    };
    std::vector<SequenceInfo> sequences = {
            {"DynamicArraySmart", [](const int* data, int size) -> Sequence<int>* { return new DynamicArraySmart<int>(data, size); }}
    };
    const int num_runs = 5;
    std::vector<PerformanceResult> performance_results;

    for (int size : sizes) {
        std::cout << "\nData Size: " << size << " elements\n";
        for (const auto& gen_info : generators) {
            if (gen_info.type == DataType::EMPTY && size != 0) continue;
            std::cout << "  Data Generator: " << gen_info.name << "\n";
            for (const auto& seq_info : sequences) {
                for (const auto& sorter_info : sorters) {
                    double total_time = 0.0;
                    for (int run = 0; run < num_runs; ++run) {
                        std::vector<int> data = generate_data(size, gen_info.type, gen_info.sorted_elements);
                        Sequence<int>* seq = nullptr;
                        if (data.empty()) {
                            seq = seq_info.create_sequence(nullptr, 0);
                        } else {
                            seq = seq_info.create_sequence(data.data(), data.size());
                        }
                        auto start = std::chrono::high_resolution_clock::now();
                        sorter_info.sorter->Sort(seq, CompareInt);
                        auto end = std::chrono::high_resolution_clock::now();
                        if (!data.empty()) {
                            assert(is_sorted(seq, CompareInt) && "Sequence is not sorted correctly.");
                        } else {
                            assert(seq->GetLength() == 0 && "Sorted empty sequence should have length 0.");
                        }
                        std::chrono::duration<double> duration = end - start;
                        total_time += duration.count();
                        delete seq;
                    }
                    double average_time = total_time / num_runs;
                    performance_results.push_back({size, gen_info.name, sorter_info.name, average_time});
                    std::cout << "    " << sorter_info.name << " with DynamicArraySmart: "
                              << average_time << " seconds (avg over " << num_runs << " runs)\n";
                }
            }
        }
    }

    for (auto& sorter_info : sorters) {
        delete sorter_info.sorter;
    }

    std::cout << "\nPerformance tests completed.\n";
    return performance_results;
}

void run_tests() {
    run_functional_tests();
    std::vector<PerformanceResult> perf_results = run_performance_tests();
    display_charts(perf_results);
}
