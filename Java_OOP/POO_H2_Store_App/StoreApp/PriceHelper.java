package StoreApp;

import org.apache.commons.lang3.Pair;

import java.util.ArrayList;

/**
 * The type Price helper.
 * <p>
 * Helper Class used for separating the {@linkplain Currency} from the price of a {@linkplain Product} and also
 * converting the price to a numeric value from a String.
 */
public class PriceHelper {
    /**
     * Paramless Constructor.
     * <p>
     * Instantiates a new Price helper.
     */
    protected PriceHelper() {
    }

    /**
     * Convert a {@linkplain Product}'s price from the 'CurrencyPrice' format to a Pair.
     *
     * @param priceWithCurrency the price with currency
     * @param currencies        the existing currencies
     * @return the pair of price and currency
     * @throws CurrencyNotFoundException the currency not found exception when the currency extracted from the price                                   does not exist in the currencies ArrayList
     */
    protected Pair< Double, Currency > convertStringToPriceCurrency(String priceWithCurrency,
                                                                    ArrayList< Currency > currencies)
            throws CurrencyNotFoundException {
        // Remove all the non-numeric characters and the commas from the string to get the price.
        String price = priceWithCurrency.replaceAll("[^0123456789.]", "");

        // Remove all the numeric characters, the points and the commas to get the currency's symbol and check if it
        // exists in the currencies ArrayList.
        String symbol = priceWithCurrency.replaceAll("[0123456789.,]", "");
        Currency currency = getCurrencyBySymbol(currencies, symbol);

        // If the currency doesn't exist throw an exception.
        if (currency == null) {
            throw new CurrencyNotFoundException();
        }

        // Return the price and currency pair.
        return new Pair<>(convertPrice(price), currency);
    }

    /**
     * Gets currency by its symbol.
     *
     * @param currencies the existing currencies
     * @param symbol     the currency's symbol
     * @return the currency with the corresponding symbol if it exists, else null
     */
    protected Currency getCurrencyBySymbol(ArrayList< Currency > currencies, String symbol) {
        for (Currency currency : currencies) {
            if (currency.getSymbol().equals(symbol)) {
                return currency;
            }
        }

        return null;
    }

    /**
     * Convert price from String to double.
     *
     * @param price the price
     * @return the double
     */
    protected double convertPrice(String price) {
        return Double.parseDouble(price);
    }

    /**
     * Convert price from a currency to another one.
     *
     * @param price           the price
     * @param currentCurrency the current currency
     * @param newCurrency     the new currency
     * @return the converted price
     */
    protected double convertPrice(double price, Currency currentCurrency, Currency newCurrency) {
        return price * currentCurrency.getParityToEur() / newCurrency.getParityToEur();
    }
}
