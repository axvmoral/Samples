#ifndef CS19_SEARCH_SORT_H_
#define CS19_SEARCH_SORT_H_

/**
 * Namespace cs19 contains functions for our search and sort assignment.
 *
 * @author Axel V. Morales Sanchez, asmorales@jeff.cis.cabrillo.edu
 */
namespace cs19 {

/**
 * Returns an iterator to the first element in the range `[first,last)` that compares equal to
 * `val`. If no such element is found, the function returns `last`.
 *
 * @tparam Iterator a position iterator that supports the [standard input iterator
 * operations](http://www.cplusplus.com/reference/iterator/InputIterator/)
 * @tparam Value a value type that supports `operator==`
 *
 * @param first the initial position in the sequence to be sorted
 * @param last one element past the final position in the sequence to be sorted
 * @param val the value for which to search
 * @return an iterator to the first element in the range that compares equal to `val`. If no
 * elements match, the function returns `last`.
 */
template <typename Iterator, typename Value>
Iterator linear_search(Iterator first, Iterator last, const Value &val) {
  for (auto pos = first; pos != last; ++pos) {
    if (*pos == val) {
      return pos;
    }
  }
  return last;
}

/**
 * Performs an index-based linear search on an indexable object for a given value.
 *
 * @tparam IndexedContainer must support `operator[]` and `size()`, e.g. `std::vector`. Container
 * elements must be of template type `Value`.
 * @tparam Value a value type that supports `operator==`
 *
 * @param haystack the object to search
 * @param needle the value for which to search
 * @return the first index found via linear search at which `haystack` contains `needle`, or `-1` if
 * `haystack` does not contain `needle`
 */
template <typename IndexedContainer, typename Value>
int linear_search(const IndexedContainer &haystack, const Value &needle) {
  int len = haystack.size();
  for (int i = 0; i != len; ++i) {
    if (haystack[i] == needle) {
      return i;
    }
  }
  return -1;
}

/**
 * Performs an index-based binary search on an indexable object for a given value.
 *
 * @tparam IndexedContainer must support `operator[]` and `size()`, e.g. `std::vector`. Container
 * elements must be of template type `Value`.
 * @tparam Value a value type that supports `operator==` and `operator<`
 *
 * @param haystack the object to search
 * @param needle the value for which to search
 * @return the first index found via binary search at which `haystack` contains `needle`, or `-1` if
 * `haystack` does not contain `needle`
 */
template <typename IndexedContainer, typename Value>
int binary_search(const IndexedContainer &haystack, const Value &needle) {
  int low = 0;
  int high = haystack.size() - 1;
  while (low <= high) {
    int mid = (low + high) / 2;
    if (haystack[mid] == needle) {
      return mid;
    } else if (needle < haystack[mid]) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return -1;
}

/**
 * Performs an index-based bubble sort on any indexable container object.
 *
 * @tparam IndexedContainer must support `operator[]` and `size()`, e.g. `std::vector`. Container
 * elements must support `operator<` and `operator=`.
 *
 * @param values the object to sort
 */
template <typename IndexedContainer>
void bubble_sort(IndexedContainer &values) {
  bool swapped = true;
  int len = values.size();
  while (swapped) {
    swapped = false;
    for (int i = 0; i != len - 1; i++) {
      auto a = values[i];
      auto b = values[i + 1];
      if (b < a) {
        values[i] = b;
        values[i + 1] = a;
        swapped = true;
      }
    }
  }
}

/**
 * Sorts the elements in the range `[first,last)` into ascending order, using the bubble-sort
 * algorithm. The elements are compared using `operator<`.
 *
 * @tparam Iterator a position iterator that supports the [standard bidirectional iterator
 * operations](http://www.cplusplus.com/reference/iterator/BidirectionalIterator/)
 *
 * @param first the initial position in the sequence to be sorted
 * @param last one element past the final position in the sequence to be sorted
 */
template <typename Iterator>
void bubble_sort(Iterator first, Iterator last) {
  bool swapped = true;
  last--;
  while (swapped) {
    swapped = false;
    for (auto pos = first; pos != last; pos++) {
      auto a = *pos;
      pos++;
      auto b = *pos;
      if (b < a) {
        *pos = a;
        pos--;
        *pos = b;
        swapped = true;
      } else {
        pos--;
      }
    }
  }
}

/**
 * Performs an index-based selection sort on an indexable object.
 *
 * @tparam IndexedContainer must support `operator[]` and `size()`, e.g. `std::vector`. Container
 * elements must support `operator<` and `operator=`.
 *
 * @param values the object to sort
 */
template <typename IndexedContainer>
void selection_sort(IndexedContainer &values) {
  int len = values.size();
  for (int i = 0; i != len - 1; i++) {
    int lowest = i;
    for (int j = i + 1; j != len; j++) {
      if (values[j] < values[lowest]) {
        lowest = j;
      }
    }
    auto temp = values[i];
    values[i] = values[lowest];
    values[lowest] = temp;
  }
}

/**
 * Sorts the elements in the range `[first,last)` into ascending order, using the selection-sort
 * algorithm.
 *
 * @tparam Iterator a position iterator that supports the [standard input iterator
 * operations](http://www.cplusplus.com/reference/iterator/InputIterator/)
 *
 * @param first the initial position in the sequence to be sorted
 * @param last one element past the final position in the sequence to be sorted
 */
template <typename Iterator>
void selection_sort(Iterator first, Iterator last) {
  last--;
  auto len = last;
  last++;
  for (auto pos = first; pos != len; pos++) {
    auto lowest = pos;
    pos++;
    auto check_pos = pos;
    pos--;
    for (auto check = check_pos; check != last; check++) {
      if (*check < *pos) {
        lowest = check;
        auto temp = *pos;
        *pos = *check;
        *check = temp;
      }
    }
  }
}

}  // namespace cs19

#endif  // CS19_SEARCH_SORT_H_