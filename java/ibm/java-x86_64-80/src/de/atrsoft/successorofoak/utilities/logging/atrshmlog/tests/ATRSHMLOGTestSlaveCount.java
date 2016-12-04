package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestSlaveCount {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int count = theLog.getFListBufferSlaveCount();
	
	System.out.println("ATRSHMLOGTestSlaveCount : " + count  + " : ");

	int newcount = Integer.parseInt(args[0]);
	
	System.out.println("ATRSHMLOGTestSlaveCount : " + newcount  + " : ");

	int oldcount = theLog.setFListBufferSlaveCount(newcount);
	
	System.out.println("ATRSHMLOGTestSlaveCount : " + oldcount  + " : ");

	count = theLog.getFListBufferSlaveCount();
	
	System.out.println("ATRSHMLOGTestSlaveCount : " + count  + " : ");

	int result = theLog.attach();

	theLog.sleepNanos(1000000000);

	int akcount = 0;

	long slave = theLog.getNextSlaveLocal(0);

	while (slave != 0)
	    {
		++akcount;
		slave = theLog.getNextSlaveLocal(slave);
	    }

	System.out.println("ATRSHMLOGTestSlaveCount : " + akcount  + " : ");

	
	System.out.println(" ");
    }

}

/* end of file */
