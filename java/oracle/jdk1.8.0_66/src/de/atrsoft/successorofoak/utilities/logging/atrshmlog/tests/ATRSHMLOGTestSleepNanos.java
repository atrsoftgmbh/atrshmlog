package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestSleepNanos {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();
	
	System.out.println("ATRSHMLOGTestSleepNanos : " + result + " : ");

	for (int n = 1000000 ; n != 0 ; n >>= 1)
	    {
		long t1, t2;
		t1 = theLog.gettime(1);
		theLog.sleepNanos(n);
		t2 = theLog.gettime(1);

		System.out.println("n " + n + " : " + (t2 - t1) + " : ");
	    }
		
	System.out.println(" ");
    }

}

/* end of file */
