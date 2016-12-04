package de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests;

/**
 * We import our jni test layer.
 */
import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_error;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_counter;

import de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG.atrshmlog_strategy;

import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

public class ATRSHMLOGTestCreate {

    ///////////////////////////////////////////////////////
    // we start here and execute as a demo
    public static void main(String args[]) {
	ATRSHMLOG theLog = new ATRSHMLOG();

	int key = Integer.parseInt(args[0]);

	System.out.println("ATRSHMLOGTestCreate: " + key  + " : ");

	int count = Integer.parseInt(args[1]);

	System.out.println("ATRSHMLOGTestCreate: " + count  + " : ");

	int ret = theLog.create(key,count);
	
	System.out.println("ATRSHMLOGTestCreate: " + ret  + " : ");

	if (ret > 0)
	    {
		BufferedWriter writer = null;
		
		try
		    {
			FileWriter f = new FileWriter( "dot.atrshmlog" );
			writer = new BufferedWriter( f );
			writer.write( "ATRSHMLOG_ID=" + ret + "\nexport ATRSHMLOG_ID\nATRSHMLOG_COUNT=" + count +"\nexport ATRSHMLOG_COUNT\n");
		    }
		catch (IOException e)
		    {
			System.out.println("ATRSHMLOGTestCreate: BUMM ");
		    }
		finally
		    {
			try
			    {
				if (writer != null)
				    writer.close();
			    }
			catch (IOException e)
			    {
				System.out.println("ATRSHMLOGTestCreate: BUMM ");
			    }
			finally
			    {
				;
			    }
		    }

	    }
	
	System.out.println(" ");
    }

}

/* end of file */
