#pragma once

#include <core/co_class.h>
#include <iplugin.h>

#include <libdjvu/ddjvuapi.h>

#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace RProto {
    class IBook;
}

class DjVuBook;

class DjVuPlugin : public Boss::CoClass<Boss::MakeId("DjVuLibre.DjVuPlugin"), RProto::IPlugin>
{
public:
    DjVuPlugin();
    virtual ~DjVuPlugin();

    //IPlugin interface
    virtual RProto::IBook* createBook(const char* file) override;
    virtual const std::vector<std::string>& fileExtensions() override;

    //Plugin private
    void removeBook(ddjvu_document_t* book);

private:
    std::vector<std::string> extensions;
    ddjvu_context_t     *djvu_context;
    std::unordered_map<ddjvu_document_t*, DjVuBook*> bookTable;

    std::thread     message_thread;
    std::mutex      msgMutex;
    std::condition_variable msgConditional;
    volatile bool stopListener;

    void messageLoop();
    static void msg_callback(ddjvu_context_t *context, void *closure);
};
