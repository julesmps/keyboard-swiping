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

TEST_F(TrieSmallTest, RemoveChildNoSiblings) {
  std::string target = "nose";
  Trie::size_type size = trie.size();
  ASSERT_NE(size, 0);
  ASSERT_TRUE(contains(trie, target));
  Trie::iterator no = trie.find("no");
  ASSERT_NE(no, trie.end());
  EXPECT_TRUE(no->has_children());
  EXPECT_EQ(trie.erase(target), trie.end());
  EXPECT_FALSE(no->has_children());
  EXPECT_FALSE(contains(trie, target));
  EXPECT_EQ(trie.size(), size-1);
}

TEST_F(TrieSmallTest, RemoveChildHasSiblings) {
  std::string target = "ant";
  Trie::size_type size = trie.size();
  ASSERT_NE(size, 0);
  ASSERT_TRUE(contains(trie, target));
  EXPECT_TRUE(contains(trie, "any"));
  EXPECT_TRUE(contains(trie, "an"));
  EXPECT_EQ(trie.erase(target), trie.end());
  EXPECT_FALSE(contains(trie, target));
  EXPECT_TRUE(contains(trie, "any"));
  EXPECT_TRUE(contains(trie, "an"));
  EXPECT_EQ(trie.size(), size-1);
}

TEST_F(TrieSmallTest, RemoveOtherWords) {
  std::string target = "in";
  std::string twin = "inn";
  Trie::size_type size = trie.size();
  ASSERT_NE(size, 0);
  ASSERT_TRUE(contains(trie, target));
  ASSERT_TRUE(contains(trie, twin));
  Trie::iterator it = trie.find(target);
  ASSERT_NE(it, trie.end());
  EXPECT_EQ(trie.find(twin), it);
  ASSERT_EQ(trie.erase(target), it);
  EXPECT_FALSE(contains(trie, target));
  EXPECT_TRUE(contains(trie, twin));
  EXPECT_TRUE(it->contains_word(twin));
  EXPECT_EQ(trie.size(), size-1);
}

TEST_F(TrieSmallTest, RemoveNonLeaf) {
  std::string target = "no";
  std::string child = "nose";
  Trie::size_type size = trie.size();
  ASSERT_NE(size, 0);
  ASSERT_TRUE(contains(trie, target));
  ASSERT_TRUE(contains(trie, child));
  EXPECT_NE(trie.erase(target), trie.end());
  EXPECT_FALSE(contains(trie, target));
  EXPECT_TRUE(contains(trie, child));
  EXPECT_EQ(trie.size(), size-1);
}

TEST_F(TrieSmallTest, RemoveNonElement) {
  const std::vector<std::string> nin
      = { "dog", "castle", "rope", "inward", "not", "dot" };
  for(const std::string& s : nin) {
    Trie::size_type size = trie.size();
    EXPECT_EQ(trie.erase(s), trie.end());
    EXPECT_EQ(trie.size(), size);
  }
}
