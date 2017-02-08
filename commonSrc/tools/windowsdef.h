#ifndef WINDOWSDEF_H_INCLUDED
#define WINDOWSDEF_H_INCLUDED



typedef float f32;
typedef double f64;

typedef signed long long s64;
typedef signed int  s32;
typedef signed short s16;
typedef signed char  s8;

typedef signed long long const sc64;
typedef signed int  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long long vs64;
typedef volatile signed int  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long long  const vsc64;
typedef volatile signed int  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long long u64;
typedef unsigned int  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned long long const uc64;
typedef unsigned int  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long long  vu64;
typedef volatile unsigned int  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long long const vuc64;  /* Read Only */
typedef volatile unsigned int  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */


/** Returns the sign of X as "1" or "-1" */
template <typename T>
inline int sign(T x) { return x < 0 ? -1 : 1; }

#ifndef M_PI
#define  M_PI  3.1415926
#endif
#ifndef M_2PI
#define  M_2PI  3.1415926*2
#endif

#if NAVIPACK_WIN
#include <windows.h>
#else

#include <unistd.h>

/* Defines ------------------------------------------------------------------*/
#ifndef __stdcall
#define __stdcall
#endif

//#define NULL    0

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define  INVALID_SOCKET -1
#define INVALID_HANDLE_VALUE	-1
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#define STATUS_WAIT_0       ((DWORD   )0x00000000L) 
#define WAIT_OBJECT_0       ((STATUS_WAIT_0 ) + 0 )


//typedef signed long bool;

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

#ifndef		NULL
#define		NULL 0
#endif

#define Sleep(n)     usleep(n*1000)

#define ASSERT_ assert

typedef int HANDLE;
typedef unsigned int DWORD;
typedef int WORD;
typedef unsigned char BYTE;
typedef int BOOL;
typedef unsigned long long UINT64;
typedef void* LPVOID;

/* Includes ------------------------------------------------------------------*/
typedef struct _DCB   // dcb
{
    DWORD DCBlength;           // sizeof(DCB)
    DWORD BaudRate;            // current baud rate 指定当前的波特率
    DWORD fBinary: 1;          // binary mode, no EOF check 指定是否允许二进制模式，WINDOWS 95中必须为TRUE
    DWORD fParity: 1;          // enable parity checking 指定奇偶校验是否允许
    DWORD fOutxCtsFlow:1;      // CTS output flow control 指定CTS是否用于检测发送控制.当为TRUE是CTS为OFF，发送将被挂起
    DWORD fOutxDsrFlow:1;      // DSR output flow control 指定DSR是否用于检测发送控制.当为TRUE是DSR为OFF，发送将被挂起
    DWORD fDtrControl:2;       // DTR flow control type DTR_CONTROL_DISABLE值将DTR置为OFF, DTR_CONTROL_ENABLE值将DTR置为ON, DTR_CONTROL_HANDSHAKE允许DTR"握手",
    DWORD fDsrSensitivity:1;   // DSR sensitivity 当该值为TRUE时DSR为OFF时接收的字节被忽略
    DWORD fTXContinueOnXoff:1; // XOFF continues Tx 指定当接收缓冲区已满,并且驱动程序已经发送出XoffChar字符时发送是否停止.TRUE时，在接收缓冲区接收到缓冲区已满的字节XoffLim且驱动程序已经发送出XoffChar字符中止接收字节之后，发送继续进行。FALSE时，在接收缓冲区接收到代表缓冲区已空的字节XonChar且驱动程序已经发送出恢复发送的XonChar之后，发送继续进行。
    DWORD fOutX: 1;            // XON/XOFF out flow control TRUE时，接收到XoffChar之后便停止发送.接收到XonChar之后将重新开始
    DWORD fInX: 1;             // XON/XOFF in flow control TRUE时，接收缓冲区接收到代表缓冲区满的XoffLim之后，XoffChar发送出去.接收缓冲区接收到代表缓冲区空的XonLim之后，XonChar发送出去
    DWORD fErrorChar: 1;       // enable error replacement 该值为TRUE且fParity为TRUE时，用ErrorChar 成员指定的字符代替奇偶校验错误的接收字符
    DWORD fNull: 1;            // enable null stripping TRUE时，接收时去掉空（0值）字节
    DWORD fRtsControl:2;       // RTS flow control RTS_CONTROL_DISABLE时,RTS置为OFF RTS_CONTROL_ENABLE时, RTS置为ON RTS_CONTROL_HANDSHAKE时,当接收缓冲区小于半满时RTS为ON 当接收缓冲区超过四分之三满时RTS为OFF RTS_CONTROL_TOGGLE时,当接收缓冲区仍有剩余字节时RTS为ON ,否则缺省为OFF
    DWORD fAbortOnError:1;     // abort reads/writes on error TRUE时,有错误发生时中止读和写操作
    DWORD fDummy2:17;          // reserved 未使用
    WORD wReserved;            // not currently used 未使用,必须为0
    WORD XonLim;               // transmit XON threshold 指定在XON字符发送这前接收缓冲区中可允许的最小字节数
    WORD XoffLim;              // transmit XOFF threshold 指定在XOFF字符发送这前接收缓冲区中可允许的最小字节数
    BYTE ByteSize;             // number of bits/byte, 4-8 指定端口当前使用的数据位
    BYTE Parity;               // 0-4=no,odd,even,mark,space 指定端口当前使用的奇偶校验方法,可能为:EVENPARITY,MARKPARITY,NOPARITY,ODDPARITY
    BYTE StopBits;             // 0,1,2 = 1, 1.5, 2 指定端口当前使用的停止位数,可能为:ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS
    char XonChar;              // Tx and Rx XON character 指定用于发送和接收字符XON的值
    char XoffChar;             // Tx and Rx XOFF character 指定用于发送和接收字符XOFF值
    char ErrorChar;            // error replacement character 本字符用来代替接收到的奇偶校验发生错误时的值
    char EofChar;              // end of input character 当没有使用二进制模式时,本字符可用来指示数据的结束
    char EvtChar;              // received event character 当接收到此字符时,会产生一个事件
    WORD wReserved1;           // reserved; do not use 未使用
} DCB;
#endif

#endif // WINDOWSDEF_H_INCLUDED
