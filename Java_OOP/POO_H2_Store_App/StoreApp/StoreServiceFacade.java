package StoreApp;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * The type Store Service Facade.
 * <p>
 * Class that describes the behaviour of a Control Panel for the Owner of the Store. It is characterized by store, a
 * list a currencies and one of discounts.
 */
public class StoreServiceFacade {
    /**
     * The Currency List.
     */
    private ArrayList< Currency > currencies;

    /**
     * The Discount List.
     */
    private ArrayList< Discount > discounts;

    /**
     * The Store.
     */
    private Store store;

    /**
     * Instantiates a new Store service facade.
     * <p>
     * By default the currency will be EUR.
     */
    protected StoreServiceFacade() {
        this.store = Store.getInstance();

        this.currencies = new ArrayList<>();
        this.currencies.add(new Currency("EUR", "€", 1.0));

        this.discounts = new ArrayList<>();
    }

    /**
     * Gets the currencies.
     *
     * @return the currencies
     */
    protected ArrayList< Currency > getCurrencies() {
        return currencies;
    }

    /**
     * Sets the currencies.
     *
     * @param currencies the currencies
     */
    protected void setCurrencies(ArrayList< Currency > currencies) {
        this.currencies = currencies;
    }

    /**
     * Gets the discounts.
     *
     * @return the discounts
     */
    protected ArrayList< Discount > getDiscounts() {
        return discounts;
    }

    /**
     * Sets the discounts.
     *
     * @param discounts the discounts
     */
    protected void setDiscounts(ArrayList< Discount > discounts) {
        this.discounts = discounts;
    }

    /**
     * Gets the store.
     *
     * @return the store
     */
    protected Store getStore() {
        return store;
    }

    /**
     * Sets the store.
     *
     * @param store the store
     */
    protected void setStore(Store store) {
        this.store = store;
    }

    /**
     * Control panel for the Store's owner.
     * <p>
     * The Store Owner controls the Store via this panel of functionalities.
     *
     * @param command the command
     */
    protected void controlPanel(String command) {
        command = command.replace("\n", "");
        String[] commandInfo = command.split(" (?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)", -1);

        switch (commandInfo[0]) {
            case "listcurrencies" -> listCurrencies();
            case "getstorecurrency" -> getStoreCurrency();
            case "addcurrency" -> addCurrency(commandInfo[1], commandInfo[2], commandInfo[3]);
            case "loadcsv" -> loadCSV(commandInfo[1]);
            case "savecsv" -> saveCSV(commandInfo[1]);
            case "setstorecurrency" -> setStoreCurrency(commandInfo[1]);
            case "updateparity" -> updateParity(commandInfo[1], commandInfo[2]);
            case "listproducts" -> listProducts();
            case "showproduct" -> showProduct(commandInfo[1]);
            case "listmanufacturers" -> listManufacturers();
            case "listproductsbymanufacturarer" -> listProductsByManufacturer(command.substring(command.indexOf(' ') + 1));
            case "listdiscounts" -> listDiscounts();
            case "addiscount" -> addDiscount(commandInfo[1], commandInfo[2], commandInfo[3]);
            case "applydiscount" -> applyDiscount(commandInfo[1], commandInfo[2]);
            case "calculatetotal" -> calculateTotal(Arrays.copyOfRange(commandInfo, 1,
                    commandInfo.length));
            case "savestore" -> saveStore(commandInfo[1]);
            case "loadstore" -> loadStore(commandInfo[1]);
            case "quit", "exit" -> System.exit(0);
        }
    }

    /**
     * Gets the Store's currency.
     */
    protected void getStoreCurrency() {
        System.out.println(store.getCurrency().getName());
    }

    /**
     * Create a new currency and try to add it to the currencies list if there isn't already a currency with the same
     * symbol in it.
     *
     * @param name        the currency's name
     * @param symbol      the currency's symbol
     * @param parityToEur the currency's parity to EUR
     */
    protected void addCurrency(String name, String symbol, String parityToEur) {
        Currency newCurrency = store.createCurrency(name, symbol, Double.parseDouble(parityToEur));

        for (Currency currency : currencies) {
            if (currency.equals(newCurrency)) {
                System.out.println("The currency " + name + " already exists in the currencies list.");
                return;
            }
        }

        currencies.add(newCurrency);
    }

    /**
     * Print the currencies from the currencies list.
     */
    protected void listCurrencies() {
        currencies.sort(Currency.CurrencyNameComparator);
        for (Currency currency : currencies) {
            System.out.println(currency.getName() + " " + currency.getParityToEur());
        }
    }

    /**
     * Load the products from a CSV file to the Store.
     *
     * @param pathToCSVFile the path to the CSV file
     */
    protected void loadCSV(String pathToCSVFile) {
        ArrayList< Product > products = store.readCSV(pathToCSVFile, currencies);
    }

    /**
     * Save the products from the Store to a CSV file.
     *
     * @param pathToCSVFile the path to the CSV file
     */
    protected void saveCSV(String pathToCSVFile) {
        store.saveCSV(pathToCSVFile);
    }

    /**
     * Try to set the Store's currency to a new one (it can only be one that's already in the currencies list).
     *
     * @param currencyName the currency's name
     */
    protected void setStoreCurrency(String currencyName) {
        Currency newCurrency = null;

        for (Currency currency : currencies) {
            if (currency.getName().equals(currencyName)) {
                newCurrency = currency;
                break;
            }
        }

        try {
            store.changeCurrency(newCurrency);
        } catch (CurrencyNotFoundException exception) {
            System.out.println(exception.getMessage());
        }
    }

    /**
     * Update a currency's parity to EUR (only if it is already in the currencies list).
     *
     * @param currencyName   the currency's name
     * @param newParityToEur the currency's new parity to eur
     */
    protected void updateParity(String currencyName, String newParityToEur) {
        for (Currency currency : currencies) {
            if (currency.getName().equals(currencyName)) {
                currency.updateParity(Double.parseDouble(newParityToEur));
                return;
            }
        }

        System.out.println("The currency " + currencyName + " does not exist in the currencies list.");
    }

    /**
     * List the products from the Store.
     */
    protected void listProducts() {
        for (Product product : store.getProducts()) {
            printProductInfo(product);
        }
    }

    /**
     * Print info about a specified product (if it exists in Store).
     *
     * @param uniqueID the unique id
     */
    protected void showProduct(String uniqueID) {
        for (Product product : store.getProducts()) {
            if (product.getUniqueId().equals(uniqueID)) {
                printProductInfo(product);
                return;
            }
        }

        System.out.println("There is not any product with the ID " + uniqueID + " in the store.");
    }

    /**
     * Print info about a specified product.
     *
     * @param product the product
     */
    protected void printProductInfo(Product product) {
        DecimalFormat df = new DecimalFormat("###,##0.00");

        System.out.println(product.getUniqueId() + "," + product.getName() + "," +
                product.getManufacturer().getName() + "," + store.getCurrency().getSymbol() +
                df.format(product.getPrice()) + "," + product.getQuantity());
    }

    /**
     * Print the existing manufacturers in the Store.
     */
    protected void listManufacturers() {
        ArrayList< Manufacturer > manufacturers = store.getManufacturers();
        manufacturers.sort(Manufacturer.ManufacturerNameComparator);

        for (Manufacturer manufacturer : manufacturers) {
            System.out.println(manufacturer.getName());
        }
    }

    /**
     * Print the products from a specified manufacturer that exist in the Store.
     *
     * @param manufacturerName the manufacturer's name
     */
    protected void listProductsByManufacturer(String manufacturerName) {
        ArrayList< Manufacturer > manufacturers = store.getManufacturers();

        for (Manufacturer manufacturer : manufacturers) {
            if (manufacturer.getName().equals(manufacturerName)) {
                ArrayList< Product > products = store.getProductsByManufacturer(manufacturer);
                for (Product product : products) {
                    printProductInfo(product);
                }
                return;
            }
        }

        System.out.println("The manufacturer " + manufacturerName + " does not exist in the Store's manufacturers list");
    }

    /**
     * Add a discount to the Store.
     *
     * @param discountType  the discount's type
     * @param discountValue the discount's value
     * @param discountName  the discount's name
     */
    protected void addDiscount(String discountType, String discountValue, String discountName) {
        try {
            Discount discount = store.createDiscount(DiscountType.valueOf(discountType + "_DISCOUNT"), discountName,
                    Double.parseDouble(discountValue));
            discounts.add(discount);
        } catch (IllegalArgumentException exception) {
            System.out.println("The Discount is not available.");
        }
    }

    /**
     * Print the existing discounts.
     */
    protected void listDiscounts() {
        for (Discount discount : discounts) {
            System.out.println(discount);
        }
    }

    /**
     * Try to apply a discount to the products in the Store (only if it exists in the discounts list).
     *
     * @param discountType  the discount type
     * @param discountValue the discount value
     */
    protected void applyDiscount(String discountType, String discountValue) {
        Discount discountToApply = null;

        try {
            for (Discount discount : discounts) {
                if (discount.getValue() == Double.parseDouble(discountValue) &&
                        discount.getDiscountType().equals(DiscountType.valueOf(discountType + "_DISCOUNT"))) {
                    discountToApply = discount;
                    break;
                }
            }
        } catch (IllegalArgumentException exception) {
            System.out.println("Discount-ul introdus este incorect.");
        }

        store.discountAllProducts(discountToApply);
    }

    /**
     * Calculate total cost of an array of products specified by their IDs.
     * <p>
     * The total cost is calculated as the sum of all the products' prices multiplied by their stocks.
     *
     * @param productsIDs the products' IDs
     */
    protected void calculateTotal(String[] productsIDs) {
        ArrayList< Product > products = new ArrayList<>();
        for (String id : productsIDs) {
            boolean productExistsInStore = false;

            for (Product product : store.getProducts()) {
                if (product.getUniqueId().equals(id)) {
                    products.add(product);
                    productExistsInStore = true;
                    break;
                }
            }

            if (!productExistsInStore) {
                System.out.println("There is not any product with the " + id + " in the store.");
            }
        }

        double total = store.calculateTotal(products);
        DecimalFormat df = new DecimalFormat("###,##0.00");

        System.out.println(store.getCurrency().getSymbol() + df.format(total));
    }

    /**
     * Save the Store's current state to a binary file.
     *
     * @param pathToBinaryFile the path to the binary file
     */
    protected void saveStore(String pathToBinaryFile) {
        store.saveStore(pathToBinaryFile);
    }

    /**
     * Load an old state of the Store from a binary file.
     *
     * @param pathToBinaryFile the path to binary file
     */
    protected void loadStore(String pathToBinaryFile) {
        store.loadStore(pathToBinaryFile);
        store = Store.getInstance();
    }
}
