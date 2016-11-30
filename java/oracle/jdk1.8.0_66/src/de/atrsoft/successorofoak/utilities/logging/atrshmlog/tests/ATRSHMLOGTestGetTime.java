package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestGetTime {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	long t = theLog.getInittimeTscBefore();
	
	System.out.println("ATRSHMLOGTestGetTime : " + t  + " : ");

	t = theLog.getInittimeTscAfter();
	
	System.out.println("ATRSHMLOGTestGetTime : " + t  + " : ");

	long[] l = new long [2];

	int gret = theLog.getInittime(l);

	System.out.println("ATRSHMLOGTestGetTime : " + l[0] + " : " + l[1]  + " : ");

	
	System.out.println(" ");
    }

}

/* end of file */
