package StoreApp;

/**
 * The type Stock helper.
 * <p>
 * Helper Class used for extracting the number available in stock of a Product from a CSV Record.
 */
public class StockHelper {
    /**
     * Paramless Constructor.
     * <p>
     * Instantiates a new Stock helper.
     */
    protected StockHelper() {
    }

    /**
     * Gets the number available in stock.
     *
     * @param numberAvailableInStock string containing the number available in stock and other useless informations
     * @return the number available in stock
     */
    protected String getNumberAvailableInStock(String numberAvailableInStock) {
        // Remove the non breaking space from the string.
        int index = numberAvailableInStock.indexOf(" ");
        if (index > 0)
            numberAvailableInStock = numberAvailableInStock.substring(0, index);

        // If the string is left empty then there aren't any products of this type left in the stock.
        if (numberAvailableInStock.equals("")) {
            numberAvailableInStock = "0";
        }

        return numberAvailableInStock;
    }
}
