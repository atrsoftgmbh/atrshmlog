package de.atrsoft.successorofoak.utilities.logging.atrshmlog;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;


/** \file ATRSHMLOGTest.java 
 * \brief The implementation of the jni layer test drive
 *
 * We simply try to use as many methods as we can.
 */


/**
 * \brief the main class for usage
 */
public class ATRSHMLOGTest {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	String payload2 = "Hello, world.";

	String payload3 = "Hello, welt.";

	
	// we do our first jni method calls
	// this will take a much longer time
	// so we simply are interested if it works.
	// not so much how fast it is
	long starttime = theLog.gettime(1);

	long endtime = theLog.gettime(1);

	int logresult = theLog.write(1, 
				     ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C,  
				     1,
				     starttime,
				     endtime);

	// this is the second call.
	// so from here on we ARE interestet in the time it takes.
	starttime = theLog.gettime(2);


	// we try to get the shortest time frame.
	// so we do not take the end time directly.
	// we use instead the internal gettime in the write
	// funcions call.
	// so out endtime is 0 and we have an interval timing
	logresult = theLog.write(2, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C,  
				 1,
				 starttime,
				 0);

	System.out.println("logging done. Times start "
			   + starttime
			   + " times end "
			   + endtime
			   + " return attach is "
			   + result
			   + " return write is "
			   + logresult);

	

	// now we want to know the overhead for things 
	// we use a simple loop to get the time a thousand times
	long interrimtime = 0;
	
	long starttimeoverhead = theLog.gettime(3);

	for (int i = 0; i < 1000; i++) {
	    interrimtime = theLog.gettime(3);
	}

	long endtimeoverhead = theLog.gettime(3);

	logresult = theLog.write(3, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS, 
				 1,
				 starttimeoverhead,
				 endtimeoverhead,
				 "loop 1000 times gettime");

	// now we meature one time taking with two times 

	long start1 = theLog.gettime(4);

	long end1 = theLog.gettime(4);

	logresult = theLog.write(4, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C,  
				 1, 
				 start1,
				 end1);

	long end2 = theLog.gettime(5);

	logresult = theLog.write(5, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS, 
				 1, 
				 end1,
				 end2);

	// ok, this should give us the time for the gettimes and the logging
	long end3 = theLog.gettime(6);
	
	logresult = theLog.write(6, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS,  
				 1, 
				 end2,
				 end3,
				 payload2);
	
	logresult = theLog.write(6, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS,  
				 2, 
				 end3,
				 0,
				 "time for payload 2 logging");
	
	long end4 = theLog.gettime(7);


	// we try now byte arrays
	// ths makes it possible to log everything.
	// but its up to your converter to interpret it right

	byte[] payloadbytes = new byte [255];

	byte[] payloadbytes2 = new byte [255];

	payloadbytes[0] = 65 ; // H
	payloadbytes[1] = 68 ; // E
	payloadbytes[2] = 71 ; // L
	payloadbytes[3] = 71 ; // L
	payloadbytes[4] = 74 ; // O
	payloadbytes[5] = 32 ; // 
	payloadbytes[6] = 82 ; // W
	payloadbytes[7] = 45 ; // E
	payloadbytes[8] = 71 ; // L
	payloadbytes[9] = 79 ; // T

	payloadbytes2[0] = 70 ; // H
	payloadbytes2[1] = 65 ; // E
	payloadbytes2[2] = 71 ; // L
	payloadbytes2[3] = 71 ; // L
	payloadbytes2[4] = 74 ; // O
	payloadbytes2[5] = 32 ; // 
	payloadbytes2[6] = 82 ; // W
	payloadbytes2[7] = 45 ; // E
	payloadbytes2[8] = 71 ; // L
	payloadbytes2[9] = 79 ; // T

	long end5 = theLog.gettime(7);
	
	logresult = theLog.write(7, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C,
				 1, 
				 end3,
				 end4,
				 payloadbytes);
	
	long end6 = theLog.gettime(8);
	
	logresult = theLog.write(8, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C, 
				 1 , 
				 end5,
				 end6,
				 payloadbytes2);

	long end7 = theLog.gettime(9);

	logresult = theLog.write(9, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS, 
				 1,
				 end6,
				 end7,
				 payload2,
				 5);

	long end8 = theLog.gettime(10);

	logresult = theLog.write(10,
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS,
				 1 ,
				 end7,
				 end8,
				 payload3,
				 5);

	long end9 = theLog.gettime(11);

	logresult = theLog.write(11, 
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C, 
				 1002 ,
				 end1,
				 end2);


	long end10 = ATRSHMLOG.sgettime(12);

	long end11 = ATRSHMLOG.sgettime(12);

	long end12 = ATRSHMLOG.sgettime(12);

	logresult = theLog.write(12,
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_C,
				 1,
				 end11,
				 end12);

	logresult = theLog.write(13,
				 ATRSHMLOG.EVENT_INTERVAL_IN_TIME_UCS,
				 1 ,
				 end7,
				 end8,
				 payload3,
				 3,
				 4);

    }
}

/* end of file */
