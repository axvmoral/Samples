""" Module for the CircleActor class. """
__author__ = 'Axel V. Morales Sanchez, asmorales@jeff.cis.cabrillo.edu'

import math


class CircleActor:
  """ Behaves as a circle in a 2D world centered on an X/Y coordinate. """

  def __init__(self, name: str, radius: float, world_size: tuple[float, float],
               position: tuple[float, float], velocity: tuple[float, float]):
    """
    Constructs a new CircleActor with a given name at a position and with an inherent velocity.

    :param name: the name of the actor, assumed to be non-empty
    :param radius: the initial radius of the actor
    :param world_size: the width and height of the world in which the actor resides
    :param position: the x/y coordinate of the center of the actor
    :param velocity: the amount in x/y dimensions by which the actor will move on each step
    """
    self._name = name
    self._radius = radius
    self._world_size = world_size
    self._position = position
    self._velocity = velocity

  def __bool__(self) -> bool:
    """
    Returns True if this actor is still "alive", meaning its radius is
    small enough for the circle to fit within the world and no less than 1.
    """
    return 2 * self._radius <= min(self._world_size[0], self._world_size[1]) and self._radius >= 1.0

  def __contains__(self, other) -> bool:
    """
    Returns True if another actor is "contained within" this one, i.e. whether the two actors
    overlap at all and this actor has a larger radius than the other.
    """
    return self - other < 0 and self._radius > other._radius

  def __repr__(self) -> str:
    """
    Returns a printable representation of this actor, appropriate for eval().
    That is, the return value of this method should be a string that is valid code for
    re-constructing this actor with the same attributes.
    """
    return 'circle_actor.CircleActor' + str(
      (self._name, self._radius, self._world_size, self._position, self._velocity)
    )

  def __str__(self) -> str:
    """
    Returns the name of this actor.
    """
    return self._name

  def __sub__(self, other) -> float:
    """
    Returns the distance between this actor and another,
    i.e. how far the two circles are from touching.
    This value will be negative if the two circles overlap.
    """
    return math.sqrt(
      (self._position[0] - other._position[0]) ** 2 +
      (self._position[1] - other._position[1]) ** 2
    ) - self._radius - other._radius

  def bounce(self):
    border_lat = 0
    border_lng = 0
    new_px = None
    new_py = None
    for sign in [-1, 1]:
      edge = (self._position[0] + sign * self._radius, self._position[1] + sign * self._radius)
      if sign * edge[0] > border_lat:
        new_px = -1 * sign * self._radius + border_lat
      if sign * edge[1] > border_lng:
        new_py = -1 * sign * self._radius + border_lng
      border_lat = self._world_size[0]
      border_lng = self._world_size[1]
    if new_px is not None:
      self._position = (new_px, self._position[1])
      self._velocity = (-1 * self._velocity[0], self._velocity[1])
    if new_py is not None:
      self._position = (self._position[0], new_py)
      self._velocity = (self._velocity[0], -1 * self._velocity[1])

  def collide(self, other) -> bool:
    """
    "Collides" this actor with another. If they overlap, the radius of the larger actor shall
    increase by 1 and that of the smaller will decrease by 1.
    """
    if other in self:
      self._radius += 1
      other._radius -= 1
      self.bounce()
      other.bounce()
    elif self in other:
      self._radius -= 1
      other._radius += 1
      self.bounce()
      other.bounce()
    return self.__bool__()

  def position(self, new_position: tuple[float, float] = None):
    """
    Given no arguments, returns this actor's position.
    Given a tuple[float, float] as an argument, sets this actor's x/y position components.
    """
    if new_position is None:
      return self._position
    elif isinstance(new_position, tuple) and all(map(lambda x: isinstance(x, float), new_position)):
      self._position = new_position

  def radius(self, new_radius: float = None):
    """
    Given no arguments, returns this actor's radius.
    Given a real number as an argument, sets this actor's radius.
    """
    if new_radius is None:
      return self._radius
    else:
      self._radius = new_radius

  def step(self):
    """
    Moves this actor in in the direction of its velocity by one unit of "time",
    i.e. one frame of animation or one discrete event.
    e.g. if position is (4, 5) and velocity is (-1, 1), the new position will be (3, 6).
    """
    self._position = (self._position[0] + self._velocity[0], self._position[1] + self._velocity[1])
    self.bounce()

  def velocity(self, new_velocity: tuple[float, float] = None):
    """
    Given no arguments, returns this actor's velocity.
    Given a tuple[float, float] as an argument, sets this actor's x/y velocity components.
    """
    if new_velocity is None:
      return self._velocity
    elif isinstance(new_velocity, tuple) and all(map(lambda x: isinstance(x, float), new_velocity)):
      self._velocity = new_velocity
