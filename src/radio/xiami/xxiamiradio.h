/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_XIAMI_RADIO_H__
#define __X_XIAMI_RADIO_H__

#include <radio/xradioservice.h>

XMR_BEGIN_NS

class XXiaMiRadioPrivate;

class XXiaMiRadio : public XRadioService
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(XXiaMiRadio)
public:
	XXiaMiRadio(QObject *parent = 0);
	~XXiaMiRadio();

public:
	QString name();
	QList<XRadioStyle *> radioStyles();
	XRadioStyle* defaultRadioStyle();
	bool nextMusic(XMusicInfo &music);
	void rateMusic(int rate);
	bool changeStyle(XRadioStyle *style);
	void login(const QMap<QString, QVariant> &auths);
	bool isLoggedIn();
	void logout();

private:
	XXiaMiRadioPrivate *d_ptr;
};

XMR_END_NS

#endif // __X_XIAMI_RADIO_H__
