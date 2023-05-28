"""
Various functions for experimenting with the call_profiler module.
"""
 
__author__ = 'Axel V. Morales Sanchez for CS 20P, asmorales@cabrillo.edu'
 
import call_profiler
 
 
@call_profiler.profile
def dna(sequence) -> dict[str, int]:
  """
  Counts the bases in a DNA string.
 
  :param sequence: A DNA string (expected to contain A/C/G/T characters).
  :return: A dict[str, int] with keys in bases 'ACGT', and associated base counts.
  """
  return {base: sequence.count(base) for base in 'ACGT'}
 
 
@call_profiler.profile
def euler_004():
  """ Single statement with walrus. """
  return max(a * b
             for a in range(100, 1000)
             for b in range(100, 1000)
             if (prod_str := str(a * b)) == prod_str[::-1])
 
 
@call_profiler.profile
def euler_009():
  """ Generator expression passed to next()—we know there can be only one solution. """
  return next(a * b * c
              for a in range(1, 334)
              for b in range(a + 1, 1000 - a)
              if a**2 + b**2 == (c := 1000 - a - b)**2)