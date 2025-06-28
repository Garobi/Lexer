// semantic.cpp

#include "semantic.h"
#include "definitions.cpp"

// SEMANTIC: Instantiate global variables.
SymbolTable symbolTable;
std::stack<Type> typeStack;
std::vector<std::string> ident_capture_list;
Type last_parsed_type; // To hold the type from a type declaration

// SEMANTIC: Helper to convert our Type enum to a string for error messages.
std::string typeToString(Type t) {
    switch (t) {
    case Type::TYPE_INTEGER: return "integer";
    case Type::TYPE_REAL:    return "real";
    case Type::TYPE_STRING:  return "string";
    case Type::TYPE_BOOL:    return "boolean";
    case Type::TYPE_VOID:    return "void";
    default:                 return "undefined";
    }
}

// SEMANTIC: Throws a formatted semantic error.
[[noreturn]] static void throwSemanticError(const std::string& message, int line) {
    std::cerr << "Erro Semantico na linha " << line << ": " << message << std::endl;
    std::exit(EXIT_FAILURE);
}


/* --- SymbolTable Implementation --- */
SymbolTable::SymbolTable() {
    enterScope(); // Create global scope
}

void SymbolTable::enterScope() {
    scope_stack.push({});
    current_level++;
}

void SymbolTable::leaveScope() {
    if (!scope_stack.empty()) {
        scope_stack.pop();
        current_level--;
    }
}

bool SymbolTable::addSymbol(const Symbol& symbol) {
    auto& current_scope = scope_stack.top();
    if (current_scope.count(symbol.name)) {
        // Symbol already exists in the current scope.
        return false;
    }
    current_scope[symbol.name] = symbol;
    return true;
}

Symbol* SymbolTable::findSymbol(const std::string& name) {
    std::stack<std::unordered_map<std::string, Symbol>> temp_stack = scope_stack;
    while (!temp_stack.empty()) {
        auto& scope = temp_stack.top();
        if (scope.count(name)) {
            // We need to return a pointer to the original symbol, not a copy
            return &scope_stack.top()[name];
        }
        temp_stack.pop();
    }
    return nullptr; // Not found
}


/* --- Semantic Action Implementations --- */

// Captures an identifier name during a declaration list
void action_capture_ident(const std::string& lexema) {
    ident_capture_list.push_back(lexema);
}

// Stores the type found in a declaration
void action_set_type(Type t) {
    last_parsed_type = t;
}

// Commits the captured variables to the symbol table with the stored type
void action_commit_vars(int line) {
    if (last_parsed_type == Type::TYPE_UNDEFINED) {
        throwSemanticError("Tipo de variavel desconhecido.", line);
    }
    for (const auto& ident_name : ident_capture_list) {
        Symbol s = { ident_name, last_parsed_type, SymbolKind::KIND_VAR, line };
        if (!symbolTable.addSymbol(s)) {
            throwSemanticError("Identificador '" + ident_name + "' ja declarado neste escopo.", line);
        }
    }
    ident_capture_list.clear();
    last_parsed_type = Type::TYPE_UNDEFINED;
}

// Looks up an identifier in an expression and pushes its type to the type stack
void action_find_and_push_type(const std::string& lexema, int line) {
    Symbol* s = symbolTable.findSymbol(lexema);
    if (!s) {
        throwSemanticError("Identificador '" + lexema + "' nao foi declarado.", line);
    }
    typeStack.push(s->type);
}

// Pushes a literal's type onto the type stack
void action_push_literal_type(Type t) {
    typeStack.push(t);
}

// Checks type compatibility for an assignment (:=)
void action_check_assignment(int line) {
    if (typeStack.size() < 2) return;

    Type rhs_type = typeStack.top(); typeStack.pop();
    Type lhs_type = typeStack.top(); typeStack.pop();

    // Allow assigning integer to real, but not the other way around without a cast.
    if (lhs_type == rhs_type || (lhs_type == Type::TYPE_REAL && rhs_type == Type::TYPE_INTEGER)) {
        // Compatible
    }
    else {
        throwSemanticError("Tipos incompativeis para atribuicao. Nao e possivel atribuir '" + typeToString(rhs_type) + "' para '" + typeToString(lhs_type) + "'.", line);
    }
}

// Checks types for an arithmetic or relational operation
void action_check_op(int op_token, int line) {
    if (typeStack.size() < 2) return;

    Type t2 = typeStack.top(); typeStack.pop();
    Type t1 = typeStack.top(); typeStack.pop();

    bool are_numeric = (t1 == Type::TYPE_INTEGER || t1 == Type::TYPE_REAL) &&
        (t2 == Type::TYPE_INTEGER || t2 == Type::TYPE_REAL);

    // Relational operators can compare numeric types
    if (op_token >= 24 && op_token <= 29) { // >= > = <> <= <
        if (!are_numeric) {
            throwSemanticError("Operadores relacionais so podem ser usados com tipos numericos (integer, real).", line);
        }
        typeStack.push(Type::TYPE_BOOL); // Result of a relation is a boolean
    }
    // Arithmetic operators
    else if (op_token == 30 || op_token == 42 || op_token == 37 || op_token == 34) { // + - * /
        if (!are_numeric) {
            throwSemanticError("Operadores aritmeticos so podem ser usados com tipos numericos (integer, real).", line);
        }
        // Type promotion: if one operand is real, the result is real.
        if (t1 == Type::TYPE_REAL || t2 == Type::TYPE_REAL) {
            typeStack.push(Type::TYPE_REAL);
        }
        else {
            typeStack.push(Type::TYPE_INTEGER);
        }
    }
}


// SEMANTIC: Main dispatcher for semantic actions.
void handle_semantic_action(int action, const std::string& currentLexema, int currentLine) {
    switch (action) {
    case SA_CAPTURE_IDENT:
        action_capture_ident(currentLexema);
        break;
    case SA_SET_TYPE_INT:
        action_set_type(Type::TYPE_INTEGER);
        break;
    case SA_SET_TYPE_REAL:
        action_set_type(Type::TYPE_REAL);
        break;
    case SA_SET_TYPE_STRING:
        action_set_type(Type::TYPE_STRING);
        break;
    case SA_COMMIT_VARS:
        action_commit_vars(currentLine);
        break;
    case SA_FIND_IDENT:
        action_find_and_push_type(currentLexema, currentLine);
        break;
    case SA_PUSH_TYPE_INT:
        action_push_literal_type(Type::TYPE_INTEGER);
        break;
    case SA_PUSH_TYPE_REAL:
        action_push_literal_type(Type::TYPE_REAL);
        break;
    case SA_PUSH_TYPE_STRING:
        action_push_literal_type(Type::TYPE_STRING);
        break;
    case SA_CHECK_ASSIGN:
        action_check_assignment(currentLine);
        break;
    case SA_CHECK_ADD: action_check_op(30, currentLine); break;
    case SA_CHECK_SUB: action_check_op(42, currentLine); break;
    case SA_CHECK_MUL: action_check_op(37, currentLine); break;
    case SA_CHECK_DIV: action_check_op(34, currentLine); break;
    case SA_CHECK_RELOP: action_check_op(26, currentLine); break; // Using '=' as a stand-in for all relational ops
    case SA_ENTER_SCOPE:
        symbolTable.enterScope();
        break;
    case SA_LEAVE_SCOPE:
        symbolTable.leaveScope();
        break;
    default:
        // Should not happen
        break;
    }
}