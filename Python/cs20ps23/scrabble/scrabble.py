"""
Generator and other functions related to Scrabble® words.
"""

from io import TextIOBase  # for type hints
from typing import Iterable, Iterator  # for type hints
import itertools as it  # suggested for permutations() and chain.from_iterable()
import re  # suggested for finditer()

with open('/srv/datasets/scrabble-letter-values') as fhand1:
  lst = fhand1.read().split()
slv_data = {lst[i]: int(lst[i + 1]) for i in range(0, len(lst), 2)}
with open('/srv/datasets/scrabble-hybrid') as fhand2:
  sh_data = set(fhand2.read().split())


def tokenize_words(file: TextIOBase) -> Iterator[str]:
  """
  Tokenizes the contents of a text-file object (e.g. from open() or sys.stdin) and yields
  all contiguous sequences of English letters from the file, in uppercase.

  >>> from pprint import pprint
  >>> pprint(list(tokenize_words(open('/srv/datasets/party.txt'))), compact=True)
  ['THE', 'PARTY', 'TOLD', 'YOU', 'TO', 'REJECT', 'THE', 'EVIDENCE', 'OF', 'YOUR',
   'EYES', 'AND', 'EARS', 'IT', 'WAS', 'THEIR', 'FINAL', 'MOST', 'ESSENTIAL',
   'COMMAND']
  >>> pprint(list(tokenize_words(open('/srv/datasets/phonewords-e.161.txt'))))
  ['ABC', 'DEF', 'GHI', 'JKL', 'MNO', 'PQRS', 'TUV', 'WXYZ']
  """
  return (match.group() for match in it.chain.from_iterable(
    re.finditer('[A-Z]+', line.upper()) for line in file))


def legal_words(words: Iterable[str]) -> Iterator[str]:
  """
  Selects from an iterable collection of strings only those that are legal Scrabble® words.

  >>> from pprint import pprint
  >>> pprint(list(legal_words(tokenize_words(open('/srv/datasets/party.txt')))), compact=True)
  ['THE', 'PARTY', 'TOLD', 'YOU', 'TO', 'REJECT', 'THE', 'EVIDENCE', 'OF', 'YOUR',
   'EYES', 'AND', 'EARS', 'IT', 'WAS', 'THEIR', 'FINAL', 'MOST', 'ESSENTIAL',
   'COMMAND']
  >>> pprint(list(legal_words(tokenize_words(open('/srv/datasets/phonewords-e.161.txt')))))
  ['DEF', 'GHI']
  >>> pprint(list(legal_words(tokenize_words(open('/srv/datasets/empty')))))
  []
  >>> list(legal_words(['all', 'in', 'lowercase']))
  []
  """
  return (word for word in words if word in sh_data)


def word_score(word: str) -> int:
  """
  Computes the sum of the tile values for a given word, or 0 if the word is illegal.

  >>> from pprint import pprint
  >>> pprint([(w, word_score(w)) for w in tokenize_words(open('/srv/datasets/party.txt'))], \
             compact=True)
  [('THE', 6), ('PARTY', 10), ('TOLD', 5), ('YOU', 6), ('TO', 2), ('REJECT', 15),
   ('THE', 6), ('EVIDENCE', 14), ('OF', 5), ('YOUR', 7), ('EYES', 7), ('AND', 4),
   ('EARS', 4), ('IT', 2), ('WAS', 6), ('THEIR', 8), ('FINAL', 8), ('MOST', 6),
   ('ESSENTIAL', 9), ('COMMAND', 14)]
  >>> pprint([(w, word_score(w)) \
                  for w in tokenize_words(open('/srv/datasets/phonewords-e.161.txt'))], \
             compact=True)
  [('ABC', 0), ('DEF', 7), ('GHI', 7), ('JKL', 0), ('MNO', 0), ('PQRS', 0),
   ('TUV', 0), ('WXYZ', 0)]
  >>> word_score('lowercase')
  0
  """
  return sum(slv_data[letter] for letter in word if word in sh_data)


def highest_value_word(words: Iterable[str]) -> str:
  """
  Selects from an iterable collection of strings the highest-valued Scrabble® word,
  as returned by the word_score() function.
  If multiple words are maximal, the function returns the first one encountered.
  If no words are present, raises an exception of some kind.

  >>> highest_value_word(tokenize_words(open('/srv/datasets/party.txt')))
  'REJECT'
  >>> highest_value_word(tokenize_words(open('/srv/datasets/phonewords-e.161.txt')))
  'DEF'
  >>> try:
  ...   highest_value_word(tokenize_words(open('/srv/datasets/empty')))
  ... except Exception as error:
  ...   print('error')
  ...
  error
  """
  word_and_counts = [(word, word_score(word)) for word in words]
  if not word_and_counts:
    raise Exception('No words present')
  word_and_counts.sort(key=lambda x: x[1], reverse=True)
  return word_and_counts[0][0]


def legal_tile_words(tiles: str) -> list[str]:
  """
  Returns a sorted list of all the legal Scrabble® words that could be formed from the "tiles"
  represented by the argument string.

  >>> legal_tile_words('JTQHDEZ')
  ['DE', 'ED', 'EDH', 'EH', 'ET', 'ETH', 'HE', 'HET', 'JET', 'TE', 'TED', 'THE', 'ZED']
  """
  words = list(
    set(
      map(''.join, it.chain.from_iterable(
        it.permutations(tiles, i) for i in range(1, len(tiles) + 1)))
        ).intersection(sh_data))
  words.sort()
  return words


if __name__ == '__main__':
  import sys
  (i1, i2) = it.tee(legal_words(tokenize_words(sys.stdin)))
  print(sum(1 for _ in i1), 'legal Scrabble words worth', sum(map(word_score, i2)), 'points')
