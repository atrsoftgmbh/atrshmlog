PROCESS : Implement the logging in your system.  {#process_implementlogging}
=============================================

You have build the module and now you want to log.

So you have to implement the logging in your target system.

We check for the process here in three examples.

The first is the already done test program atrshmlogtest00.

This helps to understand how the minimum is used in the writer.

Then we use the atrshmlogtee to see how we can use it in a bit more
complex system.

The last example is the MirBSD version of the korn shell, the so called
mksh. It was the initial target after i developed the module.

For a non C or C++ system we see in the language section what
must be done.

The atrshmlogtest00
-------------------

The minimal logging client.

You start with the classical hello world.

We make the usual commenting stuff. Then the include for the interface
header, atrshmlog.h.

We then need the output header for the printf, stdio.h .
Yes, we dont include this - its not needed in the interface header.

We then need the main function.

We print our output standard info in the printf.

We attach to the shared memory buffer.

We write to the log one string as payload, the event is number one, the
flag indicate its a point logging, the userevent is the argc value,
we dont support times.

Thats all. In code without comments:

     #include "atrshmlog.h"
     
     #include <stdio.h>
     
     int main (int argc, char *argv[])
     {
       const char hello[] = "hello, world.";
       
       printf("shm log attach %d and log : %s.\n", ATRSHMLOG_ATTACH(), hello);
     
       return ATRSHMLOG_WRITE(1, 'P', argc, 0, 0, hello, sizeof(hello) - 1);
     }

Only one more helper for the string, and two more lines of code.

The execute is simple, but we first need to make the buffer.

     $ atrshmlogcreate 4711 8 
     shm log create.
     logsystem version is 1.
     ipckey is 0X1267
     count of buffer is 8
     
     paste this into the user process environment startshell
     or into the profile or ENV file or use the dot file 
     
     
     ATRSHMLOG_ID="2424842"
     export ATRSHMLOG_ID
     
     if you have to use a login shell you can use the files instead.
     see atrshmlog_attach for this


The buffer must be initialized.

     $ atrshmloginit 8            
     shm log attach and init.
     logsystem version is 1.


The execute of the test

     $ atrshmlogtest00
     shm log attach 0 and log : hello, world..


The switch off for the logging.

     $ atrshmlogstopreader



The reader run to fetch the data.

     $ atrshmlogreaderd d1
     shm log attach and loop write file.
     logsystem version is 1.
     directory is d1
     files_per_dir is 10000
     count of initial used fetchers is 4
     count of initial used writes is 12
     logging done.
     writer data : time 930600  count 1  per use 930600


The convert of the data

     $ atrshmlogconv d1
     shm log converter from file 'd1/0/atrshmlog_p25766_t25766_f0.bin' to file 'd1/0/atrshmlog_p25766_t25766_f0.txt'.
     id    1 acquiretime     1380 pid  25766 tid    25766 slavetime      3120 readertime      7680 payloadsize      42 shmbuffer   0 filenumber       0 

And here it is.

     0000025766 0000000000025766 000 000000000000000000 000006704655212750 000006704655212750 000000000000000000 1475403105641391827 1475403105641391827 000000000000000000 0000000001 P 0000000001 hello, world.


We now can finish and delete the buffer, or we switch the logging on
again and re use it.

The atrshmlogtee
----------------

This is a bit more work because we try to make some things better.

The classical tee uses in its minmal for simply stdin and stdout.

    int main (it argc, char *argv[])
    {
	int ch;
        
        if (argc < 2) return -1;
	
        FILE* outfile = fopen(argv[1], "w");

        if (outfile == NULL) return -1;
	
        while ((ch = getchar()) != EOF) {
	    putchar(ch);
	    putc(ch, outfile);
	}
        
        fclose(outfile);
        
        return 0;
    }


So we use this here in a similar way. Only with lot more of checking.

First we get our shmid string and test it.

Then we check for a parameter.

The the attach is done.

We check if we are logging.

Then we start the loop.

Read in a part of the input, write out that part to the output.

We made the timing thing.

Now we can determine the evnt flag, and write out our log.

After this the loop starts again if all was ok.

If there was an io error we stop - thats normaly the case when input ended.

So the code is like this without comments and dense coding:

     int main (int argc, char*argv[])
     {
       const char *shmid_ptr = ATRSHMLOG_GET_ENVSHMID();
     
       int line = 0;   char flag;   ssize_t read_result;   ssize_t write_result;
     
       if (shmid_ptr == (void*)0)
         {
           fprintf(stderr, "usage: %s\n", argv[0]);
           fprintf(stderr, "ENVIRONMENT VARIABLE NOT SET !!!\n");
           fprintf(stderr, "%s_ID\n", ATRSHMLOG_GET_ENVPREFIX());
           printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
           exit(1);
         }
     
       if (argc != 1)
         {
           fprintf(stderr, "usage: %s\n", argv[0]);
           fprintf(stderr, "parameter count wrong.\n");
           exit(1);
         }
     
       ATRSHMLOG_ATTACH();
     
       if (!ATRSHMLOG_LOGGING)
         {
           fprintf (stderr, "attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
                "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENVPREFIX(), shmid_ptr);
     
           exit(1);
         }
     
     
       line = 0;
       
       do {
           atrshmlog_time_t starttime, endtime;
           char buffer [ CHUNKSIZE ];
           
           ATRSHMLOG_GETTIME(0,starttime);
           
           write_result = read_result = read(0, &buffer[0], CHUNKSIZE);
     
           if (read_result > 0)
             write_result = write(1, &buffer[0], read_result);
           
           ATRSHMLOG_GETTIME(0,endtime);
     
           ++line ;
     
           flag = 'N';
     
           if (read_result < CHUNKSIZE && write_result < CHUNKSIZE) flag = 'E';
     
           if (read_result < 0) flag = 'R';
     
           if (write_result < 0) flag = 'W';
     
           if (read_result > 0) {
               ATRSHMLOG_WRITE(0, flag, line, starttime, endtime, buffer, read_result);
             }
         } while (!(write_result <= 0 || read_result <= 0));
     
       return 0;
     }


This version uses a buffer and low level io, but its basically the same.
Most is for the handling of return codes, not the operations itself.

You execute and check this now.

     $ atrshmlogtee < atrshmlogtest00.c > outfile.c 
     $ atrshmlogoff
     shm log attach and set ich_habe_fertig to 1.
     logsystem version is 1.
     ich_habe_fertig was before 0
     $ diff atrshmlogtest00.c outfile.c
     $ 

And here is the generated log (had to add a space in the file and brief metas).

     0000026466 0000000000026466 001 000000000000000000 000016068085460840 000016068085548980 000000000000088140 1475406640835532846 1475406640835565700 000000000000032854 0000000000 N 0000000001 /***************************************
     0000026466 0000000000026466 001 000000000000000000 000016068085556280 000016068085567100 000000000000010820 1475406640835568421 1475406640835572454 000000000000004033 0000000000 N 0000000002 ********************************
     0000026466 0000000000026466 001 000000000000000000 000016068085556280 000016068085567100 000000000000010820 1475406640835568421 1475406640835572454 000000000000004033 0000000000 N 0000000002 *      
     0000026466 0000000000026466 001 000000000000000000 000016068085567670 000016068085573440 000000000000005770 1475406640835572666 1475406640835574817 000000000000002151 0000000000 N 0000000003        This software is part of the atrs
     0000026466 0000000000026466 001 000000000000000000 000016068085573680 000016068085579100 000000000000005420 1475406640835574907 1475406640835576927 000000000000002020 0000000000 N 0000000004 hmlog package           *
     0000026466 0000000000026466 001 000000000000000000 000016068085573680 000016068085579100 000000000000005420 1475406640835574907 1475406640835576927 000000000000002020 0000000000 N 0000000004 *             
     0000026466 0000000000026466 001 000000000000000000 000016068085579350 000016068085584810 000000000000005460 1475406640835577020 1475406640835579055 000000000000002035 0000000000 N 0000000005       Copyright (c) 2016  ATRSoft GmbH  
     0000026466 0000000000026466 001 000000000000000000 000016068085585060 000016068085590690 000000000000005630 1475406640835579149 1475406640835581247 000000000000002098 0000000000 N 0000000006                  *
     0000026466 0000000000026466 001 000000000000000000 000016068085585060 000016068085590690 000000000000005630 1475406640835579149 1475406640835581247 000000000000002098 0000000000 N 0000000006 *                    
     0000026466 0000000000026466 001 000000000000000000 000016068085590990 000016068085596350 000000000000005360 1475406640835581359 1475406640835583357 000000000000001998 0000000000 N 0000000007   and is licensed under the             
     0000026466 0000000000026466 001 000000000000000000 000016068085596560 000016068085601970 000000000000005410 1475406640835583435 1475406640835585452 000000000000002017 0000000000 N 0000000008           *
     0000026466 0000000000026466 001 000000000000000000 000016068085596560 000016068085601970 000000000000005410 1475406640835583435 1475406640835585452 000000000000002017 0000000000 N 0000000008 *                     Apache
     0000026466 0000000000026466 001 000000000000000000 000016068085602140 000016068085607350 000000000000005210 1475406640835585515 1475406640835587457 000000000000001942 0000000000 N 0000000009  License, Version 2.0                   
     0000026466 0000000000026466 001 000000000000000000 000016068085607560 000016068085613610 000000000000006050 1475406640835587535 1475406640835589790 000000000000002255 0000000000 N 0000000010    *
     0000026466 0000000000026466 001 000000000000000000 000016068085607560 000016068085613610 000000000000006050 1475406640835587535 1475406640835589790 000000000000002255 0000000000 N 0000000010 *                         by ATRSof
     0000026466 0000000000026466 001 000000000000000000 000016068085613840 000016068085619060 000000000000005220 1475406640835589876 1475406640835591822 000000000000001946 0000000000 N 0000000011 t GmbH                              *
     0000026466 0000000000026466 001 000000000000000000 000016068085613840 000016068085619060 000000000000005220 1475406640835589876 1475406640835591822 000000000000001946 0000000000 N 0000000011 * 
     0000026466 0000000000026466 001 000000000000000000 000016068085619300 000016068085624880 000000000000005580 1475406640835591911 1475406640835593991 000000000000002080 0000000000 N 0000000012                                         
     0000026466 0000000000026466 001 000000000000000000 000016068085625120 000016068085630310 000000000000005190 1475406640835594081 1475406640835596015 000000000000001934 0000000000 N 0000000013                              *
     0000026466 0000000000026466 001 000000000000000000 000016068085625120 000016068085630310 000000000000005190 1475406640835594081 1475406640835596015 000000000000001934 0000000000 N 0000000013 *        
     0000026466 0000000000026466 001 000000000000000000 000016068085630720 000016068085636180 000000000000005460 1475406640835596168 1475406640835598203 000000000000002035 0000000000 N 0000000014         A copy of the License is availab
     0000026466 0000000000026466 001 000000000000000000 000016068085636420 000016068085641630 000000000000005210 1475406640835598293 1475406640835600235 000000000000001942 0000000000 N 0000000015 le at                 *
     0000026466 0000000000026466 001 000000000000000000 000016068085636420 000016068085641630 000000000000005210 1475406640835598293 1475406640835600235 000000000000001942 0000000000 N 0000000015 *             ht
     0000026466 0000000000026466 001 000000000000000000 000016068085641850 000016068085647190 000000000000005340 1475406640835600317 1475406640835602307 000000000000001990 0000000000 N 0000000016 tp://www.apache.org/licenses/LICENSE-2.0
     0000026466 0000000000026466 001 000000000000000000 000016068085647370 000016068085653330 000000000000005960 1475406640835602374 1475406640835604596 000000000000002222 0000000000 N 0000000017                *
     0000026466 0000000000026466 001 000000000000000000 000016068085647370 000016068085653330 000000000000005960 1475406640835602374 1475406640835604596 000000000000002222 0000000000 N 0000000017 *                      
     0000026466 0000000000026466 001 000000000000000000 000016068085653580 000016068085659530 000000000000005950 1475406640835604689 1475406640835606907 000000000000002218 0000000000 N 0000000018                                         
     0000026466 0000000000026466 001 000000000000000000 000016068085659780 000016068085664920 000000000000005140 1475406640835607000 1475406640835608916 000000000000001916 0000000000 N 0000000019         *
     0000026466 0000000000026466 001 000000000000000000 000016068085659780 000016068085664920 000000000000005140 1475406640835607000 1475406640835608916 000000000000001916 0000000000 N 0000000019 *               Anton Rommersk
     0000026466 0000000000026466 001 000000000000000000 000016068085665170 000016068085670860 000000000000005690 1475406640835609009 1475406640835611130 000000000000002121 0000000000 N 0000000020 irchen <atr@atrsoft.de>                 
     0000026466 0000000000026466 001 000000000000000000 000016068085671080 000016068085676270 000000000000005190 1475406640835611212 1475406640835613147 000000000000001935 0000000000 N 0000000021  *
     0000026466 0000000000026466 001 000000000000000000 000016068085671080 000016068085676270 000000000000005190 1475406640835611212 1475406640835613147 000000000000001935 0000000000 N 0000000021 *                                    
     0000026466 0000000000026466 001 000000000000000000 000016068085676640 000016068085682280 000000000000005640 1475406640835613285 1475406640835615387 000000000000002102 0000000000 N 0000000022                                   *
     0000026466 0000000000026466 001 000000000000000000 000016068085676640 000016068085682280 000000000000005640 1475406640835613285 1475406640835615387 000000000000002102 0000000000 N 0000000022 ****
     0000026466 0000000000026466 001 000000000000000000 000016068085682510 000016068085687780 000000000000005270 1475406640835615473 1475406640835617437 000000000000001964 0000000000 N 0000000023 ****************************************
     0000026466 0000000000026466 001 000000000000000000 000016068085688010 000016068085693190 000000000000005180 1475406640835617523 1475406640835619454 000000000000001931 0000000000 N 0000000024 ***************************/
     0000026466 0000000000026466 001 000000000000000000 000016068085688010 000016068085693190 000000000000005180 1475406640835617523 1475406640835619454 000000000000001931 0000000000 N 0000000024 
     0000026466 0000000000026466 001 000000000000000000 000016068085688010 000016068085693190 000000000000005180 1475406640835617523 1475406640835619454 000000000000001931 0000000000 N 0000000024 /** \\ file 
     0000026466 0000000000026466 001 000000000000000000 000016068085693390 000016068085699040 000000000000005650 1475406640835619528 1475406640835621634 000000000000002106 0000000000 N 0000000025 atrshmlogtest00.c
     0000026466 0000000000026466 001 000000000000000000 000016068085693390 000016068085699040 000000000000005650 1475406640835619528 1475406640835621634 000000000000002106 0000000000 N 0000000025  * \\ brief WE are a min
     0000026466 0000000000026466 001 000000000000000000 000016068085699250 000016068085704340 000000000000005090 1475406640835621712 1475406640835623610 000000000000001898 0000000000 N 0000000026 imum test program.
     0000026466 0000000000026466 001 000000000000000000 000016068085699250 000016068085704340 000000000000005090 1475406640835621712 1475406640835623610 000000000000001898 0000000000 N 0000000026  */
     0000026466 0000000000026466 001 000000000000000000 000016068085699250 000016068085704340 000000000000005090 1475406640835621712 1475406640835623610 000000000000001898 0000000000 N 0000000026 
     0000026466 0000000000026466 001 000000000000000000 000016068085699250 000016068085704340 000000000000005090 1475406640835621712 1475406640835623610 000000000000001898 0000000000 N 0000000026 #include "atrshm
     0000026466 0000000000026466 001 000000000000000000 000016068085704610 000016068085709780 000000000000005170 1475406640835623710 1475406640835625637 000000000000001927 0000000000 N 0000000027 log.h"
     0000026466 0000000000026466 001 000000000000000000 000016068085704610 000016068085709780 000000000000005170 1475406640835623710 1475406640835625637 000000000000001927 0000000000 N 0000000027 
     0000026466 0000000000026466 001 000000000000000000 000016068085704610 000016068085709780 000000000000005170 1475406640835623710 1475406640835625637 000000000000001927 0000000000 N 0000000027 /*
     0000026466 0000000000026466 001 000000000000000000 000016068085704610 000016068085709780 000000000000005170 1475406640835623710 1475406640835625637 000000000000001927 0000000000 N 0000000027  * --------------------------
     0000026466 0000000000026466 001 000000000000000000 000016068085710020 000016068085715180 000000000000005160 1475406640835625727 1475406640835627650 000000000000001923 0000000000 N 0000000028 ----------------------------------
     0000026466 0000000000026466 001 000000000000000000 000016068085710020 000016068085715180 000000000000005160 1475406640835625727 1475406640835627650 000000000000001923 0000000000 N 0000000028  */
     0000026466 0000000000026466 001 000000000000000000 000016068085710020 000016068085715180 000000000000005160 1475406640835625727 1475406640835627650 000000000000001923 0000000000 N 0000000028 
     0000026466 0000000000026466 001 000000000000000000 000016068085710020 000016068085715180 000000000000005160 1475406640835625727 1475406640835627650 000000000000001923 0000000000 N 0000000028 
     0000026466 0000000000026466 001 000000000000000000 000016068085715410 000016068085720580 000000000000005170 1475406640835627736 1475406640835629663 000000000000001927 0000000000 N 0000000029 #include <stdio.h>
     0000026466 0000000000026466 001 000000000000000000 000016068085715410 000016068085720580 000000000000005170 1475406640835627736 1475406640835629663 000000000000001927 0000000000 N 0000000029 
     0000026466 0000000000026466 001 000000000000000000 000016068085715410 000016068085720580 000000000000005170 1475406640835627736 1475406640835629663 000000000000001927 0000000000 N 0000000029 
     0000026466 0000000000026466 001 000000000000000000 000016068085715410 000016068085720580 000000000000005170 1475406640835627736 1475406640835629663 000000000000001927 0000000000 N 0000000029 /**
     0000026466 0000000000026466 001 000000000000000000 000016068085715410 000016068085720580 000000000000005170 1475406640835627736 1475406640835629663 000000000000001927 0000000000 N 0000000029  * attach and m
     0000026466 0000000000026466 001 000000000000000000 000016068085720980 000016068085726080 000000000000005100 1475406640835629812 1475406640835631713 000000000000001901 0000000000 N 0000000030 ake some logging
     0000026466 0000000000026466 001 000000000000000000 000016068085720980 000016068085726080 000000000000005100 1475406640835629812 1475406640835631713 000000000000001901 0000000000 N 0000000030  *
     0000026466 0000000000026466 001 000000000000000000 000016068085720980 000016068085726080 000000000000005100 1475406640835629812 1475406640835631713 000000000000001901 0000000000 N 0000000030  * this is a testing
     0000026466 0000000000026466 001 000000000000000000 000016068085726330 000016068085731970 000000000000005640 1475406640835631806 1475406640835633909 000000000000002103 0000000000 N 0000000031  programm to check for write errors...
     0000026466 0000000000026466 001 000000000000000000 000016068085726330 000016068085731970 000000000000005640 1475406640835631806 1475406640835633909 000000000000002103 0000000000 N 0000000031  
     0000026466 0000000000026466 001 000000000000000000 000016068085732220 000016068085737320 000000000000005100 1475406640835634002 1475406640835635903 000000000000001901 0000000000 N 0000000032 * 
     0000026466 0000000000026466 001 000000000000000000 000016068085732220 000016068085737320 000000000000005100 1475406640835634002 1475406640835635903 000000000000001901 0000000000 N 0000000032  * simply see it and then try it...
     0000026466 0000000000026466 001 000000000000000000 000016068085732220 000016068085737320 000000000000005100 1475406640835634002 1475406640835635903 000000000000001901 0000000000 N 0000000032  
     0000026466 0000000000026466 001 000000000000000000 000016068085737490 000016068085743030 000000000000005540 1475406640835635966 1475406640835638031 000000000000002065 0000000000 N 0000000033 *
     0000026466 0000000000026466 001 000000000000000000 000016068085737490 000016068085743030 000000000000005540 1475406640835635966 1475406640835638031 000000000000002065 0000000000 N 0000000033  */
     0000026466 0000000000026466 001 000000000000000000 000016068085737490 000016068085743030 000000000000005540 1475406640835635966 1475406640835638031 000000000000002065 0000000000 N 0000000033 int main (int argc, char *argv[])
     0000026466 0000000000026466 001 000000000000000000 000016068085737490 000016068085743030 000000000000005540 1475406640835635966 1475406640835638031 000000000000002065 0000000000 N 0000000033 
     0000026466 0000000000026466 001 000000000000000000 000016068085754050 000016068085760770 000000000000006720 1475406640835642139 1475406640835644644 000000000000002505 0000000000 N 0000000034 {
     0000026466 0000000000026466 001 000000000000000000 000016068085754050 000016068085760770 000000000000006720 1475406640835642139 1475406640835644644 000000000000002505 0000000000 N 0000000034   const char hello[] = "hello, world."
     0000026466 0000000000026466 001 000000000000000000 000016068085761070 000016068085766360 000000000000005290 1475406640835644756 1475406640835646727 000000000000001971 0000000000 N 0000000035 ;
     0000026466 0000000000026466 001 000000000000000000 000016068085761070 000016068085766360 000000000000005290 1475406640835644756 1475406640835646727 000000000000001971 0000000000 N 0000000035   
     0000026466 0000000000026466 001 000000000000000000 000016068085761070 000016068085766360 000000000000005290 1475406640835644756 1475406640835646727 000000000000001971 0000000000 N 0000000035   printf("shm log attach %d and log
     0000026466 0000000000026466 001 000000000000000000 000016068085766610 000016068085771810 000000000000005200 1475406640835646821 1475406640835648759 000000000000001938 0000000000 N 0000000036  : %s.\n", ATRSHMLOG_ATTACH(), hello);
     0000026466 0000000000026466 001 000000000000000000 000016068085766610 000016068085771810 000000000000005200 1475406640835646821 1475406640835648759 000000000000001938 0000000000 N 0000000036 
     0000026466 0000000000026466 001 000000000000000000 000016068085766610 000016068085771810 000000000000005200 1475406640835646821 1475406640835648759 000000000000001938 0000000000 N 0000000036 
     0000026466 0000000000026466 001 000000000000000000 000016068085772030 000016068085777180 000000000000005150 1475406640835648841 1475406640835650760 000000000000001919 0000000000 N 0000000037   return ATRSHMLOG_WRITE(1, 'P', argc, 0
     0000026466 0000000000026466 001 000000000000000000 000016068085777560 000016068085782670 000000000000005110 1475406640835650902 1475406640835652807 000000000000001905 0000000000 N 0000000038 , 0, hello, sizeof(hello) - 1);
     0000026466 0000000000026466 001 000000000000000000 000016068085777560 000016068085782670 000000000000005110 1475406640835650902 1475406640835652807 000000000000001905 0000000000 N 0000000038 }
     0000026466 0000000000026466 001 000000000000000000 000016068085777560 000016068085782670 000000000000005110 1475406640835650902 1475406640835652807 000000000000001905 0000000000 N 0000000038 
     0000026466 0000000000026466 001 000000000000000000 000016068085777560 000016068085782670 000000000000005110 1475406640835650902 1475406640835652807 000000000000001905 0000000000 N 0000000038 /* en
     0000026466 0000000000026466 001 000000000000000000 000016068085782880 000016068085789670 000000000000006790 1475406640835652885 1475406640835655416 000000000000002531 0000000000 E 0000000039 d of file */
     0000026466 0000000000026466 001 000000000000000000 000016068085782880 000016068085789670 000000000000006790 1475406640835652885 1475406640835655416 000000000000002531 0000000000 E 0000000039 
     

If you use a cut -c191 and the see for the result its this:

     
     /***************************************
      ********************************
      *      
             This software is part of the atrs
      hmlog package           *
      *             
            Copyright (c) 2016  ATRSoft GmbH  
                       *
      *                    
        and is licensed under the             
                *
      *                     Apache
       License, Version 2.0                   
         *
      *                         by ATRSof
      t GmbH                              *
      * 
                                              
                                   *
      *        
              A copy of the License is availab
      le at                 *
      *             ht
      tp://www.apache.org/licenses/LICENSE-2.0
                     *
      *                      
                                              
              *
      *               Anton Rommersk
      irchen <atr@atrsoft.de>                 
       *
      *                                    
                                        *
      ****
      ****************************************
      ***************************/
      
      /** \\ file 
      atrshmlogtest00.c
       * \\ brief WE are a min
      imum test program.
       */
      
      #include "atrshm
      log.h"
      
      /*
       * --------------------------
      ----------------------------------
       */
      
      
      #include <stdio.h>
      
      
      /**
       * attach and m
      ake some logging
       *
       * this is a testing
       programm to check for write errors...
       
      * 
       * simply see it and then try it...
       
      *
       */
      int main (int argc, char *argv[])
      
      {
        const char hello[] = "hello, world."
      ;
        
        printf("shm log attach %d and log
       : %s.\n", ATRSHMLOG_ATTACH(), hello);
      
      
        return ATRSHMLOG_WRITE(1, 'P', argc, 0
      , 0, hello, sizeof(hello) - 1);
      }
      
      /* en
      d of file */


Compare this to the original source code.


The MirBSD korn shell
---------------------

We have here a real live program.

The shell was invented by Stephen Bourne for the first unix system
at bell labs. It was a first approch and there came many
inventions to it. Then the csh from Bill Joy came from Berkley
and the ksh from David Korn from AT&T.

The korn shell became the defacto standard then. Every Unix system
I worked in 1995 had it implemented, and even a kind of copy was running
on fenster;plural with the mks toolkit.

For the korn shell there is a small but very conservative group of
users. They abandon the use of other shells and so the lately invented
Bourne again Shell - bash - did not get all shares in the shell game.

The thing needed at that time an open source port, but first AT&T
didnt support this. The community created the pd ksh (Public domain
korn shell). It was a poor and sometimes bad crashing hack, but at
last it worked good enough to use it as a replacement for the
non open source ksh. Better than nothing.

Then the MirBSD selected this for its primary shell and started to invent
the mksh on top of the pdksh stuff. Today the ksh is available as an
open source program from Lucent. Its the real David Korn that maintained
it till 2012, but support from Lucent is at best
broken. So many people of that conservative user group switched to
the mksh for the better support.

This mksh is build from source. And its the target of this
logging implementation.

I took the 52b version at that time.

I dont plan to use this in any other way now, so i simply deliver this
52b version of the Mir Kornshell as an example how to do a big
implement of the log.

We will not discuss all changes made for the source.

Many changes have to do with the fact that you need a line number in the log.

Its a fact you want to see what happens, and you want to see it with the info
what line of input did it. So there were several additions only to get
the log with a line number ( at least with a plus minus one correct one).

For the log itself it was only one aditional define in the headers that
was added - ATRSHMLOGENTRYSIZELIMIT, at this time 256.

The code is in the mksh/mksh.logging folder.

We will not discuss the way the mksh is made, or it is build. We only
check for the changes here.

You find here two folders. On is the original code base in mksh.52b.

You find in the other folder mksh.logging the latest version of the
logging shell code.

So you can make a simple diff -r to find the differences.

###Build the shell

You have to set some compiler flags. This is done by setting via dot.build
them.

Then you execute the Build.sh with the system shell.

You execute this so

     $ . dot.build
     $ /bin/sh Build.sh

The output is a lot and i made one into output.build.20161002_1 for
you to check if yours is right.

Now for the way it was done some info.

###Implementing the module as an include

After checking for the build process i decided to keep things simple.

No use of the lib, no hard linking object file.

I included the module instead in the main driver.

Open the main.c and check for the last lines.

     /* atr */
     
     #include "atrshmlog.c"
     
     /* atr */


Ok, this wasnt so bad at all. Its only somehow unusual to include a
.c file. But it works, i have my module as a separate file there,
and can even live with a include via default directories.
UPDATE: That is the old style for now, will be changed in the next iteration to use of the lib.

The files that use the log have this way the module in place in the
main.o for linking.

###Implement the attach

Again in main.c we have the include for atrshmlog.h.

In function main then the stuff to use the modue


     int
     main(int argc, const char *argv[])
     {
     	int rv;
     	Source *s;
     	struct block *l;
     
     	atrshmlog_time_t starttime;
     	atrshmlog_time_t endtime;
     	atrshmlog_time_t lastendtime;
     
     
     	ATRSHMLOG_ATTACH();
     
     	ATRSHMLOG_GETTIME(0,starttime);
     

The following code is for tests of timings, so i spare you this.

When the shell finished her initialization it scanns the given
script - if any - and executes.

The execute is in exec.c, so
open this file and check for the function execute.

You will find that the execute is somehow done recursiv on base
of a tree representation that comes out of the scanner.

There is a big switch which makes nearly everything transparent
to the user what happens:

     
     	switch (t->type) {
     	case TCOM:
     		rv = comexec(t, tp, (const char **)ap, flags, xerrok);
     
     		ATRSHMLOG_GETTIME(204,endtime);
     		{
     		  if (ap[0])
     		    {
     		      int len = strlen(ap[0]);
     		      
     		  ATRSHMLOG_WRITE(204, 'I',
     			      atrshmlogline,
     			      starttime,
     			      endtime,
     			      ap[0],
     			      len < ATRSHMLOGENTRYSIZELIMIT ? len : ATRSHMLOGENTRYSIZELIMIT
     			      );
     		    }
     		  else 
     		    ATRSHMLOG_WRITE(204, 'I',
     			      atrshmlogline,
     			      starttime,
     			      endtime,
     				"unknown",
     				sizeof("unknown") - 1
     			      );
     		}
     		
     		break;
     
     	case TPAREN:
     		rv = execute(t->left, flags | XFORK, xerrok);
     
     		ATRSHMLOG_GETTIME(205,endtime);
     		{
     		    ATRSHMLOG_WRITE(205, 'I',
     			       atrshmlogline,
     			      starttime,
     			      endtime,
     				"(", /* ) */
     				sizeof("(") - 1 /* ) */
     				);
     		}
     		break;
     
     	case TPIPE:
     		flags |= XFORK;
     		flags &= ~XEXEC;


I spare you again the rest.

So you can check for the remaining c files for yourself. The most do
simply the gettime and then log thing.

There is a version of the calc script in this folder that is made with
the knowledge of the implementation in the mksh.
It gives better statistics for the search of problems ...

For the full story behind the mksh see the doc.

Summary
-------

We have checked for the simplest, a usual and one big system that uses the
logging and checked the way it was implemented.

So you should now understand how to do this in C and C++ source code
based systems.

For the rest of the world you have to check for the implementation in
use of the proper layer technique.

For the implementation in a C system i have used a somehow unusual way
by including the module .c file. Thats ok for me, i thik its the
simplest way and have the least impact to the used build system.

There is still one problem with it - you have to compile the thing
with the compiler settings for c11.

If you cannot do this - for the build or for the source - you have
to try the use of the linked module and the library instead.



