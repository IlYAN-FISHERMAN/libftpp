// ============================================================
//  TP C++20 RANGES — de la notation ensembliste au pipeline
//
//  Règles du jeu :
//    - Ce fichier compile tel quel. Tous les tests sont KO.
//    - Implémente les fonctions marquées TODO, une par une,
//      dans l'ordre. Recompile, relance, passe au vert.
//    - Interdit : boucles indicées (for (i = 0; ...)), push_back
//      dans un filtre manuel. Tout passe par <ranges> et
//      <algorithm> (std::ranges::...), sauf l'accumulation
//      finale où un range-for est autorisé.
//    - La moulinette en bas de fichier ne se modifie pas.
//
//  Compilation :
//    g++ -std=c++20 -Wall -Wextra -Werror tp_ranges.cpp -o tp && ./tp
//
//  Indices dans le README, solutions dans solutions.cpp
//  (à ne lire qu'après une vraie tentative — tu connais la règle).
// ============================================================

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

// ============================================================
// ex00 — contains : le symbole ∈ en code
//
//   Écris `value ∈ v` en C++ : true si value apparaît dans v.
//   Contrainte : pas de boucle, un seul appel à un algorithme
//   de std::ranges.
// ============================================================
bool contains([[maybe_unused]] const std::vector<int>& v,
              [[maybe_unused]] int                     value)
{
	return std::find(v.begin(), v.end(), value) != v.end();
}

// ============================================================
// ex01 — K : la formule de Kolmogorov (version finie)
//
//   K(x) = min { l(p) : p ∈ P_M, M(p) = x }
//
//   Traduis la formule mot à mot en pipeline :
//   filter (la condition après ":"), transform (l'expression
//   l(p)), puis min. Si aucun p ne convient, renvoie nullopt.
//   Attention : min sur un range vide est UB — teste d'abord.
// ============================================================
std::optional<std::size_t> K(
    [[maybe_unused]] const std::vector<std::string>&                       P_M,
    [[maybe_unused]] const std::function<std::string(const std::string&)>& M,
    [[maybe_unused]] const std::string&                                    x)
{
    // TODO
    return std::nullopt;
}

// ============================================================
// ex02 — first_n_squares : la paresse sur un range infini
//
//   Renvoie les n premiers carrés (1, 4, 9, ...) en partant
//   d'un range INFINI (views::iota sans borne). Le transform
//   doit incrémenter `calls` à chaque application.
//   Le test vérifie que calls == n EXACTEMENT : si ton
//   pipeline évalue plus d'éléments que nécessaire, c'est KO.
//   C'est ça, la paresse.
// ============================================================
std::vector<int> first_n_squares([[maybe_unused]] int  n,
                                 [[maybe_unused]] int& calls)
{
    // TODO
    return {};
}

// ============================================================
// ex03 — application partielle & composition d'adaptateurs
//
//   a) multiples_filter(k) : renvoie un adaptateur (closure)
//      qui garde les multiples de k. Utilisable en `v | ...`.
//      C'est l'équivalent d'une application partielle OCaml.
//   b) squares_of_multiples(k) : COMPOSE multiples_filter(k)
//      avec un transform qui élève au carré — avec `|` entre
//      les deux adaptateurs, SANS range. Oui, ça se compose
//      comme des fonctions.
// ============================================================
auto multiples_filter([[maybe_unused]] int k)
{
    // TODO
    return std::views::filter([](int) { return false; });
}

auto squares_of_multiples([[maybe_unused]] int k)
{
    // TODO
    return std::views::filter([](int) { return false; });
}

// ============================================================
// ex04 — sum_squares_below : pipeline infini borné par condition
//
//   Somme des carrés strictement inférieurs à `limit`.
//   Source infinie (iota), coupée non pas par un COMPTE
//   (take) mais par une CONDITION. Accumulation finale :
//   range-for autorisé.
// ============================================================
long sum_squares_below([[maybe_unused]] long limit)
{
    // TODO
    return 0;
}

// ============================================================
// ex05 — shortest_name : les projections (le l(p) des algos)
//
//   Renvoie le nom du Program au code le plus court.
//   Contrainte : UN appel à std::ranges::min_element avec
//   une PROJECTION (3e argument) — pas de transform, pas de
//   comparateur maison. La projection, c'est exactement le
//   rôle de l(p) dans la formule de K.
// ============================================================
struct Program
{
    std::string name;
    std::string code;
};

std::string shortest_name([[maybe_unused]] const std::vector<Program>& progs)
{
    // TODO
    return "";
}

// ============================================================
// ex06 — split_words : découper une string avec views::split
//
//   "a;b;c" -> {"a", "b", "c"}
//   views::split produit des sous-ranges, pas des strings :
//   à toi de construire chaque std::string à partir des
//   itérateurs du sous-range. Accumulation : range-for autorisé.
// ============================================================
std::vector<std::string> split_words([[maybe_unused]] std::string_view s,
                                     [[maybe_unused]] char             sep)
{
    // TODO
    return {};
}

// ============================================================
//                    MOULINETTE — NE PAS MODIFIER
// ============================================================
namespace moulinette {

int g_pass = 0;
int g_fail = 0;

void check(const char* name, bool ok)
{
    if (ok) { ++g_pass; std::cout << "\033[32m[OK]\033[0m " << name << '\n'; }
    else    { ++g_fail; std::cout << "\033[31m[KO]\033[0m " << name << '\n'; }
}

} // namespace moulinette

int main()
{
    using moulinette::check;

    // ---- ex00 ----
    {
        std::vector<int> v{3, 1, 4, 1, 5};
        check("ex00_contains", contains(v, 4) && !contains(v, 9));
    }

    // ---- ex01 ----
    {
        // M supprime les voyelles : M("banana") == "bnn"
        auto M = [](const std::string& p) {
            std::string out;
            for (char c : p)
                if (std::string_view("aeiouy").find(c) == std::string_view::npos)
                    out += c;
            return out;
        };
        std::vector<std::string> P_M{"banana", "bnn", "bonne", "bn"};
        bool ok = K(P_M, M, "bnn") == std::optional<std::size_t>{3}   // "bnn" gagne
               && K(P_M, M, "zzz") == std::nullopt;                    // aucun programme
        check("ex01_kolmogorov", ok);
    }

    // ---- ex02 ----
    {
        int calls = 0;
        auto v = first_n_squares(3, calls);
        bool ok = v == std::vector<int>{1, 4, 9} && calls == 3;
        check("ex02_lazy", ok);
    }

    // ---- ex03 ----
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        std::vector<int> a, b;
        for (int x : v | multiples_filter(3))
            a.push_back(x);
        for (int x : v | squares_of_multiples(2))
            b.push_back(x);
        bool ok = a == std::vector<int>{3, 6} && b == std::vector<int>{4, 16, 36};
        check("ex03_compose", ok);
    }

    // ---- ex04 ----
    {
        // carrés < 100 : 1+4+9+16+25+36+49+64+81 = 285
        check("ex04_take_while", sum_squares_below(100) == 285
                              && sum_squares_below(2)   == 1);
    }

    // ---- ex05 ----
    {
        std::vector<Program> progs{
            {"alpha", "return 42;"},
            {"beta",  "x;"},
            {"gamma", "while(1);"},
        };
        check("ex05_projection", shortest_name(progs) == "beta");
    }

    // ---- ex06 ----
    {
        auto w = split_words("min;filter;transform", ';');
        bool ok = w == std::vector<std::string>{"min", "filter", "transform"};
        check("ex06_split", ok);
    }

    std::cout << "\n=== " << moulinette::g_pass << "/"
              << (moulinette::g_pass + moulinette::g_fail)
              << " exercices valides ===\n";
    return moulinette::g_fail == 0 ? 0 : 1;
}
