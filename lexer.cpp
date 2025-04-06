#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

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

bool contemAcento(const std::string& str) {
    for (unsigned char c : str) {
        if (c >= 128) {
            return true;
        }
    }
    return false;
}

static int tokenizador(std::string lexema, std::vector<int>* tokenVector, std::vector<std::string>* lexemasVector) {
    std::vector<int> tokens = *tokenVector;
    std::vector<std::string> lexemas = *lexemasVector;
    int token = -1;
    try
    {
        token = tokenMap.at(lexema);
        lexemasVector->push_back(lexema);
    }
    catch (const std::exception&)
    {
        if (isalpha(lexema[0]) && islower(lexema[0]))
        {
            if (contemAcento(lexema))
            {
                
                std::cerr << "Erro lexico: ident contem caractere inválido na linha " << linhaAtual;;
                exit(EXIT_FAILURE);
            }
            else
            {
                token = tokenMap.at("ident");
                lexemasVector->push_back("ident");
            }
        }
        else
        {
            std::cerr << "Erro lexico: Token Inválido na linha " << linhaAtual;
            exit(EXIT_FAILURE);
        }
    }

    return token;
}

std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> lexer(std::vector<char> programa) {

    std::vector<char> lexema;
    std::vector<int> token, linha;
    std::vector<std::string> lexemas;
    std::string lexemaAtual;



    for (size_t i = 0; i < programa.size(); i++)
    {

        if (programa[i] == 'b')
        {
            std::cout << "teste";
        }

        // Para Comentários em linha

        if (programa[i] == '@' && programa[i + 1] == '@')
        {
            while (i < programa.size() && programa[i] != '\n') i++;
            continue;;
        }

        // Para comentários em bloco

        if (programa[i] == '@' && programa[i + 1] == '!')
        {
            i += 2;
            while (i + 1 < programa.size() && !(programa[i] == '!' && programa[i + 1] == '@')) i++;
            if (i + 1 >= programa.size())
            {
                std::cerr << "Erro lexico: comentário de bloco não fechado na linha " << linhaAtual;
                exit(EXIT_FAILURE);
                break;
            }
            i += 1;
            continue;
        }

        // Para Strings

        if (programa[i] == '"')
        {
            while (i + 1 < programa.size() && !(programa[i] == '"')) i++;
            if (i + 1 >= programa.size())
            {
                std::cerr << "Erro lexico: Bloco de string não fechado na linha" << linhaAtual;
                exit(EXIT_FAILURE);

                break;
            }
            i += 1;
            continue;
            token.push_back(tokenizador("string", &token, &lexemas));
            linha.push_back(linhaAtual);
        }

        // Para Literal

        if (programa[i] == '\'')
        {
            i += 1;
            while (i + 1 < programa.size() && !(programa[i] == '\'')) i++;
            if (i + 1 >= programa.size())
            {
                std::cerr << "Erro lexico: Bloco literal não fechado na linha " << linhaAtual;
                exit(EXIT_FAILURE);

                break;
            }
            i++;
            token.push_back(tokenizador("literal", &token, &lexemas));
            linha.push_back(linhaAtual);
        }

        // Para Números

        if (isdigit(programa[i]))
        {
            bool rNum = false;
            std::string numeroAtual;
            while (i + 1 < programa.size() && (programa[i] == '.' || isdigit(programa[i]))) {
                if (programa[i] == '.')
                {
                    if (rNum == true)
                    {
                        std::cerr << "Erro Lexico: Real não pode conter dois ou mais pontos. Linha " << linhaAtual;
                        exit(EXIT_FAILURE);
                    }
                    rNum = true;
                }
                numeroAtual.push_back(programa[i]);
                i++;
            }
            if (rNum == true)
            {
                if (std::stof(numeroAtual) < 999999999)
                {
                    token.push_back(tokenizador("nreal", &token, &lexemas));
                    linha.push_back(linhaAtual);
                }
                else
                {
                    std::cerr << "Erro Lexico: Número real maior que o permitido na linha " << linhaAtual;
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                float teste = std::stoi(numeroAtual);
                if (std::stoi(numeroAtual) < 999999999)
                {
                    token.push_back(tokenizador("nint", &token, &lexemas));
                    linha.push_back(linhaAtual);
                }
                else
                {
                    std::cerr << "Erro Lexico: Número inteiro maior que o permitido na linha " << linhaAtual;
                    exit(EXIT_FAILURE);
                }
            }
        }

        // Início da análise lexica

        if (std::find(quebras.begin(), quebras.end(), programa[i]) == quebras.end())
        {
            if (std::find(quebraComplexo.begin(), quebraComplexo.end(), programa[i]) == quebraComplexo.end())
            {
                if (programa[i] != ' ' && programa[i] != '\t')
                {
                    //Insere caracter no lexema
                    lexema.push_back(programa[i]);
                }
            }
            else if (std::find(quebraComplexo.begin(), quebraComplexo.end(), programa[i + 1]) != quebraComplexo.end())
            {
                //Insere o token de quebra de palavra com multiplos caracteres na lista de lexema
                token.push_back(tokenizador(std::string() + programa[i] + programa[i + 1], &token, &lexemas));
                linha.push_back(linhaAtual);
                i += 1;
            }
            else
            {
                //Insere o token de quebra de palavra com um caracter na lista de lexema
                token.push_back(tokenizador(std::string{ programa[i] }, &token, &lexemas));
                linha.push_back(linhaAtual);
            }
        }
        else
        {
            if (lexema.size() > 0)
            {
                for (size_t i = 0; i < lexema.size(); i++)
                {
                    lexemaAtual.push_back(lexema[i]);
                }

                // Para tokens comuns

                token.push_back(tokenizador(lexemaAtual, &token, &lexemas));
                linha.push_back(linhaAtual);
                lexemaAtual.clear();
            }
            if (programa[i] != ' ' && programa[i] != '\n' && programa[i] != '\t')
            {

                // Para quebra de linha

                token.push_back(tokenizador(std::string{ programa[i] }, &token, &lexemas));
                linha.push_back(linhaAtual);
            }
            lexema.clear();
        }

        if (programa[i] == '\n') linhaAtual++;

    }
    return std::make_tuple(lexemas, token, linha);
}