package StoreApp;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVPrinter;
import org.apache.commons.csv.CSVRecord;
import org.apache.commons.lang3.Pair;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.util.ArrayList;

/**
 * The type Store.
 * <p>
 * Class that describes the behaviour of a Store. It is characterized by a name, a list of products available in the
 * store, a currency in which then product's prices are stored (it is set by default to EUR) and a list of manufacturers
 * that produce the products available in the store.
 */
public class Store implements Serializable {
    /**
     * Instance of the Store.
     */
    private static Store storeInstance;

    /**
     * The Store's Name.
     */
    private String name;
    /**
     * The Store's Currency.
     */
    private Currency currency;
    /**
     * The Store's Products.
     */
    private ArrayList< Product > products;
    /**
     * The Store's Manufacturers.
     */
    private ArrayList< Manufacturer > manufacturers;

    /**
     * Paramless Constructor.
     * <p>
     * Instantiates a new Store.
     */
    private Store() {
        products = new ArrayList<>();
        manufacturers = new ArrayList<>();
        currency = new Currency("EUR", "€", 1.0);
    }

    /**
     * Gets the instance of the Store.
     * <p>
     * If the Store hasn't been instantiated yet a new one is created and returned, else it returns the instance of the
     * existing Store.
     *
     * @return the instance of the Store
     */
    protected static Store getInstance() {
        if (storeInstance == null) {
            storeInstance = new Store();
        }
        return storeInstance;
    }

    /**
     * Gets the Store's products.
     *
     * @return the Store's products
     */
    protected ArrayList< Product > getProducts() {
        return products;
    }

    /**
     * Sets the Store's products.
     *
     * @param products the Store's products
     */
    protected void setProducts(ArrayList< Product > products) {
        this.products = products;
    }

    /**
     * Gets the Store's manufacturers.
     *
     * @return the Store's manufacturers
     */
    protected ArrayList< Manufacturer > getManufacturers() {
        return manufacturers;
    }

    /**
     * Sets the Store's manufacturers.
     *
     * @param manufacturers the Store's manufacturers
     */
    protected void setManufacturers(ArrayList< Manufacturer > manufacturers) {
        this.manufacturers = manufacturers;
    }

    /**
     * Gets the Store's name.
     *
     * @return the Store's name
     */
    protected String getName() {
        return name;
    }

    /**
     * Sets the Store's name.
     *
     * @param name the Store's name
     */
    protected void setName(String name) {
        this.name = name;
    }

    /**
     * Gets the Store's currency.
     *
     * @return the Store's currency
     */
    protected Currency getCurrency() {
        return currency;
    }

    /**
     * Sets the Store's currency.
     *
     * @param currency the Store's currency
     */
    protected void setCurrency(Currency currency) {
        this.currency = currency;
    }

    /**
     * Read the products from a CSV file and add them to the Store.
     * <p>
     * If necessary their manufactures are also created and added to the Store manufacturer's list and their prices are
     * converted to the current currency set on the Store.
     *
     * @param filename   the CSV file
     * @param currencies the existing currencies
     * @return the array list of Products
     */
    protected ArrayList< Product > readCSV(String filename, ArrayList< Currency > currencies) {
        // Try to read the CSV file using Apache Commons CSV.
        try {
            Reader reader = Files.newBufferedReader(Paths.get(filename));
            Iterable< CSVRecord > records = CSVFormat.RFC4180.withFirstRecordAsHeader().parse(reader);

            // Read only the columns mentioned in the CSVFileHeader Enum (only there columns are using for creating a
            // new product) from each record.
            for (CSVRecord record : records) {
                String uniqID = record.get(CSVFileHeader.uniq_id);
                String product_name = record.get(CSVFileHeader.product_name);
                String manufacturer_name = record.get(CSVFileHeader.manufacturer);
                String price = record.get(CSVFileHeader.price);
                String number_available_in_stock = record.get(CSVFileHeader.number_available_in_stock);

                // If a product doesn't have a price it can't be added to the Store.
                if (price.equals("")) {
                    continue;
                }

                // Check if the manufacturer already exists in the Store's manufacturers list and return it if it does,
                // else create a new one and add it to the manufacturers list.
                Manufacturer manufacturer = createManufacturer(manufacturer_name);

                // Check if the product already exists in the Store's products list and if it doesn't create a new one
                // and add it to the products list.
                createProduct(uniqID, product_name, price.split(" ")[0], number_available_in_stock,
                        manufacturer, currencies);
            }
        } catch (IOException exception) {
            System.out.println("An error has occurred when reading the CSV file.");
        }

        return products;
    }

    /**
     * Save the products from the Store to a CSV file.
     *
     * @param filename the CSV file
     */
    protected void saveCSV(String filename) {
        // Format for the product's prices.
        DecimalFormat df = new DecimalFormat("###,##0.00");

        try {
            Writer writer = Files.newBufferedWriter(Paths.get(filename));
            CSVPrinter csvPrinter = new CSVPrinter(writer, CSVFormat.DEFAULT
                    .withHeader(CSVFileHeader.uniq_id.name(), CSVFileHeader.product_name.name(),
                            CSVFileHeader.manufacturer.name(), CSVFileHeader.price.name(),
                            CSVFileHeader.number_available_in_stock.name()));

            // Write each product from the Store to the CSV file.
            for (Product product : products) {
                csvPrinter.printRecord(product.getUniqueId(), product.getName(),
                        product.getManufacturer().getName(), currency.getSymbol() + df.format(product.getPrice()),
                        product.getQuantity() + " NEW");
            }

            csvPrinter.flush();
        } catch (IOException exception) {
            System.out.println("An error has occurred when writing the CSV file.");
        }
    }

    /**
     * Return a manufacturer corresponding to the name passed as param.
     * <p>
     * The manufacturer returned will be either a new one (if there is not any manufacturer with the same name already
     * in the Store's manufacturers list) or one with the same name.
     *
     * @param manufacturerName the manufacturer's name
     * @return the manufacturer
     */
    protected Manufacturer createManufacturer(String manufacturerName) {
        // If it doesn't have a name, then its name should be "Not Available".
        if (manufacturerName.equals("")) {
            manufacturerName = "Not Available";
        }

        // Create a new manufacturer for the name.
        Manufacturer manufacturer = new Manufacturer(manufacturerName);

        // Try to add it to the Store's manufacturers list.
        // If there's already a manufacturer with the same name in the Store's manufacturers list then return the one
        // that's already in the list.
        try {
            addManufacturer(manufacturer);
        } catch (DuplicateManufacturerException exception) {
            manufacturer = manufacturers.get(manufacturers.indexOf(manufacturer));
        }

        return manufacturer;
    }

    /**
     * Add a manufacturer to the Store's manufacturers list if it isn't already a manufacturer with the same name in it.
     *
     * @param manufacturer the manufacturer
     * @throws DuplicateManufacturerException the duplicate manufacturer exception
     */
    protected void addManufacturer(Manufacturer manufacturer) throws DuplicateManufacturerException {
        for (Manufacturer existingManufacturer : manufacturers) {
            if (manufacturer.equals(existingManufacturer)) {
                throw new DuplicateManufacturerException();
            }
        }

        manufacturers.add(manufacturer);
    }

    /**
     * Create a product and try to add it to the Store's product list if there isn't already a product with the same ID
     * in it.
     *
     * @param uniqID                 the Product's ID
     * @param productName            the Product's name
     * @param price                  the Product's price
     * @param numberAvailableInStock the Product's stock
     * @param manufacturer           the Product's manufacturer
     * @param currencies             the existing currencies
     */
    protected void createProduct(String uniqID, String productName, String price, String numberAvailableInStock,
                                 Manufacturer manufacturer, ArrayList< Currency > currencies) {
        // Get the product's stock.
        numberAvailableInStock = new StockHelper().getNumberAvailableInStock(numberAvailableInStock);

        // Get the product's price and currency.
        Pair< Double, Currency > priceCurrency;
        try {
            priceCurrency = new PriceHelper().convertStringToPriceCurrency(price, currencies);
        } catch (CurrencyNotFoundException exception) {
            System.out.println(exception.getMessage());
            return;
        }
        Currency currentCurrency = priceCurrency.right;
        double numericPrice = priceCurrency.left;

        // Convert the price to the Store's currency if necessary.
        if (!currentCurrency.equals(currency)) {
            numericPrice = new PriceHelper().convertPrice(priceCurrency.left, currentCurrency, this.currency);
        }

        // Build the Product.
        Product product = Product.Builder.newInstance()
                .setId(uniqID)
                .setName(productName)
                .setManufacturer(manufacturer)
                .setPrice(numericPrice)
                .setQuantity(Integer.parseInt(numberAvailableInStock))
                .build();

        // Try to add the Product to the Store's products list.
        try {
            addProduct(product);
        } catch (DuplicateProductException exception) {
            System.out.println(exception.getMessage());
        }
    }

    /**
     * Add a product to the Store's products list if there isn't already another product with the same ID in it.
     *
     * @param product the product
     * @throws DuplicateProductException the duplicate product exception
     */
    protected void addProduct(Product product) throws DuplicateProductException {
        // Try to add the product.
        for (Product existingProduct : products) {
            if (existingProduct.equals(product)) {
                throw new DuplicateProductException();
            }
        }
        products.add(product);

        // Increment the number of product produced by its manufacturer.
        Manufacturer manufacturer = product.getManufacturer();
        manufacturer.setCountProducts(manufacturer.getCountProducts() + 1);
    }

    /**
     * Change the Store's currency.
     * <p>
     * Recalculate all the products' prices and set the Store's currency to the new one.
     *
     * @param currency the Store's new currency
     * @throws CurrencyNotFoundException the currency not found exception
     */
    protected void changeCurrency(Currency currency) throws CurrencyNotFoundException {
        // Check if the currency exists.
        if (currency == null) {
            throw new CurrencyNotFoundException();
        }

        // Convert all the products' prices from the old currency to the new one.
        for (Product product : products) {
            double newPrice = new PriceHelper().convertPrice(product.getPrice(), this.currency, currency);
            product.setPrice(newPrice);
        }

        // Set the Store's new currency.
        this.currency = currency;
    }

    /**
     * Create a new currency.
     *
     * @param name        the currency's name
     * @param symbol      the currency's symbol
     * @param parityToEur the currency's parity to EUR
     * @return the new currency
     */
    protected Currency createCurrency(String name, String symbol, double parityToEur) {
        return new Currency(name, symbol, parityToEur);
    }

    /**
     * Create a new discount.
     *
     * @param discountType the discount's type
     * @param name         the discount's name
     * @param value        the discount's value
     * @return the new discount
     */
    protected Discount createDiscount(DiscountType discountType, String name, double value) {
        return new Discount(name, discountType, value);
    }

    /**
     * Gets the products from the Store produced by a specified manufacturer.
     *
     * @param manufacturer the manufacturer
     * @return the products produced by the manufacturer
     */
    protected ArrayList< Product > getProductsByManufacturer(Manufacturer manufacturer) {
        ArrayList< Product > productsFromManufacturer = new ArrayList<>();
        for (Product product : products) {
            if (product.getManufacturer().equals(manufacturer)) {
                productsFromManufacturer.add(product);
            }
        }

        return productsFromManufacturer;
    }

    /**
     * Calculate total cost of the products' from the list list passed as param.
     * <p>
     * A product's total cost is equal to its stock multiplied by its price.
     *
     * @param products the products
     * @return the double
     */
    protected double calculateTotal(ArrayList< Product > products) {
        double totalCost = 0;
        for (Product product : products) {
            totalCost += product.getPrice() * product.getQuantity();
        }

        return totalCost;
    }

    /**
     * Save the Store's current state to a binary file.
     *
     * @param filename the binary file
     */
    protected void saveStore(String filename) {
        try (ObjectOutputStream os = new ObjectOutputStream(new FileOutputStream(filename))) {
            os.writeObject(this);
            os.flush();
        } catch (IOException e) {
            System.out.println("An error occurred during the store's saving.");
        }
    }

    /**
     * Load an old state of the Store from a binary file.
     *
     * @param filename the binary file
     */
    protected void loadStore(String filename) {
        try (ObjectInputStream is = new ObjectInputStream(new FileInputStream(filename))) {
            storeInstance = (Store) is.readObject();
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("An error occurred during the store's loading.");
        }

    }

    /**
     * Apply a discount on all the products existing in the Store.
     *
     * @param discount the discount
     */
    protected void discountAllProducts(Discount discount) {
        for (Product product : products) {
            try {
                applyDiscount(discount, product);
            } catch (NegativePriceException exception) {
                System.out.println(exception.getMessage());
            } catch (DiscountNotFoundException exception) {
                System.out.println(exception.getMessage());
                return;
            }
        }
    }

    /**
     * Apply a discount on a product.
     *
     * @param discount the discount
     * @param product  the product
     * @throws DiscountNotFoundException the discount not found exception
     * @throws NegativePriceException    the negative price exception
     */
    protected void applyDiscount(Discount discount, Product product) throws DiscountNotFoundException,
            NegativePriceException {
        // Check if the discount exists.
        if (discount == null) {
            throw new DiscountNotFoundException();
        }

        double productPrice = product.getPrice();

        // Compute the new price depending on the discount's type.
        // If the discount would produce a negative price for the product it won't be applied and an exception will be
        // thrown.
        switch (discount.getDiscountType()) {
            case FIXED_DISCOUNT -> {
                if (Double.compare(productPrice, discount.getValue()) < 0) {
                    throw new NegativePriceException(product.getUniqueId());
                } else {
                    productPrice -= discount.getValue();
                }
            }
            case PERCENTAGE_DISCOUNT -> productPrice -= productPrice * discount.getValue() / 100;
        }

        // Set the product's new price and the discount that has been applied to it.
        product.setPrice(productPrice);
        product.setDiscount(discount);

        // Change the discount's last date applied with the current date.
        discount.setAsAppliedNow();
    }
}
