
# the hello jni header create. we use the class file 
# the class file is in the subdirectory identified by package path.
# so we have to stay at the root or have to use -classpath
# we have to write to the includes folder 
javah -classpath ".:myjnipackage" -o hellojni.h.$$ HelloJNI

if [ $? -eq 0 ]
then
    mv hellojni.h.$$ includes/hellojni.h
fi


but this didnt work.


[javajni@hydra src]$ cat includes/hellojni.h 
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class HelloJNI */

#ifndef _Included_HelloJNI
#define _Included_HelloJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     HelloJNI
 * Method:    sayHello
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_HelloJNI_sayHello
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif

there is no package inside the name for the methode....

so we need to switch 