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
    struct InstanceData
    {
        simd::float4x4 instanceTransform;
        simd::float3x3 instanceNormalTransform;
        simd::float4 instanceColor;
    };

    struct CameraData
    {
        simd::float4x4 perspectiveTransform;
        simd::float4x4 worldTransform;
        simd::float3x3 worldNormalTransform;
    };
};

#endif
