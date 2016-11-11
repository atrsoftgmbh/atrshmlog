/* hello world for jni */

public class HelloJNI {
    // we load the shared lib in at classload 
    static {
	System.loadLibrary("hellotest");
    }

    // we declare the native
    private native void sayHello();

    // we start here and execute
    public static void main(String args[]) {
	HelloJNI hello = new HelloJNI();

	hello.sayHello();
    }

}

/* end of file */
