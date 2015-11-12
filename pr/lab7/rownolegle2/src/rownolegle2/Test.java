package rownolegle2;

import java.util.List;
import java.util.ArrayList;

public class Test {
    public static void main(String[] args) {
    	Czytelnia czytelnia = new Czytelnia();
   // 	Pisarz pisarz = new Pisarz(czytelnia);
  //  	czytelnia.kolejkaPisarzy.add(pisarz);
    	
//    	czytelnia.liczba_pisarzy=1;
    	
   // 	List <Pisarz> pisarzList=new ArrayList<Pisarz>();
    	
    	for(int i=0; i<30; i++){
    		(new Thread(new Pisarz(czytelnia))).start();
    		(new Thread(new Czytelnik(czytelnia))).start();
    	}
    	
    	try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	for(int i=0; i<50; i++){
    		
    		(new Thread(new Czytelnik(czytelnia))).start();
    	}
    		
  

    }
}
