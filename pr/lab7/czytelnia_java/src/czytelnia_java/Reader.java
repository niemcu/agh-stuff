package czytelnia_java;
import java.util.Random;

public class Reader implements Runnable {

	private ReadingRoom room;
	
	public Reader(ReadingRoom room) {
		this.room = room;
	}

	@Override
	public void run() {
		System.out.println("nrwatku: " + Thread.currentThread().getId() + ": chcę czytać");
        room.startReading(this);
        try {
            System.out.println("nrwatku: " + Thread.currentThread().getId() + ": czytam");
			Thread.sleep(6000);
		} catch (InterruptedException e) {}
        room.stopReading();
        System.out.println("nrwatku: " + Thread.currentThread().getId() + ": skończyłem czytać");
	}

}   