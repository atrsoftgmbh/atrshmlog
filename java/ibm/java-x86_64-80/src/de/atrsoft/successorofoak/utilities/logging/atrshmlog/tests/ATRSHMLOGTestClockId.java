package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestClockId {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int id = theLog.getClockId();
	
	System.out.println("ATRSHMLOGTestClockId : " + id  + " : ");

	int oldid  = theLog.setClockId(2);

	System.out.println("ATRSHMLOGTestClockId : " + oldid  + " : ");
	
	id = theLog.getClockId();
	
	System.out.println("ATRSHMLOGTestClockId : " + id  + " : ");
	
	oldid  = theLog.setClockId(1);

	System.out.println("ATRSHMLOGTestClockId : " + oldid  + " : ");
	
	id = theLog.getClockId();
	
	System.out.println("ATRSHMLOGTestClockId : " + id  + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
