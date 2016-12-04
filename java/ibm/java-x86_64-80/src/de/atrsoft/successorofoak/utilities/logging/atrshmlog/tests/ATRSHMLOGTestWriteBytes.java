package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestWriteBytes {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	long T = ATRSHMLOG.sgettime(1);

	byte[] b = new byte [16];

	b[0] = 64 + 1;
	b[1] = 64 + 2;
	b[2] = 64 + 3;
	b[3] = 32 ;
	b[4] = 48 + 1;
	b[5] = 48 + 2;
	b[6] = 48 + 3;
	b[7] = 32;
	b[8] = 64 + 1;
	b[9] = 64 + 2;
	b[10] = 64 + 3;
	b[11] = 32 ;
	b[12] = 48 + 1;
	b[13] = 48 + 2;
	b[14] = 48 + 3;
	b[15] = 32;
	
	int wresult = theLog.write(1,
				   ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS ,
				   42,
				   T,
				   0,
				   b);
	
	System.out.println("ATRSHMLOGTestWriteBytes : " + wresult + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
