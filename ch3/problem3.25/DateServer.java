

import java.net.*;
import java.io.*;

public class DateServer {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6017);
            String line;
            while (true) {
                Socket client = sock.accept();

                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                InputStream is = client.getInputStream();
                BufferedReader cin = new BufferedReader(new InputStreamReader(is));

                while ((line = cin.readLine()) != null) {
                    pout.println("server:" + line);
                    pout.flush();
                    System.out.println(line);
                }
                client.close();
            }
        }
        catch (IOException ioe) {
            System.err.println(ioe);
        }
    }
}