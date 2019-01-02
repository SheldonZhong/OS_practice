import java.net.*;
import java.io.*;
import java.util.Scanner;

public class DateClient {
    public static void main (String[] args) {
        try {
            Socket sock = new Socket("127.0.0.1", 6017);

            InputStream in = sock.getInputStream();
            BufferedReader bin = new BufferedReader(new InputStreamReader(in));
            BufferedReader cin = new BufferedReader(new InputStreamReader(System.in));
            PrintWriter pout = new PrintWriter(sock.getOutputStream(), true);
            
            String line;
            while ((line = cin.readLine()) != null) {
                pout.println(line);
                pout.flush();

                // receive only once
                if ((line = bin.readLine()) != null)
                    System.out.println(line);
                if(line.toLowerCase().contains("bye")) break;
            }
            sock.close();

        } catch (IOException ioe) {
            System.err.println(ioe);
        }
    }
}
