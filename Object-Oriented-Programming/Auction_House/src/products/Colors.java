package products;

// Clasa speciala Colors de tip enum, in care este definita tipurile de culori pentru Tablou

public enum Colors {
	OIL("OIL"),
	TEMPERA("TEMPERA"),
	ACRILIC("ACRILIC");

	private final String color;

	private Colors(String color){
		this.color = color;
	}

	public String getColor() {
		return color;
	}
}
