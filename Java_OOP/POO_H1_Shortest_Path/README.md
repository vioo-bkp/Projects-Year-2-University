## Travelling Service Project

# 1. Project Description

This is my implementation of a Travelling service application similar to Bolt, Uber, Taxify etc. The main idea of this
project is to compute the cheapest path from a point to another one in a city(if there is a path between these two
points, else it would return no cost(null)). The city is described by a map that consists of nodes interconnected by
streets. The travelling cost depends on the vehicle that is used for transportation(Bicycle, Motorcycle, Car or Truck).
Some streets may be unavailable for some type of vehicles because of their sizes. Traffic jams may happen on the city
streets, for example: accidents, heavy traffic and jams. Those will increase the travelling cost for any vehicle that
is driven on these streets.
The number of nodes and the streets are read from the input file "map.in" and a city map is created according to
them(addStreet). After this different type of vehicles may be driven on the city streets(drive) and traffic jams may
happen(addRestriction). If a vehicle is being driven in the city its path and cost will be printed in the output file
"map.out".

# 2. Project Structure

Package cheapestPathInTheCity:
    Class Hierarchy:
        - Main;
        - Map;
        - MyPriorityQueue<E> (implements Queue< E >);
        - Node (implements Comparable< Node >);
        - Street;
        - TrafficJam:
            - Accident;
            - Jam;
            - Traffic.
        - TrafficRestriction;
        - Vehicle:
            - Bicycle;
            - Car;
            - Motorcycle;
            - Truck.
    Interface Hierarchy:
        - Queue< E >

# 3.Details about the Implementation

Object Oriented Programming Concepts that were used in the making of this project:
    -> Inheritance – IS-A relations:
        ~ The subclasses Accident, Jam and Traffic extend the superclass TrafficJam;
        ~ The subclasses Bike, Motorcycle, Car and Truck extend the superclass Vehicle.
    -> Subtype Polymorphism - Object takes many forms or shapes => Upcasting:
        ~ A vehicle can be a Bike, a Motorcycle, a Car or a Truck;
        ~ A Traffic jam can be an Accident, a heavy Traffic or a Jam.
    -> Abstraction - Of data & code:
        ~ Interfaces: - Queue Interface used for grouping all the methods specific to this type of data structure(There
                      many types of queues: Simple Queue, Circular Queue, Priority Queue(This is the one that I
                      implemented in this project)).
        ~ Abstract classes: - Class Vehicle is used as an abstract base class for its subclasses(defining different
                            Vehicle types).
                            - Class TrafficJam is used as an abstract base class for its subclasses(defining different
                            types of Traffic jams).

# The Map
    The city map is composed of the number of nodes and the array of nodes. These nodes are interconnected to each other
by streets.
    A street can be added to the map by creating a new street and adding it to the corresponding node's street list(the
node from which the street begins, its start node).
    A Restriction can be added to a street increasing the cost of travelling on that street. The restriction can be
either an accident, a heavy traffic or a jam.
    People can drive a Vehicle from a node on the map to another one. In order to do this the distance between nodes is
set to the maximum integer value(if after applying the Dijkstra algorithm this value will be the same then it means that
there's no path between these nodes.
    The Dijkstra algorithm is used to compute the cheapest path from a point(node) on the map to another one. Set the
distance between the start node and itself to 0, push it in the Priority Queue that I implemented and start finding the
cheapest path from the start node(source node) to the rest of the nodes. The strategy is:
    - Poll an element from the priority queue(the one with the lowest priority, the minimum cost node);
    - Process its neighbours, its adjacent nodes:
        - Check if the adjacent node hasn't been visited already(its final cost has already been computed) and if the
        street supports the vehicle's size(if one of the conditions isn't fulfilled move to the next adjacent node);
        - Compute the new cost between these two nodes, using the formula: cost = street_cost * vehicle_cost + jam_cost;
        - If the new cost is cheaper than the old one update the cost between these two nodes and the predecessor array.
        Also update the node's priority in the queue;
    - Mark the node whose final cost has been computed as visited.
    The predecessors array stores the nodes that the vehicle encounters from the start node to the end node.
    Storing Strategy: predecessor[index of currentNode] stores the predecessor of the current node.
    The value returned is a String consisting of the path(nodes that form the path from start to end) + the cost of
travelling.

# The Nodes
    As I said earlier the Map consists of an array of Nodes. These nodes have an index, a street list(containing the
streets that begin from that node) and a cost(defining the cost to get from the start node to the current node in the
Dijkstra algorithm, also it represents its priority is the Priority Queue). I chose to implement it in this way so it
would be easier to compare two nodes when inserting one in the Priority Queue via the compareTo function.

# The Streets
    Each of them is defined by two Nodes: the street's start node(beginning) and its end node(end). Every street has its
own cost and a value that defines the size of the heaviest vehicle that may be driven on that street(TrafficRestriction).
On a street 3 types of TrafficJams may happen: Accident, Jam and Traffic. These Traffic jams add an extra cost when a
vehicle is driven on that street.

# The Priority Queue
    In order to keep the nodes in ascending order of their costs reported to the start node in the Dijkstra Algorithm I
implemented my own Generic Min Priority Queue. This class implements all the methods that any type of Queue should have
(defined in the Queue Interface). It uses a basic List from Java to store its elements. I made it generic so it could be
used on any data type that implements the Comparable Interface.
    The basic operations that can be done on a Priority Queue are:
        - finding the number of elements in the priority queue;
        - check whether a queue is empty or not;
        - inserting a new element in the priority queue:
            -> if the queue is empty the element is inserted right away;
            -> else find the first element in the queue with a higher priority than the element we're trying to insert,
shift all the elements after this one(including it) with one position to the right and insert it at the position that
was found earlier;
            -> if the end of the queue is reached it means that the element we're trying to insert has the highest
priority and it should be inserted at the end of the queue.
        - removing a specific element from the priority queue;
        - accessing the first element(with the lowest priority) without removing it from the priority queue(peek);
        - accessing and removing the first element from the priority queue(poll = peek + remove).

# The Queue Interface
    It is used for grouping all the basic methods specific to this type of data structure(There many types of queues:
Simple Queue, Circular Queue, Priority Queue etc): insertion(add), extraction(remove and poll) and inspection
operations(isEmpty, peek and size).

# The Vehicles
    Abstract Base Class that stores a type of vehicle, extended by the types of vehicles existing in the city: Bicycle,
Motorcycle, Car and Truck. I set its cost and size fields to final so they could be set only once at the creation of the
vehicle(like in the daily life, for example a car cannot lose weight:))). Only the class extending it can be
instantiated, it is a base class for them.

# The Traffic restriction
    Creates a Traffic restriction for a Street in a city. A Traffic restriction is defined by a cost(the cost to travel
that street) and by the size of the heaviest Vehicle that may be driven on that street.

# The Traffic jam
    It is an abstract Base Class that stores a generic type of traffic jam, extended by the types of traffic jams that
may happen on the streets of a city: Accident, Jam and Traffic. I set its cost field to final so it could be set only
once at the creation of the Traffic jam. Only the class extending it can be instantiated, it is a base class for them.

# The Main
    It is used for reading data for the map from the input file, creating it, adding Traffic jams on the streets of the
city, driving a vehicle from a node to another one within the city and printing the path to travel and the travelling
cost into the output file.
