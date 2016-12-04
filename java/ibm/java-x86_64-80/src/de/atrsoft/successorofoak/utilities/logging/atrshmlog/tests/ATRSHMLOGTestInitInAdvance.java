package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestInitInAdvance {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	int f = theLog.getInitBuffersInAdvance();

	System.out.println("ATRSHMLOGTestInitInAdvance : " + f + " : ");

	int old = theLog.setInitBuffersInAdvanceOn();
	
	System.out.println("ATRSHMLOGTestInitInAdvance : " + old + " : ");

	f = theLog.getInitBuffersInAdvance();

	System.out.println("ATRSHMLOGTestInitInAdvance : " + f + " : ");

        old = theLog.setInitBuffersInAdvanceOff();
	
	System.out.println("ATRSHMLOGTestInitInAdvance : " + old + " : ");

	f = theLog.getInitBuffersInAdvance();

	System.out.println("ATRSHMLOGTestInitInAdvance : " + f + " : ");

        old = theLog.setInitBuffersInAdvanceOn();
	
	System.out.println("ATRSHMLOGTestInitInAdvance : " + old + " : ");

	f = theLog.getInitBuffersInAdvance();

	System.out.println("ATRSHMLOGTestInitInAdvance : " + f + " : ");

	System.out.println(" ");
    }

}

/* end of file */
