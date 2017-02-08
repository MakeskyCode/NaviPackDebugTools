#ifndef _C_FILE_H_
#define _C_FILE_H_


class CFile {
public:
	typedef enum OpenMode
	{
		READ, WRITE, RW
	}OpenMode;

	CFile();
	~CFile();

	//打开文件
	int Open(const char* fileDir,const char* fileName, OpenMode mode);
	//追加内容
	int AddBuf(char *buf,int len);
	//关闭文件
	int Close(int needSave,int isNeedMove = 1);


	char* getFileName();

	int mNeedLen;

private:
	FILE* m_pFile;
	char mFileName[64];
	char mFileDirName[128];
	int mWriteLen;
};

#endif