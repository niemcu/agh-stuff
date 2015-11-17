package czyt_pis_java;

import java.util.Random;

public class Czytelnik implements Runnable {

	Czytelnia czytelnia;
	int nr;
	
	Czytelnik(Czytelnia czytelnia, int nr){
		this.czytelnia = czytelnia;
		this.nr = nr;
	}

    public void run() {
    	czytelnia.chceCzytac(this);
    	czytam();
    	czytelnia.koniecCzytania(this);
    }

	private void czytam() {
    	Random random = new Random();
		System.out.println("czytelnik" + this.nr + ": czytam");
		try {
			Thread.sleep(random.nextInt(1000));
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
                