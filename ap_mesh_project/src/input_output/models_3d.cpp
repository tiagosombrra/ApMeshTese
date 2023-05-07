#include "../../include/input_output/models_3d.h"

Models3d::Models3d() {}

void Models3d::ModelPlanBook(std::shared_ptr<Geometry>& geometry) {
  //=============================== PATCH 1 ==================================
  auto p100 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 0.00000);
  auto p110 = std::make_shared<VertexAdaptive>(0.00000, 0.50000, 0.00000);
  auto p120 = std::make_shared<VertexAdaptive>(0.00000, -0.50000, 0.00000);
  auto p130 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 0.00000);

  auto p101 = std::make_shared<VertexAdaptive>(-0.50000, 1.00000, 0.00000);
  auto p111 = std::make_shared<VertexAdaptive>(-0.50000, 0.50000, 0.00000);
  auto p121 = std::make_shared<VertexAdaptive>(-0.50000, -0.50000, 0.00000);
  auto p131 = std::make_shared<VertexAdaptive>(-0.50000, -1.00000, 0.00000);

  auto p102 = std::make_shared<VertexAdaptive>(-1.50000, 1.00000, 0.00000);
  auto p112 = std::make_shared<VertexAdaptive>(-1.50000, 0.50000, 0.00000);
  auto p122 = std::make_shared<VertexAdaptive>(-1.50000, -0.50000, 0.00000);
  auto p132 = std::make_shared<VertexAdaptive>(-1.50000, -1.00000, 0.00000);

  auto p103 = std::make_shared<VertexAdaptive>(-2.00000, 1.00000, 0.00000);
  auto p113 = std::make_shared<VertexAdaptive>(-2.00000, 0.50000, 0.00000);
  auto p123 = std::make_shared<VertexAdaptive>(-2.00000, -0.50000, 0.00000);
  auto p133 = std::make_shared<VertexAdaptive>(-2.00000, -1.00000, 0.00000);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p110, *p120, *p130);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p130, *p131, *p132, *p133);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p103, *p113, *p123, *p133);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p101, *p102, *p103);

  auto patch1 = std::make_shared<PatchBezier>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================

  //=============================== PATCH 2 ==================================
  auto p200 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 0.00000);
  // Ponto* p210 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p220 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  auto p230 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 0.00000);

  auto p201 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 0.50000);
  auto p211 = std::make_shared<VertexAdaptive>(0.00000, 0.50000, 0.50000);
  auto p221 = std::make_shared<VertexAdaptive>(0.00000, -0.50000, 0.50000);
  auto p231 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 0.50000);

  auto p202 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 1.50000);
  auto p212 = std::make_shared<VertexAdaptive>(0.00000, 0.50000, 1.50000);
  auto p222 = std::make_shared<VertexAdaptive>(0.00000, -0.50000, 1.50000);
  auto p232 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 1.50000);

  auto p203 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 2.00000);
  auto p213 = std::make_shared<VertexAdaptive>(0.00000, 0.50000, 2.00000);
  auto p223 = std::make_shared<VertexAdaptive>(0.00000, -0.50000, 2.00000);
  auto p233 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 2.00000);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  auto patch2_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p230, *p231, *p232, *p233);
  auto patch2_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p203, *p213, *p223, *p233);
  auto patch2_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p200, *p201, *p202, *p203);

  auto patch2 = std::make_shared<PatchBezier>(
      patch1_c1, patch2_c2, patch2_c3, patch2_c4, *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================

  //=============================== PATCH 3 ==================================
  auto p300 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 0.00000);
  // Ponto* p310 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p320 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  auto p330 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 0.00000);

  auto p301 = std::make_shared<VertexAdaptive>(0.50000, 1.00000, 0.00000);
  auto p311 = std::make_shared<VertexAdaptive>(0.50000, 0.50000, 0.00000);
  auto p321 = std::make_shared<VertexAdaptive>(0.50000, -0.50000, 0.00000);
  auto p331 = std::make_shared<VertexAdaptive>(0.50000, -1.00000, 0.00000);

  auto p302 = std::make_shared<VertexAdaptive>(1.50000, 1.00000, 0.00000);
  auto p312 = std::make_shared<VertexAdaptive>(1.50000, 0.50000, 0.00000);
  auto p322 = std::make_shared<VertexAdaptive>(1.50000, -0.50000, 0.00000);
  auto p332 = std::make_shared<VertexAdaptive>(1.50000, -1.00000, 0.00000);

  auto p303 = std::make_shared<VertexAdaptive>(2.00000, 1.00000, 0.00000);
  auto p313 = std::make_shared<VertexAdaptive>(2.00000, 0.50000, 0.00000);
  auto p323 = std::make_shared<VertexAdaptive>(2.00000, -0.50000, 0.00000);
  auto p333 = std::make_shared<VertexAdaptive>(2.00000, -1.00000, 0.00000);

  // Curva* patch3_c1 = new CurvParamBezier ( *p300, *p310, *p320, *p330 );
  auto patch3_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p330, *p331, *p332, *p333);
  auto patch3_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p303, *p313, *p323, *p333);
  auto patch3_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p300, *p301, *p302, *p303);

  auto patch3 = std::make_shared<PatchBezier>(
      patch1_c1, patch3_c2, patch3_c3, patch3_c4, *p311, *p321, *p312, *p322);

  geometry->InsertPatch(patch3);
  // geo->InsertCurve ( patch3_c1 );
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  geometry->InsertCurve(patch3_c4);
  //======================== FIM DO PATCH 3 ==================================

  //=============================== PATCH 4 ==================================
  auto p400 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 0.00000);
  // Ponto* p410 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p420 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  auto p430 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 0.00000);

  auto p401 = std::make_shared<VertexAdaptive>(-0.50000, 1.00000, 1.00000);
  auto p411 = std::make_shared<VertexAdaptive>(-0.50000, 0.50000, 1.00000);
  auto p421 = std::make_shared<VertexAdaptive>(-0.50000, -0.50000, 1.00000);
  auto p431 = std::make_shared<VertexAdaptive>(-0.50000, -1.00000, 1.00000);

  auto p402 = std::make_shared<VertexAdaptive>(-1.50000, 1.00000, 1.00000);
  auto p412 = std::make_shared<VertexAdaptive>(-1.50000, 0.50000, 1.00000);
  auto p422 = std::make_shared<VertexAdaptive>(-1.50000, -0.50000, 1.00000);
  auto p432 = std::make_shared<VertexAdaptive>(-1.50000, -1.00000, 1.00000);

  auto p403 = std::make_shared<VertexAdaptive>(-2.00000, 1.00000, 1.00000);
  auto p413 = std::make_shared<VertexAdaptive>(-2.00000, 0.50000, 1.00000);
  auto p423 = std::make_shared<VertexAdaptive>(-2.00000, -0.50000, 0.50000);
  auto p433 = std::make_shared<VertexAdaptive>(-2.00000, -1.00000, 1.00000);

  // Curva* patch4_c1 = new CurvParamBezier ( *p400, *p410, *p420, *p430 );
  auto patch4_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p430, *p431, *p432, *p433);
  auto patch4_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p403, *p413, *p423, *p433);
  auto patch4_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p400, *p401, *p402, *p403);

  auto patch4 = std::make_shared<PatchBezier>(
      patch1_c1, patch4_c2, patch4_c3, patch4_c4, *p411, *p421, *p412, *p422);

  geometry->InsertPatch(patch4);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch4_c2);
  geometry->InsertCurve(patch4_c3);
  geometry->InsertCurve(patch4_c4);
  //======================== FIM DO PATCH 4 ==================================

  //=============================== PATCH 5 ==================================

  auto p500 = std::make_shared<VertexAdaptive>(0.00000, 1.00000, 0.00000);
  // Ponto* p510 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p520 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  auto p530 = std::make_shared<VertexAdaptive>(0.00000, -1.00000, 0.00000);

  auto p501 = std::make_shared<VertexAdaptive>(0.50000, 1.00000, 0.28371);
  auto p511 = std::make_shared<VertexAdaptive>(0.50000, 0.50000, 0.15342);
  auto p521 = std::make_shared<VertexAdaptive>(0.50000, -0.50000, 0.50617);
  auto p531 = std::make_shared<VertexAdaptive>(0.50000, -1.00000, 0.28371);

  auto p502 = std::make_shared<VertexAdaptive>(1.50000, 1.00000, 0.69502);
  auto p512 = std::make_shared<VertexAdaptive>(1.50000, 0.50000, 0.8342);
  auto p522 = std::make_shared<VertexAdaptive>(1.50000, -0.50000, 1.094);
  auto p532 = std::make_shared<VertexAdaptive>(1.50000, -1.00000, 0.8342);

  auto p503 = std::make_shared<VertexAdaptive>(2.00000, 1.00000, 0.96197);
  auto p513 = std::make_shared<VertexAdaptive>(2.00000, 0.50000, 0.96197);
  auto p523 = std::make_shared<VertexAdaptive>(2.00000, -0.50000, 1.2141);
  auto p533 = std::make_shared<VertexAdaptive>(2.00000, -1.00000, 0.76172);

  // Curva* patch5_c1 = new CurvParamBezier ( *p500, *p510, *p520, *p530 );
  auto patch5_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p530, *p531, *p532, *p533);
  auto patch5_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p503, *p513, *p523, *p533);
  auto patch5_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p500, *p501, *p502, *p503);

  auto patch5 = std::make_shared<PatchBezier>(
      patch1_c1, patch5_c2, patch5_c3, patch5_c4, *p511, *p521, *p512, *p522);

  geometry->InsertPatch(patch5);
  // geo->InsertCurve ( patch5_c1 );
  geometry->InsertCurve(patch5_c2);
  geometry->InsertCurve(patch5_c3);
  geometry->InsertCurve(patch5_c4);
  //======================== FIM DO PATCH 5 ==================================
}

void Models3d::ModelTresPatches(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do three_patches_curve
  //==============================================================================

  //=============================== PATCH 1 ==================================
  auto p100 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, -1.00000);
  auto p110 = std::make_shared<VertexAdaptive>(-1.00000, 0.50000, -1.00000);
  auto p120 = std::make_shared<VertexAdaptive>(-1.00000, -0.50000, -1.00000);
  auto p130 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, -1.00000);

  auto p101 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, -0.50000);
  auto p111 = std::make_shared<VertexAdaptive>(-1.00000, 0.50000, -0.50000);
  auto p121 = std::make_shared<VertexAdaptive>(-1.00000, -0.50000, -0.50000);
  auto p131 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, -0.50000);

  auto p102 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 0.50000);
  auto p112 = std::make_shared<VertexAdaptive>(-0.95046, 0.50000, 0.50000);
  auto p122 = std::make_shared<VertexAdaptive>(-1.19780, -0.50000, 0.50000);
  auto p132 = std::make_shared<VertexAdaptive>(-1.22350, -1.00000, 0.50000);

  auto p103 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 1.00000);
  auto p113 = std::make_shared<VertexAdaptive>(-0.59827, 0.50000, 1.00000);
  auto p123 = std::make_shared<VertexAdaptive>(-1.7191, -0.50000, 1.00000);
  auto p133 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, 1.00000);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p110, *p120, *p130);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p130, *p131, *p132, *p133);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p103, *p113, *p123, *p133);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p101, *p102, *p103);

  auto patch1 = std::make_shared<PatchBezier>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================
  //=============================== PATCH 2 ==================================
  auto p200 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 1.00000);
  // Ponto* p210 = new Vertice ( -0.78827, 0.50000, 1.00000 );
  // Ponto* p220 = new Vertice ( -1.2291, -0.50000, 1.00000 );
  auto p230 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, 1.00000);

  auto p201 = std::make_shared<VertexAdaptive>(-0.50000, 1.00000, 1.00000);
  auto p211 = std::make_shared<VertexAdaptive>(-0.50000, 0.50000, 1.00000);
  auto p221 = std::make_shared<VertexAdaptive>(-0.50000, -0.50000, 1.00000);
  auto p231 = std::make_shared<VertexAdaptive>(-0.50000, -1.00000, 1.00000);

  auto p202 = std::make_shared<VertexAdaptive>(0.50000, 1.00000, 1.00000);
  auto p212 = std::make_shared<VertexAdaptive>(0.50000, 0.50000, 1.00000);
  auto p222 = std::make_shared<VertexAdaptive>(0.50000, -0.50000, 1.00000);
  auto p232 = std::make_shared<VertexAdaptive>(0.50000, -1.00000, 1.00000);

  auto p203 = std::make_shared<VertexAdaptive>(1.00000, 1.00000, 1.00000);
  auto p213 = std::make_shared<VertexAdaptive>(1.00000, 0.50000, 1.00000);
  auto p223 = std::make_shared<VertexAdaptive>(1.00000, -0.50000, 1.00000);
  auto p233 = std::make_shared<VertexAdaptive>(1.00000, -1.00000, 1.00000);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  auto patch2_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p230, *p231, *p232, *p233);
  auto patch2_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p203, *p213, *p223, *p233);
  auto patch2_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p200, *p201, *p202, *p203);

  auto patch2 = std::make_shared<PatchBezier>(
      patch1_c3, patch2_c2, patch2_c3, patch2_c4, *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================

  //=============================== PATCH 3 ==================================
  auto p300 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 1.00000);
  // Ponto* p310 = new Vertice ( -0.78827, 0.50000, 1.00000 );
  // Ponto* p320 = new Vertice ( -1.2291, -0.50000, 1.00000 );
  auto p330 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, 1.00000);

  auto p301 = std::make_shared<VertexAdaptive>(0.12941, 0.65856, 0.66782);
  auto p311 = std::make_shared<VertexAdaptive>(0.46925, 0.26552, 0.78358);
  auto p321 = std::make_shared<VertexAdaptive>(-0.21486, -0.30157, 0.8773);
  auto p331 = std::make_shared<VertexAdaptive>(0.047791, -0.93912, 0.15139);

  auto p302 = std::make_shared<VertexAdaptive>(0.45814, 0.79803, 0.11771);
  auto p312 = std::make_shared<VertexAdaptive>(0.98101, 0.080047, 0.56072);
  auto p322 = std::make_shared<VertexAdaptive>(0.51486, -0.22225, 0.522937);
  auto p332 = std::make_shared<VertexAdaptive>(0.57169, -0.90869, -0.24963);

  auto p303 = std::make_shared<VertexAdaptive>(0.41625, 0.64127, -0.58839);
  auto p313 = std::make_shared<VertexAdaptive>(1.0956, 0.12175, 0.12245);
  auto p323 = std::make_shared<VertexAdaptive>(1.0956, -0.37825, 0.12405);
  auto p333 = std::make_shared<VertexAdaptive>(1.0956, -0.87825, -0.51823);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  auto patch3_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p330, *p331, *p332, *p333);
  auto patch3_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p303, *p313, *p323, *p333);
  auto patch3_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p300, *p301, *p302, *p303);

  auto patch3 = std::make_shared<PatchBezier>(
      patch1_c3, patch3_c2, patch3_c3, patch3_c4, *p311, *p321, *p312, *p322);

  geometry->InsertPatch(patch3);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  geometry->InsertCurve(patch3_c4);
  //======================== FIM DO PATCH 3 ==================================
  //==============================================================================
  // FIM do Exemplo three_patches_curve
  //==============================================================================
}

void Models3d::ModelDoisPatchesPlanosCurva1(
    std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do two_patches_curve
  //==============================================================================

  //=============================== PATCH 1 ==================================
  auto p100 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, -1.00000);
  auto p110 = std::make_shared<VertexAdaptive>(-1.00000, 0.50000, -1.00000);
  auto p120 = std::make_shared<VertexAdaptive>(-1.00000, -0.50000, -1.00000);
  auto p130 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, -1.00000);

  auto p101 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, -0.50000);
  auto p111 = std::make_shared<VertexAdaptive>(-1.00000, 0.50000, -0.50000);
  auto p121 = std::make_shared<VertexAdaptive>(-1.00000, -0.50000, -0.50000);
  auto p131 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, -0.50000);

  auto p102 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 0.50000);
  auto p112 = std::make_shared<VertexAdaptive>(-0.95046, 0.50000, 0.50000);
  auto p122 = std::make_shared<VertexAdaptive>(-1.19780, -0.50000, 0.50000);
  auto p132 = std::make_shared<VertexAdaptive>(-1.22350, -1.00000, 0.50000);

  auto p103 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 1.00000);
  auto p113 = std::make_shared<VertexAdaptive>(-0.59827, 0.50000, 1.00000);
  auto p123 = std::make_shared<VertexAdaptive>(-1.7191, -0.50000, 1.00000);
  auto p133 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, 1.00000);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p110, *p120, *p130);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p130, *p131, *p132, *p133);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p103, *p113, *p123, *p133);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p101, *p102, *p103);

  auto patch1 = std::make_shared<PatchBezier>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================
  //=============================== PATCH 2 ==================================
  auto p200 = std::make_shared<VertexAdaptive>(-1.00000, 1.00000, 1.00000);
  // Ponto* p210 = new Vertice ( -0.78827, 0.50000, 1.00000 );
  // Ponto* p220 = new Vertice ( -1.2291, -0.50000, 1.00000 );
  auto p230 = std::make_shared<VertexAdaptive>(-1.00000, -1.00000, 1.00000);

  auto p201 = std::make_shared<VertexAdaptive>(-0.50000, 1.00000, 1.00000);
  auto p211 = std::make_shared<VertexAdaptive>(-0.50000, 0.50000, 1.00000);
  auto p221 = std::make_shared<VertexAdaptive>(-0.50000, -0.50000, 1.00000);
  auto p231 = std::make_shared<VertexAdaptive>(-0.50000, -1.00000, 1.00000);

  auto p202 = std::make_shared<VertexAdaptive>(0.50000, 1.00000, 1.00000);
  auto p212 = std::make_shared<VertexAdaptive>(0.50000, 0.50000, 1.00000);
  auto p222 = std::make_shared<VertexAdaptive>(0.50000, -0.50000, 1.00000);
  auto p232 = std::make_shared<VertexAdaptive>(0.50000, -1.00000, 1.00000);

  auto p203 = std::make_shared<VertexAdaptive>(1.00000, 1.00000, 1.00000);
  auto p213 = std::make_shared<VertexAdaptive>(1.00000, 0.50000, 1.00000);
  auto p223 = std::make_shared<VertexAdaptive>(1.00000, -0.50000, 1.00000);
  auto p233 = std::make_shared<VertexAdaptive>(1.00000, -1.00000, 1.00000);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  auto patch2_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p230, *p231, *p232, *p233);
  auto patch2_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p203, *p213, *p223, *p233);
  auto patch2_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p200, *p201, *p202, *p203);

  auto patch2 = std::make_shared<PatchBezier>(
      patch1_c3, patch2_c2, patch2_c3, patch2_c4, *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================
  //==============================================================================
  // FIM do Exemplo two_patches
  //==============================================================================
}

void Models3d::ModelDoisPatchesPlanosCurva(
    std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do Utahteapot
  //==============================================================================

  //=============================== PATCH 1 ==================================
  auto p100 = std::make_shared<VertexAdaptive>(-6.0, 0.0, 0.0);
  auto p110 = std::make_shared<VertexAdaptive>(-2.0, 2.0, 0.0);
  auto p120 = std::make_shared<VertexAdaptive>(2.0, -2.0, 1.0);
  auto p130 = std::make_shared<VertexAdaptive>(6.0, 0.0, 0.0);

  auto p101 = std::make_shared<VertexAdaptive>(-6.0, 4.0, 0.0);
  auto p111 = std::make_shared<VertexAdaptive>(-2.0, 4.0, 0.0);
  auto p121 = std::make_shared<VertexAdaptive>(2.0, 4.0, 0.0);
  auto p131 = std::make_shared<VertexAdaptive>(6.0, 4.0, 0.0);

  auto p102 = std::make_shared<VertexAdaptive>(-6.0, 8.0, 0.0);
  auto p112 = std::make_shared<VertexAdaptive>(-2.0, 8.0, 0.0);
  auto p122 = std::make_shared<VertexAdaptive>(2.0, 8.0, 0.0);
  auto p132 = std::make_shared<VertexAdaptive>(6.0, 8.0, 0.0);

  auto p103 = std::make_shared<VertexAdaptive>(-6.0, 12.0, 0.0);
  auto p113 = std::make_shared<VertexAdaptive>(-2.0, 12.0, 0.0);
  auto p123 = std::make_shared<VertexAdaptive>(2.0, 12.0, 0.0);
  auto p133 = std::make_shared<VertexAdaptive>(6.0, 12.0, 0.0);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p110, *p120, *p130);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p130, *p131, *p132, *p133);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p103, *p113, *p123, *p133);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p101, *p102, *p103);

  auto patch1 = std::make_shared<PatchBezier>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================

  //=============================== PATCH 2 ==================================
  auto p200 = std::make_shared<VertexAdaptive>(-6.0, -12.0, 0.0);
  auto p210 = std::make_shared<VertexAdaptive>(-2.0, -12.0, 0.0);
  auto p220 = std::make_shared<VertexAdaptive>(2.0, -12.0, 0.0);
  auto p230 = std::make_shared<VertexAdaptive>(6.0, -12.0, 0.0);

  auto p201 = std::make_shared<VertexAdaptive>(-6.0, -8.0, 0.0);
  auto p211 = std::make_shared<VertexAdaptive>(-2.0, -8.0, 0.0);
  auto p221 = std::make_shared<VertexAdaptive>(2.0, -8.0, 0.0);
  auto p231 = std::make_shared<VertexAdaptive>(6.0, -8.0, 0.0);

  auto p202 = std::make_shared<VertexAdaptive>(-6.0, -4.0, 0.0);
  auto p212 = std::make_shared<VertexAdaptive>(-2.0, -4.0, 0.0);
  auto p222 = std::make_shared<VertexAdaptive>(2.0, -4.0, 1.0);
  auto p232 = std::make_shared<VertexAdaptive>(6.0, -4.0, 0.0);

  auto p203 = p100;  // new Vertice (  0.00000, -1.50000, 2.40000 );
  // Ponto* p213 = p110;//new Vertice ( -0.84000,-1.50000, 2.40000 );
  // Ponto* p223 = p120;//new Vertice ( -1.50000,-0.84000, 2.40000 );
  auto p233 = p130;  // new Vertice ( -1.50000, 0.00000, 2.40000 );

  auto patch2_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p200, *p210, *p220, *p230);
  auto patch2_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p230, *p231, *p232, *p233);
  auto patch2_c3 =
      patch1_c1;  // new CurvParamBezier ( *p133, *p213, *p223, *p233 );
  auto patch2_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p200, *p201, *p202, *p203);

  auto patch2 = std::make_shared<PatchBezier>(
      patch2_c1, patch2_c2, patch2_c3, patch2_c4, *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  geometry->InsertCurve(patch2_c1);
  geometry->InsertCurve(patch2_c2);
  // geo->InsertCurve ( patch2_c3 );
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================
}

void Models3d::ModelCurvaBezier(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo usando curva de Bezier
  //==============================================================================
  // ============================= Patch 1 ===================================

  auto p00 = std::make_shared<VertexAdaptive>(1.0, -1.0, 0.0);
  auto p10 = std::make_shared<VertexAdaptive>(0.5, -0.5, 0.0);
  auto p20 = std::make_shared<VertexAdaptive>(0.5, 0.5, 0.0);
  auto p30 = std::make_shared<VertexAdaptive>(1.0, 1.0, 0.0);

  auto p01 = std::make_shared<VertexAdaptive>(0.5, -1.0, 0.0);
  auto p11 = std::make_shared<VertexAdaptive>(0.5, -0.5, 0.5);
  auto p21 = std::make_shared<VertexAdaptive>(0.5, 0.5, 0.0);
  auto p31 = std::make_shared<VertexAdaptive>(0.5, 1.0, 0.0);

  auto p02 = std::make_shared<VertexAdaptive>(-0.5, -1.0, 0.0);
  auto p12 = std::make_shared<VertexAdaptive>(-0.5, -0.5, 0.0);
  auto p22 = std::make_shared<VertexAdaptive>(-0.5, 0.5, 0.0);
  auto p32 = std::make_shared<VertexAdaptive>(-0.5, 1.0, 0.0);

  auto p03 = std::make_shared<VertexAdaptive>(-1.0, -1.0, 0.0);
  auto p13 = std::make_shared<VertexAdaptive>(-1.0, -0.5, 0.0);
  auto p23 = std::make_shared<VertexAdaptive>(-1.0, 0.5, 0.0);
  auto p33 = std::make_shared<VertexAdaptive>(-1.0, 1.0, 0.0);

  auto patch1_c1 =
      std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p10, *p20, *p30);
  auto patch1_c2 =
      std::make_shared<CurveAdaptiveParametricBezier>(*p30, *p31, *p32, *p33);
  auto patch1_c3 =
      std::make_shared<CurveAdaptiveParametricBezier>(*p03, *p13, *p23, *p33);
  auto patch1_c4 =
      std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p01, *p02, *p03);

  auto patch1 = std::make_shared<PatchBezier>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *p11, *p21, *p12, *p22);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //==============================================================================
  // Fim do exemplo usando curva de Bezier
  //==============================================================================
}

void Models3d::ModelParaboloide(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do PARABOLÓIDE HIPERBÓLICO
  //==============================================================================

  //=============================== PATCH 1 ==================================
  auto patch1_p00 = std::make_shared<VertexAdaptive>(-5.0, -5.0, 0.0);
  auto patch1_p10 = std::make_shared<VertexAdaptive>(5.0, -5.0, 0.0);
  auto patch1_p01 = std::make_shared<VertexAdaptive>(-5.0, 5.0, 0.0);
  auto patch1_p11 = std::make_shared<VertexAdaptive>(5.0, 5.0, 0.0);

  auto patch1_Qu00 = std::make_shared<VectorAdaptive>(1.0, 0.0, 18.25);
  auto patch1_Qu10 = std::make_shared<VectorAdaptive>(
      patch1_Qu00->GetX(), -patch1_Qu00->GetY(), -patch1_Qu00->GetZ());
  auto patch1_Qu01 = std::make_shared<VectorAdaptive>(1.0, 0.0, 18.25);
  auto patch1_Qu11 = std::make_shared<VectorAdaptive>(
      patch1_Qu01->GetX(), -patch1_Qu01->GetY(), -patch1_Qu01->GetZ());

  auto patch1_Qv00 = std::make_shared<VectorAdaptive>(0.0, 1.0, -18.25);
  auto patch1_Qv10 = std::make_shared<VectorAdaptive>(0.0, 1.0, -18.25);
  auto patch1_Qv01 = std::make_shared<VectorAdaptive>(
      -patch1_Qv00->GetX(), patch1_Qv00->GetY(), -patch1_Qv00->GetZ());
  auto patch1_Qv11 = std::make_shared<VectorAdaptive>(
      -patch1_Qv10->GetX(), patch1_Qv10->GetY(), -patch1_Qv10->GetZ());

  auto patch1_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch1_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch1_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch1_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p00, *patch1_p10, *patch1_Qu00, *patch1_Qu10);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p10, *patch1_p11, *patch1_Qv10, *patch1_Qv11);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p01, *patch1_p11, *patch1_Qu01, *patch1_Qu11);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p00, *patch1_p01, *patch1_Qv00, *patch1_Qv01);

  auto patch1 = std::make_shared<PatchHermite>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *patch1_tw00, *patch1_tw10,
      *patch1_tw01, *patch1_tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);

  //======================== FIM DO PATCH 1 ===================================
  //==============================================================================
  // FIM do Exemplo do PARABOLÓIDE HIPERBÓLICO
  //==============================================================================
}

void Models3d::ModelPneu(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do pneu
  //==============================================================================
  auto p00 = std::make_shared<VertexAdaptive>(-5.0, 0.0, -5.0);
  auto p10 = std::make_shared<VertexAdaptive>(5.0, 0.0, -5.0);
  auto p01 = std::make_shared<VertexAdaptive>(-5.0, 0.0, 5.0);
  auto p11 = std::make_shared<VertexAdaptive>(5.0, 0.0, 5.0);

  auto Qu00 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto Qu10 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);
  auto Qu01 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto Qu11 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);

  auto Qv00 = std::make_shared<VectorAdaptive>(-15.0, 0.0, -15.0);
  auto Qv10 = std::make_shared<VectorAdaptive>(15.0, 0.0, -15.0);
  auto Qv01 = std::make_shared<VectorAdaptive>(15.0, 0.0, -15.0);
  auto Qv11 = std::make_shared<VectorAdaptive>(-15.0, 0.0, -15.0);

  auto c1 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p10, *Qu00,
                                                             *Qu10);
  auto c2 = std::make_shared<CurveAdaptiveParametricHermite>(*p10, *p11, *Qv10,
                                                             *Qv11);
  auto c3 = std::make_shared<CurveAdaptiveParametricHermite>(*p01, *p11, *Qu01,
                                                             *Qu11);
  auto c4 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p01, *Qv00,
                                                             *Qv01);
  auto c5 = std::make_shared<CurveAdaptiveParametricHermite>(*p10, *p00, *Qu10,
                                                             *Qu00);
  auto c6 = std::make_shared<CurveAdaptiveParametricHermite>(*p11, *p01, *Qu11,
                                                             *Qu01);

  auto tw_00 = std::make_shared<VectorAdaptive>(0.0, 60.0, 0.0);
  auto tw_10 = std::make_shared<VectorAdaptive>(0.0, -60.0, 0.0);
  auto tw_01 = std::make_shared<VectorAdaptive>(0.0, -60.0, 0.0);
  auto tw_11 = std::make_shared<VectorAdaptive>(0.0, 60.0, 0.0);

  auto tw00 = std::make_shared<VectorAdaptive>(0.0, -60.0, 0.0);
  auto tw10 = std::make_shared<VectorAdaptive>(0.0, 60.0, 0.0);
  auto tw01 = std::make_shared<VectorAdaptive>(0.0, 60.0, 0.0);
  auto tw11 = std::make_shared<VectorAdaptive>(0.0, -60.0, 0.0);

  auto patch1 = std::make_shared<PatchHermite>(c1, c2, c3, c4, *tw00, *tw10,
                                               *tw01, *tw11);
  auto patch2 = std::make_shared<PatchHermite>(c5, c4, c6, c2, *tw_00, *tw_10,
                                               *tw_01, *tw_11);

  geometry->InsertPatch(patch1);
  geometry->InsertPatch(patch2);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  geometry->InsertCurve(c5);
  geometry->InsertCurve(c6);

  //==============================================================================
  // Fim do Exemplo do pneu
  //=======================================================================
}

void Models3d::ModelLadoDescendente(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo de uma superfície com um dos lados descendentes
  //==============================================================================
  auto p00 = std::make_shared<VertexAdaptive>(-0.5, 0.0, 0.5);
  auto p10 = std::make_shared<VertexAdaptive>(0.5, 0.0, 0.5);
  auto p01 = std::make_shared<VertexAdaptive>(-0.5, 0.0, -0.5);
  auto p11 = std::make_shared<VertexAdaptive>(0.5, -0.5, -0.5);

  auto Qu00 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);
  auto Qu10 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);
  auto Qu01 = std::make_shared<VectorAdaptive>(3.0, 0.0, 0.0);
  auto Qu11 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);

  auto Qv00 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);
  auto Qv10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -3.0);
  auto Qv01 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);
  auto Qv11 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);

  auto tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto c1 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p10, *Qu00,
                                                             *Qu10);
  auto c2 = std::make_shared<CurveAdaptiveParametricHermite>(*p10, *p11, *Qv10,
                                                             *Qv11);
  auto c3 = std::make_shared<CurveAdaptiveParametricHermite>(*p01, *p11, *Qu01,
                                                             *Qu11);
  auto c4 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p01, *Qv00,
                                                             *Qv01);
  auto patch1 = std::make_shared<PatchHermite>(c1, c2, c3, c4, *tw00, *tw10,
                                               *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);

  //==============================================================================
  // FIM do Exemplo de uma superfície com um dos lados descendentes
  //=======================================================================
}

void Models3d::ModelBaseCircular(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo da base circular
  //==============================================================================
  auto p00 = std::make_shared<VertexAdaptive>(0.0, -5.0, 0.0);
  auto p10 = std::make_shared<VertexAdaptive>(5.0, 0.0, 0.0);
  auto p11 = std::make_shared<VertexAdaptive>(0.0, 5.0, 0.0);
  auto p01 = std::make_shared<VertexAdaptive>(-5.0, 0.0, 0.0);

  auto Qu00 = std::make_shared<VectorAdaptive>(8.25, 0.0, 0.0);
  auto Qu10 = std::make_shared<VectorAdaptive>(0.0, 8.25, 0.0);
  auto Qu01 = std::make_shared<VectorAdaptive>(0.0, 8.25, 0.0);
  auto Qu11 = std::make_shared<VectorAdaptive>(8.25, 0.0, 0.0);
  auto Qv00 = std::make_shared<VectorAdaptive>(-8.25, 0.0, 0.0);
  auto Qv10 = std::make_shared<VectorAdaptive>(0.0, 8.25, 0.0);
  auto Qv01 = std::make_shared<VectorAdaptive>(0.0, 8.25, 0.0);
  auto Qv11 = std::make_shared<VectorAdaptive>(-8.25, 0.0, 0.0);

  auto tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 10.0);
  auto tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -10.0);
  auto tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, -10.0);
  auto tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 10.0);

  auto c1 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p10, *Qu00,
                                                             *Qu10);
  auto c2 = std::make_shared<CurveAdaptiveParametricHermite>(*p10, *p11, *Qv10,
                                                             *Qv11);
  auto c3 = std::make_shared<CurveAdaptiveParametricHermite>(*p01, *p11, *Qu01,
                                                             *Qu11);
  auto c4 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p01, *Qv00,
                                                             *Qv01);

  auto patch1 = std::make_shared<PatchHermite>(c1, c2, c3, c4, *tw00, *tw10,
                                               *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  //==============================================================================
  // FIM do Exemplo da base circular
  //=======================================================================
}

void Models3d::ModelBaseQuadrada(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do patch com base quadrada
  //==============================================================================
  auto p00 = std::make_shared<VertexAdaptive>(-0.5, 0.0, 0.5);
  auto p10 = std::make_shared<VertexAdaptive>(0.5, 0.0, 0.5);
  auto p01 = std::make_shared<VertexAdaptive>(-0.5, 0.0, -0.5);
  auto p11 = std::make_shared<VertexAdaptive>(0.5, 0.0, -0.5);

  auto Qu00 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);
  auto Qu10 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);
  auto Qu01 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);
  auto Qu11 = std::make_shared<VectorAdaptive>(1.0, 0.0, 0.0);

  auto Qv00 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);
  auto Qv10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);
  auto Qv01 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);
  auto Qv11 = std::make_shared<VectorAdaptive>(0.0, 0.0, -1.0);

  auto tw00 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);
  auto tw10 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto tw01 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto tw11 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);

  auto c1 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p10, *Qu00,
                                                             *Qu10);
  auto c2 = std::make_shared<CurveAdaptiveParametricHermite>(*p10, *p11, *Qv10,
                                                             *Qv11);
  auto c3 = std::make_shared<CurveAdaptiveParametricHermite>(*p01, *p11, *Qu01,
                                                             *Qu11);
  auto c4 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p01, *Qv00,
                                                             *Qv01);

  auto patch1 = std::make_shared<PatchHermite>(c1, c2, c3, c4, *tw00, *tw10,
                                               *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  //==============================================================================
  // FIM do exemplo do patch com base quadrada
  //=======================================================================
}

void Models3d::ModelBordaCurva(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do patch com borda curva
  //==============================================================================
  auto p00 = std::make_shared<VertexAdaptive>(-5.0, -5.0, 0.0);
  auto p10 = std::make_shared<VertexAdaptive>(5.0, -5.0, 0.0);
  auto p01 = std::make_shared<VertexAdaptive>(-5.0, 5.0, 0.0);
  auto p11 = std::make_shared<VertexAdaptive>(5.0, 5.0, 0.0);

  auto Qu00 = std::make_shared<VectorAdaptive>(10.0, 10.0, 0.0);
  auto Qu10 = std::make_shared<VectorAdaptive>(10.0, 10.0, 0.0);
  auto Qu01 = std::make_shared<VectorAdaptive>(10.0, 10.0, 0.0);
  auto Qu11 = std::make_shared<VectorAdaptive>(10.0, 10.0, 0.0);

  auto Qv00 = std::make_shared<VectorAdaptive>(-10.0, 10.0, 0.0);
  auto Qv10 = std::make_shared<VectorAdaptive>(-10.0, 10.0, 0.0);
  auto Qv01 = std::make_shared<VectorAdaptive>(-10.0, 10.0, 0.0);
  auto Qv11 = std::make_shared<VectorAdaptive>(-10.0, 10.0, 0.0);

  auto tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto c1 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p10, *Qu00,
                                                             *Qu10);
  auto c2 = std::make_shared<CurveAdaptiveParametricHermite>(*p10, *p11, *Qv10,
                                                             *Qv11);
  auto c3 = std::make_shared<CurveAdaptiveParametricHermite>(*p01, *p11, *Qu01,
                                                             *Qu11);
  auto c4 = std::make_shared<CurveAdaptiveParametricHermite>(*p00, *p01, *Qv00,
                                                             *Qv01);

  auto patch1 = std::make_shared<PatchHermite>(c1, c2, c3, c4, *tw00, *tw10,
                                               *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  //==============================================================================
  // FIM do exemplo do patch com borda curva
  //=======================================================================
}

void Models3d::ModelDoisPatches(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo de dois patches vizinhos
  //==============================================================================
  auto p100 = std::make_shared<VertexAdaptive>(-1.0, 0.0, 0.5);
  auto p110 = std::make_shared<VertexAdaptive>(0.0, 0.0, 0.5);
  auto p101 = std::make_shared<VertexAdaptive>(-1.0, 0.0, -0.5);
  auto p111 = std::make_shared<VertexAdaptive>(0.0, 0.0, -0.5);

  auto p200 = std::make_shared<VertexAdaptive>(0.0, 0.0, 0.5);
  auto p210 = std::make_shared<VertexAdaptive>(1.0, 0.0, 0.5);
  auto p201 = std::make_shared<VertexAdaptive>(0.0, 0.0, -0.5);
  auto p211 = std::make_shared<VertexAdaptive>(1.0, 0.0, -0.5);

  auto Qu00 = std::make_shared<VectorAdaptive>(0.1, 0.0, 0.0);
  auto Qu10 = std::make_shared<VectorAdaptive>(0.1, 0.0, 0.0);
  auto Qu01 = std::make_shared<VectorAdaptive>(0.1, 0.0, 0.0);
  auto Qu11 = std::make_shared<VectorAdaptive>(0.1, 0.0, 0.0);

  auto Qv00 = std::make_shared<VectorAdaptive>(0.0, 0.0, -0.1);
  auto Qv10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -0.1);
  auto Qv01 = std::make_shared<VectorAdaptive>(0.0, 0.0, -0.1);
  auto Qv11 = std::make_shared<VectorAdaptive>(0.0, 0.0, -0.1);

  auto tw100 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);
  auto tw110 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto tw101 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto tw111 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);

  auto tw200 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);
  auto tw210 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto tw201 = std::make_shared<VectorAdaptive>(0.0, -20.0, 0.0);
  auto tw211 = std::make_shared<VectorAdaptive>(0.0, 20.0, 0.0);

  auto c1 = std::make_shared<CurveAdaptiveParametricHermite>(*p100, *p110,
                                                             *Qu00, *Qu10);
  auto c2 = std::make_shared<CurveAdaptiveParametricHermite>(*p110, *p111,
                                                             *Qv10, *Qv11);
  auto c3 = std::make_shared<CurveAdaptiveParametricHermite>(*p101, *p111,
                                                             *Qu01, *Qu11);
  auto c4 = std::make_shared<CurveAdaptiveParametricHermite>(*p100, *p101,
                                                             *Qv00, *Qv01);

  auto c5 = std::make_shared<CurveAdaptiveParametricHermite>(*p200, *p210,
                                                             *Qu00, *Qu10);
  auto c6 = std::make_shared<CurveAdaptiveParametricHermite>(*p210, *p211,
                                                             *Qv10, *Qv11);
  auto c7 = std::make_shared<CurveAdaptiveParametricHermite>(*p201, *p211,
                                                             *Qu01, *Qu11);
  // Curva* c8 = new CurvParamHermite ( *p200, *p201, *Qv00, *Qv01 );

  auto patch1 = std::make_shared<PatchHermite>(c1, c2, c3, c4, *tw100, *tw110,
                                               *tw101, *tw111);
  auto patch2 = std::make_shared<PatchHermite>(c5, c6, c7, c2, *tw200, *tw210,
                                               *tw201, *tw211);

  geometry->InsertPatch(patch1);
  geometry->InsertPatch(patch2);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  geometry->InsertCurve(c5);
  geometry->InsertCurve(c6);
  geometry->InsertCurve(c7);
  //==============================================================================
  // Fim do Exemplo de dois patches vizinhos
  //=======================================================================
}

void Models3d::ModelNariz(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do nariz
  //==============================================================================
  //=============================== PATCH 1 ===================================
  auto patch1_p00 = std::make_shared<VertexAdaptive>(-8.00, 2.5, 0.0);
  auto patch1_p10 = std::make_shared<VertexAdaptive>(-2.50, 0.0, 0.0);
  auto patch1_p01 = std::make_shared<VertexAdaptive>(-7.00, 2.5, 5.0);
  auto patch1_p11 = std::make_shared<VertexAdaptive>(-3.50, 0.0, 7.5);

  auto patch1_Qu00 = std::make_shared<VectorAdaptive>(5.5, -2.5, 18.0);
  auto patch1_Qu10 = std::make_shared<VectorAdaptive>(
      patch1_Qu00->GetX(), patch1_Qu00->GetY(), -patch1_Qu00->GetZ());
  auto patch1_Qu01 = std::make_shared<VectorAdaptive>(3.5, -2.5, 2.5);
  auto patch1_Qu11 = std::make_shared<VectorAdaptive>(
      patch1_Qu01->GetX(), patch1_Qu01->GetY(), patch1_Qu01->GetZ());

  auto patch1_Qv00 = std::make_shared<VectorAdaptive>(-7.5, 2.5, 0.0);
  auto patch1_Qv10 = std::make_shared<VectorAdaptive>(3.5, -5.0, 7.5);
  auto patch1_Qv01 = std::make_shared<VectorAdaptive>(5.5, 2.5, 0.0);
  auto patch1_Qv11 = std::make_shared<VectorAdaptive>(
      -patch1_Qv10->GetX(), -patch1_Qv10->GetY(), patch1_Qv10->GetZ());

  auto patch1_tw00 = std::make_shared<VectorAdaptive>(0.0, -10.0, 0.0);
  auto patch1_tw10 = std::make_shared<VectorAdaptive>(0.0, 10.0, 0.0);
  auto patch1_tw01 = std::make_shared<VectorAdaptive>(0.0, 10.0, 0.0);
  auto patch1_tw11 = std::make_shared<VectorAdaptive>(0.0, -10.0, 0.0);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p00, *patch1_p10, *patch1_Qu00, *patch1_Qu10);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p10, *patch1_p11, *patch1_Qv10, *patch1_Qv11);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p01, *patch1_p11, *patch1_Qu01, *patch1_Qu11);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch1_p00, *patch1_p01, *patch1_Qv00, *patch1_Qv01);

  auto patch1 = std::make_shared<PatchHermite>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *patch1_tw00, *patch1_tw10,
      *patch1_tw01, *patch1_tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ===================================

  //=============================== PATCH 2 ===================================
  auto patch2_p00 = patch1_p10;
  auto patch2_p10 = std::make_shared<VertexAdaptive>(
      -patch1_p10->GetX(), patch1_p10->GetY(), patch1_p10->GetZ());
  auto patch2_p01 = patch1_p11;
  auto patch2_p11 = std::make_shared<VertexAdaptive>(
      -patch1_p11->GetX(), patch1_p11->GetY(), patch1_p11->GetZ());

  auto patch2_Qu00 = std::make_shared<VectorAdaptive>(7.5, -2.5, 0.0);
  auto patch2_Qu10 = std::make_shared<VectorAdaptive>(7.5, 2.5, 0.0);
  auto patch2_Qu01 = std::make_shared<VectorAdaptive>(7.0, -5.0, 5.0);
  auto patch2_Qu11 = std::make_shared<VectorAdaptive>(7.0, 5.0, -5.0);

  // auto patch2_Qv00 = std::make_shared<VectorAdaptive> ( *patch2_p00,
  // *patch2_p01
  // );
  auto patch2_Qv10 = std::make_shared<VectorAdaptive>(
      -patch1_Qv10->GetX(), patch1_Qv10->GetY(), patch1_Qv10->GetZ());
  // auto patch2_Qv01 = std::make_shared<VectorAdaptive> ( *patch2_p00,
  // *patch2_p01
  // );
  auto patch2_Qv11 = std::make_shared<VectorAdaptive>(
      -patch1_Qv11->GetX(), patch1_Qv11->GetY(), patch1_Qv11->GetZ());

  auto patch2_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch2_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch2_tw01 = std::make_shared<VectorAdaptive>(0.0, 10.0, 0.0);
  auto patch2_tw11 = std::make_shared<VectorAdaptive>(0.0, -10.0, 0.0);

  auto patch2_c1 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch2_p00, *patch2_p10, *patch2_Qu00, *patch2_Qu10);
  auto patch2_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch2_p10, *patch2_p11, *patch2_Qv10, *patch2_Qv11);
  auto patch2_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch2_p01, *patch2_p11, *patch2_Qu01, *patch2_Qu11);
  auto patch2_c4 = patch1_c2;

  auto patch2 = std::make_shared<PatchHermite>(
      patch2_c1, patch2_c2, patch2_c3, patch2_c4, *patch2_tw00, *patch2_tw10,
      *patch2_tw01, *patch2_tw11);

  geometry->InsertPatch(patch2);
  geometry->InsertCurve(patch2_c1);
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  // geo->InsertCurve ( patch2_c4 );
  //======================== FIM DO PATCH 2 ===================================

  //=============================== PATCH 3 ===================================
  auto patch3_p00 = patch2_p10;
  auto patch3_p10 = std::make_shared<VertexAdaptive>(
      -patch1_p00->GetX(), patch1_p00->GetY(), patch1_p00->GetZ());
  auto patch3_p01 = patch2_p11;
  auto patch3_p11 = std::make_shared<VertexAdaptive>(
      -patch1_p01->GetX(), patch1_p01->GetY(), patch1_p01->GetZ());

  auto patch3_Qu00 = std::make_shared<VectorAdaptive>(-(-(*patch1_Qu10)).GetX(),
                                                      (-(*patch1_Qu10)).GetY(),
                                                      (-(*patch1_Qu10)).GetZ());
  auto patch3_Qu10 = std::make_shared<VectorAdaptive>(-(-(*patch1_Qu00)).GetX(),
                                                      (-(*patch1_Qu00)).GetY(),
                                                      (-(*patch1_Qu00)).GetZ());
  auto patch3_Qu01 = std::make_shared<VectorAdaptive>(-(-(*patch1_Qu11)).GetX(),
                                                      (-(*patch1_Qu11)).GetY(),
                                                      (-(*patch1_Qu11)).GetZ());
  auto patch3_Qu11 = std::make_shared<VectorAdaptive>(-(-(*patch1_Qu01)).GetX(),
                                                      (-(*patch1_Qu01)).GetY(),
                                                      (-(*patch1_Qu01)).GetZ());

  // auto patch3_Qv00 = std::make_shared<VectorAdaptive> ( -patch1_Qv10->x,
  // patch1_Qv10->GetY(), patch1_Qv10->GetZ() );
  auto patch3_Qv10 = std::make_shared<VectorAdaptive>(
      -patch1_Qv00->GetX(), patch1_Qv00->GetY(), patch1_Qv00->GetZ());
  // auto patch3_Qv01 = std::make_shared<VectorAdaptive> ( -patch1_Qv11->x,
  // patch1_Qv11->GetY(), patch1_Qv11->GetZ() );
  auto patch3_Qv11 = std::make_shared<VectorAdaptive>(
      -patch1_Qv01->GetX(), patch1_Qv01->GetY(), patch1_Qv01->GetZ());

  auto patch3_tw00 = std::make_shared<VectorAdaptive>(
      patch1_tw00->GetX(), patch1_tw00->GetY(), patch1_tw00->GetZ());
  auto patch3_tw10 = std::make_shared<VectorAdaptive>(
      patch1_tw10->GetX(), patch1_tw10->GetY(), patch1_tw10->GetZ());
  auto patch3_tw01 = std::make_shared<VectorAdaptive>(
      patch1_tw01->GetX(), patch1_tw01->GetY(), patch1_tw01->GetZ());
  auto patch3_tw11 = std::make_shared<VectorAdaptive>(
      patch1_tw11->GetX(), patch1_tw11->GetY(), patch1_tw11->GetZ());

  auto patch3_c1 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch3_p00, *patch3_p10, *patch3_Qu00, *patch3_Qu10);
  auto patch3_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch3_p10, *patch3_p11, *patch3_Qv10, *patch3_Qv11);
  auto patch3_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch3_p01, *patch3_p11, *patch3_Qu01, *patch3_Qu11);
  auto patch3_c4 = patch2_c2;

  auto patch3 = std::make_shared<PatchHermite>(
      patch3_c1, patch3_c2, patch3_c3, patch3_c4, *patch3_tw00, *patch3_tw10,
      *patch3_tw01, *patch3_tw11);

  geometry->InsertPatch(patch3);
  geometry->InsertCurve(patch3_c1);
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  // geo->InsertCurve ( patch3_c4 );
  //======================== FIM DO PATCH 3 ===================================

  //=============================== PATCH 4 ===================================
  auto patch4_p00 = patch1_p01;
  auto patch4_p10 = patch1_p11;
  auto patch4_p01 = std::make_shared<VertexAdaptive>(-6.00, 5.0, 2.5);
  auto patch4_p11 = std::make_shared<VertexAdaptive>(-3.50, 4.0, 7.5);

  // auto patch4_Qu00 = std::make_shared<VectorAdaptive> ( *patch4_p00,
  // *patch4_p10
  // ); auto patch4_Qu10 = std::make_shared<VectorAdaptive> ( *patch4_p00,
  // *patch4_p10 );
  auto patch4_Qu01 = std::make_shared<VectorAdaptive>(*patch4_p01, *patch4_p11);
  auto patch4_Qu11 = std::make_shared<VectorAdaptive>(*patch4_p01, *patch4_p11);

  auto patch4_Qv00 = std::make_shared<VectorAdaptive>(*patch4_p00, *patch4_p01);
  auto patch4_Qv10 = std::make_shared<VectorAdaptive>(*patch4_p10, *patch4_p11);
  auto patch4_Qv01 = std::make_shared<VectorAdaptive>(*patch4_p00, *patch4_p01);
  auto patch4_Qv11 = std::make_shared<VectorAdaptive>(*patch4_p10, *patch4_p11);

  auto patch4_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 10.0);
  auto patch4_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -10.0);
  auto patch4_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch4_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch4_c1 = patch1_c3;
  auto patch4_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch4_p10, *patch4_p11, *patch4_Qv10, *patch4_Qv11);
  auto patch4_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch4_p01, *patch4_p11, *patch4_Qu01, *patch4_Qu11);
  auto patch4_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch4_p00, *patch4_p01, *patch4_Qv00, *patch4_Qv01);

  auto patch4 = std::make_shared<PatchHermite>(
      patch4_c1, patch4_c2, patch4_c3, patch4_c4, *patch4_tw00, *patch4_tw10,
      *patch4_tw01, *patch4_tw11);

  geometry->InsertPatch(patch4);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch4_c2);
  geometry->InsertCurve(patch4_c3);
  geometry->InsertCurve(patch4_c4);
  //======================== FIM DO PATCH 4 ===================================

  //=============================== PATCH 5 ===================================
  // Ponto* patch5_p00 = patch1_p11;
  auto patch5_p10 = patch2_p11;
  auto patch5_p01 = patch4_p11;
  auto patch5_p11 = std::make_shared<VertexAdaptive>(
      -patch4_p11->GetX(), patch4_p11->GetY(), patch4_p11->GetZ());

  // auto patch5_Qu00 = std::make_shared<VectorAdaptive> ( *patch5_p00,
  // *patch5_p10
  // ); auto patch5_Qu10 = std::make_shared<VectorAdaptive> ( *patch5_p00,
  // *patch5_p10 );
  auto patch5_Qu01 = std::make_shared<VectorAdaptive>(
      patch4_Qu11->GetX(), patch4_Qu11->GetY(), patch4_Qu11->GetZ());
  auto patch5_Qu11 = std::make_shared<VectorAdaptive>(
      patch5_Qu01->GetX(), -patch5_Qu01->GetY(), -patch5_Qu01->GetZ());

  // auto patch5_Qv00 = std::make_shared<VectorAdaptive> ( *patch5_p00,
  // *patch5_p01
  // );
  auto patch5_Qv10 = std::make_shared<VectorAdaptive>(
      -patch4_Qv10->GetX(), patch4_Qv10->GetY(), patch4_Qv10->GetZ());
  // auto patch5_Qv01 = std::make_shared<VectorAdaptive> ( *patch5_p00,
  // *patch5_p01
  // );
  auto patch5_Qv11 = std::make_shared<VectorAdaptive>(
      -patch4_Qv11->GetX(), patch4_Qv11->GetY(), patch4_Qv11->GetZ());

  auto patch5_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 20.0);
  auto patch5_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -20.0);
  auto patch5_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, -5.0);
  auto patch5_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 5.0);

  auto patch5_c1 = patch2_c3;
  auto patch5_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch5_p10, *patch5_p11, *patch5_Qv10, *patch5_Qv11);
  auto patch5_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch5_p01, *patch5_p11, *patch5_Qu01, *patch5_Qu11);
  auto patch5_c4 = patch4_c2;

  auto patch5 = std::make_shared<PatchHermite>(
      patch5_c1, patch5_c2, patch5_c3, patch5_c4, *patch5_tw00, *patch5_tw10,
      *patch5_tw01, *patch5_tw11);

  geometry->InsertPatch(patch5);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch5_c2);
  geometry->InsertCurve(patch5_c3);
  // geo->InsertCurve ( patch5_c4 );
  //======================== FIM DO PATCH 5 ===================================

  //=============================== PATCH 6 ===================================
  // Ponto* patch6_p00 = patch2_p11;
  auto patch6_p10 = patch3_p11;
  auto patch6_p01 = patch5_p11;
  auto patch6_p11 = std::make_shared<VertexAdaptive>(
      -patch4_p01->GetX(), patch4_p01->GetY(), patch4_p01->GetZ());

  // auto patch6_Qu00 = std::make_shared<VectorAdaptive> ( patch5_Qu11->x,
  // patch5_Qu11->GetY(), patch5_Qu11->GetZ() ); auto patch6_Qu10 =
  // std::make_shared<VectorAdaptive> (
  // patch6_Qu00->x,-patch6_Qu00->GetY(),-patch6_Qu00->GetZ() );
  auto patch6_Qu01 = std::make_shared<VectorAdaptive>(
      patch4_Qu11->GetX(), -patch4_Qu11->GetY(), -patch4_Qu11->GetZ());
  auto patch6_Qu11 = std::make_shared<VectorAdaptive>(
      patch4_Qu01->GetX(), -patch4_Qu01->GetY(), -patch4_Qu01->GetZ());

  // auto patch6_Qv00 = std::make_shared<VectorAdaptive> ( *patch6_p00,
  // *patch6_p01
  // );
  auto patch6_Qv10 = std::make_shared<VectorAdaptive>(
      -patch4_Qv00->GetX(), patch4_Qv00->GetY(), patch4_Qv00->GetZ());
  // auto patch6_Qv01 = std::make_shared<VectorAdaptive> ( *patch6_p00,
  // *patch6_p01
  // );
  auto patch6_Qv11 = std::make_shared<VectorAdaptive>(
      -patch4_Qv01->GetX(), patch4_Qv01->GetY(), patch4_Qv01->GetZ());

  auto patch6_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 10.0);
  auto patch6_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, -10.0);
  auto patch6_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch6_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch6_c1 = patch3_c3;
  auto patch6_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch6_p10, *patch6_p11, *patch6_Qv10, *patch6_Qv11);
  auto patch6_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch6_p01, *patch6_p11, *patch6_Qu01, *patch6_Qu11);
  auto patch6_c4 = patch5_c2;

  auto patch6 = std::make_shared<PatchHermite>(
      patch6_c1, patch6_c2, patch6_c3, patch6_c4, *patch6_tw00, *patch6_tw10,
      *patch6_tw01, *patch6_tw11);

  geometry->InsertPatch(patch6);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch6_c2);
  geometry->InsertCurve(patch6_c3);
  // geo->InsertCurve ( patch5_c4 );
  //======================== FIM DO PATCH 6 ===================================

  //=============================== PATCH 8 ===================================
  auto patch8_p00 = patch4_p11;
  auto patch8_p10 = patch5_p11;
  auto patch8_p01 = std::make_shared<VertexAdaptive>(-2.5, 7.5, 6.5);
  auto patch8_p11 = std::make_shared<VertexAdaptive>(2.5, 7.5, 6.5);

  // auto patch8_Qu00 = std::make_shared<VectorAdaptive> ( patch4_Qu11->x,
  // patch4_Qu11->GetY(), patch4_Qu11->GetZ() ); auto patch8_Qu10 =
  // std::make_shared<VectorAdaptive> (
  // patch5_Qu01->x,-patch5_Qu01->GetY(),-patch5_Qu01->GetZ() );
  auto patch8_Qu01 = std::make_shared<VectorAdaptive>(5.0, -3.5, 3.5);
  auto patch8_Qu11 = std::make_shared<VectorAdaptive>(
      patch8_Qu01->GetX(), -patch8_Qu01->GetY(), -patch8_Qu01->GetZ());

  auto patch8_Qv00 = std::make_shared<VectorAdaptive>(*patch5_Qv10);
  auto patch8_Qv10 = std::make_shared<VectorAdaptive>(*patch5_Qv11);
  auto patch8_Qv01 = std::make_shared<VectorAdaptive>(*patch8_p00, *patch8_p01);
  auto patch8_Qv11 = std::make_shared<VectorAdaptive>(*patch8_p10, *patch8_p11);

  auto patch8_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, -8.0);
  auto patch8_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 8.0);
  auto patch8_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch8_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch8_c1 = patch5_c3;
  auto patch8_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch8_p10, *patch8_p11, *patch8_Qv10, *patch8_Qv11);
  auto patch8_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch8_p01, *patch8_p11, *patch8_Qu01, *patch8_Qu11);
  auto patch8_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch8_p00, *patch8_p01, *patch8_Qv00, *patch8_Qv01);

  auto patch8 = std::make_shared<PatchHermite>(
      patch8_c1, patch8_c2, patch8_c3, patch8_c4, *patch8_tw00, *patch8_tw10,
      *patch8_tw01, *patch8_tw11);

  geometry->InsertPatch(patch8);
  // geo->InsertCurve ( patch8_c1 );
  geometry->InsertCurve(patch8_c2);
  geometry->InsertCurve(patch8_c3);
  geometry->InsertCurve(patch8_c4);
  //======================== FIM DO PATCH 8 ===================================

  //=============================== PATCH 10 ==================================
  auto patch10_p00 = patch8_p01;
  auto patch10_p10 = patch8_p11;
  auto patch10_p01 = std::make_shared<VertexAdaptive>(-2.0, 22.5, 2.5);
  auto patch10_p11 = std::make_shared<VertexAdaptive>(
      -patch10_p01->GetX(), patch10_p01->GetY(), patch10_p01->GetZ());

  // auto patch10_Qu00 = std::make_shared<VectorAdaptive> ( *patch8_Qu01 );
  // auto patch10_Qu10 = std::make_shared<VectorAdaptive> ( *patch8_Qu11 );
  auto patch10_Qu01 = std::make_shared<VectorAdaptive>(1.0, 2.5, 3.5);
  auto patch10_Qu11 = std::make_shared<VectorAdaptive>(
      patch10_Qu01->GetX(), -patch10_Qu01->GetY(), -patch10_Qu01->GetZ());

  auto patch10_Qv00 = std::make_shared<VectorAdaptive>(*patch8_Qv01);
  auto patch10_Qv10 = std::make_shared<VectorAdaptive>(*patch8_Qv11);
  auto patch10_Qv01 = std::make_shared<VectorAdaptive>(-1.0, 2.5, 0.0);
  auto patch10_Qv11 = std::make_shared<VectorAdaptive>(
      -patch10_Qv01->GetX(), patch10_Qv01->GetY(), patch10_Qv01->GetZ());

  auto patch10_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch10_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch10_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch10_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch10_c1 = patch8_c3;
  auto patch10_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch10_p10, *patch10_p11, *patch10_Qv10, *patch10_Qv11);
  auto patch10_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch10_p01, *patch10_p11, *patch10_Qu01, *patch10_Qu11);
  auto patch10_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch10_p00, *patch10_p01, *patch10_Qv00, *patch10_Qv01);

  auto patch10 = std::make_shared<PatchHermite>(
      patch10_c1, patch10_c2, patch10_c3, patch10_c4, *patch10_tw00,
      *patch10_tw10, *patch10_tw01, *patch10_tw11);

  geometry->InsertPatch(patch10);
  // geo->InsertCurve ( patch10_c1 );
  geometry->InsertCurve(patch10_c2);
  geometry->InsertCurve(patch10_c3);
  geometry->InsertCurve(patch10_c4);
  //======================== FIM DO PATCH 10 ==================================

  //=============================== PATCH 7 ===================================
  auto patch7_p00 = patch4_p01;
  // Ponto* patch7_p10 = patch4_p11;
  auto patch7_p01 = patch10_p01;
  auto patch7_p11 = patch10_p00;

  // auto patch7_Qu00 = std::make_shared<VectorAdaptive> ( *patch4_Qu01  );
  // auto patch7_Qu10 = std::make_shared<VectorAdaptive> ( *patch4_Qu11  );
  auto patch7_Qu01 = std::make_shared<VectorAdaptive>(-(*patch10_Qv01));
  auto patch7_Qu11 = std::make_shared<VectorAdaptive>(-(*patch10_Qv00));

  auto patch7_Qv00 = std::make_shared<VectorAdaptive>(5.0, 1.5, -8.0);
  // auto patch7_Qv10 = std::make_shared<VectorAdaptive> ( *patch8_Qv00 );
  auto patch7_Qv01 = std::make_shared<VectorAdaptive>(
      -patch7_Qv00->GetX(), patch7_Qv00->GetY(), -patch7_Qv00->GetZ());
  // auto patch7_Qv11 = std::make_shared<VectorAdaptive> ( *patch8_Qv00 );

  auto patch7_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch7_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch7_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch7_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch7_c1 = patch4_c3;
  auto patch7_c2 = patch8_c4;
  auto patch7_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch7_p01, *patch7_p11, *patch7_Qu01, *patch7_Qu11);
  auto patch7_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch7_p00, *patch7_p01, *patch7_Qv00, *patch7_Qv01);

  auto patch7 = std::make_shared<PatchHermite>(
      patch7_c1, patch7_c2, patch7_c3, patch7_c4, *patch7_tw00, *patch7_tw10,
      *patch7_tw01, *patch7_tw11);

  geometry->InsertPatch(patch7);
  // geo->InsertCurve ( patch7_c1 );
  // geo->InsertCurve ( patch7_c2 );
  geometry->InsertCurve(patch7_c3);
  geometry->InsertCurve(patch7_c4);
  //======================== FIM DO PATCH 7 ===================================

  //=============================== PATCH 9 ===================================
  // Ponto* patch9_p00 = patch6_p01;
  auto patch9_p10 = patch6_p11;
  // Ponto* patch9_p01 = patch8_p11;
  auto patch9_p11 = patch10_p11;

  // auto patch9_Qu00 = std::make_shared<VectorAdaptive> ( *patch8_Qu10 );
  // auto patch9_Qu10 = std::make_shared<VectorAdaptive> ( *patch9_p00,
  // *patch9_p10
  // ); auto patch9_Qu01 = std::make_shared<VectorAdaptive> ( *patch9_p01,
  // *patch9_p11 ); auto patch9_Qu11 = std::make_shared<VectorAdaptive> (
  // *patch9_p01, *patch9_p11 );

  // auto patch9_Qv00 = std::make_shared<VectorAdaptive> ( *patch9_p00,
  // *patch9_p01
  // );
  auto patch9_Qv10 = std::make_shared<VectorAdaptive>(
      -patch7_Qv00->GetX(), patch7_Qv00->GetY(), patch7_Qv00->GetZ());
  // auto patch9_Qv01 = std::make_shared<VectorAdaptive> ( *patch9_p00,
  // *patch9_p01
  // );
  auto patch9_Qv11 = std::make_shared<VectorAdaptive>(
      -patch9_Qv10->GetX(), patch9_Qv10->GetY(), -patch9_Qv10->GetZ());

  auto patch9_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch9_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch9_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch9_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch9_c1 = patch6_c3;
  auto patch9_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch9_p10, *patch9_p11, *patch9_Qv10, *patch9_Qv11);
  auto patch9_c3 = patch10_c2;
  auto patch9_c4 = patch8_c2;

  auto patch9 = std::make_shared<PatchHermite>(
      patch9_c1, patch9_c2, patch9_c3, patch9_c4, *patch9_tw00, *patch9_tw10,
      *patch9_tw01, *patch9_tw11);

  geometry->InsertPatch(patch9);
  // geo->InsertCurve ( patch9_c1 );
  geometry->InsertCurve(patch9_c2);
  // geo->InsertCurve ( patch9_c3 );
  // geo->InsertCurve ( patch9_c4 );
  //======================== FIM DO PATCH 9 ===================================

  //=============================== PATCH 11 ==================================
  auto patch11_p00 = patch1_p00;
  // Ponto* patch11_p10 = patch1_p01;
  auto patch11_p01 = std::make_shared<VertexAdaptive>(-8.0, 5.0, 0.0);
  auto patch11_p11 = patch4_p01;

  // auto patch11_Qu00 = std::make_shared<VectorAdaptive> ( *patch11_p00,
  // *patch11_p10 ); auto patch11_Qu10 = std::make_shared<VectorAdaptive> (
  // *patch11_p00, *patch11_p10 );
  auto patch11_Qu01 =
      std::make_shared<VectorAdaptive>(*patch11_p01, *patch11_p11);
  auto patch11_Qu11 =
      std::make_shared<VectorAdaptive>(*patch11_p01, *patch11_p11);

  auto patch11_Qv00 =
      std::make_shared<VectorAdaptive>(*patch11_p00, *patch11_p01);
  // auto patch11_Qv10 = std::make_shared<VectorAdaptive> ( *patch11_p10,
  // *patch11_p11 );
  auto patch11_Qv01 =
      std::make_shared<VectorAdaptive>(*patch11_p00, *patch11_p01);
  // auto patch11_Qv11 = std::make_shared<VectorAdaptive> ( *patch11_p10,
  // *patch11_p11 );

  auto patch11_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch11_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch11_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch11_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch11_c1 = patch1_c4;
  auto patch11_c2 = patch4_c4;
  auto patch11_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch11_p01, *patch11_p11, *patch11_Qu01, *patch11_Qu11);
  auto patch11_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch11_p00, *patch11_p01, *patch11_Qv00, *patch11_Qv01);

  auto patch11 = std::make_shared<PatchHermite>(
      patch11_c1, patch11_c2, patch11_c3, patch11_c4, *patch11_tw00,
      *patch11_tw10, *patch11_tw01, *patch11_tw11);

  geometry->InsertPatch(patch11);
  // geo->InsertCurve ( patch11_c1 );
  // geo->InsertCurve ( patch11_c2 );
  geometry->InsertCurve(patch11_c3);
  geometry->InsertCurve(patch11_c4);
  //======================== FIM DO PATCH 11 ==================================

  //=============================== PATCH 12 ==================================
  auto patch12_p00 = patch3_p10;
  auto patch12_p10 = std::make_shared<VertexAdaptive>(
      -patch11_p01->GetX(), patch11_p01->GetY(), patch11_p01->GetZ());
  // Ponto* patch12_p01 = patch3_p11;
  auto patch12_p11 = patch6_p11;

  auto patch12_Qu00 =
      std::make_shared<VectorAdaptive>(*patch12_p00, *patch12_p10);
  auto patch12_Qu10 =
      std::make_shared<VectorAdaptive>(*patch12_p00, *patch12_p10);
  // auto patch12_Qu01 = std::make_shared<VectorAdaptive> ( *patch12_p01,
  // *patch12_p11 ); auto patch12_Qu11 = std::make_shared<VectorAdaptive> (
  // *patch12_p01, *patch12_p11 );

  // auto patch12_Qv00 = std::make_shared<VectorAdaptive> ( *patch12_p00,
  // *patch12_p01 );
  auto patch12_Qv10 =
      std::make_shared<VectorAdaptive>(*patch12_p10, *patch12_p11);
  // auto patch12_Qv01 = std::make_shared<VectorAdaptive> ( *patch12_p00,
  // *patch12_p01 );
  auto patch12_Qv11 =
      std::make_shared<VectorAdaptive>(*patch12_p10, *patch12_p11);

  auto patch12_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch12_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch12_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch12_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch12_c1 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch12_p00, *patch12_p10, *patch12_Qu00, *patch12_Qu10);
  auto patch12_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch12_p10, *patch12_p11, *patch12_Qv10, *patch12_Qv11);
  auto patch12_c3 = patch6_c2;
  auto patch12_c4 = patch3_c2;

  auto patch12 = std::make_shared<PatchHermite>(
      patch12_c1, patch12_c2, patch12_c3, patch12_c4, *patch12_tw00,
      *patch12_tw10, *patch12_tw01, *patch12_tw11);

  geometry->InsertPatch(patch12);
  geometry->InsertCurve(patch12_c1);
  geometry->InsertCurve(patch12_c2);
  // geo->InsertCurve ( patch12_c3 );
  // geo->InsertCurve ( patch12_c4 );
  //======================== FIM DO PATCH 12 ==================================

  //=============================== PATCH 13 ==================================
  auto patch13_p00 = patch11_p01;
  // Ponto* patch13_p10 = patch11_p11;
  auto patch13_p01 = std::make_shared<VertexAdaptive>(-1.0, 21.5, 2.0);
  auto patch13_p11 = patch10_p01;

  // auto patch13_Qu00 = std::make_shared<VectorAdaptive> ( *patch13_p00,
  // *patch13_p10 ); auto patch13_Qu10 = std::make_shared<VectorAdaptive> (
  // *patch13_p00, *patch13_p10 );
  auto patch13_Qu01 =
      std::make_shared<VectorAdaptive>(*patch13_p01, *patch13_p11);
  auto patch13_Qu11 =
      std::make_shared<VectorAdaptive>(*patch13_p01, *patch13_p11);

  auto patch13_Qv00 =
      std::make_shared<VectorAdaptive>(*patch13_p00, *patch13_p01);
  // auto patch13_Qv10 = std::make_shared<VectorAdaptive> ( *patch13_p10,
  // *patch13_p11 );
  auto patch13_Qv01 =
      std::make_shared<VectorAdaptive>(*patch13_p00, *patch11_p01);
  // auto patch13_Qv11 = std::make_shared<VectorAdaptive> ( *patch13_p10,
  // *patch13_p11 );

  auto patch13_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch13_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch13_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch13_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch13_c1 = patch11_c3;
  auto patch13_c2 = patch7_c4;
  auto patch13_c3 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch13_p01, *patch13_p11, *patch13_Qu01, *patch13_Qu11);
  auto patch13_c4 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch13_p00, *patch13_p01, *patch13_Qv00, *patch13_Qv01);

  auto patch13 = std::make_shared<PatchHermite>(
      patch13_c1, patch13_c2, patch13_c3, patch13_c4, *patch13_tw00,
      *patch13_tw10, *patch13_tw01, *patch13_tw11);

  geometry->InsertPatch(patch13);
  // geo->InsertCurve ( patch13_c1 );
  // geo->InsertCurve ( patch13_c2 );
  geometry->InsertCurve(patch13_c3);
  geometry->InsertCurve(patch13_c4);
  //======================== FIM DO PATCH 13 ==================================

  //=============================== PATCH 14 ==================================
  auto patch14_p00 = patch12_p10;
  auto patch14_p10 = std::make_shared<VertexAdaptive>(
      -patch13_p01->GetX(), patch13_p01->GetY(), patch13_p01->GetZ());
  // Ponto* patch14_p01 = patch6_p11;
  auto patch14_p11 = patch10_p11;

  auto patch14_Qu00 =
      std::make_shared<VectorAdaptive>(*patch14_p00, *patch14_p10);
  auto patch14_Qu10 =
      std::make_shared<VectorAdaptive>(*patch14_p00, *patch14_p10);
  // auto patch14_Qu01 = std::make_shared<VectorAdaptive> ( *patch14_p01,
  // *patch14_p11 ); auto patch14_Qu11 = std::make_shared<VectorAdaptive> (
  // *patch14_p01, *patch14_p11 );

  // auto patch14_Qv00 = std::make_shared<VectorAdaptive> ( *patch14_p00,
  // *patch14_p01 );
  auto patch14_Qv10 =
      std::make_shared<VectorAdaptive>(*patch14_p10, *patch14_p11);
  // auto patch14_Qv01 = std::make_shared<VectorAdaptive> ( *patch14_p00,
  // *patch14_p01 );
  auto patch14_Qv11 =
      std::make_shared<VectorAdaptive>(*patch14_p10, *patch14_p11);

  auto patch14_tw00 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch14_tw10 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch14_tw01 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);
  auto patch14_tw11 = std::make_shared<VectorAdaptive>(0.0, 0.0, 0.0);

  auto patch14_c1 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch14_p00, *patch14_p10, *patch14_Qu00, *patch14_Qu10);
  auto patch14_c2 = std::make_shared<CurveAdaptiveParametricHermite>(
      *patch14_p10, *patch14_p11, *patch14_Qv10, *patch14_Qv11);
  auto patch14_c3 = patch9_c2;
  auto patch14_c4 = patch12_c2;

  auto patch14 = std::make_shared<PatchHermite>(
      patch14_c1, patch14_c2, patch14_c3, patch14_c4, *patch14_tw00,
      *patch14_tw10, *patch14_tw01, *patch14_tw11);

  geometry->InsertPatch(patch14);
  geometry->InsertCurve(patch14_c1);
  geometry->InsertCurve(patch14_c2);
  // geo->InsertCurve ( patch14_c3 );
  // geo->InsertCurve ( patch14_c4 );
  //=========================== FIM DO PATCH 14
  //==================================
  // Fim do Exemplo do Nariz
  //=======================================================================
}

void Models3d::ModelUtahteapot(std::shared_ptr<Geometry>& geometry) {
  //==============================================================================
  // Exemplo do Utahteapot
  //==============================================================================

  //=============================== PATCH 1 ==================================
  auto p100 = std::make_shared<VertexAdaptive>(1.40000, 0.00000, 2.40000);
  auto p110 = std::make_shared<VertexAdaptive>(1.40000, -0.78400, 2.40000);
  auto p120 = std::make_shared<VertexAdaptive>(0.78400, -1.40000, 2.40000);
  auto p130 = std::make_shared<VertexAdaptive>(0.00000, -1.40000, 2.40000);

  auto p101 = std::make_shared<VertexAdaptive>(1.33750, 0.00000, 2.53125);
  auto p111 = std::make_shared<VertexAdaptive>(1.33750, -0.74900, 2.53125);
  auto p121 = std::make_shared<VertexAdaptive>(0.74900, -1.33750, 2.53125);
  auto p131 = std::make_shared<VertexAdaptive>(0.00000, -1.33750, 2.53125);

  auto p102 = std::make_shared<VertexAdaptive>(1.43750, 0.00000, 2.53125);
  auto p112 = std::make_shared<VertexAdaptive>(1.43750, -0.80500, 2.53125);
  auto p122 = std::make_shared<VertexAdaptive>(0.80500, -1.43750, 2.53125);
  auto p132 = std::make_shared<VertexAdaptive>(0.00000, -1.43750, 2.53125);

  auto p103 = std::make_shared<VertexAdaptive>(1.50000, 0.00000, 2.40000);
  auto p113 = std::make_shared<VertexAdaptive>(1.50000, -0.84000, 2.40000);
  auto p123 = std::make_shared<VertexAdaptive>(0.84000, -1.50000, 2.40000);
  auto p133 = std::make_shared<VertexAdaptive>(0.00000, -1.50000, 2.40000);

  auto patch1_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p110, *p120, *p130);
  auto patch1_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p130, *p131, *p132, *p133);
  auto patch1_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p103, *p113, *p123, *p133);
  auto patch1_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p100, *p101, *p102, *p103);

  auto patch1 = std::make_shared<PatchBezier>(
      patch1_c1, patch1_c2, patch1_c3, patch1_c4, *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================

  //=============================== PATCH 2 ==================================
  //		Ponto* p200 = p130; // new Vertice (  0.00000,
  //-1.40000,  2.40000
  //);
  auto p210 = std::make_shared<VertexAdaptive>(-0.78400, -1.40000, 2.40000);
  auto p220 = std::make_shared<VertexAdaptive>(-1.40000, -0.78400, 2.40000);
  auto p230 = std::make_shared<VertexAdaptive>(-1.40000, 0.00000, 2.40000);

  //		Ponto* p201 = p131; // new Vertice (  0.00000, -1.33750, 2.53125
  //);
  auto p211 = std::make_shared<VertexAdaptive>(-0.74900, -1.33750, 2.53125);
  auto p221 = std::make_shared<VertexAdaptive>(-1.33750, -0.74900, 2.53125);
  auto p231 = std::make_shared<VertexAdaptive>(-1.33750, 0.00000, 2.53125);

  //		Ponto* p202 = p132; //new Vertice (  0.00000, -1.43750, 2.53125
  //);
  auto p212 = std::make_shared<VertexAdaptive>(-0.80500, -1.43750, 2.53125);
  auto p222 = std::make_shared<VertexAdaptive>(-1.43750, -0.80500, 2.53125);
  auto p232 = std::make_shared<VertexAdaptive>(-1.43750, 0.00000, 2.53125);

  //		Ponto* p203 = p133; //new Vertice (  0.00000, -1.50000, 2.40000
  //);
  auto p213 = std::make_shared<VertexAdaptive>(-0.84000, -1.50000, 2.40000);
  auto p223 = std::make_shared<VertexAdaptive>(-1.50000, -0.84000, 2.40000);
  auto p233 = std::make_shared<VertexAdaptive>(-1.50000, 0.00000, 2.40000);

  auto patch2_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p130, *p210, *p220, *p230);  //( *p200, *p210, *p220, *p230 );
  auto patch2_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p230, *p231, *p232, *p233);
  auto patch2_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p133, *p213, *p223, *p233);
  // Curva* patch2_c4 = patch1_c2; //new CurvParamBezier ( *p130, *p131, *p132,
  // *p133 ); //( *p200, *p201, *p202, *p203 );

  auto patch2 = std::make_shared<PatchBezier>(
      patch2_c1, patch2_c2, patch2_c3, patch1_c2, *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  geometry->InsertCurve(patch2_c1);
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  // geo->InsertCurve ( patch2_c4 );
  //======================== FIM DO PATCH 2 ==================================

  //=============================== PATCH 3 ==================================
  //		Ponto* p300 = p230; //new Vertice ( -1.40000, 0.00000, 2.40000
  //);
  auto p310 = std::make_shared<VertexAdaptive>(-1.40000, 0.78400, 2.40000);
  auto p320 = std::make_shared<VertexAdaptive>(-0.78400, 1.40000, 2.40000);
  auto p330 = std::make_shared<VertexAdaptive>(0.00000, 1.40000, 2.40000);

  //		Ponto* p301 = p231; //new Vertice ( -1.33750, 0.00000, 2.53125
  //);
  auto p311 = std::make_shared<VertexAdaptive>(-1.33750, 0.74900, 2.53125);
  auto p321 = std::make_shared<VertexAdaptive>(-0.74900, 1.33750, 2.53125);
  auto p331 = std::make_shared<VertexAdaptive>(0.00000, 1.33750, 2.53125);

  //		Ponto* p302 = p232; //new Vertice ( -1.43750, 0.00000, 2.53125
  //);
  auto p312 = std::make_shared<VertexAdaptive>(-1.43750, 0.80500, 2.53125);
  auto p322 = std::make_shared<VertexAdaptive>(-0.80500, 1.43750, 2.53125);
  auto p332 = std::make_shared<VertexAdaptive>(0.00000, 1.43750, 2.53125);

  //		Ponto* p303 = p233; //new Vertice ( -1.50000, 0.00000, 2.40000
  //);
  auto p313 = std::make_shared<VertexAdaptive>(-1.50000, 0.84000, 2.40000);
  auto p323 = std::make_shared<VertexAdaptive>(-0.84000, 1.50000, 2.40000);
  auto p333 = std::make_shared<VertexAdaptive>(0.00000, 1.50000, 2.40000);

  auto patch3_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p230, *p310, *p320, *p330);
  auto patch3_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p330, *p331, *p332, *p333);
  auto patch3_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p233, *p313, *p323, *p333);
  //		Curva* patch3_c4 = new CurvParamBezier ( *p230, *p231, *p232,
  //*p233
  //);

  auto patch3 = std::make_shared<PatchBezier>(
      patch3_c1, patch3_c2, patch3_c3, patch2_c2, *p311, *p321, *p312, *p322);

  geometry->InsertPatch(patch3);
  geometry->InsertCurve(patch3_c1);
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  //		geo->InsertCurve ( patch3_c4 );
  //======================== FIM DO PATCH 3 ==================================

  //=============================== PATCH 4 ==================================
  // Ponto* p400 = p330; //new Vertice ( 0.00000, 1.40000, 2.40000 );
  auto p410 = std::make_shared<VertexAdaptive>(0.78400, 1.40000, 2.40000);
  auto p420 = std::make_shared<VertexAdaptive>(1.40000, 0.78400, 2.40000);
  // Ponto* p430 = p100; // new Vertice ( 1.40000, 0.00000, 2.40000 );

  // Ponto* p401 = p331; //new Vertice (	0.00000, 1.33750, 2.53125 );
  auto p411 = std::make_shared<VertexAdaptive>(0.74900, 1.33750, 2.53125);
  auto p421 = std::make_shared<VertexAdaptive>(1.33750, 0.74900, 2.53125);
  // Ponto* p431 = p101; // new Vertice ( 1.33750, 0.00000, 2.53125 );

  // Ponto* p402 = p332; //new Vertice (	0.00000, 1.43750, 2.53125 );
  auto p412 = std::make_shared<VertexAdaptive>(0.80500, 1.43750, 2.53125);
  auto p422 = std::make_shared<VertexAdaptive>(1.43750, 0.80500, 2.53125);
  // Ponto* p432 = p102; //new Vertice ( 1.43750, 0.00000, 2.53125 );

  // Ponto* p403 = p333; //new Vertice (	0.00000, 1.50000, 2.40000 );
  auto p413 = std::make_shared<VertexAdaptive>(0.84000, 1.50000, 2.40000);
  auto p423 = std::make_shared<VertexAdaptive>(1.50000, 0.84000, 2.40000);
  // Ponto* p433 = p103; //new Vertice ( 1.50000, 0.00000, 2.40000 );

  auto patch4_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p330, *p410, *p420, *p100);  // ( *p400, *p410, *p420, *p430 );
  // Curva* patch4_c2 = new CurvParamBezier ( *p100, *p101, *p102, *p103 ); // (
  // *p430, *p431, *p432, *p433 );
  auto patch4_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p333, *p413, *p423, *p103);  // ( *p403, *p413, *p423, *p433 );
  // Curva* patch4_c4 = new CurvParamBezier ( *p330, *p331, *p332, *p333 ); // (
  // *p400, *p401, *p402, *p403 );

  auto patch4 = std::make_shared<PatchBezier>(
      patch4_c1, patch1_c4, patch4_c3, patch3_c2, *p411, *p421, *p412, *p422);

  geometry->InsertPatch(patch4);
  geometry->InsertCurve(patch4_c1);
  // geo->InsertCurve ( patch4_c2 );
  geometry->InsertCurve(patch4_c3);
  // geo->InsertCurve ( patch4_c4 );
  //======================== FIM DO PATCH 4 ==================================

  //=============================== PATCH 5 ==================================
  //		Ponto* p500 = p103; //new Vertice (	1.50000,
  // 0.00000, 2.40000
  //); 		Ponto* p510 = p113; //new Vertice ( 1.50000, -0.84000, 2.40000
  //); Ponto* p520 = p123; //new Vertice ( 0.84000, -1.50000, 2.40000 );
  // Ponto* p530 = p133; //new Vertice ( 0.00000, -1.50000, 2.40000 );

  auto p501 = std::make_shared<VertexAdaptive>(1.75000, 0.00000, 1.87500);
  auto p511 = std::make_shared<VertexAdaptive>(1.75000, -0.98000, 1.87500);
  auto p521 = std::make_shared<VertexAdaptive>(0.98000, -1.75000, 1.87500);
  auto p531 = std::make_shared<VertexAdaptive>(0.00000, -1.75000, 1.87500);

  auto p502 = std::make_shared<VertexAdaptive>(2.00000, 0.00000, 1.35000);
  auto p512 = std::make_shared<VertexAdaptive>(2.00000, -1.12000, 1.35000);
  auto p522 = std::make_shared<VertexAdaptive>(1.12000, -2.00000, 1.35000);
  auto p532 = std::make_shared<VertexAdaptive>(0.00000, -2.00000, 1.35000);

  auto p503 = std::make_shared<VertexAdaptive>(2.00000, 0.00000, 0.90000);
  auto p513 = std::make_shared<VertexAdaptive>(2.00000, -1.12000, 0.90000);
  auto p523 = std::make_shared<VertexAdaptive>(1.12000, -2.00000, 0.90000);
  auto p533 = std::make_shared<VertexAdaptive>(0.00000, -2.00000, 0.90000);

  // Curva* patch5_c1 = patch1_c3; //new CurvParamBezier ( *p103, *p113, *p123,
  // *p133 ); // ( *p500, *p510, *p520, *p530 );
  auto patch5_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p133, *p531, *p532, *p533);  // ( *p530, *p531, *p532, *p533 );
  auto patch5_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p503, *p513, *p523, *p533);  // ( *p503, *p513, *p523, *p533 );
  auto patch5_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p103, *p501, *p502, *p503);  // ( *p500, *p501, *p502, *p503 );

  auto patch5 = std::make_shared<PatchBezier>(
      patch1_c3, patch5_c2, patch5_c3, patch5_c4, *p511, *p521, *p512, *p522);

  geometry->InsertPatch(patch5);
  // geo->InsertCurve ( patch5_c1 );
  geometry->InsertCurve(patch5_c2);
  geometry->InsertCurve(patch5_c3);
  geometry->InsertCurve(patch5_c4);
  //======================== FIM DO PATCH 5 ==================================

  //=============================== PATCH 6 ==================================
  // Ponto* p600 = p133; //new Vertice (	0.00000, -1.50000, 2.40000 );
  //		Ponto* p610 = p213; //new Vertice ( -0.84000, -1.50000, 2.40000
  //); 		Ponto* p620 = p223; //new Vertice ( -1.50000, -0.84000, 2.40000
  //); 		Ponto* p630 = p303; //new Vertice ( -1.50000, 0.00000, 2.40000
  // );

  //		Ponto* p601 = p531; //new Vertice (	0.00000,
  //-1.75000, 1.87500
  //);
  auto p611 = std::make_shared<VertexAdaptive>(-0.98000, -1.75000, 1.87500);
  auto p621 = std::make_shared<VertexAdaptive>(-1.75000, -0.98000, 1.87500);
  auto p631 = std::make_shared<VertexAdaptive>(-1.75000, 0.00000, 1.87500);

  //		Ponto* p602 = p532; //new Vertice (	0.00000,
  //-2.00000, 1.35000
  //);
  auto p612 = std::make_shared<VertexAdaptive>(-1.12000, -2.00000, 1.35000);
  auto p622 = std::make_shared<VertexAdaptive>(-2.00000, -1.12000, 1.35000);
  auto p632 = std::make_shared<VertexAdaptive>(-2.00000, 0.00000, 1.35000);

  //		Ponto* p603 = p533; //new Vertice (	0.00000, -2.00000,
  // 0.90000
  //);
  auto p613 = std::make_shared<VertexAdaptive>(-1.12000, -2.00000, 0.90000);
  auto p623 = std::make_shared<VertexAdaptive>(-2.00000, -1.12000, 0.90000);
  auto p633 = std::make_shared<VertexAdaptive>(-2.00000, 0.00000, 0.90000);

  //		Curva* patch6_c1 = new CurvParamBezier ( *p133, *p213, *p223,
  //*p233
  //); // ( *p600, *p610, *p620, *p630 );
  auto patch6_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p233, *p631, *p632, *p633);  // ( *p630, *p631, *p632, *p633 );
  auto patch6_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p533, *p613, *p623, *p633);  // ( *p603, *p613, *p623, *p633 );
  //		Curva* patch6_c4 = new CurvParamBezier ( *p133, *p531, *p532,
  //*p533
  //); // ( *p600, *p601, *p602, *p603 );

  auto patch6 = std::make_shared<PatchBezier>(
      patch2_c3, patch6_c2, patch6_c3, patch5_c2, *p611, *p621, *p612, *p622);

  geometry->InsertPatch(patch6);
  //		geo->InsertCurve ( patch6_c1 );
  geometry->InsertCurve(patch6_c2);
  geometry->InsertCurve(patch6_c3);
  //		geo->InsertCurve ( patch6_c4 );
  //======================== FIM DO PATCH 6 ==================================

  //=============================== PATCH 7 ==================================
  //		Ponto* p700 = p233; //new Vertice (	-1.50000,
  // 0.00000, 2.40000
  //); 		Ponto* p710 = p313; //new Vertice ( -1.50000, 0.84000, 2.40000
  //); Ponto* p720 = p323; //new Vertice ( -0.84000, 1.50000, 2.40000 );
  // Ponto* p730 = p333; //new Vertice ( 0.00000, 1.50000, 2.40000 );

  //		Ponto* p701 = p631; //new Vertice (-1.75000, 0.00000, 1.87500 );
  auto p711 = std::make_shared<VertexAdaptive>(-1.75000, 0.98000, 1.87500);
  auto p721 = std::make_shared<VertexAdaptive>(-0.98000, 1.75000, 1.87500);
  auto p731 = std::make_shared<VertexAdaptive>(0.00000, 1.75000, 1.87500);

  //		Ponto* p702 = p632; //new Vertice (-2.00000, 0.00000, 1.35000 );
  auto p712 = std::make_shared<VertexAdaptive>(-2.00000, 1.12000, 1.35000);
  auto p722 = std::make_shared<VertexAdaptive>(-1.12000, 2.00000, 1.35000);
  auto p732 = std::make_shared<VertexAdaptive>(0.00000, 2.00000, 1.35000);

  //		Ponto* p703 = p633; //new Vertice (-2.00000, 0.00000, 0.90000 );
  auto p713 = std::make_shared<VertexAdaptive>(-2.00000, 1.12000, 0.90000);
  auto p723 = std::make_shared<VertexAdaptive>(-1.12000, 2.00000, 0.90000);
  auto p733 = std::make_shared<VertexAdaptive>(0.00000, 2.00000, 0.90000);

  //		Curva* patch7_c1 = new CurvParamBezier ( *p233, *p313, *p323,
  //*p333
  //);
  auto patch7_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p333, *p731, *p732, *p733);
  auto patch7_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p633, *p713, *p723, *p733);
  //		Curva* patch7_c4 = new CurvParamBezier ( *p233, *p631, *p632,
  //*p633
  //);

  auto patch7 = std::make_shared<PatchBezier>(
      patch3_c3, patch7_c2, patch7_c3, patch6_c2, *p711, *p721, *p712, *p722);

  geometry->InsertPatch(patch7);
  //		geo->InsertCurve ( patch7_c1 );
  geometry->InsertCurve(patch7_c2);
  geometry->InsertCurve(patch7_c3);
  //		geo->InsertCurve ( patch7_c4 );
  //======================== FIM DO PATCH 7 ==================================

  //=============================== PATCH 8 ==================================
  //		Ponto* p800 = p333; //new Vertice (
  // 0.00000, 1.50000, 2.40000
  //); 		Ponto* p810 = p413; //new Vertice ( 0.84000, 1.50000, 2.40000 );
  // Ponto* p820 = p423; //new Vertice ( 1.50000, 0.84000, 2.40000 );
  // Ponto* p830 = p103; //new Vertice ( 1.50000, 0.00000, 2.40000 );

  //		Ponto* p801 = p731; //new Vertice (
  // 0.00000, 1.75000, 1.87500
  //);
  auto p811 = std::make_shared<VertexAdaptive>(0.98000, 1.75000, 1.87500);
  auto p821 = std::make_shared<VertexAdaptive>(1.75000, 0.98000, 1.87500);
  //		Ponto* p831 = p501; //new Vertice ( 1.75000, 0.00000, 1.87500 );

  //		Ponto* p802 = p732; //new Vertice (
  // 0.00000, 2.00000, 1.35000
  //);
  auto p812 = std::make_shared<VertexAdaptive>(1.12000, 2.00000, 1.35000);
  auto p822 = std::make_shared<VertexAdaptive>(2.00000, 1.12000, 1.35000);
  //		Ponto* p832 = p502; //new Vertice ( 2.00000, 0.00000, 1.35000 );

  //		Ponto* p803 = p733; //new Vertice (	0.00000, 2.00000,
  // 0.90000
  //);
  auto p813 = std::make_shared<VertexAdaptive>(1.12000, 2.00000, 0.90000);
  auto p823 = std::make_shared<VertexAdaptive>(2.00000, 1.12000, 0.90000);
  //		Ponto* p833 = p503; //new Vertice ( 2.00000, 0.00000, 0.90000 );

  //		Curva* patch8_c1 = new CurvParamBezier ( *p333, *p413, *p423,
  //*p103
  //); 		Curva* patch8_c2 = new CurvParamBezier ( *p103, *p501, *p502,
  //*p503 );
  auto patch8_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p733, *p813, *p823, *p503);
  //		Curva* patch8_c4 = new CurvParamBezier ( *p333, *p731, *p732,
  //*p733
  //);

  auto patch8 = std::make_shared<PatchBezier>(
      patch4_c3, patch5_c4, patch8_c3, patch7_c2, *p811, *p821, *p812, *p822);

  geometry->InsertPatch(patch8);
  //		geo->InsertCurve ( patch8_c1 );
  //		geo->InsertCurve ( patch8_c2 );
  geometry->InsertCurve(patch8_c3);
  //		geo->InsertCurve ( patch8_c4 );
  //======================== FIM DO PATCH 8 ==================================

  //=============================== PATCH 9 ==================================
  //		Ponto* p900 = p503; //new Vertice (	2.00000, 0.00000,
  // 0.90000
  //); 		Ponto* p910 = p513; //new Vertice ( 2.00000, -1.12000, 0.90000
  //); Ponto* p920 = p523; //new Vertice ( 1.12000, -2.00000, 0.90000 );
  // Ponto* p930 = p533; //new Vertice ( 0.00000, -2.00000, 0.90000 );

  auto p901 = std::make_shared<VertexAdaptive>(2.00000, 0.00000, 0.45000);
  auto p911 = std::make_shared<VertexAdaptive>(2.00000, -1.12000, 0.45000);
  auto p921 = std::make_shared<VertexAdaptive>(1.12000, -2.00000, 0.45000);
  auto p931 = std::make_shared<VertexAdaptive>(0.00000, -2.00000, 0.45000);

  auto p902 = std::make_shared<VertexAdaptive>(1.50000, 0.00000, 0.22500);
  auto p912 = std::make_shared<VertexAdaptive>(1.50000, -0.84000, 0.22500);
  auto p922 = std::make_shared<VertexAdaptive>(0.84000, -1.50000, 0.22500);
  auto p932 = std::make_shared<VertexAdaptive>(0.00000, -1.50000, 0.22500);

  auto p903 = std::make_shared<VertexAdaptive>(1.50000, 0.00000, 0.15000);
  auto p913 = std::make_shared<VertexAdaptive>(1.50000, -0.84000, 0.15000);
  auto p923 = std::make_shared<VertexAdaptive>(0.84000, -1.50000, 0.15000);
  auto p933 = std::make_shared<VertexAdaptive>(0.00000, -1.50000, 0.15000);

  //		Curva* patch9_c1 = new CurvParamBezier ( *p503, *p513, *p523,
  //*p533
  //);
  auto patch9_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p533, *p931, *p932, *p933);
  auto patch9_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p903, *p913, *p923, *p933);
  auto patch9_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p503, *p901, *p902, *p903);

  auto patch9 = std::make_shared<PatchBezier>(
      patch5_c3, patch9_c2, patch9_c3, patch9_c4, *p911, *p921, *p912, *p922);

  geometry->InsertPatch(patch9);
  //		geo->InsertCurve ( patch9_c1 );
  geometry->InsertCurve(patch9_c2);
  geometry->InsertCurve(patch9_c3);
  geometry->InsertCurve(patch9_c4);
  //======================== FIM DO PATCH 9 ==================================

  //============================== PATCH 10 ==================================
  //		Ponto* p1000 = p533; //new Vertice ( 0.00000,-2.00000, 0.90000
  //); 		Ponto* p1010 = p613; //new Vertice (-1.12000,-2.00000, 0.90000
  //); Ponto* p1020 = p623; //new Vertice (-2.00000,-1.12000, 0.90000 );
  // Ponto* p1030 = p633; //new Vertice ( -2.00000, 0.00000, 0.90000 );

  //		Ponto* p1001 = p931; //new Vertice ( 0.00000,-2.00000, 0.45000
  //);
  auto p1011 = std::make_shared<VertexAdaptive>(-1.12000, -2.00000, 0.45000);
  auto p1021 = std::make_shared<VertexAdaptive>(-2.00000, -1.12000, 0.45000);
  auto p1031 = std::make_shared<VertexAdaptive>(-2.00000, 0.00000, 0.45000);

  //		Ponto* p1002 = p932; //new Vertice ( 0.00000,-1.50000, 0.22500
  //);
  auto p1012 = std::make_shared<VertexAdaptive>(-0.84000, -1.50000, 0.22500);
  auto p1022 = std::make_shared<VertexAdaptive>(-1.50000, -0.84000, 0.22500);
  auto p1032 = std::make_shared<VertexAdaptive>(-1.50000, 0.00000, 0.22500);

  //		Ponto* p1003 = p933; //new Vertice ( 0.00000,-1.50000, 0.15000
  //);
  auto p1013 = std::make_shared<VertexAdaptive>(-0.84000, -1.50000, 0.15000);
  auto p1023 = std::make_shared<VertexAdaptive>(-1.50000, -0.84000, 0.15000);
  auto p1033 = std::make_shared<VertexAdaptive>(-1.50000, 0.00000, 0.15000);

  //		Curva* patch10_c1 = new CurvParamBezier ( *p533, *p613, *p623,
  //*p633
  //);
  auto patch10_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p633, *p1031, *p1032, *p1033);
  auto patch10_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p933, *p1013, *p1023, *p1033);
  //		Curva* patch10_c4 = new CurvParamBezier ( *p533, *p931, *p932,
  //*p933
  //);

  auto patch10 =
      std::make_shared<PatchBezier>(patch6_c3, patch10_c2, patch10_c3,
                                    patch9_c2, *p1011, *p1021, *p1012, *p1022);

  geometry->InsertPatch(patch10);
  //		geo->InsertCurve ( patch10_c1 );
  geometry->InsertCurve(patch10_c2);
  geometry->InsertCurve(patch10_c3);
  //		geo->InsertCurve ( patch10_c4 );
  //======================== FIM DO PATCH 10 =================================

  //============================== PATCH 11 ==================================
  //		Ponto* p633 = p633; //new Vertice ( -2.00000, 0.00000, 0.90000
  //); 		Ponto* p713 = p713; //new Vertice ( -2.00000, 1.12000, 0.90000
  //); Ponto* p723 = p723; //new Vertice ( -1.12000, 2.00000, 0.90000 );
  // Ponto* p733 = p733; //new Vertice ( 0.00000, 2.00000, 0.90000 );

  //		Ponto* p1031 = p1031; //new Vertice (-2.00000, 0.00000, 0.45000
  //);
  auto p1111 = std::make_shared<VertexAdaptive>(-2.00000, 1.12000, 0.45000);
  auto p1121 = std::make_shared<VertexAdaptive>(-1.12000, 2.00000, 0.45000);
  auto p1131 = std::make_shared<VertexAdaptive>(0.00000, 2.00000, 0.45000);

  //		Ponto* p1032 = p1032; //new Vertice (-1.50000, 0.00000, 0.22500
  //);
  auto p1112 = std::make_shared<VertexAdaptive>(-1.50000, 0.84000, 0.22500);
  auto p1122 = std::make_shared<VertexAdaptive>(-0.84000, 1.50000, 0.22500);
  auto p1132 = std::make_shared<VertexAdaptive>(0.00000, 1.50000, 0.22500);

  //		Ponto* p1033 = p1033; //new Vertice (-1.50000, 0.00000, 0.15000
  //);
  auto p1113 = std::make_shared<VertexAdaptive>(-1.50000, 0.84000, 0.15000);
  auto p1123 = std::make_shared<VertexAdaptive>(-0.84000, 1.50000, 0.15000);
  auto p1133 = std::make_shared<VertexAdaptive>(0.00000, 1.50000, 0.15000);

  //		Curva* patch11_c1 = new CurvParamBezier ( *p633, *p713, *p723,
  //*p733
  //);
  auto patch11_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p733, *p1131, *p1132, *p1133);
  auto patch11_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1033, *p1113, *p1123, *p1133);
  //		Curva* patch11_c4 = new CurvParamBezier ( *p633, *p1031, *p1032,
  //*p1033 );

  auto patch11 =
      std::make_shared<PatchBezier>(patch7_c3, patch11_c2, patch11_c3,
                                    patch10_c2, *p1111, *p1121, *p1112, *p1122);

  geometry->InsertPatch(patch11);
  //		geo->InsertCurve ( patch11_c1 );
  geometry->InsertCurve(patch11_c2);
  geometry->InsertCurve(patch11_c3);
  //		geo->InsertCurve ( patch11_c4 );
  //======================== FIM DO PATCH 11 =================================

  //============================== PATCH 12 ==================================
  //		Ponto* p733 = p733; //new Vertice ( 0.00000, 2.00000, 0.90000 );
  //		Ponto* p813 = p813; //new Vertice ( 1.12000, 2.00000, 0.90000 );
  //		Ponto* p823 = p823; //new Vertice ( 2.00000, 1.12000, 0.90000 );
  //		Ponto* p503 = p503; // new Vertice ( 2.00000, 0.00000, 0.90000
  //);

  //		Ponto* p1131 = p1131; //new Vertice ( 0.00000, 2.00000, 0.45000
  //);
  auto p1211 = std::make_shared<VertexAdaptive>(1.12000, 2.00000, 0.45000);
  auto p1221 = std::make_shared<VertexAdaptive>(2.00000, 1.20000, 0.45000);
  //		Ponto* p901 = p901; //new Vertice ( 2.00000, 0.00000, 0.45000 );

  //		Ponto* p1132 = p1132; //new Vertice ( 0.00000, 1.50000, 0.22500
  //);
  auto p1212 = std::make_shared<VertexAdaptive>(0.84000, 1.50000, 0.22500);
  auto p1222 = std::make_shared<VertexAdaptive>(1.50000, 0.84000, 0.22500);
  //		Ponto* p902 = p902; //new Vertice ( 1.50000, 0.00000, 0.22500 );

  //		Ponto* p1133 = p1133; //new Vertice ( 0.00000, 1.50000, 0.15000
  //);
  auto p1213 = std::make_shared<VertexAdaptive>(0.84000, 1.50000, 0.15000);
  auto p1223 = std::make_shared<VertexAdaptive>(1.50000, 0.84000, 0.15000);
  //		Ponto* p903 = p903; //new Vertice ( 1.50000, 0.00000, 0.15000 );

  //		Curva* patch12_c1 = new CurvParamBezier ( *p733, *p813, *p823,
  //*p503
  //); 		Curva* patch12_c2 = new CurvParamBezier ( *p503, *p901, *p902,
  //*p903 );
  auto patch12_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1133, *p1213, *p1223, *p903);
  //		Curva* patch12_c4 = new CurvParamBezier ( *p733, *p1131, *p1132,
  //*p1133 );

  auto patch12 =
      std::make_shared<PatchBezier>(patch8_c3, patch9_c4, patch12_c3,
                                    patch11_c2, *p1211, *p1221, *p1212, *p1222);

  geometry->InsertPatch(patch12);
  //		geo->InsertCurve ( patch12_c1 );
  //		geo->InsertCurve ( patch12_c2 );
  geometry->InsertCurve(patch12_c3);
  //		geo->InsertCurve ( patch12_c4 );
  //======================== FIM DO PATCH 12 =================================

  //============================== PATCH 13 ==================================
  auto p1300 = std::make_shared<VertexAdaptive>(-1.60000, 0.00000, 2.02500);
  auto p1310 = std::make_shared<VertexAdaptive>(-1.60000, -0.30000, 2.02500);
  auto p1320 = std::make_shared<VertexAdaptive>(-1.50000, -0.30000, 2.25000);
  auto p1330 = std::make_shared<VertexAdaptive>(-1.50000, 0.00000, 2.25000);

  auto p1301 = std::make_shared<VertexAdaptive>(-2.30000, 0.00000, 2.02500);
  auto p1311 = std::make_shared<VertexAdaptive>(-2.30000, -0.30000, 2.02500);
  auto p1321 = std::make_shared<VertexAdaptive>(-2.50000, -0.30000, 2.25000);
  auto p1331 = std::make_shared<VertexAdaptive>(-2.50000, 0.00000, 2.25000);

  auto p1302 = std::make_shared<VertexAdaptive>(-2.70000, 0.00000, 2.02500);
  auto p1312 = std::make_shared<VertexAdaptive>(-2.70000, -0.30000, 2.02500);
  auto p1322 = std::make_shared<VertexAdaptive>(-3.00000, -0.30000, 2.25000);
  auto p1332 = std::make_shared<VertexAdaptive>(-3.00000, 0.00000, 2.25000);

  auto p1303 = std::make_shared<VertexAdaptive>(-2.70000, 0.00000, 1.80000);
  auto p1313 = std::make_shared<VertexAdaptive>(-2.70000, -0.30000, 1.80000);
  auto p1323 = std::make_shared<VertexAdaptive>(-3.00000, -0.30000, 1.80000);
  auto p1333 = std::make_shared<VertexAdaptive>(-3.00000, 0.00000, 1.80000);

  auto patch13_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1300, *p1310, *p1320, *p1330);
  auto patch13_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1330, *p1331, *p1332, *p1333);
  auto patch13_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1303, *p1313, *p1323, *p1333);
  auto patch13_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1300, *p1301, *p1302, *p1303);

  auto patch13 =
      std::make_shared<PatchBezier>(patch13_c1, patch13_c2, patch13_c3,
                                    patch13_c4, *p1311, *p1321, *p1312, *p1322);

  geometry->InsertPatch(patch13);
  geometry->InsertCurve(patch13_c1);
  geometry->InsertCurve(patch13_c2);
  geometry->InsertCurve(patch13_c3);
  geometry->InsertCurve(patch13_c4);
  //======================== FIM DO PATCH 13 =================================

  //============================== PATCH 14 ==================================
  //		Ponto* p1330 = p1330; //new Vertice ( -1.50000, 0.00000, 2.25000
  //);
  auto p1410 = std::make_shared<VertexAdaptive>(-1.50000, 0.30000, 2.25000);
  auto p1420 = std::make_shared<VertexAdaptive>(-1.60000, 0.30000, 2.02500);
  //		Ponto* p1300 = p1300; //new Vertice ( -1.60000, 0.00000, 2.02500
  //);

  //		Ponto* p1331 = p1331; //new Vertice ( -2.50000, 0.00000, 2.25000
  //);
  auto p1411 = std::make_shared<VertexAdaptive>(-2.50000, 0.30000, 2.25000);
  auto p1421 = std::make_shared<VertexAdaptive>(-2.30000, 0.30000, 2.02500);
  //		Ponto* p1301 = p1301; //new Vertice ( -2.30000, 0.00000, 2.02500
  //);

  //		Ponto* p1332 = p1332; //new Vertice ( -3.00000, 0.00000, 2.25000
  //);
  auto p1412 = std::make_shared<VertexAdaptive>(-3.00000, 0.30000, 2.25000);
  auto p1422 = std::make_shared<VertexAdaptive>(-2.70000, 0.30000, 2.02500);
  //		Ponto* p1302 = p1302; //new Vertice ( -2.70000, 0.00000, 2.02500
  //);

  //		Ponto* p1333 = p1333; //new Vertice ( -3.00000, 0.00000, 1.80000
  //);
  auto p1413 = std::make_shared<VertexAdaptive>(-3.00000, 0.30000, 1.80000);
  auto p1423 = std::make_shared<VertexAdaptive>(-2.70000, 0.30000, 1.80000);
  //		Ponto* p1303 = p1303; //new Vertice ( -2.70000, 0.00000, 1.80000
  //);

  auto patch14_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1330, *p1410, *p1420, *p1300);
  //		Curva* patch14_c2 = new CurvParamBezier ( *p1300, *p1301,
  //*p1302, *p1303 );
  auto patch14_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1333, *p1413, *p1423, *p1303);
  //		Curva* patch14_c4 = new CurvParamBezier ( *p1330, *p1331,
  //*p1332, *p1333 );

  auto patch14 =
      std::make_shared<PatchBezier>(patch14_c1, patch13_c4, patch14_c3,
                                    patch13_c2, *p1411, *p1421, *p1412, *p1422);

  geometry->InsertPatch(patch14);
  geometry->InsertCurve(patch14_c1);
  //		geo->InsertCurve ( patch14_c2 );
  geometry->InsertCurve(patch14_c3);
  //		geo->InsertCurve ( patch14_c4 );
  //======================== FIM DO PATCH 14 =================================

  //============================== PATCH 15 ==================================
  //		Ponto* p1303 = p1303; //new Vertice ( -2.70000, 0.00000, 1.80000
  //); 		Ponto* p1313 = p1313; //new Vertice ( -2.70000,-0.30000, 1.80000
  //); 		Ponto* p1323 = p1323; //new Vertice ( -3.00000,-0.30000, 1.80000
  //); 		Ponto* p1333 = p1333; //new Vertice ( -3.00000, 0.00000, 1.80000
  //);

  auto p1501 = std::make_shared<VertexAdaptive>(-2.70000, 0.00000, 1.57500);
  auto p1511 = std::make_shared<VertexAdaptive>(-2.70000, -0.30000, 1.57500);
  auto p1521 = std::make_shared<VertexAdaptive>(-3.00000, -0.30000, 1.35000);
  auto p1531 = std::make_shared<VertexAdaptive>(-3.00000, 0.00000, 1.35000);

  auto p1502 = std::make_shared<VertexAdaptive>(-2.50000, 0.00000, 1.12500);
  auto p1512 = std::make_shared<VertexAdaptive>(-2.50000, -0.30000, 1.12500);
  auto p1522 = std::make_shared<VertexAdaptive>(-2.65000, -0.30000, 0.93750);
  auto p1532 = std::make_shared<VertexAdaptive>(-2.65000, 0.00000, 0.93750);

  //		Ponto* p633 = p633; //new Vertice ( -2.00000, 0.00000, 0.90000
  //);
  auto p1513 = std::make_shared<VertexAdaptive>(-2.00000, -0.30000, 0.90000);
  auto p1523 = std::make_shared<VertexAdaptive>(-1.90000, -0.30000, 0.60000);
  auto p1533 = std::make_shared<VertexAdaptive>(-1.90000, 0.00000, 0.60000);

  //		Curva* patch15_c1 = new CurvParamBezier ( *p1303, *p1313,
  //*p1323, *p1333 );
  auto patch15_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1333, *p1531, *p1532, *p1533);
  auto patch15_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p633, *p1513, *p1523, *p1533);
  auto patch15_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1303, *p1501, *p1502, *p633);

  auto patch15 =
      std::make_shared<PatchBezier>(patch13_c3, patch15_c2, patch15_c3,
                                    patch15_c4, *p1511, *p1521, *p1512, *p1522);

  geometry->InsertPatch(patch15);
  //		geo->InsertCurve ( patch15_c1 );
  geometry->InsertCurve(patch15_c2);
  geometry->InsertCurve(patch15_c3);
  geometry->InsertCurve(patch15_c4);
  //======================== FIM DO PATCH 15 =================================

  //============================== PATCH 16 ==================================
  //		Ponto* p1333 = p1333; //new Vertice ( -3.00000, 0.00000, 1.80000
  //); 		Ponto* p1413 = p1413; //new Vertice ( -3.00000, 0.30000, 1.80000
  //); 		Ponto* p1423 = p1423; //new Vertice ( -2.70000, 0.30000, 1.80000
  //); 		Ponto* p1303 = p1303; //new Vertice ( -2.70000, 0.00000, 1.80000
  //);

  //		Ponto* p1531 = p1531; //new Vertice ( -3.00000, 0.00000, 1.35000
  //);
  auto p1611 = std::make_shared<VertexAdaptive>(-3.00000, 0.30000, 1.35000);
  auto p1621 = std::make_shared<VertexAdaptive>(-2.70000, 0.30000, 1.57500);
  //		Ponto* p1501 = p1501; //new Vertice ( -2.70000, 0.00000, 1.57500
  //);

  //		Ponto* p1532 = p1532; //new Vertice ( -2.65000, 0.00000, 0.93750
  //);
  auto p1612 = std::make_shared<VertexAdaptive>(-2.65000, 0.30000, 0.93750);
  auto p1622 = std::make_shared<VertexAdaptive>(-2.50000, 0.30000, 1.12500);
  //		Ponto* p1502 = p1502; //new Vertice ( -2.50000, 0.00000, 1.12500
  //);

  //		Ponto* p1533 = p1533; //new Vertice ( -1.90000, 0.00000, 0.60000
  //);
  auto p1613 = std::make_shared<VertexAdaptive>(-1.90000, 0.30000, 0.60000);
  auto p1623 = std::make_shared<VertexAdaptive>(-2.00000, 0.30000, 0.90000);
  //		Ponto* p633 = p633; //new Vertice ( -2.00000, 0.00000, 0.90000
  //);

  //		Curva* patch16_c1 = new CurvParamBezier ( *p1333, *p1413,
  //*p1423,
  //*p1303 ); 		Curva* patch16_c2 = new CurvParamBezier ( *p1303,
  //*p1501, *p1502, *p633 );
  auto patch16_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1533, *p1613, *p1623, *p633);
  //		Curva* patch16_c4 = new CurvParamBezier ( *p1333, *p1531,
  //*p1532, *p1533 );

  auto patch16 =
      std::make_shared<PatchBezier>(patch14_c3, patch15_c4, patch16_c3,
                                    patch15_c2, *p1611, *p1621, *p1612, *p1622);

  geometry->InsertPatch(patch16);
  //		geo->InsertCurve ( patch16_c1 );
  //		geo->InsertCurve ( patch16_c2 );
  geometry->InsertCurve(patch16_c3);
  //		geo->InsertCurve ( patch16_c4 );
  //======================== FIM DO PATCH 16 =================================

  //============================== PATCH 17 ==================================
  auto p1700 = std::make_shared<VertexAdaptive>(1.70000, 0.00000, 1.42500);
  auto p1710 = std::make_shared<VertexAdaptive>(1.70000, -0.66000, 1.42500);
  auto p1720 = std::make_shared<VertexAdaptive>(1.70000, -0.66000, 0.60000);
  auto p1730 = std::make_shared<VertexAdaptive>(1.70000, 0.00000, 0.60000);

  auto p1701 = std::make_shared<VertexAdaptive>(2.60000, 0.00000, 1.42500);
  auto p1711 = std::make_shared<VertexAdaptive>(2.60000, -0.66000, 1.42500);
  auto p1721 = std::make_shared<VertexAdaptive>(3.10000, -0.66000, 0.82500);
  auto p1731 = std::make_shared<VertexAdaptive>(3.10000, 0.00000, 0.82500);

  auto p1702 = std::make_shared<VertexAdaptive>(2.30000, 0.00000, 2.10000);
  auto p1712 = std::make_shared<VertexAdaptive>(2.30000, -0.25000, 2.10000);
  auto p1722 = std::make_shared<VertexAdaptive>(2.40000, -0.25000, 2.02500);
  auto p1732 = std::make_shared<VertexAdaptive>(2.40000, 0.00000, 2.02500);

  auto p1703 = std::make_shared<VertexAdaptive>(2.70000, 0.00000, 2.40000);
  auto p1713 = std::make_shared<VertexAdaptive>(2.70000, -0.25000, 2.40000);
  auto p1723 = std::make_shared<VertexAdaptive>(3.30000, -0.25000, 2.40000);
  auto p1733 = std::make_shared<VertexAdaptive>(3.30000, 0.00000, 2.40000);

  auto patch17_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1700, *p1710, *p1720, *p1730);
  auto patch17_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1730, *p1731, *p1732, *p1733);
  auto patch17_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1703, *p1713, *p1723, *p1733);
  auto patch17_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1700, *p1701, *p1702, *p1703);

  auto patch17 =
      std::make_shared<PatchBezier>(patch17_c1, patch17_c2, patch17_c3,
                                    patch17_c4, *p1711, *p1721, *p1712, *p1722);

  geometry->InsertPatch(patch17);
  geometry->InsertCurve(patch17_c1);
  geometry->InsertCurve(patch17_c2);
  geometry->InsertCurve(patch17_c3);
  geometry->InsertCurve(patch17_c4);
  //======================== FIM DO PATCH 17 =================================

  //============================== PATCH 18 ==================================
  //		Ponto* p1730 = p1730; //new Vertice ( 1.70000, 0.00000, 0.60000
  //);
  auto p1810 = std::make_shared<VertexAdaptive>(1.70000, 0.66000, 0.60000);
  auto p1820 = std::make_shared<VertexAdaptive>(1.70000, 0.66000, 1.42500);
  //		Ponto* p1700 = p1700; //new Vertice ( 1.70000, 0.00000, 1.42500
  //);

  //		Ponto* p1731 = p1731; //new Vertice ( 3.10000, 0.00000, 0.82500
  //);
  auto p1811 = std::make_shared<VertexAdaptive>(3.10000, 0.66000, 0.82500);
  auto p1821 = std::make_shared<VertexAdaptive>(2.60000, 0.66000, 1.42500);
  //		Ponto* p1701 = p1701; //new Vertice ( 2.60000, 0.00000, 1.42500
  //);

  //		Ponto* p1732 = p1732; //new Vertice ( 2.40000, 0.00000, 2.02500
  //);
  auto p1812 = std::make_shared<VertexAdaptive>(2.40000, 0.25000, 2.02500);
  auto p1822 = std::make_shared<VertexAdaptive>(2.30000, 0.25000, 2.10000);
  //		Ponto* p1702 = p1702; //new Vertice ( 2.30000, 0.00000, 2.10000
  //);

  //		Ponto* p1733 = p1733; //new Vertice ( 3.30000, 0.00000, 2.40000
  //);
  auto p1813 = std::make_shared<VertexAdaptive>(3.30000, 0.25000, 2.40000);
  auto p1823 = std::make_shared<VertexAdaptive>(2.70000, 0.25000, 2.40000);
  //		Ponto* p1703 = p1703; //new Vertice ( 2.70000, 0.00000, 2.40000
  //);

  auto patch18_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1730, *p1810, *p1820, *p1700);
  //		Curva* patch18_c2 = new CurvParamBezier ( *p1700, *p1701,
  //*p1702, *p1703 );
  auto patch18_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1733, *p1813, *p1823, *p1703);
  //		Curva* patch18_c4 = new CurvParamBezier ( *p1730, *p1731,
  //*p1732, *p1733 );

  auto patch18 =
      std::make_shared<PatchBezier>(patch18_c1, patch17_c4, patch18_c3,
                                    patch17_c2, *p1811, *p1821, *p1812, *p1822);

  geometry->InsertPatch(patch18);
  geometry->InsertCurve(patch18_c1);
  //		geo->InsertCurve ( patch18_c2 );
  geometry->InsertCurve(patch18_c3);
  //		geo->InsertCurve ( patch18_c4 );
  //======================== FIM DO PATCH 18 =================================

  //============================== PATCH 19 ==================================
  //		Ponto* p1703 = p1703; //new Vertice ( 2.70000, 0.00000, 2.40000
  //); 		Ponto* p1713 = p1713; //new Vertice ( 2.70000,-0.25000, 2.40000
  //);3.30000, 0.25000, 2.40000 		Ponto* p1723 = p1723; //new
  // Vertice
  //( 3.30000,-0.25000, 2.40000 ); 		Ponto* p1733 = p1733; //new
  // Vertice ( 3.30000, 0.00000, 2.40000 );

  auto p1901 = std::make_shared<VertexAdaptive>(2.80000, 0.00000, 2.47500);
  auto p1911 = std::make_shared<VertexAdaptive>(2.80000, -0.25000, 2.47500);
  auto p1921 = std::make_shared<VertexAdaptive>(3.52500, -0.25000, 2.49375);
  auto p1931 = std::make_shared<VertexAdaptive>(3.52500, 0.00000, 2.49375);

  auto p1902 = std::make_shared<VertexAdaptive>(2.90000, 0.00000, 2.47500);
  auto p1912 = std::make_shared<VertexAdaptive>(2.90000, -0.15000, 2.47500);
  auto p1922 = std::make_shared<VertexAdaptive>(3.45000, -0.15000, 2.51250);
  auto p1932 = std::make_shared<VertexAdaptive>(3.45000, 0.00000, 2.51250);

  auto p1903 = std::make_shared<VertexAdaptive>(2.80000, 0.00000, 2.40000);
  auto p1913 = std::make_shared<VertexAdaptive>(2.80000, -0.15000, 2.40000);
  //		Ponto* p1933 = new Vertice ( 3.20000, 0.00000, 2.40000 );
  auto p1933 = std::make_shared<VertexAdaptive>(3.20000, 0.00000, 2.40000);

  //		Curva* patch19_c1 = new CurvParamBezier ( *p1703, *p1713,
  //*p1723, *p1733 );
  auto patch19_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1733, *p1931, *p1932, *p1933);
  auto patch19_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1903, *p1913, *p1933, *p1933);
  auto patch19_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1703, *p1901, *p1902, *p1903);

  auto patch19 =
      std::make_shared<PatchBezier>(patch17_c3, patch19_c2, patch19_c3,
                                    patch19_c4, *p1911, *p1921, *p1912, *p1922);

  geometry->InsertPatch(patch19);
  //		geo->InsertCurve ( patch19_c1 );
  geometry->InsertCurve(patch19_c2);
  geometry->InsertCurve(patch19_c3);
  geometry->InsertCurve(patch19_c4);
  //======================== FIM DO PATCH 19 =================================

  //============================== PATCH 20 ==================================
  //		Ponto* p1733 = p1733; //new Vertice ( 3.30000, 0.00000, 2.40000
  //);
  auto p2010 = std::make_shared<VertexAdaptive>(3.30000, 0.25000, 2.40000);
  auto p2020 = std::make_shared<VertexAdaptive>(2.70000, 0.25000, 2.40000);
  //		Ponto* p1703 = p1703; //new Vertice ( 2.70000, 0.00000, 2.40000
  //);

  //		Ponto* p1931 = p1931; //new Vertice ( 3.52500, 0.00000, 2.49375
  //);
  auto p2011 = std::make_shared<VertexAdaptive>(3.52500, 0.25000, 2.49375);
  auto p2021 = std::make_shared<VertexAdaptive>(2.80000, 0.25000, 2.47500);
  //		Ponto* p1901 = p1901; //new Vertice ( 2.80000, 0.00000, 2.47500
  //);

  //		Ponto* p1932 = p1932; //new Vertice ( 3.45000, 0.00000, 2.51250
  //);
  auto p2012 = std::make_shared<VertexAdaptive>(3.45000, 0.15000, 2.51250);
  auto p2022 = std::make_shared<VertexAdaptive>(2.90000, 0.15000, 2.47500);
  //		Ponto* p1902 = p1902; //new Vertice ( 2.90000, 0.00000, 2.47500
  //);

  //		Ponto* p1933 = p1933; //new Vertice ( 3.20000, 0.00000, 2.40000
  //); 		Ponto* p2013 = new Vertice ( 3.20000, 0.15000, 2.40000 );
  auto p2023 = std::make_shared<VertexAdaptive>(2.80000, 0.15000, 2.40000);
  //		Ponto* p1903 = p1903; //new Vertice ( 2.80000, 0.00000, 2.40000
  //);

  auto patch20_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1733, *p2010, *p2020, *p1703);
  //		Curva* patch20_c2 = new CurvParamBezier ( *p1703, *p1901,
  //*p1902, *p1903 );
  auto patch20_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1933, *p1933, *p2023, *p1903);
  //		Curva* patch20_c4 = new CurvParamBezier ( *p1733, *p1931,
  //*p1932, *p1933 );

  auto patch20 =
      std::make_shared<PatchBezier>(patch20_c1, patch19_c4, patch20_c3,
                                    patch19_c2, *p2011, *p2021, *p2012, *p2022);

  geometry->InsertPatch(patch20);
  geometry->InsertCurve(patch20_c1);
  //		geo->InsertCurve ( patch20_c2 );
  geometry->InsertCurve(patch20_c3);
  //		geo->InsertCurve ( patch20_c4 );
  //======================== FIM DO PATCH 20 =================================

  //============================== PATCH 21 ==================================
  auto p2100 = std::make_shared<VertexAdaptive>(0.10000, 0.00000, 3.15000);
  auto p2110 = std::make_shared<VertexAdaptive>(0.08660, -0.05000, 3.15000);
  auto p2120 = std::make_shared<VertexAdaptive>(0.05000, -0.08660, 3.15000);
  auto p2130 = std::make_shared<VertexAdaptive>(0.00000, -0.10000, 3.15000);

  auto p2101 = std::make_shared<VertexAdaptive>(0.80000, 0.00000, 3.15000);
  auto p2111 = std::make_shared<VertexAdaptive>(0.80000, -0.45000, 3.15000);
  auto p2121 = std::make_shared<VertexAdaptive>(0.45000, -0.80000, 3.15000);
  auto p2131 = std::make_shared<VertexAdaptive>(0.00000, -0.80000, 3.15000);

  auto p2102 = std::make_shared<VertexAdaptive>(0.00000, 0.00000, 2.85000);
  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // );

  auto p2103 = std::make_shared<VertexAdaptive>(0.20000, 0.00000, 2.70000);
  auto p2113 = std::make_shared<VertexAdaptive>(0.20000, -0.11200, 2.70000);
  auto p2123 = std::make_shared<VertexAdaptive>(0.11200, -0.20000, 2.70000);
  auto p2133 = std::make_shared<VertexAdaptive>(0.00000, -0.20000, 2.70000);

  auto patch21_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2100, *p2110, *p2120, *p2130);
  auto patch21_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2130, *p2131, *p2102, *p2133);
  auto patch21_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2103, *p2113, *p2123, *p2133);
  auto patch21_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2100, *p2101, *p2102, *p2103);

  auto patch21 =
      std::make_shared<PatchBezier>(patch21_c1, patch21_c2, patch21_c3,
                                    patch21_c4, *p2111, *p2121, *p2102, *p2102);

  geometry->InsertPatch(patch21);
  geometry->InsertCurve(patch21_c1);
  geometry->InsertCurve(patch21_c2);
  geometry->InsertCurve(patch21_c3);
  geometry->InsertCurve(patch21_c4);
  //======================== FIM DO PATCH 21 =================================

  //============================== PATCH 22 ==================================
  //		Ponto* p2130 = p2130; //new Vertice ( 0.00000,-0.10000, 3.15000
  //);
  auto p2210 = std::make_shared<VertexAdaptive>(-0.05000, -0.08660, 3.15000);
  auto p2220 = std::make_shared<VertexAdaptive>(-0.08660, -0.05000, 3.15000);
  auto p2230 = std::make_shared<VertexAdaptive>(-0.10000, 0.00000, 3.15000);

  //		Ponto* p2131 = p2131; //new Vertice (  0.00000,-0.80000, 3.15000
  //);
  auto p2211 = std::make_shared<VertexAdaptive>(-0.45000, -0.80000, 3.15000);
  auto p2221 = std::make_shared<VertexAdaptive>(-0.80000, -0.45000, 3.15000);
  auto p2231 = std::make_shared<VertexAdaptive>(-0.80000, 0.00000, 3.15000);

  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // ); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000,
  // 0.00000, 2.85000 );

  //		Ponto* p2133 = p2133; //new Vertice (  0.00000,-0.20000, 2.70000
  //);
  auto p2213 = std::make_shared<VertexAdaptive>(-0.11200, -0.20000, 2.70000);
  auto p2223 = std::make_shared<VertexAdaptive>(-0.20000, -0.11200, 2.70000);
  auto p2233 = std::make_shared<VertexAdaptive>(-0.20000, 0.00000, 2.70000);

  auto patch22_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2130, *p2210, *p2220, *p2230);
  auto patch22_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2230, *p2231, *p2102, *p2233);
  auto patch22_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2133, *p2213, *p2223, *p2233);
  //		Curva* patch22_c4 = new CurvParamBezier ( *p2130, *p2131,
  //*p2102, *p2133 );

  auto patch22 =
      std::make_shared<PatchBezier>(patch22_c1, patch22_c2, patch22_c3,
                                    patch21_c2, *p2211, *p2221, *p2102, *p2102);

  geometry->InsertPatch(patch22);
  geometry->InsertCurve(patch22_c1);
  geometry->InsertCurve(patch22_c2);
  geometry->InsertCurve(patch22_c3);
  //		geo->InsertCurve ( patch22_c4 );
  //======================== FIM DO PATCH 22 =================================

  //============================== PATCH 23 ==================================
  //		Ponto* p2230 = new Vertice (-0.10000, 0.00000, 3.15000 );
  auto p2310 = std::make_shared<VertexAdaptive>(-0.08660, 0.05000, 3.15000);
  auto p2320 = std::make_shared<VertexAdaptive>(-0.05000, 0.08660, 3.15000);
  auto p2330 = std::make_shared<VertexAdaptive>(0.00000, 0.10000, 3.15000);

  //		Ponto* p2231 = new Vertice ( -0.80000, 0.00000, 3.15000 );
  auto p2311 = std::make_shared<VertexAdaptive>(-0.80000, 0.45000, 3.15000);
  auto p2321 = std::make_shared<VertexAdaptive>(-0.45000, 0.80000, 3.15000);
  auto p2331 = std::make_shared<VertexAdaptive>(0.00000, 0.80000, 3.15000);

  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // ); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000,
  // 0.00000, 2.85000 );

  //		Ponto* p2233 = new Vertice ( -0.20000, 0.00000, 2.70000 );
  auto p2313 = std::make_shared<VertexAdaptive>(-0.20000, 0.11200, 2.70000);
  auto p2323 = std::make_shared<VertexAdaptive>(-0.11200, 0.20000, 2.70000);
  auto p2333 = std::make_shared<VertexAdaptive>(0.00000, 0.20000, 2.70000);

  auto patch23_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2230, *p2310, *p2320, *p2330);
  auto patch23_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2330, *p2331, *p2102, *p2333);
  auto patch23_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2233, *p2313, *p2323, *p2333);
  //		Curva* patch23_c4 = new CurvParamBezier ( *p2230, *p2231,
  //*p2102, *p2233 );

  auto patch23 =
      std::make_shared<PatchBezier>(patch23_c1, patch23_c2, patch23_c3,
                                    patch22_c2, *p2311, *p2321, *p2102, *p2102);

  geometry->InsertPatch(patch23);
  geometry->InsertCurve(patch23_c1);
  geometry->InsertCurve(patch23_c2);
  geometry->InsertCurve(patch23_c3);
  //		geo->InsertCurve ( patch23_c4 );
  //======================== FIM DO PATCH 23 =================================

  //============================== PATCH 24 ==================================
  //		Ponto* p2330 = new Vertice ( 0.00000, 0.10000, 3.15000 );
  auto p2410 = std::make_shared<VertexAdaptive>(0.05000, 0.08660, 3.15000);
  auto p2420 = std::make_shared<VertexAdaptive>(0.08660, 0.05000, 3.15000);
  //		Ponto* p2100 = new Vertice ( 0.10000, 0.00000, 3.15000 );

  //		Ponto* p2331 = new Vertice ( 0.00000, 0.80000, 3.15000 );
  auto p2411 = std::make_shared<VertexAdaptive>(0.45000, 0.80000, 3.15000);
  auto p2421 = std::make_shared<VertexAdaptive>(0.80000, 0.45000, 3.15000);
  auto p2431 = p2101;  // new Vertice ( 0.80000, 0.00000, 3.15000 );

  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // ); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000,
  // 0.00000, 2.85000 );

  //		Ponto* p2333 = new Vertice ( 0.00000, 0.20000, 2.70000 );
  auto p2413 = std::make_shared<VertexAdaptive>(0.11200, 0.20000, 2.70000);
  auto p2423 = std::make_shared<VertexAdaptive>(0.20000, 0.11200, 2.70000);
  //		Ponto* p2103 = p2103; //new Vertice ( 0.20000, 0.00000, 2.70000
  //);

  auto patch24_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2330, *p2410, *p2420, *p2100);
  auto patch24_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2100, *p2431, *p2102, *p2103);
  auto patch24_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2333, *p2413, *p2423, *p2103);
  //		Curva* patch24_c4 = new CurvParamBezier ( *p2330, *p2331,
  //*p2102, *p2333 );

  auto patch24 =
      std::make_shared<PatchBezier>(patch24_c1, patch24_c2, patch24_c3,
                                    patch23_c2, *p2411, *p2421, *p2102, *p2102);

  geometry->InsertPatch(patch24);
  geometry->InsertCurve(patch24_c1);
  geometry->InsertCurve(patch24_c2);
  geometry->InsertCurve(patch24_c3);
  //		geo->InsertCurve ( patch24_c4 );
  //======================== FIM DO PATCH 24 =================================

  //============================== PATCH 25 ==================================
  //		Ponto* p2103 = p2103; //new Vertice ( 0.20000, 0.00000, 2.70000
  //); 		Ponto* p2113 = p2113; //new Vertice ( 0.20000,-0.11200, 2.70000
  //); 		Ponto* p2123 = p2123; //new Vertice ( 0.11200,-0.20000, 2.70000
  // ); 		Ponto* p2133 = p2133; //new Vertice (
  // 0.00000,-0.20000, 2.70000 );

  auto p2501 = std::make_shared<VertexAdaptive>(0.40000, 0.00000, 2.55000);
  auto p2511 = std::make_shared<VertexAdaptive>(0.40000, -0.22400, 2.55000);
  auto p2521 = std::make_shared<VertexAdaptive>(0.22400, -0.40000, 2.55000);
  auto p2531 = std::make_shared<VertexAdaptive>(0.00000, -0.40000, 2.55000);

  auto p2502 = std::make_shared<VertexAdaptive>(1.30000, 0.00000, 2.55000);
  auto p2512 = std::make_shared<VertexAdaptive>(1.30000, -0.72800, 2.55000);
  auto p2522 = std::make_shared<VertexAdaptive>(0.72800, -1.30000, 2.55000);
  auto p2532 = std::make_shared<VertexAdaptive>(0.00000, -1.30000, 2.55000);

  auto p2503 = std::make_shared<VertexAdaptive>(1.30000, 0.00000, 2.40000);
  auto p2513 = std::make_shared<VertexAdaptive>(1.30000, -0.72800, 2.40000);
  auto p2523 = std::make_shared<VertexAdaptive>(0.72800, -1.30000, 2.40000);
  auto p2533 = std::make_shared<VertexAdaptive>(0.00000, -1.30000, 2.40000);

  //		Curva* patch25_c1 = new CurvParamBezier ( *p2103, *p2113,
  //*p2123, *p2133 );
  auto patch25_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2133, *p2531, *p2532, *p2533);
  auto patch25_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2503, *p2513, *p2523, *p2533);
  auto patch25_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2103, *p2501, *p2502, *p2503);

  auto patch25 =
      std::make_shared<PatchBezier>(patch21_c3, patch25_c2, patch25_c3,
                                    patch25_c4, *p2511, *p2521, *p2512, *p2522);

  geometry->InsertPatch(patch25);
  //		geo->InsertCurve ( patch25_c1 );
  geometry->InsertCurve(patch25_c2);
  geometry->InsertCurve(patch25_c3);
  geometry->InsertCurve(patch25_c4);
  //======================== FIM DO PATCH 25 =================================

  //============================== PATCH 26 ==================================
  //		Ponto* p2133 = p2133; //new Vertice ( 0.00000,-0.20000, 2.70000
  //); 		Ponto* p2213 = new Vertice (-0.11200,-0.20000, 2.70000 );
  // Ponto* p2223 = new Vertice (-0.20000,-0.11200, 2.70000 ); 		Ponto*
  // p2233 = new Vertice
  //(-0.20000, 0.00000, 2.70000 );

  //		Ponto* p2531 = new Vertice ( 0.00000,-0.40000, 2.55000 );
  auto p2611 = std::make_shared<VertexAdaptive>(-0.22400, -0.40000, 2.55000);
  auto p2621 = std::make_shared<VertexAdaptive>(-0.40000, -0.22400, 2.55000);
  auto p2631 = std::make_shared<VertexAdaptive>(-0.40000, 0.00000, 2.55000);

  //		Ponto* p2532 = new Vertice ( 0.00000,-1.30000, 2.55000 );
  auto p2612 = std::make_shared<VertexAdaptive>(-0.72800, -1.30000, 2.55000);
  auto p2622 = std::make_shared<VertexAdaptive>(-1.30000, -0.72800, 2.55000);
  auto p2632 = std::make_shared<VertexAdaptive>(-1.30000, 0.00000, 2.55000);

  //		Ponto* p2533 = new Vertice ( 0.00000,-1.30000, 2.40000 );
  auto p2613 = std::make_shared<VertexAdaptive>(-0.72800, -1.30000, 2.40000);
  auto p2623 = std::make_shared<VertexAdaptive>(-1.30000, -0.72800, 2.40000);
  auto p2633 = std::make_shared<VertexAdaptive>(-1.30000, 0.00000, 2.40000);

  //		Curva* patch26_c1 = new CurvParamBezier ( *p2133, *p2213,
  //*p2223, *p2233 );
  auto patch26_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2233, *p2631, *p2632, *p2633);
  auto patch26_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2533, *p2613, *p2623, *p2633);
  //		Curva* patch26_c4 = new CurvParamBezier ( *p2133, *p2531,
  //*p2532, *p2533 );

  auto patch26 =
      std::make_shared<PatchBezier>(patch22_c3, patch26_c2, patch26_c3,
                                    patch25_c2, *p2611, *p2621, *p2612, *p2622);

  geometry->InsertPatch(patch26);
  //		geo->InsertCurve ( patch26_c1 );
  geometry->InsertCurve(patch26_c2);
  geometry->InsertCurve(patch26_c3);
  //		geo->InsertCurve ( patch26_c4 );
  //======================== FIM DO PATCH 26 =================================

  //============================== PATCH 27 ==================================
  //		Ponto* p2233 = new Vertice ( -0.20000, 0.00000, 2.70000 );
  //		Ponto* p2313 = new Vertice ( -0.20000, 0.11200, 2.70000 );
  //		Ponto* p2323 = new Vertice ( -0.11200, 0.20000, 2.70000 );
  //		Ponto* p2333 = new Vertice (  0.00000, 0.20000, 2.70000 );

  //		Ponto* p2631 = new Vertice ( -0.40000, 0.00000, 2.55000 );
  auto p2711 = std::make_shared<VertexAdaptive>(-0.40000, 0.22400, 2.55000);
  auto p2721 = std::make_shared<VertexAdaptive>(-0.22400, 0.40000, 2.55000);
  auto p2731 = std::make_shared<VertexAdaptive>(0.00000, 0.40000, 2.55000);

  //		Ponto* p2632 = new Vertice ( -1.30000, 0.00000, 2.55000 );
  auto p2712 = std::make_shared<VertexAdaptive>(-1.30000, 0.72800, 2.55000);
  auto p2722 = std::make_shared<VertexAdaptive>(-0.72800, 1.30000, 2.55000);
  auto p2732 = std::make_shared<VertexAdaptive>(0.00000, 1.30000, 2.55000);

  //		Ponto* p2633 = new Vertice ( -1.30000, 0.00000, 2.40000 );
  auto p2713 = std::make_shared<VertexAdaptive>(-1.30000, 0.72800, 2.40000);
  auto p2723 = std::make_shared<VertexAdaptive>(-0.72800, 1.30000, 2.40000);
  auto p2733 = std::make_shared<VertexAdaptive>(0.00000, 1.30000, 2.40000);

  //		Curva* patch27_c1 = new CurvParamBezier ( *p2233, *p2313,
  //*p2323, *p2333 );
  auto patch27_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2333, *p2731, *p2732, *p2733);
  auto patch27_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2633, *p2713, *p2723, *p2733);
  //		Curva* patch27_c4 = new CurvParamBezier ( *p2233, *p2631,
  //*p2632, *p2633 );

  auto patch27 =
      std::make_shared<PatchBezier>(patch23_c3, patch27_c2, patch27_c3,
                                    patch26_c2, *p2711, *p2721, *p2712, *p2722);

  geometry->InsertPatch(patch27);
  //		geo->InsertCurve ( patch27_c1 );
  geometry->InsertCurve(patch27_c2);
  geometry->InsertCurve(patch27_c3);
  //		geo->InsertCurve ( patch27_c4 );
  //======================== FIM DO PATCH 27 =================================

  //============================== PATCH 28 ==================================
  //		Ponto* p2333 = new Vertice ( 0.00000, 0.20000, 2.70000 );
  //		Ponto* p2413 = new Vertice ( 0.11200, 0.20000, 2.70000 );
  //		Ponto* p2423 = new Vertice ( 0.20000, 0.11200, 2.70000 );
  //		Ponto* p2103 = p2103; //new Vertice ( 0.20000, 0.00000, 2.70000
  //);

  //		Ponto* p2731 = new Vertice ( 0.00000, 0.40000, 2.55000 );
  auto p2811 = std::make_shared<VertexAdaptive>(0.22400, 0.40000, 2.55000);
  auto p2821 = std::make_shared<VertexAdaptive>(0.40000, 0.22400, 2.55000);
  //		Ponto* p2501 = new Vertice ( 0.40000, 0.00000, 2.55000 );

  //		Ponto* p2732 = new Vertice ( 0.00000, 1.30000, 2.55000 );
  auto p2812 = std::make_shared<VertexAdaptive>(0.72800, 1.30000, 2.55000);
  auto p2822 = std::make_shared<VertexAdaptive>(1.30000, 0.72800, 2.55000);
  //		Ponto* p2502 = new Vertice ( 1.30000, 0.00000, 2.55000 );

  //		Ponto* p2733 = new Vertice ( 0.00000, 1.30000, 2.40000 );
  auto p2813 = std::make_shared<VertexAdaptive>(0.72800, 1.30000, 2.40000);
  auto p2823 = std::make_shared<VertexAdaptive>(1.30000, 0.72800, 2.40000);
  //		Ponto* p2503 = new Vertice ( 1.30000, 0.00000, 2.40000 );

  //		Curva* patch28_c1 = new CurvParamBezier ( *p2333, *p2413,
  //*p2423,
  //*p2103 ); 		Curva* patch28_c2 = new CurvParamBezier ( *p2103,
  //*p2501, *p2502, *p2503 );
  auto patch28_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2733, *p2813, *p2823, *p2503);
  //		Curva* patch28_c4 = new CurvParamBezier ( *p2333, *p2731,
  //*p2732, *p2733 );

  auto patch28 =
      std::make_shared<PatchBezier>(patch24_c3, patch25_c4, patch28_c3,
                                    patch27_c2, *p2811, *p2821, *p2812, *p2822);

  geometry->InsertPatch(patch28);
  //		geo->InsertCurve ( patch28_c1 );
  //		geo->InsertCurve ( patch28_c2 );
  geometry->InsertCurve(patch28_c3);
  //		geo->InsertCurve ( patch28_c4 );
  //======================== FIM DO PATCH 28 =================================

  //============================== PATCH 29 ==================================
  auto p2900 = std::make_shared<VertexAdaptive>(0.10000, 0.00000, 0.00001);
  auto p2910 = std::make_shared<VertexAdaptive>(0.08660, 0.05000, 0.00001);
  auto p2920 = std::make_shared<VertexAdaptive>(0.05000, 0.08660, 0.00001);
  auto p2930 = std::make_shared<VertexAdaptive>(0.00000, 0.10000, 0.00001);

  auto p2901 = std::make_shared<VertexAdaptive>(1.42500, 0.00000, 0.00000);
  auto p2911 = std::make_shared<VertexAdaptive>(1.42500, 0.79800, 0.00000);
  auto p2921 = std::make_shared<VertexAdaptive>(0.79800, 1.42500, 0.00000);
  auto p2931 = std::make_shared<VertexAdaptive>(0.00000, 1.42500, 0.00000);

  auto p2902 = std::make_shared<VertexAdaptive>(1.50000, 0.00000, 0.07500);
  auto p2912 = std::make_shared<VertexAdaptive>(1.50000, 0.84000, 0.07500);
  auto p2922 = std::make_shared<VertexAdaptive>(0.84000, 1.50000, 0.07500);
  auto p2932 = std::make_shared<VertexAdaptive>(0.00000, 1.50000, 0.07500);

  //		Ponto* p903 = p903; //new Vertice ( 1.50000, 0.00000, 0.15000 );
  //		Ponto* p1223 = new Vertice ( 1.50000, 0.84000, 0.15000 );
  //		Ponto* p1213 = new Vertice ( 0.84000, 1.50000, 0.15000 );
  //		Ponto* p1133 = p1133; //new Vertice ( 0.00000, 1.50000, 0.15000
  //);

  auto patch29_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2900, *p2910, *p2920, *p2930);
  auto patch29_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2930, *p2931, *p2932, *p1133);
  auto patch29_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p903, *p1223, *p1213, *p1133);
  auto patch29_c4 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2900, *p2901, *p2902, *p903);

  auto patch29 =
      std::make_shared<PatchBezier>(patch29_c1, patch29_c2, patch29_c3,
                                    patch29_c4, *p2911, *p2921, *p2912, *p2922);

  geometry->InsertPatch(patch29);
  geometry->InsertCurve(patch29_c1);
  geometry->InsertCurve(patch29_c2);
  geometry->InsertCurve(patch29_c3);
  geometry->InsertCurve(patch29_c4);
  //======================== FIM DO PATCH 29 =================================

  //============================== PATCH 30 ==================================
  //		Ponto* p2930 = new Vertice ( 0.00000, 0.10000, 0.00001 );
  auto p3010 = std::make_shared<VertexAdaptive>(-0.05000, 0.08660, 0.00001);
  auto p3020 = std::make_shared<VertexAdaptive>(-0.08660, 0.05000, 0.00001);
  auto p3030 = std::make_shared<VertexAdaptive>(-0.10000, 0.00000, 0.00001);

  //		Ponto* p2931 = new Vertice (  0.00000, 1.42500, 0.00000 );
  auto p3011 = std::make_shared<VertexAdaptive>(-0.79800, 1.42500, 0.00000);
  auto p3021 = std::make_shared<VertexAdaptive>(-1.42500, 0.79800, 0.00000);
  auto p3031 = std::make_shared<VertexAdaptive>(-1.42500, 0.00000, 0.00000);

  //		Ponto* p2932 = new Vertice ( 0.00000, 1.50000, 0.07500 );
  auto p3012 = std::make_shared<VertexAdaptive>(-0.84000, 1.50000, 0.07500);
  auto p3022 = std::make_shared<VertexAdaptive>(-1.50000, 0.84000, 0.07500);
  auto p3032 = std::make_shared<VertexAdaptive>(-1.50000, 0.00000, 0.07500);

  //		Ponto* p1133 = p1133; //new Vertice ( 0.00000, 1.50000, 0.15000
  //); 		Ponto* p1123 = p1123; //new Vertice (-0.84000, 1.50000, 0.15000
  //); 		Ponto* p1113 = p1113; //new Vertice (-1.50000, 0.84000, 0.15000
  // ); 		Ponto* p1033 = new Vertice (-1.50000, 0.00000, 0.15000
  // );

  auto patch30_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p2930, *p3010, *p3020, *p3030);
  auto patch30_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p3030, *p3031, *p3032, *p1033);
  auto patch30_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1133, *p1123, *p1113, *p1033);
  //		Curva* patch30_c4 = new CurvParamBezier ( *p2930, *p2931,
  //*p2932, *p1133 );

  auto patch30 =
      std::make_shared<PatchBezier>(patch30_c1, patch30_c2, patch30_c3,
                                    patch29_c2, *p3011, *p3021, *p3012, *p3022);

  geometry->InsertPatch(patch30);
  geometry->InsertCurve(patch30_c1);
  geometry->InsertCurve(patch30_c2);
  geometry->InsertCurve(patch30_c3);
  //		geo->InsertCurve ( patch30_c4 );
  //======================== FIM DO PATCH 30 =================================

  //============================== PATCH 31 ==================================
  //		Ponto* p3030 = new Vertice (-0.10000, 0.00000, 0.00001 );
  auto p3110 = std::make_shared<VertexAdaptive>(-0.08660, -0.05000, 0.00001);
  auto p3120 = std::make_shared<VertexAdaptive>(-0.05000, -0.08660, 0.00001);
  auto p3130 = std::make_shared<VertexAdaptive>(0.00000, -0.10000, 0.00001);

  //		Ponto* p3031 = new Vertice ( -1.42500, 0.00000, 0.00000 );
  auto p3111 = std::make_shared<VertexAdaptive>(-1.42500, -0.79800, 0.00000);
  auto p3121 = std::make_shared<VertexAdaptive>(-0.79800, -1.42500, 0.00000);
  auto p3131 = std::make_shared<VertexAdaptive>(0.00000, -1.42500, 0.00000);

  //		Ponto* p3032 = new Vertice ( -1.50000, 0.00000, 0.07500 );
  auto p3112 = std::make_shared<VertexAdaptive>(-1.50000, -0.84000, 0.07500);
  auto p3122 = std::make_shared<VertexAdaptive>(-0.84000, -1.50000, 0.07500);
  auto p3132 = std::make_shared<VertexAdaptive>(0.00000, -1.50000, 0.07500);

  //		Ponto* p1033 = new Vertice ( -1.50000, 0.00000, 0.15000 );
  //		Ponto* p1023 = new Vertice ( -1.50000,-0.84000, 0.15000 );
  //		Ponto* p1013 = new Vertice ( -0.84000,-1.50000, 0.15000 );
  //		Ponto* p933 = p933; //new Vertice (  0.00000,-1.50000, 0.15000
  //);

  auto patch31_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p3030, *p3110, *p3120, *p3130);
  auto patch31_c2 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p3130, *p3131, *p3132, *p933);
  auto patch31_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p1033, *p1023, *p1013, *p933);
  //		Curva* patch31_c4 = new CurvParamBezier ( *p3030, *p3031,
  //*p3032, *p1033 );

  auto patch31 =
      std::make_shared<PatchBezier>(patch31_c1, patch31_c2, patch31_c3,
                                    patch30_c2, *p3111, *p3121, *p3112, *p3122);

  geometry->InsertPatch(patch31);
  geometry->InsertCurve(patch31_c1);
  geometry->InsertCurve(patch31_c2);
  geometry->InsertCurve(patch31_c3);
  //		geo->InsertCurve ( patch31_c4 );
  //======================== FIM DO PATCH 31 =================================

  //============================== PATCH 32 ==================================
  //		Ponto* p3130 = new Vertice ( 0.00000,-0.10000, 0.00001 );
  auto p3210 = std::make_shared<VertexAdaptive>(0.05000, -0.08660, 0.00001);
  auto p3220 = std::make_shared<VertexAdaptive>(0.08660, -0.05000, 0.00001);
  //		Ponto* p2900 = new Vertice ( 0.10000, 0.00000, 0.00001 );

  //		Ponto* p3131 = new Vertice ( 0.00000,-1.42500, 0.00000 );
  auto p3211 = std::make_shared<VertexAdaptive>(0.79800, -1.42500, 0.00000);
  auto p3221 = std::make_shared<VertexAdaptive>(1.42500, -0.79800, 0.00000);
  //		Ponto* p2901 = new Vertice ( 1.42500, 0.00000, 0.00000 );

  //		Ponto* p3132 = new Vertice ( 0.00000,-1.50000, 0.07500 );
  auto p3212 = std::make_shared<VertexAdaptive>(0.84000, -1.50000, 0.07500);
  auto p3222 = std::make_shared<VertexAdaptive>(1.50000, -0.84000, 0.07500);
  //		Ponto* p2902 = new Vertice ( 1.50000, 0.00000, 0.07500 );

  //		Ponto* p933 = p933; //new Vertice ( 0.00000,-1.50000, 0.15000 );
  //		Ponto* p923 = p923; //new Vertice ( 0.84000,-1.50000, 0.15000 );
  //		Ponto* p913 = p913; //new Vertice ( 1.50000,-0.84000, 0.15000 );
  //		Ponto* p903 = p903; //new Vertice ( 1.50000, 0.00000, 0.15000 );

  auto patch32_c1 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p3130, *p3210, *p3220, *p2900);
  //		Curva* patch32_c2 = new CurvParamBezier ( *p2900, *p2901,
  //*p2902, *p903 );
  auto patch32_c3 = std::make_shared<CurveAdaptiveParametricBezier>(
      *p933, *p923, *p913, *p903);
  //		Curva* patch32_c4 = new CurvParamBezier ( *p3130, *p3131,
  //*p3132, *p933 );

  auto patch32 =
      std::make_shared<PatchBezier>(patch32_c1, patch29_c4, patch32_c3,
                                    patch31_c2, *p3211, *p3221, *p3212, *p3222);

  geometry->InsertPatch(patch32);
  geometry->InsertCurve(patch32_c1);
  //		geo->InsertCurve ( patch32_c2 );
  geometry->InsertCurve(patch32_c3);
  //		geo->InsertCurve ( patch32_c4 );
  //======================== FIM DO PATCH 32 =================================
  //==============================================================================
  // Fim do exemplo do Utahteapot
  //==============================================================================
}
