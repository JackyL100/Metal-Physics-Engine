//
//  shape.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/6/25.
//

#ifndef MY_SHAPE_H
#define MY_SHAPE_H

#include "shader_types.hpp"
#include <simd/simd.h>

class Shape
{
public:
    float scale;
    simd::float3 center_in_world;
    Shape(float scale_, simd::float3 center_): scale(scale_), center_in_world(center_){}
    virtual shader_types::InstanceData getWorldToScreenTransform(simd::float3 cameraPosition) {shader_types::InstanceData a; return a;};
};

#endif
