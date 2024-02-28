package StoreApp;

/**
 * The type Discount not found exception.
 */
public class DiscountNotFoundException extends Exception {
    /**
     * Instantiates a new Discount not found exception.
     */
    protected DiscountNotFoundException() {
        super("The discount does not exist.");
    }
}
