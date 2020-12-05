// Juliana Pacheco
// University of Florida

#include "swipe_prediction.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <queue>
#include <utility>
#include "src/trie.h"
#include "src/utils.h"

Swipe::Swipe(const char* filename) {
  read_file_with_frequency(trie_, frequencies_, filename, ','); // CSV
}

Swipe::Swipe(const Trie& trie, const FrequencyMap& freq)
      : trie_(trie), frequencies_(freq) {
}

const std::vector<std::string>& Swipe::insert(const std::string& word,
      std::size_t frequency) {
  reset();
  std::string lower = std::move(word);
  frequencies_[lower] += frequency;
  return trie_.insert(lower)->get_words();
}

void Swipe::advance(const std::set<char>& candidate_letters) {
  if(candidate_letters.empty())
    return;

  std::set<char> letters;
  for(char c : candidate_letters)
    letters.insert(std::tolower((unsigned char) c));

  if(solution_space_.empty()) {
    for(char c : letters) {
      Trie::const_iterator root = trie_.cbegin();
      if(root->contains_child(c))
        solution_space_[c].insert(root->get_child(c));
    }
  } else {
    // Use of temporary storage avoids iterator invalidation
    std::queue<std::pair<char,const Trie::Node*>> to_add;

    for(auto it = solution_space_.cbegin(); it != solution_space_.cend(); ++it) {
      char characteristic_letter = it->first;
      for(const Trie::Node* node : it->second)
        for(char c : letters)
          if(c != characteristic_letter && node->contains_child(c))
            to_add.push({ c, node->get_child(c) });
    }

    while(!to_add.empty()) {
      char characteristic_letter = to_add.front().first;
      const Trie::Node* new_node = to_add.front().second;
      solution_space_[characteristic_letter].insert(new_node);
      to_add.pop();
    }
  }

  previous_letters_ = std::move(letters);
}

std::vector<std::string> Swipe::get(std::size_t max_suggestions) const {
  std::priority_queue<std::string,
                      std::vector<std::string>,
                      std::function<bool(const std::string&,const std::string&)>
        >heap([&](const std::string& s1, const std::string& s2) {
    std::string s1_lower = std::move(utils::to_lower(s1));
    std::string s2_lower = std::move(utils::to_lower(s2));

    std::size_t s1_freq = (frequencies_.find(s1_lower) == frequencies_.cend())
        ? 0 : frequencies_.at(s1_lower);
    std::size_t s2_freq = (frequencies_.find(s2_lower) == frequencies_.cend())
        ? 0 : frequencies_.at(s2_lower);

    return (s1_freq != s2_freq) ? (s1_freq < s2_freq) : (s1 < s2);
  });
  std::vector<std::string> suggestions;

  for(char c : previous_letters_) {
    if(solution_space_.find(c) != solution_space_.cend())
      for(const Trie::Node* node : solution_space_.at(c))
        for(const std::string& s : node->get_words())
          heap.push(s);
  }

  std::size_t out_size = std::min(max_suggestions, heap.size());
  for(std::size_t i = 0; i < out_size; i++) {
    suggestions.push_back(heap.top());
    heap.pop();
  }
  return suggestions;
}
