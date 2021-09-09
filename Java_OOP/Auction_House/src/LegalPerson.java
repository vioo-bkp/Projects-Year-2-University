
// Clasa Persoanei Juridice, ce extinde tipul de Client

public class LegalPerson extends Client{

	private final Company company;
	private final double socialCapital;

	// Metoda specifica tipului de client, cu mentiunea ca persoana juridica prezinta tipul de companie si capitalul aferent acesteia
	public LegalPerson(int id, String name, String adress, int attendingsCount, int wonAuctions, Company company, double socialCapital) {
		super(id, name, adress, attendingsCount, wonAuctions);
		this.company = company;
		this.socialCapital = socialCapital;
	}
}
