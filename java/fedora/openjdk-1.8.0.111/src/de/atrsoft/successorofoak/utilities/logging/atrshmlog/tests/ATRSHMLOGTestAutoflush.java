package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestAutoflush {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int id = theLog.getAutoflush();
	
	System.out.println("ATRSHMLOGTestAutoflush : " + id  + " : ");

	int oldid  = theLog.setAutoflush(1);

	System.out.println("ATRSHMLOGTestAutoflush : " + oldid  + " : ");
	
	id = theLog.getAutoflush();
	
	System.out.println("ATRSHMLOGTestAutoflush : " + id  + " : ");
	
	oldid  = theLog.setAutoflush(0);

	System.out.println("ATRSHMLOGTestAutoflush : " + oldid  + " : ");
	
	id = theLog.getAutoflush();
	
	System.out.println("ATRSHMLOGTestAutoflush : " + id  + " : ");
	
	id = theLog.getAutoflushProcess();
	
	System.out.println("ATRSHMLOGTestAutoflush : " + id  + " : ");

	oldid  = theLog.setAutoflushProcess(1);

	System.out.println("ATRSHMLOGTestAutoflush : " + oldid  + " : ");
	
	id = theLog.getAutoflushProcess();
	
	System.out.println("ATRSHMLOGTestAutoflush : " + id  + " : ");
	
	oldid  = theLog.setAutoflushProcess(0);

	System.out.println("ATRSHMLOGTestAutoflush : " + oldid  + " : ");
	
	id = theLog.getAutoflushProcess();
	
	System.out.println("ATRSHMLOGTestAutoflush : " + id  + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
