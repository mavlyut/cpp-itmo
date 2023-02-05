#include "../huffman_lib/include/consts.h"
#include "../huffman_lib/include/huffman.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <sstream>

void ASSERT_EQ_FILES(char const* in1, char const* in2) {
  std::ifstream fin1(in1, std::ios_base::in), fin2(in2, std::ios_base::in);
  char tmp1 = 0, tmp2 = 0;
  while (!fin1.eof() && !fin2.eof()) {
    fin1 >> tmp1;
    fin2 >> tmp2;
    ASSERT_EQ(tmp1, tmp2);
  }
  ASSERT_TRUE(fin1.eof() && fin2.eof());
  fin1.close();
  fin2.close();
}

const std::string path = "unit-tests";

static void encode_file(std::string const& in, std::string const& out) {
  std::fstream fin(in, std::fstream::in);
  std::fstream fout(out, std::fstream::out);
  encode(fin, fout);
  fin.close();
  fout.close();
}

static std::string encode_string(std::string const& str) {
  std::stringstream sin(str, std::stringstream::in);
  std::stringstream sout(std::stringstream::out);
  encode(sin, sout);
  sin.clear();
  return sout.str();
}

static void decode_file(std::string const& in, std::string const& out) {
  std::fstream fin(in, std::fstream::in);
  std::fstream fout(out, std::fstream::out);
  decode(fin, fout);
  fin.close();
  fout.close();
}

static std::string decode_string(std::string const& str) {
  std::stringstream sin(str, std::stringstream::in);
  std::stringstream sout(std::stringstream::out);
  decode(sin, sout);
  sin.clear();
  return sout.str();
}

void htest(std::string const& input) {
  std::string in = path + "/files/" + input;
  std::string enc = path + "/" + input + ".huf";
  std::string dec = path + "/" + input + "_decomp";
  encode_file(in, enc);
  decode_file(enc, dec);
  ASSERT_EQ_FILES(in.c_str(), dec.c_str());
  std::filesystem::remove(enc);
  std::filesystem::remove(dec);
}

#define HTEST(input) TEST(correctness, input) { htest(#input); }

TEST(special, empty) {
  ASSERT_GE(encode_string("").length(), 0);
}

TEST(special, file_not_found) {
  ASSERT_THROW(encode_file("", ""), std::runtime_error);
}

#define THROW_DECODE_TEST(name, input) TEST(special, name) {    \
    ASSERT_THROW(decode_string(input), std::runtime_error);     \
  }

THROW_DECODE_TEST(simple, "ccdacbddfeacd cfbcabcd\n")

THROW_DECODE_TEST(bad_tree, "3 5fg0")

THROW_DECODE_TEST(no_rem, "3 3df")

THROW_DECODE_TEST(short_tree, "430 320abc1d1f0eg")

THROW_DECODE_TEST(big_tree, "1239 12jk3bkbk1jb3123bk1j2b3 lkdjksffkjl bnlkgbg'")

// 0
HTEST(empty)

// 7 b
HTEST(one_char)

// 127 b
HTEST(simple)

// 1 Kb, 165 b
HTEST(i_said_everything_i_wanted)

// 3 Kb, 167 b
HTEST(imo2022_chinese)

// 21 Kb, 667 b
HTEST(test_elf)

// 62 Kb, 839 b
HTEST(war_and_peace_wiki)

// 137 Kb, 648 b
HTEST(test_asm)

// 263 Kb, 907 b
HTEST(cpp_tutorial)

// 356 Kb, 1008 b
HTEST(some_program)

// 1 Mb, 351 Kb, 718 b
HTEST(organic_chemistry_en)

// 1 Mb, 391 Kb, 166 b
HTEST(java_tutorial)

// 5 Mb, 788 Kb, 947 b
HTEST(organic_chemistry_in_4_volumes)

#ifdef _ENABLE_BIG_TESTS

// 16 Mb, 651 Mb, 365 b
HTEST(AAA)

static char get(size_t i) {
  size_t ans = 0;
  while (i % 2) {
    i /= 2;
    ans++;
  }
  return static_cast<char>(ans + 'a');
}

// 63 Mb, 1023 Kb, 1023 b
TEST(correctness, full_abacaba) {
  std::string new_path = path + "/files/full_abacaba";
  std::ofstream fout(new_path, std::ios_base::binary | std::ios_base::out);
  size_t end = (1L << 26) - 1;
  for (size_t i = 0; i < end; i++) {
    fout << get(i);
  }
  fout.close();
  htest("full_abacaba");
  std::filesystem::remove(new_path);
}

// 1 Gb
TEST(correctness, bigfile) {
  srand(time(nullptr));
  std::string new_path = path + "/files/bigfile";
  std::ofstream fout(new_path, std::ios_base::binary | std::ios_base::out);
  for (size_t i = 0; i < 1024 * 1024 * 1024; i++) {
    fout << static_cast<char>(rand() % ALPHABET_SIZE + 'a');
  }
  fout.close();
  htest("bigfile");
  std::filesystem::remove(new_path);
}

#endif // _ENABLE_BIG_TESTS
