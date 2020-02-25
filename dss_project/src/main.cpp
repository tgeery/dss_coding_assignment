#include "JsonManager.h"
#include "GlfwMainWindow.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void runJsonManagerTest()
{
	JsonManager jmgr;
	FILE* fp = fopen("..\\test\\mlb_data.json", "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buf = (char*)malloc(len);
		if (fread(buf, 1, len, fp) > 0) {
			jmgr.parseRequest(buf);
		}
		fclose(fp);
		free(buf);
	}
}

int main(int argc, char* argv[])
{
	//runJsonManagerTest();
	JsonManager jmgr;
	MainWindow main_window(jmgr);
	main_window.Run();

	return 0;
}