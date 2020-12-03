// Juliana Pacheco
// University of Florida

#include "src/trie.h"

#include <cctype>
#include <functional>

using Node = Trie::Node;

const std::vector<std::function<bool(char)>> ALLOWED_CHAR_G
    // = { [](char c) -> bool { return (bool) std::isdigit((unsigned char) c); } };
    = {};
const std::vector<char> ALLOWED_CHAR_S = { '-' };

namespace {

template <typename T, class Container, class Compare>
bool contains(const Container& cont, const T& value, const Compare& comp) {
  for(auto it = cont.cbegin(); it != cont.cend(); ++it)
    if(comp(*it, value))
      return true;
  return false;
}

template <typename T, class Container>
bool contains(const Container& cont, const T& value) {
  return contains(cont, value, std::equal_to<T>());
}

bool is_allowable(char c) {
  return contains(ALLOWED_CHAR_G, c,
            [](std::function<bool(char)> f, char c) -> bool { return f(c); })
      || contains(ALLOWED_CHAR_S, c);
}

} /* anonymous */

Trie::Trie() : size_(0) {
  root_ = new Node();
}

Trie::Trie(const Trie& rhs) : Trie() {
  *this = rhs;
}

Trie& Trie::operator=(const Trie& rhs) {
  clear();
  *root_ = *rhs.root_;
  size_ = rhs.size_;
  return *this;
}

Trie::~Trie() {
  if(root_ != nullptr)
    delete root_;
}

bool Trie::empty() const { return size_ == 0; }

std::size_t Trie::size() const { return size_; }

void Trie::clear() {
  size_ = 0;
  if(root_ != nullptr)
    delete root_;
  root_ = new Node();
}

Trie::iterator Trie::insert(const std::string& word) {
  if(!word_is_valid(word))
    return end();

  Node* current = root_;
  char prev_c = (char) 0;
  char lower_c;
  for(char c : word) {
    if(!std::isalpha((unsigned char) c) || prev_c == c)
      continue;
    lower_c = std::tolower((unsigned char) c);
    current = current->insert_child(lower_c);
    prev_c = c;
  }
  size_++;
  current->insert_word(word);
  return iterator(current);
}

void Trie::erase(const std::string& word) {
  iterator match = find(word);
  if(match != end()) {
    match->remove_word(word);
    // TODO: if match is a leaf that is now empty, delete
  }
}

Trie::const_iterator Trie::find(const std::string& word) const {
  if(!word_is_valid(word))
    return cend();

  Node* current = root_;
  char prev_c = (char) 0;
  char lower_c;
  for(char c : word) {
    if(std::isalpha((unsigned char) c) && prev_c != c) {
      lower_c = std::tolower((unsigned char) c);
      if(current->get_child(lower_c) == nullptr)
        return cend();
      current = current->get_child(lower_c);
    }
    prev_c = c;
  }
  return const_iterator(current);
}

Trie::iterator Trie::find(const std::string& word) {
  const_iterator result = static_cast<const Trie&>(*this).find(word);
  return const_cast<Node*>(result.operator->());
}

bool Trie::contains(const std::string& word) const {
  return find(word) != cend();
}

bool Trie::word_is_valid(const std::string& word) {
  for(char c : word) {
    if(!std::isalpha((unsigned char) c) && !is_allowable(c))
      return false;
  }
  return true;
}

Node::Node(const Node& rhs) : Node() { *this = rhs; }

Node& Node::operator=(const Node& rhs) {
  words_ = rhs.words_;
  clear_children();
  rhs.do_on_children([this](char c, const Node* rhs_child) {
    if(rhs_child == nullptr)
      return;
    Node* new_child = insert_child(c);
    *new_child = *rhs_child;
  });
  return *this;
}

Node::~Node() {
  delete_children();
}

bool Node::contains_word(const std::string& word) const {
  return ::contains(words_, word);
}

void Node::remove_word(const std::string& word) {
  for(auto it = words_.begin(); it != words_.end(); ++it) {
    if(*it == word) {
      words_.erase(it);
      return;
    }
  }
}

bool Node::contains_child(char c) const {
  c = std::tolower((unsigned char) c);
  return children_.find(c) != children_.cend();
}

const Node* Node::get_child(char c) const {
  c = std::tolower((unsigned char) c);
  return contains_child(c) ? children_.at(c) : nullptr;
}

Node* Node::get_child(char c) {
  return const_cast<Node*>(static_cast<const Node&>(*this).get_child(c));
}

Node* Node::insert_child(char c) {
  c = std::tolower((unsigned char) c);

  Node* child = get_child(c);
  if(child == nullptr) {
    child = new Node();
    children_[c] = child;
  }
  return child;
}

void Node::do_on_children(const std::function<void(char,const Node*)>& func) const {
  for(auto cit = children_.cbegin(); cit != children_.cend(); ++cit)
    func(cit->first, cit->second);
}

void Node::do_on_children(const std::function<void(char,Node*)>& func) {
  for(auto it = children_.begin(); it != children_.end(); ++it)
    func(it->first, it->second);
}

void Node::delete_children() {
  do_on_children([](char, Node* child) {
    if(child != nullptr)
    delete child;
  });
}
