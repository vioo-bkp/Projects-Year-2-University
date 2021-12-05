package cheapestPathInTheCity;

/**
 * The type Vehicle.
 * <p>
 * Abstract Base Class that describes a vehicle.
 * <p>
 * It is extended by the types of vehicles that can be used to go from a place({@linkplain Node}) to another
 * one({@linkplain Node}): {@linkplain Bicycle}, {@linkplain Motorcycle}, {@linkplain Car} and {@linkplain Truck}.
 * <p>
 * Each one of the subclasses implement specific values for the {@linkplain #cost} and the {@linkplain #size}.
 */
abstract class Vehicle {
    /**
     * Integer value that can be set only once and defines the cost implied by the {@linkplain Vehicle} when
     * it is used to go from one place({@linkplain Node}) to another one({@linkplain Node}).
     */
    private final int cost;
    /**
     * Integer value that can be set only once and defines the size of the vehicle.
     */
    private final int size;

    /**
     * No-argument Constructor, never used.
     */
    private Vehicle() {
        this(0, 0);
    }

    /**
     * Constructor invoked by the subclasses when creating a new vehicle.
     *
     * @param cost value for the vehicle's cost
     * @param size value for the vehicle's size
     */
    Vehicle(int cost, int size) {
        this.cost = cost;
        this.size = size;
    }

    /**
     * Gets the vehicle's cost.
     *
     * @return the vehicle's cost
     */
    public int getCost() {
        return cost;
    }

    /**
     * Gets the vehicle's size.
     *
     * @return size the vehicle's size
     */
    public int getSize() {
        return size;
    }

}
