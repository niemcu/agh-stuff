package czytelnia_java;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.locks.*;

public class ReadingRoom {
	
	int readersNum = 0;
	int writersNum = 0;
	Condition readers, writers;
	ReentrantLock locker;
	Queue<Reader> readersQueue;
	Queue<Writer> writersQueue;

	public ReadingRoom() {
		locker = new ReentrantLock();
		readers = locker.newCondition();
		writers = locker.newCondition();
		readersQueue = new LinkedList<Reader>();
		writersQueue = new LinkedList<Writer>();
	}
	
	public void startReading(Reader re) {
		readersQueue.add(re);
		if (readersNum > 0 || /*locker.hasWaiters(writers))*/ !writersQueue.isEmpty()) {
			try {
				locker.lock();
				readers.await();
				locker.unlock();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		readersNum++;
		//readers.signal();
	}
	
	public void stopReading() {
		locker.lock();
		readersNum--;
		readers.signalAll();
		readersQueue.remove();
		if (readersQueue.isEmpty()) {
			writers.signal();
		}
		locker.unlock();
	}
	
	public void startWriting() {
		if (readersNum + writersNum > 0) {
			try {
				writers.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		writersNum++;
	}
	
	public void stopWriting() {
		writersNum--;
		if (locker.hasWaiters(readers)) {
			readers.signalAll();
		} else {
			writers.signal();
		}
	}

}
