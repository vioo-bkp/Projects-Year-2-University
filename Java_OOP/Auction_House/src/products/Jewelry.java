package products;

// Clasa "Bijuterie", cu proprietatile material si un bool cu numele jewl
// pentru a trimite true / false vizavi de tipul de valoare a bijuteriei

public class Jewelry extends Product {
	private final String material;
	private final boolean jewl;

	public Jewelry(String material, boolean jewel) {
		this.material = material;
		this.jewl = jewel;
	}
}
