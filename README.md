# PLI-
Projet ESGI A3 : Creation of a Programming Language Interpreter


Plan :


Semaine 1-2 : Calculatrice de base et entrée simple
Objectif principal : Implémenter les bases de l’interpréteur avec des expressions arithmétiques et des variables.

Étape 1 : Création du Lexer (Analyseur lexical)
Comprendre les entrées : D'abord, on doit comprendre quel type d’entrées on va recevoir. Pour cette étape, il s’agit principalement d’opérateurs arithmétiques, de nombres et de variables.
Lexage des tokens : Il faut écrire une fonction qui prend une chaîne de caractères et la découpe en éléments ("tokens"). Par exemple :
Les nombres (10, 3, etc.)
Les opérateurs (+, -, *, /)
Les identifiants de variables (x, y)
Les parenthèses et autres symboles ((, )).
Test des tokens : Après avoir écrit cette fonction, tu dois tester si ton programme convertit bien une chaîne en liste de tokens.
Étape 2 : Création du Parser (Analyseur syntaxique)
Définir les règles : Le Parser prend les tokens et les organise selon les règles de syntaxe. Par exemple, l'expression 3 + 5 * 2 devrait être comprise comme 3 + (5 * 2) (priorité des opérations).
Construire l'AST : Utilise un algorithme comme "Shunting Yard" pour convertir les tokens en une structure d'arbre (AST).
Implémentation : Crée une fonction qui prend les tokens et construit un arbre de syntaxe abstraite (AST) pour les expressions arithmétiques de base.
Étape 3 : Évaluation des expressions
Parcours de l'AST : Implémente une fonction pour parcourir l’AST et calculer le résultat de l’expression.
Variables et stockage : Crée une table de symboles (comme un dictionnaire ou une table de hachage) pour stocker les variables et leurs valeurs. Par exemple, après x = 5, l’interpréteur doit se souvenir que x vaut 5.
Étape 4 : Ajout des fonctions intégrées
Fonction print : Ajoute la fonction print() pour afficher des variables ou des résultats d'expressions.
Tester avec des cas simples : Teste ton interpréteur avec des exemples simples comme x = 10; print(x); print(5 + 2);.


Semaine 2-3 : Gestion des entrées
Objectif principal : Ajouter différents formats d’entrée pour les instructions (mode fichier et interactif).

Étape 1 : Lecture à partir de fichiers
Lecture ligne par ligne : Implémente la lecture d’un fichier qui contient du code source. Utilise des fonctions comme fopen et fgets.
Interprétation du fichier : Pour chaque ligne lue, le lexer et le parser doivent être appelés pour l’analyser et l’exécuter.
Étape 2 : Mode interactif
REPL : Crée une boucle d’entrée (Read-Eval-Print Loop ou REPL) pour accepter des commandes de l’utilisateur en direct.
Commandes spéciales : Ajoute une commande exit pour quitter le mode interactif.
Étape 3 : Tests complets
Combiner tout : Assure-toi que ton interpréteur fonctionne à la fois avec des fichiers et en mode interactif.
Affichage de l’AST (bonus) : Si tu utilises un AST, ajoute une fonction pour afficher l’arbre de manière lisible.


Semaine 3-4 : Structures de contrôle et chaînes de caractères
Objectif principal : Ajouter des structures de contrôle (boucles, conditions) et la gestion des chaînes de caractères.

Étape 1 : Implémenter les boucles while et for
Reconnaissance des boucles : Modifie le lexer et le parser pour reconnaître les mots-clés while et for.
Évaluation des boucles : Implémente la logique des boucles dans l’évaluateur, en répétant des blocs de code selon la condition.
Étape 2 : Conditions if et else
Parser des conditions : Reconnais et analyse la structure if/else dans le code.
Évaluation des conditions : Exécute le bloc de code approprié selon la condition.
Étape 3 : Chaînes de caractères
Ajout des chaînes au Lexer : Ajoute un nouveau type de token pour les chaînes (par exemple "Bonjour").
Manipulation des chaînes : Implémente la gestion des opérations simples comme la concaténation (+).
Étape 4 : Gestion de la portée des variables
Portée locale vs globale : Implémente un système de portée pour que les variables aient une durée de vie différente en fonction de l’endroit où elles sont définies (dans des boucles, conditions, etc.).
Piles de symboles : Utilise une pile pour suivre les portées imbriquées des variables.


Semaine 4-5 : Fonctions
Objectif principal : Permettre la définition et l'appel de fonctions.

Étape 1 : Déclaration de fonctions
Parser des fonctions : Ajoute la reconnaissance des définitions de fonctions dans le parser.
Stockage des fonctions : Crée une structure de données pour stocker les fonctions (nom, paramètres, corps).
Étape 2 : Appel de fonctions et gestion de la pile
Appeler une fonction : Implémente l’appel de fonction avec les arguments fournis et assure-toi que la pile d’appels fonctionne correctement.
Valeur de retour : Ajoute la gestion des valeurs de retour dans les fonctions.
Étape 3 : Gestion des variables locales et globales
Variables locales : Lors de l’appel d’une fonction, les variables déclarées dans la fonction doivent être locales et les paramètres doivent être correctement transmis.
Variables globales : Les variables globales doivent rester accessibles dans les fonctions, sauf si elles sont masquées par des variables locales.






