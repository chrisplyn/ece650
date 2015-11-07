#! /usr/bin/python

from __future__ import division  # So we are not using integer division
import sys 
import fileinput 

streets = {}
vertices = {}


def addVertex(vertices, vertex):
    if vertex not in vertices:
        vertices[vertex] = len(vertices) + 1


class street(object):
    def __init__(self, coords):
        self.coords = coords
        for point in coords:
            add_point(points, point)


def error(message, line):
    sys.stderr.write("Error : %s:%s\n" % (message, line))


def parseInputLine(line):
    tmp = line.split('"')
    coords = []

    if len(tmp) != 3:
        return None
    if not tmp[2]:
        return tmp[1]

    for pair in tmp[2].split(')'):          
        if pair:
            try:
                x, y = pair.split(',')
                x = x.strip(" (")
                point = int(x),int(y)
                coords.append(point)
            except:
                error("Invalid coordinate pair", pair + ')')
                return None
    return tmp[1].strip(),coords


def addStreet(inputLine):
    result = parseInputLine(inputLine)

    if result is not None:
        if len(result[1]) == 1:
            return "Invalid street!"
        if result[0] in streets:
            return result[0] + " has been added!"
        streets[result[0]] = street(result[1])
    else:
        return "Invalid input"


def changeStreet(inputLine):
    result = parseInputLine(inputLine)

    if result is not None:
        if result[0] not in streets:
            return result + " does not exist!"
        streets[result[0]] = street(result[1])
    else:
        return "Invalid input"


def removeStreet(inputLine):
    result = parseInputLine(inputLine)

    if result is not None:        
        if result not in streets:
            return result + "does not exist!"
        del streets[result]
    else:
        return "Invalid input"


def graph(line):

    segments = set()
    active = set()
    for name in streets:  # Go through each street
        street = streets[name]  # Get the street
        for segment in zip(street.coords, street.coords[1:]):  # make segments
            # if segment[0] > segment[1]:  # Make sure the segments are smallest x, largest x order
            #    segment = segment[1], segment[0]  # Swap the order of the segment
            segments.add(segment)  # Add it to the list
            active.add(segment[0])
            active.add(segment[1])
        intersections = set(segments)
        for first in segments:  # Get each pair
            for second in segments:
                ends = set([first[0], first[1], second[0], second[1]])
                if len(ends) == 4:  # There are 4 points, ensure that all are different
                    point = intersection(first, second)  # If there is an intersection create a new point
                    if point:  # If there is an intersection create a new point
                        intersections.add((first[0], point))
                        intersections.add((first[1], point))
                        intersections.add((second[0], point))
                        intersections.add((second[1], point))
                        add_point(points, point)
                        active.add(point)
    print_points = [None] * len(points)
    for key in active:
        print_points[points[key] - 1] = key
    print "V = {"
    for i, point in enumerate(print_points):
        if point:
            print '%d:%s' % (i + 1, point)
    print "}"
    print "E = {"
    for segment in intersections:
        print "<%d, %d>" % (points[segment[0]], points[segment[1]])
    print "}"


def unknown(line):
    return "Unknown command"


cmds = {'a': add, 'c': change, 'g': graph, 'r': remove}  # the different commands


def main():
    for line in fileinput.input():  # read from the file or stdin
        line = line.strip()  # remove the /n at the end of the line
        if line:  # ignore blank lines
            command = cmds.get(line[0], unknown)  # The first character is the command
            message = command(line)  # run the command
            if message:  # if error message, display it
                error(message, line)  # display the error


if __name__ == "__main__":
    main()  # ignore the filename of the python file