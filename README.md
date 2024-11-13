# PLI-
Projet ESGI A3 : Creation of a Programming Language Interpreter


Comment fonctionne un interpréteur ?

Pour comprendre comment construire un interpréteur, examinons les étapes principales d’exécution d’un code source dans un interpréteur.

Analyse lexicale (lexing) : Cette étape consiste à transformer le code source en une série de tokens. Un token est une unité syntaxique élémentaire (par exemple, un mot-clé, un opérateur, un identifiant de variable). L'objectif est de transformer chaque morceau de texte significatif en une structure compréhensible pour l'interpréteur.

Analyse syntaxique (parsing) : Une fois les tokens créés, on passe à l'analyse syntaxique, où on transforme cette liste de tokens en une structure hiérarchique appelée AST (Arbre de Syntaxe Abstraite). Cet arbre montre la structure du code source et est crucial pour comprendre l’ordre des opérations, comme la priorité des opérateurs (par exemple, dans 5 + 3 * 2, la multiplication se fait avant l’addition).

Évaluation (interpretation) : L’interpréteur parcourt l'AST et exécute chaque nœud, dans l’ordre indiqué par l'arbre. Par exemple :

Lorsqu'il atteint un nœud de type "assignation", il stocke la valeur dans une variable. Lorsqu'il atteint un opérateur, il effectue l'opération (addition, soustraction, etc.). Il évalue les expressions en suivant la logique de l'AST.



https://trello.com/b/w24MD7Ln/pli
