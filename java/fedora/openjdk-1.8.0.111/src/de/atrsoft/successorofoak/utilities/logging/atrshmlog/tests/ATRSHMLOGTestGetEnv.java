package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestGetEnv {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();
	
	String val = theLog.getEnv(args[0]);
	
	System.out.println("ATRSHMLOGTestGetEnv : " + val + " : ");

	val = theLog.getEnvShmid();

	System.out.println("ATRSHMLOGTestGetEnv : " + val + " : ");

	val = theLog.getEnvIdSuffix();

	System.out.println("ATRSHMLOGTestGetEnv : " + val + " : ");


	System.out.println(" ");
    }

}

/* end of file */
