source_filename = "llvm-link"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc"

%struct.clMegaKernelParams.544 = type { %struct.CLRenderData.466*, float*, float*, %struct.ParamsStaticVariables.467*, %struct.clMap.473*, i32*, float*, i32*, i32*, i32*, i32*, %struct.clSceneNode.475*, %struct.clSceneMesh.476*, float*, i32*, i32*, float*, i32*, %struct.MbGeomData.477, i32*, i32*, i32*, float*, i32*, i32*, i32*, float*, i32*, float*, i32*, %struct.clEmbBVHNode.478*, i32*, %struct.clEmbBVHNodeMB.479*, i32*, %struct.clMaterialDescriptor.486*, %struct.OCLLight.488*, float*, i32*, float*, i32*, %struct.clBitmapInfo.490*, %struct.clBitmapInfo.490*, %struct.clUserAttrToBitmap.491*, %struct.clMap.473*, i32*, i32*, i32*, i32*, float*, i32*, i32*, i32*, float*, i8*, i32*, i64*, i64*, i64*, float*, i32*, float*, i32*, float*, %struct.GPURefNode.492*, i32*, float*, %struct.GPUEnvironmentDescriptor.494*, %struct.clEnvMap.493*, %struct.clLightCache.497*, %struct.clMap.473*, %struct.clMap.473*, %struct.GPUUVWRandomizerParams.498*, i32, i32*, %struct.clUserAttrRef.474*, i32, i64*, i32*, i32, %struct.clMap.473*, i32, i32, %struct.clDynamicMesh.499*, %struct.clDynamicNode.500*, %struct.clUserAttrRef.474*, %struct.clBVHNode.502*, i32*, i32, i32, i64, i32*, i32*, %struct.clMap.473*, %struct.clMap.473*, %struct.clMap.473*, %struct.clMap.473*, i32*, float*, %struct.clMap.473*, i32*, %struct.clMap.473*, i32*, %struct.CLMiscParams.518*, %struct.clTexBakeParams.519*, %struct.clDataForAllBakeNodes.520*, i32*, i32, i32*, i32, float*, i32, i32*, i32*, %struct.PerMeshData.521*, %struct.PerMeshData.521*, %struct.PerMeshData.521*, %struct.PerMeshData.521*, %struct.PerMeshData.521*, %struct.PerDynamicMeshData.522**, i32*, i32*, i32*, i32*, %struct.clLightLinkParams.525*, i32, i32*, %struct.RTAerialPerspectiveParams.526*, %struct.RTEnvFogParams.527*, %struct.clLightGrid.528*, %struct.clLightGrid.528*, %struct.clLightGrid.528*, i32, i32*, %struct.clLightGrid.528*, %struct.clLightGrid.528*, i32*, i32*, %struct.clLightTree.531*, i32, %struct.GPUEnmeshParams.535*, %struct.GPUDisplacement2dTexture.537*, %struct.clMap.473*, %struct.GPUDisplacement2dTexturePtrToIndex.538*, i32, %struct.GPUDisplacement2dFaceData.534*, float*, %struct.ManagedMemoryWrapper.540*, i32, %struct.clAttributesData.542, i32, [4 x %struct.GPUCameraEnclosingVolume.543] }
%struct.CLRenderData.466 = type { %struct.GPUCamera.463, <4 x float>, float, i32, i32, i32, i32, float, float, float, float, i32, i32, i32, i32, i32, float, float, i32, i32, i32, %struct.CLStereoParams.464, %"class.Imath_2_2::Vec3.450", i32, i32, i32, i32, float, i32, i32, i32, i32, i32, i32, [2 x i32], %struct.CLUserAttributePresets.465 }
%struct.GPUCamera.463 = type { i8*, i8*, [12 x float], [12 x float], float, %union.anon.461, float, float, float, float, float, float, float, i32, float, float, float, i32, i32, i32, float, i32, float, float, float, float, float, float, [3 x float], float, float, float, float, i32, i32, %struct.OCLBmpRegionSampler.462*, i32, i32, float, float, float, i32, float, float, float, float, float, float, float, float, float, i32, i32, i32, [1 x i32] }
%union.anon.461 = type { %struct.GPUCamera_SphericalPanoramaSets.460 }
%struct.GPUCamera_SphericalPanoramaSets.460 = type { i32, float }
%struct.OCLBmpRegionSampler.462 = type { i32*, float*, float, i32, i32, i32, i32, float, i32, i32 }
%struct.CLStereoParams.464 = type { [4 x [3 x float]], [4 x [3 x float]], float, float, float, float, float, float, i32, i32, i32 }
%"class.Imath_2_2::Vec3.450" = type { float, float, float }
%struct.CLUserAttributePresets.465 = type { i32, i32, i32, i32, i32, i32, [2 x i32] }
%struct.ParamsStaticVariables.467 = type { [4 x float], [4 x float], [4 x float], [4 x float], [4 x float], [4 x float], i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, float, float, i32, i32, float, i32, i32, float, float, [3 x float] }
%struct.clSceneNode.475 = type { i32, i32, i32, i32, i32, i32, i32, i32, [1 x i32], i32, [9 x float], [9 x float], [12 x float], [12 x float], [12 x float], [12 x float], %struct.clUserAttrRef.474, float, i32, float, float, i32, i32, i32, i32, i64 }
%struct.clUserAttrRef.474 = type { i32, i32 }
%struct.clSceneMesh.476 = type { i32, i32, i32, i32, i32, i32, [16 x i32], [16 x i32], [16 x i32], [16 x i32], [16 x i32], i32, i32, i32, %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", i32 }
%struct.MbGeomData.477 = type { i32*, i32*, i32*, %struct.clSceneNode.475*, %struct.clSceneMesh.476*, i32*, i32*, float*, i32*, float* }
%struct.clEmbBVHNode.478 = type { [6 x <4 x float>], [4 x i32] }
%struct.clEmbBVHNodeMB.479 = type { [12 x <4 x float>], [4 x i32] }
%struct.clMaterialDescriptor.486 = type { [47 x i32], i64, i32, %union.anon.6.482, i32, i32, i32, float, i32, i32, i32, i32, i32, [4 x float], i32, i32, i32, i32, float, [3 x float], float, float, i32, i32, %union.anon.7.484, i32, i32, [129 x [3 x float]], float, float, [32 x %struct.AllBitmapsAuxiliary.485], %union.anon.0.468, i32, i64, i32, float, float, i32, i32, [2 x i32] }
%union.anon.6.482 = type { %struct.AlSurfaceSSSParams.481 }
%struct.AlSurfaceSSSParams.481 = type { i32, i32, float, [2 x %"struct.VRayOSL::v1_10::pvt::(anonymous namespace)::HashScalarPeriodic.453"], [3 x %struct.AlSurfaceSSSLayerParams.480] }
%"struct.VRayOSL::v1_10::pvt::(anonymous namespace)::HashScalarPeriodic.453" = type { i32, i32, i32, i32 }
%struct.AlSurfaceSSSLayerParams.480 = type { i32, i32, i32 }
%union.anon.7.484 = type { %struct.anon.8.483 }
%struct.anon.8.483 = type { float, float }
%struct.AllBitmapsAuxiliary.485 = type { %struct.clMap.473*, %struct.clMap.473* }
%union.anon.0.468 = type { i8* }
%struct.OCLLight.488 = type { i8*, i32, i32, %struct.OCLLightAttenuation.487, [3 x float], i32, float, float, float, float, float, float, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, %union.anon.5.472, i32, [24 x i32], i32, float }
%struct.OCLLightAttenuation.487 = type { i32, float, i32, float, float, i32, float, float }
%union.anon.5.472 = type { i32 }
%struct.clBitmapInfo.490 = type { %union.anon.0.468, %struct.anon.12.489, i32, i32, i32, float, float, i16, i16, i16, i16, i32, i16, i16, i16, i16, i16, [3 x i16] }
%struct.anon.12.489 = type { i32, i32, [8 x i32] }
%struct.clUserAttrToBitmap.491 = type { i32, [8 x i32] }
%struct.GPURefNode.492 = type { i32, [12 x float], [12 x float], i8* }
%struct.GPUEnvironmentDescriptor.494 = type { [5 x %struct.clEnvMap.493], i32, i32, i32 }
%struct.clEnvMap.493 = type { i32, float, float, float, [4 x float], [9 x float], [12 x float], i32, [3 x float], float }
%struct.clLightCache.497 = type { %struct.GPULightCacheSample.495*, %struct.GPULightCacheSampleKey.496*, i32*, float*, i32*, i32, i32, i32, i32, i32, i32, i32 }
%struct.GPULightCacheSample.495 = type { %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", i32, i32 }
%struct.GPULightCacheSampleKey.496 = type { [3 x i32], [2 x i32], i32 }
%struct.GPUUVWRandomizerParams.498 = type { i32, %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", float, i32, i32, i32 }
%struct.clDynamicMesh.499 = type { i64, i32, i32, i32, [16 x i32], [16 x i32], i32, i32, i32, i32, i32, [16 x i32], [16 x i32], [16 x i32], i32, i32, i32, %"struct.VRayOSL::v1_10::pvt::(anonymous namespace)::HashScalarPeriodic.453", [2 x i32] }
%struct.clDynamicNode.500 = type { %union.anon.0.468, i32, i32, i32, i32, i32, i32, i64, i32, i32, [9 x float], [9 x float], float, float, [12 x float], [12 x float], [12 x float], [12 x float], [3 x float], [3 x float], i32, i32, %struct.clUserAttrRef.474, [2 x i32], float, i32, i32, i32, i32, i32, i8* }
%struct.clBVHNode.502 = type { i32, %union.anon.15.501, [3 x float], [3 x float] }
%union.anon.15.501 = type { float }
%struct.CLMiscParams.518 = type { %struct.CLMaxHairNormalizeTable.503*, %struct.CLMaxHairNormalizeTable.503*, %struct.CLMaxHairNormalizeTable.503*, i32*, %struct.IrradSample_gpu.506*, i8*, i8*, i8*, i8*, %struct.CLLPEAutomaton.508*, %struct.GPULinearInterpolationTable.509*, i32*, %struct.GPUDecalData.516, %struct.GPUSkyCloudsData.517, i32, i32, i32, float, i32, i32, i32, i32, i32 }
%struct.CLMaxHairNormalizeTable.503 = type { i32, i32, i32, i32, float*, float* }
%struct.IrradSample_gpu.506 = type { [3 x float], [3 x float], float, %struct.CompressedColor_gpu.504, %struct.CompressedColor_gpu.504, %struct.CompressedUnitVector_gpu.505, %union.anon.15.501, i32, [3 x %struct.CompressedColor_gpu.504], [3 x %struct.CompressedColor_gpu.504], i32, float, float }
%struct.CompressedColor_gpu.504 = type { i8, i8, i8, i8 }
%struct.CompressedUnitVector_gpu.505 = type { i16, i16 }
%struct.CLLPEAutomaton.508 = type { i16*, %struct.CLLPETransition.507*, i8, i8 }
%struct.CLLPETransition.507 = type { i64, i8 }
%struct.GPULinearInterpolationTable.509 = type { float, float, i32, float* }
%struct.GPUDecalData.516 = type { i32*, %struct.GPUDecalNodeParams.510*, %struct.GPUStaticDecalNode.511*, %struct.GPUStaticDecalBVHNode.512*, %struct.GPUStaticDecalNode.511*, %struct.GPUMovingDecalBVHNode.513*, %struct.GPUMovingDecalTimeSliceNode.514*, %struct.GPUMovingDecalIntervalTreeNode.515*, i32*, i32, i32, i32, i32, i64, i32, [3 x i32] }
%struct.GPUDecalNodeParams.510 = type { i32, i32, i32, i32, [3 x float], [3 x float], i32, i32, float, float, float, i32, i32, %struct.clUserAttrRef.474, float, float, [3 x i32] }
%struct.GPUStaticDecalBVHNode.512 = type { i32, [3 x float], [3 x float], i32 }
%struct.GPUStaticDecalNode.511 = type { i32, [12 x float], [12 x float], [3 x i32] }
%struct.GPUMovingDecalBVHNode.513 = type { i32, [3 x float], [3 x float], [3 x float], [3 x float], [3 x i32] }
%struct.GPUMovingDecalTimeSliceNode.514 = type { i32, float, float, float }
%struct.GPUMovingDecalIntervalTreeNode.515 = type { i32, float, i32, i32 }
%struct.GPUSkyCloudsData.517 = type { float*, float*, float*, float*, float*, float*, float*, i32, i32, [4 x float], float, float, float, float, float, float, float, float, float, float, i32, i32, i32, float, float, float, float, float, float, [3 x float], [9 x float], float, float, float }
%struct.clTexBakeParams.519 = type { i32, i32, i32, float, float, float, float, float, [12 x float], [12 x float] }
%struct.clDataForAllBakeNodes.520 = type { i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, float }
%struct.PerMeshData.521 = type { i32*, float*, i32*, i32*, i32* }
%struct.PerDynamicMeshData.522 = type { i64, i64, i64 }
%struct.clLightLinkParams.525 = type { %struct.clLightLinksList.523*, i8*, [3 x %union.anon.17.524], i32, i32, [3 x i32], i32 }
%struct.clLightLinksList.523 = type { i32*, float, float, i32, i32, [2 x i32] }
%union.anon.17.524 = type { i16* }
%struct.RTAerialPerspectiveParams.526 = type { i32, float, float, float, float, [3 x float], i32, i32, i32, [3 x float], [3 x float], [3 x float], [3 x float], [3 x float], float, [1452 x float], [1452 x float] }
%struct.RTEnvFogParams.527 = type { i32*, i32, [3 x float], [3 x float], [3 x float], [3 x float], float, float, float, float, float, [3 x float], float, i32, i32, i32, i32 }
%struct.clLightGrid.528 = type { float*, i32*, i32, i32, i32, i32, i32, i32, [12 x float], i32, i32, i32, [3 x i32] }
%struct.clLightTree.531 = type { %struct.clLightTreeNode.529*, %struct.clLightTreeLightInfo.530*, i32, i32, float, float }
%struct.clLightTreeNode.529 = type { i32, i32, i32, i32, %"class.Imath_2_2::Vec3.450", float }
%struct.clLightTreeLightInfo.530 = type { <4 x float>, i32, i32, %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450" }
%struct.GPUEnmeshParams.535 = type { i32*, i32*, float*, float*, i32*, float*, i32*, i32*, float*, float*, i32, float, float, float, float, float, float, float, float, i32, float, float, [9 x float], float, [3 x float], [3 x float], [3 x float], [3 x float], %struct.GPUDisplacement2dFaceData.534, %struct.GPUDisplacement2dFaceData.534, [12 x float], [12 x float], [3 x float], [3 x float] }
%struct.GPUDisplacement2dFaceData.534 = type { i32*, [16 x %union.anon.18.532], [16 x %union.anon.19.533], i32, i32, [16 x i32] }
%union.anon.18.532 = type { i32* }
%union.anon.19.533 = type { float* }
%struct.GPUDisplacement2dTexture.537 = type { %struct.GPUDisplacementTextureTexNode.536*, float*, i32, i32, i32, i32, i32, float, float, [1 x i32] }
%struct.GPUDisplacementTextureTexNode.536 = type { float, float, i16, i16, i32, i32 }
%struct.GPUDisplacement2dTexturePtrToIndex.538 = type { i64, i32, i32 }
%struct.ManagedMemoryWrapper.540 = type { %struct.ManagedMemBlock.539*, %struct.ManagedMemBlock.539*, i32 }
%struct.ManagedMemBlock.539 = type { i8*, i8, i8 }
%struct.clAttributesData.542 = type { %struct.clUserAttr.541*, %union.anon.5.472*, <4 x float>* }
%struct.clUserAttr.541 = type { [4 x float], i32, i32, i32 }
%struct.GPUCameraEnclosingVolume.543 = type { i32, [3 x float], [3 x float] }
%struct.clMap.473 = type { %union.anon.0.468, %union.anon.0.468, %union.anon.0.468, %union.anon.3.469, [8 x float], [4 x float], %union.anon.4.471, i32, i32, float, i32, i32, i32, float, i32, float, float, i32, i32, i64, i32, i32, %union.anon.5.472, i32, float, float, i32, [1 x i32] }
%union.anon.3.469 = type { [12 x float] }
%union.anon.4.471 = type { %struct.anon.470 }
%struct.anon.470 = type { float, float, float, float }
%struct.clRayState.548 = type { %struct.clTraceData.547, %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %union.anon.20.545, %union.anon.20.545, %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", i64, float, float, float, float, float, float, i32, i32, i32, [4 x i32], [4 x i8], i32, float, i8, i8, i8, i8 }
%struct.clTraceData.547 = type { %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %union.anon.20.545, %union.anon.20.545, %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450", %union.anon.7.484, %union.anon.0.468, i64, float, float, float, float, float, float, %union.anon.15.501, %union.anon.15.501, i32, i32, float, %union.anon.5.472, i32, i32, i32, i32, i32, i32, i32, i32, i8, [4 x i8], i16, i16, i16, i16, i16, i8, %union.anon.27.546 }
%union.anon.27.546 = type { i8 }
%union.anon.20.545 = type { %"class.Imath_2_2::Vec3.450" }

; Function Attrs: alwaysinline nofree nosync nounwind readnone speculatable willreturn
declare float @llvm.floor.f32(float) #0

; Function Attrs: alwaysinline nofree nounwind willreturn
define hidden <4 x float> @evalGLSL(%struct.clMegaKernelParams.544* noalias nocapture readnone %0, %struct.clMap.473* noalias nocapture readonly %1, %struct.clMap.473* noalias nocapture readnone %2, %struct.anon.470* nocapture readonly %3, %"class.Imath_2_2::Vec3.450"* nocapture readonly %4, i32* nocapture %5, %struct.clRayState.548* nocapture readnone %6, %"class.Imath_2_2::Vec3.450"* nocapture readnone %7) local_unnamed_addr #1 {
  %9 = getelementptr %struct.clMap.473, %struct.clMap.473* %1, i64 0, i32 23
  %10 = load i32, i32* %9, align 4, !tbaa !7
  switch i32 %10, label %evalGLSL_prev3510698853.exit [
    i32 -784268443, label %11
    i32 -1056780865, label %56
    i32 2061355947, label %101
    i32 695269647, label %128
    i32 1196644373, label %155
  ]

11:                                               ; preds = %8
  %12 = load i32, i32* %5, align 4, !tbaa !14
  %13 = add nsw i32 %12, -1
  store i32 %13, i32* %5, align 4, !tbaa !14
  %.idx149.val.idx.i.i1 = bitcast %"class.Imath_2_2::Vec3.450"* %4 to float*
  %.idx149.val.idx.val.i.i = load float, float* %.idx149.val.idx.i.i1, align 4, !tbaa.struct !15
  %.idx153.val.idx.i.i = getelementptr %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450"* %4, i64 0, i32 1
  %.idx153.val.idx.val.i.i = load float, float* %.idx153.val.idx.i.i, align 4, !tbaa.struct !17
  %14 = tail call float @llvm.floor.f32(float %.idx149.val.idx.val.i.i) #2
  %15 = fsub float %.idx149.val.idx.val.i.i, %14
  %16 = tail call float @llvm.floor.f32(float %.idx153.val.idx.val.i.i) #2
  %17 = fsub float %.idx153.val.idx.val.i.i, %16
  %18 = fcmp ogt float %15, 1.000000e+00
  %19 = fcmp olt float %15, 0.000000e+00
  %20 = or i1 %18, %19
  %21 = fcmp ogt float %17, 1.000000e+00
  %22 = or i1 %20, %21
  %23 = fcmp olt float %17, 0.000000e+00
  %24 = or i1 %23, %22
  %.pre.i = add i32 %12, -2
  %25 = sext i32 %.pre.i to i64
  %26 = getelementptr inbounds %struct.anon.470, %struct.anon.470* %3, i64 %25, i32 0
  %27 = getelementptr inbounds %struct.anon.470, %struct.anon.470* %3, i64 %25, i32 1
  %28 = getelementptr inbounds %struct.anon.470, %struct.anon.470* %3, i64 %25, i32 2
  br i1 %24, label %evalGLSL_current3510698853.exit, label %29

29:                                               ; preds = %11
  %30 = load float, float* %26, align 4, !tbaa !18
  %31 = load float, float* %27, align 4, !tbaa !20
  %32 = load float, float* %28, align 4, !tbaa !21
  br label %evalGLSL_current3510698853.exit

evalGLSL_current3510698853.exit:                  ; preds = %29, %11
  %.sink2.i.i.i.i = phi float [ %30, %29 ], [ 0.000000e+00, %11 ]
  %.sink1.i.i.i.i = phi float [ %31, %29 ], [ 0.000000e+00, %11 ]
  %.sink.i.i.i.i = phi float [ %32, %29 ], [ 0.000000e+00, %11 ]
  %33 = fmul float %.sink2.i.i.i.i, 5.000000e-01
  %34 = tail call float @llvm.floor.f32(float %33) #2
  %35 = fmul float %34, 2.000000e+00
  %36 = fsub float %.sink2.i.i.i.i, %35
  %37 = fptosi float %36 to i32
  %38 = fmul float %.sink1.i.i.i.i, 5.000000e-01
  %39 = tail call float @llvm.floor.f32(float %38) #2
  %40 = fmul float %39, 2.000000e+00
  %41 = fsub float %.sink1.i.i.i.i, %40
  %42 = fptosi float %41 to i32
  %43 = fmul float %.sink.i.i.i.i, 5.000000e-01
  %44 = tail call float @llvm.floor.f32(float %43) #2
  %45 = fmul float %44, 2.000000e+00
  %46 = fsub float %.sink.i.i.i.i, %45
  %47 = fptosi float %46 to i32
  %48 = srem i32 %37, 2
  %49 = srem i32 %42, 2
  %50 = xor i32 %49, %48
  %51 = srem i32 %47, 2
  %52 = icmp eq i32 %50, %51
  %storemerge1.i.i.i = select i1 %52, float 1.000000e+00, float 0x3FC99999A0000000
  %storemerge.i.i.i = select i1 %52, float 0x3FC99999A0000000, float 1.000000e+00
  %53 = insertelement <4 x float> <float poison, float poison, float poison, float 1.000000e+00>, float %storemerge1.i.i.i, i32 0
  %54 = insertelement <4 x float> %53, float %storemerge1.i.i.i, i32 1
  %55 = insertelement <4 x float> %54, float %storemerge.i.i.i, i32 2
  store i32 %.pre.i, i32* %5, align 4, !tbaa !14
  br label %evalGLSL_prev3510698853.exit

56:                                               ; preds = %8
  %57 = load i32, i32* %5, align 4, !tbaa !14
  %58 = add nsw i32 %57, -1
  store i32 %58, i32* %5, align 4, !tbaa !14
  %.idx149.val.idx.i.i.i2 = bitcast %"class.Imath_2_2::Vec3.450"* %4 to float*
  %.idx149.val.idx.val.i.i.i = load float, float* %.idx149.val.idx.i.i.i2, align 4, !tbaa.struct !15
  %.idx153.val.idx.i.i.i = getelementptr %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450"* %4, i64 0, i32 1
  %.idx153.val.idx.val.i.i.i = load float, float* %.idx153.val.idx.i.i.i, align 4, !tbaa.struct !17
  %59 = tail call float @llvm.floor.f32(float %.idx149.val.idx.val.i.i.i) #2
  %60 = fsub float %.idx149.val.idx.val.i.i.i, %59
  %61 = tail call float @llvm.floor.f32(float %.idx153.val.idx.val.i.i.i) #2
  %62 = fsub float %.idx153.val.idx.val.i.i.i, %61
  %63 = fcmp ogt float %60, 1.000000e+00
  %64 = fcmp olt float %60, 0.000000e+00
  %65 = or i1 %63, %64
  %66 = fcmp ogt float %62, 1.000000e+00
  %67 = or i1 %65, %66
  %68 = fcmp olt float %62, 0.000000e+00
  %69 = or i1 %68, %67
  %.pre.i.i = add i32 %57, -2
  %70 = sext i32 %.pre.i.i to i64
  %71 = getelementptr inbounds %struct.anon.470, %struct.anon.470* %3, i64 %70, i32 2
  %72 = getelementptr inbounds %struct.anon.470, %struct.anon.470* %3, i64 %70, i32 1
  %73 = getelementptr inbounds %struct.anon.470, %struct.anon.470* %3, i64 %70, i32 0
  br i1 %69, label %evalGLSL_current3238186431.exit.i, label %74

74:                                               ; preds = %56
  %75 = load float, float* %73, align 4, !tbaa !18
  %76 = load float, float* %72, align 4, !tbaa !20
  %77 = load float, float* %71, align 4, !tbaa !21
  br label %evalGLSL_current3238186431.exit.i

evalGLSL_current3238186431.exit.i:                ; preds = %74, %56
  %.sink2.i.i.i.i.i = phi float [ %75, %74 ], [ 0.000000e+00, %56 ]
  %.sink1.i.i.i.i.i = phi float [ %76, %74 ], [ 0.000000e+00, %56 ]
  %.sink.i.i.i.i.i = phi float [ %77, %74 ], [ 0.000000e+00, %56 ]
  %78 = fmul float %.sink2.i.i.i.i.i, 5.000000e-01
  %79 = tail call float @llvm.floor.f32(float %78) #2
  %80 = fmul float %79, 2.000000e+00
  %81 = fsub float %.sink2.i.i.i.i.i, %80
  %82 = fptosi float %81 to i32
  %83 = fmul float %.sink1.i.i.i.i.i, 5.000000e-01
  %84 = tail call float @llvm.floor.f32(float %83) #2
  %85 = fmul float %84, 2.000000e+00
  %86 = fsub float %.sink1.i.i.i.i.i, %85
  %87 = fptosi float %86 to i32
  %88 = fmul float %.sink.i.i.i.i.i, 5.000000e-01
  %89 = tail call float @llvm.floor.f32(float %88) #2
  %90 = fmul float %89, 2.000000e+00
  %91 = fsub float %.sink.i.i.i.i.i, %90
  %92 = fptosi float %91 to i32
  %93 = srem i32 %82, 2
  %94 = srem i32 %87, 2
  %95 = xor i32 %94, %93
  %96 = srem i32 %92, 2
  %97 = icmp eq i32 %95, %96
  %storemerge.i.i.i.i = select i1 %97, float 0.000000e+00, float 1.000000e+00
  %98 = insertelement <4 x float> <float poison, float poison, float poison, float 1.000000e+00>, float %storemerge.i.i.i.i, i32 0
  %99 = insertelement <4 x float> %98, float %storemerge.i.i.i.i, i32 1
  %100 = insertelement <4 x float> %99, float %storemerge.i.i.i.i, i32 2
  store i32 %.pre.i.i, i32* %5, align 4, !tbaa !14
  br label %evalGLSL_prev3510698853.exit

101:                                              ; preds = %8
  %102 = load i32, i32* %5, align 4, !tbaa !14
  %103 = add nsw i32 %102, -1
  store i32 %103, i32* %5, align 4, !tbaa !14
  %.idx149.val.idx.i.i.i.i3 = bitcast %"class.Imath_2_2::Vec3.450"* %4 to float*
  %.idx149.val.idx.val.i.i.i.i = load float, float* %.idx149.val.idx.i.i.i.i3, align 4, !tbaa.struct !15
  %.idx153.val.idx.i.i.i.i = getelementptr %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450"* %4, i64 0, i32 1
  %.idx153.val.idx.val.i.i.i.i = load float, float* %.idx153.val.idx.i.i.i.i, align 4, !tbaa.struct !17
  %104 = fmul fast float %.idx149.val.idx.val.i.i.i.i, 0x3FEFFFFA80000000
  %105 = fadd fast float %104, 0xC00D999480000000
  %106 = fmul fast float %.idx153.val.idx.val.i.i.i.i, 0x3FEFFFFA80000000
  %107 = fadd fast float %106, 0xC00F332DE0000000
  %108 = fadd float %105, 0x400D9999A0000000
  %109 = fadd float %107, 0x400F333340000000
  %110 = fdiv float %108, 0x3FA99999A0000000
  %111 = fdiv float %109, 0x3FA99999A0000000
  %112 = fmul float %110, 5.000000e-01
  %113 = tail call float @llvm.floor.f32(float %112) #2
  %114 = fmul float %113, 2.000000e+00
  %115 = fsub float %110, %114
  %116 = fptosi float %115 to i32
  %117 = fmul float %111, 5.000000e-01
  %118 = tail call float @llvm.floor.f32(float %117) #2
  %119 = fmul float %118, 2.000000e+00
  %120 = fsub float %111, %119
  %121 = fptosi float %120 to i32
  %122 = srem i32 %116, 2
  %123 = srem i32 %121, 2
  %124 = icmp eq i32 %122, %123
  %storemerge1.i.i.i.i.i = select i1 %124, float 1.000000e+00, float 0x3FC99999A0000000
  %storemerge.i.i.i.i.i = select i1 %124, float 0x3FC99999A0000000, float 1.000000e+00
  %125 = insertelement <4 x float> <float poison, float poison, float poison, float 1.000000e+00>, float %storemerge1.i.i.i.i.i, i32 0
  %126 = insertelement <4 x float> %125, float %storemerge1.i.i.i.i.i, i32 1
  %127 = insertelement <4 x float> %126, float %storemerge.i.i.i.i.i, i32 2
  br label %evalGLSL_prev3510698853.exit

128:                                              ; preds = %8
  %129 = load i32, i32* %5, align 4, !tbaa !14
  %130 = add nsw i32 %129, -1
  store i32 %130, i32* %5, align 4, !tbaa !14
  %.idx149.val.idx.i.i.i.i.i4 = bitcast %"class.Imath_2_2::Vec3.450"* %4 to float*
  %.idx149.val.idx.val.i.i.i.i.i = load float, float* %.idx149.val.idx.i.i.i.i.i4, align 4, !tbaa.struct !15
  %.idx153.val.idx.i.i.i.i.i = getelementptr %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450"* %4, i64 0, i32 1
  %.idx153.val.idx.val.i.i.i.i.i = load float, float* %.idx153.val.idx.i.i.i.i.i, align 4, !tbaa.struct !17
  %131 = fmul fast float %.idx149.val.idx.val.i.i.i.i.i, 0x3FEFFFFA80000000
  %132 = fadd fast float %131, 0xC00D999480000000
  %133 = fmul fast float %.idx153.val.idx.val.i.i.i.i.i, 0x3FEFFFFA80000000
  %134 = fadd fast float %133, 0xC00F332DE0000000
  %135 = fadd float %132, 0x400D9999A0000000
  %136 = fadd float %134, 0x400F333340000000
  %137 = fdiv float %135, 0x3FA99999A0000000
  %138 = fdiv float %136, 0x3FA99999A0000000
  %139 = fmul float %137, 5.000000e-01
  %140 = tail call float @llvm.floor.f32(float %139) #2
  %141 = fmul float %140, 2.000000e+00
  %142 = fsub float %137, %141
  %143 = fptosi float %142 to i32
  %144 = fmul float %138, 5.000000e-01
  %145 = tail call float @llvm.floor.f32(float %144) #2
  %146 = fmul float %145, 2.000000e+00
  %147 = fsub float %138, %146
  %148 = fptosi float %147 to i32
  %149 = srem i32 %143, 2
  %150 = srem i32 %148, 2
  %151 = icmp eq i32 %149, %150
  %storemerge.i.i.i.i.i.i = select i1 %151, float 1.000000e+00, float 0.000000e+00
  %152 = insertelement <4 x float> <float poison, float poison, float poison, float 1.000000e+00>, float %storemerge.i.i.i.i.i.i, i32 0
  %153 = insertelement <4 x float> %152, float %storemerge.i.i.i.i.i.i, i32 1
  %154 = insertelement <4 x float> %153, float %storemerge.i.i.i.i.i.i, i32 2
  br label %evalGLSL_prev3510698853.exit

155:                                              ; preds = %8
  %156 = load i32, i32* %5, align 4, !tbaa !14
  %157 = add nsw i32 %156, -1
  store i32 %157, i32* %5, align 4, !tbaa !14
  %.idx149.val.idx.i.i.i.i.i.i5 = bitcast %"class.Imath_2_2::Vec3.450"* %4 to float*
  %.idx149.val.idx.val.i.i.i.i.i.i = load float, float* %.idx149.val.idx.i.i.i.i.i.i5, align 4, !tbaa.struct !15
  %.idx153.val.idx.i.i.i.i.i.i = getelementptr %"class.Imath_2_2::Vec3.450", %"class.Imath_2_2::Vec3.450"* %4, i64 0, i32 1
  %.idx153.val.idx.val.i.i.i.i.i.i = load float, float* %.idx153.val.idx.i.i.i.i.i.i, align 4, !tbaa.struct !17
  %158 = fmul fast float %.idx149.val.idx.val.i.i.i.i.i.i, 0x3FEFFFFA80000000
  %159 = fadd fast float %158, 0xC00D999480000000
  %160 = fmul fast float %.idx153.val.idx.val.i.i.i.i.i.i, 0x3FEFFFFA80000000
  %161 = fadd fast float %160, 0xC00F332DE0000000
  %162 = fadd float %159, 0x400D9999A0000000
  %163 = fadd float %161, 0x400F333340000000
  %164 = fdiv float %162, 0x3FA99999A0000000
  %165 = fdiv float %163, 0x3FA99999A0000000
  %166 = insertelement <4 x float> <float poison, float poison, float 0.000000e+00, float 1.000000e+00>, float %164, i32 0
  %167 = insertelement <4 x float> %166, float %165, i32 1
  br label %evalGLSL_prev3510698853.exit

evalGLSL_prev3510698853.exit:                     ; preds = %155, %128, %101, %evalGLSL_current3238186431.exit.i, %evalGLSL_current3510698853.exit, %8
  %168 = phi <4 x float> [ %55, %evalGLSL_current3510698853.exit ], [ %100, %evalGLSL_current3238186431.exit.i ], [ %127, %101 ], [ %154, %128 ], [ %167, %155 ], [ zeroinitializer, %8 ]
  ret <4 x float> %168
}

attributes #0 = { alwaysinline nofree nosync nounwind readnone speculatable willreturn }
attributes #1 = { alwaysinline nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+ptx32,+sm_20" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0, !0, !0, !0, !0, !0, !1, !0, !0, !0, !0, !0, !0, !1, !0, !0, !0, !0, !0, !0, !1, !0, !0, !0, !0, !0, !0, !1, !0, !0, !0, !0, !0, !0, !1}
!nvvmir.version = !{!2, !2, !2, !2, !2}
!nvvm.annotations = !{!3, !4, !3, !5, !5, !5, !5, !6, !6, !5, !3, !4, !3, !5, !5, !5, !5, !6, !6, !5, !3, !4, !3, !5, !5, !5, !5, !6, !6, !5, !3, !4, !3, !5, !5, !5, !5, !6, !6, !5, !3, !4, !3, !5, !5, !5, !5, !6, !6, !5}

!0 = !{!"clang version 8.0.1 (git://github.com/llvm-mirror/clang.git 2e4c9c5fc864c2c432e4c262a67c42d824b265c6) (https://git.llvm.org/git/llvm 92cb97c222d9e2325dc10e04c54591aced3e98fe)"}
!1 = !{!"Chaos Group clang version 12.0.1"}
!2 = !{i32 1, i32 4}
!3 = !{null, !"align", i32 8}
!4 = !{null, !"align", i32 8, !"align", i32 65544, !"align", i32 131080}
!5 = !{null, !"align", i32 16}
!6 = !{null, !"align", i32 16, !"align", i32 65552, !"align", i32 131088}
!7 = !{!8, !11, i64 204}
!8 = !{!"", !9, i64 0, !9, i64 8, !9, i64 16, !9, i64 24, !9, i64 72, !9, i64 104, !9, i64 120, !11, i64 136, !11, i64 140, !12, i64 144, !11, i64 148, !11, i64 152, !11, i64 156, !12, i64 160, !11, i64 164, !12, i64 168, !12, i64 172, !11, i64 176, !11, i64 180, !13, i64 184, !11, i64 192, !11, i64 196, !9, i64 200, !11, i64 204, !12, i64 208, !12, i64 212, !11, i64 216, !9, i64 220}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
!11 = !{!"int", !9, i64 0}
!12 = !{!"float", !9, i64 0}
!13 = !{!"long long", !9, i64 0}
!14 = !{!11, !11, i64 0}
!15 = !{i64 0, i64 4, !16, i64 4, i64 4, !16, i64 8, i64 4, !16}
!16 = !{!12, !12, i64 0}
!17 = !{i64 0, i64 4, !16, i64 4, i64 4, !16}
!18 = !{!19, !12, i64 0}
!19 = !{!"", !12, i64 0, !12, i64 4, !12, i64 8, !12, i64 12}
!20 = !{!19, !12, i64 4}
!21 = !{!19, !12, i64 8}
