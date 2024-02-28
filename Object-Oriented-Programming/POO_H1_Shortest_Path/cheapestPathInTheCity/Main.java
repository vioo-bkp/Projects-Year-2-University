package cheapestPathInTheCity;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.NoSuchElementException;
import java.util.Scanner;

/* 
    Mateescu Viorel-Cristian, 322CB
*/

/**
 * The type Main.
 * <p>
 * Main class of this Travelling Service Project.
 */
public class Main {
    /**
     * The entry point of the application.
     * The map is created according to the input file, traffic jams may be added to it, vehicles may be driven on the
     * map and finds whether there is a path for the vehicle to get from one specified point to another one while
     * computing the travelling cost.
     *
     * @param args the input arguments
     * @throws IOException the io exception
     */
    public static void main(String[] args) throws IOException {
        Scanner scanner;
        FileWriter output;
        /* Checks if the input file exists. */
        try {
            scanner = new Scanner(new File("./map.in"));
        } catch (IOException exception) {
            System.out.println(exception.getMessage());
            return;
        }

        /* Checks if the output file exists. */
        try {
            output = new FileWriter("./map.out");
        } catch (IOException exception) {
            System.out.println(exception.getMessage());
            return;
        }

        /* Checks if the input file is empty or not. */
        String line;
        try {
            line = scanner.nextLine();
        } catch (NoSuchElementException exception) {
            System.out.println(exception.getMessage());
            return;
        }

        /* Computes the Map's number of nodes and streets. */
        String[] mapData = line.split(" ");
        int noNodes = Integer.parseInt(mapData[1]), noStreets = Integer.parseInt(mapData[0]);

        /* Creates a new Map according to the input file. */
        Map map = new Map(noNodes);
        for (int i = 0; i < noStreets; i++) {
            line = scanner.nextLine();
            mapData = line.split(" ");

            /* Gets values for the streets from the input file and add them to the map. */
            int start = Integer.parseInt(mapData[0].substring(1)), end = Integer.parseInt(mapData[1].substring(1));
            int cost = Integer.parseInt(mapData[2]), size = Integer.parseInt(mapData[3]);

            map.addStreet(start, end, cost, size);
        }

        while (scanner.hasNextLine()) {
            line = scanner.nextLine();

            /* Checks if a vehicle is going to be driven from a place to another one. */
            mapData = line.split(" ");
            if (mapData[0].equals("drive")) {
                /* Gets start point and end point of the path. */
                int start = Integer.parseInt(mapData[2].substring(1));
                int end = Integer.parseInt(mapData[3].substring(1));

                /* Creates the vehicle that is used for travelling. */
                Vehicle vehicle = switch (mapData[1]) {
                    case "b" -> new Bicycle();
                    case "m" -> new Motorcycle();
                    case "a" -> new Car();
                    default -> new Truck();
                };

                /* Tries to drive from start to end with the vehicle.
                   Computes the path and the cost and prints them. */
                String path;
                path = map.drive(vehicle, start, end);
                output.write(path + System.lineSeparator());
            }
            /* Checks if a Traffic jam is going to be added to a street. */
            else if (mapData[0].equals("accident") || mapData[0].equals("blocaj") || mapData[0].equals("trafic")) {
                /* Gets start point and end point of the Traffic jam. */
                int start = Integer.parseInt(mapData[1].substring(1)), end = Integer.parseInt(mapData[2].substring(1));
                int cost = Integer.parseInt(mapData[3]);

                /* Adds the Traffic jam to the map. */
                map.AddRestriction(mapData[0], start, end, cost);
            }
        }

        /* Tries to close the output file. */
        try {
            output.close();
        } catch (IOException exception) {
            System.out.println(exception.getMessage());
        }
    }
}
