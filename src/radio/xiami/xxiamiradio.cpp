/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QEventLoop>
#include <QScopedPointer>
#include <QDomDocument>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

#include "xxiamiradio.h"

#define _MY_RADIO_NAME		"我的电台"
#define _MY_RADIO_URL		"http://www.xiami.com/radio/xml/type/4/id/"

#define _XIAMI_GUESS_NAME	"虾米猜"
#define _XIAMI_GUESS_URL	"http://www.xiami.com/radio/xml/type/8/id/"

//#define _ARTIST_RADIO_NAME	"艺人电台"
//#define _ARTIST_RADIO_URL	"http://www.xiami.com/radio/xml/type/5/id/"

#define _LOGIN_URL	"http://www.xiami.com/kuang/login"
#define _LOGOUT_URL	"http://www.xiami.com/member/logout"

/*
  Encoded url:
	4h%2Ff.moF5F%2%33%_6_pFhy%b6895eeE544%5EElt3Fmixim3E15F5%9524l3a_%52768e862E1%5E--ltA%5li.%4%3E3E24E35.%uk3Ec5f523%c-25E%%np%2.eac2%24%32F3245m3teD5fa67545%16E%55u

  Encode array: (length -> 1 + 163, row -> 4, col -> 41)
	 4 -> indicates row count
	 h % 2 F f . m o F 5 F % 2 % 3 3 % _ 6 _ p F h y % b 6 8 9 5 e e E 5 4 4 % 5 E E l
	 t 3 F m i x i m 3 E 1 5 F 5 % 9 5 2 4 l 3 a _ % 5 2 7 6 8 e 8 6 2 E 1 % 5 E - - l
	 t A % 5 l i . % 4 % 3 E 3 E 2 4 E 3 5 . % u k 3 E c 5 f 5 2 3 % c - 2 5 E % % n
	 p % 2 . e a c 2 % 2 4 % 3 2 F 3 2 4 5 m 3 t e D 5 f a 6 7 5 4 5 % 1 6 E % 5 5 u
  First round decode:
	http%3A%2F%2Fm5.file.xiami.com%2F34%5E%2F134%5E%2F33%5E23%2F3943%5E2_2346455_l.mp3%3Fauth_key%3D%5E5b2cf675a86f698575e25e834e6%5E2c%5E-141264%5E%5E%5E%5E-%5E-null
  After unescape & replace '^' to '0':
	http://m5.file.xiami.com/340/1340/33023/394302_2346455_l.mp3?auth_key=05b2cf675a86f698575e25e834e602c0-1412640000-0-null
*/
static QString _url_decode(const QString &url)
{
	if (url.isEmpty())
		return QString();

	int row = url[0].unicode() - '0';
	int len = url.length() - 1;
	int col = len / row;
	int x = len % row;

	if (x != 0)
		col += 1;

	QString strDecodeUrl(len + 1, 0);
	for(int i = 0, k = 1; i < row; ++i)
	{
		for(int j = 0; j < col; ++j, ++k)
			strDecodeUrl[i + row * j] = url[k];

		if (x > 0)
		{
			x--;
			if(x == 0)
				col -= 1;
		}
	}

	strDecodeUrl.replace("%3A", ":", Qt::CaseInsensitive);
	strDecodeUrl.replace("%2F", "/", Qt::CaseInsensitive);

	QUrl qUrl(strDecodeUrl);
	strDecodeUrl = qUrl.toDisplayString(QUrl::FullyDecoded);

	strDecodeUrl.replace("%3F", "?", Qt::CaseInsensitive);
	strDecodeUrl.replace("%3D", "=", Qt::CaseInsensitive);
	strDecodeUrl.replace('^', '0');

	return strDecodeUrl;
}

XMR_BEGIN_NS

struct XRadioStyleXiaMi : public XRadioStyle
{
	friend class XXiaMiRadioPrivate;
	QString url;

	XRadioStyleXiaMi(const QString &name,
					 const QString &logo = QString(),
					 const QString &url = QString())
		: XRadioStyle(name, logo)
		, url(url)
	{
	}

	~XRadioStyleXiaMi()
	{
		foreach (XRadioStyle *style, m_substyles)
			delete style;
	}
};

class XXiaMiRadioPrivate : public QObject
{
	Q_OBJECT
	Q_DECLARE_PUBLIC(XXiaMiRadio)
	XXiaMiRadio *q_ptr;
public:
	XXiaMiRadioPrivate();
	~XXiaMiRadioPrivate();

	void initStyles();
	bool initSubStyles(XRadioStyleXiaMi *style, const QString &url);

	void getPlaylist();

	bool changeStyle(const QList<XRadioStyle *> &styles, XRadioStyle *style);
	void logout();
	void login(const QString &email, const QString &pwd);

	enum NM_Operation
	{
		NM_None,
		NM_Playlist,
		NM_Login,
		NM_Logout,
	};

	void parsePlaylist(QNetworkReply *reply);
	void parseLogin(QNetworkReply *reply);
	void parseLogout(QNetworkReply *reply);

	void emitNeedLogin();

private Q_SLOTS:
	void replyFinished(QNetworkReply *reply);

private:
	QList<XRadioStyle *> radioStyles;
	QList<XMusicInfo> playlist;

	XRadioStyleXiaMi *currentStyle;
	XRadioStyleXiaMi *defaultStyle;

	QNetworkAccessManager *network;
	NM_Operation curOperation;

	QString userId;
	bool isLoggedIn;
};

XXiaMiRadioPrivate::XXiaMiRadioPrivate()
	: curOperation(NM_None)
	, isLoggedIn(false)
	, currentStyle(NULL)
	, defaultStyle(NULL)
	, network(new QNetworkAccessManager(this))
{
	connect(network, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
}

XXiaMiRadioPrivate::~XXiaMiRadioPrivate()
{
	foreach (XRadioStyle *style, radioStyles)
	{
		delete style;
	}
}

void XXiaMiRadioPrivate::initStyles()
{
	radioStyles.clear();

	radioStyles.push_back(new XRadioStyleXiaMi(_MY_RADIO_NAME, "", _MY_RADIO_URL));
	radioStyles.push_back(new XRadioStyleXiaMi(_XIAMI_GUESS_NAME, "", _XIAMI_GUESS_URL));
	// radioStyles.push_back(new XRadioStyleXiaMi(_ARTIST_RADIO_NAME, "", _ARTIST_RADIO_URL));

	const char *radios[] =
	{
		"星座电台", "http://www.xiami.com/kuang/radio/c/1",
		"年代电台", "http://www.xiami.com/kuang/radio/c/2",
		"风格电台", "http://www.xiami.com/kuang/radio/c/12",
	};

	for (int i = 0; i < countof(radios); i += 2)
	{
		XRadioStyleXiaMi *style = new XRadioStyleXiaMi(radios[i]);
		if (initSubStyles(style, radios[i + 1]))
			radioStyles.push_back(style);
		else
			delete style;
	}

	if (defaultStyle == NULL)
	{
		defaultStyle = dynamic_cast<XRadioStyleXiaMi *>(radioStyles[0]);
	}

	currentStyle = defaultStyle;
}

bool XXiaMiRadioPrivate::initSubStyles(XRadioStyleXiaMi *style, const QString &url)
{
	QScopedPointer<QNetworkAccessManager> manager(new QNetworkAccessManager);
	QEventLoop loop;

	connect(manager.data(), SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
	QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
	loop.exec();

	if (reply->error() != QNetworkReply::NoError)
	{
		emit q_func()->stateChanged(XRadioService::NetworkError, reply->error());
		return false;
	}

	QDomDocument doc;
	if (!doc.setContent(reply->readAll()))
		return false;

	QDomElement root = doc.firstChildElement();

	if (root.tagName() != "radioList")
	{
		qWarning() << "Unexpected tag name: " << root.tagName();
	}

	QDomNodeList radios = root.elementsByTagName("radio");
	if (radios.count() == 0)
		return false;

	for (int i = 0; i < radios.count(); ++i)
	{
		QDomNode node = radios.at(i);
		if (node.isElement())
		{
			QDomElement radio = node.toElement();

			// int id = radio.firstChildElement("radio_id").text().toInt();
			QString name = radio.firstChildElement("radio_name").text().replace("&amp;", "&");
			QString logo = radio.firstChildElement("radio_logo").text();
			QString url = radio.firstChildElement("radio_url").text();

			XRadioStyleXiaMi *substyle = new XRadioStyleXiaMi(name, logo, url);
			style->m_substyles.push_back(substyle);

			// 新歌电台
			if (defaultStyle == NULL && url == "http://www.xiami.com/kuang/xml/type/6/id/0")
			{
				defaultStyle = substyle;
			}
		}
	}

	return true;
}

void XXiaMiRadioPrivate::getPlaylist()
{
	if (currentStyle == NULL || curOperation != NM_None)
		return ;

	QString url = currentStyle->url;
	if ((currentStyle->url == QLatin1String(_MY_RADIO_URL)
			|| currentStyle->url == QLatin1String(_XIAMI_GUESS_URL)
			) && currentStyle->subStyles().isEmpty()
		)
	{
		if (!isLoggedIn)
		{
			emitNeedLogin();
			return ;
		}
		else
		{
			url += userId;
		}
	}

	curOperation = NM_Playlist;
	network->get(QNetworkRequest(url));
}

bool XXiaMiRadioPrivate::changeStyle(const QList<XRadioStyle *> &styles, XRadioStyle *style)
{
	if (styles.isEmpty() || style == NULL)
		return false;

	if (style == currentStyle)
		return true;

	if (style == defaultStyle)
	{
		currentStyle = defaultStyle;
		emit q_func()->styleChanged(style);
		return true;
	}

	XRadioStyleXiaMi *ls = dynamic_cast<XRadioStyleXiaMi *>(style);
	if (ls == 0)
		return false;

	foreach (XRadioStyle *s, styles)
	{
		XRadioStyleXiaMi *rs = dynamic_cast<XRadioStyleXiaMi *>(s);
		if (style == s /* || *ls == *rs */)
		{
			currentStyle = ls;
			emit q_func()->styleChanged(style);
			return true;
		}

		if (changeStyle(rs->m_substyles, style))
			return true;
	}

	return false;
}

void XXiaMiRadioPrivate::logout()
{
	if (!isLoggedIn || curOperation != NM_None)
		return ;

	curOperation = NM_Logout;
	network->get(QNetworkRequest(QUrl(_LOGOUT_URL)));
}

void XXiaMiRadioPrivate::login(const QString &email, const QString &pwd)
{
	if (curOperation != NM_None)
		return ;

	curOperation = NM_Login;
	QString strPostData = QString("password=%1&email=%2").arg(pwd).arg(email);

	QNetworkRequest request(QUrl(_LOGIN_URL));
	request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
	network->post(request, strPostData.toLocal8Bit());
}

void XXiaMiRadioPrivate::replyFinished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError)
	{
		emit q_func()->stateChanged(XRadioService::NetworkError, reply->error());
		return ;
	}

	switch (curOperation)
	{
	case NM_Playlist:
		parsePlaylist(reply);
		break;

	case NM_Login:
		parseLogin(reply);
		break;

	case NM_Logout:
		parseLogout(reply);
		break;
	}

	curOperation = NM_None;
}

void XXiaMiRadioPrivate::parsePlaylist(QNetworkReply *reply)
{
	Q_Q(XXiaMiRadio);

	QDomDocument doc;
	if (!doc.setContent(reply->readAll()))
	{
		emit q->stateChanged(XRadioService::NoData, 0);
		return ;
	}

	QDomNodeList tracks = doc.elementsByTagName("track");
	if (tracks.count() == 0)
	{
		qWarning() << "No <track> tags found";
		emit q->stateChanged(XRadioService::NoData, 0);
		return ;
	}

	QList<XMusicInfo> newPlaylist;
	for (int i = 0; i < tracks.count(); ++i)
	{
		QDomNode node = tracks.at(i);
		if (node.isElement())
		{
			QDomElement track = node.toElement();
			XMusicInfo music;
			music.url = _url_decode(track.firstChildElement("location").text());
			if (music.url.isEmpty())
				continue ;

			QDomElement e = track.firstChildElement("song_name");
			if (e.isNull())
				e = track.firstChildElement("title");
			music.title = e.text();

			e = track.firstChildElement("artist_name");
			if (e.isNull())
				e = track.firstChildElement("artist");
			music.artist = e.text();

			music.albumName = track.firstChildElement("album_name").text();

			e = track.firstChildElement("album_cover");
			if (e.isNull())
				e = track.firstChildElement("pic");
			music.albumImageUrl = e.text();
			music.rate = track.firstChildElement("grade").text().toInt();

			newPlaylist.append(music);
		}
	}

	if (!newPlaylist.isEmpty())
	{
		playlist << newPlaylist;
		emit q->musicAvailable();
	}
}

void XXiaMiRadioPrivate::parseLogout(QNetworkReply *reply)
{
	isLoggedIn = false;

	playlist.clear();
	network->clearAccessCache();
	network->setCookieJar(new QNetworkCookieJar);

	emit q_func()->stateChanged(XRadioService::LoggedOut, 0);

	if (currentStyle && currentStyle != defaultStyle &&
		(currentStyle->url == QLatin1String(_MY_RADIO_URL)
		 || currentStyle->url == QLatin1String(_XIAMI_GUESS_URL)
		 ) && currentStyle->subStyles().isEmpty()
		)
	{
		currentStyle = defaultStyle;
		emit q_func()->styleChanged(currentStyle);
	}
}

void XXiaMiRadioPrivate::parseLogin(QNetworkReply *reply)
{
	Q_Q(XXiaMiRadio);

	do
	{
		QDomDocument doc;
		if (!doc.setContent(reply->readAll()))
			break ;

		QDomElement root = doc.firstChildElement("login");
		if (root.isNull())
			break ;

		QString strErrMsg = root.firstChildElement("message").text();
		QDomElement e = root.firstChildElement("status");
		bool ok = false;
		int status = e.text().toInt(&ok);
		if (status != 0)
			break ;

		root = root.firstChildElement("user");
		e = root.firstChildElement("user_id");
		userId = e.text();

	} while (0);

	if (!userId.isEmpty())
	{
		isLoggedIn = true;
		emit q->stateChanged(XRadioService::LoggedIn, 0);
	}
	else
	{
		emit q->stateChanged(XRadioService::LoginFailed, 0);
	}
}

void XXiaMiRadioPrivate::emitNeedLogin()
{
	QMap<QString, QPair<QString, int> > auths;
	auths["EMAIL"] = QPair<QString, int>(tr("Email: "), XRadioService::AuthType_String);
	auths["PASS"] = QPair<QString, int>(tr("Password: "), XRadioService::AuthType_Password);

	emit q_func()->needLogin(auths);
}

// ===============================================================

XXiaMiRadio::XXiaMiRadio(QObject *parent /* = NULL */)
	: XRadioService(parent)
	, d_ptr(new XXiaMiRadioPrivate)
{
	d_ptr->q_ptr = this;
	d_ptr->initStyles();
}

XXiaMiRadio::~XXiaMiRadio()
{
	delete d_ptr;
}

QString XXiaMiRadio::name()
{
	return tr("XiaMi Radio");
}

bool XXiaMiRadio::nextMusic(XMusicInfo &music)
{
	Q_D(XXiaMiRadio);
	if (d->playlist.isEmpty())
	{
		d->getPlaylist();
		return false;
	}

	music = d->playlist.takeLast();

	return true;
}

void XXiaMiRadio::rateMusic(int rate)
{
}

QList<XRadioStyle *> XXiaMiRadio::radioStyles()
{
	return d_func()->radioStyles;
}

XRadioStyle* XXiaMiRadio::defaultRadioStyle()
{
	return d_func()->defaultStyle;
}

bool XXiaMiRadio::changeStyle(XRadioStyle *style)
{
	return d_func()->changeStyle(d_func()->radioStyles, style);
}

void XXiaMiRadio::login(const QMap<QString, QVariant> &auths)
{
	QString strPwd = auths["PASS"].toString();
	QString strEmail = auths["EMAIL"].toString();

	d_func()->login(strEmail, strPwd);
}

bool XXiaMiRadio::isLoggedIn()
{
	return d_func()->isLoggedIn;
}

void XXiaMiRadio::logout()
{
	d_func()->logout();
}

XMR_END_NS

#include "xxiamiradio.moc"
