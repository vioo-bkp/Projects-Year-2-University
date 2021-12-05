package StoreApp;

import java.io.Serializable;
import java.util.Comparator;
import java.util.Objects;

/**
 * The type Manufacturer.
 * <p>
 * Class that describes the behaviour of a Manufacturer. It is characterized by a name and the number of Products that
 * it produces.
 */
public class Manufacturer implements Serializable {
    /**
     * Comparator for Manufacturers by their names.
     */
    protected static Comparator< Manufacturer > ManufacturerNameComparator = (manufacturer1, manufacturer2) -> {
        String manufacturer1Name = manufacturer1.getName();
        String manufacturer2Name = manufacturer2.getName();

        return manufacturer1Name.compareTo(manufacturer2Name);

    };

    /**
     * The Manufacturer's Name.
     */
    private String name;

    /**
     * The number of products that the Manufacturer produces.
     */
    private int countProducts = 0;

    /**
     * Paramless Constructor.
     * <p>
     * Instantiates a new Manufacturer.
     *
     * @param name the Manufacturer's name
     */
    protected Manufacturer(String name) {
        this.name = name;
    }

    /**
     * Gets the Manufacturer's name.
     *
     * @return the Manufacturer's name
     */
    protected String getName() {
        return name;
    }

    /**
     * Sets the Manufacturer's name.
     *
     * @param name the Manufacturer's name
     */
    protected void setName(String name) {
        this.name = name;
    }

    /**
     * Gets the number of products that the Manufacturer produces.
     *
     * @return the number of products that the Manufacturer produces
     */
    protected int getCountProducts() {
        return countProducts;
    }

    /**
     * Sets the number of products that the Manufacturer produces.
     *
     * @param countProducts the number of products that the Manufacturer produces
     */
    protected void setCountProducts(int countProducts) {
        this.countProducts = countProducts;
    }

    /**
     * The method compares the equality of the current Manufacturer object with another object by their type (whether
     * they're both Manufacturers or not) and their symbol if they're both instances of the Manufacturer type.
     *
     * @return true if two manufacturers are the same or have the same name, else false
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Manufacturer that = (Manufacturer) o;
        return Objects.equals(name, that.name);
    }
}
