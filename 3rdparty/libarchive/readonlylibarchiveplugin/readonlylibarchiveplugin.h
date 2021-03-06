#ifndef READONLYLIBARCHIVEPLUGIN_H
#define READONLYLIBARCHIVEPLUGIN_H

#include "../libarchive/libarchiveplugin.h"
#include "kpluginfactory.h"


class ReadOnlyLibarchivePluginFactory : public KPluginFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.KPluginFactory" FILE "kerfuffle_libarchive_readonly.json")
    Q_INTERFACES(KPluginFactory)
public:
    explicit ReadOnlyLibarchivePluginFactory();
    ~ReadOnlyLibarchivePluginFactory();
};


class ReadOnlyLibarchivePlugin : public LibarchivePlugin
{
    Q_OBJECT

public:
    explicit ReadOnlyLibarchivePlugin(QObject *parent, const QVariantList &args);
    ~ReadOnlyLibarchivePlugin() override;


};

#endif // READONLYLIBARCHIVEPLUGIN_H
