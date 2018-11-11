/*
	CApp.h
	Connect 4 application class
	
	Leo Chan
	November 1997
*/

#ifndef CAPP_H
#define CAPP_H

#include <Application.h>
#include "CMessages.h"
#include "CWindow.h"
#include "CBoard.h"
#include "CSearch.h"

#define CAPP_SIG	"application/x-vnd.Connect-4"

class CApp : public BApplication
{
	private:
		CWindow *pWindow;
		CBoard *pBoard;
		CSearch *pSearch;
		
	public:
		CApp();
		
		virtual void ReadyToRun();
		virtual bool QuitRequested();
};

#endif
