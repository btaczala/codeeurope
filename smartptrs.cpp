
#include <benchmark/benchmark.h>
#include <boost/config.hpp>
#include <memory>

#include "common.hpp"

struct Resource {
    void aFunction(const std::string &name) {}
};

static void uniquePtrCreate(benchmark::State &state) {
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range_x(); ++i) {
            auto res = std::make_unique<Resource>();
            benchmark::DoNotOptimize(res);
        }
    }
}

void rawPointerCreate(benchmark::State &state) {
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range_x(); ++i) {
            auto res = new Resource;
            benchmark::DoNotOptimize(res);
        }
    }
}

void rawPointerCall(benchmark::State &state) {
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range_x(); ++i) {
            auto res = new Resource;
            res->aFunction("");
            benchmark::DoNotOptimize(res);
        }
    }
}

void uniquePointerCall(benchmark::State &state) {
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range_x(); ++i) {
            auto res = std::make_unique<Resource>();
            res->aFunction("");
            benchmark::DoNotOptimize(res);
        }
    }
}

BENCHMARK(rawPointerCreate)->Range(8, 8 << 10);
BENCHMARK(uniquePtrCreate)->Range(8, 8 << 10);
BENCHMARK(rawPointerCall)->Range(8, 8 << 10);
BENCHMARK(uniquePointerCall)->Range(8, 8 << 10);
BENCHMARK_MAIN()
