package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestVersion {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	int version = theLog.getVersion();

	int minorversion = theLog.getMinorVersion();
	
	int patchversion = theLog.getPatchVersion();

	System.out.println("ATRSHMLOGTestVersion : " + version + " : ");
	System.out.println("ATRSHMLOGTestVersion : " + minorversion + " : ");
	System.out.println("ATRSHMLOGTestVersion : " + patchversion + " : ");


	System.out.println(" ");
    }

}

/* end of file */
