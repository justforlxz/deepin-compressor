/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     dongsen <dongsen@deepin.com>
 *
 * Maintainer: dongsen <dongsen@deepin.com>
 *             AaronZhang <ya.zhang@archermind.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kpluginloader.h"
#include "kpluginmetadata.h"
#include "desktopfileparser_p.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLocale>
#include <QPluginLoader>
#include <QStringList>

//#include "kaboutdata.h"

class KPluginMetaDataPrivate : public QSharedData
{
public:
    QString metaDataFileName;
};

KPluginMetaData::KPluginMetaData()
{
}

KPluginMetaData::KPluginMetaData(const KPluginMetaData &other)
    : m_metaData(other.m_metaData)
    , m_fileName(other.fileName())
    , d(other.d)
{
}

KPluginMetaData &KPluginMetaData::operator=(const KPluginMetaData &other)
{
    m_metaData = other.m_metaData;
    m_fileName = other.m_fileName;
    d = other.d;
    return *this;
}

KPluginMetaData::~KPluginMetaData()
{
}

KPluginMetaData::KPluginMetaData(const QString &file)
{
    if (file.endsWith(QStringLiteral(".desktop"))) {
        loadFromDesktopFile(file, QStringList());
    } else if (file.endsWith(QStringLiteral(".json"))) {
        d = new KPluginMetaDataPrivate;
        QFile f(file);
        bool b = f.open(QIODevice::ReadOnly);
        if (!b) {
            return;
        }

        QJsonParseError error;
        m_metaData = QJsonDocument::fromJson(f.readAll(), &error).object();
        if (error.error) {
        }

        m_fileName = file;
        d->metaDataFileName = file;
    } else {
        QPluginLoader loader(file);
        m_fileName = QFileInfo(loader.fileName()).absoluteFilePath();
        m_metaData = loader.metaData().value(QStringLiteral("MetaData")).toObject();
    }
}

KPluginMetaData::KPluginMetaData(const QPluginLoader &loader)
{
    m_fileName = QFileInfo(loader.fileName()).absoluteFilePath();
    m_metaData = loader.metaData().value(QStringLiteral("MetaData")).toObject();
}

KPluginMetaData::KPluginMetaData(const KPluginLoader &loader)
{
    m_fileName = QFileInfo(loader.fileName()).absoluteFilePath();
    m_metaData = loader.metaData().value(QStringLiteral("MetaData")).toObject();
}

KPluginMetaData::KPluginMetaData(const QJsonObject &metaData, const QString &file)
{
    m_fileName = file;
    m_metaData = metaData;
}

KPluginMetaData::KPluginMetaData(const QJsonObject &metaData, const QString &pluginFile, const QString &metaDataFile)
{
    m_fileName = pluginFile;
    m_metaData = metaData;
    if (!metaDataFile.isEmpty()) {
        d = new KPluginMetaDataPrivate;
        d->metaDataFileName = metaDataFile;
    }
}

KPluginMetaData KPluginMetaData::fromDesktopFile(const QString &file, const QStringList &serviceTypes)
{
    KPluginMetaData result;
    result.loadFromDesktopFile(file, serviceTypes);
    return result;
}

void KPluginMetaData::loadFromDesktopFile(const QString &file, const QStringList &serviceTypes)
{
    QString libraryPath;
    if (!DesktopFileParser::convert(file, serviceTypes, m_metaData, &libraryPath)) {
        Q_ASSERT(!isValid());
        return; // file could not be parsed for some reason, leave this object invalid
    }

    d = new KPluginMetaDataPrivate;
    d->metaDataFileName = QFileInfo(file).absoluteFilePath();
    if (!libraryPath.isEmpty()) {
        // this was a plugin with a shared library
        m_fileName = libraryPath;
    } else {
        // no library, make filename point to the .desktop file
        m_fileName = d->metaDataFileName;
    }
}

QJsonObject KPluginMetaData::rawData() const
{
    return m_metaData;
}

QString KPluginMetaData::fileName() const
{
    return m_fileName;
}

QString KPluginMetaData::metaDataFileName() const
{
    return d ? d->metaDataFileName : m_fileName;
}

bool KPluginMetaData::isValid() const
{
    // it can be valid even if m_fileName is empty (as long as the plugin id is set in the metadata)
    qDebug() << pluginId().isEmpty() << m_metaData.isEmpty();
    return !pluginId().isEmpty() && !m_metaData.isEmpty();
}

bool KPluginMetaData::isHidden() const
{
    return rootObject()[QStringLiteral("Hidden")].toBool();
}

QJsonObject KPluginMetaData::rootObject() const
{
    return m_metaData[QStringLiteral("KPlugin")].toObject();
}

QStringList KPluginMetaData::readStringList(const QJsonObject &obj, const QString &key)
{
    const QJsonValue value = obj.value(key);
    if (value.isUndefined() || value.isObject() || value.isNull()) {
        return QStringList();
    } else if (value.isArray()) {
        return value.toVariant().toStringList();
    } else {
        QString asString = value.isString() ? value.toString() : value.toVariant().toString();
        if (asString.isEmpty()) {
            return QStringList();
        }

        const QString id = obj.value(QStringLiteral("KPlugin")).toObject().value(QStringLiteral("Id")).toString();
        return QStringList(asString);
    }
}

QJsonValue KPluginMetaData::readTranslatedValue(const QJsonObject &jo, const QString &key, const QJsonValue &defaultValue)
{
    QString languageWithCountry = QLocale().name();
    auto it = jo.constFind(key + QLatin1Char('[') + languageWithCountry + QLatin1Char(']'));
    if (it != jo.constEnd()) {
        return it.value();
    }

    const QStringRef language = languageWithCountry.midRef(0, languageWithCountry.indexOf(QLatin1Char('_')));
    it = jo.constFind(key + QLatin1Char('[') + language + QLatin1Char(']'));
    if (it != jo.constEnd()) {
        return it.value();
    }
    // no translated value found -> check key
    it = jo.constFind(key);
    if (it != jo.constEnd()) {
        return jo.value(key);
    }

    return defaultValue;
}

QString KPluginMetaData::readTranslatedString(const QJsonObject &jo, const QString &key, const QString &defaultValue)
{
    return readTranslatedValue(jo, key, defaultValue).toString(defaultValue);
}

QString KPluginMetaData::category() const
{
    return rootObject()[QStringLiteral("Category")].toString();
}

QString KPluginMetaData::description() const
{
    return readTranslatedString(rootObject(), QStringLiteral("Description"));
}

QString KPluginMetaData::iconName() const
{
    return rootObject()[QStringLiteral("Icon")].toString();
}

QString KPluginMetaData::license() const
{
    return rootObject()[QStringLiteral("License")].toString();
}

QString KPluginMetaData::name() const
{
    return readTranslatedString(rootObject(), QStringLiteral("Name"));
}

QString KPluginMetaData::copyrightText() const
{
    return readTranslatedString(rootObject(), QStringLiteral("Copyright"));
}

QString KPluginMetaData::extraInformation() const
{
    return readTranslatedString(rootObject(), QStringLiteral("ExtraInformation"));
}

QString KPluginMetaData::pluginId() const
{
    QJsonObject root = rootObject();
    auto nameFromMetaData = root.constFind(QStringLiteral("Id"));
    if (nameFromMetaData != root.constEnd()) {
        const QString id = nameFromMetaData.value().toString();
        if (!id.isEmpty()) {
            return id;
        }
    }

    // passing QFileInfo an empty string gives the CWD, which is not what we want
    if (m_fileName.isEmpty()) {
        return QString();
    }

    return QFileInfo(m_fileName).baseName();
}

QString KPluginMetaData::version() const
{
    return rootObject()[QStringLiteral("Version")].toString();
}

QString KPluginMetaData::website() const
{
    return rootObject()[QStringLiteral("Website")].toString();
}

QStringList KPluginMetaData::dependencies() const
{
    return readStringList(rootObject(), QStringLiteral("Dependencies"));
}

QStringList KPluginMetaData::serviceTypes() const
{
    return readStringList(rootObject(), QStringLiteral("ServiceTypes"));
}

QStringList KPluginMetaData::mimeTypes() const
{
    return readStringList(rootObject(), QStringLiteral("MimeTypes"));
}

QStringList KPluginMetaData::formFactors() const
{
    return readStringList(rootObject(), QStringLiteral("FormFactors"));
}

bool KPluginMetaData::isEnabledByDefault() const
{
    QJsonValue val = rootObject()[QStringLiteral("EnabledByDefault")];
    if (val.isBool()) {
        return val.toBool();
    } else if (val.isString()) {
        return val.toString() == QLatin1String("true");
    }

    return false;
}

QString KPluginMetaData::value(const QString &key, const QString &defaultValue) const
{
    const QJsonValue value = m_metaData.value(key);
    if (value.isString()) {
        return value.toString(defaultValue);
    } else if (value.isArray()) {
        const QStringList list = value.toVariant().toStringList();
        if (list.isEmpty()) {
            return defaultValue;
        }

        return list.join(QChar::fromLatin1(','));
    } else if (value.isBool()) {
        //" but it is a bool";
        return value.toBool() ? QStringLiteral("true") : QStringLiteral("false");
    }

    return defaultValue;
}

bool KPluginMetaData::operator==(const KPluginMetaData &other) const
{
    return m_fileName == other.m_fileName && m_metaData == other.m_metaData;
}

QObject *KPluginMetaData::instantiate() const
{
    return QPluginLoader(m_fileName).instance();
}
