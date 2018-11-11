/*
	CSearch.h
	Search class
	
	Leo Chan
	December 1997
*/

#ifndef CSEARCH_H
#define CSEARCH_H

#include <Looper.h>
#include <Application.h>
#include <stdio.h>
#include "CDefs.h"
#include "CMessages.h"

#define CSCH_EASY	4
#define CSCH_NORMAL	6
#define CSCH_HARD	8

#define CSCH_NAME	"Search Looper"

class CSearch : public BLooper
{
	private:
		int8 cell[CBRD_ROWS][CBRD_COLS];
		bool brdreply, statreply;
		int8 maxdepth, searchplayer;
		int score[CBRD_COLS];
		BLooper *pBoard;
		BMessenger *request;
		
		/* Message services */
		void SetDifficulty(BMessage *msg);
		void PrepareSearch(BMessage *msg);
		void StartSearch();
		
		/* Reply handling */
		void BoardReply(BMessage *msg);
		void StatusReply(BMessage *msg);
		
		/* Search function */
		int MakeMove(int col, int8 player, int8 depth);
		
		/* Checks */
		int8 CheckWinner();
		bool CheckHorizontal(int r, int c, int8 color);
		bool CheckVertical(int r, int c, int8 color);
		bool CheckNWDiagonal(int r, int c, int8 color);
		bool CheckNEDiagonal(int r, int c, int8 color);
		bool IsFull();
		
	public:
		CSearch(BLooper *board);
		virtual void MessageReceived(BMessage *msg);
};

#endif
