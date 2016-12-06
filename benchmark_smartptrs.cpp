#include <algorithm>
#include <benchmark/benchmark.h>
#include <cmath>
#include <experimental/optional>
#include <memory>
#include <random>

struct Resource {
    int aFunction(const std::string &name) { return std::pow(name.size(), 2); }
};

std::string const default_chars =
    "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

std::string randomStr() {
    std::mt19937_64 gen{std::random_device()()};

    std::uniform_int_distribution<size_t> dist{0, default_chars.length() - 1};

    std::string ret;

    const int len = 15;
    std::generate_n(std::back_inserter(ret), len,
                    [&] { return default_chars[dist(gen)]; });
    return ret;
}

bool valu;

bool foo(Resource *res) {

    if (res) {
        valu = true;
    } else {
        valu = false;
    }

    return valu;
}

bool foo(const std::experimental::optional<Resource> &res) {
    if (res) {
        valu = true;
    } else {
        valu = false;
    }
    return valu;
}

class Cpp98Fixture : public benchmark::Fixture {};
class Cpp14Fixture : public benchmark::Fixture {};

BENCHMARK_DEFINE_F(Cpp14Fixture, ptrCreate)(benchmark::State &state) {
    while (state.KeepRunning()) {
        auto res = std::make_unique<Resource>();
        benchmark::DoNotOptimize(res);
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, ptrCreate)(benchmark::State &state) {
    while (state.KeepRunning()) {
        auto res = new Resource;
        benchmark::DoNotOptimize(res);
        delete res;
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, ptrCall)(benchmark::State &state) {
    auto res = new Resource;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(res);
        auto ret = res->aFunction(randomStr());
        benchmark::DoNotOptimize(ret);
    }
    delete res;
}

BENCHMARK_DEFINE_F(Cpp14Fixture, ptrCall)(benchmark::State &state) {
    auto res = std::make_unique<Resource>();
    while (state.KeepRunning()) {
        auto ret = res->aFunction(randomStr());
        benchmark::DoNotOptimize(ret);
    }
}
BENCHMARK_DEFINE_F(Cpp14Fixture, optional_empty)(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::experimental::optional<Resource> o;
        benchmark::DoNotOptimize(valu = foo(o));
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, optional_empty)(benchmark::State &state) {
    while (state.KeepRunning()) {
        Resource *o{nullptr};
        benchmark::DoNotOptimize(valu = foo(o));
    }
}
BENCHMARK_DEFINE_F(Cpp14Fixture, optional_valid)(benchmark::State &state) {
    while (state.KeepRunning()) {
        std::experimental::optional<Resource> o{Resource{}};
        benchmark::DoNotOptimize(valu = foo(o));
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, optional_valid)(benchmark::State &state) {
    while (state.KeepRunning()) {
        Resource *o{new Resource};
        benchmark::DoNotOptimize(valu = foo(o));
    }
}

BENCHMARK_REGISTER_F(Cpp14Fixture, ptrCreate);
BENCHMARK_REGISTER_F(Cpp98Fixture, ptrCreate);
BENCHMARK_REGISTER_F(Cpp14Fixture, ptrCall);
BENCHMARK_REGISTER_F(Cpp98Fixture, ptrCall);
BENCHMARK_REGISTER_F(Cpp14Fixture, optional_empty);
BENCHMARK_REGISTER_F(Cpp98Fixture, optional_empty);
BENCHMARK_REGISTER_F(Cpp14Fixture, optional_valid);
BENCHMARK_REGISTER_F(Cpp98Fixture, optional_valid);
BENCHMARK_MAIN()
