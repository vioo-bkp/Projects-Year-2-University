package products;

import java.awt.*;

// Este definita clasa "Pictor", in care sunt definite numele pictorului si tipul de culoare

public class Painting extends Product {
	private final String painterName;
	private final Colors color;


	public Painting(String painterName, Colors color) {
		this.painterName = painterName;
		this.color = color;
	}
}
