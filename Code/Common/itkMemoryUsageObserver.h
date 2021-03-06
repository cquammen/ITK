/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMemoryUsageObserver.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkMemoryUsageObserver_h
#define __itkMemoryUsageObserver_h

#include "itkConfigure.h"
#include "itkMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

#if defined( WIN32 ) || defined( _WIN32 )
  #include <windows.h>
  #define SUPPORT_TOOLHELP32
  #if defined( SUPPORT_TOOLHELP32 )
typedef LONG NTSTATUS;
  #endif
#endif

namespace itk
{
/** \class MemoryUsageObserver
 * \brief The MemoryUsageObserver provides the memory usage of the process
 *
 * This class represents a memory load analyser object
 * and provides a memory usage in platform-independent format.
 *
 */

class ITKCommon_EXPORT MemoryUsageObserverBase
{
public:
  /** Define the type for the memory usage */
  typedef unsigned long MemoryLoadType;

  /** destructor */
  virtual ~MemoryUsageObserverBase();

  /** Returns the memory load in kO */
  virtual MemoryLoadType GetMemoryUsage() = 0;
};

#if defined( WIN32 ) || defined( _WIN32 )
class ITKCommon_EXPORT WindowsMemoryUsageObserver:public MemoryUsageObserverBase
{
public:
  WindowsMemoryUsageObserver();
  /** destructor */
  virtual ~WindowsMemoryUsageObserver();

  /** Returns the memory load in kO */
  virtual MemoryLoadType GetMemoryUsage();

protected:
#if defined( SUPPORT_TOOLHELP32 )
  typedef NTSTATUS ( WINAPI * PZwQuerySystemInformation )(UINT, PVOID, ULONG, PULONG);

  // handle ntdll.dll library
  HMODULE m_hNTLib;
  // Windows native API function to query system information
  PZwQuerySystemInformation ZwQuerySystemInformation;
#endif // defined(SUPPORT_TOOLHELP32)
};
#endif // defined(WIN32) || defined(_WIN32)

#ifdef linux
class ITKCommon_EXPORT LinuxMemoryUsageObserver:public MemoryUsageObserverBase
{
public:
  /** destructor */
  virtual ~LinuxMemoryUsageObserver();
  virtual MemoryLoadType GetMemoryUsage();
};
#endif // linux

#if defined( __APPLE__ ) && MAC_OS_X_VERSION >= MAC_OS_X_VERSION_10_2
class ITKCommon_EXPORT MacOSXMemoryUsageObserver:public MemoryUsageObserverBase
{
public:
  /** destructor */
  virtual ~MacOSXMemoryUsageObserver();
  virtual MemoryLoadType GetMemoryUsage();
};
#endif // Mac OS X

#if defined( __SUNPRO_CC ) || defined ( __sun__ )
class ITKCommon_EXPORT SunSolarisMemoryUsageObserver:public MemoryUsageObserverBase
{
public:
  /** destructor */
  virtual ~SunSolarisMemoryUsageObserver();
  virtual MemoryLoadType GetMemoryUsage();
};
#endif // Sun Solaris

#if !defined( WIN32 ) && !defined( _WIN32 )
class ITKCommon_EXPORT SysResourceMemoryUsageObserver:public MemoryUsageObserverBase
{
public:
  /** destructor */
  virtual ~SysResourceMemoryUsageObserver();
  virtual MemoryLoadType GetMemoryUsage();
};
#if !defined( __APPLE__ ) && !defined( __SUNPRO_CC ) && !defined ( __sun__ ) && !defined( __FreeBSD__ )
/** \class MallinfoMemoryUsageObserver
 * \brief The MallinfoMemoryUsageObserver
 */
class ITKCommon_EXPORT MallinfoMemoryUsageObserver:public MemoryUsageObserverBase
{
public:
  /** destructor */
  virtual ~MallinfoMemoryUsageObserver();
  virtual MemoryLoadType GetMemoryUsage();
};
#endif // Mallinfo
#endif // !defined(WIN32) && !defined(_WIN32)

/* \class MemoryUsageObserver
 * The best MemoryUsageObserver has been chosen for each OS.
 * However, SysResourceMemoryUsageObserver is far from being accurate. Other
 * way of getting the Memory Usage shall be used.
 * For FreeBSD, some alternatives would be to parse the output of
 * "sysctl vm.vmtotal" or "sysctl -a | grep -i memory"
*/
class ITKCommon_EXPORT MemoryUsageObserver:
#if defined( WIN32 ) || defined( _WIN32 )
  public WindowsMemoryUsageObserver
#elif defined( linux )
  public LinuxMemoryUsageObserver
#elif defined( __SUNPRO_CC ) || defined ( __sun__ )
  public SunSolarisMemoryUsageObserver
#elif defined( __APPLE__ ) && MAC_OS_X_VERSION >= MAC_OS_X_VERSION_10_2
  public MacOSXMemoryUsageObserver
#elif defined( __APPLE__ ) || defined( __FreeBSD__ )
  public SysResourceMemoryUsageObserver
#else
  public MallinfoMemoryUsageObserver
#endif
{
public:
  /** destructor */
  virtual ~MemoryUsageObserver(){}
};
} // end of namespace itk

#endif  // __itkMemoryUsageObserver_h
