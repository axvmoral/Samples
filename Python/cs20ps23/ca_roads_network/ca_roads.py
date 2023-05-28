'''Module for CaliforniaRoadNetwork class'''
__author__ = 'Axel V. Morales Sanchez, asmorales@jeff.cis.cabrillo.edu'

import graph
import contextlib
import heapq
import pprint
import re

_graph = graph.Graph()
coords = {}
addresses = {}
filenames = ['/srv/datasets/ca_roads/cal.cedge',
             '/srv/datasets/ca_roads/cal.cnode',
             '/srv/datasets/ca_roads/cal.cnode.names']
with contextlib.ExitStack() as stack:
    files = [stack.enter_context(open(f, 'r')) for f in filenames]
    for rows in zip(*files):
        info = list(map(lambda x: x.rstrip().split(), rows[:2]))
        addy = rows[2][rows[2].find(' ') + 1:].rstrip().strip('"')
        src, dst, edge = int(info[0][1]), int(info[0][2]), float(info[0][3])
        _graph[src][dst] = edge
        _graph[dst][src] = edge
        node, long, lat = int(info[1][0]), float(info[1][1]), float(info[1][2])
        coords[node] = (lat, long)
        addresses[node] = addy


class CaliforniaRoadNetwork():
    '''Creats a datatype that can calculate the shortest path between two nodes
    on the California Road Network'''
    def __init__(self) -> None:
        self.graph = _graph
        self.coords = coords
        self.addresses = addresses

    def shortest_path(self, _src: int, _dst: int) -> list[list]:
        '''
        Returns a list of lists providing information for the shortest path
        between two nodes of a weighted graph of the California Road Network.

        :param src: the source node
        :param dst: the destination node
        :return a list containing a list of node IDs in the shortest path between
                the source and destination; a list of the equivalent addresses that
                make up the shortest path; a list of 2-tuples consisting of the latitudes
                and longitudes of each node along the path; and a list of the unique city
                names along the path in the order encountered
        '''
        dj = self.dijkstra(_src)
        ret = [[], [], [], []]
        cities = set()
        current = _dst
        while current:
            ret[0].insert(0, current)
            _addy = self.addresses[current]
            city = self.city_finder(_addy)
            if city and city not in cities:
                cities.add(city)
                ret[3].insert(0, city)
            elif city:
                ret[3].remove(city)
                ret[3].insert(0, city)
            ret[1].insert(0, _addy)
            ret[2].insert(0, self.coords[current])
            current = dj[current][1]
        return ret

    def dijkstra(self, source: int) -> list[tuple[float, int]]:
        '''
        Provides Dijkstra's Algorithm for finding the shortest path between all nodes
        of a weighted graph from a source node.

        :param src: the source node
        :return a dictionary mapping every node to a tuple containing the previous node
                with the shortest distance to that node and the distance
        '''
        ret = [(float('inf'), None)] * len(self.coords)
        ret[source] = (0, None)
        pr_q = []
        heapq.heappush(pr_q, (0, source))
        while pr_q:
            length, _node = heapq.heappop(pr_q)
            for neighbor in self.graph.neighbors(_node):
                new_distance = length + self.graph[_node][neighbor]
                if new_distance < ret[neighbor][0]:
                    ret[neighbor] = (new_distance, _node)
                    heapq.heappush(pr_q, (new_distance, neighbor))
        return ret

    def city_finder(self, address: str) -> str:
        temp = re.findall(r'.*[+,].*?\s(.*?), CA', address)
        if temp:
            return temp[0]
        elif not temp and address.find('+') != -1:
            return ''
        else:
            address = re.findall('[A-Z].*', address)[0]
            cal_pos = address.find('California')
            comma_pos = address.rfind(',')
            cal_len = len('California')
            if cal_pos + cal_len == comma_pos:
                return address
            else:
                return address[:cal_pos + cal_len] + ', USA'


if __name__ == '__main__':
    a = CaliforniaRoadNetwork()
    sp = a.shortest_path(10620, 10636)
    pprint.pprint(sp)
