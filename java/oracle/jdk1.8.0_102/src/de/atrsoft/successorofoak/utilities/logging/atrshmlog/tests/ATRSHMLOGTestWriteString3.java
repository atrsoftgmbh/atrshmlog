package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestWriteString3 {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	long T = ATRSHMLOG.sgettime(1);

	int wresult = theLog.write(1,
				   ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS ,
				   42,
				   T,
				   0,
				   "Hallo, Welt.",
				   4,
				   5);
	
	System.out.println("ATRSHMLOGTestWriteString3 : " + wresult + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
