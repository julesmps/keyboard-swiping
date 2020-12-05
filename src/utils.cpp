// Juliana Pacheco
// University of Florida

#include "src/utils.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <string>

namespace utils {

void to_lower(std::string& str) {
  for(auto iter = str.begin(); iter != str.end(); ++iter)
    *iter = (unsigned char) std::tolower((unsigned char) *iter);
}

std::string to_lower(const std::string& str) {
  std::string result;
  for(char c : str)
    result += (unsigned char) std::tolower((unsigned char) c);
  return result;
}

void read_from_file(Trie& trie, const char* filename) {
  std::ifstream is(filename);
  while(is >> trie);
  is.close();
}

void read_from_file(Trie& trie, const std::string& filename) {
  read_from_file(trie, filename.c_str());
}

void read_file_with_frequency(Trie& trie,
      std::unordered_map<std::string,std::size_t>& map,
      const std::string& filename,
      char separator) {
  std::ifstream is(filename);
  std::string line, word, freq;
  std::istringstream ls;
  while(std::getline(is, line)) {
    if(line.empty())
      continue;
    ls = std::istringstream(line);

    std::getline(ls, word, separator);
    to_lower(word);
    trie.insert(word);

    std::getline(ls, freq);
    map[word] += std::stoul(freq);
  }
  is.close();
}

std::istream& operator>>(std::istream& is, Trie& trie) {
  std::string line;
  is >> line;
  trie.insert(line);
  return is;
}

} /* utils */
