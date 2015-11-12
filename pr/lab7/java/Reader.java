import java.util.Random;

public class Reader implements Runnable {

	private ReadingRoom room;
	
	public Reader(ReadingRoom room) {
		this.room = room;
	}

	@Override
	public void run() {
        Random random = new Random();
        for (String message = pojemnik.wyjmij(); ! message.equals("KONIEC");
                message = pojemnik.wyjmij()) {
            System.out.format("OTRZYMANY KOMUNIKAT: %s%n", message);
            try {
                Thread.sleep(random.nextInt(1000));
            } catch (InterruptedException e) {}
        }

	}

}   

