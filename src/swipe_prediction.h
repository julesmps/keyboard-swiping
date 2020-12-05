// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_SWIPE_PREDICTION_H
#define KEYBOARD_SWIPING_SWIPE_PREDICTION_H

#include <limits>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "src/trie.h"

using FrequencyMap = std::unordered_map<std::string, std::size_t>;

class Swipe {
public:
  Swipe(const std::string& filename);
  template <class InputIt> Swipe(InputIt begin, InputIt end);
  Swipe(const Trie& trie, const FrequencyMap& freq);

  const std::vector<std::string>& insert(const std::string& word, std::size_t frequency);
  bool contains(const std::string& word) const { return trie_.contains(word); }

  void reset() { solution_space_.clear(); previous_letters_.clear(); }
  void advance(const std::set<char>& candidate_letters);
  std::vector<std::string> get(std::size_t max_suggestions
        = std::numeric_limits<std::size_t>::max()) const;

private:
  Trie trie_;
  FrequencyMap frequencies_;
  std::map<char, std::unordered_set<const Trie::Node*>> solution_space_;
  std::set<char> previous_letters_;
};

// Object pointed to by InputIt must have the following accessors:
//    | first (type std::string)
//    | second (type unsigned int)
template <class InputIt>
Swipe::Swipe(InputIt begin, InputIt end) {
  for(InputIt iter = begin; iter != end; ++iter) {
    trie_.insert(iter->first);
    frequencies_[iter->first] = iter->second;
  }
}

#endif /* end of include guard: KEYBOARD_SWIPING_SWIPE_PREDICTION_H */
