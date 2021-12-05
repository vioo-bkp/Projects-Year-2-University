package cheapestPathInTheCity;

import java.util.ArrayList;
import java.util.List;

/**
 * The type Street.
 * <p>
 * Class that describes the behaviour of a Street in a city. It is defined by two Nodes: the street's start node(point)
 * and its end node(point). Every street has its own cost and a value that defines the size of the heaviest vehicle that
 * may be driven on that street({@linkplain TrafficRestriction}). On a street 3 types of {@linkplain TrafficJam}s may
 * happen: {@linkplain Accident}, {@linkplain Jam} and {@linkplain Traffic}. These Traffic jams add an extra cost when
 * a vehicle is driven on that street.
 */
class Street {
    /**
     * Traffic Restriction that is applied on the street.
     */
    TrafficRestriction restriction;
    /**
     * Node that defines where the street starts.
     */
    private Node start;
    /**
     * Node that defines where the street ends.
     */
    private Node end;
    /**
     * List of Traffic Jams that are happening on the street.
     */
    private List< TrafficJam > trafficJams;

    /**
     * No-argument Constructor.
     */
    Street() {
    }

    /**
     * Instantiates a new Street.
     *
     * @param cost  the cost to travel on the street
     * @param size  the maximum size of a vehicle supported by the street
     * @param start the street's start point
     * @param end   the street's end point
     */
    Street(int cost, int size, Node start, Node end) {
        this.restriction = new TrafficRestriction(cost, size);
        this.start = start;
        this.end = end;
        trafficJams = new ArrayList<>();
    }

    /**
     * Gets the street's start point.
     *
     * @return the street's start point
     */
    public Node getStart() {
        return start;
    }

    /**
     * Sets the street's start point.
     *
     * @param start the street's start point
     */
    public void setStart(Node start) {
        this.start = start;
    }

    /**
     * Gets the street's end point.
     *
     * @return the street's end point
     */
    public Node getEnd() {
        return end;
    }

    /**
     * Sets the street's end point.
     *
     * @param end the street's end point
     */
    public void setEnd(Node end) {
        this.end = end;
    }

    /**
     * Gets the Traffic Restriction applied on the street.
     *
     * @return the Traffic Restriction applied on the street
     */
    public TrafficRestriction getRestriction() {
        return restriction;
    }

    /**
     * Sets the Traffic Restriction on a street.
     *
     * @param restriction the Traffic Restriction to be applied on the street
     */
    public void setRestrictions(TrafficRestriction restriction) {
        this.restriction = restriction;
    }

    /**
     * Gets the Traffic Jams that are happening on the street.
     *
     * @return the Traffic Jams that are happening on the street
     */
    public List< TrafficJam > getTrafficJams() {
        return trafficJams;
    }

    /**
     * Sets the Traffic Jams to happen on the street.
     *
     * @param trafficJams the Traffic Jams to happen on the street
     */
    public void setTrafficJams(List< TrafficJam > trafficJams) {
        this.trafficJams = trafficJams;
    }

    /**
     * Add a the Traffic Jam on the street.
     *
     * @param newTrafficJam the new Traffic Jam that will be added to the street
     */
    void addTrafficJams(TrafficJam newTrafficJam) {
        this.trafficJams.add(newTrafficJam);
    }
}
