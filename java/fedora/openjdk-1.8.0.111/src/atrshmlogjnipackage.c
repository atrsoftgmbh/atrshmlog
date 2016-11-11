/* jni demo */

/** \file atrshmlogjnipackage.c
 * \brief We implement the jni stubs.
*/

/* 
 * We declare the implementation of the native method here.
 * First we need the includes.
 */

#include <string.h>

#include <jni.h>

/* this is now in the includes subdirectory */
#include "includes/atrshmlogjni.h"

/* this is for our helper module to log */

#include "includes/atrshmlog.h"

/* this is for the adminstative functions */

#include "includes/atrshmlog_internal.h"

/**********************************************************************/

/**
 * \brief  local type for makeing conversions easier 
 */
union u_conv { volatile const void* p;
  jlong l;
};

typedef union u_conv u_t;

/**********************************************************************/

/**
 * \brief we attach
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    attach
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_attach
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_ATTACH();

  return result;
}

/**
 * \brief we get the time in clicks
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    gettime
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_gettime
  (JNIEnv *i_jnienv, jobject i_myself, jint i_event)
{
  jlong result;

  ATRSHMLOG_GETTIME(i_event, result);

  return result;
}

/**
 * \brief we getthe time in clicks. static version
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    sgettime
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_sgettime
  (JNIEnv *i_jnienv, jclass i_myself, jint i_event)
{
  jlong result;

  ATRSHMLOG_GETTIME(i_event, result);

  return result;
}


/**
 * \brief we write a technical log entry 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    write
 * Signature: (IIIJJ)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_write__IIIJJ
  (JNIEnv *i_jnienv, jobject i_myself,
   jint i_userevent,
   jint i_eventflag,
   jint i_userflag,
   jlong i_starttime,
   jlong i_endtime)
{
  jint result;

  result = ATRSHMLOG_WRITE0(i_userevent,
			    (char)i_eventflag,
			    i_userflag,
			    i_starttime,
			    i_endtime);

  return result;
}

/**
 * \brief we write a string log entry
 *
 * you can choose between a UTF converted and a Ucs2 
 * with the eventflag.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    write
 * Signature: (IIIJJLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_write__IIIJJLjava_lang_String_2
 (JNIEnv *i_jnienv, jobject i_myself,
   jint i_userevent,
   jint i_eventflag,
   jint i_userflag,
   jlong i_starttime,
   jlong i_endtime,
   jstring i_payload)
{
  jint result;

  jboolean is_copy = JNI_FALSE;

  int length = 0;

  static const char* zerozero = "\0\0";

  static const char * zero = "";

  if (i_eventflag == de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_EVENT_POINT_IN_TIME_C
      || i_eventflag == de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_EVENT_INTERVAL_IN_TIME_C)
    { // we want an utf version
  
      const char *inCStr = (*i_jnienv)->GetStringUTFChars(i_jnienv, i_payload, &is_copy);
  
      const char *payload = inCStr;
  
      if (inCStr == NULL) {
	jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
	if (flag) {
	  (*i_jnienv)->ExceptionClear(i_jnienv);
	}
    
	payload = zero;
      } else {
	length = strlen(payload);
      }
  
      result = ATRSHMLOG_WRITE(i_userevent,
			       (char)i_eventflag,
			       i_userflag,
			       i_starttime,
			       i_endtime,
			       payload,
			       length);

      if (inCStr != NULL)
	(*i_jnienv)->ReleaseStringUTFChars(i_jnienv, i_payload, inCStr);  // release resources
      
    }
  else
    { // we are in an ucs2 version for java
      jsize len = (*i_jnienv)->GetStringLength(i_jnienv, i_payload);

      const jchar * the_chars = (*i_jnienv)->GetStringCritical(i_jnienv, i_payload, &is_copy);

      const jchar * payload = the_chars;
      
      if (the_chars == NULL)
	{
	  jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
	  if (flag) {
	    (*i_jnienv)->ExceptionClear(i_jnienv);
	  }
      
	  payload = (const jchar * )zerozero;
	}
      else
	{
	  length = len; // uni code chars, not bytes.
	}
      
      result = ATRSHMLOG_WRITE(i_userevent,
			       (char)i_eventflag,
			       i_userflag,
			       i_starttime,
			       i_endtime,
			       payload,
			       (length * 2));

      if (the_chars != NULL)
	(*i_jnienv)->ReleaseStringCritical(i_jnienv, i_payload, the_chars);
      
    }
  
  return result;
}

/**
 * \brief we write a lenght limited ucs2 string
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    write
 * Signature: (IIIJJLjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_write__IIIJJLjava_lang_String_2I
 (JNIEnv *i_jnienv, jobject i_myself,
   jint i_userevent,
   jint i_eventflag,
   jint i_userflag,
   jlong i_starttime,
   jlong i_endtime,
   jstring i_payload,
   jint i_payloadlength)
{
  jint result;

  jboolean is_copy = JNI_FALSE;

  int length = i_payloadlength;

  static const char* zerozero = "\0\0";

  jsize len = (*i_jnienv)->GetStringLength(i_jnienv, i_payload);
  
  const jchar * the_chars = (*i_jnienv)->GetStringCritical(i_jnienv, i_payload, &is_copy);

  const jchar * payload = the_chars;

  if (the_chars == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      
      payload = (const jchar * )zerozero;
      length = 0;
    }
  else
    {
      if (len < length)
	length = len;
    }
  
  if (i_eventflag == de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_EVENT_POINT_IN_TIME_C
      || i_eventflag == de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_EVENT_INTERVAL_IN_TIME_C)
    { // we have to change. we need the lower case chars here. we insist that its only i and p here after
      i_eventflag = i_eventflag + 32;
    }
  else 
    { // this is ok
      ; // nothing to do
    }
  
  result = ATRSHMLOG_WRITE(i_userevent,
			   (char)i_eventflag,
			   i_userflag,
			   i_starttime,
			   i_endtime,
			   payload,
			   (length * 2));

  if (the_chars != NULL)
    (*i_jnienv)->ReleaseStringCritical(i_jnienv, i_payload, the_chars);
  

  return result;
}


/**
 * \brief we write a byte array
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    write
 * Signature: (IIIJJ[B)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_write__IIIJJ_3B
  (JNIEnv *i_jnienv, jobject i_myself,
   jint i_userevent,
   jint i_eventflag,
   jint i_userflag,
   jlong i_starttime,
   jlong i_endtime,
   jbyteArray i_payload)
{
  jint result;

  jboolean is_copy = JNI_FALSE;

  jsize length = (*i_jnienv)->GetArrayLength(i_jnienv, i_payload);
  
  jbyte* the_bytes = (jbyte*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, i_payload, &is_copy);

  jbyte* payload = the_bytes;

  int len = length;

  if (the_bytes == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      
      payload = "";
      len = 0;
    }
  
  result = ATRSHMLOG_WRITE(i_userevent,
			   (char)i_eventflag,
			   i_userflag,
			   i_starttime,
			   i_endtime,
			   payload,
			   len);


  
  if (the_bytes != NULL)
    (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, JNI_ABORT); // yes. its here abort -  we will NOT copy back ...

  return result;
}




/**
 * \brief we get the highest index of the statistics
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getStatisticsMaxIndex
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getStatisticsMaxIndex
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_STATISTICS_MAX_INDEX();

  return result;
}


/**
 * \brief we get the statistics values
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getStatistics
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getStatistics
  (JNIEnv *i_jnienv, jobject i_myself,
   jintArray i_payload)
{
  jint result;
  
  jboolean is_copy = JNI_FALSE;

  int needed = ATRSHMLOG_GET_STATISTICS_MAX_INDEX();
  
  jsize length = (*i_jnienv)->GetArrayLength(i_jnienv, i_payload);
  
  jint* the_bytes = (jint*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, i_payload, &is_copy);

  int len = length;

  if (the_bytes == NULL) 
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      
      result = -1;
      
    }
  else if (needed + 1 > length)
    {
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, JNI_ABORT); // no copy back, only free is

      result = -1;
    }
  else
    {
      ATRSHMLOG_GET_STATISTICS((atrshmlog_int32_t*)the_bytes);

      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, 0); // we commit and copy back

      result = 0;
    }
  
  return result;
}

/**
 * \brief we sleep for nanos 
 *
 * this can be an approximation if the system does not have it.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    sleepNanos
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_sleepNanos
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  ATRSHMLOG_SLEEP_NANOS(i_switch);

  return;
}

/**
 * \brief we set the environment variable prefix
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setEnvPrefix
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setEnvPrefix
  (JNIEnv * i_jnienv, jobject i_myself,
   jstring i_payload)
{
  jboolean is_copy = JNI_FALSE;

  jsize len = 0;

  static const char * zero = "";
  
  const char *inCStr = (*i_jnienv)->GetStringUTFChars(i_jnienv, i_payload, &is_copy);

  const char *payload = inCStr;
  
  if (inCStr == NULL) {
    jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
    if (flag) {
      (*i_jnienv)->ExceptionClear(i_jnienv);
    }
      
    payload = zero;
    len = 0;
  } else {
    len = strlen(payload);
  }

  ATRSHMLOG_SET_ENV_PREFIX(payload);

  if (inCStr != NULL)
    (*i_jnienv)->ReleaseStringUTFChars(i_jnienv, i_payload, inCStr);  // release resources

  return ;
}


/**
 * \brief get the environment variable prefix
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getEnvPrefix
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getEnvPrefix
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jstring result;

  const char* p = ATRSHMLOG_GET_ENV_PREFIX();
  
  result = (*i_jnienv)->NewStringUTF(i_jnienv, p);

  /* we do not handel the execption. it must be done in 
   * the java code 
   */
  
  return result;
}

/**
 * \brief get an environment variable with prefix and suffix
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getEnv
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getEnv
(JNIEnv *i_jnienv, jobject i_myself, jstring i_suffix)
{
  jstring result;

  jboolean is_copy = JNI_FALSE;

  jsize len = 0;

  static const char* zero = "";
  
  const char *inCStr = (*i_jnienv)->GetStringUTFChars(i_jnienv, i_suffix, &is_copy);

  const char *payload = inCStr;
  
  if (inCStr == NULL) {
    jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
    if (flag) {
      (*i_jnienv)->ExceptionClear(i_jnienv);
    }
      
    payload = zero;
    len = 0;
  } else {
    len = strlen(payload);
  }
  
  const char* p = ATRSHMLOG_GET_ENV(payload);
  
  result = (*i_jnienv)->NewStringUTF(i_jnienv, p);

  /* we do not handle the exception. this must be done in java code
   */

  return result;
}

/**
 * \brief get the shmid value from environment
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getEnvShmid
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getEnvShmid
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jstring result;

  const char* p = ATRSHMLOG_GET_ENV_SHMID();
  
  result = (*i_jnienv)->NewStringUTF(i_jnienv, p);

  /* we do not handle the exception. this must be done in java code
   */

  return result;
}

/**
 * \brief get the suffix of the shmid variable name
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getEnvIdSuffix
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getEnvIdSuffix
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jstring result;

  const char* p = ATRSHMLOG_GET_ENV_ID_SUFFIX();
  
  result = (*i_jnienv)->NewStringUTF(i_jnienv, p);

  /* we do not handle the exception. this must be done in java code
   */

  return result;
}


/**
 * \brief get the version of the c module
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getVersion
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_VERSION();

  return result;
}

/**
 * \brief get the minor version of the c module
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getMinorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getMinorVersion
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_MINOR_VERSION();

  return result;
}


/**
 * \brief get the patch version of the c module
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getPatchVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getPatchVersion
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_PATCH_VERSION();

  return result;
}


/**
 * \brief get the max event number
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getEventLocksMax
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getEventLocksMax
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_EVENT_LOCKS_MAX();

  return result;
}


/**
 * \brief  set the new max event number
 *
 * this can trigger a dynamic memory allocation.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setEventLocksMax
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setEventLocksMax
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_EVENT_LOCKS_MAX(i_switch);

  return result;
}

/**
 * \brief get an event value
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getEvent
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getEvent
   (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_GET_EVENT(i_switch);

  return result;
}

/**
 * \brief set an event to on
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setEventOn
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setEventOn
   (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_EVENT_ON(i_switch);

  return result;
}


/**
 * \brief set an event to off
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setEventOff
 * Signature: (I)C
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setEventOff
   (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_EVENT_OFF(i_switch);

  return result;
}


/**
 * \brief get the logging state of the module
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getLogging
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getLogging
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_LOGGING();

  return result;
}

/**
 * \brief set the logging for process to on
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setLoggingProcessOn
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setLoggingProcessOn
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_LOGGING_PROCESS_ON();

  return result;
}


/**
 * \brief set the logging for the process to off
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setLoggingProcessOff
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setLoggingProcessOff
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_LOGGING_PROCESS_OFF();

  return result;
}


/**
 * \brief  set the logging final off switch
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setLoggingProcessOffFinal
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setLoggingProcessOffFinal
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_LOGGING_PROCESS_OFF_FINAL();

  return result;
}


/**
 * \brief get the shared memory id
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getShmid
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getShmid
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_SHMID();

  return result;
}

/**
 * \brief get the area start adress
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getArea
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getArea
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jlong result;

  u_t u;

  u.p = ATRSHMLOG_GET_AREA();

  result = u.l;
  
  return result;
}

/**
 * \brief get the system wide logging switch
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getAreaIchHabeFertig
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getAreaIchHabeFertig
(JNIEnv *i_jnienv, jobject i_myself, jlong i_area)
{
  jint result;

  u_t u;

  u.l = i_area;

  result = ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(u.p);
  
  return result;
}

/**
 * \brief set the system wide logging switch
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setAreaIchHabeFertig
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setAreaIchHabeFertig
(JNIEnv *i_jnienv, jobject i_myself, jlong i_area, jint i_switch)
{
  jint result;

  u_t u;

  u.l = i_area;

  result = ATRSHMLOG_SET_AREA_ICH_HABE_FERTIG(u.p, i_switch);
  
  return result;
}

/**
 * \brief get the count of log buffers from the shared memory
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getAreaCount
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getAreaCount
(JNIEnv *i_jnienv, jobject i_myself, jlong i_area)
{
  jint result;

  u_t u;

  u.l = i_area;

  result = ATRSHMLOG_GET_AREA_COUNT(u.p);
  
  return result;
}

/**
 * \brief get the version of the shared memory area
 *
 * This must be the same as the c module version or working is
 * halted
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getAreaVersion
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getAreaVersion
(JNIEnv *i_jnienv, jobject i_myself, jlong i_area)
{
  jint result;

  u_t u;

  u.l = i_area;

  result = ATRSHMLOG_GET_AREA_VERSION(u.p);
  
  return result;
}

/**
 * \brief get the max size of logging buffers
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getBufferMaxSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getBufferMaxSize
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_BUFFER_MAX_SIZE();

  return result;
}


/**
 * \brief get the actual size of logging buffers
 *
 * This is for dynamic allocated buffers only of interest.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getBufferSize
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_BUFFER_SIZE();

  return result;
}


/**
 * \brief set the buffer seize.
 *
 * This is for dynamic allocated buffers only of interest.
 * And it can only reduce size . 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setBufferSize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setBufferSize
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_BUFFER_SIZE(i_switch);

  return result;
}


/**
 * \brief get the number of slaves to start in attach or fork clone
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getFListBufferSlaveCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getFListBufferSlaveCount
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_COUNT();

  return result;
}

/**
 * \brief set the number of slaves to start in attach or fork clone
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setFListBufferSlaveCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setFListBufferSlaveCount
   (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_COUNT(i_switch);

  return result;
}

 
/**
 * \brief get the clock id
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getClockId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getClockId
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_CLOCK_ID();

  return result;
}



/**
 * \brief set the clock id
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setClockId
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setClockId
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_CLOCK_ID(i_switch);

  return result;
}


/**
 * \brief switch all slave threads to exit after next loop
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setFListBufferSlaveRunOff
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setFListBufferSlaveRunOff
  (JNIEnv *i_jnienv, jobject i_myself)
{
  ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_RUN_OFF();
  
  return ;
}

/**
 * \brief set the wait flag for the cleanup to on for waiting for
 * all slaves to finish before cleanup.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setWaitForSlavesOn
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setWaitForSlavesOn
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_WAIT_FOR_SLAVES_ON();

  return result;
}


/**
 * \brief  set the wait for slaves flag in cleanup to off
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setWaitForSlavesOff
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setWaitForSlavesOff
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_WAIT_FOR_SLAVES_OFF();

  return result;
}


/**
 * \brief get the flag wait for slaves for the cleanup
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getWaitForSlaves
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getWaitForSlaves
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_WAIT_FOR_SLAVES();

  return result;
}

/**
 * \brief get the slave wait time
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getFListBufferSlaveWait
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getFListBufferSlaveWait
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_WAIT();

  return result;
}


/**
 *  \brief set the slave wait time
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setFListBufferSlaveWait
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setFListBufferSlaveWait
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_WAIT(i_switch);

  return result;
}


/**
 * \brief get the number of acquired buffers for the logging
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getAcquireCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getAcquireCount
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_ACQUIRE_COUNT();

  return result;
}

/**
 * \brief get the number for preallocation of buffers in dynamic memory
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getPreallocBufferCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getPreallocBufferCount
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_PREALLOC_BUFFER_COUNT();

  return result;
}



/**
 * \brief set the number of buffers to allocate in dynamic alloc at once
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setPreallocBufferCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setPreallocBufferCount
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_PREALLOC_BUFFER_COUNT(i_switch);

  return result;
}


/**
 * \brief get the module init time
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getInittime
 * Signature: ([J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getInittime
  (JNIEnv *i_jnienv, jobject i_myself,
   jlongArray i_payload)
{
  jint result;
  
  jboolean is_copy = JNI_FALSE;


  jsize length = (*i_jnienv)->GetArrayLength(i_jnienv, i_payload);
  
  jlong* the_bytes = (jlong*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, i_payload, &is_copy);

  int len = length;

  
  if (the_bytes == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }

      result = -1;
    }
  else if (length < 2)
    {
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, JNI_ABORT); // we do not copy back

      result = -1;
    }
  else
    {
      atrshmlog_internal_time_t t = ATRSHMLOG_GET_INITTIME();
      
      the_bytes[0] = t.tv_nsec;
      the_bytes[1] = t.tv_sec;

      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, 0); // we copy back

      result = 0;
    }
  
  return result;
}

/**
 * \brief get the click before inittime
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getInittimeTscBefore
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getInittimeTscBefore
(JNIEnv *i_jnienv, jobject i_myself)
{
  jlong result;

  result = ATRSHMLOG_GET_INITTIME_TSC_BEFORE();
  
  return result;
}

/**
 * \brief get the click after inittime
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getInittimeTscAfter
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getInittimeTscAfter
(JNIEnv *i_jnienv, jobject i_myself)
{
  jlong result;

  result = ATRSHMLOG_GET_INITTIME_TSC_AFTER();
  
  return result;
}

/**
 * \brief get the highest buffer id so far given.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getBufferId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getBufferId
   (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_BUFFER_ID();

  return result;
}

 

/**
 * \brief stop logging for the thread that executes this 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_stop
  (JNIEnv *i_jnienv, jobject i_myself)
{
  ATRSHMLOG_STOP();
  
  return;
}

/**
 * \brief flush buffers for the thread that executes this 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    flush
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_flush
  (JNIEnv *i_jnienv, jobject i_myself)
{
  ATRSHMLOG_FLUSH();
  
  return;
}

/**
 * \brief set the thread strategy for the wait on buffers
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setStrategy
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setStrategy
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_STRATEGY(i_switch);

  return result;
}

/**
 * \brief We get the strategy for this thread
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getStrategy
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getStrategy
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_STRATEGY();

  return result;
}


/**
 * \brief set the process default strategy for wait on buffers
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setStrategyProcess
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setStrategyProcess
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_STRATEGY_PROCESS(i_switch);

  return result;
}

/**
 * \brief We get the strategy for the process
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getStrategyProcess
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getStrategyProcess
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_STRATEGY_PROCESS();

  return result;
}


/**
 * \brief create a new slave thread
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    createSlave
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_createSlave
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_CREATE_SLAVE();

  return result;
}

/**
 * \brief decrement the slave count.
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    decrementSlaveCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_decrementSlaveCount
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_DECREMENT_SLAVE_COUNT();

  return result;
}



/**
 * \brief switch the fence 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence1
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence1
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_1(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence1
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence1
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_1();

  return result;
}


/**
 * \brief switch the fence 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence2
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence2
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_2(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence2
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence2
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_2();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence3
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence3
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_3(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence3
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence3
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_3();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence4
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence4
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_4(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence4
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence4
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_4();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence5
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence5
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_5(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence5
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence5
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_5();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence6
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence6
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_6(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence6
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence6
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_6();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence7
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence7
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_7(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence7
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence7
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_7();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence8
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence8
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_8(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence8
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence8
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_8();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence9
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence9
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_9(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence9
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence9
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_9();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence10
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence10
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_10(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence10
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence10
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_10();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence11
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence11
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_11(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence11
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence11
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_11();

  return result;
}


/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence12
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence12
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_12(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence12
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence12
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_12();

  return result;
}



/**
 * \brief switch the fence 
 *

 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setThreadFence13
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setThreadFence13
  (JNIEnv *i_jnienv, jobject i_myself, jint i_switch)
{
  jint result;

  result = ATRSHMLOG_SET_THREAD_FENCE_13(i_switch);

  return result;
}

/**
 * \brief We get a fence on or off flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadFence13
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadFence13
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_THREAD_FENCE_13();

  return result;
}


/**
 * \brief get the real time 
 *
 * This can be an approximation
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getRealtime
 * Signature: ([J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getRealtime
  (JNIEnv *i_jnienv, jobject i_myself,
   jlongArray i_payload)
{
  jint result;
  
  jboolean is_copy = JNI_FALSE;


  jsize length = (*i_jnienv)->GetArrayLength(i_jnienv, i_payload);
  
  jlong* the_bytes = (jlong*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, i_payload, &is_copy);

  int len = length;

  
  if (the_bytes == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      result = -1;
    }
  else if (length < 2)
    {
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, JNI_ABORT); // we do not copy back
      /* we do nothing */
      result = -1;
    }
  else
    {
      atrshmlog_internal_time_t t = ATRSHMLOG_GET_REALTIME();
      
      the_bytes[0] = t.tv_nsec;
      the_bytes[1] = t.tv_sec;

      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, i_payload, the_bytes, 0); // we copy back

      result = 0;
    }
  
  return result;
}

/**
 * \brief We get the thread locals adress of a thread
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadLocalsAdress
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadLocalsAdress
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jlong result;

  u_t u;

  u.p = ATRSHMLOG_GET_THREAD_LOCALS_ADRESS();

  result = u.l;
  
  return result;
}


/**
  * \brief We get the thread tid 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getTid
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getTid
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jlong result;

  result = ATRSHMLOG_GET_TID();
  
  return result;
}


/**
 * \brief We switch the thread off and dispatch its buffers
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    turnLoggingOff
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_turnLoggingOff
(JNIEnv *i_jnienv, jobject i_myself, jlong i_thread_locals)
{
  u_t u;

  u.l = i_thread_locals;

  ATRSHMLOG_TURN_LOGGING_OFF(u.p);
  
  return;
}

/**
 * \brief we turn the init in advance on
 * 
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setInitBuffersInAdvanceOn
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setInitBuffersInAdvanceOn
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_ON();
  
  return result;
}


/**
 * \brief we turn the init in advance off
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    setInitBuffersInAdvanceOff
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_setInitBuffersInAdvanceOff
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_OFF();
  
  return result;
}

/**
 * \brief we get the init in advance flag
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getInitBuffersInAdvance
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getInitBuffersInAdvance
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_GET_INIT_BUFFERS_IN_ADVANCE();
  
  return result;
}

/**
 * \brief We get the next element on slave list
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getNextSlaveLocal
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getNextSlaveLocal
(JNIEnv *i_jnienv, jobject i_myself, jlong i_thread_locals)
{
  jlong result;
  
  u_t u;

  u.l = i_thread_locals;

  u.p = ATRSHMLOG_GET_NEXT_SLAVE_LOCAL(u.p);

  result = u.l;
  
  return result;
}

/**
 * \brief We get the tid of a thread local
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getThreadLocalTid
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getThreadLocalTid
(JNIEnv *i_jnienv, jobject i_myself, jlong i_thread_locals)
{
  jlong result;
  
  u_t u;

  u.l = i_thread_locals;

  result = ATRSHMLOG_GET_THREAD_LOCAL_TID(u.p);

  return result;
}

/**
 * \brief We remove the slave from the list of slaves
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    removeSlaveViaLocal
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_removeSlaveViaLocal
(JNIEnv *i_jnienv, jobject i_myself, jlong i_thread_locals)
{
  jint result;
  
  u_t u;

  u.l = i_thread_locals;

  result = ATRSHMLOG_REMOVE_SLAVE_VIA_LOCAL(u.p);

  return result;
}

/**
 * \brief We make reuse of buffers of a dead thread
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    reuseThreadBuffers
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_reuseThreadBuffers
(JNIEnv *i_jnienv, jobject i_myself, jlong i_tid)
{
  jint result;
  
  result = ATRSHMLOG_REUSE_THREAD_BUFFERS(i_tid);

  return result;
}


/**
 * \brief verify the shared memory is intact
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    verify
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_verify
  (JNIEnv *i_jnienv, jobject i_myself)
{
  jint result;

  result = ATRSHMLOG_VERIFY();

  return result;
}


/**
 * \brief get the clicktime from the clock with the set id
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    getClicktime
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_getClicktime
(JNIEnv *i_jnienv, jobject i_myself)
{
  jlong result;

  result = ATRSHMLOG_GET_CLICKTIME();
  
  return result;
}

/**
 * \brief read a log buffer from the area
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    read
 * Signature: (JI[B[J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_read
(JNIEnv *i_jnienv, jobject i_myself,
 jlong i_area,
 jint i_buffer_index,
 jbyteArray o_target,
 jlongArray o_params)
{
  jint result;
  jboolean is_copy = JNI_FALSE;

  u_t u;

  u.l = i_area;

  jsize length_target = (*i_jnienv)->GetArrayLength(i_jnienv, o_target);
  jsize length_params = (*i_jnienv)->GetArrayLength(i_jnienv, o_params);

  if (length_target < ATRSHMLOG_GET_BUFFER_MAX_SIZE()) {
    return -1;
  }
  
  if (length_params < 15) {
    return -2;
  }
  
  jbyte* the_bytes = (jbyte*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, o_target, &is_copy);

  if (the_bytes == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      
      return -3;
    }

  jlong* the_params = (jlong*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, o_params, &is_copy);
  
  if (the_params == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, JNI_ABORT); // we do not copy back
 
      return -4;
    }

  atrshmlog_int32_t o_length;

  atrshmlog_pid_t o_pid;

  atrshmlog_tid_t o_tid;

  atrshmlog_internal_time_t o_inittime;

  atrshmlog_time_t o_inittsc_before;
	      
  atrshmlog_time_t o_inittsc_after;
	      
  atrshmlog_internal_time_t o_lasttime;
	      
  atrshmlog_time_t o_lasttsc_before;
	      
  atrshmlog_time_t o_lasttsc_after;

  atrshmlog_time_t o_difftimetransfer;

  atrshmlog_time_t o_starttransfer;

  atrshmlog_time_t o_acquiretime;

  atrshmlog_int32_t o_id;

  atrshmlog_int32_t o_number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;


  result = ATRSHMLOG_READ(u.p,
			  i_buffer_index,
			  the_bytes,
			  &o_length,
			  &o_pid,
			  &o_tid,
			  &o_inittime,
			  &o_inittsc_before,
			  &o_inittsc_after,
			  &o_lasttime,
			  &o_lasttsc_before,
			  &o_lasttsc_after,
			  &o_difftimetransfer,
			  &o_starttransfer,
			  &o_acquiretime,
			  &o_id,
			  &o_number_dispatched,
			  &counter_write0,
			  &counter_write0_discard,
			  &counter_write0_wait,
			  &counter_write0_adaptive,
			  &counter_write0_adaptive_fast,
			  &counter_write0_adaptive_very_fast,
			  &counter_write1,
			  &counter_write1_discard,
			  &counter_write1_wait,
			  &counter_write1_adaptive,
			  &counter_write1_adaptive_fast,
			  &counter_write1_adaptive_very_fast,
			  &counter_write2,
			  &counter_write2_discard,
			  &counter_write2_wait,
			  &counter_write2_adaptive,
			  &counter_write2_adaptive_fast,
			  &counter_write2_adaptive_very_fast);


  if (result >= 0)
    {
      if (o_length > 0)
	{
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, 0);  // we copy back

	  the_params[0] = o_length;
	  the_params[1] = o_pid;
	  the_params[2] = o_tid;
	  the_params[3] = o_inittime.tv_nsec;
	  the_params[4] = o_inittime.tv_sec;
	  the_params[5] = o_inittsc_before;
	  the_params[6] = o_inittsc_after;
	  the_params[7] = o_lasttime.tv_nsec;
	  the_params[8] = o_lasttime.tv_sec;
	  the_params[9] = o_lasttsc_before;
	  the_params[10] = o_lasttsc_after;
	  the_params[11] = o_difftimetransfer;
	  the_params[12] = o_starttransfer;
	  the_params[13] = o_acquiretime;
	  the_params[14] = o_id;
  
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_params, the_params, 0);
	}
      else
	{
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, JNI_ABORT);  // we do not copy back
	  
	  the_params[0] = 0;
  
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_params, the_params, 0); // we copy the length back, rest is not relevant
	}
    }
  else
    {
      // bad thing. even o_length is of no help
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, JNI_ABORT);  // we do not copy back
	  
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_params, the_params, JNI_ABORT); // we do not copy the length back
    }
  
  return result;
}

/**
 * \brief read a buffer from the shared area via full list
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    readFetch
 * Signature: (J[B[J)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_readFetch
(JNIEnv *i_jnienv, jobject i_myself,
 jlong i_area,
 jbyteArray o_target,
 jlongArray o_params)
{
  jint result;
  jboolean is_copy = JNI_FALSE;

  u_t u;

  u.l = i_area;

  jsize length_target = (*i_jnienv)->GetArrayLength(i_jnienv, o_target);
  jsize length_params = (*i_jnienv)->GetArrayLength(i_jnienv, o_params);

  if (length_target < ATRSHMLOG_GET_BUFFER_MAX_SIZE()) {
    return -1;
  }
  
  if (length_params < (16 + 1 + 18)) {
    return -2;
  }
  
  jbyte* the_bytes = (jbyte*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, o_target, &is_copy);

  if (the_bytes == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }
      
      return -3;
    }

  jlong* the_params = (jlong*)(*i_jnienv)->GetPrimitiveArrayCritical(i_jnienv, o_params, &is_copy);

  if (the_params == NULL)
    {
      jboolean flag = (*i_jnienv)->ExceptionCheck(i_jnienv);
      if (flag) {
	(*i_jnienv)->ExceptionClear(i_jnienv);
      }

      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, JNI_ABORT); // we do not copy back
 
      return -4;
    }

  atrshmlog_int32_t o_length;

  atrshmlog_pid_t o_pid;

  atrshmlog_tid_t o_tid;

  atrshmlog_internal_time_t o_inittime;

  atrshmlog_time_t o_inittsc_before;
	      
  atrshmlog_time_t o_inittsc_after;
	      
  atrshmlog_internal_time_t o_lasttime;
	      
  atrshmlog_time_t o_lasttsc_before;
	      
  atrshmlog_time_t o_lasttsc_after;

  atrshmlog_time_t o_difftimetransfer;

  atrshmlog_time_t o_starttransfer;

  atrshmlog_time_t o_acquiretime;

  atrshmlog_int32_t o_id;

  atrshmlog_int32_t o_number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;


  atrshmlog_int32_t o_buffer_index;
  
  result = ATRSHMLOG_READ_FETCH(u.p,
				&o_buffer_index,
				the_bytes,
				&o_length,
				&o_pid,
				&o_tid,
				&o_inittime,
				&o_inittsc_before,
				&o_inittsc_after,
				&o_lasttime,
				&o_lasttsc_before,
				&o_lasttsc_after,
				&o_difftimetransfer,
				&o_starttransfer,
				&o_acquiretime,
				&o_id,
				&o_number_dispatched,
				&counter_write0,
				&counter_write0_discard,
				&counter_write0_wait,
				&counter_write0_adaptive,
				&counter_write0_adaptive_fast,
				&counter_write0_adaptive_very_fast,
				&counter_write1,
				&counter_write1_discard,
				&counter_write1_wait,
				&counter_write1_adaptive,
				&counter_write1_adaptive_fast,
				&counter_write1_adaptive_very_fast,
				&counter_write2,
				&counter_write2_discard,
				&counter_write2_wait,
				&counter_write2_adaptive,
				&counter_write2_adaptive_fast,
				&counter_write2_adaptive_very_fast);

  if (result >= 0)
    {
      if (o_length > 0)
	{
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, 0);  // we copy back
	  the_params[0] = o_buffer_index;
	  the_params[1] = o_length;
	  the_params[2] = o_pid;
	  the_params[3] = o_tid;
	  the_params[4] = o_inittime.tv_nsec;
	  the_params[5] = o_inittime.tv_sec;
	  the_params[6] = o_inittsc_before;
	  the_params[7] = o_inittsc_after;
	  the_params[8] = o_lasttime.tv_nsec;
	  the_params[9] = o_lasttime.tv_sec;
	  the_params[10] = o_lasttsc_before;
	  the_params[11] = o_lasttsc_after;
	  the_params[12] = o_difftimetransfer;
	  the_params[13] = o_starttransfer;
	  the_params[14] = o_acquiretime;
	  the_params[15] = o_id;
	  the_params[16] = o_number_dispatched;
	  the_params[17] = counter_write0;
	  the_params[18] = counter_write0_discard;
	  the_params[19] = counter_write0_wait;
	  the_params[20] = counter_write0_adaptive;
	  the_params[21] = counter_write0_adaptive_fast;
	  the_params[22] = counter_write0_adaptive_very_fast;
	  the_params[23] = counter_write1;
	  the_params[24] = counter_write1_discard;
	  the_params[25] = counter_write1_wait;
	  the_params[26] = counter_write1_adaptive;
	  the_params[27] = counter_write1_adaptive_fast;
	  the_params[28] = counter_write1_adaptive_very_fast;
	  the_params[29] = counter_write2;
	  the_params[30] = counter_write2_discard;
	  the_params[31] = counter_write2_wait;
	  the_params[32] = counter_write2_adaptive;
	  the_params[33] = counter_write2_adaptive_fast;
	  the_params[34] = counter_write2_adaptive_very_fast;


  	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_params, the_params, 0);
  	}
      else
	{
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, JNI_ABORT);  // we do not copy back
	  
	  the_params[0] = 0;
  
	  (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_params, the_params, 0); // we copy the length back, rest is not relevant
	}
    }
  else
    {
      // bad thing. even o_length is of no help
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_target, the_bytes, JNI_ABORT);  // we do not copy back
	  
      (*i_jnienv)->ReleasePrimitiveArrayCritical(i_jnienv, o_params, the_params, JNI_ABORT); // we do not copy the length back
    }

  return result;
}

/**
 * \brief create a shared memory buffer for logging
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    create
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_create
(JNIEnv *i_jnienv, jobject i_myself, jint i_key, jint i_count)
{
  jint result;

  result = ATRSHMLOG_CREATE(i_key, i_count);
  
  return result;
}

/**
 * \brief delete a buffer for shared memory 
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    delete
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_delete
  (JNIEnv *i_jnienv, jobject i_myself, jint i_shmid)
{
  jint result;

  result = ATRSHMLOG_DELETE(i_shmid);
  
  return result;
}

/**
 * \brief clean the locks for a area
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    cleanupLocks
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_cleanupLocks
(JNIEnv *i_jnienv, jobject i_myself, jlong i_area)
{
  u_t u;

  u.l = i_area;

  ATRSHMLOG_CLEANUP_LOCKS(u.p);

  return;
}

/**
 * \brief  init a shared memory buffer to be useable as logging area
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    initShmLog
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_initShmLog
  (JNIEnv *i_jnienv, jobject i_myself, jlong i_area, jint i_count)
{
  jint result;

  u_t u;

  u.l = i_area;

  result = ATRSHMLOG_INIT_SHM_LOG((void*)u.p, i_count);
  
  return result;
}

/**
 * \brief get a byte from the area
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    peek
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_peek
  (JNIEnv *i_jnienv, jobject i_myself, jlong i_area, jint i_offset)
{
  jint result;

  u_t u;

  u.l = i_area;

  volatile const unsigned char* p = u.p;
  
  p += i_offset;
  
  result = *p;

  return result;
}

/**
 * \brief set a byte in the area
 *
 * Class:     de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG
 * Method:    poke
 * Signature: (JIB)I
 */
JNIEXPORT jint JNICALL Java_de_atrsoft_successorofoak_utilities_logging_atrshmlog_ATRSHMLOG_poke

  (JNIEnv *i_jnienv, jobject i_myself, jlong i_area, jint i_offset, jbyte i_value)
{
  jint result;

  u_t u;

  u.l = i_area;

  unsigned char* p = (unsigned char*)u.p;
  
  p += i_offset;
  
  result = *p;

  *p = i_value;

  return result;
}



/* end of file */
