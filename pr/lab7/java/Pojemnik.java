public class Pojemnik {

    //Komunikat przekazywany poprzez pojemnik od Producenta do Konsumenta.
    private String komunikat;

    private boolean pusty = true; // Czy pojemnik pusty?

    public synchronized String wyjmij() {
        // Jesli pusty - czekaj
        while (pusty) {
            try {
                wait();
            } catch (InterruptedException e) {}
        }
        // Po pobraniu poinformuj, ze pusty.
        pusty = true;
        // Obudz producenta, jesli czekal na zabranie komunikatu.
        notifyAll();
        return komunikat;
    }

    public synchronized void wloz(String komunikat) {
        //Jesli pelny - czekaj.
        while (!pusty) {
            try { 
                wait();
            } catch (InterruptedException e) {}
        }
        // Po umieszczeniu poinformuj, ze pelny.
        pusty = false;
        // Zapisz w lokalnej zmiennej.
        this.komunikat = komunikat;
        // Obudz konsumenta, jesli czekal na zabranie komunikatu.
        notifyAll();
    }
}
