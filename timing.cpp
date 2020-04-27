///////////////////////////////////////////////////////////////////////////////
// timing.cpp
//
// Main program that runs word-frequency time trials.
//
// Students: you do not need to modify this file.
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "words.hpp"

using namespace std;
using namespace words;

const string STOPWORDS_PATH("stopwords.txt");

void print_usage() {
  cout << "usage:" << endl
       << "    timing <STRUCTURE> <PATH>" << endl
       << endl
       << "where" << endl
       << "    <STRUCTURE> is one of: vector list hash tree" << endl
       << "    <PATH> is the path of a text file" << endl
       << endl;
}

int main(int argc, char* argv[]) {

  // parse commandline arguments

  vector<string> arguments(argv, argv + argc);

  if (arguments.size() != 3) {
    print_usage();
    return 1;
  }

  auto& structure = arguments[1],
        document_path = arguments[2];

  unique_ptr<abstract_word_counter> counter;
  if (structure == "vector") {
    counter.reset(new vector_word_counter());
  } else if (structure == "list") {
    counter.reset(new list_word_counter());
  } else if (structure == "hash") {
    counter.reset(new hash_word_counter());
  } else if (structure == "tree") {
    counter.reset(new tree_word_counter());
  } else {
    print_usage();
    return 1;
  }
  assert(counter);

  // now run a trial
  try {
    // load input
    auto stopwords = read_words(STOPWORDS_PATH),
         document = read_words(document_path);

    // print parameters
    cout << "== computing frequency distribution ==" << endl
         << "structure: " << structure << endl
	 << "stop word count: " << stopwords.size() << endl
	 << "document: " << document_path << endl
	 << "document word count: " << document.size() << endl;

    // start high resolution clock
    using clock = chrono::high_resolution_clock;
    auto start = clock::now();
    // compute the frequency distribution
    auto dist = counter->count(stopwords, document);
    // stop the clock
    auto end = clock::now();
    // print elapsed time
    double seconds = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    cout << "elapsed time: " << seconds << " seconds" << endl;

    // print top 20 most frequent words
    auto top20 = most_frequent(dist, 20);
    cout << "top 20 words:" << endl;
    for (auto& entry : top20) {
      cout << "    " << entry.word() << "    " << entry.count() << endl;
    }

  } catch (io_exception e) {
    // I/O error loading input
    cout << "error loading \"" << e.path() << "\"" << endl;
    return 1;
  }

  return 0;
}
