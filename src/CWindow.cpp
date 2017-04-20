/*
	CWindow.cpp
	Connect 4 window class
	
	Leo Chan
	November 1997
*/

#include "CWindow.h"

CWindow :: CWindow(BRect frame, BLooper *board, BLooper *search)
	: BWindow(frame, CWIN_TITLE, B_TITLED_WINDOW, B_NOT_RESIZABLE)
{
	BRect f;
	BMenuBar *pMainMenu;
	BMenu *pMenu;
	int i;

	Lock();
	pBoard = board;
	pSearch = search;
	expect = NO_PLAYER;
					
	/* Setup the main menu */
	f = Bounds();
	pMainMenu = new BMenuBar(f, "Main Menu");

	/* Game Menu */
	pMenu = new BMenu("Game");
	pMenu->AddItem(new BMenuItem("New Game", new BMessage(CWIN_MSG_NEW), 'N'));
	pMenu->AddItem(new BSeparatorItem());
	
	pBlue = new BMenuItem("Blue", new BMessage(CWIN_MSG_BLUE), 'B');
	pBlue->SetMarked(true);
	pMenu->AddItem(pBlue);
	
	pRed = new BMenuItem("Red", new BMessage(CWIN_MSG_RED), 'R');
	pRed->SetMarked(false);
	pMenu->AddItem(pRed);
	
	pMenu->AddItem(new BSeparatorItem());
	pMenu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));

	pMainMenu->AddItem(pMenu);

	/* Difficulty Menu */
	pMenu = new BMenu("Difficulty");

	pEasy = new BMenuItem("Easy", new BMessage(CWIN_MSG_EASY));
	pEasy->SetMarked(true);
	pMenu->AddItem(pEasy);
	
	pNormal = new BMenuItem("Normal", new BMessage(CWIN_MSG_NORMAL));
	pNormal->SetMarked(false);
	pMenu->AddItem(pNormal);
	
	pHard = new BMenuItem("Hard", new BMessage(CWIN_MSG_HARD));
	pHard->SetMarked(false);
	pMenu->AddItem(pHard);

	pMenu->SetRadioMode(true);
	pMainMenu->AddItem(pMenu);
		
	AddChild(pMainMenu);
	
	/* Setup user selection buttons */
	f.top = pMainMenu->Frame().bottom;
	f.left = 0;
	f.right = f.left + CBTN_WIDTH;
	f.bottom = 0;
	aSelect[0] = new BButton(f, "Button1", "1", new BMessage(CWIN_MSG_BTN1));
	aSelect[0]->SetEnabled(false);
	AddChild(aSelect[0]);
	
	f.left = f.right;
	f.right = f.left + CBTN_WIDTH;
	aSelect[1] = new BButton(f, "Button2", "2", new BMessage(CWIN_MSG_BTN2));
	aSelect[1]->SetEnabled(false);
	AddChild(aSelect[1]);
	
	f.left = f.right;
	f.right = f.left + CBTN_WIDTH;
	aSelect[2] = new BButton(f, "Button3", "3", new BMessage(CWIN_MSG_BTN3));
	aSelect[2]->SetEnabled(false);
	AddChild(aSelect[2]);
	
	f.left = f.right;
	f.right = f.left + CBTN_WIDTH;
	aSelect[3] = new BButton(f, "Button4", "4", new BMessage(CWIN_MSG_BTN4));
	aSelect[3]->SetEnabled(false);
	AddChild(aSelect[3]);
	
	f.left = f.right;
	f.right = f.left + CBTN_WIDTH;
	aSelect[4] = new BButton(f, "Button5", "5", new BMessage(CWIN_MSG_BTN5));
	aSelect[4]->SetEnabled(false);
	AddChild(aSelect[4]);
	
	f.left = f.right;
	f.right = f.left + CBTN_WIDTH;
	aSelect[5] = new BButton(f, "Button6", "6", new BMessage(CWIN_MSG_BTN6));
	aSelect[5]->SetEnabled(false);
	AddChild(aSelect[5]);
	
	f.left = f.right;
	f.right = f.left + CBTN_WIDTH;
	aSelect[6] = new BButton(f, "Button7", "7", new BMessage(CWIN_MSG_BTN7));
	aSelect[6]->SetEnabled(false);
	AddChild(aSelect[6]);

	/* Setup the view */
	f = aSelect[0]->Frame();
	f.top = f.bottom;
	f.left = Bounds().left;
	f.right = Bounds().right;
	f.bottom = Bounds().bottom;	
	pDisplay = new CView(f);
	AddChild(pDisplay);

	/* Set difficulty level */
	PostMessage(CWIN_MSG_EASY, NULL);
	
	Unlock();
}

bool CWindow :: QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED, NULL);
	return true;
}

void CWindow :: MessageReceived(BMessage *msg)
{
	BMessage *diffmsg;
	
	switch (msg->what)
	{
		/* Game Menu */
		case CWIN_MSG_NEW:
			pBoard->PostMessage(CBRD_MSG_NEW_BLUE, NULL);
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BLUE:
			pBlue->SetMarked(true);
			pRed->SetMarked(false);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;

		case CWIN_MSG_RED:
			pBlue->SetMarked(false);
			pRed->SetMarked(true);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		/* Difficulty Menu */
		case CWIN_MSG_EASY:
			diffmsg = new BMessage(CSCH_MSG_DIFFICULTY);
			diffmsg->AddInt8("Level", DIFFICULTY_EASY);
			pSearch->PostMessage(diffmsg, pSearch);
			delete diffmsg;
			break;

		case CWIN_MSG_NORMAL:
			diffmsg = new BMessage(CSCH_MSG_DIFFICULTY);
			diffmsg->AddInt8("Level", DIFFICULTY_NORMAL);
			pSearch->PostMessage(diffmsg, pSearch);
			delete diffmsg;
			break;
		
		case CWIN_MSG_HARD:
			diffmsg = new BMessage(CSCH_MSG_DIFFICULTY);
			diffmsg->AddInt8("Level", DIFFICULTY_HARD);
			pSearch->PostMessage(diffmsg, pSearch);
			delete diffmsg;
			break;

		/* Buttons */
		case CWIN_MSG_BTN1:
			pBoard->PostMessage(CBRD_MSG_COL1, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BTN2:
			pBoard->PostMessage(CBRD_MSG_COL2, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BTN3:
			pBoard->PostMessage(CBRD_MSG_COL3, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BTN4:
			pBoard->PostMessage(CBRD_MSG_COL4, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BTN5:
			pBoard->PostMessage(CBRD_MSG_COL5, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BTN6:
			pBoard->PostMessage(CBRD_MSG_COL6, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		case CWIN_MSG_BTN7:
			pBoard->PostMessage(CBRD_MSG_COL7, NULL);		
			pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
			pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
			break;
			
		/* Reply received */
		case CBRD_MSG_REPLY_STAT: ProcessReply(msg); break;
		case CSCH_MSG_REPLY_MOVE: ProcessMove(msg); break;
	}
}

void CWindow :: ProcessReply(BMessage *msg)
{
	int i;
	int8 status;
	BAlert *alert;
		
	if (msg->FindInt8("Status", &status) != B_NO_ERROR) return;

	switch (status)
	{
		case STATUS_NO_GAME:
			for (i = 0; i < CBRD_COLS; i++) aSelect[i]->SetEnabled(false);
			break;
			
		case STATUS_BLUE_WIN:
			for (i = 0; i < CBRD_COLS; i++) aSelect[i]->SetEnabled(false);
			alert = new BAlert(CWIN_TITLE, "Blue player won!\n", "Ok");
			alert->Go();
			break;
			
		case STATUS_RED_WIN:
			for (i = 0; i < CBRD_COLS; i++) aSelect[i]->SetEnabled(false);
			alert = new BAlert(CWIN_TITLE, "Red player won!\n", "Ok");
			alert->Go();
			break;
			
		case STATUS_DRAW:
			for (i = 0; i < CBRD_COLS; i++) aSelect[i]->SetEnabled(false);
			alert = new BAlert(CWIN_TITLE, "Draw game!\n", "Ok");
			alert->Go();
			break;
			
		case STATUS_BLUE_TURN: ProcessTurn(BLUE_PLAYER); break;
		case STATUS_RED_TURN: ProcessTurn(RED_PLAYER); break;
	}	
}

void CWindow :: ProcessTurn(int8 player)
{
	int i;
	BMessage *msg;

	expect = player;	
	if ((player == BLUE_PLAYER && pBlue->IsMarked())
		|| (player == RED_PLAYER && pRed->IsMarked()))
	{
		for (i = 0; i < CBRD_COLS; i++) aSelect[i]->SetEnabled(true);
	}
	else
	{
		for (i = 0; i < CBRD_COLS; i++) aSelect[i]->SetEnabled(false);
		msg = new BMessage(CSCH_MSG_SEARCH_MOVE);
		pSearch->PostMessage(msg, NULL, this);
		delete msg;
	} 
}

void CWindow :: ProcessMove(BMessage *msg)
{
	int8 move;
	
	/* Are we expecting a move? */
	if ((expect == BLUE_PLAYER && pBlue->IsMarked())
		|| (expect == RED_PLAYER && pRed->IsMarked())
		|| (expect == NO_PLAYER)) return;
		
	if (msg->FindInt8("Move", &move) != B_NO_ERROR) return;
	
	switch (move)
	{
		case 0: pBoard->PostMessage(CBRD_MSG_COL1, NULL);	break;
		case 1: pBoard->PostMessage(CBRD_MSG_COL2, NULL);	break;
		case 2: pBoard->PostMessage(CBRD_MSG_COL3, NULL);	break;
		case 3: pBoard->PostMessage(CBRD_MSG_COL4, NULL);	break;
		case 4: pBoard->PostMessage(CBRD_MSG_COL5, NULL);	break;
		case 5: pBoard->PostMessage(CBRD_MSG_COL6, NULL);	break;
		case 6: pBoard->PostMessage(CBRD_MSG_COL7, NULL);	break;
	}

	pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, pDisplay);
	pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
}
