#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <map>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>

extern std::map<std::pair<int, int>, int> parsing_table;
extern std::map<int, std::vector<int>> productions;
extern std::unordered_map<std::string, int> tokenMap;
extern std::unordered_map<int, std::string> reverseTokenMap;
extern std::vector<char> quebras;
extern std::vector<char> quebraComplexo;

#endif // DEFINITIONS_H