package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestReuseBuffer {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	long tid = theLog.getTid();

	System.out.println("ATRSHMLOGTestReuseBuffer : " + tid + " : ");

	int count = theLog.reuseThreadBuffers(tid);

	System.out.println("ATRSHMLOGTestReuseBuffer : " + count + " : ");

	theLog.write(1, ATRSHMLOG.EVENT_POINT_IN_TIME_C, 42,0,0);
	
	count = theLog.reuseThreadBuffers(tid);

	System.out.println("ATRSHMLOGTestReuseBuffer : " + count + " : ");
	

	System.out.println(" ");
    }

}

/* end of file */
