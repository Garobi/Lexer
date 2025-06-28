#include "definitions.h"
#include <unordered_map>
#include <string>
#include <utility>

constexpr int SA_CAPTURE_IDENT = -1;
constexpr int SA_SET_TYPE_INT = -2;
constexpr int SA_SET_TYPE_REAL = -3;
constexpr int SA_SET_TYPE_STRING = -4;
constexpr int SA_COMMIT_VARS = -5;
constexpr int SA_FIND_IDENT = -6;
constexpr int SA_PUSH_TYPE_INT = -7;
constexpr int SA_PUSH_TYPE_REAL = -8;
constexpr int SA_PUSH_TYPE_STRING = -9;
constexpr int SA_CHECK_ASSIGN = -10;
constexpr int SA_CHECK_ADD = -11;
constexpr int SA_CHECK_SUB = -12;
constexpr int SA_CHECK_MUL = -13;
constexpr int SA_CHECK_DIV = -14;
constexpr int SA_CHECK_RELOP = -15;
constexpr int SA_ENTER_SCOPE = -16;
constexpr int SA_LEAVE_SCOPE = -17;

std::map<std::pair<int, int>, int> parsing_table = {
{{45,8},1},
{{46,21},2},
{{46,2},2},
{{48,21},3},
{{48,2},62},
{{49,2},5},
{{52,16},6},
{{55,33},62},
{{55,36},8},
{{54,16},9},
{{54,9},62},
{{54,22},62},
{{53,14},11},
{{53,6},12},
{{53,5},13},
{{50,9},14},
{{50,22},62},
{{56,31},62},
{{56,39},16},
{{57,36},18},
{{57,38},62},
{{47,22},20},
{{51,16},21},
{{51,18},62},
{{51,10},21},
{{51,15},21},
{{51,17},21},
{{51,1},21},
{{51,7},21},
{{58,16},51},
{{58,10},23},
{{58,15},41},
{{58,17},52},
{{58,1},63},
{{58,7},64},
{{59,16},24},
{{59,12},24},
{{59,39},24},
{{59,11},24},
{{59,13},24},
{{59,23},24},
{{60,36},26},
{{60,41},62},
{{61,16},27},
{{61,12},27},
{{61,39},27},
{{61,11},27},
{{61,13},27},
{{61,23},27},
{{63,31},62},
{{63,26},62},
{{63,36},62},
{{63,38},62},
{{63,41},62},
{{63,30},28},
{{63,42},29},
{{63,4},62},
{{63,27},62},
{{63,29},62},
{{63,25},62},
{{63,28},62},
{{63,24},62},
{{63,3},62},
{{63,20},62},
{{62,16},31},
{{62,12},31},
{{62,39},31},
{{62,11},31},
{{62,13},31},
{{62,23},31},
{{65,31},62},
{{65,26},62},
{{65,36},62},
{{65,38},62},
{{65,41},62},
{{65,30},62},
{{65,42},62},
{{65,37},32},
{{65,34},33},
{{65,4},62},
{{65,27},62},
{{65,29},62},
{{65,25},62},
{{65,28},62},
{{65,24},62},
{{65,3},62},
{{65,20},62},
{{64,16},57},
{{64,12},58},
{{64,39},35},
{{64,11},59},
{{64,13},39},
{{64,23},60},
{{66,16},42},
{{66,12},42},
{{66,39},42},
{{66,11},42},
{{66,13},42},
{{66,23},42},
{{68,26},43},
{{68,27},44},
{{68,29},45},
{{68,25},46},
{{68,28},47},
{{68,24},48},
{{67,31},62},
{{67,19},49},
{{69,31},53},
{{69,39},53},
{{69,32},54},
{{70,31},62},
{{70,39},55},
{{71,16},57},
{{71,12},58},
{{71,11},59},
{{71,13},65},
{{71,23},60},
{{72,36},61},
{{72,38},62},
};


std::map<int, std::vector<int>> productions = {
{1,  {8, 16, 31, 46, 47, 35}},                     // PROGRAMA ::= program ident ; DECLARACOES BLOCO .
{2,  {48, 49, 50}},                                // DECLARACOES ::= CONSTANTES VARIAVEIS PROCEDIMENTOS
{3,  {21, 16, 26, 12, 31, 48}},                    // CONSTANTES ::= const ident = nint ; CONSTANTES
{4,  {44}},                                        // CONSTANTES ::= ε
{5,  {2, 52, 33, 53, 31, 54}},                     // VARIAVEIS ::= var LISTAVARIAVEIS : TIPO ; LDVAR
{6,  {16, 55}},                                    // LISTAVARIAVEIS ::= ident REPIDENT
{7,  {44}},                                        // REPIDENT ::= ε
{8,  {36, 16, 55}},                                // REPIDENT ::= , ident REPIDENT
{9,  {52, 33, 53, 31, 54}},                        // LDVAR ::= LISTAVARIAVEIS : TIPO ; LDVAR
{10, {44}},                                        // LDVAR ::= ε
{11, {14}},                                        // TIPO ::= integer
{12, {6}},                                         // TIPO ::= real
{13, {5}},                                         // TIPO ::= string
{14, {9, 16, 56, 31, 47, 31, 50}},                 // PROCEDIMENTOS ::= procedure ident PARAMETROS ; BLOCO ; PROCEDIMENTOS
{15, {44}},                                        // PROCEDIMENTOS ::= ε
{16, {39, 52, 33, 53, 57, 38}},                    // PARAMETROS ::= ( LISTAVARIAVEIS : TIPO REPPARAMETROS )
{17, {44}},                                        // PARAMETROS ::= ε
{18, {36, 52, 33, 53, 57}},                        // REPPARAMETROS ::= , LISTAVARIAVEIS : TIPO REPPARAMETROS
{19, {44}},                                        // REPPARAMETROS ::= ε
{20, {22, 51, 18}},                                // BLOCO ::= begin COMANDOS end
{21, {58, 31, 51}},                                // COMANDOS ::= COMANDO ; COMANDOS
{22, {44}},                                        // COMANDOS ::= ε
{23, {10, 40, 59, 60, 41}},                        // COMANDO ::= print { ITEMSAIDA REPITEM }
{24, {61}},                                        // ITEMSAIDA ::= EXPRESSAO
{25, {44}},                                        // REPITEM ::= ε
{26, {36, 59, 60}},                                // REPITEM ::= , ITEMSAIDA REPITEM
{27, {62, 63}},                                    // EXPRESSAO ::= TERMO EXPR
{28, {30, 62, 63}},                                // EXPR ::= + TERMO EXPR
{29, {42, 62, 63}},                                // EXPR ::= - TERMO EXPR
{30, {44}},                                        // EXPR ::= ε
{31, {64, 65}},                                    // TERMO ::= FATOR TER
{32, {37, 64, 65}},                                // TER ::= * FATOR TER
{33, {34, 64, 65}},                                // TER ::= / FATOR TER
{34, {44}},                                        // TER ::= ε
{35, {39, 61, 38}},                                // FATOR ::= ( EXPRESSAO )
{36, {16}},                                        // FATOR ::= ident
{37, {12}},                                        // FATOR ::= nint
{38, {11}},                                        // FATOR ::= nreal
{39, {13}},                                        // FATOR ::= literal
{40, {23}},                                        // FATOR ::= vstring
{41, {15, 66, 4, 47, 67}},                         // COMANDO ::= if EXPRELACIONAL then BLOCO ELSEOPC
{42, {61, 68, 61}},                                // EXPRELACIONAL ::= EXPRESSAO OPREL EXPRESSAO
{43, {26}},                                        // OPREL ::= =
{44, {27}},                                        // OPREL ::= <>
{45, {29}},                                        // OPREL ::= <
{46, {25}},                                        // OPREL ::= >
{47, {28}},                                        // OPREL ::= <=
{48, {24}},                                        // OPREL ::= >=
{49, {19, 47}},                                    // ELSEOPC ::= else BLOCO
{50, {44}},                                        // ELSEOPC ::= ε
{51, {16, 69}},                                    // COMANDO ::= ident CHAMADAPROC
{52, {17, 16, 32, 61, 3, 61, 20, 47}},             // COMANDO ::= for ident := EXPRESSAO to EXPRESSAO do BLOCO
{53, {70}},                                        // CHAMADAPROC ::= LISTAPARAMETROS
{54, {32, 61}},                                    // CHAMADAPROC ::= := EXPRESSAO
{55, {39, 71, 72, 38}},                            // LISTAPARAMETROS ::= ( PAR REPPAR )
{56, {44}},                                        // LISTAPARAMETROS ::= ε
{57, {16}},                                        // PAR ::= ident
{58, {12}},                                        // PAR ::= nint
{59, {11}},                                        // PAR ::= nreal
{60, {23}},                                        // PAR ::= vstring
{61, {36, 71, 72}},                                    // REPPAR ::= , PAR REPPAR
{62, {44}},                                        // REPPAR ::= ε
{63, {1, 66, 20, 47}},                             // COMANDO ::= while EXPRELACIONAL do BLOCO
{64, {7, 39, 16, 38}},                             // COMANDO ::= read ( ident )
{65, {13}},                                        // PAR ::= literal
};

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

std::unordered_map<int, std::string> reverseTokenMap = {
    {1, "while"}, {2, "var"}, {3, "to"}, {4, "then"}, {5, "string"},
    {6, "real"}, {7, "read"}, {8, "program"}, {9, "procedure"}, {10, "print"},
    {11, "nreal"}, {12, "nint"}, {13, "literal"}, {14, "integer"}, {15, "if"},
    {16, "ident"}, {17, "for"}, {18, "end"}, {19, "else"}, {20, "do"},
    {21, "const"}, {22, "begin"}, {23, "vstring"}, {24, ">="}, {25, ">"},
    {26, "="}, {27, "<>"}, {28, "<="}, {29, "<"}, {30, "+"},
    {31, ";"}, {32, ":="}, {33, ":"}, {34, "/"}, {35, "."},
    {36, ","}, {37, "*"}, {38, ")"}, {39, "("}, {40, "{"},
    {41, "}"}, {42, "-"}, {43, "$"}, {44, "î"},
    {45, "PROGRAMA"}, {46, "DECLARACOES"}, {47, "BLOCO"}, {48, "CONSTANTES"},
    {49, "VARIAVEIS"}, {50, "PROCEDIMENTOS"}, {51, "COMANDOS"}, {52, "LISTAVARIAVEIS"},
    {53, "TIPO"}, {54, "LDVAR"}, {55, "REPIDENT"}, {56, "PARAMETROS"},
    {57, "REPPARAMETROS"}, {58, "COMANDO"}, {59, "ITEMSAIDA"}, {60, "REPITEM"},
    {61, "EXPRESSAO"}, {62, "TERMO"}, {63, "EXPR"}, {64, "FATOR"},
    {65, "TER"}, {66, "EXPRELACIONAL"}, {67, "ELSEOPC"}, {68, "OPREL"},
    {69, "CHAMADAPROC"}, {70, "LISTAPARAMETROS"}, {71, "PAR"}, {72, "REPPAR"}
};

std::vector<char> quebras = { ' ', '{', '}', '(', ')', '+', ';', '/', '.', ',', '*', '-', '$', '\n', '\t' };
std::vector<char> quebraComplexo = { '>', '<', ':', '@', '!', '=' };