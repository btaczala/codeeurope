
#include <benchmark/benchmark.h>
#include <boost/config.hpp>
#include <memory>

struct Resource {};

#ifdef BOOST_NO_CXX11_SMART_PTR
typedef Resource *ResourcePtr;
#else
// typedef  std::shared_ptr<Resource> ResourcePtr;
typedef std::unique_ptr<Resource> ResourcePtr;
#endif

ResourcePtr getResource() {
#ifdef BOOST_NO_CXX11_SMART_PTR
    return new Resource;
#else
    // return std::make_shared<Resource>();
    return std::make_unique<Resource>();
#endif
}

static void BM_SharedPointerUsage(benchmark::State &state) {
    while (state.KeepRunning()) {
        ResourcePtr res = getResource();
        benchmark::DoNotOptimize(res);
    }
}

BENCHMARK(BM_SharedPointerUsage);

BENCHMARK_MAIN()
