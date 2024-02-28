package products;

/**
 * Factory methods - construim un obiect cu metode nu cu constructor explicit
 */
public class ProductFactory {

	public static ProductBuilder createPainting(String painterName, Colors color) {
		return new ProductBuilder(new Painting(painterName, color));
	}

	public static ProductBuilder createJewlery(String material, Boolean bool) {
		return new ProductBuilder(new Jewelry(material, bool));
	}

	public static ProductBuilder createFurniture(String type, String material) {
		return new ProductBuilder(new Furniture(type, material));
	}
}
