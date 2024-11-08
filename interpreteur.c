#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Definition des types de tokens || enum named TokenType
typedef enum {
    TOKEN_NOMBRE,
    TOKEN_PLUS,
    TOKEN_MOINS,
    TOKEN_DIVIDE,
    TOKEN_MULTIPLY,
    TOKEN_GPAR,     // Parenthèse gauche '('
    TOKEN_DPAR,     // Parenthèse droite ')'
    TOKEN_VARIABLE, // Pour les variables (x, y, z, ...)
    TOKEN_EOF,
    TOKEN_ERROR     // caractères non reconnus
} TokenType;

// Structure du token
typedef struct {
    TokenType type;
    int value; // Utilise uniquement si le token est un nombre
    char var; // si le token est une variable
} Token;

// Fonction pour creer un nouveau token
Token new_token(TokenType type, int value, char var) {
    Token token;
    token.type = type;
    token.value = value;
    token.var = var;
    return token;
}

// Lexer pour transformer la chaîne en tokens
Token get_next_token(const char **text) {
    while (**text != '\0') {
        if (isspace(**text)) {
            (*text)++;
            continue;
        }
        if (isdigit(**text)) {
            int value = 0;
            while (isdigit(**text)) { // pos *10 + val - 56 (ascii 0) concatenation
                value = value * 10 + (**text - '0');
                (*text)++;
            }
            return new_token(TOKEN_NOMBRE, value, '\0');
        }
        if (isalpha(**text)) {
            char var = **text;
            (*text)++;
            return new_token(TOKEN_VARIABLE, 0, var);
        }
        if (**text == '+') { (*text)++; return new_token(TOKEN_PLUS, 0, '\0'); }
        if (**text == '-') { (*text)++; return new_token(TOKEN_MOINS, 0, '\0'); }
        if (**text == '/') { (*text)++; return new_token(TOKEN_DIVIDE, 0, '\0'); }
        if (**text == '*') { (*text)++; return new_token(TOKEN_MULTIPLY, 0, '\0'); }
        if (**text == '(') { (*text)++; return new_token(TOKEN_GPAR, 0, '\0'); }
        if (**text == ')') { (*text)++; return new_token(TOKEN_DPAR, 0, '\0'); }

        printf("Erreur : valeur incorrecte '%c'\n", **text);
        (*text)++;
        return new_token(TOKEN_ERROR, 0, '\0');
    }
    return new_token(TOKEN_EOF, 0, '\0');
}

// Structure pour representer les nœuds de l'AST
typedef struct ASTNode {
    enum { AST_NUMBER, AST_VARIABLE, AST_OPERATION } type;
    union {
        int value;
        char var;
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            TokenType operation;
        } operation;
    };
} ASTNode;

// Creation d'un noeud pour l'AST
ASTNode* create_number_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->value = value;
    return node;
}

ASTNode* create_variable_node(char var) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_VARIABLE;
    node->var = var;
    return node;
}

ASTNode* create_operation_node(TokenType operation, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_OPERATION;
    node->operation.operation = operation;
    node->operation.left = left;
    node->operation.right = right;
    return node;
}

// Analyseurs de priorite pour gerer les operations arithmetiques
ASTNode* parse_primary(const char **text);
ASTNode* parse_term(const char **text);
ASTNode* parse_expression(const char **text);

ASTNode* parse_primary(const char **text) {
    Token token = get_next_token(text);

    if (token.type == TOKEN_NOMBRE) {
        return create_number_node(token.value);
    } else if (token.type == TOKEN_VARIABLE) {
        return create_variable_node(token.var);
    } else if (token.type == TOKEN_GPAR) {
        ASTNode* node = parse_expression(text);
        if (get_next_token(text).type != TOKEN_DPAR) {
            printf("Erreur : Parenthèse fermante attendue\n");
            free(node);
            return NULL;
        }
        return node;
    }
    printf("Erreur : Expression primaire invalide.\n");
    return NULL;
}

ASTNode* parse_term(const char **text) {
    ASTNode* left = parse_primary(text);
    Token token = get_next_token(text);

    while (token.type == TOKEN_MULTIPLY || token.type == TOKEN_DIVIDE) {
        TokenType operation = token.type;
        ASTNode* right = parse_primary(text);
        if (!right) return NULL;
        left = create_operation_node(operation, left, right);
        token = get_next_token(text);
    }

    // Replace le dernier token non utilise
    (*text)--;
    return left;
}

ASTNode* parse_expression(const char **text) {
    ASTNode* left = parse_term(text);
    Token token = get_next_token(text);

    while (token.type == TOKEN_PLUS || token.type == TOKEN_MOINS) {
        TokenType operation = token.type;
        ASTNode* right = parse_term(text);
        if (!right) return NULL;
        left = create_operation_node(operation, left, right);
        token = get_next_token(text);
    }

    (*text)--;
    return left;
}

// evaluation de l'AST
int evaluate_ast(ASTNode* node) {
    if (node->type == AST_NUMBER) {
        return node->value;
    } else if (node->type == AST_OPERATION) {
        int left_val = evaluate_ast(node->operation.left);
        int right_val = evaluate_ast(node->operation.right);

        switch (node->operation.operation) {
            case TOKEN_PLUS: return left_val + right_val;
            case TOKEN_MOINS: return left_val - right_val;
            case TOKEN_MULTIPLY: return left_val * right_val;
            case TOKEN_DIVIDE:
                if (right_val == 0) {
                    printf("Erreur : Division par zero.\n");
                    return 0;
                }
                return left_val / right_val;
            default:
                printf("Erreur : Operation inconnue.\n");
                return 0;
        }
    }
    return 0;
}

// Liberation de la memoire de l'AST
void free_ast(ASTNode* node) {
    if (node->type == AST_OPERATION) {
        free_ast(node->operation.left);
        free_ast(node->operation.right);
    }
    free(node);
}

#define MAX_INPUT_SIZE 100
int main() {
    char input[MAX_INPUT_SIZE];
    printf("Entrez une formule: ");

    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Erreur de lecture de l'entree\n");
        return 1;
    }

    const char *expression = input;
    ASTNode* ast = parse_expression(&expression);

    if (ast != NULL) {
        int result = evaluate_ast(ast);
        printf("Resultat : %d\n", result);
        free_ast(ast);
    } else {
        printf("Erreur dans l'analyse de l'expression.\n");
    }

    return 0;
}
