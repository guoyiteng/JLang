//
//import sun.security.krb5.internal.crypto.EType;
//import sun.text.normalizer.ReplaceableString;
//
//import java.util.ArrayList;
//import java.lang.RuntimeException;
//import java.util.Hashtable;
//
//public class Sync {
//    public static void main(String[] args) {
////        Integer o = new Integer(1);
//
//        Object o1 = new Object();
////        synchronized (o) {
////            synchronized (o1) {
////        foobar();
//        Sync sync = new Sync();
//
////        synchronized (o1) {
////            int i = 1;
////        }
//
//        try {
////            synchronized (o1) {
////                throw new NullPointerException();
//                sync.foo();
////            }
//        } catch (RuntimeException e) {
//            System.out.println("Good");
//        }
//
////        synchronized (o) {
////
////        }
//        // Not sure if JLang handles scope correctly.
////        Sync s = new Sync();
////        s.foo();
////        case0();
////        case1();
////        case2();
////        case3();
////        case4();
////        case5();
//    }
//
//    public synchronized void foo() {
////        synchronized (this) {
//            int i = 2;
//            throw new NullPointerException();
////        }
//    }
//
////    public synchronized native void bar();
//
//    public synchronized static void foobar() {
//        int i = 1;
//    }
//
//    static class Sync2 extends Sync {
//        Sync2 cause;
//
//        public synchronized Sync2 foo2() {
//            return (cause==this ? null : cause);
//        }
//
//        public static synchronized boolean bar2() {
//            boolean b = true;
//            return b;
//        }
//    }
//
////    public static void case0() {
////        Object o1 = new Object();
////        Object o2 = new Object();
////        if (true) {
////            synchronized (o1) {
////                return;
////            }
////        } else {
////            synchronized (o1) {
////                synchronized (o2) {
////                    return;
////                }
////            }
////        }
////    }
////
////    // solved
////    public static void case1() {
////        Object o = new Object();
////        synchronized(o) {
////            return;
////        }
////    }
////
////    // solved
////    public static void case2() {
////        Object o = new Object();
////        while (true) {
////            synchronized(o) {
////                break; // or continue
////            }
////        }
////
////        // Need to know how many nesting levels we have.
////        while (true) {
////            while (true) {
////                synchronized(o) {
////                    break;
////                }
////            }
////            break;
////        }
////    }
////
////    // Distinguish case3 and case4.
////     public static void case3() {
////         Object o = new Object();
////         synchronized(o) {
////             try {
////                 System.out.println(1);
////                 return;
////             } finally {
////                 System.out.println(2);
////             }
////         // Don't know how many levels needed to be popped here.
////
////         }
////     }
////     public static void case4() {
////         Object o = new Object();
////         try {
////             synchronized(o) {
////                 System.out.println(1);
////                 return;
////             }
////         } finally {
////             System.out.println(2);
////         }
////     }
////
////     public static void case5() {
////         Object o1 = new Object();
////         Object o2 = new Object();
////         Object o3 = new Object();
////
////         try {
////             if (true) {
////                 synchronized(o1) {
////                     throw new RuntimeException();
////                 }
////             } else {
////                 synchronized(o2) {
////                     synchronized(o3) {
////                         throw new RuntimeException();
////                     }
////                 }
////             }
////         } catch(RuntimeException e) {
////             e.printStackTrace();
////         }
////     }
//}