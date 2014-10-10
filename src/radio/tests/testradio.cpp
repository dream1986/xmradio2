/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QCoreApplication>
#include <QEventLoop>

#include <iostream>

#include <radio/xradio.h>
#include <radio/xradioservice.h>
#include <radio/xradiofactory.h>

using namespace std;
XMR_USE_NS

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	XRadioFactory *factory = XRadioFactory::instance();
	const QList<XRadio *>& list = factory->availableRadio();

	if (list.count() == 0)
	{
		cout << "no plugin found" << endl;
		return -1;
	}

	cout << "found plugins: " << endl;
	foreach (XRadio *radio, list)
	{
		QString name = radio->name();
		cout << "--" << name.toStdString() << endl;
		cout << "    Radio Styles:" << endl;
		QList<XRadioStyle *> styles = radio->radioStyles();
		foreach(XRadioStyle *style, styles)
		{
			cout << "    |--" << style->name().toStdString() << endl;
			foreach (XRadioStyle *subsytle, style->subStyles())
			{
				cout << "      |--" << subsytle->name().toStdString() << endl;
			}
		}

		radio->changeStyle(styles[1]);
		XMusicInfo music;
		if (!radio->nextMusic(music))
		{
			cout << "No music available" << endl;
		}
		else
		{
			cout << "MUSIC TITLE: " << music.title.toStdString() << endl;
		}
	}

	return 0;
}
