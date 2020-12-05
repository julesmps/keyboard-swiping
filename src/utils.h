// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_UTILS_H
#define KEYBOARD_SWIPING_UTILS_H

#include <istream>
#include <string>
#include <unordered_map>
#include "src/trie.h"

namespace utils {

void to_lower(std::string& str);
std::string to_lower(const std::string& str);

// Note: These functions don't internally handle any possible exception.
//  Calls to these functions should be contained in a try-catch block
void read_from_file(Trie& trie, const char* filename);
void read_from_file(Trie& trie, const std::string& filename);
void read_file_with_frequency(Trie& trie,
      std::unordered_map<std::string,std::size_t>& map,
      const std::string& filename,
      char separator = ' ');
std::istream& operator>>(std::istream& is, Trie& trie);

} /* utils */

#endif /* end of include guard: KEYBOARD_SWIPING_UTILS_H */
