#include <benchmark/benchmark.h>

#include <sstream>

struct User {
  int64_t id;
  std::string username;

  std::string to_string() const {
    std::string tmp = "{\"id\":";
    tmp += std::to_string(id);
    tmp += ",\"username\":\"";
    tmp += username;
    tmp += "\"}";
    return tmp;
  }
};

std::ostream& operator<<(std::ostream& out, const User& user) {
  return out << user.to_string();
}

std::string ToJson(const User& user) {
  return user.to_string();
}

// don't change structs!
struct Message {
  int64_t id;
  std::string subject;
  std::string body;

  User from, to;

  std::string to_string() const {
    std::string tmp = "{\"id\":";
    tmp += std::to_string(id);
    tmp += ",\"subject\":\"";
    tmp += subject;
    tmp += "\",\"body\":\"";
    tmp += body;
    tmp += "\",\"from\":";
    tmp += from.to_string();
    tmp += ",\"to\":";
    tmp += to.to_string();
    tmp += "}";
    return tmp;
  }
};

// DO NOT change signature of this function.
// You may change signature of other functions.
std::string ToJson(const Message& msg) {
  return msg.to_string();
}

void BM_MessageToJson(benchmark::State& state) {
  Message msg{1000, "About modules",
              "So, when is that 'modules' proposal coming?",
              User{12345, "Herb Sutter"}, User{1, "Biern Stroustrup"}};

  for (auto _ : state) {
    benchmark::DoNotOptimize(ToJson(msg));
  }

  auto json = ToJson(msg);
  std::string expected =
    R"({"id":1000,"subject":"About modules","body":"So, when is that 'modules' proposal coming?","from":{"id":12345,"username":"Herb Sutter"},"to":{"id":1,"username":"Biern Stroustrup"}})";
  if (json != expected) {
    state.SkipWithError("Wrong output");
  }
}

BENCHMARK(BM_MessageToJson);

BENCHMARK_MAIN();
