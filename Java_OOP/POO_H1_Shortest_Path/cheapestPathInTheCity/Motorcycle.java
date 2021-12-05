package cheapestPathInTheCity;

/**
 * The type Motorcycle.
 * <p>
 * Class that describes the behaviour of a motorcycle, extending the abstract class {@linkplain Vehicle}.
 */
class Motorcycle extends Vehicle {
    /**
     * No-argument Constructor.
     * <p>
     * Instantiates a new Motorcycle using the superclass constructor with the values 2 for cost and 1 for size(values
     * common to all motorcycles).
     */
    Motorcycle() {
        super(2, 1);
    }
}
