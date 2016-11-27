
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_create_mapped_file.c
 *
 * test t_create_mapped_file.c
 */

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

int atrshmlog_create_mapped_file(int index, int size, int *already)
{
  // we create the mapped file for the index.
  // size is parameter.
  // so we give this as outut via env, and the others simply
  // use its to connect to the named file too

  // we first follow the usual thing, no file but pagefile space
  
  HANDLE hBackupFile = INVALID_HANDLE_VALUE;

  // default security, keep it simple
  LPSECURITY_ATTRIBUTES lSecurity = NULL;

  // protection modes
  DWORD dwProtection = PAGE_READWRITE;

  // size high dword
  DWORD dwSizeHigh = 0;

  // size low dword 
  DWORD dwSizeLow = size;

  // the name of the object 
  LPCTSTR lpName = atrshmlog_mapped_files[index];
  
  
  HANDLE hMapping = CreateFileMappingA(hBackupFile,
				    lSecurity,
				    dwProtection,
				    dwSizeHigh,
				    dwSizeLow,
				    lpName);

  if (hMapping == NULL)
    {
      return -1;
    }

  if( hMapping == INVALID_HANDLE_VALUE)
    {
      return -1;
    }

  int alreadythere = (ERROR_ALREADY_EXISTS == GetLastError());

  *already = alreadythere;

  if (alreadythere == 0)
    {
      SIZE_T szSize = size;

      void* p = (void*)MapViewOfFile(hMapping,
				     FILE_MAP_ALL_ACCESS,
				     0,
				     0,
				     szSize);

      if (p == NULL)
	{
	  CloseHandle(hMapping);
	  return -1;
	}
		    
    }

  // we do not unmap, so the thing will now reside. use it.
  
  return index;
}


void * atrshmlog_attach_mapped_file(int index, int size)
{

  HANDLE hMapping;

  DWORD dwAccess =  FILE_MAP_ALL_ACCESS;

  BOOL bInheritPossible = FALSE;

  LPCTSTR lpName = atrshmlog_mapped_files[index];


  SIZE_T szSize = size;

  hMapping = OpenFileMappingA(dwAccess,
			      bInheritPossible,
			      lpName);

#if ATRSHMLOGDEBUG == 1
  printf("%s %p %d %d\n", lpName, (void*)hMapping, index, size);
#endif
  
  if (hMapping == NULL)
    {
      return (void*)-1;
    }

  if( hMapping == INVALID_HANDLE_VALUE)
    {
      return (void*)-1;
    }

  void* p = (void*)MapViewOfFile(hMapping,
				 FILE_MAP_ALL_ACCESS,
				 0,
				 0,
				 szSize);


#if ATRSHMLOGDEBUG == 1
  printf("%p\n", p);
#endif
  
  if (p == NULL)
    {
      CloseHandle(hMapping);
      return  (void*)-1;
    }

  return p;
}


#endif

