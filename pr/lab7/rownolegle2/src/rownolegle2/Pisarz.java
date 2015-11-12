package rownolegle2;
import java.util.Random;

import javax.swing.plaf.SliderUI;

public class Pisarz implements Runnable {
  
	Czytelnia czytelnia;

    public Pisarz(Czytelnia czytelnia) {
       this.czytelnia=czytelnia;
    }

    public void run() {
    	czytelnia.chcePisac(this);    	
    	this.pisze();
    	czytelnia.koniecPisania();
    }
    
    void pisze(){
    	System.out.println("PISZE HUJU");
      	try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}
