/*
	CBoard.cpp
	Connect 4 board class
	
	Leo Chan
	November 1997
*/

#include "CBoard.h"

void CBoard :: Reset()
{
	int i, j;
	
	turn = CCEL_EMPTY;
	for (i = 0; i < CBRD_ROWS; i++)
		for (j = 0; j < CBRD_COLS; j++)
			cell[i][j] = CCEL_EMPTY;
}

CBoard :: CBoard()
 : BLooper(CBRD_NAME)
{
	Reset();
	turn = NO_PLAYER;
	Run();
}
		
void CBoard :: Start(char first)
{
	if (first == CCEL_BLUE || first == CCEL_RED)
	{
		Reset();
		turn = first;
	}
}
		
void CBoard :: Move(int column)
{
	int i;
	
	if (turn != BLUE_PLAYER && turn != RED_PLAYER) return;
		
	/* find next empty row */
	for (i = 0; i < CBRD_ROWS && cell[i][column] != CCEL_EMPTY; i++);
	
	if (i < CBRD_ROWS)
	{
		cell[i][column] = turn;
		if (turn == BLUE_PLAYER) turn = RED_PLAYER;
		else turn = BLUE_PLAYER;
	}
}
		
char CBoard :: CheckWinner()
{
	int i, j;
	int8 winner;

	winner = NO_PLAYER;
		
	for (i = 0; i < CBRD_ROWS && winner == NO_PLAYER; i++)
		for (j = 0; j < CBRD_COLS && winner == NO_PLAYER; j++)
		{
			if (cell[i][j] != CCEL_EMPTY)
			{
				if (CheckHorizontal(i, j, cell[i][j])) winner = cell[i][j];
				else if (CheckVertical(i, j, cell[i][j])) winner = cell[i][j];
				else if (CheckNWDiagonal(i, j, cell[i][j])) winner = cell[i][j];
				else if (CheckNEDiagonal(i, j, cell[i][j])) winner = cell[i][j];
			}
		}

	return winner;
}

bool CBoard :: CheckHorizontal(int r, int c, char color)
{
	int i, j, count;

	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && j < CBRD_COLS && cell[i][j] == color;
		count++, j++);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CBoard :: CheckVertical(int r, int c, char color)
{
	int i, j, count;

	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && i < CBRD_ROWS && cell[i][j] == color;
		count++, i++);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CBoard :: CheckNWDiagonal(int r, int c, char color)
{
	int i, j, count;
	
	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && i < CBRD_ROWS && j >= 0 && cell[i][j] == color; 
		count++, i++, j--);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CBoard :: CheckNEDiagonal(int r, int c, char color)
{
	int i, j, count;
	
	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && i < CBRD_ROWS && j < CBRD_COLS && cell[i][j] == color;
		count++, i++, j++);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CBoard :: IsFull()
{
	int j;
	bool full;

	full = true;
	for (j = 0; j < CBRD_COLS; j++)
			if (cell[CBRD_ROWS - 1][j] == CCEL_EMPTY) full = false;
	
	return full;
}

void CBoard :: MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case CBRD_MSG_COL1: Move(0); break;
		case CBRD_MSG_COL2: Move(1); break;
		case CBRD_MSG_COL3: Move(2); break;
		case CBRD_MSG_COL4: Move(3); break;
		case CBRD_MSG_COL5: Move(4); break;
		case CBRD_MSG_COL6: Move(5); break;
		case CBRD_MSG_COL7: Move(6); break;
		
		case CBRD_MSG_NEW_BLUE: Start(BLUE_PLAYER); break;
		case CBRD_MSG_NEW_RED: Start(RED_PLAYER); break;
		
		case CBRD_MSG_GET_STAT: ReplyStatus(msg); break;
		case CBRD_MSG_GET_BRD: ReplyBoard(msg); break;
	}
}

void CBoard :: ReplyStatus(BMessage *msg)
{
	BMessage *reply;
	char winner;

	/* Check status */
	reply = new BMessage(CBRD_MSG_REPLY_STAT);

	/* Is there a game going on? */	
	if (turn != NO_PLAYER)
	{
		/* Has someone won? */
		winner = CheckWinner();
		if (winner == NO_PLAYER)
		{
			/* Is it a draw? */
			if (!IsFull())
			{
				/* Who's turn is it? */	
				if (turn == BLUE_PLAYER) reply->AddInt8("Status", STATUS_BLUE_TURN);
				else reply->AddInt8("Status", STATUS_RED_TURN);
			}
			else
			{
				turn = NO_PLAYER;
				reply->AddInt8("Status", STATUS_DRAW);
			}
		}
		else
		{
			turn = NO_PLAYER;
			if (winner == BLUE_PLAYER) reply->AddInt8("Status", STATUS_BLUE_WIN);
			else reply->AddInt8("Status", STATUS_RED_WIN);
		}
	}
	else reply->AddInt8("Status", STATUS_NO_GAME);
	
	msg->SendReply(reply);
	delete reply;
}

void CBoard :: ReplyBoard(BMessage *msg)
{
	BMessage *reply;
	int i, j;
		
	/* Return the current board */
	reply = new BMessage(CBRD_MSG_REPLY_BRD);

	for (i = 0; i < CBRD_ROWS; i++)
		for (j = 0; j < CBRD_COLS; j++)
			reply->AddInt8("Board", cell[i][j]);
			
	msg->SendReply(reply);
	delete reply;
}
