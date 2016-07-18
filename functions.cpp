#include <benchmark/benchmark.h>
#include <iostream>
#include <functional>
#include <map>
#include "common.hpp"

typedef std::function<std::string(const std::string &,
                                  const std::vector<std::string> &)> Callback_t;

struct DifferentType {
    std::map<std::string, std::string> anotherFunction(const Callback_t &t) {
        std::map<std::string, std::string> ret;
        ret[t("", std::vector<std::string>())];
        return ret;
    }
};

void freeFunctionTest(benchmark::State &state) {
    while (state.KeepRunning()) {
        Callback_t cb{freeFunction};
        for (int i = 0; i < state.range_x(); ++i) {
            cb("", std::vector<std::string>());
        }
    }
}

void memberFunctionTest(benchmark::State &state) {
    SomeType t;
    while (state.KeepRunning()) {
        Callback_t cb;
        for (int i = 0; i < state.range_x(); ++i) {
            cb = std::bind(&SomeType::aFunction, &t, std::placeholders::_1,
                           std::placeholders::_2);
            cb("a string", {"asd", "qwe"});
        }
    }
}

void indirectCall(benchmark::State &state) {
    DifferentType dif;
    SomeType t;
    while (state.KeepRunning()) {
        Callback_t cb;
        for (int i = 0; i < state.range_x(); ++i) {
            dif.anotherFunction(std::bind(&SomeType::aFunction, &t,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
        }
    }
}

BENCHMARK(freeFunctionTest)->Range(8, 8 << 10);
BENCHMARK(memberFunctionTest)->Range(8, 8 << 10);
BENCHMARK(indirectCall)->Range(8, 8 << 10);
BENCHMARK_MAIN()

