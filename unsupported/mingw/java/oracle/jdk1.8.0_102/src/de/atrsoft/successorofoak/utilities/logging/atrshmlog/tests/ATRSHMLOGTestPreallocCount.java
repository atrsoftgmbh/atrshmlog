package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestPreallocCount {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int count = theLog.getPreallocBufferCount();
	
	System.out.println("ATRSHMLOGTestPreallocCount : " + count  + " : ");

	int oldid  = theLog.setPreallocBufferCount(42);

	System.out.println("ATRSHMLOGTestPreallocCount : " + oldid  + " : ");
	
	 count = theLog.getPreallocBufferCount();
	
	System.out.println("ATRSHMLOGTestPreallocCount : " + count  + " : ");

	 oldid  = theLog.setPreallocBufferCount(128);

	System.out.println("ATRSHMLOGTestPreallocCount : " + oldid  + " : ");
	
	 count = theLog.getPreallocBufferCount();
	
	System.out.println("ATRSHMLOGTestPreallocCount : " + count  + " : ");

	
	System.out.println(" ");
    }

}

/* end of file */
