/*
	CBoard.h
	Connect 4 board class
	
	Leo Chan
	November 1997

	CBRD_ROWS
	+
	+
	+
	+
	0+++++++++++ CBRD_COLS
*/

#ifndef CBOARD_H
#define CBOARD_H

#include <Looper.h>
#include "CDefs.h"
#include "CMessages.h"

#define CBRD_NAME	"Board Looper"

class CBoard : public BLooper
{
	private:
		int8 cell[CBRD_ROWS][CBRD_COLS];
		int8 turn;	// who's turn

		bool CheckHorizontal(int r, int c, char color);
		bool CheckVertical(int r, int c, char color);
		bool CheckNWDiagonal(int r, int c, char color);
		bool CheckNEDiagonal(int r, int c, char color);
		void Reset();

		// Start game with this color first
		void Start(char first);
		
		// Put a move in this column
		void Move(int column);
		
		// Check for winner; returns 0 if no winner, else the winner's color
		char CheckWinner();
		
		// returns true if board is full
		bool IsFull();

		// Replies
		void ReplyStatus(BMessage *msg);
		void ReplyBoard(BMessage *msg);
				
	public:
		CBoard();

		// Message handler
		virtual void MessageReceived(BMessage *msg);
};

#endif
