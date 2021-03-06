#include <bookfactory.h>
#include <iplugin.h>
#include <ibook.h>

#include <core/module.h>
#include <core/base.h>
#include "../lib/plugins/pdfium/src/plugin.h"

#include <QFileInfo>

using namespace RProto;

BookFactory::BookFactory()
{
    //auto ptr = Boss::CreateObject<RProto::IPlugin>(Boss::MakeId("PDFium.Plugin"));
    auto ptr = Boss::Base<Plugin>::CreatePtr();
    ptr->AddRef();
    registerPlugin(ptr);
}

BookFactory::~BookFactory()
{
    for(IPlugin* plug : pluginTable.values()){
        plug->Release();
    }
}

bool BookFactory::registerPlugin(IPlugin* plug)
{
    if(plug == nullptr)
        return false;

    for(const std::string& str : plug->fileExtensions()){
        pluginTable.insert(str.data(), plug);
    }
    return true;
}

QStringList BookFactory::supportedExtensions()
{
    return pluginTable.keys();
}
IBook* BookFactory::createBook(const QString& path)
{
    QFileInfo finfo(path);
    if(finfo.exists() && finfo.isFile()){
        auto it = pluginTable.find(finfo.suffix());
        if(it != pluginTable.end()){
            IPlugin* plug = it.value();
            return plug->createBook(path.toLocal8Bit().constData());
        }
    }
    return nullptr;
}
