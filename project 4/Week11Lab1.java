// Sudarshan Tiwari

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Week11Lab1 {
    public static void main(String[] args) {
        String fileName = "numbers.txt";
        
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            String line;
            
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            
        } catch (IOException e) {
            System.out.println("Error reading the file: " + e.getMessage());
        }
    }
}