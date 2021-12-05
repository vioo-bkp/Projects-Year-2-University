package cheapestPathInTheCity;

/**
 * The type Car.
 * <p>
 * Class that describes the behaviour of a car, extending the abstract class {@linkplain Vehicle}.
 */
class Car extends Vehicle {
    /**
     * No-argument constructor.
     * <p>
     * Instantiates a new Car using the superclass constructor with the values 4 for cost and 2 for size(values common
     * to all cars).
     */
    Car() {
        super(4, 2);
    }
}