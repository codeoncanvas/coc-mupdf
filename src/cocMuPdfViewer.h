/**
 *
 *      ┌─┐╔═╗┌┬┐┌─┐
 *      │  ║ ║ ││├┤
 *      └─┘╚═╝─┴┘└─┘
 *   ┌─┐┌─┐╔╗╔┬  ┬┌─┐┌─┐
 *   │  ├─┤║║║└┐┌┘├─┤└─┐
 *   └─┘┴ ┴╝╚╝ └┘ ┴ ┴└─┘
 *
 * Copyright (c) 2016 Code on Canvas Pty Ltd, http://CodeOnCanvas.cc
 *
 * This software is distributed under the MIT license
 * https://tldrlegal.com/license/mit-license
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code
 *
 **/

//! \brief Basic wrapper for MuPDF library

#pragma once

#include "cinder/app/App.h"
#include <mupdf/fitz.h>

namespace coc {
    
class MuPdfViewer {

public:

    //! load document and generate textures for all pages
    void setup( std::string _assetPath, int _dpi = 72, float _zoom = 1.0f );

    void draw( int _page, ci::vec2 _pos );
    void draw( int _page, ci::Rectf _bounds );

    //! returns dimensions of first page texture
    ci::vec2 getSize();

    //! returns mupdf page count from doc load
    int getNumPages();

    //! returns loaded pages as textures
    std::vector<ci::gl::TextureRef> & getPages() { return pages; }

protected:

    void generateAll();
    void cleanup();

    std::vector<ci::gl::TextureRef> pages;
    int pageCount;

    fz_context *ctx;
    fz_document *doc;
    fz_matrix ctm;

};//class MuPdfViewer

}//namespace coc