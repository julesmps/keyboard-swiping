// Juliana Pacheco
// University of Florida

#include "src/utils.h"

#include <cctype>
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

} /* utils */
