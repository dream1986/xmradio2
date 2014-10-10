/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_RADIO_SERVICE_H__
#define __X_RADIO_SERVICE_H__

#include <QObject>
#include <QList>
#include <QMap>
#include <QVariant>

#include <xglobal.h>
#include <radio/xmusicinfo.h>
#include <radio/xradiostyle.h>

XMR_BEGIN_NS

class XRadioService : public QObject
{
	Q_OBJECT
protected:
	XRadioService(QObject *parent = 0)
		: QObject(parent)
	{}

public:
	virtual ~XRadioService() {}

	virtual QString name() = 0;
	virtual QList<XRadioStyle *> radioStyles() = 0;
	virtual bool nextMusic(XMusicInfo &music) = 0;
	virtual void rateMusic(int rate) = 0;
	virtual bool changeStyle(XRadioStyle *style) = 0;
	virtual void login(const QMap<QString, QVariant> &auths) = 0;
	virtual bool isLoggedIn() = 0;
	virtual void logout() = 0;

	enum State
	{
		LoggedIn,
		LoggedOut,
		LoginFailed,
		NetworkError,
		NoData,
	};

	enum AuthType
	{
		AuthType_String,
		AuthType_Number,
		AuthType_Password,
	};

Q_SIGNALS:
	void musicAvailable(const XMusicInfo &music);
	void styleChanged(XRadioStyle *style);
	void stateChanged(int state, int code);
	void needLogin(const QMap<QString, QPair<QString, int> > &authKeys);
};

XMR_END_NS

#endif // __X_RADIO_SERVICE_H__
