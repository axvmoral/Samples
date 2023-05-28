'''Module for the Graph class'''
__author__ = 'Axel V. Morales Sanchez, asmorales@jeff.cis.cabrillo.edu'

from typing import Sequence
import copy


class Graph(dict):
    '''Provides a Graph data type with a Pythonic API suitable
    for representing graphs that are any combinations of
    directed/undirected and weighted/unweighted'''

    def __missing__(self, vertex):
        self[vertex] = _Edges(graph=self)
        return self[vertex]

    def __delitem__(self, vertex) -> None:
        super().__delitem__(vertex)
        for key in self:
            if self.adjacent(key, vertex):
                del self[key][vertex]

    def copy(self):
        return copy.deepcopy(self)

    def vertices(self):
        return set(super().keys())

    def edges(self):
        to_return = set()
        for vertex in self:
            _edges = self[vertex]
            for edge, _weight in zip(_edges.keys(), _edges.values()):
                to_return.add((vertex, edge, _weight))
        return to_return

    def adjacent(self, src, dst):
        return dst in self[src]

    def neighbors(self, vertex):
        return set(self[vertex])

    def degree(self, vertex):
        return len(self[vertex])

    def path_valid(self, vertices: Sequence):
        lim = len(vertices)
        if lim == 1:
            return True
        i = 0
        while i < lim - 1:
            src = vertices[i]
            dst = vertices[i + 1]
            if not self.adjacent(src, dst):
                return False
            i += 1
        return True

    def path_length(self, vertices: Sequence):
        lim = len(vertices)
        if lim == 1:
            return None
        i = 0
        length = None
        while i < lim - 1:
            src = vertices[i]
            dst = vertices[i + 1]
            if not self.adjacent(src, dst):
                return None
            elif i == 0:
                length = self[src][dst]
            else:
                length += self[src][dst]
            i += 1
        return length

    def are_connected(self, src, dst):
        neighbors = self.neighbors(src)
        new_neighbors = neighbors
        while True:
            if dst in neighbors:
                return True
            else:
                running_len = len(neighbors)
                prior_neighbors = neighbors
                for vertex in new_neighbors:
                    neighbors = neighbors.union(self.neighbors(vertex))
                if len(neighbors) == running_len:
                    return False
                else:
                    new_neighbors = neighbors - prior_neighbors

    def is_connected(self):
        vertices = list(self.vertices())
        for i in range(len(vertices) - 1):
            source = vertices[i]
            dest = vertices[i + 1]
            if not self.are_connected(source, dest) or not self.are_connected(dest, source):
                return False
        return True


class _Edges(dict):
    '''Represents edges in Graph data type'''

    def __init__(self, graph=None):
        dict.__init__(self)
        self.graph = graph

    def __setitem__(self, dst, _weight) -> None:
        self.graph[dst]
        return super().__setitem__(dst, _weight)

    def copy(self, graph=None):
        return type(self)(graph)


if __name__ == '__main__':
    g = Graph()
    assert len(g) == 0
    assert 'wat' not in g
    assert not g.vertices()
    edges = ('a', 'c', 8), ('a', 'd', 4), ('c', 'b', 6), ('d', 'b', 10), ('d', 'c', 2)
    for (v_from, v_to, weight) in edges:
        g[v_from][v_to] = weight
    assert len(g) == 4
    assert 'a' in g
    assert 'c' in g['a']
    assert g.vertices() == set('abcd')
    assert g.edges() == set(edges)
    assert g.degree('d') == 2 and not g.degree('b')
    assert g.adjacent('a', 'c')
    assert not g.adjacent('c', 'a')
    assert g.path_valid(('a', 'c', 'b'))
    assert not g.path_valid(('c', 'b', 'a'))
    assert not g.is_connected()
    g['b']['a'] = 1
    assert g.degree('b') == 1 and g.degree('a') == 2
    assert g.path_valid(('c', 'b', 'a'))
    assert g.path_length(('c', 'b', 'a')) == 7
    assert g.is_connected()
    del g['a']
    assert not g.is_connected()
    assert g.vertices() == set('bcd')
    assert g.degree('b') == 0
    g2 = g.copy()
    assert g2 == g
    g2['b']['e'] = 1
    assert g2 != g
    assert g2.vertices() == set('bcde')
    g2['e']['d'] = 15
    assert g2.is_connected()
    assert g2.path_length(('e', 'd', 'c', 'b')) == 23
    del g2['e']['d']
    assert g2.degree('e') == 0
    assert g2.vertices() == set('bcde')
    assert not g2.is_connected()
    g.clear()
    assert len(g) == 0
    assert len(g2) == 4
