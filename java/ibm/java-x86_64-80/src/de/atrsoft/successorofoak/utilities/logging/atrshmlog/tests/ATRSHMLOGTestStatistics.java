package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestStatistics {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	int maxindex = theLog.getStatisticsMaxIndex();

	System.out.println("ATRSHMLOGTestStatistics : " + maxindex +  " : ");

	int[] stats = new int [maxindex + 1];

	int statresult = theLog.getStatistics(stats);

	System.out.println("ATRSHMLOGTestStatistics : " + statresult +  " : ");

	if (statresult == 0)
	    {
		for (int index  = 0; index <= maxindex; index++)
		    {
			System.out.println("stat " + index + " : " + stats[index] + " : ");
		    }
	    }
	
	System.out.println(" ");
    }

}

/* end of file */
