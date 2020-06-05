cmake_minimum_required(VERSION 3.16)
project(asparagus)

set(CMAKE_CXX_STANDARD 14)

set(ASPARAGUS_SOURCES
        board.cc
        board.h
        cache.cc
        cache.h
        cell_set.cc
        cell_set.h
        common.h
        config.cc
        config.h
        engine.cc
        engine.h
        gomocup_protocol.cc
        gomocup_protocol.h
        main.cc
        patterns.cc
        patterns.h
        protocol.cc
        protocol.h
        randoms.cc
        randoms.h
        simple_protocol.cc
        simple_protocol.h
        controller.h controller.cc)

add_executable(asparagus ${ASPARAGUS_SOURCES})
