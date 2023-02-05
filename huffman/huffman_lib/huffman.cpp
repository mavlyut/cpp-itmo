//
// Created by mavlyut on 01/10/22.
//

#include "include/huffman.h"
#include "include/binary_reader.h"
#include "include/binary_writer.h"
#include "include/huffman_tree.h"

static void check_stream(std::istream& in) {
  if (!in.good())
    throw std::runtime_error("Broken file");
}

void encode(std::istream& fin, std::ostream& fout) {
  counter cntr;
  cntr.read_from_file(fin);
  encode_huffman_tree tr(cntr);
  fin.clear();
  fin.seekg(0, std::ios::beg);
  check_stream(fin);
  binary_writer bw(fout);
  write(bw, tr);
  auto rdbuf = fin.rdbuf();
  std::array<char, BUFSIZE> buf{};
  std::streamsize sz = -1;
  while (sz != 0) {
    sz = rdbuf->sgetn(buf.data(), BUFSIZE);
    for (size_t i = 0; i < sz; i++)
      bw.write(tr.get_code(buf[i]));
  }
  bw.flush();
}

void decode(std::istream& fin, std::ostream& fout) {
  check_stream(fin);
  binary_reader br(fin);
  decode_huffman_tree tr(br);
  ind_t tmp_node = tr.get_root();
  std::string buf(BUFSIZE, 0);
  size_t pos = 0;
  while (!br.eof()) {
    bool b = br.next_bool();
    tr.move(tmp_node, b);
    if (tr.is_leaf(tmp_node)) {
      buf[pos++] = tr.get_char(tmp_node);
      if (pos == BUFSIZE) {
        fout << buf;
        pos = 0;
      }
      tmp_node = tr.get_root();
    }
  }
  for (size_t i = 0; i < pos; i++)
    fout << buf[i];
}
