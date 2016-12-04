package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestCleanupLocks {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	System.out.println("ATRSHMLOGTestCleanupLocks : " + result  + " : ");

	if (result != 0)
	    {
		return;
	    }
	
	long area = theLog.getArea();
	
	theLog.cleanupLocks(area);
	
	System.out.println("ATRSHMLOGTestCleanupLocks  : ");

	
	System.out.println(" ");
    }

}

/* end of file */
