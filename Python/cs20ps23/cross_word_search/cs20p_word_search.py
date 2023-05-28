"""
Searches for legal English words in grid made up of
English letters inputed line by line on standard input
"""
import sys
import numpy as np


def n_gram_words(str_list, min_length, max_length):
    length = min_length
    to_return = set()
    while length != max_length + 1:
        limit = len(str_list) - length
        to_return = to_return.union(
            {str_list[i: i + length] for i in range(len(str_list)) if i <= limit}
        )
        length += 1
    return to_return


def get_words(orientiation, min_length, max_length):
    return n_gram_words(
        orientiation, min_length, max_length
    ).union(
        n_gram_words(
            orientiation[::-1], min_length, max_length
        )
    )


if __name__ == "__main__":
    target_len = int(sys.argv[1])
    with open(sys.argv[2]) as file:
        raw = set(file.read().upper().split())
    words = set(
        (word.strip() for word in raw)
    )
    grid = np.array([list(row.strip()) for row in sys.stdin])
    grid_len = len(grid)
    grid_range = range(grid_len)
    for row in grid:
        print(row)
    grid_flip = np.flip(grid, 1)

    combs = set()
    for i in range(len(grid)):
        combs = combs.union(
            get_words("".join(grid[i]), target_len, len(grid)),
            get_words("".join(grid[:, i]), target_len, len(grid)),
            get_words("".join(grid.diagonal(offset=i)), target_len, len(grid)),
            get_words("".join(grid.diagonal(offset=-i)), target_len, len(grid)),
            get_words("".join(grid_flip.diagonal(offset=i)), target_len, len(grid)),
            get_words("".join(grid_flip.diagonal(offset=-i)), target_len, len(grid))
        )
    final_words = list(words.intersection(combs))
    final_words.sort()
    for word in final_words:
        print(word)
