// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_TRIE_H
#define KEYBOARD_SWIPING_TRIE_H

#include <functional>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class Trie {
protected:
  template <typename T>
  class NodeIterator {
  public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::forward_iterator_tag;

    NodeIterator(pointer ptr = nullptr) : ptr_(ptr) {}
    NodeIterator(const NodeIterator<T>&) = default;
    ~NodeIterator(){}
    NodeIterator<T>& operator=(const NodeIterator<T>&) = default;
    NodeIterator<T>& operator=(pointer ptr) { ptr_=ptr; return *this; }

    operator bool() const { return ptr_; }
    bool operator==(const NodeIterator<T>& rhs) const { return ptr_ == rhs.ptr_; }
    bool operator!=(const NodeIterator<T>& rhs) const { return ptr_ != rhs.ptr_; }
    NodeIterator operator[](char c) const { return NodeIterator<T>(ptr_->get_child(c)); }
    value_type& operator*() { return *ptr_; }
    const value_type& operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }

  protected:
    pointer ptr_;
  };

public:
  class Node;
  typedef NodeIterator<Node> iterator;
  typedef NodeIterator<const Node> const_iterator;
  typedef std::size_t size_type;

  Trie();
  Trie(const Trie& rhs);
  Trie& operator=(const Trie& rhs);
  ~Trie();

  bool empty() const;
  size_type size() const;

  iterator begin() { return iterator(root_); }
  const_iterator cbegin() const { return const_iterator(root_); }
  iterator end() { return iterator(); }
  const_iterator cend() const { return const_iterator(); }

  void clear();
  iterator insert(const std::string& word);
  iterator erase(const std::string& word);

  const_iterator find(const std::string& word) const;
  iterator find(const std::string& word);
  bool contains(const std::string& word) const;

private:
  static bool word_is_valid(const std::string& word);
  const_iterator find_common(const std::string& word,
        std::function<void(const Node*)> func) const;

  Node* root_;
  size_type size_;
};

class Trie::Node {
public:
  Node() = default;
  Node(const Node& rhs);
  Node& operator=(const Node& rhs);
  ~Node();

  void clear() { clear_words(); clear_children(); }

  bool has_words() const { return !words_.empty(); }
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
  void remove_child(char c);
  void clear_children() { delete_children(); children_.clear(); }
  void do_on_children(const std::function<void(char,const Node*)>& func) const;
  void do_on_children(const std::function<void(char,Node*)>& func);
  bool do_on_children_while(const std::function<bool(char,const Node*)>& func) const;

private:
  void delete_children();

  std::vector<std::string> words_;
  std::map<char,Node*> children_;
};


// Note: These functions don't internally handle any possible exception.
//  Calls to these functions should be contained in a try-catch block
void read_from_file(Trie& trie, const char* filename);
void read_from_file(Trie& trie, const std::string& filename);
void read_file_with_frequency(Trie& trie,
      std::unordered_map<std::string,std::size_t>& map,
      const std::string& filename,
      char separator = ' ');
std::istream& operator>>(std::istream& is, Trie& trie);

#endif /* end of include guard: KEYBOARD_SWIPING_TRIE_H */
