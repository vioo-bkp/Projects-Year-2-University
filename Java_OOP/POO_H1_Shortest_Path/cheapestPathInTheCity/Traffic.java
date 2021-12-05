package cheapestPathInTheCity;

/**
 * The type Traffic.
 * <p>
 * Class that describes the behaviour of a heavy traffic on a street, extending the abstract class
 * {@linkplain TrafficJam}.
 */
class Traffic extends TrafficJam {
    /**
     * No-argument Constructor.
     */
    Traffic() {
    }

    /**
     * Instantiates a new heavy Traffic on a street using the superclass constructor with values specific for each heavy
     * traffic.
     *
     * @param cost the accident's cost
     */
    Traffic(int cost) {
        super(cost);
    }
}
