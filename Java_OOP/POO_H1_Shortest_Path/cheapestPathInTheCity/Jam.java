package cheapestPathInTheCity;

/**
 * The type Jam.
 * <p>
 * Class that describes the behaviour of a jam on a street, extending the abstract class {@linkplain TrafficJam}.
 */
class Jam extends TrafficJam {
    /**
     * No-argument Constructor.
     */
    Jam() {
    }

    /**
     * Instantiates a new Jam on a street using the superclass constructor with values specific for each jam.
     *
     * @param cost the accident's cost
     */
    Jam(int cost) {
        super(cost);
    }
}
