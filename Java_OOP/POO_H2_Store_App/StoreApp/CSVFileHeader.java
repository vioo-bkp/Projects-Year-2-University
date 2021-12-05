package StoreApp;

/**
 * The enum Csv file header.
 * <p>
 * Used for reading only the uniq_id, product_name, manufacturer, price and number_available_in_stock columns from a
 * CSV file.
 */
enum CSVFileHeader {
    /**
     * Uniq id column name.
     */
    uniq_id,
    /**
     * Product name column name.
     */
    product_name,
    /**
     * Manufacturer column name.
     */
    manufacturer,
    /**
     * Price column name.
     */
    price,
    /**
     * Number available in stock column name.
     */
    number_available_in_stock
}