package StoreApp;

import java.io.Serializable;
import java.util.Comparator;
import java.util.Objects;

/**
 * The type Currency.
 * <p>
 * Class that describes the behaviour of a Currency. It is characterized by a name, a symbol and a parity to EUR.
 */
public class Currency implements Serializable {
    /**
     * Comparator for Currencies by their names.
     */
    protected static Comparator< Currency > CurrencyNameComparator = (currency1, currency2) -> {
        String currency1Name = currency1.getName();
        String currency2Name = currency2.getName();

        return currency1Name.compareTo(currency2Name);
    };

    /**
     * The Currency's Name.
     */
    private final String name;
    /**
     * The Currency's symbol.
     */
    private final String symbol;

    /**
     * The Currency's Parity to EUR.
     */
    private double parityToEur;

    /**
     * Instantiates a new Currency.
     *
     * @param name        the Currency's name
     * @param symbol      the Currency's symbol
     * @param parityToEur the Currency's parity to EUR
     */
    protected Currency(String name, String symbol, double parityToEur) {
        this.name = name;
        this.symbol = symbol;
        this.parityToEur = parityToEur;
    }

    /**
     * Gets the Currency's name.
     *
     * @return the Currency's name
     */
    protected String getName() {
        return name;
    }

    /**
     * Gets the Currency's symbol.
     *
     * @return the Currency's symbol
     */
    protected String getSymbol() {
        return symbol;
    }

    /**
     * Gets the Currency's parity to EUR.
     *
     * @return the Currency's parity to EUR
     */
    protected double getParityToEur() {
        return parityToEur;
    }

    /**
     * Sets the Currency's parity to EUR.
     *
     * @param parityToEur the Currency's parity to EUR
     */
    protected void setParityToEur(double parityToEur) {
        this.parityToEur = parityToEur;
    }

    /**
     * Update the Currency's parity to EUR.
     *
     * @param parityToEUR the Currency's new parity to EUR
     */
    protected void updateParity(double parityToEUR) {
        this.setParityToEur(parityToEUR);
    }

    /**
     * The method compares the equality of the current Currency object with another object by their type (whether
     * they're both Currencies or not) and their symbol if they're both instances of the Currency type.
     *
     * @return true if two currencies are the same or have the same symbol, else false
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Currency currency = (Currency) o;
        return Objects.equals(getSymbol(), currency.getSymbol());
    }
}
