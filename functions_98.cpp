#include <benchmark/benchmark.h>
#include <iostream>
#include <functional>
#include <map>

#include "common.hpp"

struct SomeType;
typedef std::string (SomeType::*Callback_t)(const std::string &,
                                            const std::vector<std::string> &);
typedef std::string (*FreeCallback_t)(const std::string &,
                                      const std::vector<std::string> &);

struct DifferentType {
    std::map<std::string, std::string> anotherFunction(Callback_t &t,
                                                       SomeType &type) {
        std::map<std::string, std::string> ret;
        ret[((type).*(t))("", std::vector<std::string>())];
        return ret;
    }
};

void freeFunctionTest(benchmark::State &state) {
    while (state.KeepRunning()) {
        FreeCallback_t cb = freeFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            cb("", std::vector<std::string>());
        }
    }
}

void memberFunctionTest(benchmark::State &state) {
    SomeType t;
    while (state.KeepRunning()) {
        Callback_t cb = &SomeType::aFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            // std::invoke(cb, t, "", vector");
            ((t).*(cb))("", std::vector<std::string>());
        }
    }
}

void indirectCall(benchmark::State &state) {
    DifferentType dif;
    SomeType t;
    while (state.KeepRunning()) {
        Callback_t cb = &SomeType::aFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            dif.anotherFunction(cb, t);
        }
    }
}

BENCHMARK(freeFunctionTest)->Range(8, 8 << 10);
BENCHMARK(memberFunctionTest)->Range(8, 8 << 10);
BENCHMARK(indirectCall)->Range(8, 8 << 10);
BENCHMARK_MAIN()
