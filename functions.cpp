#include <benchmark/benchmark.h>
#include <iostream>
#include <functional>
#include <map>
#include "common.hpp"

std::string freeFunction(const std::string &,
                         const std::vector<std::string> &vec) {
    return "";
}

typedef std::function<std::string(
    const std::string &, const std::vector<std::string> &)> StdCallback_t;
// function pointers
struct SomeType;
typedef std::string (SomeType::*Callback_t)(const std::string &,
                                            const std::vector<std::string> &);
typedef std::string (*FreeCallback_t)(const std::string &,
                                      const std::vector<std::string> &);

struct DifferentType {
    std::map<std::string, std::string> anotherFunction(const StdCallback_t &t) {
        std::map<std::string, std::string> ret;
        ret[t("", std::vector<std::string>())];
        return ret;
    }

    std::map<std::string, std::string> anotherFunction(Callback_t &t,
                                                       SomeType &type) {
        std::map<std::string, std::string> ret;
        ret[((type).*(t))("", std::vector<std::string>())];
        return ret;
    }
};

void oldStylefreeFunctionTest(benchmark::State &state) {
    while (state.KeepRunning()) {
        FreeCallback_t cb = &freeFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            auto result = cb("", std::vector<std::string>());
            benchmark::DoNotOptimize(result);
        }
    }
}

void newStylefreeFunctionTest(benchmark::State &state) {
    while (state.KeepRunning()) {
        StdCallback_t cb{&freeFunction};
        for (int i = 0; i < state.range_x(); ++i) {
            auto result = cb("", std::vector<std::string>());
            benchmark::DoNotOptimize(result);
        }
    }
}

void memberFunctionTest(benchmark::State &state) {
    SomeType t;
    while (state.KeepRunning()) {
        StdCallback_t cb;
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

BENCHMARK(oldStylefreeFunctionTest)->Range(8, 8 << 10);
BENCHMARK(newStylefreeFunctionTest)->Range(8, 8 << 10);
// BENCHMARK(memberFunctionTest)->Range(8, 8 << 10);
// BENCHMARK(indirectCall)->Range(8, 8 << 10);
BENCHMARK_MAIN()
