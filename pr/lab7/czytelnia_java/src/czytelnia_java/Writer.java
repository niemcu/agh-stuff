package czytelnia_java;
import java.util.Random;

public class Writer implements Runnable {

	private ReadingRoom room;
	
	public Writer(ReadingRoom room) {
		this.room = room;
	}

	@Override
	public void run() {
		System.out.println("nrwatku: " + Thread.currentThread().getId() + ": chcę pisać");
        room.startWriting();
        System.out.println("nrwatku: " + Thread.currentThread().getId() + ": piszę");
        try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {}
        room.stopWriting();
        System.out.println("nrwatku: " + Thread.currentThread().getId() + ": skończyłem pisać");
	}

}   