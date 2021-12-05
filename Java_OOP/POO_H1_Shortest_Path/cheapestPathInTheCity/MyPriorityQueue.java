package cheapestPathInTheCity;

import java.util.ArrayList;

/**
 * The type My priority queue.
 * <p>
 * As its name suggests, this is my own implementation of a generic priority queue.
 * <p>
 * Because of its implementation it's a "Min" Priority Queue: the element with the lowest priority will be the
 * first one in the queue.
 * <p>
 * It implements the Interface {@linkplain Queue} that defines the basic operations that can be done on any type of
 * Queue: {@linkplain Queue#size}, {@linkplain Queue#isEmpty}, {@linkplain Queue#add}, {@linkplain Queue#remove},
 * {@linkplain Queue#peek} and {@linkplain Queue#poll}.
 *
 * @param <E> the type parameter for the elements in the priority queue
 */
class MyPriorityQueue< E extends Comparable< E > > implements Queue< E > {
    /**
     * ArrayList of generic elements to store the Priority Queue's elements.
     */
    private ArrayList< E > elements;

    /**
     * No-argument constructor.
     * <p>
     * Instantiates a new My priority queue.
     * <p>
     * Sets the arraylist of {@linkplain #elements} to be empty.
     */
    public MyPriorityQueue() {
        this.elements = new ArrayList<>();
    }

    /**
     * Gets the priority queue's elements.
     *
     * @return the priority queue's elements
     */
    ArrayList< E > getElements() {
        return elements;
    }

    /**
     * Sets the priority queue's elements.
     *
     * @param elements the elements for the priority queue
     */
    void setElements(ArrayList< E > elements) {
        this.elements = elements;
    }

    /**
     * Returns an integer value, the priority queue's size(number of elements).
     *
     * @return the number of elements in the priority queue
     */
    public int size() {
        return this.elements.size();
    }

    /**
     * Checks if the priority queue is empty or not.
     *
     * @return whether the priority queue is empty(true) or not(false)
     */
    public boolean isEmpty() {
        return this.size() == 0;
    }

    /**
     * Adds a new element to the priority queue.
     * <p>
     * The element is inserted to its corresponding position after comparing its priority with the priorities of the
     * elements that are already in the queue.
     *
     * @param newElement the element to be added in the priority queue
     */
    public void add(E newElement) {
        // If the queue is empty the element is inserted right away.
        if (this.isEmpty()) {
            this.elements.add(newElement);
            return;
        }

        /* Else we have to find its place by comparing its priority with the priorities of the elements that are already
          in the queue.
          Steps:
            - Find an element with a higher priority than the one we're trying to insert.
            - Shift all the elements from that position to the end of the queue with one place to right.
            - Insert the new element at the position we found earlier.
          */
        for (int i = 0; i < this.elements.size(); i++) {
            E element = this.elements.get(i);
            if (newElement.compareTo(element) < 0) {
                this.elements.add(i, newElement);
                return;
            }
        }

        /* If this line is reached it means that the element we're trying to insert has the highest priority and it
        should be at the end of the priority queue.
         */
        this.elements.add(newElement);
    }

    /**
     * Remove a specific element from the priority queue.
     *
     * @param elem the element to be removed
     */
    public void remove(E elem) {
        this.elements.remove(elem);
    }

    /**
     * Peek the first element without removing it from the priority queue(the element with the lowest priority).
     *
     * @return the first element from the priority queue
     */
    public E peek() {
        return this.elements.get(0);
    }

    /**
     * Poll the first element from the priority queue(the element with the lowest priority).
     * <p>
     * poll = {@linkplain #peek} + {@linkplain #remove}
     *
     * @return the first element from the priority queue
     */
    public E poll() {
        E elem = this.peek();
        this.remove(elem);
        return elem;
    }

}