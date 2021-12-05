package StoreApp;

/**
 * The type Currency not found exception.
 */
public class CurrencyNotFoundException extends Exception {
    /**
     * Instantiates a new Currency not found exception.
     */
    protected CurrencyNotFoundException() {
        super("The currency does not exist.");
    }
}
