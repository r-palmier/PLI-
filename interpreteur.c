#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Définition des types de tokens représentant les différents éléments dans une expression
typedef enum {
    TOKEN_NOMBRE,   // Un nombre entier
    TOKEN_PLUS,     // Symbole '+'
    TOKEN_MOINS,    // Symbole '-'
    TOKEN_DIVIDE,   // Symbole '/'
    TOKEN_MULTIPLY, // Symbole '*'
    TOKEN_GPAR,     // Parenthèse gauche '('
    TOKEN_DPAR,     // Parenthèse droite ')'
    TOKEN_VARIABLE, // Variable (ex: x, y, z, ...)
    TOKEN_EOF,      // Fin de l'expression
    TOKEN_ERROR     // Caractère non reconnu
} TokenType;

// Structure représentant un token contenant son type, une valeur pour les nombres et une variable si applicable
typedef struct {
    TokenType type; // Type du token
    int value;      // Valeur entière, utilisée si le token est un nombre
    char var;       // Variable, utilisée si le token est une lettre (ex: x, y)
} Token;

// Fonction pour créer un nouveau token avec un type, une valeur et une variable
Token new_token(TokenType type, int value, char var) {
    Token token;
    token.type = type;
    token.value = value;
    token.var = var;
    return token;
}

// Lexer qui analyse la chaîne de caractères et crée des tokens
Token get_next_token(const char **text) {
    while (**text != '\0') { // Parcourt jusqu'à la fin de la chaîne
        if (isspace(**text)) { // Ignore les espaces
            (*text)++;
            continue;
        }
        // Crée un token pour les nombres
        if (isdigit(**text)) {
            int value = 0;
            while (isdigit(**text)) { // Conversion de la séquence de chiffres en entier
                value = value * 10 + (**text - '0');
                (*text)++;
            }
            return new_token(TOKEN_NOMBRE, value, '\0');
        }
        // Crée un token pour les variables (ex: x, y)
        if (isalpha(**text)) {
            char var = **text;
            (*text)++;
            return new_token(TOKEN_VARIABLE, 0, var);
        }
        // Crée un token pour les opérateurs et parenthèses
        if (**text == '+') { (*text)++; return new_token(TOKEN_PLUS, 0, '\0'); }
        if (**text == '-') { (*text)++; return new_token(TOKEN_MOINS, 0, '\0'); }
        if (**text == '/') { (*text)++; return new_token(TOKEN_DIVIDE, 0, '\0'); }
        if (**text == '*') { (*text)++; return new_token(TOKEN_MULTIPLY, 0, '\0'); }
        if (**text == '(') { (*text)++; return new_token(TOKEN_GPAR, 0, '\0'); }
        if (**text == ')') { (*text)++; return new_token(TOKEN_DPAR, 0, '\0'); }

        // Gère les erreurs de caractères non reconnus
        printf("Erreur : valeur incorrecte '%c'\n", **text);
        (*text)++;
        return new_token(TOKEN_ERROR, 0, '\0');
    }
    return new_token(TOKEN_EOF, 0, '\0'); // Fin de l'expression
}

// Structure de l'arbre syntaxique abstrait (AST) pour représenter l'expression
typedef struct ASTNode {
    enum { AST_NUMBER, AST_VARIABLE, AST_OPERATION } type; // Type de nœud
    union {
        int value;       // Valeur pour les nombres
        char var;        // Variable pour les lettres
        struct {         // Opération pour les opérateurs
            struct ASTNode *left;
            struct ASTNode *right;
            TokenType operation;
        } operation;
    };
} ASTNode;

// Fonctions de création de nœuds pour chaque type d'élément de l'AST
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

// Analyseurs pour gérer les différentes priorités des opérations
ASTNode* parse_primary(const char **text);     // Analyse les valeurs primaires (nombres et variables)
ASTNode* parse_term(const char **text);        // Analyse les termes (multiplication et division)
ASTNode* parse_expression(const char **text);  // Analyse les expressions complètes (addition et soustraction)

// Analyse les éléments primaires (nombres, variables, et expressions entre parenthèses)
ASTNode* parse_primary(const char **text) {
    Token token = get_next_token(text);

    if (token.type == TOKEN_NOMBRE) {
        return create_number_node(token.value);
    } else if (token.type == TOKEN_VARIABLE) {
        return create_variable_node(token.var);
    } else if (token.type == TOKEN_GPAR) {
        ASTNode* node = parse_expression(text);
        if (get_next_token(text).type != TOKEN_DPAR) { // Vérifie la fermeture de la parenthèse
            printf("Erreur : Parenthèse fermante attendue\n");
            free(node);
            return NULL;
        }
        return node;
    }
    printf("Erreur : Expression primaire invalide.\n");
    return NULL;
}

// Analyse les termes (priorité pour multiplication et division)
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

    (*text)--; // Remet le dernier token non utilisé
    return left;
}

// Analyse les expressions (priorité pour addition et soustraction)
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

    (*text)--; // Remet le dernier token non utilisé
    return left;
}

// Fonction pour évaluer l'AST récursivement
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

// Fonction pour libérer la mémoire de l'AST après l'évaluation
void free_ast(ASTNode* node) {
    if (node->type == AST_OPERATION) {
        free_ast(node->operation.left);
        free_ast(node->operation.right);
    }
    free(node);
}

// Fonction principale pour lire l'entrée utilisateur, analyser l'expression et afficher le résultat
#define MAX_INPUT_SIZE 100
int main() {
    char input[MAX_INPUT_SIZE];
    printf("Entrez une formule: ");

    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) { // Récupère la chaîne de l'utilisateur
        printf("Erreur de lecture de l'entree\n");
        return 1;
    }

    const char *expression = input;
    ASTNode* ast = parse_expression(&expression);

    if (ast != NULL) { // Si l'AST a été créé avec succès, on l'évalue
        int result = evaluate_ast(ast);
        printf("Resultat : %d\n", result);
        free_ast(ast); // Libère la mémoire
    } else {
        printf("Erreur dans l'analyse de l'expression.\n");
    }

    return 0;
}
