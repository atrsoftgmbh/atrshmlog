package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestDecrementSlaveCount {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	theLog.sleepNanos(1000000000);
	
	int akcount = 0;

	long slave = theLog.getNextSlaveLocal(0);

	while (slave != 0)
	    {
		++akcount;
		slave = theLog.getNextSlaveLocal(slave);
	    }

	System.out.println("ATRSHMLOGTestDecrementSlaveCount : " + akcount  + " : ");

	int s = theLog.createSlave();

	theLog.sleepNanos(1000000000);

	System.out.println("ATRSHMLOGTestDecrementSlaveCount : " + s + " : ");

	akcount = 0;

	slave = theLog.getNextSlaveLocal(0);

	while (slave != 0)
	    {
		++akcount;
		slave = theLog.getNextSlaveLocal(slave);
	    }

	System.out.println("ATRSHMLOGTestDecrementSlaveCount : " + akcount  + " : ");

	int r = theLog.decrementSlaveCount();
	
	System.out.println("ATRSHMLOGTestDecrementSlaveCount : " + r  + " : ");

	while (r > 0)
	    {
		r = theLog.decrementSlaveCount();
		System.out.println("ATRSHMLOGTestDecrementSlaveCount : " + r  + " : ");
	    }

	r = theLog.decrementSlaveCount();
	System.out.println("ATRSHMLOGTestDecrementSlaveCount : " + r  + " : ");	

	System.out.println(" ");
    }

}

/* end of file */
