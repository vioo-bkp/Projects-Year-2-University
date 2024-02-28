import java.util.ArrayList;
import java.util.List;

/**
 * licitatia propiu zisa unde se intampla lupta pentru produsul dorit de participanti (brokeri)
 */
public class Auction implements Runnable {
	private final int id;
	private int noPart;
	private final int idProduct;
	private final int maxSteps;
	private int step = 0;
	List<Broker> participants;

	public Auction(int id, int noPart, int idProduct, int maxSteps) {
		this.id = id;
		this.noPart = noPart;
		this.idProduct = idProduct;
		this.maxSteps = maxSteps;
		this.participants = new ArrayList<>();
	}

	public void addParticipant(Broker broker) {
		participants.add(broker);
		if (participants.size() == noPart) {
			AuctionHouse.startAuction(this.id);
		}
	}
	public int getId() {
		return id;
	}

	public int getNoPart() {
		return noPart;
	}

	public int getIdProduct() {
		return idProduct;
	}

	public int getMaxSteps() {
		return maxSteps;
	}

	@Override
	public void run() {
		// implementare de circulare a licitatiei
		// daca un broker a castigat ii punem la statusul result true;
		// altfel ii punem false
	}
}
