#include "library.h"
#include <fpdfview.h>

Library::Library()
{
}

Library::~Library(){
    Close();
}

void Library::Init(){
    FPDF_InitLibrary(nullptr);
}

void Library::Close(){
    FPDF_DestroyLibrary();
}

