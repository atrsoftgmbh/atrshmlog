package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestEventLocksMax {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int maxindex = theLog.getEventLocksMax();

	System.out.println("ATRSHMLOGEventLocksMax : " + maxindex + " : ");

	int newindex = Integer.parseInt(args[0]);
	
	System.out.println("ATRSHMLOGEventLocksMax : " + newindex + " : ");

	int oldindex = theLog.setEventLocksMax(newindex);
					       
	System.out.println("ATRSHMLOGEventLocksMax : " + oldindex + " : ");

	maxindex = theLog.getEventLocksMax();

	System.out.println("ATRSHMLOGEventLocksMax : " + maxindex + " : ");

	int result = theLog.attach();

	oldindex = theLog.setEventLocksMax(newindex + 100);
					       
	System.out.println("ATRSHMLOGEventLocksMax : " + oldindex + " : ");

	maxindex = theLog.getEventLocksMax();

	System.out.println("ATRSHMLOGEventLocksMax : " + maxindex + " : ");

	System.out.println(" ");
    }

}

/* end of file */
