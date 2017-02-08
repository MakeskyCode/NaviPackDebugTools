#ifdef NAVIPACK_WIN
#include "MyFunc.h"

int stretch_scale_buf = 2;
int stretch_scale;

using namespace std;

/** \brief String×ª»» */
void MarshalString(System::String ^ s, std::string& os) 
{
	const char* chars =	(const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}
#endif