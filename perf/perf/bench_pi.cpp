#include <benchmark/benchmark.h>

#include <cstdint>
#include <cstdlib>
#include <random>

// Computes PI using monte carlo method.
// DO NOT change the algorithm.
void BM_ComputePi(benchmark::State& state) {
  int64_t total = 0;
  int64_t inside_circle = 0;

  std::default_random_engine engine(42);
  std::uniform_int_distribution<int> randomInt(0, 1000);

  for (auto _ : state) {
    int x = randomInt(engine);
    int y = randomInt(engine);

    total++;
    if (x * x + y * y < 1000 * 1000) {
      inside_circle++;
    }
  }

  state.counters["pi"] = 4 * double(inside_circle) / total;
}

BENCHMARK(BM_ComputePi)->Threads(2);

BENCHMARK_MAIN();
