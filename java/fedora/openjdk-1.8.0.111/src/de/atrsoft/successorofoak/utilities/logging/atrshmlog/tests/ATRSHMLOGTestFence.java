package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestFence {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int result = theLog.attach();

	int f = theLog.getThreadFence1();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	int old  = theLog.setThreadFence1(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence1();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence1(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence1();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

        f = theLog.getThreadFence2();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence2(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence2();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence2(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence2();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	f = theLog.getThreadFence3();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence3(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence3();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence3(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence3();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	f = theLog.getThreadFence4();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence4(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence4();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence4(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence4();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence5();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence5(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence5();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence5(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence5();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence6();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence6(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence6();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence6(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence6();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence7();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence7(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence7();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence7(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence7();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence8();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence8(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence8();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence8(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence8();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence9();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence9(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence9();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence9(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence9();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence10();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence10(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence10();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence10(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence10();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence11();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence11(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence11();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence11(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence11();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence12();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence12(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence12();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence12(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence12();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");


	f = theLog.getThreadFence13();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence13(1);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence13();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	old  = theLog.setThreadFence13(0);

	System.out.println("ATRSHMLOGTestFence : " + old + " : ");

	f = theLog.getThreadFence13();
	
	System.out.println("ATRSHMLOGTestFence : " + f + " : ");

	System.out.println(" ");
    }

}

/* end of file */
