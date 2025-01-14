//
//  shader_types.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/6/25.
//

#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

#include <simd/simd.h>

namespace shader_types
{
    struct CubeVertexData
    {
        simd::float3 position;
        simd::float3 normal;
    };
    struct SphereVertexData
    {
        simd::float3 position;
    };
    struct CubeInstanceData
    {
        simd::float4x4 instanceTransform;
        simd::float3x3 instanceNormalTransform;
        simd::float4 instanceColor;
    };

    struct SphereInstanceData
    {
        simd::float3 color;
        simd::float3 center;
        float radius;

        float ambience;
        float diffuse;
        float specular;
        float shininess;
        
        simd::float4x4 instanceTransform;
    };
    struct CameraData
    {
        simd::float4x4 perspectiveTransform;
        simd::float4x4 worldTransform;
        simd::float3x3 worldNormalTransform;
        simd::float3 position;
        simd::float3 direction;
        float zoom;
    };
};

#endif
