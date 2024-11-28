cmake_minimum_required(VERSION 3.16)
project(lab2_3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

include_directories(DataStructures)


include_directories(DataStructures)

set(DATA_STRUCTURES
        DataStructures/ArraySequence.h
        DataStructures/ArraySequence.tpp
        DataStructures/DynamicArray.h
        DataStructures/DynamicArray.tpp
        DataStructures/ImmutableArraySequence.h
        DataStructures/ImmutableArraySequence.tpp
        DataStructures/ImmutableSequence.h
        DataStructures/LinkedList.h
        DataStructures/LinkedList.tpp
        DataStructures/ListSequence.h
        DataStructures/ListSequence.tpp
        DataStructures/Sequence.h
        DataStructures/DynamicArraySmart.h
        DataStructures/LinkedListSmart.h
        DataStructures/UnqPtr.h
        DataStructures/ShrdPtr.h
)

add_executable(lab2_3
        main.cpp
        ISorter.h
        HeapSorter.h
        MergeSorter.h
        interface.h
        interface.cpp
        test.h
        test.cpp
        display_charts.cpp
        display_charts.h
        ${DATA_STRUCTURES}
)

target_link_libraries(lab2_3 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts)
