package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestRead {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int index = Integer.parseInt(args[0]);

	byte[] b = new byte[1000000];

	long[] r = new long[100];

	long area = theLog.getArea();
	
	int ret = theLog.read(area,
			      index,
			      b,
			      r);

	
	System.out.println("ATRSHMLOGTestRead: " + ret  + " : ");

	if (ret == 0)
	    {
		if (r[0] > 0)
		    {
			System.out.println("ATRSHMLOGTestRead: " + r[0]  + " : ");
		    }
	    }
	
	System.out.println(" ");
    }

}

/* end of file */
