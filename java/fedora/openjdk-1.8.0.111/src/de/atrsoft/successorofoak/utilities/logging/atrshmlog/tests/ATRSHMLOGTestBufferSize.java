package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestBufferSize {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();


	int size = theLog.getBufferMaxSize();
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + size  + " : ");

	int aksize = theLog.getBufferSize();
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + aksize  + " : ");

	int oldsize = theLog.setBufferSize(size * 2);
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + oldsize  + " : ");

	aksize = theLog.getBufferSize();
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + aksize  + " : ");

	oldsize = theLog.setBufferSize(aksize / 2);
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + oldsize  + " : ");

	aksize = theLog.getBufferSize();
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + aksize  + " : ");

	oldsize = theLog.setBufferSize(size);
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + oldsize  + " : ");

	aksize = theLog.getBufferSize();
	
	System.out.println("ATRSHMLOGTestBufferMaxSize : " + aksize  + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
