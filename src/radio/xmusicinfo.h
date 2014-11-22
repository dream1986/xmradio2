/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_MUSIC_INFO_H__
#define __X_MUSIC_INFO_H__

#include <QString>
#include <core/xglobal.h>

XMR_BEGIN_NS

struct XMusicInfo
{
	QString title;
	QString artist;
	QString albumName;
	QString albumImageUrl;
	QString url;
	int rate;

	XMusicInfo()
		: rate(0)
	{
	}

	XMusicInfo(const XMusicInfo &other)
	{
		this->title = other.title;
		this->artist = other.artist;
		this->albumName = other.albumName;
		this->albumImageUrl = other.albumImageUrl;
		this->url = other.url;
		this->rate = other.rate;
	}

	virtual ~XMusicInfo() {}
};

XMR_END_NS

#endif // __X_MUSIC_INFO_H__
