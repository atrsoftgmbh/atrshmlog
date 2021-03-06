package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestWaitForSlaves {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int flag = theLog.getWaitForSlaves();
	
	System.out.println("ATRSHMLOGTestWaitForSlaves : " + flag  + " : ");

	int old  = theLog.setWaitForSlavesOn();

	System.out.println("ATRSHMLOGTestWaitForSlaves : " + old  + " : ");

	flag = theLog.getWaitForSlaves();
	
	System.out.println("ATRSHMLOGTestWaitForSlaves : " + flag  + " : ");

	old  = theLog.setWaitForSlavesOff();

	System.out.println("ATRSHMLOGTestWaitForSlaves : " + old  + " : ");

	flag = theLog.getWaitForSlaves();
	
	System.out.println("ATRSHMLOGTestWaitForSlaves : " + flag  + " : ");

	old  = theLog.setWaitForSlavesOn();

	System.out.println("ATRSHMLOGTestWaitForSlaves : " + old  + " : ");

	flag = theLog.getWaitForSlaves();
	
	System.out.println("ATRSHMLOGTestWaitForSlaves : " + flag  + " : ");

	// we have to switch off or the slaves will not go down
	// this hangs on mingw - on the others it semm NOT to hang...
	// so i wonder if mingw is the only to do it right...
	theLog.setLoggingProcessOffFinal();
	
	System.out.println(" ");
    }

}

/* end of file */
