/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_RADIO_H__
#define __X_RADIO_H__

#include <QObject>
#include <QMap>
#include <xglobal.h>
#include <radio/xmusicinfo.h>
#include <radio/xradiostyle.h>

#include "radio_export.h"

XMR_BEGIN_NS

class XRadioService;
class XRadioPrivate;

class RADIO_EXPORT XRadio : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(XRadio)
	Q_DISABLE_COPY(XRadio)
	friend class XRadioFactoryPrivate;
private:
	XRadio(XRadioService *service, QObject *parent = 0);
public:
	~XRadio();

public:
	QString name();
	bool nextMusic(XMusicInfo &music);
	void rateMusic(int rate);
	QList<XRadioStyle *> radioStyles();
	bool changeStyle(XRadioStyle *style);
	void login(const QMap<QString, QVariant> &auths);

Q_SIGNALS:
	void musicAvailable(const XMusicInfo &music);
	void stateChanged(int state, int code);

private:
	XRadioPrivate *d_ptr;
};

XMR_END_NS

#endif // __X_RADIO_H__