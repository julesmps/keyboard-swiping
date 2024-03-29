// Juliana Pacheco
// University of Florida

#include "src/trie.h"

#include <cctype>
#include <fstream>
#include <functional>
#include <limits>
#include <sstream>
#include <stack>
#include <utility>
#include "utils.h"

using Node = Trie::Node;

const std::vector<std::function<bool(char)>> ALLOWED_CHAR_G
    // = { [](char c) -> bool { return (bool) std::isdigit((unsigned char) c); } };
    = {};
const std::vector<char> ALLOWED_CHAR_S = { '-' };

namespace {

bool is_allowable(char c) {
  return utils::contains(ALLOWED_CHAR_G, c,
            [](std::function<bool(char)> f, char c) -> bool { return f(c); })
      || utils::contains(ALLOWED_CHAR_S, c);
}

std::pair<bool,char> find_key(const Node& node, const Node* child) {
  char key = 0;
  bool result = node.do_on_children_while([&](char c, const Node* n) -> bool {
    if(child == n) {
      key = c;
      return true;
    }
    return false;
  });
  return { result, key };
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

Trie::size_type Trie::size() const { return size_; }

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
  for(char c : word) {
    if(!std::isalpha((unsigned char) c) || prev_c == c)
      continue;
    c = std::tolower((unsigned char) c);
    if(current->contains_child(c))
      current = current->get_child(c);
    else
      current = current->insert_child(c);
    prev_c = c;
  }
  size_++;
  current->insert_word(word);
  return iterator(current);
}

Trie::iterator Trie::erase(const std::string& word) {
  std::stack<Node*> path;
  iterator match = const_cast<Node*>(find_common(word, [&](const Node* n) {
      path.push(const_cast<Node*>(n));
    }
  ).operator->());
  path.push(match.operator->());

  if(match != end()) {
    match->remove_word(word);
    size_--;
    if(match->has_words() || match->has_children())
      return match;

    // delete empty nodes to root
    while(!path.empty()&& !path.top()->has_words() && !path.top()->has_children()) {
      Node* to_del = path.top();
      path.pop();
      if(to_del == root_ || path.empty()) break; // don't delete root
      Node* parent = path.top();
      std::pair<bool,char> key = find_key(*parent, to_del);
      if(key.first)
        parent->remove_child(key.second);
    }
  }
  return end();
}

Trie::const_iterator Trie::find(const std::string& word) const {
  return find_common(word, [](const Node*) {});
}

Trie::iterator Trie::find(const std::string& word) {
  const_iterator result = static_cast<const Trie&>(*this).find(word);
  return const_cast<Node*>(result.operator->());
}

bool Trie::contains(const std::string& word) const {
  const_iterator result = find(word);
  if(result == cend())
    return false;
  return result->contains_word(word);
}

bool Trie::word_is_valid(const std::string& word) {
  for(char c : word) {
    if(!std::isalpha((unsigned char) c) && !is_allowable(c))
      return false;
  }
  return true;
}

Trie::const_iterator Trie::find_common(const std::string& word, std::function<void(const Node*)> func) const {
  if(!word_is_valid(word))
    return cend();

  Node* current = root_;
  char prev_c = (char) 0;
  for(char c : word) {
    if(std::isalpha((unsigned char) c) && prev_c != c) {
      c = std::tolower((unsigned char) c);
      if(current->get_child(c) == nullptr)
        return cend();
      func(current);
      current = current->get_child(c);
      prev_c = c;
    }
  }
  return const_iterator(current);
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
  return utils::contains(words_, word);
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

void Node::remove_child(char c) {
  auto child = children_.find(c);
  if(child != children_.end()) {
    if(child->second != nullptr)
      delete child->second;
    children_.erase(child);
  }
}

void Node::do_on_children(const std::function<void(char,const Node*)>& func) const {
  for(auto cit = children_.cbegin(); cit != children_.cend(); ++cit)
    func(cit->first, cit->second);
}

bool Node::do_on_children_while(const std::function<bool(char,const Node*)>& func) const {
  for(auto cit = children_.cbegin(); cit != children_.cend(); ++cit)
    if(func(cit->first, cit->second))
      return true;
  return false;
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
      const char* filename,
      char separator,
      bool has_header) {
  std::ifstream is(filename);
  std::string line, word, freq;
  std::istringstream ls;

  if(has_header)
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  while(std::getline(is, line)) {
    if(line.empty())
      continue;
    ls = std::istringstream(line);

    std::getline(ls, word, separator);
    utils::to_lower(word);
    trie.insert(word);

    std::getline(ls, freq);
    map[word] += std::stoul(freq);
  }
  is.close();
}

void read_file_with_frequency(Trie& trie,
      std::unordered_map<std::string,std::size_t>& map,
      const std::string& filename,
      char separator,
      bool has_header) {
  read_file_with_frequency(trie, map, filename.c_str(), separator, has_header);
}

std::istream& operator>>(std::istream& is, Trie& trie) {
  std::string line;
  is >> line;
  trie.insert(line);
  return is;
}
