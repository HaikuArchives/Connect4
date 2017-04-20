/*
	CView.h
	Connect 4 view class
	
	Leo Chan
	November 1997
*/

#ifndef CVIEW_H
#define CVIEW_H

#include <View.h>
#include "CMessages.h"
#include "CDefs.h"

class CView : public BView
{
	private:
		int8 cell[CBRD_ROWS][CBRD_COLS];
		
	public:
		CView(BRect frame);
		virtual void Draw(BRect updateRect);
		virtual void MessageReceived(BMessage *msg);
};

#endif
