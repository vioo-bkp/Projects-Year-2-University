
// Clasa speciala Company de tip enum, in care este definita tipul de companie pentru persoana juridica

public enum Company {

	SRL("SRL"),
	SA("SA");

	private final String company;

	Company(String company) {
		this.company = company;
	}
}
