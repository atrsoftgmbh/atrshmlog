package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestChecksum {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int id = theLog.getChecksum();
	
	System.out.println("ATRSHMLOGTestChecksum : " + id  + " : ");

	int oldid  = theLog.setChecksum(1);

	System.out.println("ATRSHMLOGTestChecksum : " + oldid  + " : ");
	
	id = theLog.getChecksum();
	
	System.out.println("ATRSHMLOGTestChecksum : " + id  + " : ");
	
	oldid  = theLog.setChecksum(0);

	System.out.println("ATRSHMLOGTestChecksum : " + oldid  + " : ");
	
	id = theLog.getChecksum();
	
	System.out.println("ATRSHMLOGTestChecksum : " + id  + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
