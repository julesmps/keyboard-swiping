// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_UTILS_H
#define KEYBOARD_SWIPING_UTILS_H

#include <cctype>
#include <functional>
#include <string>

namespace utils {

  void to_lower(std::string& str) {
    for(auto iter = str.begin(); iter != str.end(); ++iter)
      *iter = (unsigned char) std::tolower((unsigned char) *iter);
  }

  std::string to_lower(const std::string& str) {
    std::string result;
    for(char c : str)
      result += (unsigned char) std::tolower((unsigned char) c);
    return result;
  }

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

} /* utils */

#endif /* end of include guard: KEYBOARD_SWIPING_UTILS_H */
