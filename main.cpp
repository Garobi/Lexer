#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include "lexer.h"


static void printLexer(std::tuple<std::vector<std::string>, std::vector<int>, std::vector<int>> respostaLexer) {
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

    wchar_t filename[MAX_PATH];

    std::wstring message;

    OPENFILENAMEW ofn;
    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
    ofn.lpstrFilter  = L"Text Files\0*.txt\0Any File\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Escolha um arquivo para a analise!";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        std::wcout << "You chose the file " << filename << std::endl;
    }
    else
    {
        // All this stuff below is to tell you exactly how you messed up above. 
        // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
        switch (CommDlgExtendedError())
        {
        case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
        case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
        case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
        case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
        case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
        case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
        case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
        case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
        case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
        case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
        case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
        case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
        case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
        case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
        case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
        default: std::cout << "You cancelled.\n";
        }
    }

    std::ifstream programa(filename);
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
