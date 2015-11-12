import java.util.Random;

public class Konsument implements Runnable {
    private Pojemnik pojemnik;

    public Konsument(Pojemnik pojemnik) {
        this.pojemnik = pojemnik;
    }

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
                

