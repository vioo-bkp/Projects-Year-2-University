/**
 * Entitatea Parinte de tipul Client, ce va solicita produsele pe care le doreste de la casa de licitatii
 */

// Definirea proprietatilor pe care le detine Clientul
public class Client {
	private final int id;
	private final String name;
	private final String adress;
	private final int attendingsCount;
	private final int wonAuctions;
	private Broker allocatedBroker;

	// Metoda Client ce detine caracteristicile persoanei Fizice
	public Client(int id, String name, String adress, int attendingsCount, int wonAuctions) {
		this.id = id;
		this.name = name;
		this.adress = adress;
		this.attendingsCount = attendingsCount;
		this.wonAuctions = wonAuctions;
	}
	// Metoda folosita pentru tipul de obiect pe care il doreste Clientul
	public void askForProduct(int id) {
		allocatedBroker.request(id);
	}

	// Clientii care vor sa liciteze pentru un produs solicita specificand id-ul produsului dorit.
	// Casa de licitati asociaza in mod aleatoriu un Broker fiecarui Client, iar Brokerul intermediază licitatia ulterior
	public void askForBroker() {
		allocatedBroker = AuctionHouse.getInstance().getRandomBroker();
		allocatedBroker.setClient(this);
	}
	// Informatiile folosite de catre Client pentru produsul dorit
	public int getId() {
		return id;
	}

	public String getName() {
		return name;
	}

	public String getAdress() {
		return adress;
	}

	public int getAttendingsCount() {
		return attendingsCount;
	}

	public int getWonAuctions() {
		return wonAuctions;
	}
}
