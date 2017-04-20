/*
	CView.cpp
	Connect 4 view class
	
	Leo Chan
	November 1997
*/

#include "CView.h"

CView :: CView(BRect frame)
	: BView(frame, "Connect 4 View", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	int i, j;
	
	for (i = 0; i < CBRD_ROWS; i++)
		for (j = 0; j < CBRD_COLS; j++)
			cell[i][j] = CCEL_EMPTY;
			
	SetViewColor(192, 192, 192);
}

void CView :: Draw(BRect updateRect)
{
	int i, j;
	BRect rect(8, 8, 36, 36);

	for (i = CBRD_ROWS - 1; i >= 0; i--)
	{
		for (j = 0; j < CBRD_COLS; j++)
		{
			if (rect.Intersects(updateRect))
				switch (cell[i][j])
				{
					case CCEL_BLUE:
						SetHighColor(0,0,255);
						FillEllipse(rect);
						break;
					
					case CCEL_RED:
						SetHighColor(255,0,0);
						FillEllipse(rect);
						break;
					
					default:
						SetHighColor(255,255,255);
						FillEllipse(rect);
						break;
				}
			rect.OffsetBy(45, 0);
		}
		rect.OffsetTo(8, rect.bottom + 8);
	}
}

void CView :: MessageReceived(BMessage *msg)
{
	int i, j, k;
	BRect rect(8, 188, 36, 232);
	int8 color;
	
	if (msg->what == CBRD_MSG_REPLY_BRD)
	{
		for (i = 0, k = 0; i < CBRD_ROWS; i++)
		{
			for (j = 0; j < CBRD_COLS; j++, k++)
			{
				msg->FindInt8("Board", k, &color);
				
				if (color != cell[i][j])
				{
					cell[i][j] = color;
					Invalidate(rect);
				}
				rect.OffsetBy(45, 0);
			}
			rect.OffsetTo(8, rect.top - 36);
		}
	}
}
