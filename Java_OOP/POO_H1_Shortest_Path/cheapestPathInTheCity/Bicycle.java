package cheapestPathInTheCity;

/**
 * The type Bicycle.
 * <p>
 * Class that describes the behaviour of a bicycle, extending the abstract class {@linkplain Vehicle}.
 */
class Bicycle extends Vehicle {
    /**
     * No-argument constructor.
     * <p>
     * Instantiates a new Bicycle using the superclass constructor with the values 1 for cost and 1 for size(values
     * common to all bicycles).
     */
    Bicycle() {
        super(1, 1);
    }
}
