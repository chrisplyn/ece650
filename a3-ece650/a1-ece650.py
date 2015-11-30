#!/usr/bin/python
from __future__ import division
import sys,fileinput,re
from signal import signal, SIGPIPE, SIG_DFL
signal(SIGPIPE,SIG_DFL)
#####################################################################
#### Data structures and helper functions
#####################################################################

class endpoint:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __eq__(self, other):
        return (self.x == other.x and self.y == other.y)
    def __hash__(self):
        return hash(self.x)^hash(self.y)

class lineSegment:
    def __init__(self, endpoint1, endpoint2):
        self.endpoint1 = endpoint1
        self.endpoint2 = endpoint2
        self.vertices = []
    def makeVertices(self,intersection):
        if self.endpoint1 not in self.vertices:
            self.vertices.append(self.endpoint1)
        
        if self.endpoint2 not in self.vertices:
            self.vertices.append(self.endpoint2)
        
        #intersection is not one of the end points
        if (self.endpoint1 != intersection or self.endpoint2 != intersection) and (intersection not in self.vertices): 
            self.vertices.append(intersection)
            self.vertices.sort(key=lambda x: (x.x,x.y)) #sort vertex list

class street:
    def __init__(self, coords):
        self.ls = []
        for tmp in zip(coords, coords[1:]):#create list of line segments for a street
            self.ls.append(lineSegment(endpoint(tmp[0][0],tmp[0][1]),endpoint(tmp[1][0],tmp[1][1])))

def lineSegEquation(endpoint1, endpoint2):
    A = endpoint1.y - endpoint2.y
    B = endpoint2.x - endpoint1.x
    C = endpoint1.x*endpoint2.y - endpoint2.x*endpoint1.y
    return A, B, -C

def orientation(p, q, r):
    val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0):
        return 0;  
    elif val > 0:
        return 1
    else:
        return 2

def doIntersect(ls1, ls2):
    p1 = ls1.endpoint1;
    q1 = ls1.endpoint2;
    p2 = ls2.endpoint1;
    q2 = ls2.endpoint2;
    o1 = orientation(p1, q1, p2);
    o2 = orientation(p1, q1, q2);
    o3 = orientation(p2, q2, p1);
    o4 = orientation(p2, q2, q1);
    
    #do not check co-linear case
    if (o1 != o2 and o3 != o4):
        L1 = lineSegEquation(p1,q1)
        L2 = lineSegEquation(p2,q2)
        D  = L1[0] * L2[1] - L1[1] * L2[0]
        Dx = L1[2] * L2[1] - L1[1] * L2[2]
        Dy = L1[0] * L2[2] - L1[2] * L2[0]
        x = Dx / D
        y = Dy / D
        p = endpoint(x,y)
        return p

    return False

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
                error("Invalid (x,y) pair", pair + ')')
                return None
    return tmp[1].strip(),coords

def addStreet(streets,inputLine):
    result = parseInputLine(inputLine)
    if result is not None:
        if len(result[1]) == 1:
            return "Invalid street!"
        if result[0] in streets:
            return result[0] + " has been added!"
        streets[result[0]] = street(result[1])
    else:
        return "Invalid input"

def changeStreet(streets,inputLine):
    result = parseInputLine(inputLine)
    if result is not None:
        if result[0] not in streets:
            return result + " does not exist!"
        streets[result[0]] = street(result[1])
    else:
        return "Invalid input"

def removeStreet(streets,inputLine):
	streets.clear()    #remove all streets!!! must use d.clear()

def error(message, line):
    sys.stderr.write("Error : %s:%s\n" % (message, line))

def unknown(line):
    return "Unknown python command!!"

def addVertex(vertices, vertex):
    if vertex not in vertices:
        vertices[vertex] = len(vertices)


#####################################################################
#### core part: generate graph
#####################################################################


def generateGraph(streets,inputLine):
    vertices = {}
    edges = set()
    keyList = streets.keys()
    
    #remove formerly computed vertices on each line segments
    for i in range(len(keyList)):
        for ls in streets[keyList[i]].ls:
            if ls.vertices:
                del ls.vertices[:]
    
    #re-compute the vertices on each line segments 
    for i in range(len(keyList)):
        j = i+1
        while(j < len(keyList)):
            try:
                for ls1 in streets[keyList[i]].ls:
                    for ls2 in streets[keyList[j]].ls:
                        res = doIntersect(ls1, ls2)
                        
                        if res:
                            ls1.makeVertices(res)
                            ls2.makeVertices(res)
                j+=1
            except:
                break;
    
    #based on the vertices on each line segment, assign it an unique ID
    #and output edges
    for i in range(len(keyList)):
        for ls in streets[keyList[i]].ls:
            if ls.vertices:
                for vertex in ls.vertices:
                    addVertex(vertices,vertex)
                k = 0
                while(k < len(ls.vertices)-1):
                    edges.add('<'+str(vertices[ls.vertices[k]])+','+str(vertices[ls.vertices[k+1]])+'>')
                    k+=1
    
    #output format matches street specification in assignment 2
    print('V '+str(len(vertices)))
    sys.stdout.flush()
    outStr =''
    for edge in edges:
        outStr += (edge+',')
    print(re.sub(',}','}','E {'+outStr+'}'))
    sys.stdout.flush()


#####################################################################
#### main function
#####################################################################

streets = {}

cmds = {'a': addStreet, 'c': changeStreet, 'g': generateGraph, 'r': removeStreet} 
def main():
    for line in iter(sys.stdin.readline, ""):
        line = line.strip()
        if line:
            command = cmds.get(line[0],unknown)
            message = command(streets,line)
            if message:
                error(message, line)
 
if __name__ == "__main__":
    main()