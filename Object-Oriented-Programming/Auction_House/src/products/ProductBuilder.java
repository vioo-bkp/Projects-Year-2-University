package products;

// Clasa Builder pentru crearea obiectelor complexe (cu multe atribute)

public class ProductBuilder {
	public final Product product;

	public ProductBuilder(Product product) {
		this.product = product;
	}

	public ProductBuilder withId(int id) {
		product.setId(id);
		return this;
	}

	public ProductBuilder withSellingPrice(double sellingPrice) {
		product.setSellingPrice(sellingPrice);
		return this;
	}

	public ProductBuilder withMinPrice(double minPrice) {
		product.setMinPrice(minPrice);
		return this;
	}

	public ProductBuilder withName(String name) {
		product.setName(name);
		return this;
	}

	public Product build() {
		return product;
	}
}
