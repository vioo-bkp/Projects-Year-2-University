package StoreApp;

/**
 * The type Duplicate manufacturer exception.
 */
public class DuplicateManufacturerException extends Exception {
    /**
     * Instantiates a new Duplicate manufacturer exception.
     */
    protected DuplicateManufacturerException() {
        super("This manufacturer already exists in the manufacturers list.");
    }
}
