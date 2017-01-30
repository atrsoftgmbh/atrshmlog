PROCESS : Implement the converter for your system.  {#process_implementconverter}
=============================================

When it comes to logging most people think about text.

And so this logging is normally a text logging. See for example the
mksh implementation.

The write functions 1 and 2 have for the payload a non c string part.
This is moved with memcpy, so in theory you can also log binary data.

This will be transported by the reader d again with a memcpy, so it is
finally up to the converter you use to make use of it.

To use binary data its best to write the own converter.

This will give you some hints how to do it.

The converter in place
----------------------

The converter that is deliverd with the module makes a simple
text approch. So binary data is - nearly - not an option here.

Nearly.

In fact it uses the eventflag to determine if the payload is in fact a ucs2
java string.

Then it tries its best to give you an output of that string.

For now it is simply using the low bytes and replaces any nonprintable with
a space - with one exception, the lf thing.

They trigger already in the existing converter a multiline write with
one line per part between the lf thing.

So you have here in case a 'p' or 'i' is used a rough text output.

In a later minor version this will change to a utf replacement.

See for the thing now.

Open atrshmlogconvert.c in your editor.

The program starts with the usual parameter checkings and the log message.

Then it tries to read in the the header information.

If anything seems wrong it stops.

After the header the filler is read.

Then the real buffer data follows.

After the buffer is read in the function operate does the rest.

So in operate we have the interpretation.

See for the test of the eventflag.

Swith to the while loop:

      /* and now we can start with a realtime adder */
       
       while (akindex < i_read_len)
         {
           int chunklen;
           atrshmlog_time_t starttime;
           atrshmlog_time_t endtime;
           int event;
           char eventflag = 0;
           int userflag;


Then switch to the usage of the flag.


     	char eventflag = *(h + (ATRSHMLOGCONTROLDATASIZE - 1));
     	eventflagstr[0] = eventflag;
     	eventflagstr[1] = ' ' ; 
     	eventflagstr[2] = '\0';
     	
     	strcat(controlbuffer, eventflagstr);
     

So here we have it in the eventflag var.

Check now the test for the flag.


     	if (eventflag == ATRSHMLOGPOINTINTIMEp || eventflag == ATRSHMLOGPOINTINTIMEi)
     	  ucs2string = 1;
	


Ok. We see that conversion can be steared by the event flag - which
was the reason to have it in the first place.

Now we use it here to set the ucs2flag if the user (!) supported the
corresponding eventflags 'p' or 'i'.

The next things are same for all types of buffers. The header is
converted to the end.

Now we have again the ucs2string flag:


     	if (ucs2string)
     	  {
     	    convertucs2(nextcontrol, (totallen - ATRSHMLOGCONTROLDATASIZE) / 2, controlbuffer, i_fout);
     	  }
     	else
     	  {
     	    /* simple text convert, somehow dirty done */ 
     


Ok. Here we are.  If we had an eventflag for the 'p' or 'i', we go to the
convert of ucs2 string to output. 

If we have no 'p' or 'i' we go on with the text thing.

Making your own
---------------

You made a log in your systems write for the well known structure 'hugo'.

The definition of 'hugo' is left to your imagination. But you should at least
make a c program to test this thing standalone.

Now you have decided about the eventflag , its a for the hugo a 'h'.

By convention we use small characters for binary contents.

Copy the atrshmlogconvert.c to a new name.

Add the thing to the smballfiles and sbmbinfiles.

Add a new flag to the operate right after the ucs2string.

We will call it 'hugofound'.

       /* and now we can start with a realtime adder */
       
       while (akindex < i_read_len)
         {
           int chunklen;
           atrshmlog_time_t starttime;
           atrshmlog_time_t endtime;
           int event;
           char eventflag = 0;
           int ucs2string = 0;
           int hugofound = 0;
           int userflag;
	   

Ok. now we go for the test

     if (eventflag == ATRSHMLOGPOINTINTIMEp || eventflag == ATRSHMLOGPOINTINTIMEi)
       ucs2string = 1;

     if (eventflag == 'h')
       hugofound = 1;


Next we have to see for the final use.

     /* printf(" payoad %s\n", payloadbuffer); */

     if (ucs2string)
       {
         convertucs2(nextcontrol, (totallen - ATRSHMLOGCONTROLDATASIZE) / 2, controlbuffer, i_fout);
       }
     else if (hugofound)
       {
          /* found a hugo, now i have to do it */
          converthugo(nextcontrol, (totallen - ATRSHMLOGCONTROLDATASIZE), controlbuffer, i_fout);
       }
     else
       {
         /* simple text convert, somehow dirty done */ 


The definition of converthugo is clear for the prototype.

      static void converthugo(char *start, int size, char* prefix, FILE* i_fout);



The Implementation is - of course - up to you.

There is only a simple rule : If your hugo needs a multiline output
you should allways prefix it. This will give the text output the chance
to be used with the existing tool, namely atrshmlogsort and atrshmlogcalc.

If you plan to use only binary for the logging you can of course try to
do it diffrent, but this will cost at least some kind of effort then
for the tools to use the text files.

Output changes
--------------

For the output there is also the database thing.

The converter then does not use a file for output, it uses a database.
The interface is made in functions and a vanilla pointer type for the
context info the thing needs. So you can also put anything you want as
a backend under that converter.
Together with different types of conversions this make it possible to
make outputs to different systems and even to different types of targets.
In theory you can also implement a multi db approach here.

There is also a reader with an integrated convert so you can do the
transfer imemdiate in the reader if you insist.

Check the dbs directory for details.


Summary
-------

Writing a binary convert for your stuff is far from complex as long
as you have the thing under control. If you need different binary you
can use the eventflag to make the choice. Alternativ would be to use
a union with the flag of kind and a proper eventflag to do all kind of
transfers. For the downside of the union it has to use the right length.
But this is also not a problem. Use a struct for the length and then
the union and you are in.

So we have at least shown the path - you have to walk it if you need.

