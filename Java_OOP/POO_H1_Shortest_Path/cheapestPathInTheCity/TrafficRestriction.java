package cheapestPathInTheCity;

/**
 * The type Traffic restriction.
 * <p>
 * Class that describes a restriction on a {@linkplain Street} in a city. A Traffic restriction is defined by a
 * {@linkplain #cost}(the cost to travel that street) and by the {@linkplain #size} of the heaviest {@linkplain Vehicle}
 * that may be driven on that street.
 * <p>
 * These two values, {@linkplain #cost} and {@linkplain #size}, are set only once(when the street is added on the map)
 * and they can't be changed.
 */
class TrafficRestriction {
    /**
     * Integer value that can be set only once and defines the cost of travelling on that street.
     */
    private final int cost;

    /**
     * Integer value that can be set only once and defines size/weight of the heaviest vehicle that may be driven on
     * that street.
     */
    private final int size;

    /**
     * No-argument Constructor, never used.
     */
    private TrafficRestriction() {
        this(0, 0);
    }

    /**
     * Instantiates a new Traffic restriction for a street. It is used when a street is added on the map.
     *
     * @param cost the cost implied by the Traffic restriction
     * @param size the maximum size of a vehicle supported on that street
     */
    TrafficRestriction(int cost, int size) {
        this.cost = cost;
        this.size = size;
    }

    /**
     * Gets the cost implied by the Traffic restriction.
     *
     * @return the cost implied by the Traffic restriction
     */
    public int getCost() {
        return cost;
    }

    /**
     * Gets the maximum size of a vehicle supported by that street.
     *
     * @return the maximum size of a vehicle supported by that street
     */
    public int getSize() {
        return size;
    }
}
