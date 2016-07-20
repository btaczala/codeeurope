/**
 */
#include "common.hpp"
#include <benchmark/benchmark.h>
#include <functional>
#include <iostream>
#include <map>

struct SomeType {
    std::string aFunction(const std::string &,
                          const std::vector<std::string> &vec) {
        return "";
    }
};

std::string freeFunction(const std::string &,
                         const std::vector<std::string> &vec) {
    return "";
}

typedef std::function<std::string(const std::string &,
                                  const std::vector<std::string> &)>
    StdCallback_t;
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

class Cpp98Fixture : public benchmark::Fixture {};
class Cpp14Fixture : public benchmark::Fixture {};

BENCHMARK_DEFINE_F(Cpp98Fixture, freeFunctionTest)(benchmark::State &state) {
    while (state.KeepRunning()) {
        FreeCallback_t cb = &freeFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            auto result = cb("", std::vector<std::string>());
        }
    }
}

BENCHMARK_DEFINE_F(Cpp14Fixture, freeFunctionTest)(benchmark::State &state) {
    while (state.KeepRunning()) {
        StdCallback_t cb{&freeFunction};
        for (int i = 0; i < state.range_x(); ++i) {
            auto result = cb("", std::vector<std::string>());
        }
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, memberFunctionTest)(benchmark::State &state) {
    SomeType t;
    while (state.KeepRunning()) {
        Callback_t cb = &SomeType::aFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            auto result = ((t).*(cb))("", std::vector<std::string>());
        }
    }
}

BENCHMARK_DEFINE_F(Cpp14Fixture, memberFunctionTest)(benchmark::State &state) {
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

BENCHMARK_DEFINE_F(Cpp98Fixture, indirectCall)(benchmark::State &state) {
    DifferentType dif;
    SomeType t;
    while (state.KeepRunning()) {
        Callback_t cb = &SomeType::aFunction;
        for (int i = 0; i < state.range_x(); ++i) {
            auto result = dif.anotherFunction(cb, t);
        }
    }
}

BENCHMARK_DEFINE_F(Cpp14Fixture, indirectCall)(benchmark::State &state) {
    DifferentType dif;
    SomeType t;
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range_x(); ++i) {
            dif.anotherFunction(std::bind(&SomeType::aFunction, &t,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
        }
    }
}

BENCHMARK_REGISTER_F(Cpp98Fixture, freeFunctionTest)->Range(8, 8 << 10);
BENCHMARK_REGISTER_F(Cpp14Fixture, freeFunctionTest)->Range(8, 8 << 10);
BENCHMARK_REGISTER_F(Cpp98Fixture, memberFunctionTest)->Range(8, 8 << 10);
BENCHMARK_REGISTER_F(Cpp14Fixture, memberFunctionTest)->Range(8, 8 << 10);
BENCHMARK_REGISTER_F(Cpp98Fixture, indirectCall)->Range(8, 8 << 10);
BENCHMARK_REGISTER_F(Cpp14Fixture, indirectCall)->Range(8, 8 << 10);
BENCHMARK_MAIN()
