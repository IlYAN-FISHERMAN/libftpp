# TP C++20 Ranges — de la notation ensembliste au pipeline

Le fil rouge : la formule `K(x) = min { l(p) : p ∈ P_M, M(p) = x }`
se lit comme un pipeline. Ce TP te fait construire chaque brique,
puis la formule entière, puis les mécanismes qui la rendent possible
(paresse, composition, projections).

## Mise en route

```
g++ -std=c++20 -Wall -Wextra -Werror tp_ranges.cpp -o tp && ./tp
```

Le fichier compile tel quel : `0/7`, tous KO. Implémente les `TODO`
dans l'ordre, recompile, relance. Objectif `7/7` (l'exécutable
renvoie alors 0).

`solutions.cpp` contient le corrigé complet. Règle piscine : tu ne
l'ouvres qu'après une vraie tentative, ou pour comparer une fois
ton exercice au vert.

## Indices par exercice

Chaque indice te donne le concept et le nom à chercher sur
cppreference — pas le code.

**ex00 — contains.** Le `∈` mathématique. Cherche `std::ranges::find`
(ou `std::ranges::any_of`). Un algorithme ranges prend le conteneur
directement, sans paire begin/end. Que renvoie `find` quand il ne
trouve rien ?

**ex01 — K.** La formule du fil rouge, sur un P_M fini. Trois étages :
`views::filter` pour la condition `M(p) == x` (capture par référence
nécessaire — pourquoi ?), `views::transform` pour extraire `l(p)`,
`std::ranges::min` pour conclure. Piège central : `min` sur un range
vide est un comportement indéfini. Comment teste-t-on qu'une view
paresseuse est vide sans la matérialiser ? (Regarde ce que comparer
`begin()` et `end()` déclenche sur un filter_view.)

**ex02 — first_n_squares.** `views::iota(1)` sans deuxième argument :
range infini. Si ton code se termine, c'est que quelque chose a borné
l'évaluation — `views::take`. Le compteur `calls` est le témoin : il
prouve que `transform` n'est appliqué que lors des déréférencements
réels. Si `calls > n`, ton pipeline évalue trop tôt ou trop large.

**ex03 — composition.** Deux choses à découvrir. (a) `views::filter(pred)`
sans range est une application partielle : l'objet retourné attend son
range via `|`. Ta fonction fabrique et renvoie cet objet — le type est
innommable, d'où le retour `auto`. (b) Deux adaptateurs se composent
entre eux : `adaptateurA | adaptateurB` donne un nouvel adaptateur,
sans aucun range en jeu. Composition de fonctions, au sens OCaml.
Bonus : fais appeler `multiples_filter` par `squares_of_multiples`
plutôt que de dupliquer le prédicat.

**ex04 — sum_squares_below.** Même squelette qu'ex02, mais la coupe
est une *condition*, pas un compte : `views::take_while`. Question à
te poser avant de coder : le prédicat doit-il porter sur v ou sur v² ?
Place le `take_while` en conséquence. L'accumulation finale se fait
en range-for (les folds propres arrivent en C++23, on reste honnête
en C++20).

**ex05 — projection.** Le troisième argument des algorithmes ranges.
`std::ranges::min_element(range, {}, projection)` : le `{}` est le
comparateur par défaut (`ranges::less`), la projection est appliquée
à chaque élément *avant* comparaison — exactement le rôle de `l(p)`
dans K. Pourquoi `min_element` plutôt que `min` ici ? Réfléchis à ce
que tu veux récupérer : la longueur, ou l'objet qui la porte ?

**ex06 — split_words.** `views::split(sep)` produit un range de
sous-ranges de `char`, pas des strings. Chaque sous-range est
contigu : `std::string` a un constructeur (first, last) qui fait
l'affaire. C'est l'exercice le plus « friction réelle » du lot —
tout le monde bute dessus la première fois.

## Après le 7/7

Trois pistes pour continuer, par difficulté croissante :

1. Réécris ex01 pour renvoyer *le programme* le plus court, pas sa
   longueur (indice : ex05 t'a donné l'outil).
2. Ajoute un `views::take(1000000)` devant un pipeline infini et
   mesure avec `calls` combien d'éléments sont réellement évalués
   selon la position du `take` dans la chaîne. Déplace-le. Observe.
3. Le vrai morceau : écris ta propre view (`enumerate_view` — chaque
   élément accompagné de son indice) avec iterator/sentinel maison.
   C'est le niveau au-dessus, et le sujet d'un prochain TP si tu veux.
