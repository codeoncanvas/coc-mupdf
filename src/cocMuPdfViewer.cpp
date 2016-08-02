#include "cocMuPdfViewer.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace coc {

    
void MuPdfViewer::setup( std::string _assetPath, int _dpi, float _zoom ) {
        
    float zoom = _zoom; //Zoom level is in percent (100 percent is 72 dpi)
    float rotate = 0; //Rotation is in degrees clockwise

    
    //CREATE CONTEXT
    ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!ctx)
    {
        CI_LOG_E("cannot create mupdf context");
    }
    
    //REGISTER DEFAULT FILE TYPES
    fz_try(ctx)
    fz_register_document_handlers(ctx);
    fz_catch(ctx)
    {
        CI_LOG_E("cannot register document handlers: " << fz_caught_message(ctx));
        fz_drop_context(ctx);
    }
    
    //OPEN
    string path = getAssetPath(_assetPath).generic_string(); // path of PDF, XPS, CBZ or EPUB document to open
    CI_LOG_V("Opening: " << path );

    fz_try(ctx)
    doc = fz_open_document(ctx, &path[0]);
    fz_catch(ctx)
    {
        CI_LOG_E("cannot open document: " << fz_caught_message(ctx));
        fz_drop_context(ctx);
    }

    //COUNT PAGES
    fz_try(ctx)
    pageCount = fz_count_pages(ctx, doc);
    fz_catch(ctx)
    {
        CI_LOG_E("cannot count number of pages: " << fz_caught_message(ctx));
        fz_drop_document(ctx, doc);
        fz_drop_context(ctx);
    }
    CI_LOG_V("Document has num pages: "<<pageCount);
    
    //CALC MATRIX
    fz_scale(&ctm, zoom , zoom );
    fz_pre_rotate(&ctm, rotate);



    generateAll();

    cleanup();

    
}

void MuPdfViewer::generateAll()
{

    ivec2 lastSize;

    for (int i=0; i< pageCount; i++) {

        fz_pixmap *pix = nullptr;

        fz_try(ctx)
                        pix = fz_new_pixmap_from_page_number(ctx, doc, i, &ctm, fz_device_rgb(ctx), 0);
        fz_catch(ctx)
        {
            CI_LOG_E("cannot render page: " << fz_caught_message(ctx));
            fz_drop_document(ctx, doc);
            fz_drop_context(ctx);
            break;
        }

        if (pix) {
            Surface surface(pix->w, pix->h, false);
            if (i>0) {
                ivec2 newSize = surface.getSize();
                if (newSize != lastSize) {
                    CI_LOG_E("Page size different to last");
                }
                lastSize = newSize;
            }
            else {
                lastSize = surface.getSize();
            }
            memcpy( surface.getData(), pix->samples, pix->w * pix->h * 3);
            pages.push_back( gl::Texture::create( surface ) );

            fz_drop_pixmap(ctx, pix);//cleanup
        }
    }
}

void MuPdfViewer::cleanup()
{
    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
}


void MuPdfViewer::draw( int _page, vec2 _pos ) {
    if ( _page > pages.size() ) {
        CI_LOG_E("Page index out of bounds");
        return;
    }
    gl::draw( pages[_page-1], _pos );
}

void MuPdfViewer::draw( int _page, ci::Rectf _bounds ){
    if ( _page > pages.size() ) {
        CI_LOG_E("Page index out of bounds");
        return;
    }
    gl::draw( pages[_page-1], _bounds );
}

ci::vec2 MuPdfViewer::getSize(){
    if(pages.size()) {
        return pages[0]->getSize();
    }
    return vec2(0,0);
}

int MuPdfViewer::getNumPages() {
    return pageCount;
}


    
}//namespace coc