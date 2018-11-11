/*
	CWindow.h
	Connect4 Window class
	
	Leo Chan
	November 1997
*/

#ifndef CWINDOW_H
#define CWINDOW_H
#include <Application.h>
#include <InterfaceKit.h>

#include <Window.h>
#include <View.h>
#include <MenuBar.h>
#include <Button.h>
#include <Alert.h>

#include "CView.h"

#define CWIN_TITLE	"Connect-4"
#define CWIN_WIDTH	CCEL_WIDTH * 7
#define CWIN_HEIGHT	CCEL_HEIGHT * 6


#define CBTN_WIDTH	CCEL_WIDTH
#define CBTN_HEIGHT	10

#define CCEL_WIDTH	45
#define CCEL_HEIGHT	45

class BMenuItem;

class CWindow : public BWindow
{
	private:
		BButton *aSelect[7];
		CView *pDisplay;
		BMenuItem *pBlue, *pRed;
		BMenuItem *pEasy, *pNormal, *pHard;
		BLooper *pBoard, *pSearch;
		int8 expect;
		
		void ProcessReply(BMessage *msg);
		void ProcessTurn(int8 player);
		void ProcessMove(BMessage *msg);
				
	public:
		CWindow(BRect frame, BLooper *board, BLooper *search);
		
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage *msg);
};

#endif
