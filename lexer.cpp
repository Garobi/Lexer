#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

std::unordered_map<std::string, int> tokenMap = {
    {"while", 1}, {"var", 2}, {"to", 3}, {"then", 4}, {"string", 5},
    {"real", 6}, {"read", 7}, {"program", 8}, {"procedure", 9}, {"print", 10},
    {"nreal", 11}, {"nint", 12}, {"literal", 13}, {"integer", 14}, {"if", 15},
    {"ident", 16}, {"for", 17}, {"end", 18}, {"else", 19}, {"do", 20},
    {"const", 21}, {"begin", 22}, {"vstring", 23}, {">=", 24}, {">", 25},
    {"=", 26}, {"<>", 27}, {"<=", 28}, {"<", 29}, {"+", 30},
    {";", 31}, {":=", 32}, {":", 33}, {"/", 34}, {".", 35},
    {",", 36}, {"*", 37}, {")", 38}, {"(", 39}, {"{", 40},
    {"}", 41}, {"-", 42}, {"$", 43}, {"î", 44}
};

std::vector<char> quebras = { ' ', '{', '}', '(', ')', '+', ';', '/', '.', ',', '*', '-', '$', '\n', '\t'};
std::vector<char> quebraComplexo = { '>', '<', ':', '@', '!', '=' };
int linhaAtual = 1;

static bool contemAcento(const std::string& str) {
    for (unsigned char c : str) {
        if (c > 0x7A || c < 0x30) {
            return true;
        }
        if (c >= 0x3A && c <= 0x40)
        {
            return true;
        }
        if (c >= 0x5B && c <= 0x60)
        {
            return true;
        }
    }
    return false;
}

[[noreturn]] static void throwErroLexico(const std::string& message) {
    std::cerr << "Erro lexico na linha " << linhaAtual << ": " << message << std::endl;
    std::exit(EXIT_FAILURE);
}

static int tokenizador(std::string lexema, std::vector<std::string>& lexemasVector) {
    if (tokenMap.count(lexema))
    {
        lexemasVector.push_back(lexema);
        return tokenMap.at(lexema);
    }

    if (isalpha(lexema[0]) && islower(lexema[0]))
    {
        if (contemAcento(lexema))
        {
            throwErroLexico("Identificador contem caractere invalido");
        }
        if (lexema.size() > 30)
        {
            throwErroLexico("Identificador excede o numero de caracteres validos!");
        }
        lexemasVector.push_back("ident");
        return tokenMap.at("ident");
    }

    throwErroLexico("Token Invalido: " + lexema);
}

std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> lexer(std::vector<char> programa) {

    std::vector<char> lexema;
    std::vector<int> token, linha;
    std::vector<std::string> lexemas;
    std::string lexemaAtual;



    for (size_t i = 0; i < programa.size(); i++)
    {
        char caractereAtual = programa[i];

        // Para Comentários em linha

        if (caractereAtual == '@' && programa[i + 1] == '@')
        {
            while (i < programa.size() && programa[i] != '\n') i++;
            linhaAtual++;
            continue;;
        }

        // Para comentários em bloco

        if (caractereAtual == '@' && programa[i + 1] == '!')
        {
            i += 2;
            while (i + 1 < programa.size() && !(programa[i] == '!' && programa[i + 1] == '@')) {
                if (programa[i] == '\n') linhaAtual++;
                i++;
            }
            if (i + 1 >= programa.size())
            {
                throwErroLexico("Comentario de bloco nao fechado");
            }
            i += 1;
            continue;
        }

        // Para Strings

        if (caractereAtual == '"')
        {
            i++;
            while (i + 1 < programa.size() && !(programa[i] == '"')) i++;
            if (i + 1 >= programa.size()) throwErroLexico("String nao fechada");
            token.push_back(tokenMap.at("string"));
            lexemas.push_back("string");
            linha.push_back(linhaAtual);
            continue;
        }

        // Para Literal

        if (programa[i] == '\'')
        {
            i += 1;
            while (i + 1 < programa.size() && !(programa[i] == '\'')) i++;
            if (i + 1 >= programa.size()) throwErroLexico("Literal nao fechado");
            token.push_back(tokenMap.at("literal"));
            lexemas.push_back("literal");
            linha.push_back(linhaAtual);
            continue;
        }

        // Separador de lexema unico

        if (std::find(quebras.begin(), quebras.end(), caractereAtual) != quebras.end())
        {
            if (!lexemaAtual.empty())
            {
                if (isdigit(lexemaAtual[0]))
                {
                    // Trata número
                    bool rNum = false;
                    if (lexemaAtual.find_first_not_of("0123456789.") != std::string::npos)
                        throwErroLexico("Numero invalido com caracteres nao numericos");

                    if (std::count(lexemaAtual.begin(), lexemaAtual.end(), '.') > 1)
                        throwErroLexico("Numero real com dois ou mais pontos");

                    if (lexemaAtual.find('.') != std::string::npos)
                    {
                        if (std::stof(lexemaAtual) > 999999999) throwErroLexico("Numero real muito grande");
                        token.push_back(tokenMap.at("nreal"));
                        lexemas.push_back("nreal");
                    }
                    else
                    {
                        if (lexemaAtual.size() > 9) throwErroLexico("Numero inteiro muito grande");
                        token.push_back(tokenMap.at("nint"));
                        lexemas.push_back("nint");
                    }
                    linha.push_back(linhaAtual);
                }
                else
                {
                    token.push_back(tokenizador(lexemaAtual, lexemas));
                    linha.push_back(linhaAtual);
                }

                lexemaAtual.clear();
            }


            if (!isspace(caractereAtual))
            {
                token.push_back(tokenizador(std::string(1, caractereAtual), lexemas));
                linha.push_back(linhaAtual);
            }

            if (caractereAtual == '\n') linhaAtual++;
            continue;
        }

        // Separador complexo

        if (std::find(quebraComplexo.begin(), quebraComplexo.end(), caractereAtual) != quebraComplexo.end())
        {
            std::string caracterComplexo = std::string(1, caractereAtual);
            if (i + 1 < programa.size()) caracterComplexo += programa[i + 1];

            if (tokenMap.count(caracterComplexo))
            {
                token.push_back(tokenMap.at(caracterComplexo));
                lexemas.push_back(caracterComplexo);
                linha.push_back(linhaAtual);
                i++;
            }
            else
            {
                token.push_back(tokenizador(std::string(1, caractereAtual), lexemas));
                linha.push_back(linhaAtual);
            }
            continue;
        }

        lexemaAtual += caractereAtual;
    }

    if (!lexemaAtual.empty())
    {
        token.push_back(tokenizador(lexemaAtual, lexemas));
        linha.push_back(linhaAtual);
    }

    return std::make_tuple(lexemas, token, linha);
}