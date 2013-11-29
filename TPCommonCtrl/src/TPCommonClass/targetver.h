
#pragma once

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
//#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
//#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
//#endif
//
//#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
//#define _WIN32_WINNT 0x0501     // Change this to the appropriate value to target other versions of Windows.
//#endif
//
//#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif
//
//#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
//#define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
//#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0500		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0510 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define  _WIN32_IE 0x0600
#endif
