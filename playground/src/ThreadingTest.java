import java.util.LinkedList;
import java.util.List;

public class ThreadingTest {

    public static int i = 0;
    public static final Object lock = new Object();

    public synchronized static int nextI() {
        return i++;
    }

    public static class RunA extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 10; i++)
//            synchronized (lock) {
      System.out.println ("Thread " + 
            Thread.currentThread().getId() + 
            " is running");
//            }
        }
    }

    public static class RunB extends Thread {
        @Override
        public void run() {
      System.out.println ("Thread " + 
            Thread.currentThread().getId() + 
            " is running"); 
        }
    }

    public static void main(String[] args) throws Exception {
        System.out.println("hello");
        for(int i = 0; i < 10; i++) {
            RunA runA = new RunA();
            runA.start();
        }
    }
}