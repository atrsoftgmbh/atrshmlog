package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestStrategy {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int s = theLog.getStrategy();

	System.out.println("ATRSHMLOGTestStrategy : " + s + " : ");

	int old = theLog.setStrategy(4);
	
	System.out.println("ATRSHMLOGTestStrategy : " + old + " : ");

	s = theLog.getStrategy();

	System.out.println("ATRSHMLOGTestStrategy : " + s + " : ");

	 old = theLog.setStrategy(1);
	
	System.out.println("ATRSHMLOGTestStrategy : " + old + " : ");

	s = theLog.getStrategy();

	System.out.println("ATRSHMLOGTestStrategy : " + s + " : ");

	s = theLog.getStrategyProcess();

	System.out.println("ATRSHMLOGTestStrategy : " + s + " : ");

	 old = theLog.setStrategyProcess(4);
	
	System.out.println("ATRSHMLOGTestStrategy : " + old + " : ");

	s = theLog.getStrategyProcess();

	System.out.println("ATRSHMLOGTestStrategy : " + s + " : ");

	 old = theLog.setStrategyProcess(1);
	
	System.out.println("ATRSHMLOGTestStrategy : " + old + " : ");

	s = theLog.getStrategyProcess();

	System.out.println("ATRSHMLOGTestStrategy : " + s + " : ");

	System.out.println(" ");
    }

}

/* end of file */
