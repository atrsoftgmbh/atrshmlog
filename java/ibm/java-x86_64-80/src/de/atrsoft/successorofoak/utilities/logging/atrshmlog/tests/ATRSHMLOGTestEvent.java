package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestEvent {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	int event = theLog.getEvent(42);

	System.out.println("ATRSHMLOGEvent : " + event + " : ");

	int oldevent = theLog.setEventOff(42);
	
	System.out.println("ATRSHMLOGEvent : " + oldevent + " : ");

	event = theLog.getEvent(42);
	
	System.out.println("ATRSHMLOGEvent : " + event + " : ");

	oldevent = theLog.setEventOn(42);
	
	System.out.println("ATRSHMLOGEvent : " + oldevent + " : ");

	event = theLog.getEvent(42);
	
	System.out.println("ATRSHMLOGEvent : " + event + " : ");


	System.out.println(" ");
    }

}

/* end of file */
