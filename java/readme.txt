hello !

We have here the following directory structure.

- bin
The little helper scripts default versions

- src
The sources that are the base of the jni bridge.
These are the java class fiels and the c bridge code file.
The rest is made by the java tools.

- oracle
Vendor tree for oracle - former sun - jdk

There are two jdk for now, one is a bit older 1.8.0 66
which worked for my box as the installed default jdk.

The other jdk is a 1.8.0. 102 which was at the time I
made the cygwin and mingw ports the windows version
default.

- fedora
Vendor tree for the fedora openjdk.
It was added after the oracle versions and ran out of the box.
So the open source hard liner can ignore the commercials now.

- ibm
Vendor tree for the ibm jdk.
Its a 8.0 version. So you are i relative new territory for
the guys with the big pockedbooks ...
This is definetely a port you cannot ignore for commercial
users, some tool of the ibm simply need this jdk for
running - even if it were for the licensing stuff...

The vendor trees are for now identical in layout.

You find the bin directory with an adapted dot.java.sh file.

The doc contains a helpful intro to jni for the new guys.

In the src you find again the bin scripts. This makes it a bit
easier to play with them when you adapt a new platform....
Ok, its a bit oversized, but works for me. We have here the
source for the bridge and we have the includes directory for the
h files.

To start quick you cange to BASEDIR/java, execute a bin/getfrommain.sh
to transfer the formerly generated headers and libs to the vendor trees
and then change in the src for your target. Source the dot.java.sh
and then hit the create_jni_lib.sh.

The rest is in the documentation.

If you make a new vendor tree simply clone one and adjust the scripts.
For the java and jni files there should be no diffrence - at least
in theory...
