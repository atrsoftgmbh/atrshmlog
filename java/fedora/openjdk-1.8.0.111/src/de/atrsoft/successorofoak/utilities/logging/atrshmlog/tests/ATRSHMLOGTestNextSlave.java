package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestNextSlave {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();


	int target = Integer.parseInt(args[0]);
	
	int result = theLog.attach();

	theLog.sleepNanos(1000000000);
	
	int akcount = 0;

	long slave = theLog.getNextSlaveLocal(0);

	while (slave != 0)
	    {
		++akcount;
		slave = theLog.getNextSlaveLocal(slave);
	    }

	System.out.println("ATRSHMLOGTestNextSlave : " + akcount  + " : ");

	int s = 0;
	
	for (int i = 0; i < 8 ; i++)
	    {
		 s = theLog.createSlave();
	    }

	System.out.println("ATRSHMLOGTestNextSlave : " + s + " : ");

	theLog.sleepNanos(1000000000);

	long targetslave = 0;
	
	akcount = 0;

	slave = theLog.getNextSlaveLocal(0);

	while (slave != 0)
	    {
		++akcount;

		System.out.println("ATRSHMLOGTestNextSlave : " + akcount  + " : ");
		System.out.println("ATRSHMLOGTestNextSlave : " + slave  + " : ");
		long tid = theLog.getSlaveTid(slave);
		
		System.out.println("ATRSHMLOGTestNextSlave : " + tid  + " : ");

		if (akcount == target)
		    targetslave = slave;

		slave = theLog.getNextSlaveLocal(slave);
	    }

	System.out.println("ATRSHMLOGTestCreateSlave : " + akcount  + " : ");

	if (targetslave != 0)
	    {

		theLog.turnSlaveOff(targetslave);
		
		theLog.sleepNanos(1000000000);

		akcount = 0;

		slave = theLog.getNextSlaveLocal(0);

		while (slave != 0)
		    {
			++akcount;

			System.out.println("ATRSHMLOGTestNextSlave : " + akcount  + " : ");
			System.out.println("ATRSHMLOGTestNextSlave : " + slave  + " : ");
			long tid = theLog.getSlaveTid(slave);
		
			System.out.println("ATRSHMLOGTestNextSlave : " + tid  + " : ");


			slave = theLog.getNextSlaveLocal(slave);
		    }
	    }
	
	System.out.println(" ");
    }

}

/* end of file */
