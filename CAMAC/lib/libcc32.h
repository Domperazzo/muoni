#ifndef __CC32LIB_H__
#define __CC32LIB_H__
//****************************************************************************
// Copyright (C) 2000-2004  ARW Elektronik Germany
//
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// This product is not authorized for use as critical component in 
// life support systems without the express written approval of 
// ARW Elektronik Germany.
//  
// Please announce changes and hints to ARW Elektronik
//
// Maintainer(s): Klaus Hitschler (klaus.hitschler@gmx.de)
//
//****************************************************************************

//****************************************************************************
//
// cc32lib.h -- header for a simple access library for the 
//                                     PCICC32 PCI to CAMAC Interface
//
// $Log: libcc32.h,v $
// Revision 1.9  2005/10/03 17:46:31  klaus
// simple cosmetic changes, upgrade lib to libcc32.so.3.0.0
//
// Revision 1.8  2004/11/29 20:43:12  klaus
// added _qx functions to get Q and X for every transfer. Release libcc32.so.2.
//
// Revision 1.7  2004/08/13 19:48:25  klaus
// changed license from GPL to LGPL
//
// Revision 1.6  2004/08/12 20:00:41  klaus
// conversion to kernel-version 2.6, released version 6.0
//
// Revision 1.5  2003/05/29 08:29:08  klaus
// added _cplusplus extention
//
// Revision 1.4  2002/04/17 19:41:06  klaus
// added support for autoread
//
// Revision 1.3  2002/04/14 18:25:38  klaus
// added interrupt handling, driver 4.4. ...3.5.tar.gz
//
// Revision 1.2  2001/11/20 20:12:50  klaus
// included new header and CVS log
//
//****************************************************************************

/*--- INCLUDES --------------------------------------------------------------------------*/
#include <asm/types.h>

/*--- TYPEDEFS --------------------------------------------------------------------------*/
typedef void* CC32_HANDLE;  /* type of the device handle */

/*--- DEFINES ---------------------------------------------------------------------------*/

/*--- PROTOTYPES ------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/* open a path to a device. E.g. "/dev/pcicc32_1" */
int   cc32_open(char *cszPath, CC32_HANDLE *handle);

/* close the opened path */
int   cc32_close(CC32_HANDLE handle); 

/* read only a word - 16 bits - from a address made out of N,A,F */
__u16 cc32_read_word(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F);

/* read only a word - 16 bits - from a address made out of N,A,F, get Q and X */
__u16 cc32_read_word_qx(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F, int *Q, int *X);

/* read a long - 32 bits - from a address made out of N,A,F and get the result Q and X */
__u32 cc32_read_long(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F);

/* read a long - 32 bits - from a address made out of N,A,F get Q and X */
__u32 cc32_read_long_qx(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F, int *Q, int *X);

/* read a long - 32 bits - without any interpretaion */
__u32 cc32_read_long_all(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F);

/* write a word - 16 bits - to a destination made out of N,A,F */
void  cc32_write_word(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F, __u16 uwData);

/* write a word - 16 bits - to a destination made out of N,A,F, get Q and X */
void  cc32_write_word_qx(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F, __u16 uwData, int *Q, int *X);

/* write a long - 32 bits - uninterpreted to a destination made out of N,A,F */
void  cc32_write_long(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F, __u32 ulData);

/* write a long - 32 bits - uninterpreted to a destination made out of N,A,F, get Q and X */
void  cc32_write_long_qx(CC32_HANDLE handle, unsigned int N, unsigned int A, unsigned int F, __u32 ulData, int *Q, int *X);

/* poll the state of the timeout line and the LAM state. The timeout line is cleared if it was set */
int   cc32_poll_event(CC32_HANDLE handle, int *nTimeout, int *nLam);

/* control interrupts caused by timeouts or LAMs */
int   cc32_interrupt_disable(CC32_HANDLE handle);
int   cc32_interrupt_enable(CC32_HANDLE handle);

/* same as cc32_poll_event(), but wait for a state change of timeout or LAMs. */
int   cc32_wait_event(CC32_HANDLE handle, int *nTimeout, int *nLam);

/* switch cc32 autoread on or off - return the last switch state */
int   cc32_autoread_on(CC32_HANDLE handle);
int   cc32_autoread_off(CC32_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif /* __CC32LIB_H__ */
