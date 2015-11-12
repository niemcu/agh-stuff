package rownolegle2;
import java.util.Random;

public class Czytelnik implements Runnable {

	Czytelnia czytelnia;
	
	Czytelnik(Czytelnia czytelnia){
		this.czytelnia = czytelnia;
	}
	

    public void run() {
    	
    	czytelnia.chceCzytac(this);
    	
    	czytam();
    	
    	czytelnia.koniecCzytania();
    	
    	
    }


	private void czytam() {
		System.out.println("CZYTAM CIPOOO");
		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
                
