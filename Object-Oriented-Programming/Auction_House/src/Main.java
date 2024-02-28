import products.Colors;
import products.Product;
import products.ProductFactory;

import java.util.ArrayList;
import java.util.List;

/**
 * Clasa principala unde construim niste produse si incercam testarea aplicatiei
 */
public class Main {

	// Genericitate / Polimorfism
	public static void main(String[] args) {
		AuctionHouse auctionHouse = AuctionHouse.getInstance();
		List<Product> products = new ArrayList<>();
		products.add(ProductFactory.createFurniture("dulap","lemn" )
				.withName("canapea mica")
				.withId(1)
				.withMinPrice(3.0)
				.withSellingPrice(23.0)
				.build());
		products.add(ProductFactory.createJewlery("fier",true )
				.withName("canapea mare")
				.withId(2)
				.withMinPrice(3.0)
				.withSellingPrice(23.0)
				.build());
		products.add(ProductFactory.createFurniture("canapea","lemn" )
				.withName("canapea mare")
				.withId(3)
				.withMinPrice(3.0)
				.withSellingPrice(23.0)
				.build());
		products.add(ProductFactory.createPainting("Painter1", Colors.ACRILIC )
				.withName("canapea mare")
				.withId(4)
				.withMinPrice(3.0)
				.withSellingPrice(23.0)
				.build());
		products.add(ProductFactory.createFurniture("dulap","lemn" )
				.withName("canapea mare")
				.withId(5)
				.withMinPrice(3.0)
				.withSellingPrice(23.0)
				.build());


	}


}
