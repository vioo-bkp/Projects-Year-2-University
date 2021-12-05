package cheapestPathInTheCity;

/**
 * The type Truck.
 * <p>
 * Class that describes the behaviour of a truck, extending the abstract class {@linkplain Vehicle}.
 */
class Truck extends Vehicle {
    /**
     * No-argument Constructor.
     * <p>
     * Instantiates a new Truck using the superclass constructor with the values 6 for cost and 3 for size(values common
     * to all trucks).
     */
    Truck() {
        super(6, 3);
    }
}