package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestLogging {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	int logging = theLog.getLogging();

	System.out.println("ATRSHMLOGTestLogging : " + logging + " : ");

	int oldlogging = theLog.setLoggingProcessOff();
	
	System.out.println("ATRSHMLOGTestLogging : " + oldlogging + " : ");

	logging = theLog.getLogging();

	System.out.println("ATRSHMLOGTestLogging : " + logging + " : ");

	 oldlogging = theLog.setLoggingProcessOn();
	
	System.out.println("ATRSHMLOGTestLogging : " + oldlogging + " : ");

	logging = theLog.getLogging();

	System.out.println("ATRSHMLOGTestLogging : " + logging + " : ");



	long area = theLog.getArea();

	int old = theLog.setAreaIchHabeFertig(area, 1);
	
	logging = theLog.getLogging();

	System.out.println("ATRSHMLOGTestLogging : " + logging + " : ");

	old = theLog.setAreaIchHabeFertig(area, 0);
	
	logging = theLog.getLogging();

	System.out.println("ATRSHMLOGTestLogging : " + logging + " : ");

	oldlogging = theLog.setLoggingProcessOffFinal();
	
	System.out.println("ATRSHMLOGTestLogging : " + oldlogging + " : ");

	logging = theLog.getLogging();

	System.out.println("ATRSHMLOGTestLogging : " + logging + " : ");
	

	System.out.println(" ");
    }

}

/* end of file */
