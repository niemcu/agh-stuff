import java.util.Random;

public class Producent implements Runnable {
    private Pojemnik pojemnik;

    public Producent(Pojemnik pojemnik) {
        this.pojemnik = pojemnik;
    }

    public void run() {
        String Info[] = {
            "Komunikat_1",
            "Komunikat_2",
            "Komunikat_3",
            "Komunikat_4"
        };
        Random random = new Random();

        for (int i = 0; i < Info.length; i++) {
            pojemnik.wloz(Info[i]);
            try {
                Thread.sleep(random.nextInt(1000));
            } catch (InterruptedException e) {}
        }
        pojemnik.wloz("KONIEC");
    }
}

