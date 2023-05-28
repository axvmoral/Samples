""" Module for the CircleActor class. """
__author__ = 'Axel V. Morales Sanchez, asmorales@jeff.cis.cabrillo.edu'

import math
import collections.abc


class CircularRange(collections.abc.Sequence):
    """ Behaves as an infinite looping range. """

    def __init__(self, start, stop, step=1):
        """Constructs a CircularRange with the given start/stop/step values."""
        self.start = start
        self.stop = stop
        self.step = step

    def __contains__(self, element):
        return element in self.__iter__()

    def __iter__(self):
        return (self.start + self.step * i for i in range(self.__len__()))

    def __repr__(self):
        return 'circular_range.CircularRange' + str(
            (self.start, self.stop, self.step)
        )

    def __len__(self):
        mx = max(self.start, self.stop)
        mn = min(self.start, self.stop)
        return math.ceil((mx - mn) / abs(self.step))

    def __getitem__(self, index):
        if isinstance(index, slice):
            start = index.start
            stop = index.stop
            step = index.step
            if step is None:
                step = 1
            if start is not None:
                start = self.start + start % self.__len__() * self.step
            else:
                start = self.start
            if stop is not None:
                stop = self.start + stop % self.__len__() * self.step
            else:
                stop = self.stop
            if step < 0:
                return CircularRange(
                    max(start, stop), min(start, stop), step
                )
            else:
                return CircularRange(
                    min(start, stop), max(start, stop), step
                )
        elif isinstance(index, int):
            return self.start + index % self.__len__() * self.step
