#include <algorithm>
#include <array>
#include <benchmark/benchmark.h>
#include <iostream>
#include <numeric>
#include <random>
#include <tuple>
#include <utility>

template <typename T, std::size_t Size, std::size_t... Sizes>
struct MultiArray : public std::array<MultiArray<T, Sizes...>, Size> {};

template <typename T, std::size_t Size>
struct MultiArray<T, Size> : public std::array<T, Size> {};

template <std::size_t... Is> struct seq {};
template <unsigned N, unsigned... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};
template <unsigned... Is> struct gen_seq<0, Is...> : seq<Is...> {};

constexpr static std::size_t staticArraySize{100};

struct BigObject {
    char arr[100];
    const std::string aString{"This string is not so big"};
};

template <std::size_t N, typename T, typename Callable, std::size_t... Is>
constexpr std::array<T, N> rr(Callable &&c, std::index_sequence<Is...>) {
    return std::array<T, N>{c(Is)...};
}

template <typename T, std::size_t N, typename Callable>
constexpr std::array<T, N> randomValues(Callable &&c) {
    return rr<N, T>(std::forward<Callable>(c), std::make_index_sequence<N>{});
}

class Cpp98Fixture : public benchmark::Fixture {};
class Cpp14Fixture : public benchmark::Fixture {};

template <typename T, std::size_t N> auto randoms() {
    std::random_device rd;
    std::uniform_int_distribution<std::size_t> dist(0, staticArraySize - 1);

    return randomValues<T, N>(
        [&rd, &dist](std::size_t) -> T { return dist(rd); });
}

BENCHMARK_DEFINE_F(Cpp14Fixture, staticCreateAndUse)(benchmark::State &state) {
    std::array<int, 100> rads = randoms<int, 100>();

    while (state.KeepRunning()) {
        int index = 0;
        std::int64_t sum = 0;
        benchmark::DoNotOptimize(sum += rads[index++]);
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, staticCreateAndUse)(benchmark::State &state) {
    int rads[100];
    std::generate_n(rads, 100, std::rand);

    while (state.KeepRunning()) {
        int index = 0;
        std::int64_t sum = 0;
        benchmark::DoNotOptimize(sum += rads[index++]);
    }
}

BENCHMARK_DEFINE_F(Cpp14Fixture, copy_and_sum)(benchmark::State &state) {
    std::array<int, 100> rads = randoms<int, 100>();
    std::array<int, 100> rads2;

    while (state.KeepRunning()) {
        rads2 = rads;
        std::int64_t sum = 0;
        benchmark::DoNotOptimize(
            sum = std::accumulate(rads2.begin(), rads2.end(), 0));
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, copy_and_sum)(benchmark::State &state) {
    int rads[100];
    std::generate_n(rads, 100, std::rand);
    int rads2[100];

    while (state.KeepRunning()) {
        std::copy(std::begin(rads), std::end(rads), std::begin(rads2));
        std::int64_t sum = 0;
        benchmark::DoNotOptimize(sum = std::accumulate(rads2, rads2 + 100, 0));
    }
}
BENCHMARK_DEFINE_F(Cpp14Fixture, multi_array_sum)(benchmark::State &state) {
    MultiArray<int, 100, 100> arr;

    for (std::array<int, 100> &a : arr) {
        a = randoms<int, 100>();
    }
    while (state.KeepRunning()) {
        std::int64_t sum = 0;
        for (std::array<int, 100> &a : arr) {
            benchmark::DoNotOptimize(
                sum = std::accumulate(a.begin(), a.end(), 0));
        }
    }
}

BENCHMARK_DEFINE_F(Cpp98Fixture, multi_array_sum)(benchmark::State &state) {
    int rads[100][100];
    for (auto &a : rads) {
        std::generate_n(a, 100, std::rand);
    }

    while (state.KeepRunning()) {
        std::int64_t sum = 0;
        for (auto &a : rads) {
            benchmark::DoNotOptimize(sum = std::accumulate(a, a + 100, 0));
        }
    }
}
BENCHMARK_REGISTER_F(Cpp14Fixture, staticCreateAndUse);
BENCHMARK_REGISTER_F(Cpp98Fixture, staticCreateAndUse);
BENCHMARK_REGISTER_F(Cpp14Fixture, copy_and_sum);
BENCHMARK_REGISTER_F(Cpp98Fixture, copy_and_sum);

BENCHMARK_REGISTER_F(Cpp14Fixture, multi_array_sum);
BENCHMARK_REGISTER_F(Cpp98Fixture, multi_array_sum);
BENCHMARK_MAIN()
