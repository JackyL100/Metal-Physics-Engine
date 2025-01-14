//
//  sphere.cpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/8/25.
//

#include "sphere.hpp"

Sphere::Sphere(float size_, simd::float3 center_, simd::float3 color_) : size{size_}, center{center_}, color{color_}, radius{size_ / 4.0f}
{
}

shader_types::SphereInstanceData Sphere::getSphereStruct(simd::float3 cameraPosition, const std::vector<simd::float4x4>& objectTransforms)
{
    simd::float4x4 scaleTransform = math::makeScale( (simd::float3){ size, size, size } );
    simd::float4x4 cameraTranslate = math::makeTranslate( math::add( cameraPosition, center ) );
    simd::float4x4 instanceTransform = cameraTranslate;
    for (int i = 0; i < objectTransforms.size(); i++)
    {
        instanceTransform = instanceTransform * objectTransforms[i];
    }
    instanceTransform = instanceTransform * scaleTransform;
    return {color, center, radius, ambience, diffuse, specular, shininess, instanceTransform};
}

shader_types::SphereInstanceData Sphere::getSphereStruct(simd::float3 cameraPosition)
{
    simd::float4x4 scaleTransform = math::makeScale( (simd::float3){ size, size, size } );
    simd::float4x4 cameraTranslate = math::makeTranslate( math::add( cameraPosition, center ) );
    simd::float4x4 instanceTransform = cameraTranslate;
    instanceTransform = instanceTransform * scaleTransform;
    return {color, center, radius, ambience, diffuse, specular, shininess, instanceTransform};
}
