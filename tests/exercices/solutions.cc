// ============================================================
//  TP C++20 RANGES — SOLUTIONS
//  À ne consulter qu'après avoir tenté chaque exercice !
//  Compilation : g++ -std=c++20 -Wall -Wextra -Werror solutions.cpp -o tp
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
// ============================================================
bool contains(const std::vector<int>& v, int value)
{
    return std::ranges::find(v, value) != v.end();
}

// ============================================================
// ex01 — K : la formule de Kolmogorov (version finie)
//   K(x) = min { l(p) : p ∈ P_M, M(p) = x }
// ============================================================
std::optional<std::size_t> K(const std::vector<std::string>&                 P_M,
                             const std::function<std::string(const std::string&)>& M,
                             const std::string&                              x)
{
    auto lens = P_M
        | std::views::filter([&](const std::string& p) { return M(p) == x; })
        | std::views::transform([](const std::string& p) { return p.size(); });

    if (lens.begin() == lens.end())
        return std::nullopt;
    return std::ranges::min(lens);
}

// ============================================================
// ex02 — first_n_squares : la paresse sur un range infini
// ============================================================
std::vector<int> first_n_squares(int n, int& calls)
{
    auto squares = std::views::iota(1)
        | std::views::transform([&calls](int v) { ++calls; return v * v; })
        | std::views::take(n);

    std::vector<int> out;
    for (int s : squares)
        out.push_back(s);
    return out;
}

// ============================================================
// ex03 — application partielle & composition d'adaptateurs
// ============================================================
auto multiples_filter(int k)
{
    return std::views::filter([k](int v) { return v % k == 0; });
}

auto squares_of_multiples(int k)
{
    return multiples_filter(k)
         | std::views::transform([](int v) { return v * v; });
}

// ============================================================
// ex04 — sum_squares_below : pipeline infini borné par take_while
// ============================================================
long sum_squares_below(long limit)
{
    auto squares = std::views::iota(1L)
        | std::views::transform([](long v) { return v * v; })
        | std::views::take_while([limit](long sq) { return sq < limit; });

    long sum = 0;
    for (long s : squares)
        sum += s;
    return sum;
}

// ============================================================
// ex05 — shortest_name : les projections (le l(p) des algos)
// ============================================================
struct Program
{
    std::string name;
    std::string code;
};

std::string shortest_name(const std::vector<Program>& progs)
{
    auto it = std::ranges::min_element(
        progs, {}, [](const Program& p) { return p.code.size(); });
    return it->name;
}

// ============================================================
// ex06 — split_words : découper une string avec views::split
// ============================================================
std::vector<std::string> split_words(std::string_view s, char sep)
{
    std::vector<std::string> out;
    for (auto sub : s | std::views::split(sep))
        out.emplace_back(sub.begin(), sub.end());
    return out;
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
