package StoreApp;

import java.util.Scanner;

/**
 * The type Store owner.
 * <p>
 * Class that describes the behaviour of a Store Owner. It interacts with the store via a facade that serves as a
 * control panel for the Store, the existing currencies and discounts.
 */
public class StoreOwner {
    /**
     * The entry point of application.
     *
     * @param args the input arguments
     */
    public static void main(String[] args) {
        // Open the control panel.
        StoreServiceFacade storeOwnerInterface = new StoreServiceFacade();
        Scanner scanner = new Scanner(System.in);

        // Give instructions that should be executed on the Store.
        while (scanner.hasNextLine()) {
            String command = scanner.nextLine();
            storeOwnerInterface.controlPanel(command);
        }
    }
}
