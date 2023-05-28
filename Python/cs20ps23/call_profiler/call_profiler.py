"""
Provides decorator function "profile" that counts calls and cumulative execution time for all
decorated functions for the duration of an interpreter session.
"""

__author__ = 'Axel V. Morales Sanchez, asmorales@jeff.cis.cabrillo.edu'

import functools
import time

logger = {}
timer = {}


def profile(function):
  """
  Decorates a function so that the number of calls and cumulative execution time of all calls can
  be reported by call_count() and cumulative_time(), respectively. Execution time is measured by
  calling time.perf_counter() before and after a call to the decorated function.
  """
  @functools.wraps(function)
  def wrapper(*args, **kwargs):
    st = time.perf_counter()
    ret = function(*args, **kwargs)
    et = time.perf_counter()
    logger[wrapper] = logger.get(wrapper, 0) + 1
    timer[wrapper] = timer.get(wrapper, 0) + (et - st)
    return ret
  logger[wrapper] = 0
  timer[wrapper] = 0.0
  return wrapper


def call_count(function):
  """
  Returns the number of times a given function has been called during this interpreter session,
  assuming the function has been decorated by profile().
  """
  return logger[function]


def call_counts():
  """
  Returns a dictionary mapping functions decorated by profile() to the number of times they have
  been called during this interpreter session.
  """
  return logger


def cumulative_time(function):
  """
  Returns the cumulative amount of time (in seconds) that have been spent executing calls to a given
  function during this interpreter session, assuming the function has been decorated by profile().
  """
  return timer[function]


def cumulative_times():
  """
  Returns a dictionary mapping functions decorated by profile() to the cumulative amount of time (in
  seconds) that have been spent executing calls to a given function during this interpreter session.
  """
  return timer
