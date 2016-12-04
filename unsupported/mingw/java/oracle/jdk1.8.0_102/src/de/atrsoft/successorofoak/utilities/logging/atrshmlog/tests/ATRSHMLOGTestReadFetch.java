package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestReadFetch {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	byte[] b = new byte[1000000];

	long[] r = new long[100];

	long area = theLog.getArea();
	
	int ret = theLog.readFetch(area,
				   b,
				   r);

	
	System.out.println("ATRSHMLOGTestReadFetch: " + ret  + " : ");

	if (ret == 0)
	    {
		if (r[1] > 0)
		    {
			System.out.println("ATRSHMLOGTestReadFetch: " + r[0]  + " : " + r[1] + " : ");
		    }
	    }
	
	System.out.println(" ");
    }

}

/* end of file */
