//
//  sphere_shader.metal
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/8/25.
//

#include <metal_stdlib>
#include "shader_types.hpp"
using namespace metal;

struct spherePix
{
    float4 position [[position]];
    float3 local_space;
    float3 center;
//    float3 normal;
    half3 color;
};

//struct InstanceData
//{
//    float4x4 instanceTransform;
//    float3x3 instanceNormalTransform;
//    float4 instanceColor;
//};

//struct CameraData
//{
//    float4x4 perspectiveTransform;
//    float4x4 worldTransform;
//    float3x3 worldNormalTransform;
//};

spherePix vertex sphereVertexMain( device const shader_types::SphereVertexData* vertexData [[buffer(0)]],
                        device const shader_types::InstanceData* instanceData [[buffer(1)]],
                        device const shader_types::CameraData& cameraData [[buffer(2)]],
                        uint vertexId [[vertex_id]],
                        uint instanceId [[instance_id]])
{
    
    spherePix o;
    const device shader_types::SphereVertexData& vd = vertexData[vertexId];
    float4 pos = float4(vd.position, 1.0);
//    o.local_space = float3(pos[0], pos[1], sqrt(1 - pow(pos[0], 2) - pow(pos[1], 2)));
    o.local_space = float3(pos[0], pos[1], pos[2]);
    pos = instanceData[ instanceId ].instanceTransform * pos;
    pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
    o.position = pos;
//    float3 normal = instanceData[instanceId].instanceNormalTransform * vd.normal;
//    normal = cameraData.worldNormalTransform * normal;
//    o.normal = normal;
    o.color = half3 ( instanceData[ instanceId ].instanceColor.rgb );
    return o;
}

half4 fragment sphereFragmentMain( spherePix in [[stage_in]] )
{
    if (length(in.local_space) < 0.3)
    {
//        float3 normal = normalize(in.local_space - in.center);
//        float3 light = normalize(float3(1.0,1.0,0.8));
//        float ndotl = saturate(dot(normal, light));
//        return half4(in.color * 0.1 + in.color * ndotl, 1.0);
        return half4(in.color, 1.0);
    } else
    {
        discard_fragment();
    }
}



