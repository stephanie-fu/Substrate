/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Josef Gajdusek
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _SUBSTRATE_H
#define _SUBSTRATE_H

#include <Point.h>
#include <Handler.h>
#include <ScreenSaver.h>
#include <ObjectList.h>

struct Crack {
	BPoint		p;
	float		fAngle;

	float		fAngleInc;
	BPoint		ps;
	float		fAngleDrawn;

	rgb_color	fColor;
	float		fGain;
};

class Substrate : public BScreenSaver, public BHandler {
public:
						Substrate(BMessage* archive, image_id id);
						~Substrate();

	virtual void		StartConfig(BView* view);

	virtual status_t	StartSaver(BView* view, bool preview);
	virtual void		Draw(BView* view, int32 frame);

	virtual void		MessageReceived(BMessage* msg);
	virtual status_t	SaveState(BMessage* into) const;

private:
	void						_AddCrack();
	void						_StepCracks(BView* view);
	void						_DrawSand(BView* view, Crack* c);
	void						_Restart(BView* view);

	int							fWidth;
	int							fHeight;
	bool						fPreview;

	BObjectList<struct Crack>	fCracks;
	int**						fCrackGrid;

	int							fNumCracks;
	int							fDuration;
	int							fGrains;
	int							fCirclePerc;
	bool						fDark;

	bool						fWantRestart;
};

rgb_color InvertColor(rgb_color clr);

#endif
