package cheapestPathInTheCity;

/**
 * The type Traffic jam.
 * <p>
 * Abstract Base Class that describes a traffic jam.
 * <p>
 * It is extended by the types of traffic jams that may happen on the streets of a city: {@linkplain Accident},
 * {@linkplain Jam} and {@linkplain Traffic}.
 */
abstract class TrafficJam {
    /**
     * Integer value that can be set only once and defines the extra cost created by the traffic jam on a specific
     * street.
     */
    private final int cost;

    /**
     * No-argument Constructor.
     */
    TrafficJam() {
        this(0);
    }

    /**
     * Constructor invoked by the subclasses when creating a new Traffic jam.
     *
     * @param cost value for the extra cost created by the traffic jam
     */
    TrafficJam(int cost) {
        this.cost = cost;
    }

    /**
     * Gets the traffic jam's cost.
     *
     * @return the traffic jam's cost
     */
    public int getCost() {
        return cost;
    }

}