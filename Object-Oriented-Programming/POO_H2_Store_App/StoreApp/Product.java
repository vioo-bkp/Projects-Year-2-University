package StoreApp;

import java.io.Serializable;
import java.util.Objects;

/**
 * The type Product.
 * <p>
 * Class that describes the behaviour of a Product. It is characterized by an ID, a name, a manufacturer, a quantity, a
 * discount (null if no discount has been applied on it) and a price.
 */
public class Product implements Serializable {
    /**
     * The Product's ID.
     */
    private final String uniqueId;

    /**
     * The Product's name.
     */
    private final String name;

    /**
     * The Product's manufacturer.
     */
    private final Manufacturer manufacturer;

    /**
     * The Product's quantity.
     */
    private int quantity;

    /**
     * The Product's discount.
     */
    private Discount discount;

    /**
     * The Product's price.
     */
    private double price;

    /**
     * Instantiates a new Product using the productBuilder.
     *
     * @param productBuilder Instance of Builder containing all the the Product's info.
     */
    private Product(Builder productBuilder) {
        this.uniqueId = productBuilder.uniqueId;
        this.name = productBuilder.name;
        this.manufacturer = productBuilder.manufacturer;
        this.price = productBuilder.price;
        this.quantity = productBuilder.quantity;
        this.discount = productBuilder.discount;
    }

    /**
     * Gets the Product's ID.
     *
     * @return the the Product's ID
     */
    protected String getUniqueId() {
        return uniqueId;
    }

    /**
     * Gets the Product's name.
     *
     * @return the Product's name
     */
    protected String getName() {
        return name;
    }

    /**
     * Gets the Product's manufacturer.
     *
     * @return the Product's manufacturer
     */
    protected Manufacturer getManufacturer() {
        return manufacturer;
    }

    /**
     * Gets the Product's price.
     *
     * @return the Product's price
     */
    protected double getPrice() {
        return price;
    }

    /**
     * Sets the Product's price.
     *
     * @param price the Product's new price
     */
    protected void setPrice(double price) {
        this.price = price;
    }

    /**
     * Gets the Product's quantity.
     *
     * @return the Product's quantity
     */
    protected int getQuantity() {
        return quantity;
    }

    /**
     * Sets the Product's quantity.
     *
     * @param quantity the the Product's new quantity
     */
    protected void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    /**
     * Gets the Product's discount.
     *
     * @return the Product's discount
     */
    protected Discount getDiscount() {
        return discount;
    }

    /**
     * Sets the Product's discount.
     *
     * @param discount the discount that has been applied on the product
     */
    protected void setDiscount(Discount discount) {
        this.discount = discount;
    }

    /**
     * The method compares the equality of the current Product object with another object by their type (whether
     * they're both Products or not) and their ID if they're both instances of the Product type.
     *
     * @return true if two products are the same or have the same ID, else false
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Product product = (Product) o;
        return Objects.equals(getUniqueId(), product.getUniqueId());
    }

    /**
     * The type Builder.
     * <p>
     * Used for building a Product.
     */
    protected static class Builder {
        /**
         * The ID.
         */
        private String uniqueId;

        /**
         * The Name.
         */
        private String name;

        /**
         * The Manufacturer.
         */
        private Manufacturer manufacturer;

        /**
         * The Price.
         */
        private double price;

        /**
         * The Quantity.
         */
        private int quantity;

        /**
         * The Discount.
         */
        private Discount discount;

        /**
         * Paramless Constructor.
         * <p>
         * Instantiates a new Builder.
         */
        private Builder() {
        }

        /**
         * New instance Builder.
         *
         * @return the new Builder
         */
        protected static Builder newInstance() {
            return new Builder();
        }

        /**
         * Sets the ID.
         *
         * @param uniqueId the ID
         * @return the ID
         */
        protected Builder setId(String uniqueId) {
            this.uniqueId = uniqueId;
            return this;
        }

        /**
         * Sets the manufacturer.
         *
         * @param manufacturer the manufacturer
         * @return the manufacturer
         */
        protected Builder setManufacturer(Manufacturer manufacturer) {
            this.manufacturer = manufacturer;
            return this;
        }

        /**
         * Sets the price.
         *
         * @param price the price
         * @return the price
         */
        protected Builder setPrice(double price) {
            this.price = price;
            return this;
        }

        /**
         * Sets the quantity.
         *
         * @param quantity the quantity
         * @return the quantity
         */
        protected Builder setQuantity(int quantity) {
            this.quantity = quantity;
            return this;
        }

        /**
         * Sets the discount.
         *
         * @param discount the discount
         * @return the discount
         */
        protected Builder setDiscount(Discount discount) {
            this.discount = discount;
            return this;
        }

        /**
         * Sets the name.
         *
         * @param name the name
         * @return the name
         */
        protected Builder setName(String name) {
            this.name = name;
            return this;
        }

        /**
         * Builds the product.
         *
         * @return the product
         */
        protected Product build() {
            return new Product(this);
        }
    }

}
