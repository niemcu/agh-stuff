package czytelnia_java;

public class TestReadingRoom {

	public TestReadingRoom() {
		// TODO Auto-generated constructor stub
	}

	public static void main(String[] args) {
		ReadingRoom rr = new ReadingRoom();
        (new Thread(new Reader(rr))).start();
        (new Thread(new Reader(rr))).start();
        (new Thread(new Reader(rr))).start();
        (new Thread(new Reader(rr))).start();
        //(new Thread(new Writer(rr))).start();
	}

}
