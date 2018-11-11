/*
	CMain.cpp
	Connect 4 main program
	
	Leo Chan
	November 1997
*/

#include "CApp.h"

int main()
{
	CApp *pApp;
	
	pApp = new CApp();
	pApp->Run();
	delete pApp;
	return 0;
}
