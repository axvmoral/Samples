/**
 * @file cs19_dna.h
 *
 * A basic implementation of a class representing DNA that inherits most of its functionality
 * from a parent class. DO NOT MODIFY THIS FILE.
 *
 * @author Jeffrey Bergamini for CS 19, jeffrey.bergamini@cabrillo.edu
 */
 
#ifndef _CS19_DNA_H
#define _CS19_DNA_H
 
#include <string>
#include "cs19_polynucleotide.h"
 
namespace cs19 {
 
/**
 * Class Dna models a mutable DNA sequence, with operators providing an idiomatic C++ interface.
 *
 * Most of the functionality is inherited from class Polynucleotide.
 */
class Dna : public Polynucleotide {
 public:
  /**
   * Default constructor.
   * Creates an empty DNA sequence.
   */
  Dna() : Polynucleotide({{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}}) {
    // nothing to do here other than call base-class constructor
  }
 
  /**
   * Copy constructor.
   * Creates a copy of a pre-existing DNA sequence.
   */
  Dna(const Dna &that) : Polynucleotide(that) {
    // nothing to do here other than call base-class constructor
  }
 
  /**
   * C string contructor: Works with any C string containing valid nucleotide characters.
   * @throws std::domain_error for any string containing invalid nucleotide characters.
   */
  Dna(const char *contents)
      : Polynucleotide(std::string(contents), {{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}}) {
    // nothing to do here other than call base-class constructor
  }
 
  /**
   * Initializer-list constructor for a list of valid DNA characters.
   * e.g. cs19::Dna{'G', 'A', 'T', 'T', 'A', 'C', 'A'}
   * @throws std::domain_error if any element in the list is an invalid DNA nucleotide character.
   */
  Dna(std::initializer_list<char> list)
      : Polynucleotide(list, {{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}}) {
    // nothing to do here other than call base-class constructor
  }
 
  /**
   * Template sequence contructor: Works with any iterable sequence of valid DNA characters.
   * @tparam Sequence iterable type containing A/C/G/T characters, e.g. std::string/std::list<char>
   * @throws std::domain_error for any sequence containing invalid DNA characters.
   */
  template <typename Sequence>
  Dna(const Sequence &contents)
      : Polynucleotide(contents, {{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}}) {
    // nothing to do here other than call base-class constructor
  }
 
  /**
   * Initializer-list constructor for a list of objects containing valid DNA characters.
   * e.g.
   * using namespace std::string_literals;
   * cs19::Dna{"GA"s, "TT"s, "ACA"s}
   * cs19::Dna{std::list{'G', 'A'}, std::list{'T', 'T'}, std::list{'A', 'C', 'A'}}
   * @tparam Sequence iterable type containing A/C/G/T characters, e.g. std::string/std::list<char>
   * @throws std::domain_error for any sequence containing invalid DNA characters.
   */
  template <typename Sequence>
  Dna(std::initializer_list<Sequence> list)
      : Polynucleotide(list, {{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}}) {
    // nothing to do here other than call base-class constructor
  }
 
  /**
   * Returns the GC-content of this sequence.
   */
  double gc_content() const {
    if (this->size() == 0)
      return 0;
    int gc_count = 0;
    for (char c : this->sequence_) {
      if (c == 'G' || c == 'C')
        ++gc_count;
    }
    return static_cast<double>(gc_count) / this->sequence_.size();
  }
};
 
}  // namespace cs19
 
#endif  // _CS19_DNA_H