/*
	CApp.cpp
	Connect 4 Application Class
	
	Leo Chan
	November 1997
*/

#include "CApp.h"

CApp :: CApp()
	: BApplication(CAPP_SIG)
{
	BScreen s;
	BRect f;

	/* Construct peer loops */	
	f.left = (s.Frame().Width() - CWIN_WIDTH) / 2;
	f.top = (s.Frame().Height() - CWIN_HEIGHT) / 2;
	f.right = f.left + CWIN_WIDTH;
	f.bottom = f.top + CWIN_HEIGHT;
	
	pBoard = new CBoard();
	pSearch = new CSearch(pBoard);
	pWindow = new CWindow(f, pBoard, pSearch);
}

void CApp :: ReadyToRun()
{
	pWindow->Show();
}

bool CApp :: QuitRequested()
{
	pBoard->PostMessage(B_QUIT_REQUESTED, NULL);
	pSearch->PostMessage(B_QUIT_REQUESTED, NULL);
	pWindow->PostMessage(B_QUIT_REQUESTED, NULL);
	return true;
}
