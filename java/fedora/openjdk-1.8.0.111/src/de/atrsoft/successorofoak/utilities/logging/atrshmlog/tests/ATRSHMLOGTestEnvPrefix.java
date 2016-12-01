package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestEnvPrefix {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	theLog.setEnvPrefix("OTTO");

	String prefix = theLog.getEnvPrefix();
	
	System.out.println("ATRSHMLOGTestEnvPrefix : " + prefix + " : ");

	theLog.setEnvPrefix("HUGO");

	prefix = theLog.getEnvPrefix();
	
	System.out.println("ATRSHMLOGTestEnvPrefix : " + prefix + " : ");

	System.out.println(" ");
    }

}

/* end of file */
