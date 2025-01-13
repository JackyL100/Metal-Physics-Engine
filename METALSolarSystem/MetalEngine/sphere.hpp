//
//  sphere.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/8/25.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "shape.hpp"
#include "matrix_math.hpp"
#include <simd/simd.h>

class Sphere : public Shape
{
public:
    constexpr static shader_types::SphereVertexData vertices[] = { // triangle that contains circle
        {{-1.0f, 1.0f, 0.f}},
        {{0.f, -1.0f, 0.f}},
        {{1.0f, 1.0f, 0.f}}
    };
    
    Sphere(float size_, simd::float3 center_);
    virtual shader_types::InstanceData getWorldToScreenTransform(simd::float3 cameraPosition);
};

#endif
