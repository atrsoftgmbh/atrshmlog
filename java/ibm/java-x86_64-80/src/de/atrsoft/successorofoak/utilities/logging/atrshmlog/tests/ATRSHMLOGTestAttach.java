package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

public class ATRSHMLOGTestAttach {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int result = theLog.attach();

	System.out.println("ATRSHMLOGTestAttach : " + result + " : ");

	System.out.println("Shmid : " + theLog.getShmid() + " : ");
	
	System.out.println("AcquireCount : " + theLog.getAcquireCount() + " : ");
	System.out.println("BufferId : " + theLog.getBufferId() + " : ");
	
	System.out.println("InitBuffersInAdvance : " + theLog.getInitBuffersInAdvance() + " : ");
	
	System.out.println("BufferSize : " + theLog.getBufferSize() + " : ");
	
	System.out.println("PreallocBufferCount : " + theLog.getPreallocBufferCount() + " : ");
	
	System.out.println("FListBufferSlaveCount : " + theLog.getFListBufferSlaveCount() + " : ");
	
	System.out.println("FListBufferSlaveWait : " + theLog.getFListBufferSlaveWait() + " : ");
	
	System.out.println("WaitForSlaves : " + theLog.getWaitForSlaves() + " : ");
	
	System.out.println("StrategyProcess : " + theLog.getStrategyProcess() + " : ");

	System.out.println("AutoflushProcess : " + theLog.getAutoflushProcess() + " : ");

	
	System.out.println("Checksum : " + theLog.getChecksum() + " : ");
	
	System.out.println("ThreadFence1 : " + theLog.getThreadFence1() + " : ");
	System.out.println("ThreadFence2 : " + theLog.getThreadFence2() + " : ");
	System.out.println("ThreadFence3 : " + theLog.getThreadFence3() + " : ");
	System.out.println("ThreadFence4 : " + theLog.getThreadFence4() + " : ");
	System.out.println("ThreadFence5 : " + theLog.getThreadFence5() + " : ");
	System.out.println("ThreadFence6 : " + theLog.getThreadFence6() + " : ");
	System.out.println("ThreadFence7 : " + theLog.getThreadFence7() + " : ");
	System.out.println("ThreadFence8 : " + theLog.getThreadFence8() + " : ");
	System.out.println("ThreadFence9 : " + theLog.getThreadFence9() + " : ");
	System.out.println("ThreadFence10 : " + theLog.getThreadFence10() + " : ");
	System.out.println("ThreadFence11 : " + theLog.getThreadFence11() + " : ");
	System.out.println("ThreadFence12 : " + theLog.getThreadFence12() + " : ");
	System.out.println("ThreadFence13 : " + theLog.getThreadFence13() + " : ");

	System.out.println("InittimeTscBefore : " + theLog.getInittimeTscBefore() + " : ");
	System.out.println("InittimeTscAfter : " + theLog.getInittimeTscAfter() + " : ");
	
	long[] inittime = new long[2];

	theLog.getInittime(inittime);

	System.out.println("Inittime : " + inittime[0] + " " + inittime[1] + " : ");
	
	System.out.println("EventLocksMax : " + theLog.getEventLocksMax() + " : ");
	
	System.out.println("ClockId : " + theLog.getClockId() + " : ");
	
	System.out.println("Version : " + theLog.getVersion() + " : ");

	System.out.println("MinorVersion : " + theLog.getMinorVersion() + " : ");
	
	System.out.println("PatchVersion : " + theLog.getPatchVersion() + " : ");

	System.out.println("BufferMaxSize : " + theLog.getBufferMaxSize() + " : ");
	
	System.out.println(" ");
    }

}

/* end of file */
