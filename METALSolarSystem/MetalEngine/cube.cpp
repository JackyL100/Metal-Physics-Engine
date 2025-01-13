//
//  cube.cpp
//  METALCube::solarCube::syCube::stem
//
//  Created by Jacky Lei on 1/7/25.
//

#include "cube.hpp"

Cube::Cube(float size_, simd::float3 center_) : Shape(size_, center_)
{
    
}

shader_types::InstanceData Cube::getWorldToScreenTransform(simd::float3 cameraPosition)
{
    shader_types::InstanceData instanceData;
    simd::float4x4 scaleTransform = math::makeScale( (simd::float3){ scale, scale, scale } );
    simd::float4x4 cameraTranslate = math::makeTranslate( math::add( cameraPosition, center_in_world ) );
    instanceData.instanceTransform = cameraTranslate * scaleTransform; // camera_rotation * camera_translate * object+transformations * scale
    instanceData.instanceNormalTransform = math::discardTranslation(instanceData.instanceTransform);
    instanceData.instanceColor = (simd::float4){1.0f, 0.0f, 0.0f, 1.0f};
    return instanceData;
}
