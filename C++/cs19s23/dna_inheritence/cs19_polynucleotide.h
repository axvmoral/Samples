/**
 * @file cs19_polynucleotide.h
 *
 * A basic implementation of a class meant to serve as a base class for specific polynucleotides
 * such as DNA and RNA.
 *
 * @author Jeffrey Bergamini for CS 19, jeffrey.bergamini@cabrillo.edu
 * @author A Cabrillo student for CS 19, someone@jeff.cis.cabrillo.edu
 */
#ifndef _CS19_POLYNUCLEOTIDE_H
#define _CS19_POLYNUCLEOTIDE_H

#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>

namespace cs19 {

/**
 * Class Polynucleotide models a class representing a mutable nucleic acid sequence, with operators
 * providing an idiomatic C++ interface, meant to serve as a base class for classes modeling
 * specific polynucleotides such as DNA and RNA.
 */
class Polynucleotide {
 public:
  /**
   * Default constructor: Creates an empty sequence.
   *
   * @param complements a mapping of valid nucleotides to their complementary nucleotides
   */
  Polynucleotide(std::map<char, char> complements) {
    this->complements_ = complements;
  }

  /**
   * Copy constructor: Copies from a pre-existing Polynucleotide.
   *
   * @param extant a pre-existing Polynucleotide object
   */
  Polynucleotide(const Polynucleotide &extant) {
    this->complements_ = extant.complements_;
    std::string ext_sequence = extant.to_string();
    for (const char c : ext_sequence) {
      this->operator+=(c);
    }
  }

  /**
   * C string contructor: Creates a sequence from a C string containing nucleotide characters.
   *
   * @param contents a C string containing nucleotide characters
   * @param complements a mapping of valid nucleotides to their complementary nucleotides
   * @throws std::domain_error for any string containing invalid nucleotide characters
   */
  Polynucleotide(const char *contents, std::map<char, char> complements) {
    this->complements_ = complements;
    for (; *contents; ++contents) {
      this->operator+=(*contents);
    }
  }

  /**
   * Initializer-list constructor: Creates a sequence from an initializer_list of characters.
   *
   * @param list a list of nucleotide characters
   * @param complements a mapping of valid nucleotides to their complementary nucleotides
   * @throws std::domain_error if any element in the list is an invalid nucleotide character
   */
  Polynucleotide(std::initializer_list<char> list, std::map<char, char> complements) {
    this->complements_ = complements;
    for (const char& c : list) {
      this->operator+=(c);
    }
  }

  /**
   * Template sequence contructor: Creates a sequence from an existing sequence object.
   *
   * @tparam Sequence iterable sequence type containing characters e.g. std::string/std::list<char>
   * @param contents a sequence containing nucleotide characters
   * @param complements a mapping of valid nucleotides to their complementary nucleotides
   * @throws std::domain_error for any sequence containing invalid nucleotide characters
   */
  template <typename Sequence>
  Polynucleotide(const Sequence &contents, std::map<char, char> complements) {
    this->complements_ = complements;
    this->operator+=(contents);  // reuse compound assignment/addition function
  }

  /**
   * Initializer-list constructor: Creates a sequence from an initializer_list of sequences.
   * @tparam Sequence iterable sequence type containing characters e.g. std::string/std::list<char>
   *
   * @param list a list of sequences containing nucleotide characters
   * @param complements a mapping of valid nucleotides to their complementary nucleotides
   * @throws std::domain_error for any sequence containing invalid nucleotide characters
   */
  template <typename Sequence>
  Polynucleotide(std::initializer_list<Sequence> list, std::map<char, char> complements) {
    this->complements_ = complements;
    for (Sequence initial_sequence : list)
      this->operator+=(initial_sequence);  // reuse compound assignment/addition function
  }

  /**
   * Compound addition/assignment:
   * Appends valid nucleotide characters from a pre-existing Polynucleotide to this sequence.
   * @param appendage the sequence to append
   * @throws std::domain_error if the appendage has a different complement mapping
   */
  Polynucleotide &operator+=(const Polynucleotide &appendage) {
    std::string app_sequence = appendage.to_string();
    std::map<char, char> app_comp = appendage.complements_;
    for (const char& c : app_sequence) {
      if (app_comp[c] == this->complements_[c]) {
        this->sequence_ += c;
      } else {
        throw std::domain_error("Different Mapping");
      }
    }
    return *this;
  }

  /**
   * Compound addition/assignment:
   * Appends valid nucleotide characters from a string to this sequence.
   * @param appendage the sequence to append
   * @throws std::domain_error if the appendage contains invalid characters
   */
  Polynucleotide &operator+=(const std::string &appendage) {
    for (const char& c : appendage) {
      if (this->complements_[c]) {
        this->sequence_ += c;
      } else {
        throw std::domain_error("Invalid Character");
      }
    }
    return *this;
  }

  /**
   * Compound addition/assignment:
   * Appends valid nucleotide characters from a C string to this sequence.
   * @param appendage the sequence to append
   * @throws std::domain_error if the appendage contains invalid characters
   */
  Polynucleotide &operator+=(const char *appendage) {
    for (; *appendage; ++appendage) {
      if (this->complements_[*appendage]) {
        this->sequence_ += *appendage;
      } else {
        throw std::domain_error("Invalid Character");
      }
    }
    return *this;
  }

  /**
   * Compound addition/assignment: Appends one nucleotide character to this sequence.
   * @param nucleotide the character to append
   * @throws std::domain_error if nucleotide is an invalid character
   */
  Polynucleotide &operator+=(char nucleotide) {
    if (this->complements_[nucleotide]) {
      this->sequence_ += nucleotide;
      return *this;
    } else {
      throw std::domain_error("Invalid Character");
    }
  }

  /**
   * Compound multiplication/assignment:
   * Repeats/concatenates this sequence a number of times.
   *
   * @param repeat_count the number of times to repeat the sequence
   * @throws std::domain_error if repeat_count is negative
   */
  Polynucleotide &operator*=(int repeat_count) {
    if (repeat_count < 0) {
      throw std::domain_error("Repeat count is negative");
    } else {
      for (int i = 0; i < repeat_count; ++i) {
        this->sequence_ += this->sequence_;
      }
      return *this;
    }
  }

  /**
   * const subscript: Returns the nucleotide at the given position.
   */
  char operator[](std::size_t pos) const {
    return this->sequence_[pos];
  }

  /**
   * Complement operator: Returns this sequence's complementary sequence.
   */
  Polynucleotide operator~() const {
    cs19::Polynucleotide complement_sequence(this->complements_);
    std::map<char, char> comp = this->complements_;
    for (const char c : this->to_string()) {
      complement_sequence += comp[c];
    }
    return complement_sequence;
  }

  /**
   * Unary minus operator: Returns this sequence in reverse.
   */
  Polynucleotide operator-() const {
    cs19::Polynucleotide reverse_sequence(this->complements_);
    for (auto pos = this->sequence_.crbegin(); pos != this->sequence_.crend(); ++pos) {
      reverse_sequence += *pos;
    }
    return reverse_sequence;
  }

  /**
   * Addition operator: Returns this sequence concatenated with another.
   */
  Polynucleotide operator+(const Polynucleotide &that) const {
    cs19::Polynucleotide concatenated(this->complements_);
    concatenated.operator+=(this->sequence_ + that.sequence_);
    return concatenated;
  }

  /**
   * Multiplication operator: Returns this sequence repeated/concatenated a number of times.
   *
   * @param repeat_count the number of times to repeat the sequence
   * @throws std::domain_error if repeat_count is negative
   */
  Polynucleotide operator*(int repeat_count) const {
    if (repeat_count < 0) {
      throw std::domain_error("Repeat count is negative");
    } else {
      cs19::Polynucleotide repeated(this->complements_);
      for (int i = 0; i < repeat_count; ++i) {
        repeated += this->sequence_;
      }
      return repeated;
    }
  }

  /**
   * Assignment operator: Copies another polynucleotide's attributes to this object.
   */
  Polynucleotide &operator=(const Polynucleotide &that) {
    this->sequence_ = that.to_string();
    this->complements_ = that.complements_;
    return *this;
  }

  /**
   * Logical equality:
   * Two Polynucleotide objects compare equal if they contain the same sequence of nucleotides.
   */
  bool operator==(const Polynucleotide &that) const {
    return this->sequence_ == that.sequence_;
  }

  /**
   * Stream insertion operator: object will appear as a plain string sequence, e.g. "GATTACA".
   */
  friend std::ostream &operator<<(std::ostream &out, const Polynucleotide &polynucleotide) {
    std::string seq = polynucleotide.to_string();
    std::size_t len = seq.size();
    for (int i = 0; i < len; ++i) {
      out << seq[i];
    }
    return out;
  }

  /**
   * Removes all elements from this sequence, resulting in an empty sequence.
   */
  void clear() {
    this->sequence_ = "";
  }

  /**
   * Returns a string containing this sequence's nucleotides.
   */
  std::string to_string() const {
    return this->sequence_;  // Will return a *copy* of our private, encapsulated string object
  }

  /**
   * Returns the length of this sequence.
   */
  std::size_t size() const {
    return this->sequence_.size();
  }

  /**
   * Sets the value of the nucleotide at the given position to the given value.
   * @param pos the position/index to set
   * @param nucleotide the new value for (*this)[pos]
   * @throws std::domain_error if nucleotide is an invalid character
   */
  void set(std::size_t pos, char nucleotide) {
    if (this->complements_[nucleotide]) {
      this->sequence_[pos] = nucleotide;
    } else {
      throw std::domain_error("Invalid Character");
    }
  }

  /**
   * Returns a newly constructed sequence object with its value initialized to a copy of a
   * subsequence of this object. The substring is the portion of the object that starts at character
   * position pos and spans len characters (or until the end of the string, whichever comes first).
   * @param pos position of the first nucleotide to be copied as a subsequence. If this is equal to
   * the string length, the function returns an empty sequence. If this is greater than the string
   * length, it throws out_of_range. Note: The first nucleotide is denoted by a value of 0 (not 1).
   * @param len number of nucleotides to include in the subsequence (if the sequence is shorter, as
   * many nucleotides as possible are used).
   */
  Polynucleotide subsequence(std::size_t pos, std::size_t count) const {
    std::string sub_sequence = this->sequence_.substr(pos, count);
    cs19::Polynucleotide sub_seq(this->complements_);
    sub_seq += sub_sequence;
    return sub_seq;
  }

  /**
   * Computes the Hamming distance between this sequence and another.
   * @param that the other sequence
   * @return the Hamming distance
   * @throws std::domain_error if the two sequences are of unequal length
   */
  int hamming_distance(const Polynucleotide &that) {
    std::string that_sequence = that.to_string();
    std::size_t len = that_sequence.size();
    if (len != this->sequence_.size()) {
      throw std::domain_error("String sizes not equal");
    } else {
      int hamming_distance = 0;
      for (int i = 0; i < len; ++i) {
        if (that_sequence[i] != this->sequence_[i]) {
          ++hamming_distance;
        }
      }
      return hamming_distance;
    }
  }

  /**
   * Returns a mapping of each unique nucleotide in the sequence to its frequency in the sequence.
   */
  std::map<char, int> nucleotide_counts() const {
    std::map<char, int> nuc_counts;
    for (const char& c : this->sequence_) {
      ++nuc_counts[c];
    }
    return nuc_counts;
  }

  // Convenience functions to make a Polynucleotide const-iterable
  auto cbegin() const {
    return this->sequence_.cbegin();
  }
  auto cend() const {
    return this->sequence_.cend();
  }
  auto crbegin() const {
    return this->sequence_.crbegin();
  }
  auto crend() const {
    return this->sequence_.crend();
  }

 protected:
  std::map<char, char> complements_;  // use this for determining valid nucleotides and complements
  std::string sequence_;              // we'll store the actual nucleotide sequence here
};

}  // namespace cs19

#endif  // _CS19_POLYNUCLEOTIDE_H