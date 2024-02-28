package products;

// Clasa parinte Produs, din care sunt extinse celelalte clase
// Definirea tuturor informatiilor despre produs
public class Product {
	private int id;
	private String name;
	private double sellingPrice;
	private double minPrice;
	private int year;
	// o variabila intermediara, ce verifica daca un produs se afla deja intr-o licitatie
	private int auctionId;

	// Definirea proprietatilor (nume, pret, pret minim, an) pe care le are produsul pentru vanzarea acestuia

	public Product(int id, String nume, double sellingPrice, double minPrice, int an) {
		this.id = id;
		this.name = nume;
		this.sellingPrice = sellingPrice;
		this.minPrice = minPrice;
		this.year = an;
	}
	// Declararea de Setter si Getter pentru fiecare cod de identificare a produsului
	public Product() {

	}
	// Un Client trimite o solicitare Casei de licitatie pentru un anumit produs, identificat prin id
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	// Un Client trimite o solicitare Casei de licitatie pentru un anumit produs, identificat prin nume
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	// Un Client trimite o solicitare Casei de licitatie pentru un anumit produs, identificat prin pretul propus
	public double getSellingPrice() {
		return sellingPrice;
	}

	public void setSellingPrice(double sellingPrice) {
		this.sellingPrice = sellingPrice;
	}

	// Un Client trimite o solicitare Casei de licitatie pentru un anumit produs, identificat prin pretul minim propus
	public double getMinPrice() {
		return minPrice;
	}

	public void setMinPrice(double minPrice) {
		this.minPrice = minPrice;
	}

	// Un Client trimite o solicitare Casei de licitatie pentru un anumit produs, identificat prin anul produsului
	public int getYear() {
		return year;
	}

	public void setYear(int year) {
		this.year = year;
	}

	// Verific daca produsul aferent ID-ului propus de Client este deja intr-o tranzactie / licitare
	public int getAuctionId() {
		return auctionId;
	}

	public void setAuctionId(int auctionId) {
		this.auctionId = auctionId;
	}
}
