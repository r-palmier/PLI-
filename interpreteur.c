#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Définition des types de tokens || enum named TokenType
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

// Structure du token || tsruct named Token
typedef struct {
    TokenType type;
    int value; // Utilisé uniquement si le token est un nombre
    char var; // si le token est une variable
} Token;

// Fonction pour créer un nouveau token 
Token new_token(TokenType type, int value, char var) {
    Token token;
    token.type = type;
    token.value = value;
    token.var = var;
    return token;
}


// Lexer pour transformer la chaîne en tokens

Token get_next_token(const char **text) {
    while (**text != '\0') { // parcour de le chaine
        if (isspace(**text)) {
            (*text)++;
            continue;
        }
        if (isdigit(**text)) { // si chiffre
            int value = 0;
            while (isdigit(**text)) {
                value = value * 10 + (**text - '0'); // pos *10 + val - 56 (ascii 0) concatenation
                (*text)++;
            }
            return new_token(TOKEN_NOMBRE, value, '\0');
        }
        if (isalpha(**text)) { // si var
            char var = **text;
            (*text)++;
            return new_token(TOKEN_VARIABLE, 0, var);
        }
        if (**text == '+') { (*text)++; return new_token(TOKEN_PLUS, 0, '\0'); } //else
        if (**text == '-') { (*text)++; return new_token(TOKEN_MOINS, 0, '\0'); }
        if (**text == '/') { (*text)++; return new_token(TOKEN_DIVIDE, 0, '\0'); }
        if (**text == '*') { (*text)++; return new_token(TOKEN_MULTIPLY, 0, '\0'); }
        if (**text == '(') { (*text)++; return new_token(TOKEN_GPAR, 0, '\0'); }
        if (**text == ')') { (*text)++; return new_token(TOKEN_DPAR, 0, '\0'); }

        printf("Erreur : valeur incorrecte'%c'\n", **text);
        (*text)++;
        return new_token(TOKEN_ERROR, 0, '\0');
    }
    return new_token(TOKEN_EOF, 0, '\0');
}


// Fonction de test
#define MAX_INPUT_SIZE 100

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Entrez une formule: ");

    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Erreur de lecture de l'entrée\n");
        return 1;
    }

    const char *expression = input;
    Token token;

    while ((token = get_next_token(&expression)).type != TOKEN_EOF) {
        switch (token.type) {
            case TOKEN_NOMBRE:
                printf("Token: NOMBRE, Value: %d\n", token.value);
                break;
            case TOKEN_PLUS:
                printf("Token: PLUS\n");
                break;
            case TOKEN_MOINS:
                printf("Token: MOINS\n");
                break;
            case TOKEN_DIVIDE:
                printf("Token: DIVIDE\n");
                break;
            case TOKEN_MULTIPLY:
                printf("Token: MULTIPLY\n");
                break;
            case TOKEN_GPAR:
                printf("Token: GPAR (Parenthese gauche)\n");
                break;
            case TOKEN_DPAR:
                printf("Token: DPAR (Parenthese droite)\n");
                break;
            case TOKEN_VARIABLE:
                printf("Token: VARIABLE, Name: %c\n", token.var);
                break;
            case TOKEN_ERROR:
                printf("Token: ERROR\n");
                break;
            default:
                break;
        }
    }
    return 0;
}