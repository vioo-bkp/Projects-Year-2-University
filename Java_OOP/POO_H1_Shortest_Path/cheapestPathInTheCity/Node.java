package cheapestPathInTheCity;

import java.util.ArrayList;
import java.util.List;

/**
 * The type Node.
 * <p>
 * Class that describes the behaviour of a node(vertex) in a graph(map).
 */
class Node implements Comparable< Node > {
    /**
     * Integer value that defines the nodes index in the graph(map).
     */
    private int nodeIndex;
    /**
     * List of Streets that start from the node.
     */
    private List< Street > streets;
    /**
     * Integer value that defines the cost to get from another node(the start node in the Dijkstra Algorithm) to this
     * one(This field will define the priority when we'll insert the nodes in the Priority Queue).
     */
    private int cost = Integer.MAX_VALUE;

    /**
     * No-argument Constructor.
     */
    Node() {
    }

    /**
     * Instantiates a new Node.
     * <p>
     * Creates an empty list of streets for the node.
     *
     * @param nodeIndex the node's index
     */
    Node(int nodeIndex) {
        this.nodeIndex = nodeIndex;
        this.streets = new ArrayList<>();
    }

    /**
     * Gets node's index.
     *
     * @return the node's index
     */
    public int getNodeIndex() {
        return nodeIndex;
    }

    /**
     * Gets the streets that start from this node.
     *
     * @return the streets that start from this node
     */
    public List< Street > getStreets() {
        return streets;
    }

    /**
     * Sets the streets that start from this node..
     *
     * @param streets the streets that start from this node.
     */
    public void setStreets(List< Street > streets) {
        this.streets = streets;
    }

    /**
     * Gets the cost to get from another node(the start node in the Dijkstra Algorithm) to this one.
     *
     * @return the cost to get from another node(the start node in the Dijkstra Algorithm) to this one
     */
    public int getCost() {
        return cost;
    }

    /**
     * Sets the cost to get from another node(the start node in the Dijkstra Algorithm) to this one.
     *
     * @param cost the cost to get from another node(the start node in the Dijkstra Algorithm) to this one
     */
    public void setCost(int cost) {
        this.cost = cost;
    }

    /**
     * Gets a street that starts from this node and ends in some other specified node.
     *
     * @param end the end of the street that we're looking for
     * @return the street that we're looking for or null if it doesn't exist
     */
    Street getStreet(int end) {
        /* Compare each street's end node index with the one we're looking for. */
        for (Street street : this.streets) {
            if (street.getEnd().getNodeIndex() == end) {
                return street;
            }
        }

        /* If a street between these two nodes doesn't exist, null is returned. */
        return null;
    }

    /**
     * Node in a format friendly for printing.
     *
     * @return P + the node's index
     */
    @Override
    public String toString() {
        return ("P" + this.nodeIndex);
    }

    /**
     * Compare two nodes by their cost reported to some other node(the start node in the Dijkstra Algorithm).
     *
     * @param otherNode node to be compared
     * @return a negative integer, zero, or a positive integer as the first node's cost is less than, equal to, or
     * greater than the second one's cost.
     */
    @Override
    public int compareTo(Node otherNode) {
        return Integer.compare(this.cost, otherNode.getCost());
    }
}