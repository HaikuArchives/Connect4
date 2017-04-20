/*
	CSearch.cpp
	CSearch class
	
	Leo Chan
	December 1997
*/

#include "CSearch.h"

CSearch :: CSearch(BLooper *board)
 : BLooper(CSCH_NAME)
{
	pBoard = board;
	maxdepth = CSCH_EASY;
	request = NULL;
	Run();
}

void CSearch :: MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case CSCH_MSG_SEARCH_MOVE: PrepareSearch(msg); break;
		case CSCH_MSG_DIFFICULTY: SetDifficulty(msg); break;

		case CBRD_MSG_REPLY_BRD: BoardReply(msg); break;
		case CBRD_MSG_REPLY_STAT: StatusReply(msg); break;

		case CSCH_MSG_READY: if (brdreply && statreply) StartSearch(); break;
	}
}

void CSearch :: SetDifficulty(BMessage *msg)
{
	int8 level;
	
	if (msg->FindInt8("Level", &level) != B_NO_ERROR) return;
	
	if (level == DIFFICULTY_EASY) maxdepth = CSCH_EASY;
	else if (level == DIFFICULTY_NORMAL) maxdepth = CSCH_NORMAL;
	else if (level == DIFFICULTY_HARD) maxdepth = CSCH_HARD;
}

void CSearch :: PrepareSearch(BMessage *msg)
{
	int8 player;
	int j;

	/* Are we in the middle of a search? */
	if (request) return;
	request = new BMessenger(msg->ReturnAddress());
	
	/* Reset for search */
	for (j = 0; j < CBRD_COLS; j++) score[j] = 0;
	 
	/* Request for board and current state */
	brdreply = statreply = false;
	pBoard->PostMessage(CBRD_MSG_GET_BRD, NULL, this);
	pBoard->PostMessage(CBRD_MSG_GET_STAT, NULL, this);
}

void CSearch :: BoardReply(BMessage *msg)
{
	int i, j, k;

	brdreply = true;	
	for (i = 0, k = 0; i < CBRD_ROWS; i++)
		for (j = 0; j < CBRD_COLS; j++, k++)
			msg->FindInt8("Board", k, &(cell[i][j]));
	if (statreply) PostMessage(CSCH_MSG_READY, NULL);
}

void CSearch :: StatusReply(BMessage *msg)
{
	int8 status;
		
	if (msg->FindInt8("Status", &status) != B_NO_ERROR) return;

	statreply = true;
	switch (status)
	{
		case STATUS_NO_GAME:
		case STATUS_BLUE_WIN:
		case STATUS_RED_WIN:
		case STATUS_DRAW:
			return;
			break;
			
		case STATUS_BLUE_TURN: searchplayer = BLUE_PLAYER; break;
		case STATUS_RED_TURN: searchplayer = RED_PLAYER; break;
	}	
	if (brdreply) PostMessage(CSCH_MSG_READY, NULL);
}

void CSearch :: StartSearch()
{
	int j, max;
	BMessage *reply;
	
	score[0] = 0; score[1] = 1; score[2] = 2; score[3] = 3;
	score[4] = 2; score[5] = 1; score[6] = 0;
	
	for (j = 0; j < CBRD_COLS; j++)
		if (cell[CBRD_ROWS - 1][j] == CCEL_EMPTY)
			score[j] += MakeMove(j, searchplayer, maxdepth);
		else
			score[j] = INT_MIN;
			
	/* Find max */
	max = 0;
	for (j = 0; j < CBRD_COLS; j++)
	{
		printf("%d ", score[j]);
		if (score[j] > score[max]) max = j;
	}
	printf("\n");
	
	/* Send reply */
	reply = new BMessage(CSCH_MSG_REPLY_MOVE);
	reply->AddInt8("Move", max);
	request->SendMessage(reply);
	delete request;
	delete reply;
	request = NULL;
}

int CSearch :: MakeMove(int col, int8 player, int8 depth)
{
	int8 winner, next;
	int i, j, score;

	if (depth == 0) return 0;
	
	/* Find next slot */
	for (i = 0; i < CBRD_ROWS && cell[i][col] != CCEL_EMPTY; i++);
	
	if (i == CBRD_ROWS) return 0;
	
	cell[i][col] = player;

	/* Is this game done? */
	winner = CheckWinner();
	if (winner == NO_PLAYER)
	{	
		/* Draw game */
		if (IsFull())
		{
			score = 0;
		}
		else
		{
			/* Next player */
			if (player == BLUE_PLAYER) next = RED_PLAYER;
			else next = BLUE_PLAYER;
		
			for (j = 0, score = 0; j < CBRD_COLS; j++)
				score += MakeMove(j, next, depth - 1);
		}
	}
	else
	{
		if (winner == searchplayer) score = pow(depth * maxdepth, depth);
		else score = -(pow(depth * maxdepth, depth));
	}
			
	cell[i][col] = CCEL_EMPTY;

	return score;
}

int8 CSearch :: CheckWinner()
{
	int i, j;
	char winner;

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

bool CSearch :: CheckHorizontal(int r, int c, int8 color)
{
	int i, j, count;
	
	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && j < CBRD_COLS && cell[i][j] == color; 
		count++, j++);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CSearch :: CheckVertical(int r, int c, int8 color)
{
	int i, j, count;
	
	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && i < CBRD_ROWS && cell[i][j] == color;
		count++, i++);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CSearch :: CheckNWDiagonal(int r, int c, int8 color)
{
	int i, j, count;
	
	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && i < CBRD_ROWS && j >= 0 && cell[i][j] == color;
		count++, i++, j--);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CSearch :: CheckNEDiagonal(int r, int c, int8 color)
{
	int i, j, count;
	
	i = r; j = c;
	for (count = 0; 
		count < WIN_CELLS && i < CBRD_ROWS && j < CBRD_COLS && cell[i][j] == color;
		count++, i++, j++);
		
	if (count == WIN_CELLS) return true;
	else return false;
}

bool CSearch :: IsFull()
{
	int j;
	bool full;

	full = true;
	for (j = 0; j < CBRD_COLS; j++)
			if (cell[CBRD_ROWS - 1][j] == CCEL_EMPTY) full = false;
	
	return full;
}
