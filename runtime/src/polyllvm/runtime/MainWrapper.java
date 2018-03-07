package polyllvm.runtime;

// Wraps the Java entry point in a try-catch block so that we can
// report uncaught exceptions to the client.
class MainWrapper {

    // This native method is generated by PolyLLVM when it encounters
    // the main method in the client program.
    static native void main(String[] args);

    static void runMain(String[] args) {
        try {
            main(args);
        }
        catch (Throwable e) {
            System.err.println(
                "Terminating due to uncaught exception "
                + e.getClass().getName() + ": " + e.getMessage());
            // TODO: System.exit(1);
        }
    }
}
