//
//  sphere.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/8/25.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "shader_types.hpp"
#include "matrix_math.hpp"
#include <simd/simd.h>
#include <vector>

class Sphere
{
public:
    constexpr static shader_types::SphereVertexData vertices[] = { // triangle that contains circle
        {{-1.0f, 1.0f, 0.f}},
        {{0.f, -0.73205, 0.f}},
        {{1.0f, 1.0f, 0.f}}
    };
    
    float size;
    float radius;
    simd::float3 center;
    simd::float3 color;

    float ambience = 0.2;
    float diffuse = 0.7;
    float specular = 0.5;
    float shininess = 10000.0;
    Sphere(float size_, simd::float3 center_, simd::float3 color_);
    shader_types::SphereInstanceData getSphereStruct(simd::float3 cameraPosition, const std::vector<simd::float4x4>& objectTransforms);
    shader_types::SphereInstanceData getSphereStruct(simd::float3 cameraPosition);
};

#endif
