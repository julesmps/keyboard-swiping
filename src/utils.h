// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_UTILS_H
#define KEYBOARD_SWIPING_UTILS_H

#include <istream>
#include <string>
#include "src/trie.h"

bool read_from_file(Trie& trie, const char* filename);
bool read_from_file(Trie& trie, const std::string& filename);
std::istream& operator>>(std::istream& is, Trie& trie);

#endif /* end of include guard: KEYBOARD_SWIPING_UTILS_H */
