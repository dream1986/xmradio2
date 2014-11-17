/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QtTest/QtTest>

#include "core/xunzip.h"

XMR_USE_NS

class test_XUnZip : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void basicTest();
	void invalidFileTest();
};

void test_XUnZip::basicTest()
{
	XUnZip unzip(QFINDTESTDATA("test.zip"));

	XUnZip::Status s = unzip.status();
	QVERIFY(s == XUnZip::NoError);

	int error = 0;
	QByteArray data = unzip.getData("text.txt", &error);
	QCOMPARE(error, int(XUnZip::NoError));
	QCOMPARE(data.size(), 11);

	data = unzip.getData("/test.txt");
	s = unzip.status();
	QVERIFY(data.isEmpty() && s == XUnZip::FileNotFound);
}

void test_XUnZip::invalidFileTest()
{
	{
		XUnZip unzip("invalidfile.xxx");
		QVERIFY(unzip.status() == XUnZip::FileError);

		int error = 0;
		unzip.getData("text.txt", &error);
		QCOMPARE(error, int(XUnZip::FileError));
	}

	{
		XUnZip unzip(QFINDTESTDATA("CMakeLists.txt"));
		QVERIFY(unzip.status() == XUnZip::FileError);
	}
}

QTEST_GUILESS_MAIN(test_XUnZip)
#include "test_xunzip.moc"
