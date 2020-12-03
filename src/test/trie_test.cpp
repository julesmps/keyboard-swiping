// Juliana Pacheco
// University of Florida

#include "src/trie.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>

testing::AssertionResult contains(const Trie& t, const std::string& s) {
  if(t.contains(s))
    return testing::AssertionSuccess() << "Trie contains word '" << s << '\'';
  else
    return testing::AssertionFailure() << "Trie doesn't contain word '" << s << '\'';
}

TEST(TrieTest, CreateEmpty) {
  Trie trie;
  EXPECT_TRUE(trie.empty());
}

TEST(TrieTest, CBegin) {
  Trie trie;
  Trie::const_iterator it = trie.cbegin();
  if(it != trie.cend()) {
    EXPECT_FALSE(it->has_words());
    EXPECT_FALSE(it->has_children());
  }
}

TEST(TrieTest, InsertSingle) {
  Trie trie;
  Trie::iterator result = trie.insert("a");
  EXPECT_NE(result, trie.end());
  EXPECT_TRUE(contains(trie, "a"));
  Trie::const_iterator cit = trie.cbegin();
  ASSERT_NE(cit, trie.cend());
  EXPECT_TRUE(cit->has_children());
  EXPECT_EQ(result, cit['a']);
}

class TrieSmallTest : public testing::Test {
protected:
  void SetUp() override {
    for(const std::string& s : init_list)
      trie.insert(s);
  };

  Trie trie;
  static const std::vector<std::string> init_list;
};
const std::vector<std::string> TrieSmallTest::init_list
    = { "a", "I", "in", "inn", "an", "ant", "any", "nose", "no" };

TEST_F(TrieSmallTest, ContainsAllEntries) {
  for(const std::string& s : init_list)
    EXPECT_TRUE(contains(trie, s));
}

TEST_F(TrieSmallTest, DoesNotContainEntries) {
  const std::vector<std::string> nin
      = { "dog", "castle", "rope", "inward", "not", "dot" };
  for(const std::string& s : nin)
    EXPECT_FALSE(contains(trie, s));
}

TEST_F(TrieSmallTest, RepeatedLettersTogether) {
  const Trie& const_trie = trie;
  Trie::const_iterator in = const_trie.find("in");
  EXPECT_NE(in, const_trie.cend());
  EXPECT_EQ(in, const_trie.find("inn"));
}

TEST_F(TrieSmallTest, ClearTrie) {
  trie.clear();
  EXPECT_TRUE(trie.empty());
  Trie::const_iterator it = trie.cbegin();
  if(it != trie.cend()) {
    EXPECT_FALSE(it->has_words());
    EXPECT_FALSE(it->has_children());
  }
}

TEST_F(TrieSmallTest, TraverseToAnt) {
  const Trie& const_trie = trie;
  Trie::const_iterator iter = const_trie.cbegin();
  const std::string target = "ant";
  for(char c : target) {
    ASSERT_NE(iter, const_trie.cend());
    EXPECT_FALSE(iter->contains_word(target));
    iter = iter[c];
  }
  ASSERT_NE(iter, const_trie.cend());
  EXPECT_TRUE(contains(trie, target));
}
