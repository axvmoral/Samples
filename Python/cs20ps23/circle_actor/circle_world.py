#!/usr/bin/env python3
""" Quick and dirty Circle World GUI using tkinter """
__author__ = 'Axel V. Morales Sanchez for CS 20P, asmorales@cabrillo.edu'
 
import circle_actor
import random
import tkinter
import time
 
WINDOW_TITLE = 'CS 20P Circle World'
WIDTH = 1280
HEIGHT = 720
FRAME_RATE = 60
STUDENTS = ('adcuthbertson', 'agbrunetto', 'ajgrushkin', 'altorresmoran', 'asmorales', 'atspencer',
            'csantiago', 'djrobertson', 'ecweiler', 'fjmorales', 'gehult', 'ikhorvath', 'jebentley',
            'jnchristofferso', 'kkonrad', 'lnkleijnen', 'mkowalski', 'mmozer', 'nekoehler',
            'nepedrotti', 'rrojasresendiz', 'wsschaffer', 'ymoralesgalvan', 'ysmohamed')
DEFAULT_RADIUS = min(WIDTH, HEIGHT) // len(STUDENTS) // 2
MAX_DEFAULT_VELOCITY_COMPONENT = 2
 
 
def create_actors(canvas):
  actors = []
  for name in STUDENTS:
    radius = random.randint(DEFAULT_RADIUS // 2, DEFAULT_RADIUS * 2)
    actor = circle_actor.CircleActor(
        name,
        radius=radius,
        world_size=(WIDTH, HEIGHT),
        position=(random.randint(DEFAULT_RADIUS, WIDTH - DEFAULT_RADIUS),
                  random.randint(DEFAULT_RADIUS, HEIGHT - DEFAULT_RADIUS)),
        velocity=(random.random() * MAX_DEFAULT_VELOCITY_COMPONENT * 2 -
                  MAX_DEFAULT_VELOCITY_COMPONENT,
                  random.random() * MAX_DEFAULT_VELOCITY_COMPONENT * 2 -
                  MAX_DEFAULT_VELOCITY_COMPONENT))
    actors.append(actor)
    posx, posy = actor.position()
    color_int = int.from_bytes(bytes(0xff - random.randint(0, 127) for _ in range(3)),
                               byteorder='big')
    actor.oval = canvas.create_oval(posx - radius,
                                    posy - radius,
                                    posx + radius - 1,
                                    posy + radius - 1,
                                    fill=f'#{color_int:06x}')
    actor.label = canvas.create_text(posx, posy, text=str(actor), fill='black', font='Helvetica 12')
  return actors
 
 
def animate(window, canvas, actors):
  while True:
    # Revive the dead
    for actor in actors:
      if not actor:
        actor.radius(random.randint(DEFAULT_RADIUS // 2, DEFAULT_RADIUS * 2))
        actor.position((random.randint(DEFAULT_RADIUS, WIDTH - DEFAULT_RADIUS),
                        random.randint(DEFAULT_RADIUS, HEIGHT - DEFAULT_RADIUS)))
        actor.velocity(
            (random.random() * MAX_DEFAULT_VELOCITY_COMPONENT * 2 - MAX_DEFAULT_VELOCITY_COMPONENT,
             random.random() * MAX_DEFAULT_VELOCITY_COMPONENT * 2 - MAX_DEFAULT_VELOCITY_COMPONENT))
    # All actors move one step
    for actor in actors:
      actor.step()
    # They may collide with each other and respond
    for actor in actors:
      for other in actors:
        if actor is not other:
          actor.collide(other)
    # Sort the stacking order by size
    for actor in sorted(actors, key=circle_actor.CircleActor.radius, reverse=True):
      canvas.lift(actor.oval)
      canvas.lift(actor.label)
    # Display them
    for actor in actors:
      posx, posy = actor.position()
      radius = actor.radius()
      canvas.coords(actor.oval, round(posx - radius), round(posy - radius),
                    round(posx + radius + 1), round(posy + radius + 1))
      canvas.coords(actor.label, round(posx), round(posy))
    window.update()
    time.sleep(1 / FRAME_RATE)
 
 
if __name__ == '__main__':
  _window = tkinter.Tk()
  _window.title(WINDOW_TITLE)
  _window.geometry(f'{WIDTH}x{HEIGHT}')
  _canvas = tkinter.Canvas(_window)
  _canvas.configure(bg='white')
  _canvas.pack(fill='both', expand=True)
  _actors = create_actors(_canvas)
  animate(_window, _canvas, _actors)