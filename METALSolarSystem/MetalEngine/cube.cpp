//
//  cube.cpp
//  METALCube::solarCube::syCube::stem
//
//  Created by Jacky Lei on 1/7/25.
//

#include "cube.hpp"

Cube::Cube(float size_, simd::float3 center_) : size{size_}, center{center_}
{
    
}

shader_types::CubeInstanceData Cube::getWorldToScreenTransform(simd::float3 cameraPosition)
{
    
    shader_types::CubeInstanceData instanceData;
    simd::float4x4 scaleTransform = math::makeScale( (simd::float3){ size, size, size } );
    simd::float4x4 cameraTranslate = math::makeTranslate( math::add( cameraPosition, center ) );
    instanceData.instanceTransform = cameraTranslate * scaleTransform; // camera_rotation * camera_translate * object+transformations * scale
    instanceData.instanceNormalTransform = math::discardTranslation(instanceData.instanceTransform);
    instanceData.instanceColor = (simd::float4){1.0f, 0.0f, 0.0f, 1.0f};
    return instanceData;
}
