
/**
 * Time-of-day server listening to port 6013.
 *
 * Figure 3.21
 *
 * @author Silberschatz, Gagne, and Galvin. 
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013.
 */

// Modify the socket-based date server (Figure 3.21) in Chapter 3 so that
// the server services each client request in a separate thread.

import java.net.*;
import java.io.*;

class Reply implements Runnable {
	private Socket client;

	public Reply(Socket client) {
		this.client = client;
	}

	public void run() {
		try {
			System.out.println("Thread started");
			PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
			// write the Date to the socket
			pout.println(new java.util.Date().toString());

			// close the socket and resume listening for more connections
			client.close();
} catch (Exception e) {
			// TODO: handle exception
		}

	}
}

public class DateServer {
	public static void main(String[] args) {
		try {
			ServerSocket sock = new ServerSocket(6013);

			// now listen for connections
			while (true) {
				Socket client = sock.accept();
				// we have a connection
				Thread thrd = new Thread(new Reply(client));
				thrd.start();
			}
		} catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
