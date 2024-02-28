package StoreApp;

import java.time.LocalDateTime;

/**
 * The type Discount.
 * <p>
 * Class that describes the behaviour of a Discount. It is characterized by a name, a type (it can be a whether a
 * PERCENTAGE DISCOUNT or a FIXED DISCOUNT), a value and the last date that it has been applied on a
 * {@linkplain Product}.
 */
public class Discount {
    /**
     * The Discount's Name.
     */
    private String name;

    /**
     * The Discount's type.
     */
    private DiscountType discountType;

    /**
     * The Discount's Value.
     */
    private double value;

    /**
     * The Last date the Discount has been applied.
     */
    private LocalDateTime lastDateApplied = null;

    /**
     * Paramless Constructor.
     * <p>
     * Instantiates a new Discount.
     */
    protected Discount() {
    }

    /**
     * Instantiates a new Discount.
     *
     * @param name         the Discount's name
     * @param discountType the Discount's type
     * @param value        the Discount's value
     */
    protected Discount(String name, DiscountType discountType, double value) {
        this.name = name;
        this.discountType = discountType;
        this.value = value;
    }

    /**
     * Gets the Discount's name.
     *
     * @return the Discount's name
     */
    protected String getName() {
        return name;
    }

    /**
     * Sets the Discount's name.
     *
     * @param name the Discount's name
     */
    protected void setName(String name) {
        this.name = name;
    }

    /**
     * Gets the Discount's type.
     *
     * @return the Discount's type
     */
    protected DiscountType getDiscountType() {
        return discountType;
    }

    /**
     * Sets the Discount's type.
     *
     * @param discountType the the Discount's type
     */
    public void setDiscountType(DiscountType discountType) {
        this.discountType = discountType;
    }

    /**
     * Gets the Discount's value.
     *
     * @return the Discount's value
     */
    protected double getValue() {
        return value;
    }

    /**
     * Sets the Discount's value.
     *
     * @param value the Discount's value
     */
    protected void setValue(double value) {
        this.value = value;
    }

    /**
     * Gets the last date the Discount has been applied.
     *
     * @return the last date the Discount has been applied
     */
    protected LocalDateTime getLastDateApplied() {
        return lastDateApplied;
    }

    /**
     * Sets the last date the Discount has been applied.
     *
     * @param lastDateApplied the last date the Discount has been applied
     */
    protected void setLastDateApplied(LocalDateTime lastDateApplied) {
        this.lastDateApplied = lastDateApplied;
    }

    /**
     * Sets the last date the Discount has been applied with the current date.
     */
    protected void setAsAppliedNow() {
        setLastDateApplied(LocalDateTime.now());
    }

    /**
     * Creates a string for the Discount.
     *
     * @return the Discount in a human-readable format
     */
    @Override
    public String toString() {
        return discountType + " " + value + " " + name + " " + lastDateApplied;
    }
}
