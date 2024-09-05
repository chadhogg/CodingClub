# My (partial) solution to https://open.kattis.com/problems/communicationssatellite from August 30, 2024
# I never quite figured out how to detect whether or not a line would pass through
#   another line or through a dish.  I wanted to do this without using anything that
#   would not be understandable to anyone who completed high school geometry, but
#   ran out of time.
# If I had succeeded at that part, setting up and solving the minimum spanning tree
#   problem seemed like it would be pretty straightforward.

from __future__ import annotations
from dataclasses import dataclass

@dataclass
class Point:
    """A point on the Cartesian plane."""
    x: float
    y: float

    def distance_to(self: Point, other: Point) -> float:
        """Calculates the distance between two 2D points."""
        return ((self.x - other.x) ** 2 + (self.y - other.y) ** 2) ** 0.5
    
@dataclass
class LineSegment:
    """A line segment."""
    start: Point
    end: Point

    def length(self: LineSegment) -> float:
        return self.start.distance_to(self.end)
    
    def intersects(self: LineSegment, other: LineSegment) -> bool:
        pass

@dataclass
class Dish:
    """A communications dish that should be included on the satellite."""
    index: int
    center: Point
    radius: float

    def connector(self: Dish, other: Dish) -> LineSegment:
        """Find the shortest line segment that connects this Dish to another.
        I based this idea on thinking of the places where the segment between
           the centers intersects the boundaries of the circles as weighted
           averages of the two centers.
        I *think* it works, based on extensive drawings and experiments."""
        centersDist: float = self.center.distance_to(other.center)
        percentMine: float = self.radius / centersDist
        percentHis: float = other.radius / centersDist
        start: Point = Point((1 - percentMine) * self.center.x + percentMine * other.center.x,
                             (1 - percentMine) * self.center.y + percentMine * other.center.y)
        end: Point = Point((1 - percentHis) * other.center.x + percentHis * self.center.x,
                           (1 - percentHis) * other.center.y + percentHis * self.center.y)
        return LineSegment(start, end)


@dataclass
class Edge:
    startIndex: int
    endIndex: int
    cost: float


if __name__ == '__main__':
    numLines: int = int(input())
    dishes = []
    for index in range(numLines):
        line = input()
        dishes.append(Dish(index, Point(float(line.split()[0]), float(line.split()[1])), float(line.split()[2])))
    print(dishes)
    