// Juliana Pacheco
// University of Florida

#include "src/utils.h"

#include <fstream>
#include <string>

bool read_from_file(Trie& trie, const char* filename) {
  try {
    std::ifstream is(filename);
    while(is >> trie);
    is.close();
  } catch(...) {
    return false;
  }
  return true;
}

bool read_from_file(Trie& trie, const std::string& filename) {
  return read_from_file(trie, filename.c_str());
}

std::istream& operator>>(std::istream& is, Trie& trie) {
  std::string line;
  is >> line;
  trie.insert(line);
  return is;
}
