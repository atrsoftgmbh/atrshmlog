package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestThreadLocal {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();


	long tl = theLog.getThreadLocalsAdress();

	System.out.println("ATRSHMLOGTestThreadLocal : " + tl + " : ");

	long tid = theLog.getTid();

	System.out.println("ATRSHMLOGTestThreadLocal : " + tid + " : ");

	long tltid = theLog.getThreadLocalTid (tl);

	System.out.println("ATRSHMLOGTestThreadLocal : " + tltid + " : ");

	theLog.write(1, ATRSHMLOG.EVENT_POINT_IN_TIME_C, 42,0,0);
	
	 tltid = theLog.getThreadLocalTid (tl);

	System.out.println("ATRSHMLOGTestThreadLocal : " + tltid + " : ");



	theLog.turnLoggingOff(tl);

	System.out.println("ATRSHMLOGTestThreadLocal ");

	

	System.out.println(" ");
    }

}

/* end of file */
