/**
 * @file dna_tests.cpp
 *
 * Several tests of class cs19::Dna.
 *
 * @author Jeffrey Bergamini for CS 19, jeffrey.bergamini@cabrillo.edu
 * @author A Cabrillo student for CS 19, someone@jeff.cis.cabrillo.edu
 */
 
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
 
#include "cs19_dna.h"
 
// A bunch of tests with GATTACA. Can't get enough GATTACA! 😉
int main() {
  std::map<char, int> counts{{'A', 3}, {'C', 1}, {'G', 1}, {'T', 2}};
  for (cs19::Dna test : std::vector{cs19::Dna("GATTACA"), cs19::Dna(std::string("GATTACA")),
                                    cs19::Dna{'G', 'A', 'T', 'T', 'A', 'C', 'A'},
                                    cs19::Dna{std::string("GATT"), std::string("ACA")}}) {
    assert(test.size() == 7);
    std::string string_version = test.to_string();
    assert(string_version == "GATTACA");
    string_version.clear();
    assert(test.nucleotide_counts() == counts);
    assert(test.gc_content() == 2.0 / 7);
    assert(~test == cs19::Dna("CTAATGT"));
    assert(-test == cs19::Dna("ACATTAG"));
    assert(test.hamming_distance(-~test) == 5);
    std::stringstream stream;
    stream << test;
    assert(stream.str() == "GATTACA");
    assert(test[0] == 'G');
    assert(test[test.size() - 1] == 'A');
    test.set(0, 'C');
    assert(test[0] == 'C');
    test.clear();
    assert(test.size() == 0);
    assert(test.to_string() == "");
    test += "GATTACA";
    assert(test.nucleotide_counts() == counts);
    assert(test + test == cs19::Dna("GATTACAGATTACA"));
    assert(test * 4 == cs19::Dna("GATTACAGATTACAGATTACAGATTACA"));
    //cs19::Dna o = test*4;
    test += 'T';
    assert(test.to_string() == "GATTACAT");
    test += test;
    assert(test.to_string() == "GATTACATGATTACAT");
    try {
      test += "this had better not work!";
      assert(false);
    } catch (std::domain_error &err) {
      assert(true);
    }
    test *= 2;
    assert(test.to_string() == "GATTACATGATTACATGATTACATGATTACAT");
    assert(test.subsequence(5, 3) == cs19::Dna("CAT"));
    try {
      test.hamming_distance(cs19::Dna("GATTACA"));
      assert(false);
    } catch (std::domain_error &err) {
      assert(true);
    }
  }
  try {
    cs19::Dna("GAUUACA");
    assert(false);
  } catch (std::domain_error &err) {
    assert(true);
  }
}