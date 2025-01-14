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
    float3 local_position;
    float radius;
    float3 center;
    half3 color;

    float ambience;
    float diffuse;
    float specular;
    float shininess;
    
    float3 cameraPosition;
    float zoom;
};

spherePix vertex sphereVertexMain( device const shader_types::SphereVertexData* vertexData [[buffer(0)]],
                        device const shader_types::SphereInstanceData* instanceData [[buffer(1)]],
                        device const shader_types::CameraData& cameraData [[buffer(2)]],
                        uint vertexId [[vertex_id]],
                        uint instanceId [[instance_id]])
{
    spherePix o;
    o.radius = instanceData[instanceId].radius;
    o.center = instanceData[instanceId].center;
    o.color = half3(instanceData[instanceId].color);
    float4 position = float4(vertexData[vertexId].position, 1.0);
    o.local_position = vertexData[vertexId].position;
    position = instanceData[instanceId].instanceTransform * position;
    position = cameraData.perspectiveTransform * cameraData.worldTransform * position;
    o.position = position;
    o.ambience = instanceData[instanceId].ambience;
    o.diffuse = instanceData[instanceId].diffuse;
    o.specular = instanceData[instanceId].specular;
    o.shininess = instanceData[instanceId].shininess;
    o.cameraPosition = cameraData.position;
    o.zoom = cameraData.zoom;
    return o;
}

bool solveQuadratic(float a, float b, float c, thread float& t0, thread float& t1)
{
    float disc = b * b - 4. * a * c;
    
    if (disc < 0.)
    {
        return false;
    }
    
    if (disc == 0.)
    {
        t0 = t1 = -b / (2. * a);
        return true;
    }
    
    t0 = (-b + sqrt(disc)) / (2. * a);
    t1 = (-b - sqrt(disc)) / (2. * a);
    return true;
}

bool intersect(simd::float3 direction, thread const spherePix& sphere, thread simd::float3& surfaceNormal)
{
    simd::float3 L = normalize(sphere.cameraPosition - sphere.center);
    
    float a = dot(direction, direction);
    float b = 2. * dot(direction, L);
    float c = dot(L, L) - pow(sphere.radius, 2.);
    
    float t0;
    float t1;
    
    if (solveQuadratic(a, b, c, t0, t1))
    {
        float t = t0;
        if (t1 < t0)
        {
            t = t1;
        }
        
        simd::float3 Phit = sphere.cameraPosition + t * direction;
        surfaceNormal = normalize(Phit - sphere.center);
        
        return true;
    }
     
    return false;
}

half3 rayTrace(simd::float3 direction, thread const spherePix& sphere)
{
    simd::float3 surfaceNormal;
    
    if (intersect(direction, sphere, surfaceNormal))
    {
        simd::float3 light = {1.0,1.0,0.8};
        light = normalize(light);
        float coeff = -dot(light, surfaceNormal);
        
//        simd::float3 ambient = sphere.ambience * float3(sphere.color);
//        simd::float3 diffuse = sphere.diffuse * max(coeff, 0.) * float3(sphere.color);
//        
//        float shininess = pow(max(-dot(direction, reflect(light, surfaceNormal)), 0.), sphere.shininess);
//        simd::float3 specular = sphere.specular * shininess * float3(sphere.color);
//        
//        return ambient + diffuse + specular;
        return coeff * sphere.color;
    }
    
    return (half3){0., 0., 0.};
}

half4 fragment sphereFragmentMain( spherePix in [[stage_in]] )
{
    in.local_position.x -= 1.5;
    in.local_position.y += 0.5;
    simd::float3 direction = normalize(in.local_position - in.cameraPosition);
    half3 color = rayTrace(direction, in);
//    if (color.x == 0.0 && color.y == 0.0 && color.z == 0.0)
//    {
//        return (half4){1.0,1.0,1.0,1.0};
////        discard_fragment();
//    }
    return half4(half3(color), 1.0);
//    return half4(in.color, 1.0);
}



