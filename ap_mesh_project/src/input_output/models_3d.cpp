#include "../../include/input_output/models_3d.h"

Models3d::Models3d() {}

Geometry* Models3d::ModelPlanBook(Geometry* geometry) {
  //=============================== PATCH 1 ==================================
  PointAdaptive* p100 = new VertexAdaptive(0.00000, 1.00000, 0.00000);
  PointAdaptive* p110 = new VertexAdaptive(0.00000, 0.50000, 0.00000);
  PointAdaptive* p120 = new VertexAdaptive(0.00000, -0.50000, 0.00000);
  PointAdaptive* p130 = new VertexAdaptive(0.00000, -1.00000, 0.00000);

  PointAdaptive* p101 = new VertexAdaptive(-0.50000, 1.00000, 0.00000);
  PointAdaptive* p111 = new VertexAdaptive(-0.50000, 0.50000, 0.00000);
  PointAdaptive* p121 = new VertexAdaptive(-0.50000, -0.50000, 0.00000);
  PointAdaptive* p131 = new VertexAdaptive(-0.50000, -1.00000, 0.00000);

  PointAdaptive* p102 = new VertexAdaptive(-1.50000, 1.00000, 0.00000);
  PointAdaptive* p112 = new VertexAdaptive(-1.50000, 0.50000, 0.00000);
  PointAdaptive* p122 = new VertexAdaptive(-1.50000, -0.50000, 0.00000);
  PointAdaptive* p132 = new VertexAdaptive(-1.50000, -1.00000, 0.00000);

  PointAdaptive* p103 = new VertexAdaptive(-2.00000, 1.00000, 0.00000);
  PointAdaptive* p113 = new VertexAdaptive(-2.00000, 0.50000, 0.00000);
  PointAdaptive* p123 = new VertexAdaptive(-2.00000, -0.50000, 0.00000);
  PointAdaptive* p133 = new VertexAdaptive(-2.00000, -1.00000, 0.00000);

  CurveAdaptive* patch1_c1 =
      new CurveAdaptiveParametricBezier(*p100, *p110, *p120, *p130);
  CurveAdaptive* patch1_c2 =
      new CurveAdaptiveParametricBezier(*p130, *p131, *p132, *p133);
  CurveAdaptive* patch1_c3 =
      new CurveAdaptiveParametricBezier(*p103, *p113, *p123, *p133);
  CurveAdaptive* patch1_c4 =
      new CurveAdaptiveParametricBezier(*p100, *p101, *p102, *p103);

  Patch* patch1 = new PatchBezier(patch1_c1, patch1_c2, patch1_c3, patch1_c4,
                                  *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================

  //=============================== PATCH 2 ==================================
  PointAdaptive* p200 = new VertexAdaptive(0.00000, 1.00000, 0.00000);
  // Ponto* p210 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p220 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  PointAdaptive* p230 = new VertexAdaptive(0.00000, -1.00000, 0.00000);

  PointAdaptive* p201 = new VertexAdaptive(0.00000, 1.00000, 0.50000);
  PointAdaptive* p211 = new VertexAdaptive(0.00000, 0.50000, 0.50000);
  PointAdaptive* p221 = new VertexAdaptive(0.00000, -0.50000, 0.50000);
  PointAdaptive* p231 = new VertexAdaptive(0.00000, -1.00000, 0.50000);

  PointAdaptive* p202 = new VertexAdaptive(0.00000, 1.00000, 1.50000);
  PointAdaptive* p212 = new VertexAdaptive(0.00000, 0.50000, 1.50000);
  PointAdaptive* p222 = new VertexAdaptive(0.00000, -0.50000, 1.50000);
  PointAdaptive* p232 = new VertexAdaptive(0.00000, -1.00000, 1.50000);

  PointAdaptive* p203 = new VertexAdaptive(0.00000, 1.00000, 2.00000);
  PointAdaptive* p213 = new VertexAdaptive(0.00000, 0.50000, 2.00000);
  PointAdaptive* p223 = new VertexAdaptive(0.00000, -0.50000, 2.00000);
  PointAdaptive* p233 = new VertexAdaptive(0.00000, -1.00000, 2.00000);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  CurveAdaptive* patch2_c2 =
      new CurveAdaptiveParametricBezier(*p230, *p231, *p232, *p233);
  CurveAdaptive* patch2_c3 =
      new CurveAdaptiveParametricBezier(*p203, *p213, *p223, *p233);
  CurveAdaptive* patch2_c4 =
      new CurveAdaptiveParametricBezier(*p200, *p201, *p202, *p203);

  Patch* patch2 = new PatchBezier(patch1_c1, patch2_c2, patch2_c3, patch2_c4,
                                  *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================

  //=============================== PATCH 3 ==================================
  PointAdaptive* p300 = new VertexAdaptive(0.00000, 1.00000, 0.00000);
  // Ponto* p310 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p320 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  PointAdaptive* p330 = new VertexAdaptive(0.00000, -1.00000, 0.00000);

  PointAdaptive* p301 = new VertexAdaptive(0.50000, 1.00000, 0.00000);
  PointAdaptive* p311 = new VertexAdaptive(0.50000, 0.50000, 0.00000);
  PointAdaptive* p321 = new VertexAdaptive(0.50000, -0.50000, 0.00000);
  PointAdaptive* p331 = new VertexAdaptive(0.50000, -1.00000, 0.00000);

  PointAdaptive* p302 = new VertexAdaptive(1.50000, 1.00000, 0.00000);
  PointAdaptive* p312 = new VertexAdaptive(1.50000, 0.50000, 0.00000);
  PointAdaptive* p322 = new VertexAdaptive(1.50000, -0.50000, 0.00000);
  PointAdaptive* p332 = new VertexAdaptive(1.50000, -1.00000, 0.00000);

  PointAdaptive* p303 = new VertexAdaptive(2.00000, 1.00000, 0.00000);
  PointAdaptive* p313 = new VertexAdaptive(2.00000, 0.50000, 0.00000);
  PointAdaptive* p323 = new VertexAdaptive(2.00000, -0.50000, 0.00000);
  PointAdaptive* p333 = new VertexAdaptive(2.00000, -1.00000, 0.00000);

  // Curva* patch3_c1 = new CurvParamBezier ( *p300, *p310, *p320, *p330 );
  CurveAdaptive* patch3_c2 =
      new CurveAdaptiveParametricBezier(*p330, *p331, *p332, *p333);
  CurveAdaptive* patch3_c3 =
      new CurveAdaptiveParametricBezier(*p303, *p313, *p323, *p333);
  CurveAdaptive* patch3_c4 =
      new CurveAdaptiveParametricBezier(*p300, *p301, *p302, *p303);

  Patch* patch3 = new PatchBezier(patch1_c1, patch3_c2, patch3_c3, patch3_c4,
                                  *p311, *p321, *p312, *p322);

  geometry->InsertPatch(patch3);
  // geo->InsertCurve ( patch3_c1 );
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  geometry->InsertCurve(patch3_c4);
  //======================== FIM DO PATCH 3 ==================================

  //=============================== PATCH 4 ==================================
  PointAdaptive* p400 = new VertexAdaptive(0.00000, 1.00000, 0.00000);
  // Ponto* p410 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p420 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  PointAdaptive* p430 = new VertexAdaptive(0.00000, -1.00000, 0.00000);

  PointAdaptive* p401 = new VertexAdaptive(-0.50000, 1.00000, 1.00000);
  PointAdaptive* p411 = new VertexAdaptive(-0.50000, 0.50000, 1.00000);
  PointAdaptive* p421 = new VertexAdaptive(-0.50000, -0.50000, 1.00000);
  PointAdaptive* p431 = new VertexAdaptive(-0.50000, -1.00000, 1.00000);

  PointAdaptive* p402 = new VertexAdaptive(-1.50000, 1.00000, 1.00000);
  PointAdaptive* p412 = new VertexAdaptive(-1.50000, 0.50000, 1.00000);
  PointAdaptive* p422 = new VertexAdaptive(-1.50000, -0.50000, 1.00000);
  PointAdaptive* p432 = new VertexAdaptive(-1.50000, -1.00000, 1.00000);

  PointAdaptive* p403 = new VertexAdaptive(-2.00000, 1.00000, 1.00000);
  PointAdaptive* p413 = new VertexAdaptive(-2.00000, 0.50000, 1.00000);
  PointAdaptive* p423 = new VertexAdaptive(-2.00000, -0.50000, 0.50000);
  PointAdaptive* p433 = new VertexAdaptive(-2.00000, -1.00000, 1.00000);

  // Curva* patch4_c1 = new CurvParamBezier ( *p400, *p410, *p420, *p430 );
  CurveAdaptive* patch4_c2 =
      new CurveAdaptiveParametricBezier(*p430, *p431, *p432, *p433);
  CurveAdaptive* patch4_c3 =
      new CurveAdaptiveParametricBezier(*p403, *p413, *p423, *p433);
  CurveAdaptive* patch4_c4 =
      new CurveAdaptiveParametricBezier(*p400, *p401, *p402, *p403);

  Patch* patch4 = new PatchBezier(patch1_c1, patch4_c2, patch4_c3, patch4_c4,
                                  *p411, *p421, *p412, *p422);

  geometry->InsertPatch(patch4);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch4_c2);
  geometry->InsertCurve(patch4_c3);
  geometry->InsertCurve(patch4_c4);
  //======================== FIM DO PATCH 4 ==================================

  //=============================== PATCH 5 ==================================

  PointAdaptive* p500 = new VertexAdaptive(0.00000, 1.00000, 0.00000);
  // Ponto* p510 = new Vertice ( 0.00000, 0.50000, 0.00000 );
  // Ponto* p520 = new Vertice ( 0.00000, -0.50000, 0.00000 );
  PointAdaptive* p530 = new VertexAdaptive(0.00000, -1.00000, 0.00000);

  PointAdaptive* p501 = new VertexAdaptive(0.50000, 1.00000, 0.28371);
  PointAdaptive* p511 = new VertexAdaptive(0.50000, 0.50000, 0.15342);
  PointAdaptive* p521 = new VertexAdaptive(0.50000, -0.50000, 0.50617);
  PointAdaptive* p531 = new VertexAdaptive(0.50000, -1.00000, 0.28371);

  PointAdaptive* p502 = new VertexAdaptive(1.50000, 1.00000, 0.69502);
  PointAdaptive* p512 = new VertexAdaptive(1.50000, 0.50000, 0.8342);
  PointAdaptive* p522 = new VertexAdaptive(1.50000, -0.50000, 1.094);
  PointAdaptive* p532 = new VertexAdaptive(1.50000, -1.00000, 0.8342);

  PointAdaptive* p503 = new VertexAdaptive(2.00000, 1.00000, 0.96197);
  PointAdaptive* p513 = new VertexAdaptive(2.00000, 0.50000, 0.96197);
  PointAdaptive* p523 = new VertexAdaptive(2.00000, -0.50000, 1.2141);
  PointAdaptive* p533 = new VertexAdaptive(2.00000, -1.00000, 0.76172);

  // Curva* patch5_c1 = new CurvParamBezier ( *p500, *p510, *p520, *p530 );
  CurveAdaptive* patch5_c2 =
      new CurveAdaptiveParametricBezier(*p530, *p531, *p532, *p533);
  CurveAdaptive* patch5_c3 =
      new CurveAdaptiveParametricBezier(*p503, *p513, *p523, *p533);
  CurveAdaptive* patch5_c4 =
      new CurveAdaptiveParametricBezier(*p500, *p501, *p502, *p503);

  Patch* patch5 = new PatchBezier(patch1_c1, patch5_c2, patch5_c3, patch5_c4,
                                  *p511, *p521, *p512, *p522);

  geometry->InsertPatch(patch5);
  // geo->InsertCurve ( patch5_c1 );
  geometry->InsertCurve(patch5_c2);
  geometry->InsertCurve(patch5_c3);
  geometry->InsertCurve(patch5_c4);
  //======================== FIM DO PATCH 5 ==================================

  return geometry;
}

Geometry* Models3d::ModelTresPatches(Geometry* geometry) {
  //==============================================================================
  // Exemplo do three_patches_curve
  //==============================================================================

  //=============================== PATCH 1 ==================================
  PointAdaptive* p100 = new VertexAdaptive(-1.00000, 1.00000, -1.00000);
  PointAdaptive* p110 = new VertexAdaptive(-1.00000, 0.50000, -1.00000);
  PointAdaptive* p120 = new VertexAdaptive(-1.00000, -0.50000, -1.00000);
  PointAdaptive* p130 = new VertexAdaptive(-1.00000, -1.00000, -1.00000);

  PointAdaptive* p101 = new VertexAdaptive(-1.00000, 1.00000, -0.50000);
  PointAdaptive* p111 = new VertexAdaptive(-1.00000, 0.50000, -0.50000);
  PointAdaptive* p121 = new VertexAdaptive(-1.00000, -0.50000, -0.50000);
  PointAdaptive* p131 = new VertexAdaptive(-1.00000, -1.00000, -0.50000);

  PointAdaptive* p102 = new VertexAdaptive(-1.00000, 1.00000, 0.50000);
  PointAdaptive* p112 = new VertexAdaptive(-0.95046, 0.50000, 0.50000);
  PointAdaptive* p122 = new VertexAdaptive(-1.19780, -0.50000, 0.50000);
  PointAdaptive* p132 = new VertexAdaptive(-1.22350, -1.00000, 0.50000);

  PointAdaptive* p103 = new VertexAdaptive(-1.00000, 1.00000, 1.00000);
  PointAdaptive* p113 = new VertexAdaptive(-0.59827, 0.50000, 1.00000);
  PointAdaptive* p123 = new VertexAdaptive(-1.7191, -0.50000, 1.00000);
  PointAdaptive* p133 = new VertexAdaptive(-1.00000, -1.00000, 1.00000);

  CurveAdaptive* patch1_c1 =
      new CurveAdaptiveParametricBezier(*p100, *p110, *p120, *p130);
  CurveAdaptive* patch1_c2 =
      new CurveAdaptiveParametricBezier(*p130, *p131, *p132, *p133);
  CurveAdaptive* patch1_c3 =
      new CurveAdaptiveParametricBezier(*p103, *p113, *p123, *p133);
  CurveAdaptive* patch1_c4 =
      new CurveAdaptiveParametricBezier(*p100, *p101, *p102, *p103);

  Patch* patch1 = new PatchBezier(patch1_c1, patch1_c2, patch1_c3, patch1_c4,
                                  *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================
  //=============================== PATCH 2 ==================================
  PointAdaptive* p200 = new VertexAdaptive(-1.00000, 1.00000, 1.00000);
  // Ponto* p210 = new Vertice ( -0.78827, 0.50000, 1.00000 );
  // Ponto* p220 = new Vertice ( -1.2291, -0.50000, 1.00000 );
  PointAdaptive* p230 = new VertexAdaptive(-1.00000, -1.00000, 1.00000);

  PointAdaptive* p201 = new VertexAdaptive(-0.50000, 1.00000, 1.00000);
  PointAdaptive* p211 = new VertexAdaptive(-0.50000, 0.50000, 1.00000);
  PointAdaptive* p221 = new VertexAdaptive(-0.50000, -0.50000, 1.00000);
  PointAdaptive* p231 = new VertexAdaptive(-0.50000, -1.00000, 1.00000);

  PointAdaptive* p202 = new VertexAdaptive(0.50000, 1.00000, 1.00000);
  PointAdaptive* p212 = new VertexAdaptive(0.50000, 0.50000, 1.00000);
  PointAdaptive* p222 = new VertexAdaptive(0.50000, -0.50000, 1.00000);
  PointAdaptive* p232 = new VertexAdaptive(0.50000, -1.00000, 1.00000);

  PointAdaptive* p203 = new VertexAdaptive(1.00000, 1.00000, 1.00000);
  PointAdaptive* p213 = new VertexAdaptive(1.00000, 0.50000, 1.00000);
  PointAdaptive* p223 = new VertexAdaptive(1.00000, -0.50000, 1.00000);
  PointAdaptive* p233 = new VertexAdaptive(1.00000, -1.00000, 1.00000);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  CurveAdaptive* patch2_c2 =
      new CurveAdaptiveParametricBezier(*p230, *p231, *p232, *p233);
  CurveAdaptive* patch2_c3 =
      new CurveAdaptiveParametricBezier(*p203, *p213, *p223, *p233);
  CurveAdaptive* patch2_c4 =
      new CurveAdaptiveParametricBezier(*p200, *p201, *p202, *p203);

  Patch* patch2 = new PatchBezier(patch1_c3, patch2_c2, patch2_c3, patch2_c4,
                                  *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================

  //=============================== PATCH 3 ==================================
  PointAdaptive* p300 = new VertexAdaptive(-1.00000, 1.00000, 1.00000);
  // Ponto* p310 = new Vertice ( -0.78827, 0.50000, 1.00000 );
  // Ponto* p320 = new Vertice ( -1.2291, -0.50000, 1.00000 );
  PointAdaptive* p330 = new VertexAdaptive(-1.00000, -1.00000, 1.00000);

  PointAdaptive* p301 = new VertexAdaptive(0.12941, 0.65856, 0.66782);
  PointAdaptive* p311 = new VertexAdaptive(0.46925, 0.26552, 0.78358);
  PointAdaptive* p321 = new VertexAdaptive(-0.21486, -0.30157, 0.8773);
  PointAdaptive* p331 = new VertexAdaptive(0.047791, -0.93912, 0.15139);

  PointAdaptive* p302 = new VertexAdaptive(0.45814, 0.79803, 0.11771);
  PointAdaptive* p312 = new VertexAdaptive(0.98101, 0.080047, 0.56072);
  PointAdaptive* p322 = new VertexAdaptive(0.51486, -0.22225, 0.522937);
  PointAdaptive* p332 = new VertexAdaptive(0.57169, -0.90869, -0.24963);

  PointAdaptive* p303 = new VertexAdaptive(0.41625, 0.64127, -0.58839);
  PointAdaptive* p313 = new VertexAdaptive(1.0956, 0.12175, 0.12245);
  PointAdaptive* p323 = new VertexAdaptive(1.0956, -0.37825, 0.12405);
  PointAdaptive* p333 = new VertexAdaptive(1.0956, -0.87825, -0.51823);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  CurveAdaptive* patch3_c2 =
      new CurveAdaptiveParametricBezier(*p330, *p331, *p332, *p333);
  CurveAdaptive* patch3_c3 =
      new CurveAdaptiveParametricBezier(*p303, *p313, *p323, *p333);
  CurveAdaptive* patch3_c4 =
      new CurveAdaptiveParametricBezier(*p300, *p301, *p302, *p303);

  Patch* patch3 = new PatchBezier(patch1_c3, patch3_c2, patch3_c3, patch3_c4,
                                  *p311, *p321, *p312, *p322);

  geometry->InsertPatch(patch3);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  geometry->InsertCurve(patch3_c4);
  //======================== FIM DO PATCH 3 ==================================
  //==============================================================================
  // FIM do Exemplo three_patches_curve
  //==============================================================================
  return geometry;
}

Geometry* Models3d::ModelDoisPatchesPlanosCurva1(Geometry* geometry) {
  //==============================================================================
  // Exemplo do two_patches_curve
  //==============================================================================

  //=============================== PATCH 1 ==================================
  PointAdaptive* p100 = new VertexAdaptive(-1.00000, 1.00000, -1.00000);
  PointAdaptive* p110 = new VertexAdaptive(-1.00000, 0.50000, -1.00000);
  PointAdaptive* p120 = new VertexAdaptive(-1.00000, -0.50000, -1.00000);
  PointAdaptive* p130 = new VertexAdaptive(-1.00000, -1.00000, -1.00000);

  PointAdaptive* p101 = new VertexAdaptive(-1.00000, 1.00000, -0.50000);
  PointAdaptive* p111 = new VertexAdaptive(-1.00000, 0.50000, -0.50000);
  PointAdaptive* p121 = new VertexAdaptive(-1.00000, -0.50000, -0.50000);
  PointAdaptive* p131 = new VertexAdaptive(-1.00000, -1.00000, -0.50000);

  PointAdaptive* p102 = new VertexAdaptive(-1.00000, 1.00000, 0.50000);
  PointAdaptive* p112 = new VertexAdaptive(-0.95046, 0.50000, 0.50000);
  PointAdaptive* p122 = new VertexAdaptive(-1.19780, -0.50000, 0.50000);
  PointAdaptive* p132 = new VertexAdaptive(-1.22350, -1.00000, 0.50000);

  PointAdaptive* p103 = new VertexAdaptive(-1.00000, 1.00000, 1.00000);
  PointAdaptive* p113 = new VertexAdaptive(-0.59827, 0.50000, 1.00000);
  PointAdaptive* p123 = new VertexAdaptive(-1.7191, -0.50000, 1.00000);
  PointAdaptive* p133 = new VertexAdaptive(-1.00000, -1.00000, 1.00000);

  CurveAdaptive* patch1_c1 =
      new CurveAdaptiveParametricBezier(*p100, *p110, *p120, *p130);
  CurveAdaptive* patch1_c2 =
      new CurveAdaptiveParametricBezier(*p130, *p131, *p132, *p133);
  CurveAdaptive* patch1_c3 =
      new CurveAdaptiveParametricBezier(*p103, *p113, *p123, *p133);
  CurveAdaptive* patch1_c4 =
      new CurveAdaptiveParametricBezier(*p100, *p101, *p102, *p103);

  Patch* patch1 = new PatchBezier(patch1_c1, patch1_c2, patch1_c3, patch1_c4,
                                  *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================
  //=============================== PATCH 2 ==================================
  PointAdaptive* p200 = new VertexAdaptive(-1.00000, 1.00000, 1.00000);
  // Ponto* p210 = new Vertice ( -0.78827, 0.50000, 1.00000 );
  // Ponto* p220 = new Vertice ( -1.2291, -0.50000, 1.00000 );
  PointAdaptive* p230 = new VertexAdaptive(-1.00000, -1.00000, 1.00000);

  PointAdaptive* p201 = new VertexAdaptive(-0.50000, 1.00000, 1.00000);
  PointAdaptive* p211 = new VertexAdaptive(-0.50000, 0.50000, 1.00000);
  PointAdaptive* p221 = new VertexAdaptive(-0.50000, -0.50000, 1.00000);
  PointAdaptive* p231 = new VertexAdaptive(-0.50000, -1.00000, 1.00000);

  PointAdaptive* p202 = new VertexAdaptive(0.50000, 1.00000, 1.00000);
  PointAdaptive* p212 = new VertexAdaptive(0.50000, 0.50000, 1.00000);
  PointAdaptive* p222 = new VertexAdaptive(0.50000, -0.50000, 1.00000);
  PointAdaptive* p232 = new VertexAdaptive(0.50000, -1.00000, 1.00000);

  PointAdaptive* p203 = new VertexAdaptive(1.00000, 1.00000, 1.00000);
  PointAdaptive* p213 = new VertexAdaptive(1.00000, 0.50000, 1.00000);
  PointAdaptive* p223 = new VertexAdaptive(1.00000, -0.50000, 1.00000);
  PointAdaptive* p233 = new VertexAdaptive(1.00000, -1.00000, 1.00000);

  // Curva* patch2_c1 = new CurvParamBezier ( *p200, *p210, *p220, *p230 );
  CurveAdaptive* patch2_c2 =
      new CurveAdaptiveParametricBezier(*p230, *p231, *p232, *p233);
  CurveAdaptive* patch2_c3 =
      new CurveAdaptiveParametricBezier(*p203, *p213, *p223, *p233);
  CurveAdaptive* patch2_c4 =
      new CurveAdaptiveParametricBezier(*p200, *p201, *p202, *p203);

  Patch* patch2 = new PatchBezier(patch1_c3, patch2_c2, patch2_c3, patch2_c4,
                                  *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  // geo->InsertCurve ( patch2_c1 );
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================
  //==============================================================================
  // FIM do Exemplo two_patches
  //==============================================================================
  return geometry;
}

Geometry* Models3d::ModelDoisPatchesPlanosCurva(Geometry* geometry) {
  //==============================================================================
  // Exemplo do Utahteapot
  //==============================================================================

  //=============================== PATCH 1 ==================================
  PointAdaptive* p100 = new VertexAdaptive(-6.0, 0.0, 0.0);
  PointAdaptive* p110 = new VertexAdaptive(-2.0, 2.0, 0.0);
  PointAdaptive* p120 = new VertexAdaptive(2.0, -2.0, 1.0);
  PointAdaptive* p130 = new VertexAdaptive(6.0, 0.0, 0.0);

  PointAdaptive* p101 = new VertexAdaptive(-6.0, 4.0, 0.0);
  PointAdaptive* p111 = new VertexAdaptive(-2.0, 4.0, 0.0);
  PointAdaptive* p121 = new VertexAdaptive(2.0, 4.0, 0.0);
  PointAdaptive* p131 = new VertexAdaptive(6.0, 4.0, 0.0);

  PointAdaptive* p102 = new VertexAdaptive(-6.0, 8.0, 0.0);
  PointAdaptive* p112 = new VertexAdaptive(-2.0, 8.0, 0.0);
  PointAdaptive* p122 = new VertexAdaptive(2.0, 8.0, 0.0);
  PointAdaptive* p132 = new VertexAdaptive(6.0, 8.0, 0.0);

  PointAdaptive* p103 = new VertexAdaptive(-6.0, 12.0, 0.0);
  PointAdaptive* p113 = new VertexAdaptive(-2.0, 12.0, 0.0);
  PointAdaptive* p123 = new VertexAdaptive(2.0, 12.0, 0.0);
  PointAdaptive* p133 = new VertexAdaptive(6.0, 12.0, 0.0);

  CurveAdaptive* patch1_c1 =
      new CurveAdaptiveParametricBezier(*p100, *p110, *p120, *p130);
  CurveAdaptive* patch1_c2 =
      new CurveAdaptiveParametricBezier(*p130, *p131, *p132, *p133);
  CurveAdaptive* patch1_c3 =
      new CurveAdaptiveParametricBezier(*p103, *p113, *p123, *p133);
  CurveAdaptive* patch1_c4 =
      new CurveAdaptiveParametricBezier(*p100, *p101, *p102, *p103);

  Patch* patch1 = new PatchBezier(patch1_c1, patch1_c2, patch1_c3, patch1_c4,
                                  *p111, *p121, *p112, *p122);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ==================================

  //=============================== PATCH 2 ==================================
  PointAdaptive* p200 = new VertexAdaptive(-6.0, -12.0, 0.0);
  PointAdaptive* p210 = new VertexAdaptive(-2.0, -12.0, 0.0);
  PointAdaptive* p220 = new VertexAdaptive(2.0, -12.0, 0.0);
  PointAdaptive* p230 = new VertexAdaptive(6.0, -12.0, 0.0);

  PointAdaptive* p201 = new VertexAdaptive(-6.0, -8.0, 0.0);
  PointAdaptive* p211 = new VertexAdaptive(-2.0, -8.0, 0.0);
  PointAdaptive* p221 = new VertexAdaptive(2.0, -8.0, 0.0);
  PointAdaptive* p231 = new VertexAdaptive(6.0, -8.0, 0.0);

  PointAdaptive* p202 = new VertexAdaptive(-6.0, -4.0, 0.0);
  PointAdaptive* p212 = new VertexAdaptive(-2.0, -4.0, 0.0);
  PointAdaptive* p222 = new VertexAdaptive(2.0, -4.0, 1.0);
  PointAdaptive* p232 = new VertexAdaptive(6.0, -4.0, 0.0);

  PointAdaptive* p203 = p100;  // new Vertice (  0.00000, -1.50000, 2.40000 );
  // Ponto* p213 = p110;//new Vertice ( -0.84000,-1.50000, 2.40000 );
  // Ponto* p223 = p120;//new Vertice ( -1.50000,-0.84000, 2.40000 );
  PointAdaptive* p233 = p130;  // new Vertice ( -1.50000, 0.00000, 2.40000 );

  CurveAdaptive* patch2_c1 =
      new CurveAdaptiveParametricBezier(*p200, *p210, *p220, *p230);
  CurveAdaptive* patch2_c2 =
      new CurveAdaptiveParametricBezier(*p230, *p231, *p232, *p233);
  CurveAdaptive* patch2_c3 =
      patch1_c1;  // new CurvParamBezier ( *p133, *p213, *p223, *p233 );
  CurveAdaptive* patch2_c4 =
      new CurveAdaptiveParametricBezier(*p200, *p201, *p202, *p203);

  Patch* patch2 = new PatchBezier(patch2_c1, patch2_c2, patch2_c3, patch2_c4,
                                  *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  geometry->InsertCurve(patch2_c1);
  geometry->InsertCurve(patch2_c2);
  // geo->InsertCurve ( patch2_c3 );
  geometry->InsertCurve(patch2_c4);
  //======================== FIM DO PATCH 2 ==================================
  return geometry;
}

Geometry* Models3d::ModelCurvaBezier(Geometry* geometry) {
  //==============================================================================
  // Exemplo usando curva de Bezier
  //==============================================================================
  // ============================= Patch 1 ===================================

  PointAdaptive* p00 = new VertexAdaptive(1.0, -1.0, 0.0);
  PointAdaptive* p10 = new VertexAdaptive(0.5, -0.5, 0.0);
  PointAdaptive* p20 = new VertexAdaptive(0.5, 0.5, 0.0);
  PointAdaptive* p30 = new VertexAdaptive(1.0, 1.0, 0.0);

  PointAdaptive* p01 = new VertexAdaptive(0.5, -1.0, 0.0);
  PointAdaptive* p11 = new VertexAdaptive(0.5, -0.5, 0.5);
  PointAdaptive* p21 = new VertexAdaptive(0.5, 0.5, 0.0);
  PointAdaptive* p31 = new VertexAdaptive(0.5, 1.0, 0.0);

  PointAdaptive* p02 = new VertexAdaptive(-0.5, -1.0, 0.0);
  PointAdaptive* p12 = new VertexAdaptive(-0.5, -0.5, 0.0);
  PointAdaptive* p22 = new VertexAdaptive(-0.5, 0.5, 0.0);
  PointAdaptive* p32 = new VertexAdaptive(-0.5, 1.0, 0.0);

  PointAdaptive* p03 = new VertexAdaptive(-1.0, -1.0, 0.0);
  PointAdaptive* p13 = new VertexAdaptive(-1.0, -0.5, 0.0);
  PointAdaptive* p23 = new VertexAdaptive(-1.0, 0.5, 0.0);
  PointAdaptive* p33 = new VertexAdaptive(-1.0, 1.0, 0.0);

  CurveAdaptive* patch1_c1 =
      new CurveAdaptiveParametricBezier(*p00, *p10, *p20, *p30);
  CurveAdaptive* patch1_c2 =
      new CurveAdaptiveParametricBezier(*p30, *p31, *p32, *p33);
  CurveAdaptive* patch1_c3 =
      new CurveAdaptiveParametricBezier(*p03, *p13, *p23, *p33);
  CurveAdaptive* patch1_c4 =
      new CurveAdaptiveParametricBezier(*p00, *p01, *p02, *p03);

  Patch* patch1 = new PatchBezier(patch1_c1, patch1_c2, patch1_c3, patch1_c4,
                                  *p11, *p21, *p12, *p22);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //==============================================================================
  // Fim do exemplo usando curva de Bezier
  //==============================================================================
  return geometry;
}

Geometry* Models3d::ModelParaboloide(Geometry* geometry) {
  //==============================================================================
  // Exemplo do PARABOLÓIDE HIPERBÓLICO
  //==============================================================================

  //=============================== PATCH 1 ==================================
  PointAdaptive* patch1_p00 = new VertexAdaptive(-5.0, -5.0, 0.0);
  PointAdaptive* patch1_p10 = new VertexAdaptive(5.0, -5.0, 0.0);
  PointAdaptive* patch1_p01 = new VertexAdaptive(-5.0, 5.0, 0.0);
  PointAdaptive* patch1_p11 = new VertexAdaptive(5.0, 5.0, 0.0);

  VectorAdaptive* patch1_Qu00 = new VectorAdaptive(1.0, 0.0, 18.25);
  VectorAdaptive* patch1_Qu10 = new VectorAdaptive(
      patch1_Qu00->GetX(), -patch1_Qu00->GetY(), -patch1_Qu00->GetZ());
  VectorAdaptive* patch1_Qu01 = new VectorAdaptive(1.0, 0.0, 18.25);
  VectorAdaptive* patch1_Qu11 = new VectorAdaptive(
      patch1_Qu01->GetX(), -patch1_Qu01->GetY(), -patch1_Qu01->GetZ());

  VectorAdaptive* patch1_Qv00 = new VectorAdaptive(0.0, 1.0, -18.25);
  VectorAdaptive* patch1_Qv10 = new VectorAdaptive(0.0, 1.0, -18.25);
  VectorAdaptive* patch1_Qv01 = new VectorAdaptive(
      -patch1_Qv00->GetX(), patch1_Qv00->GetY(), -patch1_Qv00->GetZ());
  VectorAdaptive* patch1_Qv11 = new VectorAdaptive(
      -patch1_Qv10->GetX(), patch1_Qv10->GetY(), -patch1_Qv10->GetZ());

  VectorAdaptive* patch1_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch1_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch1_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch1_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch1_c1 = new CurveAdaptiveParametricHermite(
      *patch1_p00, *patch1_p10, *patch1_Qu00, *patch1_Qu10);
  CurveAdaptive* patch1_c2 = new CurveAdaptiveParametricHermite(
      *patch1_p10, *patch1_p11, *patch1_Qv10, *patch1_Qv11);
  CurveAdaptive* patch1_c3 = new CurveAdaptiveParametricHermite(
      *patch1_p01, *patch1_p11, *patch1_Qu01, *patch1_Qu11);
  CurveAdaptive* patch1_c4 = new CurveAdaptiveParametricHermite(
      *patch1_p00, *patch1_p01, *patch1_Qv00, *patch1_Qv01);

  Patch* patch1 =
      new PatchHermite(patch1_c1, patch1_c2, patch1_c3, patch1_c4, *patch1_tw00,
                       *patch1_tw10, *patch1_tw01, *patch1_tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);

  //======================== FIM DO PATCH 1 ===================================
  //==============================================================================
  // FIM do Exemplo do PARABOLÓIDE HIPERBÓLICO
  //==============================================================================
  return geometry;
}

Geometry* Models3d::ModelPneu(Geometry* geometry) {
  //==============================================================================
  // Exemplo do pneu
  //==============================================================================
  PointAdaptive* p00 = new VertexAdaptive(-5.0, 0.0, -5.0);
  PointAdaptive* p10 = new VertexAdaptive(5.0, 0.0, -5.0);
  PointAdaptive* p01 = new VertexAdaptive(-5.0, 0.0, 5.0);
  PointAdaptive* p11 = new VertexAdaptive(5.0, 0.0, 5.0);

  VectorAdaptive* Qu00 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* Qu10 = new VectorAdaptive(0.0, 20.0, 0.0);
  VectorAdaptive* Qu01 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* Qu11 = new VectorAdaptive(0.0, 20.0, 0.0);

  VectorAdaptive* Qv00 = new VectorAdaptive(-15.0, 0.0, -15.0);
  VectorAdaptive* Qv10 = new VectorAdaptive(15.0, 0.0, -15.0);
  VectorAdaptive* Qv01 = new VectorAdaptive(15.0, 0.0, -15.0);
  VectorAdaptive* Qv11 = new VectorAdaptive(-15.0, 0.0, -15.0);

  CurveAdaptive* c1 =
      new CurveAdaptiveParametricHermite(*p00, *p10, *Qu00, *Qu10);
  CurveAdaptive* c2 =
      new CurveAdaptiveParametricHermite(*p10, *p11, *Qv10, *Qv11);
  CurveAdaptive* c3 =
      new CurveAdaptiveParametricHermite(*p01, *p11, *Qu01, *Qu11);
  CurveAdaptive* c4 =
      new CurveAdaptiveParametricHermite(*p00, *p01, *Qv00, *Qv01);
  CurveAdaptive* c5 =
      new CurveAdaptiveParametricHermite(*p10, *p00, *Qu10, *Qu00);
  CurveAdaptive* c6 =
      new CurveAdaptiveParametricHermite(*p11, *p01, *Qu11, *Qu01);

  VectorAdaptive* tw_00 = new VectorAdaptive(0.0, 60.0, 0.0);
  VectorAdaptive* tw_10 = new VectorAdaptive(0.0, -60.0, 0.0);
  VectorAdaptive* tw_01 = new VectorAdaptive(0.0, -60.0, 0.0);
  VectorAdaptive* tw_11 = new VectorAdaptive(0.0, 60.0, 0.0);

  VectorAdaptive* tw00 = new VectorAdaptive(0.0, -60.0, 0.0);
  VectorAdaptive* tw10 = new VectorAdaptive(0.0, 60.0, 0.0);
  VectorAdaptive* tw01 = new VectorAdaptive(0.0, 60.0, 0.0);
  VectorAdaptive* tw11 = new VectorAdaptive(0.0, -60.0, 0.0);

  Patch* patch1 = new PatchHermite(c1, c2, c3, c4, *tw00, *tw10, *tw01, *tw11);
  Patch* patch2 =
      new PatchHermite(c5, c4, c6, c2, *tw_00, *tw_10, *tw_01, *tw_11);

  geometry->InsertPatch(patch1);
  geometry->InsertPatch(patch2);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  geometry->InsertCurve(c5);
  geometry->InsertCurve(c6);
  return geometry;
  //==============================================================================
  // Fim do Exemplo do pneu
  //==============================================================================
}

Geometry* Models3d::ModelLadoDescendente(Geometry* geometry) {
  //==============================================================================
  // Exemplo de uma superfície com um dos lados descendentes
  //==============================================================================
  PointAdaptive* p00 = new VertexAdaptive(-0.5, 0.0, 0.5);
  PointAdaptive* p10 = new VertexAdaptive(0.5, 0.0, 0.5);
  PointAdaptive* p01 = new VertexAdaptive(-0.5, 0.0, -0.5);
  PointAdaptive* p11 = new VertexAdaptive(0.5, -0.5, -0.5);

  VectorAdaptive* Qu00 = new VectorAdaptive(1.0, 0.0, 0.0);
  VectorAdaptive* Qu10 = new VectorAdaptive(1.0, 0.0, 0.0);
  VectorAdaptive* Qu01 = new VectorAdaptive(3.0, 0.0, 0.0);
  VectorAdaptive* Qu11 = new VectorAdaptive(1.0, 0.0, 0.0);

  VectorAdaptive* Qv00 = new VectorAdaptive(0.0, 0.0, -1.0);
  VectorAdaptive* Qv10 = new VectorAdaptive(0.0, 0.0, -3.0);
  VectorAdaptive* Qv01 = new VectorAdaptive(0.0, 0.0, -1.0);
  VectorAdaptive* Qv11 = new VectorAdaptive(0.0, 0.0, -1.0);

  VectorAdaptive* tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* c1 =
      new CurveAdaptiveParametricHermite(*p00, *p10, *Qu00, *Qu10);
  CurveAdaptive* c2 =
      new CurveAdaptiveParametricHermite(*p10, *p11, *Qv10, *Qv11);
  CurveAdaptive* c3 =
      new CurveAdaptiveParametricHermite(*p01, *p11, *Qu01, *Qu11);
  CurveAdaptive* c4 =
      new CurveAdaptiveParametricHermite(*p00, *p01, *Qv00, *Qv01);
  Patch* patch1 = new PatchHermite(c1, c2, c3, c4, *tw00, *tw10, *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);

  return geometry;

  //==============================================================================
  // FIM do Exemplo de uma superfície com um dos lados descendentes
  //==============================================================================
}

Geometry* Models3d::ModelBaseCircular(Geometry* geometry) {
  //==============================================================================
  // Exemplo da base circular
  //==============================================================================
  PointAdaptive* p00 = new VertexAdaptive(0.0, -5.0, 0.0);
  PointAdaptive* p10 = new VertexAdaptive(5.0, 0.0, 0.0);
  PointAdaptive* p11 = new VertexAdaptive(0.0, 5.0, 0.0);
  PointAdaptive* p01 = new VertexAdaptive(-5.0, 0.0, 0.0);

  VectorAdaptive* Qu00 = new VectorAdaptive(8.25, 0.0, 0.0);
  VectorAdaptive* Qu10 = new VectorAdaptive(0.0, 8.25, 0.0);
  VectorAdaptive* Qu01 = new VectorAdaptive(0.0, 8.25, 0.0);
  VectorAdaptive* Qu11 = new VectorAdaptive(8.25, 0.0, 0.0);
  VectorAdaptive* Qv00 = new VectorAdaptive(-8.25, 0.0, 0.0);
  VectorAdaptive* Qv10 = new VectorAdaptive(0.0, 8.25, 0.0);
  VectorAdaptive* Qv01 = new VectorAdaptive(0.0, 8.25, 0.0);
  VectorAdaptive* Qv11 = new VectorAdaptive(-8.25, 0.0, 0.0);

  VectorAdaptive* tw00 = new VectorAdaptive(0.0, 0.0, 10.0);
  VectorAdaptive* tw10 = new VectorAdaptive(0.0, 0.0, -10.0);
  VectorAdaptive* tw01 = new VectorAdaptive(0.0, 0.0, -10.0);
  VectorAdaptive* tw11 = new VectorAdaptive(0.0, 0.0, 10.0);

  CurveAdaptive* c1 =
      new CurveAdaptiveParametricHermite(*p00, *p10, *Qu00, *Qu10);
  CurveAdaptive* c2 =
      new CurveAdaptiveParametricHermite(*p10, *p11, *Qv10, *Qv11);
  CurveAdaptive* c3 =
      new CurveAdaptiveParametricHermite(*p01, *p11, *Qu01, *Qu11);
  CurveAdaptive* c4 =
      new CurveAdaptiveParametricHermite(*p00, *p01, *Qv00, *Qv01);

  Patch* patch1 = new PatchHermite(c1, c2, c3, c4, *tw00, *tw10, *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  return geometry;
  //==============================================================================
  // FIM do Exemplo da base circular
  //==============================================================================
}

Geometry* Models3d::ModelBaseQuadrada(Geometry* geometry) {
  //==============================================================================
  // Exemplo do patch com base quadrada
  //==============================================================================
  PointAdaptive* p00 = new VertexAdaptive(-0.5, 0.0, 0.5);
  PointAdaptive* p10 = new VertexAdaptive(0.5, 0.0, 0.5);
  PointAdaptive* p01 = new VertexAdaptive(-0.5, 0.0, -0.5);
  PointAdaptive* p11 = new VertexAdaptive(0.5, 0.0, -0.5);

  VectorAdaptive* Qu00 = new VectorAdaptive(1.0, 0.0, 0.0);
  VectorAdaptive* Qu10 = new VectorAdaptive(1.0, 0.0, 0.0);
  VectorAdaptive* Qu01 = new VectorAdaptive(1.0, 0.0, 0.0);
  VectorAdaptive* Qu11 = new VectorAdaptive(1.0, 0.0, 0.0);

  VectorAdaptive* Qv00 = new VectorAdaptive(0.0, 0.0, -1.0);
  VectorAdaptive* Qv10 = new VectorAdaptive(0.0, 0.0, -1.0);
  VectorAdaptive* Qv01 = new VectorAdaptive(0.0, 0.0, -1.0);
  VectorAdaptive* Qv11 = new VectorAdaptive(0.0, 0.0, -1.0);

  VectorAdaptive* tw00 = new VectorAdaptive(0.0, 20.0, 0.0);
  VectorAdaptive* tw10 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* tw01 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* tw11 = new VectorAdaptive(0.0, 20.0, 0.0);

  CurveAdaptive* c1 =
      new CurveAdaptiveParametricHermite(*p00, *p10, *Qu00, *Qu10);
  CurveAdaptive* c2 =
      new CurveAdaptiveParametricHermite(*p10, *p11, *Qv10, *Qv11);
  CurveAdaptive* c3 =
      new CurveAdaptiveParametricHermite(*p01, *p11, *Qu01, *Qu11);
  CurveAdaptive* c4 =
      new CurveAdaptiveParametricHermite(*p00, *p01, *Qv00, *Qv01);

  Patch* patch1 = new PatchHermite(c1, c2, c3, c4, *tw00, *tw10, *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  return geometry;
  //==============================================================================
  // FIM do exemplo do patch com base quadrada
  //==============================================================================
}

Geometry* Models3d::ModelBordaCurva(Geometry* geometry) {
  //==============================================================================
  // Exemplo do patch com borda curva
  //==============================================================================
  PointAdaptive* p00 = new VertexAdaptive(-5.0, -5.0, 0.0);
  PointAdaptive* p10 = new VertexAdaptive(5.0, -5.0, 0.0);
  PointAdaptive* p01 = new VertexAdaptive(-5.0, 5.0, 0.0);
  PointAdaptive* p11 = new VertexAdaptive(5.0, 5.0, 0.0);

  VectorAdaptive* Qu00 = new VectorAdaptive(10.0, 10.0, 0.0);
  VectorAdaptive* Qu10 = new VectorAdaptive(10.0, 10.0, 0.0);
  VectorAdaptive* Qu01 = new VectorAdaptive(10.0, 10.0, 0.0);
  VectorAdaptive* Qu11 = new VectorAdaptive(10.0, 10.0, 0.0);

  VectorAdaptive* Qv00 = new VectorAdaptive(-10.0, 10.0, 0.0);
  VectorAdaptive* Qv10 = new VectorAdaptive(-10.0, 10.0, 0.0);
  VectorAdaptive* Qv01 = new VectorAdaptive(-10.0, 10.0, 0.0);
  VectorAdaptive* Qv11 = new VectorAdaptive(-10.0, 10.0, 0.0);

  VectorAdaptive* tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* c1 =
      new CurveAdaptiveParametricHermite(*p00, *p10, *Qu00, *Qu10);
  CurveAdaptive* c2 =
      new CurveAdaptiveParametricHermite(*p10, *p11, *Qv10, *Qv11);
  CurveAdaptive* c3 =
      new CurveAdaptiveParametricHermite(*p01, *p11, *Qu01, *Qu11);
  CurveAdaptive* c4 =
      new CurveAdaptiveParametricHermite(*p00, *p01, *Qv00, *Qv01);

  Patch* patch1 = new PatchHermite(c1, c2, c3, c4, *tw00, *tw10, *tw01, *tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  return geometry;
  //==============================================================================
  // FIM do exemplo do patch com borda curva
  //==============================================================================
}

Geometry* Models3d::ModelDoisPatches(Geometry* geometry) {
  //==============================================================================
  // Exemplo de dois patches vizinhos
  //==============================================================================
  PointAdaptive* p100 = new VertexAdaptive(-1.0, 0.0, 0.5);
  PointAdaptive* p110 = new VertexAdaptive(0.0, 0.0, 0.5);
  PointAdaptive* p101 = new VertexAdaptive(-1.0, 0.0, -0.5);
  PointAdaptive* p111 = new VertexAdaptive(0.0, 0.0, -0.5);

  PointAdaptive* p200 = new VertexAdaptive(0.0, 0.0, 0.5);
  PointAdaptive* p210 = new VertexAdaptive(1.0, 0.0, 0.5);
  PointAdaptive* p201 = new VertexAdaptive(0.0, 0.0, -0.5);
  PointAdaptive* p211 = new VertexAdaptive(1.0, 0.0, -0.5);

  VectorAdaptive* Qu00 = new VectorAdaptive(0.1, 0.0, 0.0);
  VectorAdaptive* Qu10 = new VectorAdaptive(0.1, 0.0, 0.0);
  VectorAdaptive* Qu01 = new VectorAdaptive(0.1, 0.0, 0.0);
  VectorAdaptive* Qu11 = new VectorAdaptive(0.1, 0.0, 0.0);

  VectorAdaptive* Qv00 = new VectorAdaptive(0.0, 0.0, -0.1);
  VectorAdaptive* Qv10 = new VectorAdaptive(0.0, 0.0, -0.1);
  VectorAdaptive* Qv01 = new VectorAdaptive(0.0, 0.0, -0.1);
  VectorAdaptive* Qv11 = new VectorAdaptive(0.0, 0.0, -0.1);

  VectorAdaptive* tw100 = new VectorAdaptive(0.0, 20.0, 0.0);
  VectorAdaptive* tw110 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* tw101 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* tw111 = new VectorAdaptive(0.0, 20.0, 0.0);

  VectorAdaptive* tw200 = new VectorAdaptive(0.0, 20.0, 0.0);
  VectorAdaptive* tw210 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* tw201 = new VectorAdaptive(0.0, -20.0, 0.0);
  VectorAdaptive* tw211 = new VectorAdaptive(0.0, 20.0, 0.0);

  CurveAdaptive* c1 =
      new CurveAdaptiveParametricHermite(*p100, *p110, *Qu00, *Qu10);
  CurveAdaptive* c2 =
      new CurveAdaptiveParametricHermite(*p110, *p111, *Qv10, *Qv11);
  CurveAdaptive* c3 =
      new CurveAdaptiveParametricHermite(*p101, *p111, *Qu01, *Qu11);
  CurveAdaptive* c4 =
      new CurveAdaptiveParametricHermite(*p100, *p101, *Qv00, *Qv01);

  CurveAdaptive* c5 =
      new CurveAdaptiveParametricHermite(*p200, *p210, *Qu00, *Qu10);
  CurveAdaptive* c6 =
      new CurveAdaptiveParametricHermite(*p210, *p211, *Qv10, *Qv11);
  CurveAdaptive* c7 =
      new CurveAdaptiveParametricHermite(*p201, *p211, *Qu01, *Qu11);
  // Curva* c8 = new CurvParamHermite ( *p200, *p201, *Qv00, *Qv01 );

  Patch* patch1 =
      new PatchHermite(c1, c2, c3, c4, *tw100, *tw110, *tw101, *tw111);
  Patch* patch2 =
      new PatchHermite(c5, c6, c7, c2, *tw200, *tw210, *tw201, *tw211);

  geometry->InsertPatch(patch1);
  geometry->InsertPatch(patch2);
  geometry->InsertCurve(c1);
  geometry->InsertCurve(c2);
  geometry->InsertCurve(c3);
  geometry->InsertCurve(c4);
  geometry->InsertCurve(c5);
  geometry->InsertCurve(c6);
  geometry->InsertCurve(c7);
  return geometry;
  //==============================================================================
  // Fim do Exemplo de dois patches vizinhos
  //==============================================================================
}

Geometry* Models3d::ModelNariz(Geometry* geometry) {
  //==============================================================================
  // Exemplo do nariz
  //==============================================================================
  //=============================== PATCH 1 ===================================
  PointAdaptive* patch1_p00 = new VertexAdaptive(-8.00, 2.5, 0.0);
  PointAdaptive* patch1_p10 = new VertexAdaptive(-2.50, 0.0, 0.0);
  PointAdaptive* patch1_p01 = new VertexAdaptive(-7.00, 2.5, 5.0);
  PointAdaptive* patch1_p11 = new VertexAdaptive(-3.50, 0.0, 7.5);

  VectorAdaptive* patch1_Qu00 = new VectorAdaptive(5.5, -2.5, 18.0);
  VectorAdaptive* patch1_Qu10 = new VectorAdaptive(
      patch1_Qu00->GetX(), patch1_Qu00->GetY(), -patch1_Qu00->GetZ());
  VectorAdaptive* patch1_Qu01 = new VectorAdaptive(3.5, -2.5, 2.5);
  VectorAdaptive* patch1_Qu11 = new VectorAdaptive(
      patch1_Qu01->GetX(), patch1_Qu01->GetY(), patch1_Qu01->GetZ());

  VectorAdaptive* patch1_Qv00 = new VectorAdaptive(-7.5, 2.5, 0.0);
  VectorAdaptive* patch1_Qv10 = new VectorAdaptive(3.5, -5.0, 7.5);
  VectorAdaptive* patch1_Qv01 = new VectorAdaptive(5.5, 2.5, 0.0);
  VectorAdaptive* patch1_Qv11 = new VectorAdaptive(
      -patch1_Qv10->GetX(), -patch1_Qv10->GetY(), patch1_Qv10->GetZ());

  VectorAdaptive* patch1_tw00 = new VectorAdaptive(0.0, -10.0, 0.0);
  VectorAdaptive* patch1_tw10 = new VectorAdaptive(0.0, 10.0, 0.0);
  VectorAdaptive* patch1_tw01 = new VectorAdaptive(0.0, 10.0, 0.0);
  VectorAdaptive* patch1_tw11 = new VectorAdaptive(0.0, -10.0, 0.0);

  CurveAdaptive* patch1_c1 = new CurveAdaptiveParametricHermite(
      *patch1_p00, *patch1_p10, *patch1_Qu00, *patch1_Qu10);
  CurveAdaptive* patch1_c2 = new CurveAdaptiveParametricHermite(
      *patch1_p10, *patch1_p11, *patch1_Qv10, *patch1_Qv11);
  CurveAdaptive* patch1_c3 = new CurveAdaptiveParametricHermite(
      *patch1_p01, *patch1_p11, *patch1_Qu01, *patch1_Qu11);
  CurveAdaptive* patch1_c4 = new CurveAdaptiveParametricHermite(
      *patch1_p00, *patch1_p01, *patch1_Qv00, *patch1_Qv01);

  Patch* patch1 =
      new PatchHermite(patch1_c1, patch1_c2, patch1_c3, patch1_c4, *patch1_tw00,
                       *patch1_tw10, *patch1_tw01, *patch1_tw11);

  geometry->InsertPatch(patch1);
  geometry->InsertCurve(patch1_c1);
  geometry->InsertCurve(patch1_c2);
  geometry->InsertCurve(patch1_c3);
  geometry->InsertCurve(patch1_c4);
  //======================== FIM DO PATCH 1 ===================================

  //=============================== PATCH 2 ===================================
  PointAdaptive* patch2_p00 = patch1_p10;
  PointAdaptive* patch2_p10 = new VertexAdaptive(
      -patch1_p10->GetX(), patch1_p10->GetY(), patch1_p10->GetZ());
  PointAdaptive* patch2_p01 = patch1_p11;
  PointAdaptive* patch2_p11 = new VertexAdaptive(
      -patch1_p11->GetX(), patch1_p11->GetY(), patch1_p11->GetZ());

  VectorAdaptive* patch2_Qu00 = new VectorAdaptive(7.5, -2.5, 0.0);
  VectorAdaptive* patch2_Qu10 = new VectorAdaptive(7.5, 2.5, 0.0);
  VectorAdaptive* patch2_Qu01 = new VectorAdaptive(7.0, -5.0, 5.0);
  VectorAdaptive* patch2_Qu11 = new VectorAdaptive(7.0, 5.0, -5.0);

  // VectorAdaptive* patch2_Qv00 = new VectorAdaptive ( *patch2_p00, *patch2_p01
  // );
  VectorAdaptive* patch2_Qv10 = new VectorAdaptive(
      -patch1_Qv10->GetX(), patch1_Qv10->GetY(), patch1_Qv10->GetZ());
  // VectorAdaptive* patch2_Qv01 = new VectorAdaptive ( *patch2_p00, *patch2_p01
  // );
  VectorAdaptive* patch2_Qv11 = new VectorAdaptive(
      -patch1_Qv11->GetX(), patch1_Qv11->GetY(), patch1_Qv11->GetZ());

  VectorAdaptive* patch2_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch2_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch2_tw01 = new VectorAdaptive(0.0, 10.0, 0.0);
  VectorAdaptive* patch2_tw11 = new VectorAdaptive(0.0, -10.0, 0.0);

  CurveAdaptive* patch2_c1 = new CurveAdaptiveParametricHermite(
      *patch2_p00, *patch2_p10, *patch2_Qu00, *patch2_Qu10);
  CurveAdaptive* patch2_c2 = new CurveAdaptiveParametricHermite(
      *patch2_p10, *patch2_p11, *patch2_Qv10, *patch2_Qv11);
  CurveAdaptive* patch2_c3 = new CurveAdaptiveParametricHermite(
      *patch2_p01, *patch2_p11, *patch2_Qu01, *patch2_Qu11);
  CurveAdaptive* patch2_c4 = patch1_c2;

  Patch* patch2 =
      new PatchHermite(patch2_c1, patch2_c2, patch2_c3, patch2_c4, *patch2_tw00,
                       *patch2_tw10, *patch2_tw01, *patch2_tw11);

  geometry->InsertPatch(patch2);
  geometry->InsertCurve(patch2_c1);
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  // geo->InsertCurve ( patch2_c4 );
  //======================== FIM DO PATCH 2 ===================================

  //=============================== PATCH 3 ===================================
  PointAdaptive* patch3_p00 = patch2_p10;
  PointAdaptive* patch3_p10 = new VertexAdaptive(
      -patch1_p00->GetX(), patch1_p00->GetY(), patch1_p00->GetZ());
  PointAdaptive* patch3_p01 = patch2_p11;
  PointAdaptive* patch3_p11 = new VertexAdaptive(
      -patch1_p01->GetX(), patch1_p01->GetY(), patch1_p01->GetZ());

  VectorAdaptive* patch3_Qu00 =
      new VectorAdaptive(-(-(*patch1_Qu10)).GetX(), (-(*patch1_Qu10)).GetY(),
                         (-(*patch1_Qu10)).GetZ());
  VectorAdaptive* patch3_Qu10 =
      new VectorAdaptive(-(-(*patch1_Qu00)).GetX(), (-(*patch1_Qu00)).GetY(),
                         (-(*patch1_Qu00)).GetZ());
  VectorAdaptive* patch3_Qu01 =
      new VectorAdaptive(-(-(*patch1_Qu11)).GetX(), (-(*patch1_Qu11)).GetY(),
                         (-(*patch1_Qu11)).GetZ());
  VectorAdaptive* patch3_Qu11 =
      new VectorAdaptive(-(-(*patch1_Qu01)).GetX(), (-(*patch1_Qu01)).GetY(),
                         (-(*patch1_Qu01)).GetZ());

  // VectorAdaptive* patch3_Qv00 = new VectorAdaptive ( -patch1_Qv10->x,
  // patch1_Qv10->GetY(), patch1_Qv10->GetZ() );
  VectorAdaptive* patch3_Qv10 = new VectorAdaptive(
      -patch1_Qv00->GetX(), patch1_Qv00->GetY(), patch1_Qv00->GetZ());
  // VectorAdaptive* patch3_Qv01 = new VectorAdaptive ( -patch1_Qv11->x,
  // patch1_Qv11->GetY(), patch1_Qv11->GetZ() );
  VectorAdaptive* patch3_Qv11 = new VectorAdaptive(
      -patch1_Qv01->GetX(), patch1_Qv01->GetY(), patch1_Qv01->GetZ());

  VectorAdaptive* patch3_tw00 = new VectorAdaptive(
      patch1_tw00->GetX(), patch1_tw00->GetY(), patch1_tw00->GetZ());
  VectorAdaptive* patch3_tw10 = new VectorAdaptive(
      patch1_tw10->GetX(), patch1_tw10->GetY(), patch1_tw10->GetZ());
  VectorAdaptive* patch3_tw01 = new VectorAdaptive(
      patch1_tw01->GetX(), patch1_tw01->GetY(), patch1_tw01->GetZ());
  VectorAdaptive* patch3_tw11 = new VectorAdaptive(
      patch1_tw11->GetX(), patch1_tw11->GetY(), patch1_tw11->GetZ());

  CurveAdaptive* patch3_c1 = new CurveAdaptiveParametricHermite(
      *patch3_p00, *patch3_p10, *patch3_Qu00, *patch3_Qu10);
  CurveAdaptive* patch3_c2 = new CurveAdaptiveParametricHermite(
      *patch3_p10, *patch3_p11, *patch3_Qv10, *patch3_Qv11);
  CurveAdaptive* patch3_c3 = new CurveAdaptiveParametricHermite(
      *patch3_p01, *patch3_p11, *patch3_Qu01, *patch3_Qu11);
  CurveAdaptive* patch3_c4 = patch2_c2;

  Patch* patch3 =
      new PatchHermite(patch3_c1, patch3_c2, patch3_c3, patch3_c4, *patch3_tw00,
                       *patch3_tw10, *patch3_tw01, *patch3_tw11);

  geometry->InsertPatch(patch3);
  geometry->InsertCurve(patch3_c1);
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  // geo->InsertCurve ( patch3_c4 );
  //======================== FIM DO PATCH 3 ===================================

  //=============================== PATCH 4 ===================================
  PointAdaptive* patch4_p00 = patch1_p01;
  PointAdaptive* patch4_p10 = patch1_p11;
  PointAdaptive* patch4_p01 = new VertexAdaptive(-6.00, 5.0, 2.5);
  PointAdaptive* patch4_p11 = new VertexAdaptive(-3.50, 4.0, 7.5);

  // VectorAdaptive* patch4_Qu00 = new VectorAdaptive ( *patch4_p00, *patch4_p10
  // ); VectorAdaptive* patch4_Qu10 = new VectorAdaptive ( *patch4_p00,
  // *patch4_p10 );
  VectorAdaptive* patch4_Qu01 = new VectorAdaptive(*patch4_p01, *patch4_p11);
  VectorAdaptive* patch4_Qu11 = new VectorAdaptive(*patch4_p01, *patch4_p11);

  VectorAdaptive* patch4_Qv00 = new VectorAdaptive(*patch4_p00, *patch4_p01);
  VectorAdaptive* patch4_Qv10 = new VectorAdaptive(*patch4_p10, *patch4_p11);
  VectorAdaptive* patch4_Qv01 = new VectorAdaptive(*patch4_p00, *patch4_p01);
  VectorAdaptive* patch4_Qv11 = new VectorAdaptive(*patch4_p10, *patch4_p11);

  VectorAdaptive* patch4_tw00 = new VectorAdaptive(0.0, 0.0, 10.0);
  VectorAdaptive* patch4_tw10 = new VectorAdaptive(0.0, 0.0, -10.0);
  VectorAdaptive* patch4_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch4_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch4_c1 = patch1_c3;
  CurveAdaptive* patch4_c2 = new CurveAdaptiveParametricHermite(
      *patch4_p10, *patch4_p11, *patch4_Qv10, *patch4_Qv11);
  CurveAdaptive* patch4_c3 = new CurveAdaptiveParametricHermite(
      *patch4_p01, *patch4_p11, *patch4_Qu01, *patch4_Qu11);
  CurveAdaptive* patch4_c4 = new CurveAdaptiveParametricHermite(
      *patch4_p00, *patch4_p01, *patch4_Qv00, *patch4_Qv01);

  Patch* patch4 =
      new PatchHermite(patch4_c1, patch4_c2, patch4_c3, patch4_c4, *patch4_tw00,
                       *patch4_tw10, *patch4_tw01, *patch4_tw11);

  geometry->InsertPatch(patch4);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch4_c2);
  geometry->InsertCurve(patch4_c3);
  geometry->InsertCurve(patch4_c4);
  //======================== FIM DO PATCH 4 ===================================

  //=============================== PATCH 5 ===================================
  // Ponto* patch5_p00 = patch1_p11;
  PointAdaptive* patch5_p10 = patch2_p11;
  PointAdaptive* patch5_p01 = patch4_p11;
  PointAdaptive* patch5_p11 = new VertexAdaptive(
      -patch4_p11->GetX(), patch4_p11->GetY(), patch4_p11->GetZ());

  // VectorAdaptive* patch5_Qu00 = new VectorAdaptive ( *patch5_p00, *patch5_p10
  // ); VectorAdaptive* patch5_Qu10 = new VectorAdaptive ( *patch5_p00,
  // *patch5_p10 );
  VectorAdaptive* patch5_Qu01 = new VectorAdaptive(
      patch4_Qu11->GetX(), patch4_Qu11->GetY(), patch4_Qu11->GetZ());
  VectorAdaptive* patch5_Qu11 = new VectorAdaptive(
      patch5_Qu01->GetX(), -patch5_Qu01->GetY(), -patch5_Qu01->GetZ());

  // VectorAdaptive* patch5_Qv00 = new VectorAdaptive ( *patch5_p00, *patch5_p01
  // );
  VectorAdaptive* patch5_Qv10 = new VectorAdaptive(
      -patch4_Qv10->GetX(), patch4_Qv10->GetY(), patch4_Qv10->GetZ());
  // VectorAdaptive* patch5_Qv01 = new VectorAdaptive ( *patch5_p00, *patch5_p01
  // );
  VectorAdaptive* patch5_Qv11 = new VectorAdaptive(
      -patch4_Qv11->GetX(), patch4_Qv11->GetY(), patch4_Qv11->GetZ());

  VectorAdaptive* patch5_tw00 = new VectorAdaptive(0.0, 0.0, 20.0);
  VectorAdaptive* patch5_tw10 = new VectorAdaptive(0.0, 0.0, -20.0);
  VectorAdaptive* patch5_tw01 = new VectorAdaptive(0.0, 0.0, -5.0);
  VectorAdaptive* patch5_tw11 = new VectorAdaptive(0.0, 0.0, 5.0);

  CurveAdaptive* patch5_c1 = patch2_c3;
  CurveAdaptive* patch5_c2 = new CurveAdaptiveParametricHermite(
      *patch5_p10, *patch5_p11, *patch5_Qv10, *patch5_Qv11);
  CurveAdaptive* patch5_c3 = new CurveAdaptiveParametricHermite(
      *patch5_p01, *patch5_p11, *patch5_Qu01, *patch5_Qu11);
  CurveAdaptive* patch5_c4 = patch4_c2;

  Patch* patch5 =
      new PatchHermite(patch5_c1, patch5_c2, patch5_c3, patch5_c4, *patch5_tw00,
                       *patch5_tw10, *patch5_tw01, *patch5_tw11);

  geometry->InsertPatch(patch5);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch5_c2);
  geometry->InsertCurve(patch5_c3);
  // geo->InsertCurve ( patch5_c4 );
  //======================== FIM DO PATCH 5 ===================================

  //=============================== PATCH 6 ===================================
  // Ponto* patch6_p00 = patch2_p11;
  PointAdaptive* patch6_p10 = patch3_p11;
  PointAdaptive* patch6_p01 = patch5_p11;
  PointAdaptive* patch6_p11 = new VertexAdaptive(
      -patch4_p01->GetX(), patch4_p01->GetY(), patch4_p01->GetZ());

  // VectorAdaptive* patch6_Qu00 = new VectorAdaptive ( patch5_Qu11->x,
  // patch5_Qu11->GetY(), patch5_Qu11->GetZ() ); VectorAdaptive* patch6_Qu10 =
  // new VectorAdaptive (
  // patch6_Qu00->x,-patch6_Qu00->GetY(),-patch6_Qu00->GetZ() );
  VectorAdaptive* patch6_Qu01 = new VectorAdaptive(
      patch4_Qu11->GetX(), -patch4_Qu11->GetY(), -patch4_Qu11->GetZ());
  VectorAdaptive* patch6_Qu11 = new VectorAdaptive(
      patch4_Qu01->GetX(), -patch4_Qu01->GetY(), -patch4_Qu01->GetZ());

  // VectorAdaptive* patch6_Qv00 = new VectorAdaptive ( *patch6_p00, *patch6_p01
  // );
  VectorAdaptive* patch6_Qv10 = new VectorAdaptive(
      -patch4_Qv00->GetX(), patch4_Qv00->GetY(), patch4_Qv00->GetZ());
  // VectorAdaptive* patch6_Qv01 = new VectorAdaptive ( *patch6_p00, *patch6_p01
  // );
  VectorAdaptive* patch6_Qv11 = new VectorAdaptive(
      -patch4_Qv01->GetX(), patch4_Qv01->GetY(), patch4_Qv01->GetZ());

  VectorAdaptive* patch6_tw00 = new VectorAdaptive(0.0, 0.0, 10.0);
  VectorAdaptive* patch6_tw10 = new VectorAdaptive(0.0, 0.0, -10.0);
  VectorAdaptive* patch6_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch6_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch6_c1 = patch3_c3;
  CurveAdaptive* patch6_c2 = new CurveAdaptiveParametricHermite(
      *patch6_p10, *patch6_p11, *patch6_Qv10, *patch6_Qv11);
  CurveAdaptive* patch6_c3 = new CurveAdaptiveParametricHermite(
      *patch6_p01, *patch6_p11, *patch6_Qu01, *patch6_Qu11);
  CurveAdaptive* patch6_c4 = patch5_c2;

  Patch* patch6 =
      new PatchHermite(patch6_c1, patch6_c2, patch6_c3, patch6_c4, *patch6_tw00,
                       *patch6_tw10, *patch6_tw01, *patch6_tw11);

  geometry->InsertPatch(patch6);
  // geo->InsertCurve ( patch4_c1 );
  geometry->InsertCurve(patch6_c2);
  geometry->InsertCurve(patch6_c3);
  // geo->InsertCurve ( patch5_c4 );
  //======================== FIM DO PATCH 6 ===================================

  //=============================== PATCH 8 ===================================
  PointAdaptive* patch8_p00 = patch4_p11;
  PointAdaptive* patch8_p10 = patch5_p11;
  PointAdaptive* patch8_p01 = new VertexAdaptive(-2.5, 7.5, 6.5);
  PointAdaptive* patch8_p11 = new VertexAdaptive(2.5, 7.5, 6.5);

  // VectorAdaptive* patch8_Qu00 = new VectorAdaptive ( patch4_Qu11->x,
  // patch4_Qu11->GetY(), patch4_Qu11->GetZ() ); VectorAdaptive* patch8_Qu10 =
  // new VectorAdaptive (
  // patch5_Qu01->x,-patch5_Qu01->GetY(),-patch5_Qu01->GetZ() );
  VectorAdaptive* patch8_Qu01 = new VectorAdaptive(5.0, -3.5, 3.5);
  VectorAdaptive* patch8_Qu11 = new VectorAdaptive(
      patch8_Qu01->GetX(), -patch8_Qu01->GetY(), -patch8_Qu01->GetZ());

  VectorAdaptive* patch8_Qv00 = new VectorAdaptive(*patch5_Qv10);
  VectorAdaptive* patch8_Qv10 = new VectorAdaptive(*patch5_Qv11);
  VectorAdaptive* patch8_Qv01 = new VectorAdaptive(*patch8_p00, *patch8_p01);
  VectorAdaptive* patch8_Qv11 = new VectorAdaptive(*patch8_p10, *patch8_p11);

  VectorAdaptive* patch8_tw00 = new VectorAdaptive(0.0, 0.0, -8.0);
  VectorAdaptive* patch8_tw10 = new VectorAdaptive(0.0, 0.0, 8.0);
  VectorAdaptive* patch8_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch8_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch8_c1 = patch5_c3;
  CurveAdaptive* patch8_c2 = new CurveAdaptiveParametricHermite(
      *patch8_p10, *patch8_p11, *patch8_Qv10, *patch8_Qv11);
  CurveAdaptive* patch8_c3 = new CurveAdaptiveParametricHermite(
      *patch8_p01, *patch8_p11, *patch8_Qu01, *patch8_Qu11);
  CurveAdaptive* patch8_c4 = new CurveAdaptiveParametricHermite(
      *patch8_p00, *patch8_p01, *patch8_Qv00, *patch8_Qv01);

  Patch* patch8 =
      new PatchHermite(patch8_c1, patch8_c2, patch8_c3, patch8_c4, *patch8_tw00,
                       *patch8_tw10, *patch8_tw01, *patch8_tw11);

  geometry->InsertPatch(patch8);
  // geo->InsertCurve ( patch8_c1 );
  geometry->InsertCurve(patch8_c2);
  geometry->InsertCurve(patch8_c3);
  geometry->InsertCurve(patch8_c4);
  //======================== FIM DO PATCH 8 ===================================

  //=============================== PATCH 10 ==================================
  PointAdaptive* patch10_p00 = patch8_p01;
  PointAdaptive* patch10_p10 = patch8_p11;
  PointAdaptive* patch10_p01 = new VertexAdaptive(-2.0, 22.5, 2.5);
  PointAdaptive* patch10_p11 = new VertexAdaptive(
      -patch10_p01->GetX(), patch10_p01->GetY(), patch10_p01->GetZ());

  // VectorAdaptive* patch10_Qu00 = new VectorAdaptive ( *patch8_Qu01 );
  // VectorAdaptive* patch10_Qu10 = new VectorAdaptive ( *patch8_Qu11 );
  VectorAdaptive* patch10_Qu01 = new VectorAdaptive(1.0, 2.5, 3.5);
  VectorAdaptive* patch10_Qu11 = new VectorAdaptive(
      patch10_Qu01->GetX(), -patch10_Qu01->GetY(), -patch10_Qu01->GetZ());

  VectorAdaptive* patch10_Qv00 = new VectorAdaptive(*patch8_Qv01);
  VectorAdaptive* patch10_Qv10 = new VectorAdaptive(*patch8_Qv11);
  VectorAdaptive* patch10_Qv01 = new VectorAdaptive(-1.0, 2.5, 0.0);
  VectorAdaptive* patch10_Qv11 = new VectorAdaptive(
      -patch10_Qv01->GetX(), patch10_Qv01->GetY(), patch10_Qv01->GetZ());

  VectorAdaptive* patch10_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch10_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch10_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch10_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch10_c1 = patch8_c3;
  CurveAdaptive* patch10_c2 = new CurveAdaptiveParametricHermite(
      *patch10_p10, *patch10_p11, *patch10_Qv10, *patch10_Qv11);
  CurveAdaptive* patch10_c3 = new CurveAdaptiveParametricHermite(
      *patch10_p01, *patch10_p11, *patch10_Qu01, *patch10_Qu11);
  CurveAdaptive* patch10_c4 = new CurveAdaptiveParametricHermite(
      *patch10_p00, *patch10_p01, *patch10_Qv00, *patch10_Qv01);

  Patch* patch10 = new PatchHermite(patch10_c1, patch10_c2, patch10_c3,
                                    patch10_c4, *patch10_tw00, *patch10_tw10,
                                    *patch10_tw01, *patch10_tw11);

  geometry->InsertPatch(patch10);
  // geo->InsertCurve ( patch10_c1 );
  geometry->InsertCurve(patch10_c2);
  geometry->InsertCurve(patch10_c3);
  geometry->InsertCurve(patch10_c4);
  //======================== FIM DO PATCH 10 ==================================

  //=============================== PATCH 7 ===================================
  PointAdaptive* patch7_p00 = patch4_p01;
  // Ponto* patch7_p10 = patch4_p11;
  PointAdaptive* patch7_p01 = patch10_p01;
  PointAdaptive* patch7_p11 = patch10_p00;

  // VectorAdaptive* patch7_Qu00 = new VectorAdaptive ( *patch4_Qu01  );
  // VectorAdaptive* patch7_Qu10 = new VectorAdaptive ( *patch4_Qu11  );
  VectorAdaptive* patch7_Qu01 = new VectorAdaptive(-(*patch10_Qv01));
  VectorAdaptive* patch7_Qu11 = new VectorAdaptive(-(*patch10_Qv00));

  VectorAdaptive* patch7_Qv00 = new VectorAdaptive(5.0, 1.5, -8.0);
  // VectorAdaptive* patch7_Qv10 = new VectorAdaptive ( *patch8_Qv00 );
  VectorAdaptive* patch7_Qv01 = new VectorAdaptive(
      -patch7_Qv00->GetX(), patch7_Qv00->GetY(), -patch7_Qv00->GetZ());
  // VectorAdaptive* patch7_Qv11 = new VectorAdaptive ( *patch8_Qv00 );

  VectorAdaptive* patch7_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch7_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch7_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch7_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch7_c1 = patch4_c3;
  CurveAdaptive* patch7_c2 = patch8_c4;
  CurveAdaptive* patch7_c3 = new CurveAdaptiveParametricHermite(
      *patch7_p01, *patch7_p11, *patch7_Qu01, *patch7_Qu11);
  CurveAdaptive* patch7_c4 = new CurveAdaptiveParametricHermite(
      *patch7_p00, *patch7_p01, *patch7_Qv00, *patch7_Qv01);

  Patch* patch7 =
      new PatchHermite(patch7_c1, patch7_c2, patch7_c3, patch7_c4, *patch7_tw00,
                       *patch7_tw10, *patch7_tw01, *patch7_tw11);

  geometry->InsertPatch(patch7);
  // geo->InsertCurve ( patch7_c1 );
  // geo->InsertCurve ( patch7_c2 );
  geometry->InsertCurve(patch7_c3);
  geometry->InsertCurve(patch7_c4);
  //======================== FIM DO PATCH 7 ===================================

  //=============================== PATCH 9 ===================================
  // Ponto* patch9_p00 = patch6_p01;
  PointAdaptive* patch9_p10 = patch6_p11;
  // Ponto* patch9_p01 = patch8_p11;
  PointAdaptive* patch9_p11 = patch10_p11;

  // VectorAdaptive* patch9_Qu00 = new VectorAdaptive ( *patch8_Qu10 );
  // VectorAdaptive* patch9_Qu10 = new VectorAdaptive ( *patch9_p00, *patch9_p10
  // ); VectorAdaptive* patch9_Qu01 = new VectorAdaptive ( *patch9_p01,
  // *patch9_p11 ); VectorAdaptive* patch9_Qu11 = new VectorAdaptive (
  // *patch9_p01, *patch9_p11 );

  // VectorAdaptive* patch9_Qv00 = new VectorAdaptive ( *patch9_p00, *patch9_p01
  // );
  VectorAdaptive* patch9_Qv10 = new VectorAdaptive(
      -patch7_Qv00->GetX(), patch7_Qv00->GetY(), patch7_Qv00->GetZ());
  // VectorAdaptive* patch9_Qv01 = new VectorAdaptive ( *patch9_p00, *patch9_p01
  // );
  VectorAdaptive* patch9_Qv11 = new VectorAdaptive(
      -patch9_Qv10->GetX(), patch9_Qv10->GetY(), -patch9_Qv10->GetZ());

  VectorAdaptive* patch9_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch9_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch9_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch9_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch9_c1 = patch6_c3;
  CurveAdaptive* patch9_c2 = new CurveAdaptiveParametricHermite(
      *patch9_p10, *patch9_p11, *patch9_Qv10, *patch9_Qv11);
  CurveAdaptive* patch9_c3 = patch10_c2;
  CurveAdaptive* patch9_c4 = patch8_c2;

  Patch* patch9 =
      new PatchHermite(patch9_c1, patch9_c2, patch9_c3, patch9_c4, *patch9_tw00,
                       *patch9_tw10, *patch9_tw01, *patch9_tw11);

  geometry->InsertPatch(patch9);
  // geo->InsertCurve ( patch9_c1 );
  geometry->InsertCurve(patch9_c2);
  // geo->InsertCurve ( patch9_c3 );
  // geo->InsertCurve ( patch9_c4 );
  //======================== FIM DO PATCH 9 ===================================

  //=============================== PATCH 11 ==================================
  PointAdaptive* patch11_p00 = patch1_p00;
  // Ponto* patch11_p10 = patch1_p01;
  PointAdaptive* patch11_p01 = new VertexAdaptive(-8.0, 5.0, 0.0);
  PointAdaptive* patch11_p11 = patch4_p01;

  // VectorAdaptive* patch11_Qu00 = new VectorAdaptive ( *patch11_p00,
  // *patch11_p10 ); VectorAdaptive* patch11_Qu10 = new VectorAdaptive (
  // *patch11_p00, *patch11_p10 );
  VectorAdaptive* patch11_Qu01 = new VectorAdaptive(*patch11_p01, *patch11_p11);
  VectorAdaptive* patch11_Qu11 = new VectorAdaptive(*patch11_p01, *patch11_p11);

  VectorAdaptive* patch11_Qv00 = new VectorAdaptive(*patch11_p00, *patch11_p01);
  // VectorAdaptive* patch11_Qv10 = new VectorAdaptive ( *patch11_p10,
  // *patch11_p11 );
  VectorAdaptive* patch11_Qv01 = new VectorAdaptive(*patch11_p00, *patch11_p01);
  // VectorAdaptive* patch11_Qv11 = new VectorAdaptive ( *patch11_p10,
  // *patch11_p11 );

  VectorAdaptive* patch11_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch11_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch11_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch11_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch11_c1 = patch1_c4;
  CurveAdaptive* patch11_c2 = patch4_c4;
  CurveAdaptive* patch11_c3 = new CurveAdaptiveParametricHermite(
      *patch11_p01, *patch11_p11, *patch11_Qu01, *patch11_Qu11);
  CurveAdaptive* patch11_c4 = new CurveAdaptiveParametricHermite(
      *patch11_p00, *patch11_p01, *patch11_Qv00, *patch11_Qv01);

  Patch* patch11 = new PatchHermite(patch11_c1, patch11_c2, patch11_c3,
                                    patch11_c4, *patch11_tw00, *patch11_tw10,
                                    *patch11_tw01, *patch11_tw11);

  geometry->InsertPatch(patch11);
  // geo->InsertCurve ( patch11_c1 );
  // geo->InsertCurve ( patch11_c2 );
  geometry->InsertCurve(patch11_c3);
  geometry->InsertCurve(patch11_c4);
  //======================== FIM DO PATCH 11 ==================================

  //=============================== PATCH 12 ==================================
  PointAdaptive* patch12_p00 = patch3_p10;
  PointAdaptive* patch12_p10 = new VertexAdaptive(
      -patch11_p01->GetX(), patch11_p01->GetY(), patch11_p01->GetZ());
  // Ponto* patch12_p01 = patch3_p11;
  PointAdaptive* patch12_p11 = patch6_p11;

  VectorAdaptive* patch12_Qu00 = new VectorAdaptive(*patch12_p00, *patch12_p10);
  VectorAdaptive* patch12_Qu10 = new VectorAdaptive(*patch12_p00, *patch12_p10);
  // VectorAdaptive* patch12_Qu01 = new VectorAdaptive ( *patch12_p01,
  // *patch12_p11 ); VectorAdaptive* patch12_Qu11 = new VectorAdaptive (
  // *patch12_p01, *patch12_p11 );

  // VectorAdaptive* patch12_Qv00 = new VectorAdaptive ( *patch12_p00,
  // *patch12_p01 );
  VectorAdaptive* patch12_Qv10 = new VectorAdaptive(*patch12_p10, *patch12_p11);
  // VectorAdaptive* patch12_Qv01 = new VectorAdaptive ( *patch12_p00,
  // *patch12_p01 );
  VectorAdaptive* patch12_Qv11 = new VectorAdaptive(*patch12_p10, *patch12_p11);

  VectorAdaptive* patch12_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch12_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch12_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch12_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch12_c1 = new CurveAdaptiveParametricHermite(
      *patch12_p00, *patch12_p10, *patch12_Qu00, *patch12_Qu10);
  CurveAdaptive* patch12_c2 = new CurveAdaptiveParametricHermite(
      *patch12_p10, *patch12_p11, *patch12_Qv10, *patch12_Qv11);
  CurveAdaptive* patch12_c3 = patch6_c2;
  CurveAdaptive* patch12_c4 = patch3_c2;

  Patch* patch12 = new PatchHermite(patch12_c1, patch12_c2, patch12_c3,
                                    patch12_c4, *patch12_tw00, *patch12_tw10,
                                    *patch12_tw01, *patch12_tw11);

  geometry->InsertPatch(patch12);
  geometry->InsertCurve(patch12_c1);
  geometry->InsertCurve(patch12_c2);
  // geo->InsertCurve ( patch12_c3 );
  // geo->InsertCurve ( patch12_c4 );
  //======================== FIM DO PATCH 12 ==================================

  //=============================== PATCH 13 ==================================
  PointAdaptive* patch13_p00 = patch11_p01;
  // Ponto* patch13_p10 = patch11_p11;
  PointAdaptive* patch13_p01 = new VertexAdaptive(-1.0, 21.5, 2.0);
  PointAdaptive* patch13_p11 = patch10_p01;

  // VectorAdaptive* patch13_Qu00 = new VectorAdaptive ( *patch13_p00,
  // *patch13_p10 ); VectorAdaptive* patch13_Qu10 = new VectorAdaptive (
  // *patch13_p00, *patch13_p10 );
  VectorAdaptive* patch13_Qu01 = new VectorAdaptive(*patch13_p01, *patch13_p11);
  VectorAdaptive* patch13_Qu11 = new VectorAdaptive(*patch13_p01, *patch13_p11);

  VectorAdaptive* patch13_Qv00 = new VectorAdaptive(*patch13_p00, *patch13_p01);
  // VectorAdaptive* patch13_Qv10 = new VectorAdaptive ( *patch13_p10,
  // *patch13_p11 );
  VectorAdaptive* patch13_Qv01 = new VectorAdaptive(*patch13_p00, *patch11_p01);
  // VectorAdaptive* patch13_Qv11 = new VectorAdaptive ( *patch13_p10,
  // *patch13_p11 );

  VectorAdaptive* patch13_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch13_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch13_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch13_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch13_c1 = patch11_c3;
  CurveAdaptive* patch13_c2 = patch7_c4;
  CurveAdaptive* patch13_c3 = new CurveAdaptiveParametricHermite(
      *patch13_p01, *patch13_p11, *patch13_Qu01, *patch13_Qu11);
  CurveAdaptive* patch13_c4 = new CurveAdaptiveParametricHermite(
      *patch13_p00, *patch13_p01, *patch13_Qv00, *patch13_Qv01);

  Patch* patch13 = new PatchHermite(patch13_c1, patch13_c2, patch13_c3,
                                    patch13_c4, *patch13_tw00, *patch13_tw10,
                                    *patch13_tw01, *patch13_tw11);

  geometry->InsertPatch(patch13);
  // geo->InsertCurve ( patch13_c1 );
  // geo->InsertCurve ( patch13_c2 );
  geometry->InsertCurve(patch13_c3);
  geometry->InsertCurve(patch13_c4);
  //======================== FIM DO PATCH 13 ==================================

  //=============================== PATCH 14 ==================================
  PointAdaptive* patch14_p00 = patch12_p10;
  PointAdaptive* patch14_p10 = new VertexAdaptive(
      -patch13_p01->GetX(), patch13_p01->GetY(), patch13_p01->GetZ());
  // Ponto* patch14_p01 = patch6_p11;
  PointAdaptive* patch14_p11 = patch10_p11;

  VectorAdaptive* patch14_Qu00 = new VectorAdaptive(*patch14_p00, *patch14_p10);
  VectorAdaptive* patch14_Qu10 = new VectorAdaptive(*patch14_p00, *patch14_p10);
  // VectorAdaptive* patch14_Qu01 = new VectorAdaptive ( *patch14_p01,
  // *patch14_p11 ); VectorAdaptive* patch14_Qu11 = new VectorAdaptive (
  // *patch14_p01, *patch14_p11 );

  // VectorAdaptive* patch14_Qv00 = new VectorAdaptive ( *patch14_p00,
  // *patch14_p01 );
  VectorAdaptive* patch14_Qv10 = new VectorAdaptive(*patch14_p10, *patch14_p11);
  // VectorAdaptive* patch14_Qv01 = new VectorAdaptive ( *patch14_p00,
  // *patch14_p01 );
  VectorAdaptive* patch14_Qv11 = new VectorAdaptive(*patch14_p10, *patch14_p11);

  VectorAdaptive* patch14_tw00 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch14_tw10 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch14_tw01 = new VectorAdaptive(0.0, 0.0, 0.0);
  VectorAdaptive* patch14_tw11 = new VectorAdaptive(0.0, 0.0, 0.0);

  CurveAdaptive* patch14_c1 = new CurveAdaptiveParametricHermite(
      *patch14_p00, *patch14_p10, *patch14_Qu00, *patch14_Qu10);
  CurveAdaptive* patch14_c2 = new CurveAdaptiveParametricHermite(
      *patch14_p10, *patch14_p11, *patch14_Qv10, *patch14_Qv11);
  CurveAdaptive* patch14_c3 = patch9_c2;
  CurveAdaptive* patch14_c4 = patch12_c2;

  Patch* patch14 = new PatchHermite(patch14_c1, patch14_c2, patch14_c3,
                                    patch14_c4, *patch14_tw00, *patch14_tw10,
                                    *patch14_tw01, *patch14_tw11);

  geometry->InsertPatch(patch14);
  geometry->InsertCurve(patch14_c1);
  geometry->InsertCurve(patch14_c2);
  // geo->InsertCurve ( patch14_c3 );
  // geo->InsertCurve ( patch14_c4 );
  return geometry;
  //=========================== FIM DO PATCH 14
  //==================================
  // Fim do Exemplo do Nariz
  //==============================================================================
}

Geometry* Models3d::ModelUtahteapot(Geometry* geometry) {
  //==============================================================================
  // Exemplo do Utahteapot
  //==============================================================================

  //=============================== PATCH 1 ==================================
  PointAdaptive* p100 = new VertexAdaptive(1.40000, 0.00000, 2.40000);
  PointAdaptive* p110 = new VertexAdaptive(1.40000, -0.78400, 2.40000);
  PointAdaptive* p120 = new VertexAdaptive(0.78400, -1.40000, 2.40000);
  PointAdaptive* p130 = new VertexAdaptive(0.00000, -1.40000, 2.40000);

  PointAdaptive* p101 = new VertexAdaptive(1.33750, 0.00000, 2.53125);
  PointAdaptive* p111 = new VertexAdaptive(1.33750, -0.74900, 2.53125);
  PointAdaptive* p121 = new VertexAdaptive(0.74900, -1.33750, 2.53125);
  PointAdaptive* p131 = new VertexAdaptive(0.00000, -1.33750, 2.53125);

  PointAdaptive* p102 = new VertexAdaptive(1.43750, 0.00000, 2.53125);
  PointAdaptive* p112 = new VertexAdaptive(1.43750, -0.80500, 2.53125);
  PointAdaptive* p122 = new VertexAdaptive(0.80500, -1.43750, 2.53125);
  PointAdaptive* p132 = new VertexAdaptive(0.00000, -1.43750, 2.53125);

  PointAdaptive* p103 = new VertexAdaptive(1.50000, 0.00000, 2.40000);
  PointAdaptive* p113 = new VertexAdaptive(1.50000, -0.84000, 2.40000);
  PointAdaptive* p123 = new VertexAdaptive(0.84000, -1.50000, 2.40000);
  PointAdaptive* p133 = new VertexAdaptive(0.00000, -1.50000, 2.40000);

  CurveAdaptive* patch1_c1 =
      new CurveAdaptiveParametricBezier(*p100, *p110, *p120, *p130);
  CurveAdaptive* patch1_c2 =
      new CurveAdaptiveParametricBezier(*p130, *p131, *p132, *p133);
  CurveAdaptive* patch1_c3 =
      new CurveAdaptiveParametricBezier(*p103, *p113, *p123, *p133);
  CurveAdaptive* patch1_c4 =
      new CurveAdaptiveParametricBezier(*p100, *p101, *p102, *p103);

  Patch* patch1 = new PatchBezier(patch1_c1, patch1_c2, patch1_c3, patch1_c4,
                                  *p111, *p121, *p112, *p122);

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
  PointAdaptive* p210 = new VertexAdaptive(-0.78400, -1.40000, 2.40000);
  PointAdaptive* p220 = new VertexAdaptive(-1.40000, -0.78400, 2.40000);
  PointAdaptive* p230 = new VertexAdaptive(-1.40000, 0.00000, 2.40000);

  //		Ponto* p201 = p131; // new Vertice (  0.00000, -1.33750, 2.53125
  //);
  PointAdaptive* p211 = new VertexAdaptive(-0.74900, -1.33750, 2.53125);
  PointAdaptive* p221 = new VertexAdaptive(-1.33750, -0.74900, 2.53125);
  PointAdaptive* p231 = new VertexAdaptive(-1.33750, 0.00000, 2.53125);

  //		Ponto* p202 = p132; //new Vertice (  0.00000, -1.43750, 2.53125
  //);
  PointAdaptive* p212 = new VertexAdaptive(-0.80500, -1.43750, 2.53125);
  PointAdaptive* p222 = new VertexAdaptive(-1.43750, -0.80500, 2.53125);
  PointAdaptive* p232 = new VertexAdaptive(-1.43750, 0.00000, 2.53125);

  //		Ponto* p203 = p133; //new Vertice (  0.00000, -1.50000, 2.40000
  //);
  PointAdaptive* p213 = new VertexAdaptive(-0.84000, -1.50000, 2.40000);
  PointAdaptive* p223 = new VertexAdaptive(-1.50000, -0.84000, 2.40000);
  PointAdaptive* p233 = new VertexAdaptive(-1.50000, 0.00000, 2.40000);

  CurveAdaptive* patch2_c1 = new CurveAdaptiveParametricBezier(
      *p130, *p210, *p220, *p230);  //( *p200, *p210, *p220, *p230 );
  CurveAdaptive* patch2_c2 =
      new CurveAdaptiveParametricBezier(*p230, *p231, *p232, *p233);
  CurveAdaptive* patch2_c3 =
      new CurveAdaptiveParametricBezier(*p133, *p213, *p223, *p233);
  // Curva* patch2_c4 = patch1_c2; //new CurvParamBezier ( *p130, *p131, *p132,
  // *p133 ); //( *p200, *p201, *p202, *p203 );

  Patch* patch2 = new PatchBezier(patch2_c1, patch2_c2, patch2_c3, patch1_c2,
                                  *p211, *p221, *p212, *p222);

  geometry->InsertPatch(patch2);
  geometry->InsertCurve(patch2_c1);
  geometry->InsertCurve(patch2_c2);
  geometry->InsertCurve(patch2_c3);
  // geo->InsertCurve ( patch2_c4 );
  //======================== FIM DO PATCH 2 ==================================

  //=============================== PATCH 3 ==================================
  //		Ponto* p300 = p230; //new Vertice ( -1.40000, 0.00000, 2.40000
  //);
  PointAdaptive* p310 = new VertexAdaptive(-1.40000, 0.78400, 2.40000);
  PointAdaptive* p320 = new VertexAdaptive(-0.78400, 1.40000, 2.40000);
  PointAdaptive* p330 = new VertexAdaptive(0.00000, 1.40000, 2.40000);

  //		Ponto* p301 = p231; //new Vertice ( -1.33750, 0.00000, 2.53125
  //);
  PointAdaptive* p311 = new VertexAdaptive(-1.33750, 0.74900, 2.53125);
  PointAdaptive* p321 = new VertexAdaptive(-0.74900, 1.33750, 2.53125);
  PointAdaptive* p331 = new VertexAdaptive(0.00000, 1.33750, 2.53125);

  //		Ponto* p302 = p232; //new Vertice ( -1.43750, 0.00000, 2.53125
  //);
  PointAdaptive* p312 = new VertexAdaptive(-1.43750, 0.80500, 2.53125);
  PointAdaptive* p322 = new VertexAdaptive(-0.80500, 1.43750, 2.53125);
  PointAdaptive* p332 = new VertexAdaptive(0.00000, 1.43750, 2.53125);

  //		Ponto* p303 = p233; //new Vertice ( -1.50000, 0.00000, 2.40000
  //);
  PointAdaptive* p313 = new VertexAdaptive(-1.50000, 0.84000, 2.40000);
  PointAdaptive* p323 = new VertexAdaptive(-0.84000, 1.50000, 2.40000);
  PointAdaptive* p333 = new VertexAdaptive(0.00000, 1.50000, 2.40000);

  CurveAdaptive* patch3_c1 =
      new CurveAdaptiveParametricBezier(*p230, *p310, *p320, *p330);
  CurveAdaptive* patch3_c2 =
      new CurveAdaptiveParametricBezier(*p330, *p331, *p332, *p333);
  CurveAdaptive* patch3_c3 =
      new CurveAdaptiveParametricBezier(*p233, *p313, *p323, *p333);
  //		Curva* patch3_c4 = new CurvParamBezier ( *p230, *p231, *p232,
  //*p233
  //);

  Patch* patch3 = new PatchBezier(patch3_c1, patch3_c2, patch3_c3, patch2_c2,
                                  *p311, *p321, *p312, *p322);

  geometry->InsertPatch(patch3);
  geometry->InsertCurve(patch3_c1);
  geometry->InsertCurve(patch3_c2);
  geometry->InsertCurve(patch3_c3);
  //		geo->InsertCurve ( patch3_c4 );
  //======================== FIM DO PATCH 3 ==================================

  //=============================== PATCH 4 ==================================
  // Ponto* p400 = p330; //new Vertice ( 0.00000, 1.40000, 2.40000 );
  PointAdaptive* p410 = new VertexAdaptive(0.78400, 1.40000, 2.40000);
  PointAdaptive* p420 = new VertexAdaptive(1.40000, 0.78400, 2.40000);
  // Ponto* p430 = p100; // new Vertice ( 1.40000, 0.00000, 2.40000 );

  // Ponto* p401 = p331; //new Vertice (	0.00000, 1.33750, 2.53125 );
  PointAdaptive* p411 = new VertexAdaptive(0.74900, 1.33750, 2.53125);
  PointAdaptive* p421 = new VertexAdaptive(1.33750, 0.74900, 2.53125);
  // Ponto* p431 = p101; // new Vertice ( 1.33750, 0.00000, 2.53125 );

  // Ponto* p402 = p332; //new Vertice (	0.00000, 1.43750, 2.53125 );
  PointAdaptive* p412 = new VertexAdaptive(0.80500, 1.43750, 2.53125);
  PointAdaptive* p422 = new VertexAdaptive(1.43750, 0.80500, 2.53125);
  // Ponto* p432 = p102; //new Vertice ( 1.43750, 0.00000, 2.53125 );

  // Ponto* p403 = p333; //new Vertice (	0.00000, 1.50000, 2.40000 );
  PointAdaptive* p413 = new VertexAdaptive(0.84000, 1.50000, 2.40000);
  PointAdaptive* p423 = new VertexAdaptive(1.50000, 0.84000, 2.40000);
  // Ponto* p433 = p103; //new Vertice ( 1.50000, 0.00000, 2.40000 );

  CurveAdaptive* patch4_c1 = new CurveAdaptiveParametricBezier(
      *p330, *p410, *p420, *p100);  // ( *p400, *p410, *p420, *p430 );
  // Curva* patch4_c2 = new CurvParamBezier ( *p100, *p101, *p102, *p103 ); // (
  // *p430, *p431, *p432, *p433 );
  CurveAdaptive* patch4_c3 = new CurveAdaptiveParametricBezier(
      *p333, *p413, *p423, *p103);  // ( *p403, *p413, *p423, *p433 );
  // Curva* patch4_c4 = new CurvParamBezier ( *p330, *p331, *p332, *p333 ); // (
  // *p400, *p401, *p402, *p403 );

  Patch* patch4 = new PatchBezier(patch4_c1, patch1_c4, patch4_c3, patch3_c2,
                                  *p411, *p421, *p412, *p422);

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

  PointAdaptive* p501 = new VertexAdaptive(1.75000, 0.00000, 1.87500);
  PointAdaptive* p511 = new VertexAdaptive(1.75000, -0.98000, 1.87500);
  PointAdaptive* p521 = new VertexAdaptive(0.98000, -1.75000, 1.87500);
  PointAdaptive* p531 = new VertexAdaptive(0.00000, -1.75000, 1.87500);

  PointAdaptive* p502 = new VertexAdaptive(2.00000, 0.00000, 1.35000);
  PointAdaptive* p512 = new VertexAdaptive(2.00000, -1.12000, 1.35000);
  PointAdaptive* p522 = new VertexAdaptive(1.12000, -2.00000, 1.35000);
  PointAdaptive* p532 = new VertexAdaptive(0.00000, -2.00000, 1.35000);

  PointAdaptive* p503 = new VertexAdaptive(2.00000, 0.00000, 0.90000);
  PointAdaptive* p513 = new VertexAdaptive(2.00000, -1.12000, 0.90000);
  PointAdaptive* p523 = new VertexAdaptive(1.12000, -2.00000, 0.90000);
  PointAdaptive* p533 = new VertexAdaptive(0.00000, -2.00000, 0.90000);

  // Curva* patch5_c1 = patch1_c3; //new CurvParamBezier ( *p103, *p113, *p123,
  // *p133 ); // ( *p500, *p510, *p520, *p530 );
  CurveAdaptive* patch5_c2 = new CurveAdaptiveParametricBezier(
      *p133, *p531, *p532, *p533);  // ( *p530, *p531, *p532, *p533 );
  CurveAdaptive* patch5_c3 = new CurveAdaptiveParametricBezier(
      *p503, *p513, *p523, *p533);  // ( *p503, *p513, *p523, *p533 );
  CurveAdaptive* patch5_c4 = new CurveAdaptiveParametricBezier(
      *p103, *p501, *p502, *p503);  // ( *p500, *p501, *p502, *p503 );

  Patch* patch5 = new PatchBezier(patch1_c3, patch5_c2, patch5_c3, patch5_c4,
                                  *p511, *p521, *p512, *p522);

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
  PointAdaptive* p611 = new VertexAdaptive(-0.98000, -1.75000, 1.87500);
  PointAdaptive* p621 = new VertexAdaptive(-1.75000, -0.98000, 1.87500);
  PointAdaptive* p631 = new VertexAdaptive(-1.75000, 0.00000, 1.87500);

  //		Ponto* p602 = p532; //new Vertice (	0.00000,
  //-2.00000, 1.35000
  //);
  PointAdaptive* p612 = new VertexAdaptive(-1.12000, -2.00000, 1.35000);
  PointAdaptive* p622 = new VertexAdaptive(-2.00000, -1.12000, 1.35000);
  PointAdaptive* p632 = new VertexAdaptive(-2.00000, 0.00000, 1.35000);

  //		Ponto* p603 = p533; //new Vertice (	0.00000, -2.00000,
  // 0.90000
  //);
  PointAdaptive* p613 = new VertexAdaptive(-1.12000, -2.00000, 0.90000);
  PointAdaptive* p623 = new VertexAdaptive(-2.00000, -1.12000, 0.90000);
  PointAdaptive* p633 = new VertexAdaptive(-2.00000, 0.00000, 0.90000);

  //		Curva* patch6_c1 = new CurvParamBezier ( *p133, *p213, *p223,
  //*p233
  //); // ( *p600, *p610, *p620, *p630 );
  CurveAdaptive* patch6_c2 = new CurveAdaptiveParametricBezier(
      *p233, *p631, *p632, *p633);  // ( *p630, *p631, *p632, *p633 );
  CurveAdaptive* patch6_c3 = new CurveAdaptiveParametricBezier(
      *p533, *p613, *p623, *p633);  // ( *p603, *p613, *p623, *p633 );
  //		Curva* patch6_c4 = new CurvParamBezier ( *p133, *p531, *p532,
  //*p533
  //); // ( *p600, *p601, *p602, *p603 );

  Patch* patch6 = new PatchBezier(patch2_c3, patch6_c2, patch6_c3, patch5_c2,
                                  *p611, *p621, *p612, *p622);

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
  PointAdaptive* p711 = new VertexAdaptive(-1.75000, 0.98000, 1.87500);
  PointAdaptive* p721 = new VertexAdaptive(-0.98000, 1.75000, 1.87500);
  PointAdaptive* p731 = new VertexAdaptive(0.00000, 1.75000, 1.87500);

  //		Ponto* p702 = p632; //new Vertice (-2.00000, 0.00000, 1.35000 );
  PointAdaptive* p712 = new VertexAdaptive(-2.00000, 1.12000, 1.35000);
  PointAdaptive* p722 = new VertexAdaptive(-1.12000, 2.00000, 1.35000);
  PointAdaptive* p732 = new VertexAdaptive(0.00000, 2.00000, 1.35000);

  //		Ponto* p703 = p633; //new Vertice (-2.00000, 0.00000, 0.90000 );
  PointAdaptive* p713 = new VertexAdaptive(-2.00000, 1.12000, 0.90000);
  PointAdaptive* p723 = new VertexAdaptive(-1.12000, 2.00000, 0.90000);
  PointAdaptive* p733 = new VertexAdaptive(0.00000, 2.00000, 0.90000);

  //		Curva* patch7_c1 = new CurvParamBezier ( *p233, *p313, *p323,
  //*p333
  //);
  CurveAdaptive* patch7_c2 =
      new CurveAdaptiveParametricBezier(*p333, *p731, *p732, *p733);
  CurveAdaptive* patch7_c3 =
      new CurveAdaptiveParametricBezier(*p633, *p713, *p723, *p733);
  //		Curva* patch7_c4 = new CurvParamBezier ( *p233, *p631, *p632,
  //*p633
  //);

  Patch* patch7 = new PatchBezier(patch3_c3, patch7_c2, patch7_c3, patch6_c2,
                                  *p711, *p721, *p712, *p722);

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
  PointAdaptive* p811 = new VertexAdaptive(0.98000, 1.75000, 1.87500);
  PointAdaptive* p821 = new VertexAdaptive(1.75000, 0.98000, 1.87500);
  //		Ponto* p831 = p501; //new Vertice ( 1.75000, 0.00000, 1.87500 );

  //		Ponto* p802 = p732; //new Vertice (
  // 0.00000, 2.00000, 1.35000
  //);
  PointAdaptive* p812 = new VertexAdaptive(1.12000, 2.00000, 1.35000);
  PointAdaptive* p822 = new VertexAdaptive(2.00000, 1.12000, 1.35000);
  //		Ponto* p832 = p502; //new Vertice ( 2.00000, 0.00000, 1.35000 );

  //		Ponto* p803 = p733; //new Vertice (	0.00000, 2.00000,
  // 0.90000
  //);
  PointAdaptive* p813 = new VertexAdaptive(1.12000, 2.00000, 0.90000);
  PointAdaptive* p823 = new VertexAdaptive(2.00000, 1.12000, 0.90000);
  //		Ponto* p833 = p503; //new Vertice ( 2.00000, 0.00000, 0.90000 );

  //		Curva* patch8_c1 = new CurvParamBezier ( *p333, *p413, *p423,
  //*p103
  //); 		Curva* patch8_c2 = new CurvParamBezier ( *p103, *p501, *p502,
  //*p503 );
  CurveAdaptive* patch8_c3 =
      new CurveAdaptiveParametricBezier(*p733, *p813, *p823, *p503);
  //		Curva* patch8_c4 = new CurvParamBezier ( *p333, *p731, *p732,
  //*p733
  //);

  Patch* patch8 = new PatchBezier(patch4_c3, patch5_c4, patch8_c3, patch7_c2,
                                  *p811, *p821, *p812, *p822);

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

  PointAdaptive* p901 = new VertexAdaptive(2.00000, 0.00000, 0.45000);
  PointAdaptive* p911 = new VertexAdaptive(2.00000, -1.12000, 0.45000);
  PointAdaptive* p921 = new VertexAdaptive(1.12000, -2.00000, 0.45000);
  PointAdaptive* p931 = new VertexAdaptive(0.00000, -2.00000, 0.45000);

  PointAdaptive* p902 = new VertexAdaptive(1.50000, 0.00000, 0.22500);
  PointAdaptive* p912 = new VertexAdaptive(1.50000, -0.84000, 0.22500);
  PointAdaptive* p922 = new VertexAdaptive(0.84000, -1.50000, 0.22500);
  PointAdaptive* p932 = new VertexAdaptive(0.00000, -1.50000, 0.22500);

  PointAdaptive* p903 = new VertexAdaptive(1.50000, 0.00000, 0.15000);
  PointAdaptive* p913 = new VertexAdaptive(1.50000, -0.84000, 0.15000);
  PointAdaptive* p923 = new VertexAdaptive(0.84000, -1.50000, 0.15000);
  PointAdaptive* p933 = new VertexAdaptive(0.00000, -1.50000, 0.15000);

  //		Curva* patch9_c1 = new CurvParamBezier ( *p503, *p513, *p523,
  //*p533
  //);
  CurveAdaptive* patch9_c2 =
      new CurveAdaptiveParametricBezier(*p533, *p931, *p932, *p933);
  CurveAdaptive* patch9_c3 =
      new CurveAdaptiveParametricBezier(*p903, *p913, *p923, *p933);
  CurveAdaptive* patch9_c4 =
      new CurveAdaptiveParametricBezier(*p503, *p901, *p902, *p903);

  Patch* patch9 = new PatchBezier(patch5_c3, patch9_c2, patch9_c3, patch9_c4,
                                  *p911, *p921, *p912, *p922);

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
  PointAdaptive* p1011 = new VertexAdaptive(-1.12000, -2.00000, 0.45000);
  PointAdaptive* p1021 = new VertexAdaptive(-2.00000, -1.12000, 0.45000);
  PointAdaptive* p1031 = new VertexAdaptive(-2.00000, 0.00000, 0.45000);

  //		Ponto* p1002 = p932; //new Vertice ( 0.00000,-1.50000, 0.22500
  //);
  PointAdaptive* p1012 = new VertexAdaptive(-0.84000, -1.50000, 0.22500);
  PointAdaptive* p1022 = new VertexAdaptive(-1.50000, -0.84000, 0.22500);
  PointAdaptive* p1032 = new VertexAdaptive(-1.50000, 0.00000, 0.22500);

  //		Ponto* p1003 = p933; //new Vertice ( 0.00000,-1.50000, 0.15000
  //);
  PointAdaptive* p1013 = new VertexAdaptive(-0.84000, -1.50000, 0.15000);
  PointAdaptive* p1023 = new VertexAdaptive(-1.50000, -0.84000, 0.15000);
  PointAdaptive* p1033 = new VertexAdaptive(-1.50000, 0.00000, 0.15000);

  //		Curva* patch10_c1 = new CurvParamBezier ( *p533, *p613, *p623,
  //*p633
  //);
  CurveAdaptive* patch10_c2 =
      new CurveAdaptiveParametricBezier(*p633, *p1031, *p1032, *p1033);
  CurveAdaptive* patch10_c3 =
      new CurveAdaptiveParametricBezier(*p933, *p1013, *p1023, *p1033);
  //		Curva* patch10_c4 = new CurvParamBezier ( *p533, *p931, *p932,
  //*p933
  //);

  Patch* patch10 = new PatchBezier(patch6_c3, patch10_c2, patch10_c3, patch9_c2,
                                   *p1011, *p1021, *p1012, *p1022);

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
  PointAdaptive* p1111 = new VertexAdaptive(-2.00000, 1.12000, 0.45000);
  PointAdaptive* p1121 = new VertexAdaptive(-1.12000, 2.00000, 0.45000);
  PointAdaptive* p1131 = new VertexAdaptive(0.00000, 2.00000, 0.45000);

  //		Ponto* p1032 = p1032; //new Vertice (-1.50000, 0.00000, 0.22500
  //);
  PointAdaptive* p1112 = new VertexAdaptive(-1.50000, 0.84000, 0.22500);
  PointAdaptive* p1122 = new VertexAdaptive(-0.84000, 1.50000, 0.22500);
  PointAdaptive* p1132 = new VertexAdaptive(0.00000, 1.50000, 0.22500);

  //		Ponto* p1033 = p1033; //new Vertice (-1.50000, 0.00000, 0.15000
  //);
  PointAdaptive* p1113 = new VertexAdaptive(-1.50000, 0.84000, 0.15000);
  PointAdaptive* p1123 = new VertexAdaptive(-0.84000, 1.50000, 0.15000);
  PointAdaptive* p1133 = new VertexAdaptive(0.00000, 1.50000, 0.15000);

  //		Curva* patch11_c1 = new CurvParamBezier ( *p633, *p713, *p723,
  //*p733
  //);
  CurveAdaptive* patch11_c2 =
      new CurveAdaptiveParametricBezier(*p733, *p1131, *p1132, *p1133);
  CurveAdaptive* patch11_c3 =
      new CurveAdaptiveParametricBezier(*p1033, *p1113, *p1123, *p1133);
  //		Curva* patch11_c4 = new CurvParamBezier ( *p633, *p1031, *p1032,
  //*p1033 );

  Patch* patch11 = new PatchBezier(patch7_c3, patch11_c2, patch11_c3,
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
  PointAdaptive* p1211 = new VertexAdaptive(1.12000, 2.00000, 0.45000);
  PointAdaptive* p1221 = new VertexAdaptive(2.00000, 1.20000, 0.45000);
  //		Ponto* p901 = p901; //new Vertice ( 2.00000, 0.00000, 0.45000 );

  //		Ponto* p1132 = p1132; //new Vertice ( 0.00000, 1.50000, 0.22500
  //);
  PointAdaptive* p1212 = new VertexAdaptive(0.84000, 1.50000, 0.22500);
  PointAdaptive* p1222 = new VertexAdaptive(1.50000, 0.84000, 0.22500);
  //		Ponto* p902 = p902; //new Vertice ( 1.50000, 0.00000, 0.22500 );

  //		Ponto* p1133 = p1133; //new Vertice ( 0.00000, 1.50000, 0.15000
  //);
  PointAdaptive* p1213 = new VertexAdaptive(0.84000, 1.50000, 0.15000);
  PointAdaptive* p1223 = new VertexAdaptive(1.50000, 0.84000, 0.15000);
  //		Ponto* p903 = p903; //new Vertice ( 1.50000, 0.00000, 0.15000 );

  //		Curva* patch12_c1 = new CurvParamBezier ( *p733, *p813, *p823,
  //*p503
  //); 		Curva* patch12_c2 = new CurvParamBezier ( *p503, *p901, *p902,
  //*p903 );
  CurveAdaptive* patch12_c3 =
      new CurveAdaptiveParametricBezier(*p1133, *p1213, *p1223, *p903);
  //		Curva* patch12_c4 = new CurvParamBezier ( *p733, *p1131, *p1132,
  //*p1133 );

  Patch* patch12 = new PatchBezier(patch8_c3, patch9_c4, patch12_c3, patch11_c2,
                                   *p1211, *p1221, *p1212, *p1222);

  geometry->InsertPatch(patch12);
  //		geo->InsertCurve ( patch12_c1 );
  //		geo->InsertCurve ( patch12_c2 );
  geometry->InsertCurve(patch12_c3);
  //		geo->InsertCurve ( patch12_c4 );
  //======================== FIM DO PATCH 12 =================================

  //============================== PATCH 13 ==================================
  PointAdaptive* p1300 = new VertexAdaptive(-1.60000, 0.00000, 2.02500);
  PointAdaptive* p1310 = new VertexAdaptive(-1.60000, -0.30000, 2.02500);
  PointAdaptive* p1320 = new VertexAdaptive(-1.50000, -0.30000, 2.25000);
  PointAdaptive* p1330 = new VertexAdaptive(-1.50000, 0.00000, 2.25000);

  PointAdaptive* p1301 = new VertexAdaptive(-2.30000, 0.00000, 2.02500);
  PointAdaptive* p1311 = new VertexAdaptive(-2.30000, -0.30000, 2.02500);
  PointAdaptive* p1321 = new VertexAdaptive(-2.50000, -0.30000, 2.25000);
  PointAdaptive* p1331 = new VertexAdaptive(-2.50000, 0.00000, 2.25000);

  PointAdaptive* p1302 = new VertexAdaptive(-2.70000, 0.00000, 2.02500);
  PointAdaptive* p1312 = new VertexAdaptive(-2.70000, -0.30000, 2.02500);
  PointAdaptive* p1322 = new VertexAdaptive(-3.00000, -0.30000, 2.25000);
  PointAdaptive* p1332 = new VertexAdaptive(-3.00000, 0.00000, 2.25000);

  PointAdaptive* p1303 = new VertexAdaptive(-2.70000, 0.00000, 1.80000);
  PointAdaptive* p1313 = new VertexAdaptive(-2.70000, -0.30000, 1.80000);
  PointAdaptive* p1323 = new VertexAdaptive(-3.00000, -0.30000, 1.80000);
  PointAdaptive* p1333 = new VertexAdaptive(-3.00000, 0.00000, 1.80000);

  CurveAdaptive* patch13_c1 =
      new CurveAdaptiveParametricBezier(*p1300, *p1310, *p1320, *p1330);
  CurveAdaptive* patch13_c2 =
      new CurveAdaptiveParametricBezier(*p1330, *p1331, *p1332, *p1333);
  CurveAdaptive* patch13_c3 =
      new CurveAdaptiveParametricBezier(*p1303, *p1313, *p1323, *p1333);
  CurveAdaptive* patch13_c4 =
      new CurveAdaptiveParametricBezier(*p1300, *p1301, *p1302, *p1303);

  Patch* patch13 = new PatchBezier(patch13_c1, patch13_c2, patch13_c3,
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
  PointAdaptive* p1410 = new VertexAdaptive(-1.50000, 0.30000, 2.25000);
  PointAdaptive* p1420 = new VertexAdaptive(-1.60000, 0.30000, 2.02500);
  //		Ponto* p1300 = p1300; //new Vertice ( -1.60000, 0.00000, 2.02500
  //);

  //		Ponto* p1331 = p1331; //new Vertice ( -2.50000, 0.00000, 2.25000
  //);
  PointAdaptive* p1411 = new VertexAdaptive(-2.50000, 0.30000, 2.25000);
  PointAdaptive* p1421 = new VertexAdaptive(-2.30000, 0.30000, 2.02500);
  //		Ponto* p1301 = p1301; //new Vertice ( -2.30000, 0.00000, 2.02500
  //);

  //		Ponto* p1332 = p1332; //new Vertice ( -3.00000, 0.00000, 2.25000
  //);
  PointAdaptive* p1412 = new VertexAdaptive(-3.00000, 0.30000, 2.25000);
  PointAdaptive* p1422 = new VertexAdaptive(-2.70000, 0.30000, 2.02500);
  //		Ponto* p1302 = p1302; //new Vertice ( -2.70000, 0.00000, 2.02500
  //);

  //		Ponto* p1333 = p1333; //new Vertice ( -3.00000, 0.00000, 1.80000
  //);
  PointAdaptive* p1413 = new VertexAdaptive(-3.00000, 0.30000, 1.80000);
  PointAdaptive* p1423 = new VertexAdaptive(-2.70000, 0.30000, 1.80000);
  //		Ponto* p1303 = p1303; //new Vertice ( -2.70000, 0.00000, 1.80000
  //);

  CurveAdaptive* patch14_c1 =
      new CurveAdaptiveParametricBezier(*p1330, *p1410, *p1420, *p1300);
  //		Curva* patch14_c2 = new CurvParamBezier ( *p1300, *p1301,
  //*p1302, *p1303 );
  CurveAdaptive* patch14_c3 =
      new CurveAdaptiveParametricBezier(*p1333, *p1413, *p1423, *p1303);
  //		Curva* patch14_c4 = new CurvParamBezier ( *p1330, *p1331,
  //*p1332, *p1333 );

  Patch* patch14 = new PatchBezier(patch14_c1, patch13_c4, patch14_c3,
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

  PointAdaptive* p1501 = new VertexAdaptive(-2.70000, 0.00000, 1.57500);
  PointAdaptive* p1511 = new VertexAdaptive(-2.70000, -0.30000, 1.57500);
  PointAdaptive* p1521 = new VertexAdaptive(-3.00000, -0.30000, 1.35000);
  PointAdaptive* p1531 = new VertexAdaptive(-3.00000, 0.00000, 1.35000);

  PointAdaptive* p1502 = new VertexAdaptive(-2.50000, 0.00000, 1.12500);
  PointAdaptive* p1512 = new VertexAdaptive(-2.50000, -0.30000, 1.12500);
  PointAdaptive* p1522 = new VertexAdaptive(-2.65000, -0.30000, 0.93750);
  PointAdaptive* p1532 = new VertexAdaptive(-2.65000, 0.00000, 0.93750);

  //		Ponto* p633 = p633; //new Vertice ( -2.00000, 0.00000, 0.90000
  //);
  PointAdaptive* p1513 = new VertexAdaptive(-2.00000, -0.30000, 0.90000);
  PointAdaptive* p1523 = new VertexAdaptive(-1.90000, -0.30000, 0.60000);
  PointAdaptive* p1533 = new VertexAdaptive(-1.90000, 0.00000, 0.60000);

  //		Curva* patch15_c1 = new CurvParamBezier ( *p1303, *p1313,
  //*p1323, *p1333 );
  CurveAdaptive* patch15_c2 =
      new CurveAdaptiveParametricBezier(*p1333, *p1531, *p1532, *p1533);
  CurveAdaptive* patch15_c3 =
      new CurveAdaptiveParametricBezier(*p633, *p1513, *p1523, *p1533);
  CurveAdaptive* patch15_c4 =
      new CurveAdaptiveParametricBezier(*p1303, *p1501, *p1502, *p633);

  Patch* patch15 = new PatchBezier(patch13_c3, patch15_c2, patch15_c3,
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
  PointAdaptive* p1611 = new VertexAdaptive(-3.00000, 0.30000, 1.35000);
  PointAdaptive* p1621 = new VertexAdaptive(-2.70000, 0.30000, 1.57500);
  //		Ponto* p1501 = p1501; //new Vertice ( -2.70000, 0.00000, 1.57500
  //);

  //		Ponto* p1532 = p1532; //new Vertice ( -2.65000, 0.00000, 0.93750
  //);
  PointAdaptive* p1612 = new VertexAdaptive(-2.65000, 0.30000, 0.93750);
  PointAdaptive* p1622 = new VertexAdaptive(-2.50000, 0.30000, 1.12500);
  //		Ponto* p1502 = p1502; //new Vertice ( -2.50000, 0.00000, 1.12500
  //);

  //		Ponto* p1533 = p1533; //new Vertice ( -1.90000, 0.00000, 0.60000
  //);
  PointAdaptive* p1613 = new VertexAdaptive(-1.90000, 0.30000, 0.60000);
  PointAdaptive* p1623 = new VertexAdaptive(-2.00000, 0.30000, 0.90000);
  //		Ponto* p633 = p633; //new Vertice ( -2.00000, 0.00000, 0.90000
  //);

  //		Curva* patch16_c1 = new CurvParamBezier ( *p1333, *p1413,
  //*p1423,
  //*p1303 ); 		Curva* patch16_c2 = new CurvParamBezier ( *p1303,
  //*p1501, *p1502, *p633 );
  CurveAdaptive* patch16_c3 =
      new CurveAdaptiveParametricBezier(*p1533, *p1613, *p1623, *p633);
  //		Curva* patch16_c4 = new CurvParamBezier ( *p1333, *p1531,
  //*p1532, *p1533 );

  Patch* patch16 = new PatchBezier(patch14_c3, patch15_c4, patch16_c3,
                                   patch15_c2, *p1611, *p1621, *p1612, *p1622);

  geometry->InsertPatch(patch16);
  //		geo->InsertCurve ( patch16_c1 );
  //		geo->InsertCurve ( patch16_c2 );
  geometry->InsertCurve(patch16_c3);
  //		geo->InsertCurve ( patch16_c4 );
  //======================== FIM DO PATCH 16 =================================

  //============================== PATCH 17 ==================================
  PointAdaptive* p1700 = new VertexAdaptive(1.70000, 0.00000, 1.42500);
  PointAdaptive* p1710 = new VertexAdaptive(1.70000, -0.66000, 1.42500);
  PointAdaptive* p1720 = new VertexAdaptive(1.70000, -0.66000, 0.60000);
  PointAdaptive* p1730 = new VertexAdaptive(1.70000, 0.00000, 0.60000);

  PointAdaptive* p1701 = new VertexAdaptive(2.60000, 0.00000, 1.42500);
  PointAdaptive* p1711 = new VertexAdaptive(2.60000, -0.66000, 1.42500);
  PointAdaptive* p1721 = new VertexAdaptive(3.10000, -0.66000, 0.82500);
  PointAdaptive* p1731 = new VertexAdaptive(3.10000, 0.00000, 0.82500);

  PointAdaptive* p1702 = new VertexAdaptive(2.30000, 0.00000, 2.10000);
  PointAdaptive* p1712 = new VertexAdaptive(2.30000, -0.25000, 2.10000);
  PointAdaptive* p1722 = new VertexAdaptive(2.40000, -0.25000, 2.02500);
  PointAdaptive* p1732 = new VertexAdaptive(2.40000, 0.00000, 2.02500);

  PointAdaptive* p1703 = new VertexAdaptive(2.70000, 0.00000, 2.40000);
  PointAdaptive* p1713 = new VertexAdaptive(2.70000, -0.25000, 2.40000);
  PointAdaptive* p1723 = new VertexAdaptive(3.30000, -0.25000, 2.40000);
  PointAdaptive* p1733 = new VertexAdaptive(3.30000, 0.00000, 2.40000);

  CurveAdaptive* patch17_c1 =
      new CurveAdaptiveParametricBezier(*p1700, *p1710, *p1720, *p1730);
  CurveAdaptive* patch17_c2 =
      new CurveAdaptiveParametricBezier(*p1730, *p1731, *p1732, *p1733);
  CurveAdaptive* patch17_c3 =
      new CurveAdaptiveParametricBezier(*p1703, *p1713, *p1723, *p1733);
  CurveAdaptive* patch17_c4 =
      new CurveAdaptiveParametricBezier(*p1700, *p1701, *p1702, *p1703);

  Patch* patch17 = new PatchBezier(patch17_c1, patch17_c2, patch17_c3,
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
  PointAdaptive* p1810 = new VertexAdaptive(1.70000, 0.66000, 0.60000);
  PointAdaptive* p1820 = new VertexAdaptive(1.70000, 0.66000, 1.42500);
  //		Ponto* p1700 = p1700; //new Vertice ( 1.70000, 0.00000, 1.42500
  //);

  //		Ponto* p1731 = p1731; //new Vertice ( 3.10000, 0.00000, 0.82500
  //);
  PointAdaptive* p1811 = new VertexAdaptive(3.10000, 0.66000, 0.82500);
  PointAdaptive* p1821 = new VertexAdaptive(2.60000, 0.66000, 1.42500);
  //		Ponto* p1701 = p1701; //new Vertice ( 2.60000, 0.00000, 1.42500
  //);

  //		Ponto* p1732 = p1732; //new Vertice ( 2.40000, 0.00000, 2.02500
  //);
  PointAdaptive* p1812 = new VertexAdaptive(2.40000, 0.25000, 2.02500);
  PointAdaptive* p1822 = new VertexAdaptive(2.30000, 0.25000, 2.10000);
  //		Ponto* p1702 = p1702; //new Vertice ( 2.30000, 0.00000, 2.10000
  //);

  //		Ponto* p1733 = p1733; //new Vertice ( 3.30000, 0.00000, 2.40000
  //);
  PointAdaptive* p1813 = new VertexAdaptive(3.30000, 0.25000, 2.40000);
  PointAdaptive* p1823 = new VertexAdaptive(2.70000, 0.25000, 2.40000);
  //		Ponto* p1703 = p1703; //new Vertice ( 2.70000, 0.00000, 2.40000
  //);

  CurveAdaptive* patch18_c1 =
      new CurveAdaptiveParametricBezier(*p1730, *p1810, *p1820, *p1700);
  //		Curva* patch18_c2 = new CurvParamBezier ( *p1700, *p1701,
  //*p1702, *p1703 );
  CurveAdaptive* patch18_c3 =
      new CurveAdaptiveParametricBezier(*p1733, *p1813, *p1823, *p1703);
  //		Curva* patch18_c4 = new CurvParamBezier ( *p1730, *p1731,
  //*p1732, *p1733 );

  Patch* patch18 = new PatchBezier(patch18_c1, patch17_c4, patch18_c3,
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

  PointAdaptive* p1901 = new VertexAdaptive(2.80000, 0.00000, 2.47500);
  PointAdaptive* p1911 = new VertexAdaptive(2.80000, -0.25000, 2.47500);
  PointAdaptive* p1921 = new VertexAdaptive(3.52500, -0.25000, 2.49375);
  PointAdaptive* p1931 = new VertexAdaptive(3.52500, 0.00000, 2.49375);

  PointAdaptive* p1902 = new VertexAdaptive(2.90000, 0.00000, 2.47500);
  PointAdaptive* p1912 = new VertexAdaptive(2.90000, -0.15000, 2.47500);
  PointAdaptive* p1922 = new VertexAdaptive(3.45000, -0.15000, 2.51250);
  PointAdaptive* p1932 = new VertexAdaptive(3.45000, 0.00000, 2.51250);

  PointAdaptive* p1903 = new VertexAdaptive(2.80000, 0.00000, 2.40000);
  PointAdaptive* p1913 = new VertexAdaptive(2.80000, -0.15000, 2.40000);
  //		Ponto* p1933 = new Vertice ( 3.20000, 0.00000, 2.40000 );
  PointAdaptive* p1933 = new VertexAdaptive(3.20000, 0.00000, 2.40000);

  //		Curva* patch19_c1 = new CurvParamBezier ( *p1703, *p1713,
  //*p1723, *p1733 );
  CurveAdaptive* patch19_c2 =
      new CurveAdaptiveParametricBezier(*p1733, *p1931, *p1932, *p1933);
  CurveAdaptive* patch19_c3 =
      new CurveAdaptiveParametricBezier(*p1903, *p1913, *p1933, *p1933);
  CurveAdaptive* patch19_c4 =
      new CurveAdaptiveParametricBezier(*p1703, *p1901, *p1902, *p1903);

  Patch* patch19 = new PatchBezier(patch17_c3, patch19_c2, patch19_c3,
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
  PointAdaptive* p2010 = new VertexAdaptive(3.30000, 0.25000, 2.40000);
  PointAdaptive* p2020 = new VertexAdaptive(2.70000, 0.25000, 2.40000);
  //		Ponto* p1703 = p1703; //new Vertice ( 2.70000, 0.00000, 2.40000
  //);

  //		Ponto* p1931 = p1931; //new Vertice ( 3.52500, 0.00000, 2.49375
  //);
  PointAdaptive* p2011 = new VertexAdaptive(3.52500, 0.25000, 2.49375);
  PointAdaptive* p2021 = new VertexAdaptive(2.80000, 0.25000, 2.47500);
  //		Ponto* p1901 = p1901; //new Vertice ( 2.80000, 0.00000, 2.47500
  //);

  //		Ponto* p1932 = p1932; //new Vertice ( 3.45000, 0.00000, 2.51250
  //);
  PointAdaptive* p2012 = new VertexAdaptive(3.45000, 0.15000, 2.51250);
  PointAdaptive* p2022 = new VertexAdaptive(2.90000, 0.15000, 2.47500);
  //		Ponto* p1902 = p1902; //new Vertice ( 2.90000, 0.00000, 2.47500
  //);

  //		Ponto* p1933 = p1933; //new Vertice ( 3.20000, 0.00000, 2.40000
  //); 		Ponto* p2013 = new Vertice ( 3.20000, 0.15000, 2.40000 );
  PointAdaptive* p2023 = new VertexAdaptive(2.80000, 0.15000, 2.40000);
  //		Ponto* p1903 = p1903; //new Vertice ( 2.80000, 0.00000, 2.40000
  //);

  CurveAdaptive* patch20_c1 =
      new CurveAdaptiveParametricBezier(*p1733, *p2010, *p2020, *p1703);
  //		Curva* patch20_c2 = new CurvParamBezier ( *p1703, *p1901,
  //*p1902, *p1903 );
  CurveAdaptive* patch20_c3 =
      new CurveAdaptiveParametricBezier(*p1933, *p1933, *p2023, *p1903);
  //		Curva* patch20_c4 = new CurvParamBezier ( *p1733, *p1931,
  //*p1932, *p1933 );

  Patch* patch20 = new PatchBezier(patch20_c1, patch19_c4, patch20_c3,
                                   patch19_c2, *p2011, *p2021, *p2012, *p2022);

  geometry->InsertPatch(patch20);
  geometry->InsertCurve(patch20_c1);
  //		geo->InsertCurve ( patch20_c2 );
  geometry->InsertCurve(patch20_c3);
  //		geo->InsertCurve ( patch20_c4 );
  //======================== FIM DO PATCH 20 =================================

  //============================== PATCH 21 ==================================
  PointAdaptive* p2100 = new VertexAdaptive(0.10000, 0.00000, 3.15000);
  PointAdaptive* p2110 = new VertexAdaptive(0.08660, -0.05000, 3.15000);
  PointAdaptive* p2120 = new VertexAdaptive(0.05000, -0.08660, 3.15000);
  PointAdaptive* p2130 = new VertexAdaptive(0.00000, -0.10000, 3.15000);

  PointAdaptive* p2101 = new VertexAdaptive(0.80000, 0.00000, 3.15000);
  PointAdaptive* p2111 = new VertexAdaptive(0.80000, -0.45000, 3.15000);
  PointAdaptive* p2121 = new VertexAdaptive(0.45000, -0.80000, 3.15000);
  PointAdaptive* p2131 = new VertexAdaptive(0.00000, -0.80000, 3.15000);

  PointAdaptive* p2102 = new VertexAdaptive(0.00000, 0.00000, 2.85000);
  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // );

  PointAdaptive* p2103 = new VertexAdaptive(0.20000, 0.00000, 2.70000);
  PointAdaptive* p2113 = new VertexAdaptive(0.20000, -0.11200, 2.70000);
  PointAdaptive* p2123 = new VertexAdaptive(0.11200, -0.20000, 2.70000);
  PointAdaptive* p2133 = new VertexAdaptive(0.00000, -0.20000, 2.70000);

  CurveAdaptive* patch21_c1 =
      new CurveAdaptiveParametricBezier(*p2100, *p2110, *p2120, *p2130);
  CurveAdaptive* patch21_c2 =
      new CurveAdaptiveParametricBezier(*p2130, *p2131, *p2102, *p2133);
  CurveAdaptive* patch21_c3 =
      new CurveAdaptiveParametricBezier(*p2103, *p2113, *p2123, *p2133);
  CurveAdaptive* patch21_c4 =
      new CurveAdaptiveParametricBezier(*p2100, *p2101, *p2102, *p2103);

  Patch* patch21 = new PatchBezier(patch21_c1, patch21_c2, patch21_c3,
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
  PointAdaptive* p2210 = new VertexAdaptive(-0.05000, -0.08660, 3.15000);
  PointAdaptive* p2220 = new VertexAdaptive(-0.08660, -0.05000, 3.15000);
  PointAdaptive* p2230 = new VertexAdaptive(-0.10000, 0.00000, 3.15000);

  //		Ponto* p2131 = p2131; //new Vertice (  0.00000,-0.80000, 3.15000
  //);
  PointAdaptive* p2211 = new VertexAdaptive(-0.45000, -0.80000, 3.15000);
  PointAdaptive* p2221 = new VertexAdaptive(-0.80000, -0.45000, 3.15000);
  PointAdaptive* p2231 = new VertexAdaptive(-0.80000, 0.00000, 3.15000);

  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // ); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000,
  // 0.00000, 2.85000 );

  //		Ponto* p2133 = p2133; //new Vertice (  0.00000,-0.20000, 2.70000
  //);
  PointAdaptive* p2213 = new VertexAdaptive(-0.11200, -0.20000, 2.70000);
  PointAdaptive* p2223 = new VertexAdaptive(-0.20000, -0.11200, 2.70000);
  PointAdaptive* p2233 = new VertexAdaptive(-0.20000, 0.00000, 2.70000);

  CurveAdaptive* patch22_c1 =
      new CurveAdaptiveParametricBezier(*p2130, *p2210, *p2220, *p2230);
  CurveAdaptive* patch22_c2 =
      new CurveAdaptiveParametricBezier(*p2230, *p2231, *p2102, *p2233);
  CurveAdaptive* patch22_c3 =
      new CurveAdaptiveParametricBezier(*p2133, *p2213, *p2223, *p2233);
  //		Curva* patch22_c4 = new CurvParamBezier ( *p2130, *p2131,
  //*p2102, *p2133 );

  Patch* patch22 = new PatchBezier(patch22_c1, patch22_c2, patch22_c3,
                                   patch21_c2, *p2211, *p2221, *p2102, *p2102);

  geometry->InsertPatch(patch22);
  geometry->InsertCurve(patch22_c1);
  geometry->InsertCurve(patch22_c2);
  geometry->InsertCurve(patch22_c3);
  //		geo->InsertCurve ( patch22_c4 );
  //======================== FIM DO PATCH 22 =================================

  //============================== PATCH 23 ==================================
  //		Ponto* p2230 = new Vertice (-0.10000, 0.00000, 3.15000 );
  PointAdaptive* p2310 = new VertexAdaptive(-0.08660, 0.05000, 3.15000);
  PointAdaptive* p2320 = new VertexAdaptive(-0.05000, 0.08660, 3.15000);
  PointAdaptive* p2330 = new VertexAdaptive(0.00000, 0.10000, 3.15000);

  //		Ponto* p2231 = new Vertice ( -0.80000, 0.00000, 3.15000 );
  PointAdaptive* p2311 = new VertexAdaptive(-0.80000, 0.45000, 3.15000);
  PointAdaptive* p2321 = new VertexAdaptive(-0.45000, 0.80000, 3.15000);
  PointAdaptive* p2331 = new VertexAdaptive(0.00000, 0.80000, 3.15000);

  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // ); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000,
  // 0.00000, 2.85000 );

  //		Ponto* p2233 = new Vertice ( -0.20000, 0.00000, 2.70000 );
  PointAdaptive* p2313 = new VertexAdaptive(-0.20000, 0.11200, 2.70000);
  PointAdaptive* p2323 = new VertexAdaptive(-0.11200, 0.20000, 2.70000);
  PointAdaptive* p2333 = new VertexAdaptive(0.00000, 0.20000, 2.70000);

  CurveAdaptive* patch23_c1 =
      new CurveAdaptiveParametricBezier(*p2230, *p2310, *p2320, *p2330);
  CurveAdaptive* patch23_c2 =
      new CurveAdaptiveParametricBezier(*p2330, *p2331, *p2102, *p2333);
  CurveAdaptive* patch23_c3 =
      new CurveAdaptiveParametricBezier(*p2233, *p2313, *p2323, *p2333);
  //		Curva* patch23_c4 = new CurvParamBezier ( *p2230, *p2231,
  //*p2102, *p2233 );

  Patch* patch23 = new PatchBezier(patch23_c1, patch23_c2, patch23_c3,
                                   patch22_c2, *p2311, *p2321, *p2102, *p2102);

  geometry->InsertPatch(patch23);
  geometry->InsertCurve(patch23_c1);
  geometry->InsertCurve(patch23_c2);
  geometry->InsertCurve(patch23_c3);
  //		geo->InsertCurve ( patch23_c4 );
  //======================== FIM DO PATCH 23 =================================

  //============================== PATCH 24 ==================================
  //		Ponto* p2330 = new Vertice ( 0.00000, 0.10000, 3.15000 );
  PointAdaptive* p2410 = new VertexAdaptive(0.05000, 0.08660, 3.15000);
  PointAdaptive* p2420 = new VertexAdaptive(0.08660, 0.05000, 3.15000);
  //		Ponto* p2100 = new Vertice ( 0.10000, 0.00000, 3.15000 );

  //		Ponto* p2331 = new Vertice ( 0.00000, 0.80000, 3.15000 );
  PointAdaptive* p2411 = new VertexAdaptive(0.45000, 0.80000, 3.15000);
  PointAdaptive* p2421 = new VertexAdaptive(0.80000, 0.45000, 3.15000);
  PointAdaptive* p2431 = p2101;  // new Vertice ( 0.80000, 0.00000, 3.15000 );

  //		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  //); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000, 0.00000, 2.85000
  // ); 		Ponto* p2102 = p2102; //new Vertice ( 0.00000,
  // 0.00000, 2.85000 );

  //		Ponto* p2333 = new Vertice ( 0.00000, 0.20000, 2.70000 );
  PointAdaptive* p2413 = new VertexAdaptive(0.11200, 0.20000, 2.70000);
  PointAdaptive* p2423 = new VertexAdaptive(0.20000, 0.11200, 2.70000);
  //		Ponto* p2103 = p2103; //new Vertice ( 0.20000, 0.00000, 2.70000
  //);

  CurveAdaptive* patch24_c1 =
      new CurveAdaptiveParametricBezier(*p2330, *p2410, *p2420, *p2100);
  CurveAdaptive* patch24_c2 =
      new CurveAdaptiveParametricBezier(*p2100, *p2431, *p2102, *p2103);
  CurveAdaptive* patch24_c3 =
      new CurveAdaptiveParametricBezier(*p2333, *p2413, *p2423, *p2103);
  //		Curva* patch24_c4 = new CurvParamBezier ( *p2330, *p2331,
  //*p2102, *p2333 );

  Patch* patch24 = new PatchBezier(patch24_c1, patch24_c2, patch24_c3,
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

  PointAdaptive* p2501 = new VertexAdaptive(0.40000, 0.00000, 2.55000);
  PointAdaptive* p2511 = new VertexAdaptive(0.40000, -0.22400, 2.55000);
  PointAdaptive* p2521 = new VertexAdaptive(0.22400, -0.40000, 2.55000);
  PointAdaptive* p2531 = new VertexAdaptive(0.00000, -0.40000, 2.55000);

  PointAdaptive* p2502 = new VertexAdaptive(1.30000, 0.00000, 2.55000);
  PointAdaptive* p2512 = new VertexAdaptive(1.30000, -0.72800, 2.55000);
  PointAdaptive* p2522 = new VertexAdaptive(0.72800, -1.30000, 2.55000);
  PointAdaptive* p2532 = new VertexAdaptive(0.00000, -1.30000, 2.55000);

  PointAdaptive* p2503 = new VertexAdaptive(1.30000, 0.00000, 2.40000);
  PointAdaptive* p2513 = new VertexAdaptive(1.30000, -0.72800, 2.40000);
  PointAdaptive* p2523 = new VertexAdaptive(0.72800, -1.30000, 2.40000);
  PointAdaptive* p2533 = new VertexAdaptive(0.00000, -1.30000, 2.40000);

  //		Curva* patch25_c1 = new CurvParamBezier ( *p2103, *p2113,
  //*p2123, *p2133 );
  CurveAdaptive* patch25_c2 =
      new CurveAdaptiveParametricBezier(*p2133, *p2531, *p2532, *p2533);
  CurveAdaptive* patch25_c3 =
      new CurveAdaptiveParametricBezier(*p2503, *p2513, *p2523, *p2533);
  CurveAdaptive* patch25_c4 =
      new CurveAdaptiveParametricBezier(*p2103, *p2501, *p2502, *p2503);

  Patch* patch25 = new PatchBezier(patch21_c3, patch25_c2, patch25_c3,
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
  PointAdaptive* p2611 = new VertexAdaptive(-0.22400, -0.40000, 2.55000);
  PointAdaptive* p2621 = new VertexAdaptive(-0.40000, -0.22400, 2.55000);
  PointAdaptive* p2631 = new VertexAdaptive(-0.40000, 0.00000, 2.55000);

  //		Ponto* p2532 = new Vertice ( 0.00000,-1.30000, 2.55000 );
  PointAdaptive* p2612 = new VertexAdaptive(-0.72800, -1.30000, 2.55000);
  PointAdaptive* p2622 = new VertexAdaptive(-1.30000, -0.72800, 2.55000);
  PointAdaptive* p2632 = new VertexAdaptive(-1.30000, 0.00000, 2.55000);

  //		Ponto* p2533 = new Vertice ( 0.00000,-1.30000, 2.40000 );
  PointAdaptive* p2613 = new VertexAdaptive(-0.72800, -1.30000, 2.40000);
  PointAdaptive* p2623 = new VertexAdaptive(-1.30000, -0.72800, 2.40000);
  PointAdaptive* p2633 = new VertexAdaptive(-1.30000, 0.00000, 2.40000);

  //		Curva* patch26_c1 = new CurvParamBezier ( *p2133, *p2213,
  //*p2223, *p2233 );
  CurveAdaptive* patch26_c2 =
      new CurveAdaptiveParametricBezier(*p2233, *p2631, *p2632, *p2633);
  CurveAdaptive* patch26_c3 =
      new CurveAdaptiveParametricBezier(*p2533, *p2613, *p2623, *p2633);
  //		Curva* patch26_c4 = new CurvParamBezier ( *p2133, *p2531,
  //*p2532, *p2533 );

  Patch* patch26 = new PatchBezier(patch22_c3, patch26_c2, patch26_c3,
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
  PointAdaptive* p2711 = new VertexAdaptive(-0.40000, 0.22400, 2.55000);
  PointAdaptive* p2721 = new VertexAdaptive(-0.22400, 0.40000, 2.55000);
  PointAdaptive* p2731 = new VertexAdaptive(0.00000, 0.40000, 2.55000);

  //		Ponto* p2632 = new Vertice ( -1.30000, 0.00000, 2.55000 );
  PointAdaptive* p2712 = new VertexAdaptive(-1.30000, 0.72800, 2.55000);
  PointAdaptive* p2722 = new VertexAdaptive(-0.72800, 1.30000, 2.55000);
  PointAdaptive* p2732 = new VertexAdaptive(0.00000, 1.30000, 2.55000);

  //		Ponto* p2633 = new Vertice ( -1.30000, 0.00000, 2.40000 );
  PointAdaptive* p2713 = new VertexAdaptive(-1.30000, 0.72800, 2.40000);
  PointAdaptive* p2723 = new VertexAdaptive(-0.72800, 1.30000, 2.40000);
  PointAdaptive* p2733 = new VertexAdaptive(0.00000, 1.30000, 2.40000);

  //		Curva* patch27_c1 = new CurvParamBezier ( *p2233, *p2313,
  //*p2323, *p2333 );
  CurveAdaptive* patch27_c2 =
      new CurveAdaptiveParametricBezier(*p2333, *p2731, *p2732, *p2733);
  CurveAdaptive* patch27_c3 =
      new CurveAdaptiveParametricBezier(*p2633, *p2713, *p2723, *p2733);
  //		Curva* patch27_c4 = new CurvParamBezier ( *p2233, *p2631,
  //*p2632, *p2633 );

  Patch* patch27 = new PatchBezier(patch23_c3, patch27_c2, patch27_c3,
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
  PointAdaptive* p2811 = new VertexAdaptive(0.22400, 0.40000, 2.55000);
  PointAdaptive* p2821 = new VertexAdaptive(0.40000, 0.22400, 2.55000);
  //		Ponto* p2501 = new Vertice ( 0.40000, 0.00000, 2.55000 );

  //		Ponto* p2732 = new Vertice ( 0.00000, 1.30000, 2.55000 );
  PointAdaptive* p2812 = new VertexAdaptive(0.72800, 1.30000, 2.55000);
  PointAdaptive* p2822 = new VertexAdaptive(1.30000, 0.72800, 2.55000);
  //		Ponto* p2502 = new Vertice ( 1.30000, 0.00000, 2.55000 );

  //		Ponto* p2733 = new Vertice ( 0.00000, 1.30000, 2.40000 );
  PointAdaptive* p2813 = new VertexAdaptive(0.72800, 1.30000, 2.40000);
  PointAdaptive* p2823 = new VertexAdaptive(1.30000, 0.72800, 2.40000);
  //		Ponto* p2503 = new Vertice ( 1.30000, 0.00000, 2.40000 );

  //		Curva* patch28_c1 = new CurvParamBezier ( *p2333, *p2413,
  //*p2423,
  //*p2103 ); 		Curva* patch28_c2 = new CurvParamBezier ( *p2103,
  //*p2501, *p2502, *p2503 );
  CurveAdaptive* patch28_c3 =
      new CurveAdaptiveParametricBezier(*p2733, *p2813, *p2823, *p2503);
  //		Curva* patch28_c4 = new CurvParamBezier ( *p2333, *p2731,
  //*p2732, *p2733 );

  Patch* patch28 = new PatchBezier(patch24_c3, patch25_c4, patch28_c3,
                                   patch27_c2, *p2811, *p2821, *p2812, *p2822);

  geometry->InsertPatch(patch28);
  //		geo->InsertCurve ( patch28_c1 );
  //		geo->InsertCurve ( patch28_c2 );
  geometry->InsertCurve(patch28_c3);
  //		geo->InsertCurve ( patch28_c4 );
  //======================== FIM DO PATCH 28 =================================

  //============================== PATCH 29 ==================================
  PointAdaptive* p2900 = new VertexAdaptive(0.10000, 0.00000, 0.00001);
  PointAdaptive* p2910 = new VertexAdaptive(0.08660, 0.05000, 0.00001);
  PointAdaptive* p2920 = new VertexAdaptive(0.05000, 0.08660, 0.00001);
  PointAdaptive* p2930 = new VertexAdaptive(0.00000, 0.10000, 0.00001);

  PointAdaptive* p2901 = new VertexAdaptive(1.42500, 0.00000, 0.00000);
  PointAdaptive* p2911 = new VertexAdaptive(1.42500, 0.79800, 0.00000);
  PointAdaptive* p2921 = new VertexAdaptive(0.79800, 1.42500, 0.00000);
  PointAdaptive* p2931 = new VertexAdaptive(0.00000, 1.42500, 0.00000);

  PointAdaptive* p2902 = new VertexAdaptive(1.50000, 0.00000, 0.07500);
  PointAdaptive* p2912 = new VertexAdaptive(1.50000, 0.84000, 0.07500);
  PointAdaptive* p2922 = new VertexAdaptive(0.84000, 1.50000, 0.07500);
  PointAdaptive* p2932 = new VertexAdaptive(0.00000, 1.50000, 0.07500);

  //		Ponto* p903 = p903; //new Vertice ( 1.50000, 0.00000, 0.15000 );
  //		Ponto* p1223 = new Vertice ( 1.50000, 0.84000, 0.15000 );
  //		Ponto* p1213 = new Vertice ( 0.84000, 1.50000, 0.15000 );
  //		Ponto* p1133 = p1133; //new Vertice ( 0.00000, 1.50000, 0.15000
  //);

  CurveAdaptive* patch29_c1 =
      new CurveAdaptiveParametricBezier(*p2900, *p2910, *p2920, *p2930);
  CurveAdaptive* patch29_c2 =
      new CurveAdaptiveParametricBezier(*p2930, *p2931, *p2932, *p1133);
  CurveAdaptive* patch29_c3 =
      new CurveAdaptiveParametricBezier(*p903, *p1223, *p1213, *p1133);
  CurveAdaptive* patch29_c4 =
      new CurveAdaptiveParametricBezier(*p2900, *p2901, *p2902, *p903);

  Patch* patch29 = new PatchBezier(patch29_c1, patch29_c2, patch29_c3,
                                   patch29_c4, *p2911, *p2921, *p2912, *p2922);

  geometry->InsertPatch(patch29);
  geometry->InsertCurve(patch29_c1);
  geometry->InsertCurve(patch29_c2);
  geometry->InsertCurve(patch29_c3);
  geometry->InsertCurve(patch29_c4);
  //======================== FIM DO PATCH 29 =================================

  //============================== PATCH 30 ==================================
  //		Ponto* p2930 = new Vertice ( 0.00000, 0.10000, 0.00001 );
  PointAdaptive* p3010 = new VertexAdaptive(-0.05000, 0.08660, 0.00001);
  PointAdaptive* p3020 = new VertexAdaptive(-0.08660, 0.05000, 0.00001);
  PointAdaptive* p3030 = new VertexAdaptive(-0.10000, 0.00000, 0.00001);

  //		Ponto* p2931 = new Vertice (  0.00000, 1.42500, 0.00000 );
  PointAdaptive* p3011 = new VertexAdaptive(-0.79800, 1.42500, 0.00000);
  PointAdaptive* p3021 = new VertexAdaptive(-1.42500, 0.79800, 0.00000);
  PointAdaptive* p3031 = new VertexAdaptive(-1.42500, 0.00000, 0.00000);

  //		Ponto* p2932 = new Vertice ( 0.00000, 1.50000, 0.07500 );
  PointAdaptive* p3012 = new VertexAdaptive(-0.84000, 1.50000, 0.07500);
  PointAdaptive* p3022 = new VertexAdaptive(-1.50000, 0.84000, 0.07500);
  PointAdaptive* p3032 = new VertexAdaptive(-1.50000, 0.00000, 0.07500);

  //		Ponto* p1133 = p1133; //new Vertice ( 0.00000, 1.50000, 0.15000
  //); 		Ponto* p1123 = p1123; //new Vertice (-0.84000, 1.50000, 0.15000
  //); 		Ponto* p1113 = p1113; //new Vertice (-1.50000, 0.84000, 0.15000
  // ); 		Ponto* p1033 = new Vertice (-1.50000, 0.00000, 0.15000
  // );

  CurveAdaptive* patch30_c1 =
      new CurveAdaptiveParametricBezier(*p2930, *p3010, *p3020, *p3030);
  CurveAdaptive* patch30_c2 =
      new CurveAdaptiveParametricBezier(*p3030, *p3031, *p3032, *p1033);
  CurveAdaptive* patch30_c3 =
      new CurveAdaptiveParametricBezier(*p1133, *p1123, *p1113, *p1033);
  //		Curva* patch30_c4 = new CurvParamBezier ( *p2930, *p2931,
  //*p2932, *p1133 );

  Patch* patch30 = new PatchBezier(patch30_c1, patch30_c2, patch30_c3,
                                   patch29_c2, *p3011, *p3021, *p3012, *p3022);

  geometry->InsertPatch(patch30);
  geometry->InsertCurve(patch30_c1);
  geometry->InsertCurve(patch30_c2);
  geometry->InsertCurve(patch30_c3);
  //		geo->InsertCurve ( patch30_c4 );
  //======================== FIM DO PATCH 30 =================================

  //============================== PATCH 31 ==================================
  //		Ponto* p3030 = new Vertice (-0.10000, 0.00000, 0.00001 );
  PointAdaptive* p3110 = new VertexAdaptive(-0.08660, -0.05000, 0.00001);
  PointAdaptive* p3120 = new VertexAdaptive(-0.05000, -0.08660, 0.00001);
  PointAdaptive* p3130 = new VertexAdaptive(0.00000, -0.10000, 0.00001);

  //		Ponto* p3031 = new Vertice ( -1.42500, 0.00000, 0.00000 );
  PointAdaptive* p3111 = new VertexAdaptive(-1.42500, -0.79800, 0.00000);
  PointAdaptive* p3121 = new VertexAdaptive(-0.79800, -1.42500, 0.00000);
  PointAdaptive* p3131 = new VertexAdaptive(0.00000, -1.42500, 0.00000);

  //		Ponto* p3032 = new Vertice ( -1.50000, 0.00000, 0.07500 );
  PointAdaptive* p3112 = new VertexAdaptive(-1.50000, -0.84000, 0.07500);
  PointAdaptive* p3122 = new VertexAdaptive(-0.84000, -1.50000, 0.07500);
  PointAdaptive* p3132 = new VertexAdaptive(0.00000, -1.50000, 0.07500);

  //		Ponto* p1033 = new Vertice ( -1.50000, 0.00000, 0.15000 );
  //		Ponto* p1023 = new Vertice ( -1.50000,-0.84000, 0.15000 );
  //		Ponto* p1013 = new Vertice ( -0.84000,-1.50000, 0.15000 );
  //		Ponto* p933 = p933; //new Vertice (  0.00000,-1.50000, 0.15000
  //);

  CurveAdaptive* patch31_c1 =
      new CurveAdaptiveParametricBezier(*p3030, *p3110, *p3120, *p3130);
  CurveAdaptive* patch31_c2 =
      new CurveAdaptiveParametricBezier(*p3130, *p3131, *p3132, *p933);
  CurveAdaptive* patch31_c3 =
      new CurveAdaptiveParametricBezier(*p1033, *p1023, *p1013, *p933);
  //		Curva* patch31_c4 = new CurvParamBezier ( *p3030, *p3031,
  //*p3032, *p1033 );

  Patch* patch31 = new PatchBezier(patch31_c1, patch31_c2, patch31_c3,
                                   patch30_c2, *p3111, *p3121, *p3112, *p3122);

  geometry->InsertPatch(patch31);
  geometry->InsertCurve(patch31_c1);
  geometry->InsertCurve(patch31_c2);
  geometry->InsertCurve(patch31_c3);
  //		geo->InsertCurve ( patch31_c4 );
  //======================== FIM DO PATCH 31 =================================

  //============================== PATCH 32 ==================================
  //		Ponto* p3130 = new Vertice ( 0.00000,-0.10000, 0.00001 );
  PointAdaptive* p3210 = new VertexAdaptive(0.05000, -0.08660, 0.00001);
  PointAdaptive* p3220 = new VertexAdaptive(0.08660, -0.05000, 0.00001);
  //		Ponto* p2900 = new Vertice ( 0.10000, 0.00000, 0.00001 );

  //		Ponto* p3131 = new Vertice ( 0.00000,-1.42500, 0.00000 );
  PointAdaptive* p3211 = new VertexAdaptive(0.79800, -1.42500, 0.00000);
  PointAdaptive* p3221 = new VertexAdaptive(1.42500, -0.79800, 0.00000);
  //		Ponto* p2901 = new Vertice ( 1.42500, 0.00000, 0.00000 );

  //		Ponto* p3132 = new Vertice ( 0.00000,-1.50000, 0.07500 );
  PointAdaptive* p3212 = new VertexAdaptive(0.84000, -1.50000, 0.07500);
  PointAdaptive* p3222 = new VertexAdaptive(1.50000, -0.84000, 0.07500);
  //		Ponto* p2902 = new Vertice ( 1.50000, 0.00000, 0.07500 );

  //		Ponto* p933 = p933; //new Vertice ( 0.00000,-1.50000, 0.15000 );
  //		Ponto* p923 = p923; //new Vertice ( 0.84000,-1.50000, 0.15000 );
  //		Ponto* p913 = p913; //new Vertice ( 1.50000,-0.84000, 0.15000 );
  //		Ponto* p903 = p903; //new Vertice ( 1.50000, 0.00000, 0.15000 );

  CurveAdaptive* patch32_c1 =
      new CurveAdaptiveParametricBezier(*p3130, *p3210, *p3220, *p2900);
  //		Curva* patch32_c2 = new CurvParamBezier ( *p2900, *p2901,
  //*p2902, *p903 );
  CurveAdaptive* patch32_c3 =
      new CurveAdaptiveParametricBezier(*p933, *p923, *p913, *p903);
  //		Curva* patch32_c4 = new CurvParamBezier ( *p3130, *p3131,
  //*p3132, *p933 );

  Patch* patch32 = new PatchBezier(patch32_c1, patch29_c4, patch32_c3,
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
  return geometry;
}
