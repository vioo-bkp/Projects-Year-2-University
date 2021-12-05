package cheapestPathInTheCity;

/**
 * The type Accident.
 * <p>
 * Class that describes the behaviour of an accident on a street, extending the abstract class {@linkplain TrafficJam}.
 */
class Accident extends TrafficJam {
    /**
     * No-argument constructor.
     */
    Accident() {
    }

    /**
     * Instantiates a new Accident on a street using the superclass constructor with values specific to each accident.
     *
     * @param cost the accident's cost
     */
    Accident(int cost) {
        super(cost);
    }
}
