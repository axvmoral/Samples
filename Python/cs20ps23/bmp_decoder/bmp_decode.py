""" BMP image decoding. """

__author__ = "Axel V. Morales Sanchez, asmorales@cabrillo.edu"


class BmpImage:
  """A BMP Image Decoder!"""

  def __init__(self, path: str):
    """
    Constructs a BmpImage from a file path.

    >>> demo = BmpImage('/srv/datasets/cabrillo-logo.bmp')
    >>> demo.dimensions()
    (300, 151)
    """
    with open(path, mode="rb") as file:
      offset = int.from_bytes(file.read(14)[10:14], "little")
      DIB = file.read(offset - 14)
      self.width = int.from_bytes(DIB[4:8], "little")
      self.height = int.from_bytes(DIB[8:12], "little")
      values_count = int.from_bytes(DIB[14:16], "little") // 8
      self.pixels = {}
      row_byte_count = self.width * values_count
      padding = 0
      while row_byte_count % 4 != 0:
        row_byte_count += 1
        padding += 1
      for i in range(self.height - 1, -1, -1):
        row = {}
        for j in range(self.width):
          pixel = []
          for k in range(values_count):
            byte = file.read(1)
            if k < 3:
              pixel.insert(0, int.from_bytes(byte, "little"))
            else:
              pixel.append(int.from_bytes(byte, "little"))
          row[j] = tuple(pixel)
        self.pixels[i] = row
        file.read(padding)

  def __getitem__(self,
                  coord: int | tuple[int, int]) -> tuple[int, int, int] | tuple[int, int, int, int]:
    """
    Returns a 3-tuple or 4-tuple of integers in [0, 255] representing the RGB or RGBA values,
    respectively, of a pixel in this image. The pixel coordinate is either specified as a
    tuple[int, int] specifying the x and y pixel coordinates, or as a single integer specifying
    an index into the array of pixels, where index 0 is the upper-left corner of the image, and
    index len() - 1 is the lower-right corner.

    >>> demo = BmpImage('/srv/datasets/cabrillo-logo.bmp')
    >>> demo[0]
    (255, 255, 255)
    >>> demo[(224, 24)]
    (120, 188, 233)
    >>> demo[(82, 121)]
    (57, 97, 226)
    >>> demo[len(demo) - 1]
    (255, 255, 255)
    >>> demo = BmpImage('/srv/datasets/pelota.bmp')
    >>> demo[(9, 4)]
    (0, 0, 0, 0)
    >>> demo[(10, 4)]
    (15, 16, 17, 82)
    >>> demo[(11, 4)]
    (33, 34, 35, 252)
    """
    if isinstance(coord, int):
      row = coord // self.width
      column = coord - row * self.width
      return self.pixels[row][column]
    elif isinstance(coord, tuple):
      return self.pixels[coord[1]][coord[0]]

  def __len__(self):
    """
    Returns the total number of pixels in the image.

    >>> len(BmpImage('/srv/datasets/cabrillo-logo.bmp'))
    45300
    """
    return self.width * self.height

  def __iter__(self):
    """
    Yields all pixels in the same format as __getitem__(), from the upper-left corner (0, 0)
    to the lower-right corner (width - 1, height - 1).

    >>> list(BmpImage('/srv/datasets/simple.bmp'))
    [(0, 0, 255), (0, 255, 0), (255, 0, 0), (255, 255, 255)]
    >>> len(list(BmpImage('/srv/datasets/cabrillo-logo.bmp')))
    45300
    >>> len(set(BmpImage('/srv/datasets/cabrillo-logo.bmp')))
    520
    """
    for i in range(self.height):
      for j in range(self.width):
        yield self.pixels[i][j]

  def dimensions(self):
    """
    Returns a tuple consisting of the width and height of this image, in pixels.

    >>> BmpImage('/srv/datasets/simple.bmp').dimensions()
    (2, 2)
    >>> BmpImage('/srv/datasets/cabrillo-logo.bmp').dimensions()
    (300, 151)
    """
    return (self.width, self.height)
