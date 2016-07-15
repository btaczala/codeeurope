#include <benchmark/benchmark.h>
#include <benchmark/benchmark_api.h>
#include <iostream>

std::string stringReturn() { return std::string("This is a test string"); }
std::string stringAddAndReturn() {
    return std::string("This is a test string") + std::string("this is a test string") +
           std::string("this is a test string") + std::string("this is a test string");
}

static void BM_StringReturn(benchmark::State &state) {
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range_x(); ++i) {
            std::string empty_string = stringReturn();
        }
    }
}

static void BM_StringAddAdnReturn(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::string empty_string = stringAddAndReturn();
    }
}

BENCHMARK(BM_StringReturn)->Range(8, 8 << 10);
BENCHMARK(BM_StringAddAdnReturn);

BENCHMARK_MAIN()
