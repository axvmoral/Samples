"""
Practice implementing a hash table!
"""

from typing import Iterable, Hashable


class HashSet:
  """
  Implementation of a hash table similar to built-in type set, using an internal list as a table.
  """

  def __init__(self, iterable: Iterable[Hashable] = None):
    """
    Constructs an empty set with a table size of 8, or a set containing the values in `iterable`.

    >>> h = HashSet()
    >>> len(h)
    0
    >>> h = HashSet((1, 9))
    >>> len(h)
    2
    """
    self.set = [None] * 8
    self.hashes = [None] * 8
    self._table_size = 8
    self.size = 0
    self.repr = "hash_set.HashSet()"
    if iterable:
      self.repr = f"hash_set.HashSet({iterable})"
      for item in iterable:
        self.add(item)

  def __bool__(self):
    """
    Returns ''True'' if the set is non-empty, and ''False'' otherwise.

    >>> h = HashSet((1, 9))
    >>> bool(h)
    True
    >>> h.remove(1)
    >>> bool(h)
    True
    >>> h.remove(9)
    >>> bool(h)
    False
    """
    return bool(self.size)

  def __contains__(self, key):
    r"""
    Tests for membership in the set.

    >>> import re
    >>> h = HashSet(re.findall(r"[\w']+", open('/srv/datasets/cat-in-the-hat.txt').read()))
    >>> 'play' in h
    True
    >>> 'plays' in h
    False
    >>> 'NOW' in h
    True
    >>> 100 in h
    False
    >>> None in h
    False
    """
    hsh = hash(key)
    index = hsh % self._table_size
    return self.hashes[index] and (self.set[index] == key or key in self.hashes[index][1:])

  def __getitem__(self, index):
    """
    Returns the key at the given index of the internal table.

    >>> h = HashSet(range(1, 15, 3))
    >>> [h[i] for i in range(h.table_size())]
    [None, 1, 10, None, 4, 13, None, 7]
    """
    return self.set[index]

  def __iter__(self):
    """
    Yields keys from the internal table in the order encountered.

    >>> h = HashSet([2, 4, 6, 0, 1])
    >>> list(h)
    [0, 1, 2, 4, 6]
    """
    for item in self.set:
      if item is not None:
        yield item

  def __repr__(self):
    """
    Returns a string representation of this set, suitable for eval().

    >>> h = HashSet([2, 4, 6, 0, 1])
    >>> eval(repr(h))
    HashSet([0, 1, 2, 4, 6])
    """
    return self.repr

  def __len__(self):
    """
    Returns the number of keys in this set.

    >>> h = HashSet(map(str.rstrip, open('/srv/datasets/many-english-words.txt')))
    >>> len(h)
    704240
    """
    return self.size

  def add(self, key):
    """
    Adds a key to this set, if not already present.

    >>> h = HashSet()
    >>> for new_key in range(1, 21, 3):
    ...   h.add(new_key)
    ...   [h[i] for i in range(h.table_size())]
    ...
    [None, 1, None, None, None, None, None, None]
    [None, 1, None, None, 4, None, None, None]
    [None, 1, None, None, 4, None, None, 7]
    [None, 1, 10, None, 4, None, None, 7]
    [None, 1, 10, None, 4, 13, None, 7]
    [None, 1, None, None, 4, None, None, 7, None, None, 10, None, None, 13, None, None, 16, None]
    [None, 1, 19, None, 4, None, None, 7, None, None, 10, None, None, 13, None, None, 16, None]
    """
    if not self.__contains__(key):
      hsh = hash(key)
      index = hsh % self._table_size
      if (self.size + 1) / self._table_size <= 2 / 3:
        self.add_probe(index, hsh, key)
        self.size += 1
        return None
      else:
        self.add_probe(index, hsh, key)
        temp = self.hashes
        self._table_size = (self.size + 1) * 3
        self.set = [None] * self._table_size
        self.hashes = [None] * self._table_size
        for item in temp:
          if item is not None:
            self.add_probe(item[0] % self._table_size, item[0], item[1])
        self.size += 1
        return None
    else:
      return None

  def add_probe(self, index, hsh, key):
    if self.set[index] is None and not self.hashes[index]:
      self.set[index] = key
      self.hashes[index] = [hsh, key]
      return None
    else:
      self.hashes[index].append(key)
      i = 0
      while True:
        if index + i < self._table_size and self[index + i] is None and not self.hashes[index + i]:
          self.set[index + i] = key
          self.hashes[index + i] = [hsh, key]
          return None
        elif index - i > -1 and self[index - i] is None and not self.hashes[index - i]:
          self.set[index - i] = key
          self.hashes[index - i] = [hsh, key]
          return None
        else:
          i += 1

  def clear(self):
    """
    Removes all keys from this set, returning the table to its initial state (size 8).

    >>> h = HashSet(map(str.rstrip, open('/srv/datasets/many-english-words.txt')))
    >>> len(h)
    704240
    >>> h.table_size()
    1376253
    >>> h.clear()
    >>> len(h)
    0
    >>> h.table_size()
    8
    """
    self.set = [None] * 8
    self.hashes = [None] * 8
    self._table_size = 8
    self.size = 0

  def remove(self, key):
    """
    Removes a key from the set, if present. Does not raise an error if the key is not present.

    >>> h = HashSet((1, 9))
    >>> [h[i] for i in range(h.table_size())]
    [None, 1, 9, None, None, None, None, None]
    >>> 1 in h
    True
    >>> 9 in h
    True
    >>> 'one' in h
    False
    >>> h.remove(1)
    >>> len(h)
    1
    >>> 1 in h
    False
    >>> 9 in h
    True
    >>> [h[i] for i in range(h.table_size())]
    [None, None, 9, None, None, None, None, None]
    >>> h.remove(9)
    >>> len(h)
    0
    >>> [h[i] for i in range(h.table_size())]
    [None, None, None, None, None, None, None, None]
    """
    if self.__contains__(key):
      hsh = hash(key)
      index = hsh % self._table_size
      if self.set[index] == key:
        self.set[index] = None
        to_pop = self.hashes[index].index(key, 1)
        self.hashes[index].pop(to_pop)
        if len(self.hashes[index]) < 2:
          self.hashes[index] = None
        self.size -= 1
        return None
      else:
        i = 1
        while True:
          if index + i < self._table_size and self.set[index + i] == key:
            self.set[index + i] = None
            to_pop = self.hashes[index + i].index(key, 1)
            self.hashes[index + i].pop(to_pop)
            if len(self.hashes[index + i]) < 2:
              self.hashes[index + i] = None
            to_pop = self.hashes[index].index(key, 1)
            self.hashes[index].pop(to_pop)
            if len(self.hashes[index]) < 2:
              self.hashes[index] = None
            self.size -= 1
            return None
          elif index - i > -1 and self.set[index - i] == key:
            self.set[index - i] = None
            to_pop = self.hashes[index - i].index(key, 1)
            self.hashes[index - i].pop(to_pop)
            if len(self.hashes[index - i]) < 2:
              self.hashes[index - i] = None
            to_pop = self.hashes[index].index(key, 1)
            self.hashes[index].pop(to_pop)
            if len(self.hashes[index]) < 2:
              self.hashes[index] = None
            self.size -= 1
            return None
          else:
            i += 1
    else:
      return None

  def table_size(self):
    """
    Returns the size of the internal table.

    >>> h = HashSet()
    >>> for v in range(10):
    ...   h.add(v)
    ...   h.table_size()
    ...
    8
    8
    8
    8
    8
    18
    18
    18
    18
    18
    """
    return self._table_size
