package czyt_pis_java;

import java.util.Random;

public class Pisarz implements Runnable {
  
	Czytelnia czytelnia;
	int nr;

    public Pisarz(Czytelnia czytelnia, int nr) {
       this.czytelnia=czytelnia;
       this.nr = nr;
    }

    public void run() {
    	czytelnia.chcePisac(this);    	
    	this.pisze();
    	czytelnia.koniecPisania(this);
    }
    
    void pisze(){
    	Random random = new Random();
		System.out.println("pisarz" + this.nr + ": pisze");
      	try {
			Thread.sleep(random.nextInt(1000));
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}
