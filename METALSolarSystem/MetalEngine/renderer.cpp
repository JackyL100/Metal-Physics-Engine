//
//  renderer.cpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/1/25.
//

#include "renderer.hpp"

const int Renderer::kMaxFramesInFlight = 3;

Renderer::Renderer( MTL::Device* pDevice )
: _pDevice( pDevice->retain() ),
_angle (0.f),
_cameraPosition{ 0.f, 0.f, -5.f },
_frame(0)
{
    _pCommandQueue = _pDevice->newCommandQueue();
    for (int i = 0; i < 2; i++)
    {
        cubes.push_back(std::make_unique<Cube>(0.2f, (simd::float3){1.0f * (i - 0.5f), 1.0f * i, 0}));
    }
    
    for (int i = 0; i < 3; i++)
    {
        spheres.push_back(std::make_unique<Sphere>(0.2f, (simd::float3){-0.3f * (i + 2.0f), 1.0f, -0.5f * (i - 1.0f) }));
    }
    buildShaders();
    buildDepthStencilShaders();
    buildBuffers();
    
    _semaphore = dispatch_semaphore_create(Renderer::kMaxFramesInFlight);
    
    
}

Renderer::~Renderer()
{
    _pShaderLibrary->release();
    _pDepthStencilState->release();
    _pCubeVertexDataBuffer->release();
    _pSphereVertexDataBuffer->release();
    for (int i = 0; i < kMaxFramesInFlight; i++)
    {
        _pCubeInstanceDataBuffer[i]->release();
    }
    for (int i = 0; i < kMaxFramesInFlight; i++)
    {
        _pSphereInstanceDataBuffer[i]->release();
    }
    for (int i = 0; i < kMaxFramesInFlight; i++)
    {
        _pCameraDataBuffer[i]->release();
    }
    _pCubeIndexBuffer->release();
    _pCubePSO->release();
    _pSpherePSO->release();
    _pCommandQueue->release();
    _pDevice->release();
}

MTL::RenderPipelineState* Renderer::createPSO(MTL::Function* vertexShader, MTL::Function* fragmentShader)
{
    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pDesc->setVertexFunction( vertexShader );
    pDesc->setFragmentFunction( fragmentShader );
    pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
    pDesc->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth16Unorm);
    NS::Error* pError = nullptr;
    MTL::RenderPipelineState* PSO = _pDevice->newRenderPipelineState( pDesc, &pError );
    if ( !PSO )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }
    pDesc->release();
    return PSO;
}


void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* pError = nullptr;
    MTL::Library* pLibrary = _pDevice->newDefaultLibrary();
    if ( !pLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }
    MTL::Function* pCubeVertexFn = pLibrary->newFunction( NS::String::string("cubeVertexMain", UTF8StringEncoding) );
    MTL::Function* pCubeFragFn = pLibrary->newFunction( NS::String::string("cubeFragmentMain", UTF8StringEncoding) );
    MTL::Function* pSphereVertexFn = pLibrary->newFunction( NS::String::string("sphereVertexMain", UTF8StringEncoding) );
    MTL::Function* pSphereFragFn = pLibrary->newFunction( NS::String::string("sphereFragmentMain", UTF8StringEncoding) );
    if ( !pCubeFragFn || !pCubeVertexFn || !pSphereFragFn || !pSphereVertexFn)
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    _pCubePSO = createPSO(pCubeVertexFn, pCubeFragFn);
    _pSpherePSO = createPSO(pSphereVertexFn, pSphereFragFn);
    
    pCubeVertexFn->release();
    pCubeFragFn->release();
    pSphereVertexFn->release();
    pSphereFragFn->release();
    _pShaderLibrary = pLibrary;
}

void Renderer::buildDepthStencilShaders()
{
    MTL::DepthStencilDescriptor* pDsDesc = MTL::DepthStencilDescriptor::alloc()->init();
    pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    pDsDesc->setDepthWriteEnabled(true);
    
    _pDepthStencilState = _pDevice->newDepthStencilState(pDsDesc);
    pDsDesc->release();
}

void Renderer::buildBuffers()
{
    using simd::float3;
    
    // setting up cube
    const size_t cubeVertexDataSize = sizeof(Cube::vertices);
    const size_t cubeIndexDataSize = sizeof(Cube::indices);
    
    MTL::Buffer* pCubeVertexBuffer = _pDevice->newBuffer(cubeVertexDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* pCubeIndexBuffer = _pDevice->newBuffer(cubeIndexDataSize, MTL::ResourceStorageModeManaged);
    
    _pCubeVertexDataBuffer = pCubeVertexBuffer;
    _pCubeIndexBuffer = pCubeIndexBuffer;
    
    memcpy(_pCubeVertexDataBuffer->contents(), Cube::vertices, cubeVertexDataSize);
    memcpy(_pCubeIndexBuffer->contents(), Cube::indices, cubeIndexDataSize);
    
    _pCubeVertexDataBuffer->didModifyRange(NS::Range::Make(0, _pCubeVertexDataBuffer->length()));
    _pCubeIndexBuffer->didModifyRange(NS::Range::Make(0, _pCubeIndexBuffer->length()));
    
    const size_t cubeInstanceDataSize = kMaxFramesInFlight * cubes.size() * sizeof(shader_types::InstanceData);
    
    for (size_t i = 0; i < kMaxFramesInFlight; i++)
    {
        _pCubeInstanceDataBuffer[i] = _pDevice->newBuffer(cubeInstanceDataSize, MTL::ResourceStorageModeManaged);
    }
    
    // setting up sphere
    const size_t sphereVertexSize = sizeof(Sphere::vertices);
    MTL::Buffer* pSphereVertexBuffer = _pDevice->newBuffer(sphereVertexSize, MTL::ResourceStorageModeManaged);
    _pSphereVertexDataBuffer = pSphereVertexBuffer;
    memcpy(_pSphereVertexDataBuffer->contents(), Sphere::vertices, sphereVertexSize);
    _pSphereVertexDataBuffer->didModifyRange(NS::Range::Make(0, _pSphereVertexDataBuffer->length()));
    
    const size_t sphereInstanceDataSize = kMaxFramesInFlight * spheres.size() * sizeof(shader_types::InstanceData);
    
    for (size_t i = 0; i < kMaxFramesInFlight; i++)
    {
        _pSphereInstanceDataBuffer[i] = _pDevice->newBuffer(sphereInstanceDataSize, MTL::ResourceStorageModeManaged);
    }
    
    const size_t cameraDataSize = kMaxFramesInFlight * sizeof(shader_types::CameraData);
    for (size_t i = 0; i < kMaxFramesInFlight; i++)
    {
        _pCameraDataBuffer[i] = _pDevice->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged);
    }
}

void Renderer::draw( MTK::View* pView )
{
    using simd::float3;
    using simd::float4;
    using simd::float4x4;

    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    _frame = (_frame + 1) % Renderer::kMaxFramesInFlight;
    MTL::Buffer* pCubeInstanceDataBuffer = _pCubeInstanceDataBuffer[ _frame ];
    MTL::Buffer* pSphereInstanceDataBuffer = _pSphereInstanceDataBuffer[ _frame ];

    MTL::CommandBuffer* pCmd = _pCommandQueue->commandBuffer();
    dispatch_semaphore_wait( _semaphore, DISPATCH_TIME_FOREVER );
    Renderer* pRenderer = this;
    pCmd->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd ){
        dispatch_semaphore_signal( pRenderer->_semaphore );
    });

//    _angle += 0.002f;

//    const float scl = 0.2f;
    shader_types::InstanceData* pCubeInstanceData = reinterpret_cast< shader_types::InstanceData *>( pCubeInstanceDataBuffer->contents() );
    shader_types::InstanceData* pSphereInstanceData = reinterpret_cast< shader_types::InstanceData* >(pSphereInstanceDataBuffer->contents());

//    float4x4 rt = math::makeTranslate( _cameraPosition );
//    float4x4 rr1 = math::makeYRotate( -_angle );
//    float4x4 rr0 = math::makeXRotate( _angle * 0.5);
//    float4x4 rtInv = math::makeTranslate( { -_cameraPosition.x, -_cameraPosition.y, -_cameraPosition.z } );
//    float4x4 fullObjectRot = rt * rr1 * rr0* rtInv;
//
//    size_t ix = 0;
//    size_t iy = 0;
//    size_t iz = 0;
    for ( size_t i = 0; i < cubes.size(); ++i )
    {
//        if ( ix == kInstanceRows )
//        {
//            ix = 0;
//            iy += 1;
//        }
//        if ( iy == kInstanceRows )
//        {
//            iy = 0;
//            iz += 1;
//        }
//
//        float4x4 scale = math::makeScale( (float3){ scl, scl, scl } );
//        float4x4 zrot = math::makeZRotate( _angle * sinf((float)ix) );
//        float4x4 yrot = math::makeYRotate( _angle * cosf((float)iy));
//
//        float x = ((float)ix - (float)kInstanceRows/2.f) * (2.f * scl) + scl;
//        float y = ((float)iy - (float)kInstanceColumns/2.f) * (2.f * scl) + scl;
//        float z = ((float)iz - (float)kInstanceDepth/2.f) * (2.f * scl);
//        float4x4 translate = math::makeTranslate( math::add( _cameraPosition, { x, y, z } ) );
//
////     pInstanceData[ i ].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;
//        pInstanceData[ i ].instanceTransform = translate * yrot * zrot * scale;
//        pInstanceData[ i ].instanceNormalTransform = math::discardTranslation( pInstanceData[ i ].instanceTransform );
//
//        float iDivNumInstances = i / (float)kNumInstances;
//        float r = iDivNumInstances;
//        float g = 1.0f - r;
//        float b = sinf( M_PI * 2.0f * iDivNumInstances );
//        pInstanceData[ i ].instanceColor = (float4){ r, g, b, 1.0f };
//
//        ix += 1;
        pCubeInstanceData[i] = cubes[i]->getWorldToScreenTransform(_cameraPosition);
        
    }
    for (int i = 0; i < spheres.size(); i++)
    {
        pSphereInstanceData[i] = spheres[i]->getWorldToScreenTransform(_cameraPosition);
    }
    pCubeInstanceDataBuffer->didModifyRange( NS::Range::Make( 0, pCubeInstanceDataBuffer->length() ) );
    pSphereInstanceDataBuffer->didModifyRange(NS::Range::Make(0, pSphereInstanceDataBuffer->length()));
    // Update camera state:

    MTL::Buffer* pCameraDataBuffer = _pCameraDataBuffer[ _frame ];
    shader_types::CameraData* pCameraData = reinterpret_cast< shader_types::CameraData *>( pCameraDataBuffer->contents() );
    pCameraData->perspectiveTransform = math::makePerspective( 45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f ) ;
    pCameraData->worldTransform = math::makeIdentity();
    pCameraData->worldNormalTransform = math::discardTranslation( pCameraData->worldTransform );
    pCameraDataBuffer->didModifyRange( NS::Range::Make( 0, sizeof( shader_types::CameraData ) ) );

    // Begin render pass:

    MTL::RenderPassDescriptor* pRpd = pView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder( pRpd );

    // Draw Cubes
    pEnc->setRenderPipelineState( _pCubePSO );
    pEnc->setDepthStencilState( _pDepthStencilState );

    pEnc->setVertexBuffer( _pCubeVertexDataBuffer, /* offset */ 0, /* index */ 0 );
    pEnc->setVertexBuffer( pCubeInstanceDataBuffer, /* offset */ 0, /* index */ 1 );
    pEnc->setVertexBuffer( pCameraDataBuffer, /* offset */ 0, /* index */ 2 );

    pEnc->setCullMode( MTL::CullModeBack );
    pEnc->setFrontFacingWinding( MTL::Winding::WindingCounterClockwise );

    pEnc->drawIndexedPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle,
                                6 * 6, MTL::IndexType::IndexTypeUInt16,
                                _pCubeIndexBuffer,
                                0,
                                cubes.size());
    
    // Draw Spheres
    pEnc->setRenderPipelineState( _pSpherePSO );
    pEnc->setDepthStencilState( _pDepthStencilState );

    pEnc->setVertexBuffer( _pSphereVertexDataBuffer, /* offset */ 0, /* index */ 0 );
    pEnc->setVertexBuffer( pSphereInstanceDataBuffer, /* offset */ 0, /* index */ 1 );
    pEnc->setVertexBuffer( pCameraDataBuffer, /* offset */ 0, /* index */ 2 );

    pEnc->setCullMode( MTL::CullModeBack );
    pEnc->setFrontFacingWinding( MTL::Winding::WindingCounterClockwise );

    pEnc->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle,NS::UInteger(0), NS::UInteger(3), spheres.size());

    pEnc->endEncoding();
    pCmd->presentDrawable( pView->currentDrawable() );
    pCmd->commit();

    pPool->release();
}
