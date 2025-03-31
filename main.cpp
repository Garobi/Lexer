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
    {"}", 41}, {"-", 42}, {"$", 43}, {"�", 44}
};

std::vector<char> quebras = { ' ', '{', '}', '(', ')', '+', ';', '/', '.', ',', '*', '-', '$', '\n' };
std::vector<char> quebraComplexo = { '>', '<', ':', '@', '!', '=' };

static int tokenizador(std::string lexema, std::vector<int> *tokenVector) {
    std::vector<int> tokens = *tokenVector;
    int token = -1;
    try
    {
        token = tokenMap.at(lexema);
    }
    catch (const std::exception&)
    {
        if (isalpha(lexema[0]) && islower(lexema[0]))
        {
            switch (tokens.back()) {
            case 2:
            case 8:
            case 21:
                token = tokenMap.at("ident");
                break;
            }
        }
    }
    
    return token;
}

std::tuple<std::vector<std::string>, std::vector<char>> lexer(std::vector<char> programa) {

    std::vector<char> lexema;
    std::vector<int> token;
    std::vector<std::string> lexemas;
    std::string lexemaAtual;

    for (size_t i = 0; i < programa.size(); i++)
    {

        // Para Coment�rios em linha

        if (programa[i] == '@' && programa[i+1] == '@')
        {
            while (i < programa.size() && programa[i] != '\n') i++;
            continue;;
        }

        // Para coment�rios em bloco

        if (programa[i] == '@' && programa[i + 1] == '!')
        {
            i += 2;
            while (i + 1 < programa.size() && !(programa[i] == '!' && programa[i + 1] == '@')) i++;
            if (i + 1 >= programa.size())
            {
                std::cerr << "Erro lexico: coment�rio de bloco n�o fechado" << std::endl;
                break;
            }
            i += 1;
            continue;
        }

        if (programa[i] == '"')
        {
            while (i + 1 < programa.size() && !(programa[i] == '"')) i++;
            if (i + 1 >= programa.size())
            {
                std::cerr << "Erro lexico: coment�rio de bloco n�o fechado" << std::endl;
                break;
            }
            i += 1;
            continue;
        }

        // In�cio da an�lise lexica

        if (std::find(quebras.begin(), quebras.end(), programa[i]) == quebras.end())
        {
            if (std::find(quebraComplexo.begin(), quebraComplexo.end(), programa[i]) == quebraComplexo.end())
            {
                if (programa[i] != ' ')
                {
                    //Insere caracter no lexema
                    lexema.push_back(programa[i]);
                }
            }
            else if (std::find(quebraComplexo.begin(), quebraComplexo.end(), programa[i+1]) != quebraComplexo.end())
            {
                //Insere o token de quebra de palavra com multiplos caracteres na lista de lexema
                lexemas.push_back(std::string() + programa[i] + programa[i + 1]);
                token.push_back(tokenizador(lexemas.back(), &token));
                i += 1;
            }
            else
            {
                //Insere o token de quebra de palavra com um caracter na lista de lexema
                lexemas.push_back(std::string{ programa[i] });
                token.push_back(tokenizador(lexemas.back(), &token));
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
                lexemas.push_back(lexemaAtual);
                token.push_back(tokenizador(lexemaAtual, &token));
                lexemaAtual.clear();
            }
            if (programa[i] != ' ' && programa[i] != '\n')
            {
                lexemas.push_back(std::string{ programa[i] });
                token.push_back(tokenizador(lexemas.back(), &token));
            }
            lexema.clear();
        }
    }

    for (size_t i = 0; i < lexemas.size(); i++)
    {
        std::cout << "'" << lexemas[i] << "' , ";
    }

    for (size_t i = 0; i < token.size(); i++)
    {
        std::cout << " " << token[i];
    }
    std::vector<char> samuel;
    return std::make_tuple(lexemas, samuel);
}

int main(int argc, char const* argv[]) {

    char programaCh;
    std::vector<char> programaVector;
    std::tuple<std::vector<std::string>, std::vector<char>> respostaLexer;

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

    //respostaLexer = lexer(programa);
    

    programa.close();

	return 0;
}