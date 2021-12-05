package cheapestPathInTheCity;

import java.util.ArrayList;
import java.util.List;

/**
 * The type Map.
 * <p>
 * Class that describes the behaviour of a city Map which consists of an array of nodes interconnected by streets. Each
 * street has different restrictions, jams(accidents, heavy traffic, jams).
 * <p>
 * People can get from one place(node) to another by driving a vehicle and they're interested in getting there via the
 * cheapest path(the minimum cost path).
 */
class Map {
    /**
     * Integer value that defines the number of nodes in the map.
     */
    private int noNodes;
    /**
     * Array of nodes that define the city map.
     */
    private Node[] nodes;

    /**
     * No-argument Constructor.
     */
    Map() {
    }

    /**
     * Instantiates a new Map.
     * <p>
     * Sets the number of nodes and instantiates each node from the array.
     *
     * @param noNodes the number of nodes that form the map
     */
    Map(int noNodes) {
        this.noNodes = noNodes;
        this.nodes = new Node[noNodes];

        for (int i = 0; i < noNodes; i++) {
            this.nodes[i] = new Node(i);
        }

    }

    /**
     * Gets the number of nodes that form the map.
     *
     * @return number of nodes that form the map
     */
    public int getNoNodes() {
        return noNodes;
    }

    /**
     * Sets number of nodes that form the map.
     *
     * @param noNodes number of nodes that form the map
     */
    public void setNoNodes(int noNodes) {
        this.noNodes = noNodes;
    }

    /**
     * Get the nodes that form the map.
     *
     * @return the nodes that form the map
     */
    public Node[] getNodes() {
        return nodes;
    }

    /**
     * Sets the nodes that form the map.
     *
     * @param nodes the nodes that form the map
     */
    public void setNodes(Node[] nodes) {
        this.nodes = nodes;
    }

    /**
     * Add a new street to the map.
     *
     * @param start the start point of the street
     * @param end   the end point of the street
     * @param cost  the cost associated with the street
     * @param size  the maximum size of a vehicle that can be supported by the street
     */
    public void addStreet(int start, int end, int cost, int size) {
        /* Create new street and add it to the corresponding start node's streets list. */
        Street newStreet = new Street(cost, size, this.nodes[start], this.nodes[end]);
        List< Street > streets = this.nodes[start].getStreets();
        streets.add(newStreet);
    }

    /**
     * Add a restriction to a street.
     * <p>
     * The restriction that we're trying to add can be either an accident, a heavy traffic or a jam.
     *
     * @param type  the restriction's type
     * @param start the start point of the street
     * @param end   the end point of the street
     * @param cost  the cost associated with this restriction
     */
    public void AddRestriction(String type, int start, int end, int cost) {
        /* Checks if the street exists on the map first.
        If it doesn't exist then we can't add a restriction to it. */
        Street street = this.nodes[start].getStreet(end);
        if (street == null) {
            return;
        }

        /* Adds the corresponding Traffic jam to the street. */
        if (type.equals("accident")) {
            street.addTrafficJams(new Accident(cost));
        } else if (type.equals("blocaj")) {
            street.addTrafficJams(new Jam(cost));
        } else {
            street.addTrafficJams(new Traffic(cost));
        }
    }

    /**
     * Try to drive from a point(node) on the map to another one via the path with the cheapest cost.
     *
     * @param vehicle the vehicle used for driving
     * @param start   the start point
     * @param end     the end point
     * @return the path from start to end + the cost of driving
     */
    public String drive(Vehicle vehicle, int start, int end) {
        /* Sets the distance between the start node and the rest of the nodes on the map to the maximum integer
        value(if after applying the Dijkstra algorithm this value will be the same then it means that there's no path
        between these nodes). */
        for (Node node : this.nodes) {
            node.setCost(Integer.MAX_VALUE);
        }

        Node endNode = this.nodes[end], startNode = this.nodes[start];

        /* Apply the Dijkstra algorithm from the start node to the end node in order to find the cheapest path. */
        Node[] predecessors = Dijkstra(startNode, vehicle);

        /* Return the path between the start node and the end node + the cost associated with it. */
        return processPath(startNode, endNode, predecessors);
    }

    /**
     * Dijkstra algorithm to compute the cheapest path from a point(node) on the map to another one.
     * <p>
     * The cheapest path is the path with the minimum travelling cost.
     * <p>
     * The travelling cost is influenced by the vehicle's type, the traffic jams that could pe encountered on streets,
     * the maximum size of a vehicle that can be driven on that street and the street's cost.
     *
     * @param startNode the start node
     * @param vehicle   the vehicle used for driving
     * @return the nodes that form the path
     */
    private Node[] Dijkstra(Node startNode, Vehicle vehicle) {
        /* Sets distance between the start node and itself to be 0. */
        startNode.setCost(0);

        /* Creates a Priority Queue of Nodes and inserts the start node in it. */
        MyPriorityQueue< Node > priorityQueue = new MyPriorityQueue<>();
        priorityQueue.add(startNode);

        /* Creates a List of Visited Nodes(it will store the nodes for which the cost from the start node to them has
        already been computed) and inserts the start node in it. */
        List< Node > visited = new ArrayList<>();
        visited.add(startNode);

        /* Creates an array of Nodes that will be used to store the nodes that form the path.
        Storing Strategy: predecessor[index of currentNode] stores the predecessor of the current node. */
        Node[] predecessors = new Node[this.noNodes];
        predecessors[startNode.getNodeIndex()] = startNode;

        /* Finds the cheapest path from the start node(source node) to the rest of the nodes. */
        while (!priorityQueue.isEmpty()) {
            /* Remove the minimum cost node from the priority queue. */
            Node node = priorityQueue.poll();

            /* Processes the current node's neighbours(nodes that are directly connected to the start node via a street,
            its adjacent nodes). */
            processNeighbours(node, vehicle, priorityQueue, visited, predecessors);

            /* Marks the node whose final cost has been computed as visited. */
            visited.add(node);
        }
        /* Returns the array containing the path from the start Node to the end Node. */
        return predecessors;
    }

    /**
     * Process the node's neighbours, update the node's cost relative to the start node(their priority) in case a
     * cheaper cost is found, update the predecessors array of nodes that will form the path and add new nodes to the
     * Priority Queue.
     *
     * @param node          the Node whose neighbours we're processing
     * @param vehicle       the Vehicle that is used to drive on the streets
     * @param priorityQueue the Priority Queue used for storing the Nodes ordered ascending by their cost reported to
     *                      the startNode
     * @param visited       the List of Nodes that have already been processed
     * @param predecessors  the nodes that form the path
     */
    private void processNeighbours(Node node, Vehicle vehicle, MyPriorityQueue< Node > priorityQueue,
                                   List< Node > visited, Node[] predecessors) {
        /* For each node that is directly connected to the current Node. */
        for (Street street : node.getStreets()) {
            Node endNode = street.getEnd();
            TrafficRestriction streetRestriction = street.getRestriction();

            /* Checks if the end node hasn't been visited already(its final cost has already been computed) and if the
            street supports the vehicle's size. */
            if (!visited.contains(endNode) && streetRestriction.getSize() >= vehicle.getSize()) {

                /* Computes the new cost between these two nodes. */
                int newCost = node.getCost() + streetRestriction.getCost() * vehicle.getCost();
                int endIndex = endNode.getNodeIndex();
                for (TrafficJam trafficJam : street.getTrafficJams()) {
                    newCost += trafficJam.getCost();
                }

                /* If the new cost is cheaper than the old one it updates the cost(between the start node and the
                current one) and the predecessor array.
                It also updates the node's priority in the queue. */
                if (newCost < endNode.getCost()) {
                    priorityQueue.remove(endNode);
                    endNode.setCost(newCost);
                    predecessors[endIndex] = node;
                    priorityQueue.add(endNode);
                }
            }
        }
    }

    /**
     * Create a String containing the nodes that form the path from the start node to the end node + the cost of it.
     *
     * @param startNode    the start node
     * @param endNode      the end node
     * @param predecessors the array of nodes that defines the path
     * @return the path between the two nodes + its cost
     */
    private String processPath(Node startNode, Node endNode, Node[] predecessors) {
        /* Gets the cost from start node to end node. */
        int cost = endNode.getCost();
        StringBuilder path = new StringBuilder();

        /* Follows the path from the end node to the start node and add each node to the path.
           end node -> ... -> start node
           The predecessors array stores at the current node's index its predecessor in the path. */
        for (Node node = endNode; node != null && node.getNodeIndex() != startNode.getNodeIndex();
             node = predecessors[node.getNodeIndex()]) {
            path.insert(0, " ");
            path.insert(0, node);
        }

        path.insert(0, " ");
        path.insert(0, startNode);

        /* If the cost is equal to the maximum integer value it means that there's no path between the two nodes, so
         we append null instead of the cost to get from start node to end node.
         */
        if (cost == Integer.MAX_VALUE) {
            path.append("null");
        } else {
            path.append(cost);
        }

        /* Returns path + cost of the path. */
        return path.toString();
    }

}