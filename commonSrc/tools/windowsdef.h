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
    DWORD BaudRate;            // current baud rate ָ����ǰ�Ĳ�����
    DWORD fBinary: 1;          // binary mode, no EOF check ָ���Ƿ����������ģʽ��WINDOWS 95�б���ΪTRUE
    DWORD fParity: 1;          // enable parity checking ָ����żУ���Ƿ�����
    DWORD fOutxCtsFlow:1;      // CTS output flow control ָ��CTS�Ƿ����ڼ�ⷢ�Ϳ���.��ΪTRUE��CTSΪOFF�����ͽ�������
    DWORD fOutxDsrFlow:1;      // DSR output flow control ָ��DSR�Ƿ����ڼ�ⷢ�Ϳ���.��ΪTRUE��DSRΪOFF�����ͽ�������
    DWORD fDtrControl:2;       // DTR flow control type DTR_CONTROL_DISABLEֵ��DTR��ΪOFF, DTR_CONTROL_ENABLEֵ��DTR��ΪON, DTR_CONTROL_HANDSHAKE����DTR"����",
    DWORD fDsrSensitivity:1;   // DSR sensitivity ����ֵΪTRUEʱDSRΪOFFʱ���յ��ֽڱ�����
    DWORD fTXContinueOnXoff:1; // XOFF continues Tx ָ�������ջ���������,�������������Ѿ����ͳ�XoffChar�ַ�ʱ�����Ƿ�ֹͣ.TRUEʱ���ڽ��ջ��������յ��������������ֽ�XoffLim�����������Ѿ����ͳ�XoffChar�ַ���ֹ�����ֽ�֮�󣬷��ͼ������С�FALSEʱ���ڽ��ջ��������յ����������ѿյ��ֽ�XonChar�����������Ѿ����ͳ��ָ����͵�XonChar֮�󣬷��ͼ������С�
    DWORD fOutX: 1;            // XON/XOFF out flow control TRUEʱ�����յ�XoffChar֮���ֹͣ����.���յ�XonChar֮�����¿�ʼ
    DWORD fInX: 1;             // XON/XOFF in flow control TRUEʱ�����ջ��������յ�������������XoffLim֮��XoffChar���ͳ�ȥ.���ջ��������յ����������յ�XonLim֮��XonChar���ͳ�ȥ
    DWORD fErrorChar: 1;       // enable error replacement ��ֵΪTRUE��fParityΪTRUEʱ����ErrorChar ��Աָ�����ַ�������żУ�����Ľ����ַ�
    DWORD fNull: 1;            // enable null stripping TRUEʱ������ʱȥ���գ�0ֵ���ֽ�
    DWORD fRtsControl:2;       // RTS flow control RTS_CONTROL_DISABLEʱ,RTS��ΪOFF RTS_CONTROL_ENABLEʱ, RTS��ΪON RTS_CONTROL_HANDSHAKEʱ,�����ջ�����С�ڰ���ʱRTSΪON �����ջ����������ķ�֮����ʱRTSΪOFF RTS_CONTROL_TOGGLEʱ,�����ջ���������ʣ���ֽ�ʱRTSΪON ,����ȱʡΪOFF
    DWORD fAbortOnError:1;     // abort reads/writes on error TRUEʱ,�д�����ʱ��ֹ����д����
    DWORD fDummy2:17;          // reserved δʹ��
    WORD wReserved;            // not currently used δʹ��,����Ϊ0
    WORD XonLim;               // transmit XON threshold ָ����XON�ַ�������ǰ���ջ������п��������С�ֽ���
    WORD XoffLim;              // transmit XOFF threshold ָ����XOFF�ַ�������ǰ���ջ������п��������С�ֽ���
    BYTE ByteSize;             // number of bits/byte, 4-8 ָ���˿ڵ�ǰʹ�õ�����λ
    BYTE Parity;               // 0-4=no,odd,even,mark,space ָ���˿ڵ�ǰʹ�õ���żУ�鷽��,����Ϊ:EVENPARITY,MARKPARITY,NOPARITY,ODDPARITY
    BYTE StopBits;             // 0,1,2 = 1, 1.5, 2 ָ���˿ڵ�ǰʹ�õ�ֹͣλ��,����Ϊ:ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS
    char XonChar;              // Tx and Rx XON character ָ�����ڷ��ͺͽ����ַ�XON��ֵ
    char XoffChar;             // Tx and Rx XOFF character ָ�����ڷ��ͺͽ����ַ�XOFFֵ
    char ErrorChar;            // error replacement character ���ַ�����������յ�����żУ�鷢������ʱ��ֵ
    char EofChar;              // end of input character ��û��ʹ�ö�����ģʽʱ,���ַ�������ָʾ���ݵĽ���
    char EvtChar;              // received event character �����յ����ַ�ʱ,�����һ���¼�
    WORD wReserved1;           // reserved; do not use δʹ��
} DCB;
#endif

#endif // WINDOWSDEF_H_INCLUDED
