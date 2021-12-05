package cheapestPathInTheCity;

/**
 * The interface Queue.
 * <p>
 * An interface that defines the basic operations on any type of Queue: insertion({@linkplain #add}),
 * extraction({@linkplain #remove} and {@linkplain #poll}) and inspection operations({@linkplain #isEmpty},
 * {@linkplain #peek} and {@linkplain #size}).
 *
 * @param <E> the type parameter for the elements in the queue
 */
public interface Queue< E > {
    /**
     * Returns an integer value, the queue's size(number of elements).
     *
     * @return the number of elements in the queue
     */
    int size();

    /**
     * Checks if the queue is empty or not.
     *
     * @return whether the queue is empty(true) or not(false)
     */
    boolean isEmpty();

    /**
     * Adds a new element to the queue.
     *
     * @param newElement the element to be added in the queue
     */
    void add(E newElement);

    /**
     * Remove an element from the queue.
     *
     * @param elem the element to be removed from the queue
     */
    void remove(E elem);

    /**
     * Peek the first element without removing it from the queue.
     *
     * @return the first element from the queue
     */
    E peek();

    /**
     * Poll the first element from the queue({@linkplain #peek} + {@linkplain #remove}).
     *
     * @return the first element from the queue
     */
    E poll();

}
