import products.Product;

import java.util.List;
import java.util.Random;

/**
 * Casa de licitatii propiu zisa, un singletone ce se ocupa de managerierea intregei operatiuni
 */
public class AuctionHouse {
	private static AuctionHouse INSTANCE = null;
	private static List<Product> products;
	private static List<Client> clients;
	private static List<Auction> activeAuctions;
	private static Admin admin;
	private static List<Broker> brokers;

	// Singletone
	public static AuctionHouse getInstance() {
		if (INSTANCE == null) {
			INSTANCE = new AuctionHouse();
		}
		return INSTANCE;
	}

	// Multithreading
	public static void startAuction(int id) {
		for (var auction : activeAuctions) {
			if (auction.getId() == id) {
				Thread thread = new Thread(auction);
				thread.start();
			}
		}
	}

	// Observer design pattern
	public void notifyBrokers(List<Broker> brokers, Product product) {
		for (Broker broker : brokers) {
			broker.notifyClient(product, broker.getResult());
		}
	}

	public void request(Broker broker) {
		// find the product in products and if I find it add new participant (broker)
		for (var product : products) {
			if (product.getId() == broker.getProductId()) {
				for (var auction : activeAuctions) {
					if (auction.getId() == product.getAuctionId()) {
						auction.addParticipant(broker);
					}
				}
			}
		}
	}

	public Broker getRandomBroker() {
		return brokers.remove(new Random().nextInt(brokers.size()));
	}
}
