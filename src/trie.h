// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_TRIE_H
#define KEYBOARD_SWIPING_TRIE_H

#include <functional>
#include <map>
#include <string>
#include <vector>

class Trie {
public:
  class Node;
  Trie();
  Trie(const Trie& rhs);
  Trie& operator=(const Trie& rhs);
  ~Trie();

  bool empty() const;
  std::size_t size() const;

  void clear();
  Node* insert(const std::string& word);
  void erase(const std::string& word);

  const Node* find(const std::string& word) const;
  Node* find(const std::string& word);
  bool contains(const std::string& word) const;

private:
  static bool word_is_valid(const std::string& word);

  Node* root_;
  std::size_t size_;
};

class Trie::Node {
public:
  Node() = default;
  Node(const Node& rhs);
  Node& operator=(const Node& rhs);
  ~Node();

  void clear() { clear_words(); clear_children(); }

  bool contains_word(const std::string& word) const;
  void insert_word(const std::string& word) { words_.push_back(word); }
  void remove_word(const std::string& word);
  void clear_words() { words_.clear(); }
  const std::vector<std::string>& get_words() const { return words_; }

  bool has_children() const { return !children_.empty(); }
  bool contains_child(char c) const;
  const Node* get_child(char c) const;
  Node* get_child(char c);
  Node* insert_child(char c);
  void clear_children() { delete_children(); children_.clear(); }
  void do_on_children(const std::function<void(char,const Node*)>& func) const;
  void do_on_children(const std::function<void(char,Node*)>& func);

private:
  void delete_children();

  std::vector<std::string> words_;
  std::map<char,Node*> children_;
};

#endif /* end of include guard: KEYBOARD_SWIPING_TRIE_H */
