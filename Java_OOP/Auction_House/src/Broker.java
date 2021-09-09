import products.Product;

/**
 * Clasa Broker ce mediaza interactiunea dintre Casa de Licitatii si Client
 */
public class Broker implements Employee{

	private int wantedProductId;
	private Client client;
	private boolean result;

	public void request(int id) {
		this.wantedProductId = id;
		AuctionHouse.getInstance().request(this);
	}

	// Notificarea clientului pentru o licitatie castigata sau pierduta
	public void notifyClient(Product product, boolean won) {
		if (won) {
			System.out.println(client.getName() +  " "  + "WON");
		}
		else {
			System.out.println(client.getName() +  " "  + "DID LOST");
		}
	}

	public void setClient(Client client) {
		this.client = client;
	}
	public int getProductId() {
		return this.wantedProductId;
	}

	public boolean getResult() {
		return result;
	}
}
