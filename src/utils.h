// Juliana Pacheco
// University of Florida

#ifndef KEYBOARD_SWIPING_UTILS_H
#define KEYBOARD_SWIPING_UTILS_H

#include <functional>
#include <string>

namespace utils {

  void to_lower(std::string& str);
  std::string to_lower(const std::string& str);

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
