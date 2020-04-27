///////////////////////////////////////////////////////////////////////////////
// words.hpp
//
// Computing the frequency distribution of a list of words, skipping stopwords.
//
// Students: you do not need to modify this file.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace words {

  // Type alias for a vector of strings, holding words.
  using word_vector = std::vector<std::string>;

  // Exception class for read_words below.
  class io_exception {
  private:
    std::string path_;

  public:
    io_exception(const std::string& path) noexcept
    : path_(path) { }

    const std::string& path() const noexcept { return path_; }
  };

  // Read whitespace-delimited words from the file at the given path,
  // and return the result as a word_vector. Throws io_exception on
  // error.
  word_vector read_words(const std::string& path);

  // One word, and its frequency (number of occurrences).
  class word_frequency {
  private:
    std::string word_;
    unsigned count_;

  public:

    // Default constructor, initializes word to empty and count to
    // zero.
    word_frequency() noexcept
    : word_(), count_(0) { }

    // Initialization constructor.
    word_frequency(const std::string& word, unsigned count) noexcept
    : word_(word), count_(count) { }
    word_frequency(const std::string&& word, unsigned count) noexcept
    : word_(std::move(word)), count_(count) { }

    // accessors and mutators

    const std::string& word() const noexcept { return word_; }

    unsigned count() const noexcept { return count_; }
    void set_count(unsigned count) noexcept { count_ = count; }
    void increment_count() noexcept { ++count_; }
  };

  // A frequency_distribution is a vector of word_frequency objects.
  using frequency_distribution = std::vector<word_frequency>;

  // most_frequent takes a frequency_distribution, and non-negative
  // parameter k, and returns a frequency_distribution that:
  //
  // - Is sorted in order from most-frequent to least-frequent. Ties
  // - are broken by taking the words in alphabetical order (so "a"
  // - comes before "b").
  //
  // - Contains at most k words.
  //
  frequency_distribution
  most_frequent(
    const frequency_distribution& dist,
    size_t k
  ) noexcept;

  // Abstract base class for classes that can compute a
  // frequency_distribution.
  class abstract_word_counter {
  public:
    virtual ~abstract_word_counter() {}

    // Given stop_words and document, create and return a
    // frequency_distribution containing the count of every unique
    // word that appears in document. Words contained in stop_words
    // are excluded from the frequency_distribution (skipped). Words
    // are case-insensitive, so "hello" and "Hello" are considered the
    // same word. Every word in the returned frequency_distribution is
    // lower case.
    virtual frequency_distribution
    count(
      const word_vector& stop_words,
      const word_vector& document
    ) const noexcept = 0;
  };

  // Word counter that uses an unsorted vector (std::vector) to search
  // for each word_frequency.
  class vector_word_counter : public abstract_word_counter {
  public:
    virtual frequency_distribution
    count(
      const word_vector& stop_words,
      const word_vector& document
    ) const noexcept;
  };

  // Word counter that uses an unsorted linked list (std::list) to
  // search for each word_frequency.
  class list_word_counter : public abstract_word_counter {
  public:
    virtual frequency_distribution
    count(
      const word_vector& stop_words,
      const word_vector& document
    ) const noexcept;
  };

  // Word counter that uses a hash table (std::unordered_map) to
  // search for each word_frequency.
  class hash_word_counter : public abstract_word_counter {
  public:
    virtual frequency_distribution
    count(
      const word_vector& stop_words,
      const word_vector& document
    ) const noexcept;
  };

  // Word counter that uses a binary search tree (std::map) to search
  // for each word_frequency.
  class tree_word_counter : public abstract_word_counter {
  public:
    virtual frequency_distribution
    count(
      const word_vector& stop_words,
      const word_vector& document
    ) const noexcept;
  };
}
