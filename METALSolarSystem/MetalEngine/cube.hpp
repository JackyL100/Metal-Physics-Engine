//
//  cube.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/6/25.
//

#ifndef CUBE_H
#define CUBE_H

#include "shape.hpp"
#include "matrix_math.hpp"
#include <vector>

class Cube : public Shape
{
public:
    
    constexpr static float s = 0.5f;
    
    constexpr static shader_types::CubeVertexData vertices[]  =  {
        //   PoCube::sitionCube::s of Cube VerticeCube::s          NormalCube::s
                { { -Cube::s, -Cube::s, +Cube::s }, { 0.f,  0.f,  1.f } },
                { { +Cube::s, -Cube::s, +Cube::s }, { 0.f,  0.f,  1.f } },
                { { +Cube::s, +Cube::s, +Cube::s }, { 0.f,  0.f,  1.f } },
                { { -Cube::s, +Cube::s, +Cube::s }, { 0.f,  0.f,  1.f } },

                { { +Cube::s, -Cube::s, +Cube::s }, { 1.f,  0.f,  0.f } },
                { { +Cube::s, -Cube::s, -Cube::s }, { 1.f,  0.f,  0.f } },
                { { +Cube::s, +Cube::s, -Cube::s }, { 1.f,  0.f,  0.f } },
                { { +Cube::s, +Cube::s, +Cube::s }, { 1.f,  0.f,  0.f } },

                { { +Cube::s, -Cube::s, -Cube::s }, { 0.f,  0.f, -1.f } },
                { { -Cube::s, -Cube::s, -Cube::s }, { 0.f,  0.f, -1.f } },
                { { -Cube::s, +Cube::s, -Cube::s }, { 0.f,  0.f, -1.f } },
                { { +Cube::s, +Cube::s, -Cube::s }, { 0.f,  0.f, -1.f } },

                { { -Cube::s, -Cube::s, -Cube::s }, { -1.f, 0.f,  0.f } },
                { { -Cube::s, -Cube::s, +Cube::s }, { -1.f, 0.f,  0.f } },
                { { -Cube::s, +Cube::s, +Cube::s }, { -1.f, 0.f,  0.f } },
                { { -Cube::s, +Cube::s, -Cube::s }, { -1.f, 0.f,  0.f } },

                { { -Cube::s, +Cube::s, +Cube::s }, { 0.f,  1.f,  0.f } },
                { { +Cube::s, +Cube::s, +Cube::s }, { 0.f,  1.f,  0.f } },
                { { +Cube::s, +Cube::s, -Cube::s }, { 0.f,  1.f,  0.f } },
                { { -Cube::s, +Cube::s, -Cube::s }, { 0.f,  1.f,  0.f } },

                { { -Cube::s, -Cube::s, -Cube::s }, { 0.f, -1.f,  0.f } },
                { { +Cube::s, -Cube::s, -Cube::s }, { 0.f, -1.f,  0.f } },
                { { +Cube::s, -Cube::s, +Cube::s }, { 0.f, -1.f,  0.f } },
                { { -Cube::s, -Cube::s, +Cube::s }, { 0.f, -1.f,  0.f } },
    };
    
    constexpr static uint16_t indices[] = {
        0,  1,  2,  2,  3,  0, /* front */
        4,  5,  6,  6,  7,  4, /* right */
        8,  9, 10, 10, 11,  8, /* back */
       12, 13, 14, 14, 15, 12, /* left */
       16, 17, 18, 18, 19, 16, /* top */
       20, 21, 22, 22, 23, 20, /* bottom */
    };
    
    Cube(float size_, simd::float3 center_);
    virtual shader_types::InstanceData getWorldToScreenTransform(simd::float3 cameraPosition);
};

#endif
