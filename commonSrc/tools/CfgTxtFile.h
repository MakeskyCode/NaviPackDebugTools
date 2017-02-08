#ifndef CFG_TXT_H
#define CFG_TXT_H

#define OUT



/****************************************
 *
 *
 *
 *return value:
 *  1:success.
 * -1:open file failure.
 * -2:no the element
 * -3:the element value is null.
 ****************************************/
int ReadString(const char *FileName, const char *Key, OUT char *pValue); //Type=EKIP




/****************************************
 *
 *
 *
 *return value:
 *  1:success.
 * -1:some parameter is null.
 * -2:open file failure.
 * -3:malloc error.
 ****************************************/
int WriteString(const char *FileName, const char *Key, const char *Value); //Type=AES





/****************************************
 *
 *
 *
 *return value:
 *  1:success.
 * -1:open file failure.
 * -2:no the element
 * -3:the element value is null.
 ****************************************/

int ReadInt(const char *FileName, const char *Key, int *pValue);




int ReadLongLong(const char *FileName, const char *Key, long long *pValue);
int WriteLongLong(const char *FileName, const char *Key, long long pValue);


/****************************************
 *
 *
 *
 *return value:
 *  1:success.
 * -1:some parameter is null.
 * -2:open file failure.
 * -3:malloc error.
 ****************************************/
int WriteInt(const char *FileName, const char *Key, int Value);


#endif
