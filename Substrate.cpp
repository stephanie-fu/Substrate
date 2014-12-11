/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Josef Gajdusek
 * Based on xscreensaver's hacks/substrate.c
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

#include "Substrate.h"

#include <math.h>
#include <stdlib.h>

#include <Alignment.h>
#include <TextView.h>
#include <GroupLayout.h>
#include <Slider.h>
#include <String.h>
#include <Window.h>

#define ARRAY_SIZE(a) \
	(sizeof(a) / sizeof(a[0]))

#define radians(d) \
	(d * M_PI / 180)

#define degrees(r) \
	(r * 180 / M_PI)

#define STEP 0.42

#define GRID_INIT 361

static const BString kName = "Substrate";
static const BString kAuthor = "ported from xscreensaver by Josef Gajdusek";

static const int kMinimumCracks = 1;
static const int kMaximumCracks = 20;

static const int kMinimumDuration = 3000;
static const int kMaximumDuration = 10000;

static const int kMinimumSandGrains = 0;
static const int kMaximumSandGrains = 50;

static const int kMinimumCirclePerc = 0;
static const int kMaximumCirclePerc = 100;

static const rgb_color kCrackColor = { 0, 0, 0, 0xff };
static const rgb_color kBGColor = { 0xff, 0xfd, 0xe6, 0xff };

static const rgb_color kColorMap[] = {
	{ 0x20, 0x1f, 0x21, 0x0f },
	{ 0x26, 0x2c, 0x2e, 0x0f },
	{ 0x35, 0x26, 0x26, 0x0f },
	{ 0x37, 0x2b, 0x27, 0x0f },
	{ 0x30, 0x2c, 0x2e, 0x0f },
	{ 0x39, 0x2b, 0x2d, 0x0f },
	{ 0x32, 0x32, 0x29, 0x0f },
	{ 0x3f, 0x32, 0x29, 0x0f },
	{ 0x38, 0x32, 0x2e, 0x0f },
	{ 0x2e, 0x33, 0x3d, 0x0f },
	{ 0x33, 0x3a, 0x3d, 0x0f },
	{ 0x47, 0x33, 0x29, 0x0f },
	{ 0x40, 0x39, 0x2c, 0x0f },
	{ 0x40, 0x39, 0x2e, 0x0f },
	{ 0x47, 0x40, 0x2c, 0x0f },
	{ 0x47, 0x40, 0x2e, 0x0f },
	{ 0x4e, 0x40, 0x2c, 0x0f },
	{ 0x4f, 0x40, 0x2e, 0x0f },
	{ 0x4e, 0x47, 0x38, 0x0f },
	{ 0x58, 0x40, 0x37, 0x0f },
	{ 0x65, 0x47, 0x2d, 0x0f },
	{ 0x6d, 0x5d, 0x3d, 0x0f },
	{ 0x74, 0x55, 0x30, 0x0f },
	{ 0x75, 0x55, 0x32, 0x0f },
	{ 0x74, 0x5d, 0x32, 0x0f },
	{ 0x74, 0x64, 0x33, 0x0f },
	{ 0x7c, 0x6c, 0x36, 0x0f },
	{ 0x52, 0x31, 0x52, 0x0f },
	{ 0x44, 0x48, 0x42, 0x0f },
	{ 0x4c, 0x56, 0x47, 0x0f },
	{ 0x65, 0x5d, 0x45, 0x0f },
	{ 0x6d, 0x5d, 0x44, 0x0f },
	{ 0x6c, 0x5d, 0x4e, 0x0f },
	{ 0x74, 0x6c, 0x43, 0x0f },
	{ 0x7c, 0x6c, 0x42, 0x0f },
	{ 0x7c, 0x6c, 0x4b, 0x0f },
	{ 0x6b, 0x73, 0x4b, 0x0f },
	{ 0x73, 0x73, 0x4b, 0x0f },
	{ 0x7b, 0x7b, 0x4a, 0x0f },
	{ 0x6b, 0x6c, 0x55, 0x0f },
	{ 0x69, 0x6d, 0x5e, 0x0f },
	{ 0x7b, 0x6c, 0x5d, 0x0f },
	{ 0x6b, 0x73, 0x53, 0x0f },
	{ 0x6a, 0x74, 0x5d, 0x0f },
	{ 0x72, 0x7b, 0x52, 0x0f },
	{ 0x7b, 0x7b, 0x52, 0x0f },
	{ 0x57, 0x74, 0x6e, 0x0f },
	{ 0x68, 0x74, 0x66, 0x0f },
	{ 0x9c, 0x54, 0x2b, 0x0f },
	{ 0x9d, 0x54, 0x32, 0x0f },
	{ 0x9d, 0x5b, 0x35, 0x0f },
	{ 0x93, 0x6b, 0x36, 0x0f },
	{ 0xaa, 0x73, 0x30, 0x0f },
	{ 0xc4, 0x5a, 0x27, 0x0f },
	{ 0xd9, 0x52, 0x23, 0x0f },
	{ 0xd8, 0x5a, 0x20, 0x0f },
	{ 0xdb, 0x5a, 0x23, 0x0f },
	{ 0xe5, 0x70, 0x37, 0x0f },
	{ 0x83, 0x6c, 0x4b, 0x0f },
	{ 0x8c, 0x6b, 0x4b, 0x0f },
	{ 0x82, 0x73, 0x5c, 0x0f },
	{ 0x93, 0x73, 0x52, 0x0f },
	{ 0x81, 0x7b, 0x63, 0x0f },
	{ 0x81, 0x7b, 0x6d, 0x0f },
	{ 0x92, 0x7b, 0x63, 0x0f },
	{ 0xd9, 0x89, 0x3b, 0x0f },
	{ 0xe4, 0x98, 0x32, 0x0f },
	{ 0xdf, 0xa1, 0x33, 0x0f },
	{ 0xe5, 0xa0, 0x37, 0x0f },
	{ 0xf0, 0xab, 0x3b, 0x0f },
	{ 0x8a, 0x8a, 0x59, 0x0f },
	{ 0xb2, 0x9a, 0x58, 0x0f },
	{ 0x89, 0x82, 0x6b, 0x0f },
	{ 0x9a, 0x82, 0x62, 0x0f },
	{ 0x88, 0x8b, 0x7c, 0x0f },
	{ 0x90, 0x9a, 0x7a, 0x0f },
	{ 0xa2, 0x82, 0x62, 0x0f },
	{ 0xa1, 0x8a, 0x69, 0x0f },
	{ 0xa9, 0x99, 0x68, 0x0f },
	{ 0x99, 0xa1, 0x60, 0x0f },
	{ 0x99, 0xa1, 0x68, 0x0f },
	{ 0xca, 0x81, 0x48, 0x0f },
	{ 0xeb, 0x8d, 0x43, 0x0f },
	{ 0xc2, 0x91, 0x60, 0x0f },
	{ 0xc2, 0x91, 0x68, 0x0f },
	{ 0xd1, 0xa9, 0x77, 0x0f },
	{ 0xc9, 0xb9, 0x7f, 0x0f },
	{ 0xf0, 0xe2, 0x7b, 0x0f },
	{ 0x9f, 0x92, 0x8b, 0x0f },
	{ 0xc0, 0xb9, 0x99, 0x0f },
	{ 0xe6, 0xb8, 0x8f, 0x0f },
	{ 0xc8, 0xc1, 0x87, 0x0f },
	{ 0xe0, 0xc8, 0x86, 0x0f },
	{ 0xf2, 0xcc, 0x85, 0x0f },
	{ 0xf5, 0xda, 0x83, 0x0f },
	{ 0xec, 0xde, 0x9d, 0x0f },
	{ 0xf5, 0xd2, 0x94, 0x0f },
	{ 0xf5, 0xda, 0x94, 0x0f },
	{ 0xf4, 0xe7, 0x84, 0x0f },
	{ 0xf4, 0xe1, 0x8a, 0x0f },
	{ 0xf4, 0xe1, 0x93, 0x0f },
	{ 0xe7, 0xd8, 0xa7, 0x0f },
	{ 0xf1, 0xd4, 0xa5, 0x0f },
	{ 0xf1, 0xdc, 0xa5, 0x0f },
	{ 0xf4, 0xdb, 0xad, 0x0f },
	{ 0xf1, 0xdc, 0xae, 0x0f },
	{ 0xf4, 0xdb, 0xb5, 0x0f },
	{ 0xf5, 0xdb, 0xbd, 0x0f },
	{ 0xf4, 0xe2, 0xad, 0x0f },
	{ 0xf5, 0xe9, 0xad, 0x0f },
	{ 0xf4, 0xe3, 0xbe, 0x0f },
	{ 0xf5, 0xea, 0xbe, 0x0f },
	{ 0xf7, 0xf0, 0xb6, 0x0f },
	{ 0xd9, 0xd1, 0xc1, 0x0f },
	{ 0xe0, 0xd0, 0xc0, 0x0f },
	{ 0xe7, 0xd8, 0xc0, 0x0f },
	{ 0xf1, 0xdd, 0xc6, 0x0f },
	{ 0xe8, 0xe1, 0xc0, 0x0f },
	{ 0xf3, 0xed, 0xc7, 0x0f },
	{ 0xf6, 0xec, 0xce, 0x0f },
};

enum {
	MSG_SET_CRACKS		= 'scrn',
	MSG_SET_DURATION	= 'sdur',
	MSG_SET_GRAINS		= 'sgrs',
	MSG_SET_CIRCLE		= 'scrc',
	MSG_SET_WIREFRAME	= 'swrf',
};

extern "C" BScreenSaver*
instantiate_screen_saver(BMessage* msg, image_id id)
{
	return new Substrate(msg, id);
}


Substrate::Substrate(BMessage* archive, image_id id)
	:
	BScreenSaver(archive, id),
	fCracks(20, true),
	fNumCracks(5),
	fDuration(2000),
	fGrains(50),
	fCirclePerc(30)
{
	fNumCracks = archive->GetInt32("fNumCracks", fNumCracks);
	fDuration = archive->GetInt32("fDuration", fDuration);
	fGrains = archive->GetInt32("fGrains", fGrains);
	fCirclePerc = archive->GetInt32("fCirclePerc", fCirclePerc);
}


Substrate::~Substrate()
{
	for (int i = 0; i < fWidth; i++)
		delete fCrackGrid[i];
	delete fCrackGrid;
}


void Substrate::StartConfig(BView* view)
{
	BGroupLayout* group = new BGroupLayout(B_VERTICAL, B_USE_ITEM_SPACING);
	group->SetInsets(B_USE_WINDOW_INSETS);
	group->SetExplicitAlignment(
			BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP));
	view->SetLayout(group);

	BWindow* win = view->Window();
	if (win)
		win->AddHandler(this);

	BTextView* textView = new BTextView("name");
	textView->SetViewColor(view->ViewColor());
	textView->Insert(kName);
	textView->Insert("\n");
	textView->Insert(kAuthor);
	textView->SetStylable(true);
	textView->MakeEditable(false);
	textView->SetFontAndColor(0, kName.Length(), be_bold_font);
	view->AddChild(textView);

	BSlider* cnumSlider = new BSlider("name", "Amount of cracks:",
			new BMessage(MSG_SET_CRACKS),
			kMinimumCracks, kMaximumCracks,
			B_HORIZONTAL);
	cnumSlider->SetValue(fNumCracks);
	cnumSlider->ResizeToPreferred();
	cnumSlider->SetTarget(this);
	group->AddView(cnumSlider);

	BSlider* durationSlider = new BSlider("name", "Duration:",
			new BMessage(MSG_SET_DURATION),
			kMinimumDuration, kMaximumDuration,
			B_HORIZONTAL);
	durationSlider->SetValue(fDuration);
	durationSlider->ResizeToPreferred();
	durationSlider->SetTarget(this);
	group->AddView(durationSlider);

	BSlider* grainSlider = new BSlider("name", "Amount of grains:",
			new BMessage(MSG_SET_GRAINS),
			kMinimumSandGrains, kMaximumSandGrains,
			B_HORIZONTAL);
	grainSlider->SetValue(fGrains);
	grainSlider->ResizeToPreferred();
	grainSlider->SetTarget(this);
	group->AddView(grainSlider);

	BSlider* circleSlider = new BSlider("name", "Circle percentage:",
			new BMessage(MSG_SET_CIRCLE),
			kMinimumCirclePerc, kMaximumCirclePerc,
			B_HORIZONTAL);
	circleSlider->SetValue(fCirclePerc);
	circleSlider->ResizeToPreferred();
	circleSlider->SetTarget(this);
	group->AddView(circleSlider);
}


status_t Substrate::StartSaver(BView* view, bool prev)
{
	fPreview = prev;
	SetTickSize(prev ? 100000 : 10000);
	srandom(time(NULL));

	BRect rect = view->Bounds();
	fWidth = (int) rect.Width();
	fHeight = (int) rect.Height();

	fCrackGrid = new int*[fWidth];
	for (int i = 0; i < fWidth; i++)
		fCrackGrid[i] = new int[fHeight];

	view->SetDrawingMode(B_OP_ALPHA);
	view->SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
	view->SetFlags(view->Flags() | B_SUBPIXEL_PRECISE);

	return B_OK;
}


void Substrate::_Restart(BView* view)
{
	view->SetHighColor(kBGColor);
	view->FillRect(view->Bounds());

	fCracks.MakeEmpty();
	for (int i = 0; i < fNumCracks; i++)
		_AddCrack();

	for (int x = 0; x < fWidth; x++)
		for (int y = 0; y < fHeight; y++)
			fCrackGrid[x][y] = GRID_INIT;
}


void Substrate::Draw(BView* view, int32 frame)
{
	if (frame % fDuration == 0)
		_Restart(view);
	_StepCracks(view);
}


void Substrate::_AddCrack()
{
	Crack* c = new Crack();
	fCracks.AddItem(c);

	int timeout = 10000;
	int x = 0;
	int y = 0;
	do {
		x = (int) random() % fWidth;
		y = (int) random() % fHeight;
		timeout--;
	} while (fCrackGrid[x][y] > 360 && timeout > 0);

	if (timeout != 0) // Move perpendicular
		c->fAngle = fCrackGrid[x][y] + (random() % 2 ? 90 : -90);
	else // Or just pick random direction
		c->fAngle = random() % 360;

	c->p.x = x;
	c->p.y = y;
	c->fColor = kColorMap[random() % ARRAY_SIZE(kColorMap)];
	c->fGain = random() / RAND_MAX;
	c->fAngleDrawn = 0;

	if ((random() % 100) <= fCirclePerc) {
		float r = 10 + (random() % ((fWidth + fHeight) / 2));
		if (random() % 2)
			r *= -1;
		float radinc = STEP / r;
		c->fAngleInc = degrees(radinc);
		c->ps.x = r * (1 - cos(radinc));
		c->ps.y = r * sin(radinc);
	} else {
		c->fAngleInc = 0;
		c->ps.x = 0;
		c->ps.y = 0;
	}
}


void Substrate::_StepCracks(BView* view)
{
	view->SetPenSize(2);
	for (int i = 0; i < fCracks.CountItems(); i++) {
		Crack* c = fCracks.ItemAt(i);
		Crack old = *c;

		if (c->fAngleInc != 0) {
			c->p.x += c->ps.y * cos(radians(c->fAngle));
			c->p.y += c->ps.y * sin(radians(c->fAngle));

			c->p.x += c->ps.x * cos(radians(c->fAngle) - M_PI / 2);
			c->p.y += c->ps.x * sin(radians(c->fAngle) - M_PI / 2);
			c->fAngle += c->fAngleInc;
			c->fAngleDrawn += fabs(c->fAngleInc);
		} else {
			c->p.x += STEP * cos(radians(c->fAngle));
			c->p.y += STEP * sin(radians(c->fAngle));
		}

		if (!view->Bounds().Contains(c->p)
				|| (fCrackGrid[(int)c->p.x][(int)c->p.y] <= 360
					&& fabs(fCrackGrid[(int)c->p.x][(int)c->p.y] - c->fAngle) > 5)
				|| c->fAngleDrawn > 360) {
			fCracks.RemoveItem(c);
			_AddCrack();
			continue;
		}

		fCrackGrid[(int) c->p.x][(int) c->p.y] = (int) c->fAngle;

		_DrawSand(view, c);

		view->BeginLineArray(1);
		view->AddLine(old.p, c->p, kCrackColor);
		view->EndLineArray();
	}
}


void Substrate::_DrawSand(BView* view, Crack* c)
{
	BPoint r(c->p);
	// We need some headstart here
	r.x += (4 * sin(radians(c->fAngle)));
	r.y -= (4 * cos(radians(c->fAngle)));
	// Move perpendicular until we hit the edge or another crack
	while (true) {
		r.x += (0.81 * sin(radians(c->fAngle)));
		r.y -= (0.81 * cos(radians(c->fAngle)));

		int rx = (int) r.x;
		int ry = (int) r.y;
		if (rx < 0 || rx >= fWidth || ry < 0 || ry >= fHeight
				|| fCrackGrid[rx][ry] <= 360) {
			break;
		}
	}

	c->fGain += (random() / (float) RAND_MAX - 0.5) * 0.2;
	if (c->fGain < 0)
		c->fGain = 0;
	if (c->fGain > 1)
		c->fGain = 1;

	float w = c->fGain / (fGrains - 1);
	view->SetHighColor(c->fColor);
	for (int i = 0; i < fGrains; i++) {
		int gx = (int)(c->p.x + (r.x - c->p.x) * sin(sin(i * w)));
		int gy = (int)(c->p.y + (r.y - c->p.y) * sin(sin(i * w)));
		view->FillRect(BRect(gx, gy, gx + 1, gy  + 1));
	}
}


void Substrate::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MSG_SET_CRACKS:
		fNumCracks = msg->GetInt32("be:value", fNumCracks);
		break;
	case MSG_SET_DURATION:
		fDuration = msg->GetInt32("be:value", fDuration);
		break;
	case MSG_SET_GRAINS:
		fGrains = msg->GetInt32("be:value", fGrains);
		break;
	case MSG_SET_CIRCLE:
		fCirclePerc = msg->GetInt32("be:value", fCirclePerc);
		break;
	default:
		BHandler::MessageReceived(msg);
	}
}


status_t Substrate::SaveState(BMessage* into) const
{
	into->AddInt32("fNumCracks", fNumCracks);
	into->AddInt32("fDuration", fDuration);
	into->AddInt32("fGrains", fGrains);
	into->AddInt32("fCirclePerc", fCirclePerc);
	return B_OK;
}
