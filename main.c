#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Définit un type de 0 à 7 
typedef enum {
    Nombre,
    Variable,
    Plus,
    Moins,
    Multi,
    Div,
    GPar,
    DPar,
    Fin
} TokenType;

// Crée une structure qui retourne le type et la value
typedef struct
{
    TokenType type;
    char* value;

} Token;

// Scanne une chaîne et génère une liste de token 
Token* lexer(const char* input, int* token_count){
    Token* tokens = malloc(sizeof(Token) * 100);
    *token_count = 0;

    const char* p = input;
    
    // Continue tant qu'on est pas arrivé à la fin de la chaîne
    while (*p != '\0'){

        //ignore les espaces
        if(isspace(*p)){
            p++;
            continue;
        }

        //Crée une structure token 
        Token token;

        //Vérifie si c'est un chiffre
        if (isdigit(*p)) {
            token.type = Nombre;
            token.value = malloc(20);

            int i = 0;

            //Lit les chiffrs consécutifs
            while (isdigit(*p)){
                //Stocke dans value et avance dans la chaine
                token.value[i++] = *p++;
            }
            // Rajoute à la fin
            token.value[i] = '\0';
        }
        
        //Verifie si lettre 
        else if (isalpha(*p)){
            token.type = Variable;
            token.value = malloc(2);
            token.value[0] = *p;
            token.value[1] = '\0'; //termine la chaine
            p++;
        }

        else if (*p == '+'){
                token.type = Plus;
                token.value = malloc(2);
                strcpy(token.value, "+");
                p++;
        }

        else if (*p == '-'){
            token.type = Moins;
            token.value = malloc(2);
            strcpy(token.value, "-");
            p++;
        }

        else if (*p == '*'){
            token.type = Multi;
            token.value = malloc(2);
            strcpy(token.value, "*");
            p++;
        }

        else if (*p == '/'){
            token.type = Div;
            token.value = malloc(2);
            strcpy(token.value, "/");
            p++;
        }
        
        else if (*p == '('){
            token.type = GPar;
            token.value = malloc(2);
            strcpy(token.value, "()");
            p++;
        }

        else if (*p == ')'){
            token.type = DPar;
            token.value = malloc(2);
            strcpy(token.value, ")");
            p++;
        }

        //Gestion des erreurs
        else {
            printf("Erreur : '%c'", *p);
            p++;
            continue;
        }
        
        //Ajoute le token ds le tableau tokens et augmente token_count
        tokens[*token_count] = token;
        (*token_count) ++;
    }

    //Crée un token de fin pour indiquer la fin
    Token fin_token = {Fin, NULL};
    tokens[*token_count] = fin_token;

    return tokens;
}

// Fonction pour print les tokens (type et value)
void print_tokens(Token* tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        printf("Token: Type: %d, Value: %s\n", tokens[i].type, tokens[i].value);
    }
}

int main(){
    // Stocker l'input
   char input[100];
   printf("Entrez une formule: ");
   //Lit jusqu'à 99 caractère jusqu'au retour à la ligne
   scanf("%99[^\n]", input);

    //Appel des fonctions
   int token_count;
   Token* tokens = lexer(input, &token_count);
   print_tokens(tokens, token_count);

    //FREE
   for (int i = 0; i < token_count; i++){
    free(tokens[i].value);
   }
   free(tokens);
   
   //Terminé avec succès
   return 0;
}


