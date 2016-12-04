package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestClicktime {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int id = theLog.getClockId();

	System.out.println("ATRSHMLOGTestClicktime : " + id  + " : ");

	int result = theLog.attach();

	id = theLog.getClockId();


	System.out.println("ATRSHMLOGTestClicktime : " + id  + " : ");

	long ret = theLog.getClicktime();
	
	System.out.println("ATRSHMLOGTestClicktime : " + ret  + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
