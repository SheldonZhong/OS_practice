import java.net.*;
import java.io.*;

class Reply implements Runnable {
	private Socket client;

	public Reply(Socket client) {
		this.client = client;
	}

	public void run() {
		try {
            String line;
			PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
            InputStream is = client.getInputStream();
            BufferedReader cin = new BufferedReader(new InputStreamReader(is));

            while ((line = cin.readLine()) != null) {
                pout.println("server:" + line);
                pout.flush();
                System.out.println(line);
            }
            client.close();
		} catch (Exception e) {
			// TODO: handle exception
		}

	}
}

public class DateServer {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6017);

            while (true) {
                Socket client = sock.accept();

                Thread thrd = new Thread(new Reply(client));
				thrd.start();
            }
        }
        catch (IOException ioe) {
            System.err.println(ioe);
        }
    }
}