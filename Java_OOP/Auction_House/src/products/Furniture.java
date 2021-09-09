package products;

// Clasa "Mobila", cu proprietatile tipului si materialului din care este confectionata

public class Furniture extends Product {

	private final String type;
	private final String material;

	public Furniture(String type, String material) {
		this.type = type;
		this.material = material;
	}
}
