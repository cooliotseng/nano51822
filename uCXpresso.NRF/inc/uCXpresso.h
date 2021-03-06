/*! \headerfile ucxpresso.h "uCXpresso.h"
 *
 */
#ifndef UCXPRESSO_H
#define UCXPRESSO_H

#define UCXPRESSO

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <cr_section_macros.h>

#ifdef __cplusplus
extern "C" {
#endif

// VERSION
#define RELEASED				255

/*! \cond PRIVATE */
#define uCXpresso_VER_MAJOR		1
#define uCXpresso_VER_MINOR		1
#define uCXpresso_VER_REV		2
#define uCXpresso_VER_RC		2
#define uCXpresso_VER_BUILD		"2016/7/29"
#define uCXpresso_VER_STR		"V1.1.2"

//
//	defined
//
#define DEFAULT_POOL_SIZE	(6000)

#if __GNUC__				// GCC
	#if __bool_true_false_are_defined==0
		typedef enum BOOLEAN
		{
			false = 0,
			true = !false
		}bool;
	#endif
//	#define TRUE	true
//	#define FALSE	false
//	#define BOOL	bool
//	#define __packed __attribute__ ((__packed__))
#define ALIGN4 __attribute__ ((aligned (4)))
#endif

typedef const char*			LPCTSTR;
typedef char*				LPTSTR;

/* These types must be 16-bit, 32-bit or larger integer */
typedef int					INT;
typedef unsigned int		UINT;

/* These types must be 8-bit integer  */
#ifndef CHAR
typedef char				CHAR;
#endif

#ifndef TCHAR
typedef char TCHAR;
#endif

typedef unsigned char		UCHAR;
typedef unsigned char		BYTE;

/* These types must be 16-bit integer */
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef unsigned short		WORD;
typedef unsigned short		WCHAR;

/* These types must be 32-bit integer */
typedef long				LONG;
typedef unsigned long		ULONG;
typedef unsigned long		DWORD;

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned long		dword;
typedef bool 				boolean;



/*! \cond PRIVATE */
/*! \union _u16_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u16_u
{
	uint16_t   Int;
	uint8_t	Char[2];
}u16_u;

/*! \union _u132_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u32_u
{
	uint32_t	Long;
	uint16_t	Int[2];
	uint8_t	Char[4];
}u32_u;

/*! \union _u64_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u64_u
{
	uint64_t	Int64;
    uint32_t   	Long[2];
    uint16_t   	Int[4];
	uint8_t		Char[8];
}u64_u;
/*! \endcond */

typedef void* 				xHandle;
#define null_str			((const char *)0)

//
// C++
//
#define PURE_VIRTUAL_FUNC	0
#define NO_EFFECT			{}
#define NOTHING

//
// Core
//
extern uint32_t MAIN_STACK_SIZE;
#define DEF_MAIN_STACK_SIZE		360
#define USB_MEM_BASE			0x20004000
#define USB_MEM_SIZE			0x7FC
extern void system_reset(void);
extern size_t heapAvailableSize(void);
extern void bootloader_start(void);
#define gotoDFU()	bootloader_start()

//
// RTOS
//
#define MAX_DELAY_TIME		0xffffffff
#define WAIT_FOR_ANY_BYTE	(MAX_DELAY_TIME-1)
extern void sleep(int ms);
extern void sysReboot(void);
extern uint32_t kernel_code_level();
extern uint32_t GetSystemTickCount();

//
// Bit Control
//
#define bit(x) 		(1<<(x))
#define bitmask(x) 	~bit(x)

#define bit_set(f,b)	f |= bit(b)
#define bit_clr(f,b)	f &= ~bit(b)
#define bit_chk(f,b)    ((f & bit(b))? true : false)

//
// pool memory functions
//
#ifndef _POOL_MEM_FUNCS_
#define _POOL_MEM_FUNCS_
extern void *pool_memcpy(void *dest, const void *sour, size_t size);
extern void *pool_memset(void *dest, int val, size_t size);
#endif

#define MEM_ALIGNMENT 4
#define MEM_SIZE_ALIGN(size)    	(((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT-1))    // align=4

extern void pool_memadd(uint32_t base, size_t size);
//#define memcpy(x,y,z)	xMemcpy(x,y,z)
//#define memset(x,y,z)	xMemset(x,y,z)
#define IS_FLASH_MEM(x)	(((uint32_t)x)<0x100000)	// Max flash memory 1MB

extern size_t heapAvailableSize();		// return the available size of heap memory
extern void *tryMalloc(size_t size);	// try to malloc a memory with

#define MEM_ALIGNMENT 4
#define MEM_SIZE_ALIGN(size)    	(((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT-1))    // align=4

//
// END OF PIN
//
#define END		0
#define NONE	0

//
// Miscellaneous
//
#define MHZ(x)				(x*1000000ul)
#define KHZ(x)				(x*1000ul)
//#define MAX(a, b)			(((a) > (b)) ? (a) : (b))
//#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define ABS(a)				(((a) < 0) ? -(a) : (a))
#define OFFSET(a, b, max)	(((a) >= (b)) ? (a-b):(max-b+a+1))		// offset

#define MAX_UINT32			0xffffffff
#define CRLF				"\r\n"

//#ifndef MSB
//#define MSB(a) (uint8_t)(((a) & 0xFF00) >> 8)
//#endif
//
//#ifndef LSB
//#define LSB(a) (uint8_t)((a) & 0xFF)
//#endif
#define MSB_FIRST(x)	((x>>8) & 0xFF), (x & 0xFF)

extern const unsigned char _zero_[];

//
// for GCC/G++ (newlib)
//
#ifndef __IMPORT
#define __IMPORT
#endif

#ifndef _READ_WRITE_RETURN_TYPE
#define _READ_WRITE_RETURN_TYPE _ssize_t
#endif

#ifdef __cplusplus
}
#endif

//
// ARDUINO marco Library
//
#define map(value, fromLow, fromHigh, toLow, toHigh)	((value-fromLow) * (toHigh-toLow) / (fromHigh-fromLow) + toLow)
#define constrain(x, a, b) 								((x<a) ? a : (x>b) ? b : x)

static void inline delay_us(uint32_t volatile number_of_us)
{
    do
    {
    __asm volatile (
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
        "NOP\n\t"
    );
    } while (--number_of_us);
}

#endif // NANO_H
