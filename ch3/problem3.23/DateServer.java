/**
 * This should be the same with 3.24, since we only need to change 
 * the content on the server side and config the right port numbers.
 */
import java.net.*;
import java.io.*;

public class DateServer {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6017);

            while (true) {
                Socket client = sock.accept();

                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);

                // pout.println(new java.util.Date().toString());
                pout.println("Quote of the day!");

                client.close();
            }
        }
        catch (IOException ioe) {
            System.err.println(ioe);
        }
    }
}