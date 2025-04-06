#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <iomanip>
#include "lexer.h"

void printLexer(std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> respostaLexer) {
    std::vector<std::string> lexemas = std::get<0>(respostaLexer);
    std::vector<int> tokens = std::get<1>(respostaLexer);
    std::vector<int> linhas = std::get<2>(respostaLexer);

    std::cout << std::left << std::setw(16) << "Lexema"
        << std::setw(16) << "Token"
        << std::setw(8) << "Linha" << "\n";
    std::cout << "-----------------------------------------------\n";

    for (size_t i = 0; i < lexemas.size(); ++i) {
        std::cout << std::left << std::setw(16) << lexemas[i]
            << std::setw(16) << tokens[i]
                << std::setw(8) << linhas[i] << "\n";
    }
}

int main(int argc, char const* argv[]) {

    char programaCh;
    std::vector<char> programaVector;
    std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> respostaLexer;

    if (argc < 2) {
        std::cerr << "Aponte um arquivo para a leitura!" << std::endl;
        return 1;
    }
    else if (argc > 2) {
        std::cerr << "Multiplos argumentos apontados." << std::endl;
        return 1;
    }

    //std::ifstream file(argv[1]);
    std::ifstream programa("D:\\Dev\\CLexer\\Lexer\\teste.txt");
    if (!programa) {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
    }

    while (programa.get(programaCh)) {
        programaVector.push_back(programaCh);
    }

    respostaLexer = lexer(programaVector);

    printLexer(respostaLexer);

    programa.close();

	return 0;
}
