//
//  renderer.hpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/1/25.
//
#pragma once
#include "matrix_math.hpp"
#include "shader_types.hpp"
#include "cube.hpp"
#include "sphere.hpp"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <simd/simd.h>
#include <vector>
#include <memory>
#include <iostream>


//static constexpr size_t kInstanceRows = 5;
//static constexpr size_t kInstanceColumns = 5;
//static constexpr size_t kInstanceDepth = 5;
//static constexpr size_t kNumInstances = (kInstanceRows * kInstanceColumns * kInstanceDepth);
static constexpr size_t kMaxFramesInFlight = 3;

class Renderer
{
    public:
        Renderer( MTL::Device* pDevice );
        ~Renderer();
        void buildShaders();
        void buildDepthStencilShaders();
        void buildBuffers();
        void draw( MTK::View* pView );

    private:
        MTL::Device* _pDevice;
        MTL::CommandQueue* _pCommandQueue;
        MTL::Library* _pShaderLibrary;
        MTL::RenderPipelineState* _pCubePSO;
        MTL::RenderPipelineState* _pSpherePSO;
        MTL::DepthStencilState* _pDepthStencilState;
        // Cube data
        MTL::Buffer* _pCubeVertexDataBuffer;
        MTL::Buffer* _pCubeInstanceDataBuffer[kMaxFramesInFlight];
        MTL::Buffer* _pCubeIndexBuffer;
        // Sphere data
        MTL::Buffer* _pSphereVertexDataBuffer;
        MTL::Buffer* _pSphereInstanceDataBuffer[kMaxFramesInFlight];
        MTL::Buffer* _pCameraDataBuffer[kMaxFramesInFlight];
        float _angle;
        simd::float3 _cameraPosition;
        int _frame;
        dispatch_semaphore_t _semaphore;
        static const int kMaxFramesInFlight;
        std::vector<std::unique_ptr<Cube>> cubes;
        std::vector<std::unique_ptr<Sphere>> spheres;
    
        MTL::RenderPipelineState* createPSO(MTL::Function* vertexShader, MTL::Function* fragmentShader);
};
