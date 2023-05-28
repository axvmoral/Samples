#include <cassert>
#include <cmath>
#include "cs19_recurrence.h"
 
int main() {
  constexpr double EPSILON = 1e-15;  // threshold for precision
  // test recursive phi
  assert(cs19::phi(1) == std::sqrt(1));
  assert(cs19::phi(2) == std::sqrt(1 + std::sqrt(1)));
  assert(cs19::phi(3) == std::sqrt(1 + std::sqrt(1 + std::sqrt(1))));
  assert(cs19::phi(4) == std::sqrt(1 + std::sqrt(1 + std::sqrt(1 + std::sqrt(1)))));
  double most_precise_phi = cs19::most_precise(cs19::phi);
  assert(most_precise_phi == cs19::phi(32));  // # nested-radical cs19::phi converges at 32 terms
  assert(std::abs(most_precise_phi - (1 + std::sqrt(5)) / 2) < EPSILON);  // closed-form solution
  // verify the relationship between Fibonacci sequence and phi (30 terms is within a billionth)
  assert(std::abs(cs19::fibonacci(30).get_d() / cs19::fibonacci(29).get_d() - most_precise_phi) <
         1e-9);
  // test cs19::Padovan and cs19::Perrin sequences
  assert(cs19::padovan(7) == 5 && cs19::padovan(8) == 7 && cs19::padovan(9) == 9);
  assert(cs19::perrin(7) == 7 && cs19::perrin(8) == 10 && cs19::perrin(9) == 12);
  // # test recursive rho
  assert(cs19::rho(1) == std::cbrt(1));
  assert(cs19::rho(2) == std::cbrt(1 + std::cbrt(1)));
  assert(cs19::rho(3) == std::cbrt(1 + std::cbrt(1 + std::cbrt(1))));
  assert(cs19::rho(4) == std::cbrt(1 + std::cbrt(1 + std::cbrt(1 + std::cbrt(1)))));
  double most_precise_rho = cs19::most_precise(cs19::rho);
  assert(most_precise_rho == cs19::rho(23));  // nested-radical rho hits peak precision at 23 terms
  // ensure closed-form solution is acceptably close
  assert(std::abs(most_precise_rho - (std::cbrt((9 - std::sqrt(69)) / 18) +
                                      std::cbrt((9 + std::sqrt(69)) / 18))) < EPSILON);
  // # verify the relationship between the Padovan/Perrin sequences and rho
  assert(abs(cs19::padovan(52) / cs19::padovan(51) - most_precise_rho) < 1e-9);
  assert(abs(cs19::perrin(52) / cs19::perrin(51) - most_precise_rho) < 1e-9);
}