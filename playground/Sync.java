//
//import java.util.ArrayList;
//import java.lang.RuntimeException;
//
//public class Sync {
//    public static void main(String[] args) {
//        // case1();
//        // case2();
//        // case3();
//        // case4();
//        // case5();
//    }
//
//    public static void case0() {
//        Object o = new Object();
//        Object
//    }
//
//    // solved
//    public static void case1() {
//        Object o = new Object();
//        synchronized(o) {
//            return;
//        }
//    }
//
//    // temporarily sovled
//    // Currently, the Synchronized frame is in util. We should move it to
//    // FnCtx instead.
//    public static void case2() {
//        Object o = new Object();
//        while (true) {
//            synchronized(o) {
//                break; // or continue
//            }
//        }
//
//        // Need to know how many nesting levels we have.
//        while (true) {
//            while (true) {
//                synchronized(o) {
//                    break;
//                }
//            }
//            break;
//        }
//    }
//
//    // Distinguish case3 and case4.
//    // public static void case3() {
//    //     Object o = new Object();
//    //     synchronized(o) {
//    //         try {
//    //             System.out.println(1);
//    //             return;
//    //         } finally {
//    //             System.out.println(2);
//    //         }
//    //     // Don't know how many levels needed to be popped here.
//
//    //     }
//    // }
//    // public static void case4() {
//    //     Object o = new Object();
//    //     try {
//    //         synchronized(o) {
//    //             System.out.println(1);
//    //             return;
//    //         }
//    //     } finally {
//    //         System.out.println(2);
//    //     }
//    // }
//
//    // public static void case5() {
//    //     Object o1 = new Object();
//    //     Object o2 = new Object();
//    //     Object o3 = new Object();
//
//    //     try {
//    //         if (true) {
//    //             synchronized(o1) {
//    //                 throw new RuntimeException();
//    //             }
//    //         } else {
//    //             synchronized(o2) {
//    //                 synchronized(o3) {
//    //                     throw new RuntimeException();
//    //                 }
//    //             }
//    //         }
//    //     } catch(RuntimeException e) {
//    //         e.printstacktrace();
//    //     }
//    // }
//}