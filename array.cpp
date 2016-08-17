
#include <algorithm>
#include <array>
#include <benchmark/benchmark.h>
#include <random>

constexpr static std::size_t staticArraySize{10};

struct BigObject {
    char arr[100];
    const std::string aString{"This string is not so big"};
};

template <typename T, std::size_t N, typename Callable>
constexpr std::array<T, N> randomValues(Callable &&c) {
    std::array<T, N> ret{c()};
    return ret;
}

class Cpp98Fixture : public benchmark::Fixture {};
class Cpp14Fixture : public benchmark::Fixture {};

auto randoms() {
    std::random_device rd;
    std::uniform_int_distribution<std::size_t> dist(0, staticArraySize - 1);

    return randomValues<std::size_t, 4>(
        [&rd, &dist]() -> std::size_t { return dist(rd); });
}

BENCHMARK_DEFINE_F(Cpp98Fixture, staticCreateAndUse)(benchmark::State &state) {
    auto rads = randoms();

    while (state.KeepRunning()) {
    }
}

BENCHMARK_MAIN()
