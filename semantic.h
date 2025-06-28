// semantic.h

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include "definitions.h" // For reverseTokenMap

// SEMANTIC: Defines the possible types in our language.
enum class Type {
    TYPE_UNDEFINED,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_BOOL,    // For results of relational expressions
    TYPE_VOID,    // For procedures
    TYPE_PROGRAM  // Special type for the program name
};

// SEMANTIC: Defines the kind of an identifier.
enum class SymbolKind {
    KIND_UNDEFINED,
    KIND_VAR,
    KIND_CONST,
    KIND_PROC,
    KIND_PROGRAM
};

// SEMANTIC: Represents a single symbol (identifier) in the symbol table.
struct Symbol {
    std::string name;
    Type type;
    SymbolKind kind;
    int line;
    bool initialized = false;
    // For constants, we could store their value here
    // For procedures, we could store a list of parameter types
};

// SEMANTIC: The Symbol Table to manage scopes.
class SymbolTable {
private:
    std::stack<std::unordered_map<std::string, Symbol>> scope_stack;
    int current_level = -1;

public:
    SymbolTable();
    void enterScope();
    void leaveScope();
    bool addSymbol(const Symbol& symbol);
    Symbol* findSymbol(const std::string& name);
};

// SEMANTIC: Global instance of the symbol table.
extern SymbolTable symbolTable;

// SEMANTIC: A stack to hold types during expression evaluation.
extern std::stack<Type> typeStack;

// SEMANTIC: A vector to temporarily hold identifier names during declaration.
extern std::vector<std::string> ident_capture_list;

// SEMANTIC: Action function declarations
void handle_semantic_action(int action, const std::string& currentLexema, int currentLine);

#endif // SEMANTIC_H