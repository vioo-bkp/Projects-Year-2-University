package StoreApp;

/**
 * The type Negative price exception.
 */
public class NegativePriceException extends Exception {
    /**
     * Instantiates a new Negative price exception.
     *
     * @param id the {@linkplain Product}'s id
     */
    protected NegativePriceException(String id) {
        super("The product " + id + " will have a negative price.");
    }
}
