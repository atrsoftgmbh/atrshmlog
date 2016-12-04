package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestInitShmLog {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	System.out.println("ATRSHMLOGTestInitShmLog: " + result  + " : ");

	if (result != 0)
	    {
		return;
	    }
	
	int count = Integer.parseInt(args[0]);

	System.out.println("ATRSHMLOGTestInitShmLog: " + count  + " : ");

	long area = theLog.getArea();
	
	int iret = theLog.initShmLog(area,count);
	
	System.out.println("ATRSHMLOGTestInitShmLog: " + iret  + " : ");

	
	System.out.println(" ");
    }

}

/* end of file */
