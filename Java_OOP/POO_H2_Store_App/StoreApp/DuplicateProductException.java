package StoreApp;

/**
 * The type Duplicate product exception.
 */
public class DuplicateProductException extends Exception {
    /**
     * Instantiates a new Duplicate product exception.
     */
    protected DuplicateProductException() {
        super("This product already exists in the store.");
    }
}
