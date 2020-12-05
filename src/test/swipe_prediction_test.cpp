// Juliana Pacheco
// University of Florida

#include "src/swipe_prediction.h"
#include "gtest/gtest.h"

#include <string>
#include <utility>
#include <vector>

using source_type = std::vector<std::pair<std::string, std::size_t>>;
using input_type = std::vector<std::set<char>>;
using test_param_type = std::pair<input_type, std::string>;

testing::AssertionResult contains(const Swipe& obj, const std::string& w) {
  if(obj.contains(w))
    return testing::AssertionSuccess() << "Swipe contains word '" << w << '\'';
  else
    return testing::AssertionFailure() << "Swipe doesn't contain word '" << w << '\'';
}

template <typename T>
testing::AssertionResult contains(const std::vector<T>& vect, const T& q) {
  bool found = false;
  for(auto iter = vect.cbegin(); iter != vect.cend(); ++iter)
    if(*iter == q) {
      found = true;
      break;
    }
  if(found)
    return testing::AssertionSuccess() << "Vector contains item '" << q << '\'';
  else
    return testing::AssertionFailure() << "Vector doesn't contain item'" << q << '\'';
}

const source_type init_list = {
  {"test",      350 },
  {"pizza",     982 },
  {"pasta",     953 },
  {"find",      512 },
  {"fiend",      42 },
  {"friend",    477 },
  {"utility",    98 },
  {"page",      105 },
  {"book",       87 },
  {"fund",       66 },
  {"map",       345 },
  {"geography",  53 },
  {"train",     612 },
  {"teach",     214 }
};
Swipe swipe(init_list.cbegin(), init_list.cend());

const std::vector<test_param_type> params = {
  { { { 'T' }, { 'R', 'T' }, { 'R' }, { 'E', 'R' }, { 'E' }, { 'E', 'S', 'D' },
      { 'W', 'E', 'S', 'D' }, { 'S', 'D' }, { 'D' }, { 'D', 'F' }, { 'F' },
      { 'R', 'T', 'F' }, { 'R', 'T', 'F', 'G' }, { 'T', 'F', 'G' }, { 'T' }
  }, "test" },
  { { { 'F' }, { 'R', 'T', 'F' }, { 'R', 'F' }, { 'R' }, { 'R', 'T' }, { 'T' },
      { 'T', 'Y' }, { 'Y' }, { 'Y', 'U' }, { 'U' }, { 'U', 'I' }, { 'U' },
      { 'Y', 'U' }, { 'Y' }, { 'T', 'Y' }, { 'T' }, { 'R', 'T' }, { 'R' },
      { 'E', 'R' }, { 'E' }, { 'E', 'R' }, { 'R', 'D', 'F' }, { 'R', 'F' },
      { 'F' }, { 'F', 'G' }, { 'G', 'V' }, { 'G', 'V', 'B' },
      { 'G', 'H', 'V', 'B' }, { 'G', 'H', 'B' }, { 'B' }, { 'B', 'N' }, { 'B' },
      { 'V', 'B' }, { 'G', 'V', 'B' }, { 'F', 'G', 'V' }, { 'F', 'G', 'C', 'V' },
      { 'F', 'C', 'V' }, { 'F', 'C' }, { 'D', 'F', 'C' }, { 'D', 'F', 'X', 'C' },
      { 'D', 'X', 'C' }, { 'D' }
  }, "friend" },
  { { { 'P' }, { 'O', 'P' }, { 'O' }, { 'I', 'O' }, { 'I' }, { 'U', 'I' },
      { 'U' }, { 'Y', 'U', 'H', 'J' }, { 'Y', 'H' }, { 'Y', 'G', 'H' },
      { 'T', 'Y', 'G', 'H' }, { 'T', 'Y', 'G' }, { 'T', 'G' }, { 'T', 'F', 'G' },
      { 'R', 'T', 'F', 'G' }, { 'R', 'T', 'F' }, { 'R', 'F' }, { 'R', 'D', 'F' },
      { 'E', 'R', 'D', 'F' }, { 'E', 'R', 'D' }, { 'E', 'D' }, { 'E', 'S', 'D' },
      { 'W', 'E', 'S', 'D' }, { 'W', 'E', 'S' }, { 'W', 'S' }, { 'W', 'A', 'S' },
      { 'Q', 'W', 'A', 'S' }, { 'Q', 'W', 'A' }, { 'A' }, { 'A', 'S' }, { 'S' },
      { 'S', 'D' }, { 'D' }, { 'D', 'F' }, { 'E', 'R', 'D', 'F' },
      { 'R', 'D', 'F' }, { 'R', 'F' }, { 'R', 'T', 'F' }, { 'R', 'T', 'F', 'G' },
      { 'T', 'F', 'G' }, { 'T' }, { 'R', 'T' }, { 'R' }, { 'R', 'D', 'F' },
      { 'E', 'R', 'D', 'F' }, { 'E', 'R', 'D' }, { 'E', 'D' }, { 'E', 'S', 'D' },
      { 'W', 'E', 'S', 'D' }, { 'S' }, { 'A', 'S' }, { 'A' }
  }, "pasta" },
  { { { 'M' }, { 'N', 'M' }, { 'N' }, { 'B', 'N' }, { 'B' }, { 'V', 'B' },
      { 'V' }, { 'C', 'V' }, { 'C' }, { 'X', 'C' }, { 'D', 'X', 'C' },
      { 'D', 'X' }, { 'S', 'D', 'X' }, { 'S', 'D', 'Z', 'X' }, { 'S', 'Z', 'X' },
      { 'S', 'Z' }, { 'A', 'S', 'Z' }, { 'A', 'S' }, { 'A' }, { 'A', 'S' },
      { 'S' }, { 'S', 'D' }, { 'D' }, { 'D', 'F' }, { 'F' }, { 'R', 'F' },
      { 'R', 'T', 'F' }, { 'R', 'T', 'F', 'G' }, { 'T', 'F', 'G' }, { 'T', 'G' },
      { 'T', 'Y', 'G' }, { 'T', 'Y', 'G', 'H' }, { 'Y', 'G', 'H' }, { 'Y', 'H' },
      { 'Y', 'U', 'H' }, { 'Y', 'U', 'H', 'J' }, { 'U', 'H', 'J' }, { 'U', 'J' },
      { 'U', 'I', 'J' }, { 'U', 'I', 'J', 'K' }, { 'I', 'J', 'K' }, { 'I', 'K' },
      { 'I', 'O', 'K' }, { 'I', 'O', 'K', 'L' }, { 'O', 'K', 'L' }, { 'O', 'L' },
      { 'O', 'P', 'L' }, { 'P', 'L' }, { 'P' }
  }, "map" }
};

class SwipePredictionTest : public testing::TestWithParam<test_param_type> {};

TEST_P(SwipePredictionTest, SuggestionsContains) {
  ASSERT_TRUE(contains(swipe, GetParam().second));
  swipe.reset();
  const input_type& input = GetParam().first;

  for(const std::set<char>& keys : input)
    swipe.advance(keys);

  EXPECT_TRUE(contains(swipe.get(), GetParam().second));
}

INSTANTIATE_TEST_CASE_P(PredictionMatch, SwipePredictionTest, testing::ValuesIn(params));
