package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestArea {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	long area = theLog.getArea();
	
	System.out.println("ATRSHMLOGTestArea : " + area + " : ");

	int ihf = theLog.getAreaIchHabeFertig(area);
	
	System.out.println("ATRSHMLOGTestArea : " + ihf + " : ");

	int oldihf = theLog.setAreaIchHabeFertig(area,1);
	
	System.out.println("ATRSHMLOGTestArea : " + oldihf + " : ");

	ihf = theLog.getAreaIchHabeFertig(area);
	
	System.out.println("ATRSHMLOGTestArea : " + ihf + " : ");

	oldihf = theLog.setAreaIchHabeFertig(area,0);
	
	System.out.println("ATRSHMLOGTestArea : " + oldihf + " : ");
	
	ihf = theLog.getAreaIchHabeFertig(area);
	
	System.out.println("ATRSHMLOGTestArea : " + ihf + " : ");

	int count = theLog.getAreaCount(area);
	
	System.out.println("ATRSHMLOGTestArea : " + count + " : ");

	int version = theLog.getAreaVersion(area);
	
	System.out.println("ATRSHMLOGTestArea : " + version + " : ");

	System.out.println(" ");
    }

}

/* end of file */
