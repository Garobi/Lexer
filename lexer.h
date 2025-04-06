#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <tuple>

std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> lexer(std::vector<char>programa);

#endif // !LEXER_H
