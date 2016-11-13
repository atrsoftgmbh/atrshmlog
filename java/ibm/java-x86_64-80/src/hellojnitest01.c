/* jni demo */

/** \file */

/* 
 * We declare the implementation of the native method here.
 * First we need the includes.
 */

#include <stdio.h>

#include <jni.h>

#include "hellojni.h"

/*
 * Now we can declare the needed implementation.
 * Its in Java, then in the package <empty>, then in class HelloJNI
 * and has name sayHello.
 * \param i_jnienv
 * The JNI environment object reference pointer.
 * \param i_myself
 * The calling object reference.
 *
 * \return
 * void
 */

JNIEXPORT /* linkage is this */
void /* return value type */
JNICALL /* unknown */
Java_HelloJNI_sayHello(JNIEnv* i_jnienv, jobject i_myself)
{
  printf("Hello, world.\n");

  return ;
}

/* end of file */
