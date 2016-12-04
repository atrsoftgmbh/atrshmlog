package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestBufferSlaveWait {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int nanos = theLog.getFListBufferSlaveWait();
	
	System.out.println("ATRSHMLOGTestBufferSlaveWait : " + nanos  + " : ");

	int old  = theLog.setFListBufferSlaveWait(250000);

	System.out.println("ATRSHMLOGTestWaitForSlaves : " + old  + " : ");

	nanos = theLog.getFListBufferSlaveWait();
	
	System.out.println("ATRSHMLOGTestBufferSlaveWait : " + nanos  + " : ");

	System.out.println(" ");
    }

}

/* end of file */
