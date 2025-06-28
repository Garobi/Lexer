#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <utility>
#include "definitions.h"
#include "semantic.h"

constexpr int tokenEOF = 43;
constexpr int tokenStart = 45;
constexpr int EPSILON = 44;

bool parser(std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> respostaLexer) {
    std::vector<std::string> lexemas = std::get<0>(respostaLexer);
    std::vector<int> tokens = std::get<1>(respostaLexer);
    std::vector<int> linhas = std::get<2>(respostaLexer);

    std::stack<int> stackParser;
    stackParser.push(tokenEOF);
    stackParser.push(tokenStart);

    lexemas.push_back("$");
    tokens.push_back(tokenEOF);
    linhas.push_back(linhas.back());
    size_t inputIndex = 0;

    while (!stackParser.empty()) {
        std::cout << "Pilha: " << std::endl;
        std::stack<int> tempStack = stackParser;
        while (!tempStack.empty()) {
            std::cout << reverseTokenMap.at(tempStack.top()) << std::endl;
            tempStack.pop();
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        int top = stackParser.top();
        stackParser.pop();

        if (top == 64) {
            std::cout << "teste";
        }

        if (top == EPSILON) {
            continue;
        }

        if (inputIndex >= tokens.size()) {
            std::cerr << "Erro: Fim dos tokens antes do esperado." << std::endl;
            return false;
        }

        std::string currentLexema = lexemas[inputIndex];
        int currentToken = tokens[inputIndex];
        int currentLinha = linhas[inputIndex];

        if (top < tokenStart) {
            if (top == currentToken) {
                inputIndex++;
            }
            else {
                std::cerr << "Erro sintatico na linha " << linhas[inputIndex]
                    << ": token inesperado '" << lexemas[inputIndex]
                        << "' (esperado token '" << reverseTokenMap.at(top) << "', encontrado token '"
                            << reverseTokenMap.at(currentToken) << "')." << std::endl;
                        return false;
            }
        }
        else {
            auto it = parsing_table.find({ top, currentToken });
            if (it != parsing_table.end()) {
                const auto& production = productions[it->second];
                for (auto itProd = production.rbegin(); itProd != production.rend(); ++itProd) {
                    if (*itProd != EPSILON) {
                        stackParser.push(*itProd);
                    }
                }
            }
            else {
                std::cerr << "Erro sintatico na linha " << linhas[inputIndex]
                    << ": nenhuma producao encontrada para [nao-terminal " << reverseTokenMap.at(top)
                        << ", token " << reverseTokenMap.at(currentToken) << "] (lexema '" << lexemas[inputIndex] << "')." << std::endl;
                    return false;
            }
        }
    }

    if (inputIndex == tokens.size() && tokens[inputIndex - 1] == tokenEOF) {
        return true;
    }
    else {
        std::cerr << "Erro: Parsing incompleto. Tokens restantes apos analise." << std::endl;
        return false;
    }
}
