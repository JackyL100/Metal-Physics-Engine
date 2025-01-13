//
//  metal_view_delegate.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/1/25.
//
#pragma once
#include "renderer.hpp"

class MyMTKViewDelegate : public MTK::ViewDelegate
{
    public:
        MyMTKViewDelegate( MTL::Device* pDevice );
        virtual ~MyMTKViewDelegate() override;
        virtual void drawInMTKView( MTK::View* pView ) override;

    private:
        Renderer* _pRenderer;
};
