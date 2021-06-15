#include "../../header/generator/GenerateBezierPatch.h"

GenerateBezierPatch::GenerateBezierPatch()
{
    //Inicialização da Matrix N
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            matrixN[i][j] = 0.0;
        }
    }
    
    //    //Inicialização dos vetores de pontos da malha e pontos de controle
    //    for (int i = 0; i < 48; ++i) {
    //        controlPoints[i] = 0.0;
    //        points[i] = 0.0;
    //    }
    
    matrix16d <<1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0.343,0,0,0,0.441,0,0,0,0.189,0,0,0,0.027,0,0,0,
            0.027,0,0,0,0.189,0,0,0,0.441,0,0,0,0.343,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
            0.343,0.441,0.189,0.027,0,0,0,0,0,0,0,0,0,0,0,0,
            0.117649,0.151263,0.064827,0.009261,0.151263,0.194481,0.083349,0.011907,0.064827,0.083349,0.035721,0.005103,0.009261,0.011907,0.005103,0.000729,
            0.009261,0.011907,0.005103,0.000729,0.064827,0.083349,0.035721,0.005103,0.151263,0.194481,0.083349,0.011907,0.117649,0.151263,0.064827,0.009261,
            0,0,0,0,0,0,0,0,0,0,0,0,0.343,0.441,0.189,0.027,
            0.027,0.189,0.441,0.343,0,0,0,0,0,0,0,0,0,0,0,0,
            0.009261,0.064827,0.151263,0.117649,0.011907,0.083349,0.194481,0.151263,0.005103,0.035721,0.083349,0.064827,0.000729,0.005103,0.011907,0.009261,
            0.000729,0.005103,0.011907,0.009261,0.005103,0.035721,0.083349,0.064827,0.011907,0.083349,0.194481,0.151263,0.009261,0.064827,0.151263,0.117649,
            0,0,0,0,0,0,0,0,0,0,0,0,0.027,0.189,0.441,0.343,
            0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0.343,0,0,0,0.441,0,0,0,0.189,0,0,0,0.027,
            0,0,0,0.027,0,0,0,0.189,0,0,0,0.441,0,0,0,0.343,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1;
    
    //  cout<<matrix16d<<endl;

}

VectorTuple48d GenerateBezierPatch::calculateControlPoints(VectorTuple48d vecPoints)
{
    VectorTuple48d vectorTuple48d;

    Vector16d vec16dX;
    Vector16d vec16dY;
    Vector16d vec16dZ;

    Vector16d vecCpX;
    Vector16d vecCpY;
    Vector16d vecCpZ;
    
    for (VectorTuple48d::iterator it = vecPoints.begin() ; it != vecPoints.end(); ++it) {

        vec16dX <<get<1>(*it)(0,0), get<1>(*it)(3,0), get<1>(*it)(6,0), get<1>(*it)(9,0), get<1>(*it)(12,0),
                get<1>(*it)(15,0), get<1>(*it)(18,0), get<1>(*it)(21,0), get<1>(*it)(24,0), get<1>(*it)(27,0),
                get<1>(*it)(30,0), get<1>(*it)(33,0), get<1>(*it)(36,0), get<1>(*it)(39,0), get<1>(*it)(42,0), get<1>(*it)(45,0);

        vec16dY <<get<1>(*it)(1,0), get<1>(*it)(4,0), get<1>(*it)(7,0), get<1>(*it)(10,0), get<1>(*it)(13,0),
                get<1>(*it)(16,0), get<1>(*it)(19,0), get<1>(*it)(22,0), get<1>(*it)(25,0), get<1>(*it)(28,0),
                get<1>(*it)(31,0), get<1>(*it)(34,0), get<1>(*it)(37,0), get<1>(*it)(40,0), get<1>(*it)(43,0), get<1>(*it)(46,0);

        vec16dZ <<get<1>(*it)(2,0), get<1>(*it)(5,0), get<1>(*it)(8,0), get<1>(*it)(11,0), get<1>(*it)(14,0),
                get<1>(*it)(17,0), get<1>(*it)(20,0), get<1>(*it)(23,0), get<1>(*it)(26,0), get<1>(*it)(29,0),
                get<1>(*it)(32,0), get<1>(*it)(35,0), get<1>(*it)(38,0), get<1>(*it)(41,0), get<1>(*it)(44,0), get<1>(*it)(47,0);

        vecCpX = matrix16d.colPivHouseholderQr().solve(vec16dX);
        vecCpY = matrix16d.colPivHouseholderQr().solve(vec16dY);
        vecCpZ = matrix16d.colPivHouseholderQr().solve(vec16dZ);

        int j = 0;

        Vector48d vector48d;

        for (int i = 0; i < 48; i = i + 3) {
            vector48d(i,0) = vecCpX(j,0);
            vector48d(i+1,0) = vecCpY(j,0);
            vector48d(i+2,0) = vecCpZ(j,0);
            j++;
        }

        TupleVector48d tupleVector48d(get<0>(*it), vector48d);

        vectorTuple48d.push_back(tupleVector48d);

    }

    return vectorTuple48d;
}

void GenerateBezierPatch::calculateMatrixN()
{
    double uv[4] = {0.0, 0.3, 0.7, 1.0};
    
    //Obs: Verificar o ordem pois a mesma não está de arcodo com o mostrado pelo Prof. Creto.
    calculateLineMatrixN(uv[0], uv[0], 0); //1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    calculateLineMatrixN(uv[1], uv[0], 1); //0.343 0 0 0 0.441 0 0 0 0.189 0 0 0 0.027 0 0 0
    calculateLineMatrixN(uv[2], uv[0], 2); //0.027 0 0 0 0.189 0 0 0 0.441 0 0 0 0.343 0 0 0
    calculateLineMatrixN(uv[3], uv[0], 3); //0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0

    calculateLineMatrixN(uv[0], uv[1], 4);//0.343 0.441 0.189 0.027 0 0 0 0 0 0 0 0 0 0 0 0
    calculateLineMatrixN(uv[1], uv[1], 5);//
    calculateLineMatrixN(uv[2], uv[1], 6);
    calculateLineMatrixN(uv[3], uv[1], 7);
    
    calculateLineMatrixN(uv[0], uv[2], 8);
    calculateLineMatrixN(uv[1], uv[2], 9);
    calculateLineMatrixN(uv[2], uv[2], 10);
    calculateLineMatrixN(uv[3], uv[2], 11);
    
    calculateLineMatrixN(uv[0], uv[3], 12);
    calculateLineMatrixN(uv[1], uv[3], 13);
    calculateLineMatrixN(uv[2], uv[3], 14);
    calculateLineMatrixN(uv[3], uv[3], 15);

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            //    cout<<matrixN[i][j]<<",";
        }
        //        cout<<endl;
    }

}

void GenerateBezierPatch::calculateLineMatrixN(double u, double v, int line)
{
    double berns_i = 0.0;
    double berns_j = 0.0;

    int w = 0;

    for (int i = 0; i < 4; ++i) {
        berns_i = unitBernstein(u, 3, i);
        for (int j = 0; j < 4; ++j) {
            berns_j = unitBernstein(v, 3, j);
            matrixN[line][w] = berns_i * berns_j;
            w++;
        }
    }
}

double GenerateBezierPatch::unitBernstein(double u, int n, int i)
{
    return bernstein(n,i)*pow(u,i)*(pow(1-u, n-i));
}

double GenerateBezierPatch::bernstein(int n, int i)
{
    return factorial(n)/(factorial(i)*factorial(n-i));
}

int GenerateBezierPatch::factorial(int n)
{
    if (n == 0){
        return 1;
    }

    return n * factorial(n - 1);
}

VectorTuple48d GenerateBezierPatch::reducePatch(TupleVector48d tuplePoints)
{
    //--------------------------------------------------------------------------------------------------//

    ///rodado no sentido u do patch

    ///inseri os pontos r00, r10, r30, r40, r50, r70, r80

    // r0 == pc00 e r80 == pc30

    //p0 == pc00, p1 == pc10, p2 == pc20, p3 == pc30

    //r00
    //r00 = (get<1>(vecTuplePoints.at(0)));
    r00 << get<1>(tuplePoints)(0,0), get<1>(tuplePoints)(1,0), get<1>(tuplePoints)(2,0);

    //r80
    //r80 = (get<2>(tuplePoints.at(3)));
    r80 << get<1>(tuplePoints)(9,0), get<1>(tuplePoints)(10,0), get<1>(tuplePoints)(11,0);

    Vector3d vec1;
    vec1 << get<1>(tuplePoints)(3,0), get<1>(tuplePoints)(4,0), get<1>(tuplePoints)(5,0);

    Vector3d vec2;
    vec2 << get<1>(tuplePoints)(6,0), get<1>(tuplePoints)(7,0), get<1>(tuplePoints)(8,0);

    //r10
    // r10 = (calcReduceNearMarge(get<2>(tuplePoints.at(0)), get<2>(tuplePoints.at(1))));

    r10 = calcReduceNearMarge(r00, vec1);

    //r30
    //r30 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(0)), get<2>(tuplePoints.at(1)), get<2>(tuplePoints.at(2))));

    r30 = calcReduceNearMidPoint(r00, vec1, vec2);

    //r40
    //r40 = (calcReduceMidPoint(get<2>(tuplePoints.at(0)), get<2>(tuplePoints.at(1)), get<2>(tuplePoints.at(2)), get<2>(tuplePoints.at(3))));
    r40 = calcReduceMidPoint(r00, vec1, vec2, r80);

    //r50
    //r50 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(1)), get<2>(tuplePoints.at(2)), get<2>(tuplePoints.at(3))));
    r50 = calcReduceNearMidPoint(vec1, vec2, r80);

    //r70
    r70 = calcReduceNearMarge(vec2, r80);

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos tr02, tr12, tr32, tr42, tr52, tr72, tr82

    // tr02 == pc01 e tr80 == pc32

    //p0 == pc01, p1 == pc11, p2 == pc21, p3 == pc31

    //r02
    //tr02 = (get<2>(tuplePoints.at(4)));
    tr02 << get<1>(tuplePoints)(12,0), get<1>(tuplePoints)(13,0), get<1>(tuplePoints)(14,0);

    //tr82
    //tr82 = (get<2>(tuplePoints.at(7)));
    tr82 << get<1>(tuplePoints)(21,0), get<1>(tuplePoints)(22,0), get<1>(tuplePoints)(23,0);

    Vector3d vec5;
    vec5 << get<1>(tuplePoints)(15,0), get<1>(tuplePoints)(16,0), get<1>(tuplePoints)(17,0);

    Vector3d vec6;
    vec6 << get<1>(tuplePoints)(18,0), get<1>(tuplePoints)(19,0), get<1>(tuplePoints)(20,0);


    //tr12
    //tr12 = calcReduceNearMarge(get<2>(tuplePoints.at(4)), get<2>(tuplePoints.at(5))));
    tr12 = calcReduceNearMarge(tr02, vec5);

    //tr32
    //tr32 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(4)), get<2>(tuplePoints.at(5)), get<2>(tuplePoints.at(6))));
    tr32 = calcReduceNearMidPoint(tr02, vec5, vec6);

    //tr42
    //tr42 = (calcReduceMidPoint(get<2>(tuplePoints.at(4)), get<2>(tuplePoints.at(5)), get<2>(tuplePoints.at(6)), get<2>(tuplePoints.at(7))));
    tr42 = calcReduceMidPoint(tr02, vec5, vec6, tr82);

    //tr52
    //tr52 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(5)), get<2>(tuplePoints.at(6)), get<2>(tuplePoints.at(7))));
    tr52 = calcReduceNearMidPoint(vec5, vec6, tr82);

    //tr72
    tr72 = calcReduceNearMarge(vec6, tr82);



    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos tr06, tr16, tr36, tr46, tr56, tr76, tr86

    // tr06 == pc01 e tr86 == pc32

    //p0 == pc02, p1 == pc12, p2 == pc22, p3 == pc32

    //tr06
    //tr06 = (get<2>(tuplePoints.at(8)));
    tr06 << get<1>(tuplePoints)(24,0), get<1>(tuplePoints)(25,0), get<1>(tuplePoints)(26,0);

    //tr86
    //tr86 = (get<2>(tuplePoints.at(11)));
    tr86 << get<1>(tuplePoints)(33,0), get<1>(tuplePoints)(34,0), get<1>(tuplePoints)(35,0);

    Vector3d vec9;
    vec9 << get<1>(tuplePoints)(27,0), get<1>(tuplePoints)(28,0), get<1>(tuplePoints)(29,0);

    Vector3d vec10;
    vec10 << get<1>(tuplePoints)(30,0), get<1>(tuplePoints)(31,0), get<1>(tuplePoints)(32,0);

    //tr16
    //tr16 = (calcReduceNearMarge(get<2>(tuplePoints.at(8)), get<2>(tuplePoints.at(9))));
    tr16 = calcReduceNearMarge(tr06, vec9);

    //tr36
    //tr36 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(8)), get<2>(tuplePoints.at(9)), get<2>(tuplePoints.at(10))));
    tr36 = calcReduceNearMidPoint(tr06, vec9, vec10);

    //tr46
    //tr46 = (calcReduceMidPoint(get<2>(tuplePoints.at(8)), get<2>(tuplePoints.at(9)), get<2>(tuplePoints.at(10)), get<2>(tuplePoints.at(11))));
    tr46 = calcReduceMidPoint(tr06, vec9, vec10, tr86);

    //tr56
    //tr56 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(9)), get<2>(tuplePoints.at(10)), get<2>(tuplePoints.at(11))));
    tr56 = calcReduceNearMidPoint(vec9, vec10, tr86);

    //tr76
    //tr76 = (calcReduceNearMarge(get<2>(tuplePoints.at(10)), get<2>(tuplePoints.at(11))));
    tr76 = calcReduceNearMarge(vec10, tr86);



    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r08, r18, r38, r48, r58, r78, r88

    // r08 == pc03 e r88 == pc33

    //p0 == pc03, p1 == pc13, p2 == pc23, p3 == pc33

    //r08
    //r08 = (get<2>(tuplePoints.at(12)));
    r08 << get<1>(tuplePoints)(36,0), get<1>(tuplePoints)(37,0), get<1>(tuplePoints)(38,0);

    //r88
    //r88 = (get<2>(tuplePoints.at(15)));
    r88 << get<1>(tuplePoints)(45,0), get<1>(tuplePoints)(46,0), get<1>(tuplePoints)(47,0);

    Vector3d vec13;
    vec13 << get<1>(tuplePoints)(39,0), get<1>(tuplePoints)(40,0), get<1>(tuplePoints)(41,0);

    Vector3d vec14;
    vec14 << get<1>(tuplePoints)(42,0), get<1>(tuplePoints)(43,0), get<1>(tuplePoints)(44,0);


    //r18
    //r18 = (calcReduceNearMarge(get<2>(tuplePoints.at(12)), get<2>(tuplePoints.at(13))));
    r18 = calcReduceNearMarge(r08, vec13);

    //r38
    //r38 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(12)), get<2>(tuplePoints.at(13)), get<2>(tuplePoints.at(14))));
    r38 = calcReduceNearMidPoint(r08, vec13, vec14);

    // r48
    //r48 = (calcReduceMidPoint(get<2>(tuplePoints.at(12)), get<2>(tuplePoints.at(13)), get<2>(tuplePoints.at(14)), get<2>(tuplePoints.at(15))));
    r48 = calcReduceMidPoint(r08, vec13, vec14, r88);

    //r58
    //r58 = (calcReduceNearMidPoint(get<2>(tuplePoints.at(13)), get<2>(tuplePoints.at(14)), get<2>(tuplePoints.at(15))));
    r58 = calcReduceNearMidPoint(vec13, vec14, r88);

    //r78
    //r78 = (calcReduceNearMarge(get<2>(tuplePoints.at(14)), get<2>(tuplePoints.at(15))));
    r78 = calcReduceNearMarge(vec14, r88);




    ///--------------------------------------------------------------------------------------------------//

    //--------------------------------------------------------------------------------------------------//

    ///rodado no sentido v do patch

    ///inseri os pontos r07, r05, r04, r03, r01

    // r08 e r00 == já foi inseridos

    //p0 == r08, p1 == tr06, p2 == tr02, p3 == r00

    //r07
    r07 = (calcReduceNearMarge(r08, tr06));

    //r05
    r05 = (calcReduceNearMidPoint(r08, tr06, tr02));

    //r04
    r04 = (calcReduceMidPoint(r08, tr06, tr02, r00));

    //r03
    r03 = (calcReduceNearMidPoint(tr06, tr02, r00));

    //r01
    r01 = (calcReduceNearMarge(tr02, r00));

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r17, r15, r14, r13, r11

    // r18 e r10 == já foi inseridos

    //p0 == r18, p1 == tr16, p2 == tr12, p3 == r10

    //r17
    r17 = (calcReduceNearMarge(r18, tr16));

    //r15
    r15 = (calcReduceNearMidPoint(r18, tr16, tr12));

    //r14
    r14 = (calcReduceMidPoint(r18, tr16, tr12, r10));

    //r13
    r13 = (calcReduceNearMidPoint(tr16, tr12, r10));

    //r11
    r11 = (calcReduceNearMarge(tr12, r10));

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r37, r35, r34, r33, r31

    // r38 e r30 == já foi inseridos

    //p0 == r38, p1 == tr36, p2 == tr32, p3 == r30

    //r37
    r37 = (calcReduceNearMarge(r38, tr36));

    //r35
    r35 = (calcReduceNearMidPoint(r38, tr36, tr32));

    //r34
    r34 = (calcReduceMidPoint(r38, tr36, tr32, r30));

    //r33
    r33 = (calcReduceNearMidPoint(tr36, tr32, r30));

    //r31
    r31 = (calcReduceNearMarge(tr32, r30));

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r47, r45, r44, r43, r41

    // r48 e r40 == já foi inseridos

    //p0 == r48, p1 == tr46, p2 == tr42, p3 == r40

    //r47
    r47 = (calcReduceNearMarge(r48, tr46));

    //r45
    r45 = (calcReduceNearMidPoint(r48, tr46, tr42));

    //r44
    r44 = (calcReduceMidPoint(r48, tr46, tr42, r40));

    //r43
    r43 = (calcReduceNearMidPoint(tr46, tr42, r40));

    //r41
    r41 = (calcReduceNearMarge(tr42, r40));

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r57, r55, r54, r53, r51

    // r58 e r50 == já foi inseridos

    //p0 == r58, p1 == tr56, p2 == tr52, p3 == r50

    //r57
    r57 = (calcReduceNearMarge(r58, tr56));

    //r55
    r55 = (calcReduceNearMidPoint(r58, tr56, tr52));

    //r54
    r54 = (calcReduceMidPoint(r58, tr56, tr52, r50));

    //r53
    r53 = (calcReduceNearMidPoint(tr56, tr52, r50));

    //r51
    r51 = (calcReduceNearMarge(tr52, r50));

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r77, r75, r74, r73, r71

    // r78 e r70 == já foi inseridos

    //p0 == r78, p1 == tr76, p2 == tr72, p3 == r70

    //r77
    r77 = (calcReduceNearMarge(r78, tr76));

    //r75
    r75 = (calcReduceNearMidPoint(r78, tr76, tr72));

    //r74
    r74 = (calcReduceMidPoint(r78, tr76, tr72, r70));

    //r73
    r73 = (calcReduceNearMidPoint(tr76, tr72, r70));

    //r71
    r71 = (calcReduceNearMarge(tr72, r70));

    //--------------------------------------------------------------------------------------------------//

    ///inseri os pontos r87, r85, r84, r83, r81

    // r88 e r80 == já foi inseridos

    //p0 == r88, p1 == tr86, p2 == tr82, p3 == r80

    //r87
    r87 = (calcReduceNearMarge(r88, tr86));

    //r85
    r85 = (calcReduceNearMidPoint(r88, tr86, tr82));

    //r84
    r84 = (calcReduceMidPoint(r88, tr86, tr82, r80));

    //r83
    r83 = (calcReduceNearMidPoint(tr86, tr82, r80));

    //r81
    r81 = (calcReduceNearMarge(tr82, r80));

    //--------------------------------------------------------------------------------------------------//
    ///--------------------------------------------------------------------------------------------------//

    //formação dos quatros patches

    //da esquerda para direita, debaixo para cima

    /*
     *  -----------------
     * | patch2 | patch3 |
     * | -------|--------|
     * | patch0 | patch1 |
     *  -----------------
    */

    // Spline::PatchBezier patch0;

    /* patch0 formado pelos pontos de controle
     *
     *   ----------------------
     * | r04 | r14 | r34 | r44 |
     * | ----------------------|
     * | r03 | r13 | r33 | r43 |
     * | ----------------------|
     * | r01 | r11 | r31 | r41 |
     * | ----------------------|
     * | r00 | r10 | r30 | r40 |
     *   ----------------------
    */

    //limpa a lista usada no metodo para entrada dos patches, para reutiliza-la.
    VectorTuple48d vectorTuple48d;
    vectorTuple48d.clear();

    // insere valores no patch0
    Vector48d vector48d0;
    vector48d0 << r00(0,0),r00(1,0), r00(2,0),
            r10(0,0),r10(1,0), r10(2,0),
            r30(0,0),r30(1,0), r30(2,0),
            r40(0,0),r40(1,0), r40(2,0),
            r01(0,0),r01(1,0), r01(2,0),
            r11(0,0),r11(1,0), r11(2,0),
            r31(0,0),r31(1,0), r31(2,0),
            r41(0,0),r41(1,0), r41(2,0),
            r03(0,0),r03(1,0), r03(2,0),
            r13(0,0),r13(1,0), r13(2,0),
            r33(0,0),r33(1,0), r33(2,0),
            r43(0,0),r43(1,0), r43(2,0),
            r04(0,0),r04(1,0), r04(2,0),
            r14(0,0),r14(1,0), r14(2,0),
            r34(0,0),r34(1,0), r34(2,0),
            r44(0,0),r44(1,0), r44(2,0);

    TupleVector48d tupleVec48d0(0,vector48d0);
    vectorTuple48d.push_back(tupleVec48d0);

    //    TuplePoint point00(0,1, r00), point10(0,2,r10), point20(0,3,r30), point30(0,4,r40);
    //    TuplePoint point01(0,5, r01), point11(0,6,r11), point21(0,7,r31), point31(0,8,r41);
    //    TuplePoint point02(0,9, r03), point12(0,10,r13), point22(0,11,r33), point32(0,12,r43);
    //    TuplePoint point03(0,13, r04), point13(0,14,r14), point23(0,15,r34), point33(0,0,r44);

    //insere patch0 na lista
    //    vectorTuplePoints.push_back(point00); vectorTuplePoints.push_back(point10); vectorTuplePoints.push_back(point20); vectorTuplePoints.push_back(point30);
    //    vectorTuplePoints.push_back(point01); vectorTuplePoints.push_back(point11); vectorTuplePoints.push_back(point21); vectorTuplePoints.push_back(point31);
    //    vectorTuplePoints.push_back(point02); vectorTuplePoints.push_back(point12); vectorTuplePoints.push_back(point22); vectorTuplePoints.push_back(point32);
    //    vectorTuplePoints.push_back(point03); vectorTuplePoints.push_back(point13); vectorTuplePoints.push_back(point23); vectorTuplePoints.push_back(point33);


    /* patch1 formado pelos pontos de controle
     *
     *   ----------------------
     * | r44 | r54 | r74 | r84 |
     * | ----------------------|
     * | r43 | r53 | r73 | r83 |
     * | ----------------------|
     * | r41 | r51 | r71 | r81 |
     * | ----------------------|
     * | r40 | r50 | r70 | r80 |
     *   ----------------------
    */


    Vector48d vector48d1;
    vector48d1 << r40(0,0),r40(1,0), r40(2,0),
            r50(0,0),r50(1,0), r50(2,0),
            r70(0,0),r70(1,0), r70(2,0),
            r80(0,0),r80(1,0), r80(2,0),
            r41(0,0),r41(1,0), r41(2,0),
            r51(0,0),r51(1,0), r51(2,0),
            r71(0,0),r71(1,0), r71(2,0),
            r81(0,0),r81(1,0), r81(2,0),
            r43(0,0),r43(1,0), r43(2,0),
            r53(0,0),r53(1,0), r53(2,0),
            r73(0,0),r73(1,0), r73(2,0),
            r83(0,0),r83(1,0), r83(2,0),
            r44(0,0),r44(1,0), r44(2,0),
            r54(0,0),r54(1,0), r54(2,0),
            r74(0,0),r74(1,0), r74(2,0),
            r84(0,0),r84(1,0), r84(2,0);

    TupleVector48d tupleVec48d1(0,vector48d1);
    vectorTuple48d.push_back(tupleVec48d1);

    // insere valores no patch1
    //    TuplePoint point100(1,1, r40), point110(1,2,r50), point120(1,3,r70), point130(1,4,r80);
    //    TuplePoint point101(1,5, r41), point111(1,6,r51), point121(1,7,r71), point131(1,8,r81);
    //    TuplePoint point102(1,9, r43), point112(1,10,r53), point122(1,11,r73), point132(1,12,r83);
    //    TuplePoint point103(1,13, r44), point113(1,14,r54), point123(1,15,r74), point133(1,0,r84);

    //insere patch1 na lista
    //    vectorTuplePoints.push_back(point100); vectorTuplePoints.push_back(point110); vectorTuplePoints.push_back(point120); vectorTuplePoints.push_back(point130);
    //    vectorTuplePoints.push_back(point101); vectorTuplePoints.push_back(point111); vectorTuplePoints.push_back(point121); vectorTuplePoints.push_back(point131);
    //    vectorTuplePoints.push_back(point102); vectorTuplePoints.push_back(point112); vectorTuplePoints.push_back(point122); vectorTuplePoints.push_back(point132);
    //    vectorTuplePoints.push_back(point103); vectorTuplePoints.push_back(point113); vectorTuplePoints.push_back(point123); vectorTuplePoints.push_back(point133);


    /* patch2 formado pelos pontos de controle
     *
     *   ----------------------
     * | r08 | r18 | r38 | r48 |
     * | ----------------------|
     * | r07 | r17 | r37 | r47 |
     * | ----------------------|
     * | r05 | r15 | r35 | r45 |
     * | ----------------------|
     * | r04 | r14 | r34 | r44 |
     *   ----------------------
    */


    Vector48d vector48d2;
    vector48d2 << r04(0,0),r04(1,0), r04(2,0),
            r14(0,0),r14(1,0), r14(2,0),
            r34(0,0),r34(1,0), r34(2,0),
            r44(0,0),r44(1,0), r44(2,0),
            r05(0,0),r05(1,0), r05(2,0),
            r15(0,0),r15(1,0), r15(2,0),
            r35(0,0),r35(1,0), r35(2,0),
            r45(0,0),r45(1,0), r45(2,0),
            r07(0,0),r07(1,0), r07(2,0),
            r17(0,0),r17(1,0), r17(2,0),
            r37(0,0),r37(1,0), r37(2,0),
            r47(0,0),r47(1,0), r47(2,0),
            r08(0,0),r08(1,0), r08(2,0),
            r18(0,0),r18(1,0), r18(2,0),
            r38(0,0),r38(1,0), r38(2,0),
            r48(0,0),r48(1,0), r48(2,0);

    TupleVector48d tupleVec48d2(0,vector48d2);
    vectorTuple48d.push_back(tupleVec48d2);

    // insere valores no patch2
    //    TuplePoint point200(2,1, r04), point210(2,2,r14), point220(2,3,r34), point230(2,4,r44);
    //    TuplePoint point201(2,5, r05), point211(2,6,r15), point221(2,7,r35), point231(2,8,r45);
    //    TuplePoint point202(2,9, r07), point212(2,10,r17), point222(2,11,r37), point232(2,12,r47);
    //    TuplePoint point203(2,13, r08), point213(2,14,r18), point223(2,15,r38), point233(2,0,r48);

    //insere patch2 na lista
    //    vectorTuplePoints.push_back(point200); vectorTuplePoints.push_back(point210); vectorTuplePoints.push_back(point220); vectorTuplePoints.push_back(point230);
    //    vectorTuplePoints.push_back(point201); vectorTuplePoints.push_back(point211); vectorTuplePoints.push_back(point221); vectorTuplePoints.push_back(point231);
    //    vectorTuplePoints.push_back(point202); vectorTuplePoints.push_back(point212); vectorTuplePoints.push_back(point222); vectorTuplePoints.push_back(point232);
    //    vectorTuplePoints.push_back(point203); vectorTuplePoints.push_back(point213); vectorTuplePoints.push_back(point223); vectorTuplePoints.push_back(point233);


    /* patch3 formado pelos pontos de controle
     *
     *   ----------------------
     * | r48 | r58 | r78 | r88 |
     * | ----------------------|
     * | r47 | r57 | r77 | r87 |
     * | ----------------------|
     * | r45 | r55 | r75 | r85 |
     * | ----------------------|
     * | r44 | r54 | r74 | r84 |
     *   ----------------------
    */

    Vector48d vector48d3;
    vector48d3 << r44(0,0),r44(1,0), r44(2,0),
            r54(0,0),r54(1,0), r54(2,0),
            r74(0,0),r74(1,0), r74(2,0),
            r84(0,0),r84(1,0), r84(2,0),
            r45(0,0),r45(1,0), r45(2,0),
            r55(0,0),r55(1,0), r55(2,0),
            r75(0,0),r75(1,0), r75(2,0),
            r85(0,0),r85(1,0), r85(2,0),
            r47(0,0),r47(1,0), r47(2,0),
            r57(0,0),r57(1,0), r57(2,0),
            r77(0,0),r77(1,0), r77(2,0),
            r87(0,0),r87(1,0), r87(2,0),
            r48(0,0),r48(1,0), r48(2,0),
            r58(0,0),r58(1,0), r58(2,0),
            r78(0,0),r78(1,0), r78(2,0),
            r88(0,0),r88(1,0), r88(2,0);

    TupleVector48d tupleVec48d3(0,vector48d3);
    vectorTuple48d.push_back(tupleVec48d3);

    // insere valores no patch3
    //    TuplePoint point300(3,1, r44), point310(3,2,r54), point320(3,3,r74), point330(3,4,r84);
    //    TuplePoint point301(3,5, r45), point311(3,6,r55), point321(3,7,r75), point331(3,8,r85);
    //    TuplePoint point302(3,9, r47), point312(3,10,r57), point322(3,11,r77), point332(3,12,r87);
    //    TuplePoint point303(3,13, r48), point313(3,14,r58), point323(3,15,r78), point333(3,0,r88);

    //insere patch3 na lista
    //    vectorTuplePoints.push_back(point300); vectorTuplePoints.push_back(point310); vectorTuplePoints.push_back(point320); vectorTuplePoints.push_back(point330);
    //    vectorTuplePoints.push_back(point301); vectorTuplePoints.push_back(point311); vectorTuplePoints.push_back(point321); vectorTuplePoints.push_back(point331);
    //    vectorTuplePoints.push_back(point302); vectorTuplePoints.push_back(point312); vectorTuplePoints.push_back(point322); vectorTuplePoints.push_back(point332);
    //    vectorTuplePoints.push_back(point303); vectorTuplePoints.push_back(point313); vectorTuplePoints.push_back(point323); vectorTuplePoints.push_back(point333);

    return vectorTuple48d;

}

VectorTuple48d GenerateBezierPatch::reduceAllPatches(VectorTuple48d vecTuplePoints, int nivel)
{
    VectorTuple48d vectorTuple48dTemp;
    int nivelTemp = 0;
    int size = 0;
    bool increment = true;

    if (nivel == 0) {
        return vecTuplePoints;
    }else{

        while (increment) {

            nivelTemp++;

            size = (int)vectorTuplePointsReduce.size();

            for (VectorTuple48d::iterator it = vecTuplePoints.begin() ; it != vecTuplePoints.end(); ++it) {

                vectorTuple48dTemp = reducePatch((*it));
                addPatchesVectorTuplePointsReduce(vectorTuple48dTemp);
            }

            std::cout<<vectorTuplePointsReduce.size()<<std::endl;

            removePatchesVectorTuplePointsReduce(size);


            std::cout<<vectorTuplePointsReduce.size()<<std::endl;

            if (nivelTemp == nivel) {
                increment = false;
            }

            vecTuplePoints = vectorTuplePointsReduce;
        }

        return vectorTuplePointsReduce;

    }
    
}

void GenerateBezierPatch::addPatchesVectorTuplePointsReduce(VectorTuple48d vectorTuplePoints)
{
    for (VectorTuple48d::iterator it = vectorTuplePoints.begin() ; it != vectorTuplePoints.end(); ++it) {
        vectorTuplePointsReduce.push_back((*it));
    }
}

void GenerateBezierPatch::removePatchesVectorTuplePointsReduce(int size)
{
    vectorTuplePointsReduce.erase(vectorTuplePointsReduce.begin(), vectorTuplePointsReduce.begin()+size);
}

Vector3d GenerateBezierPatch::calcReduceMidPoint(Vector3d p0, Vector3d p1, Vector3d p2, Vector3d p3)
{
    Vector3d rMid;

    for (int i = 0; i < 3; ++i) {
        rMid(i) = (0.125)*(p0(i) + 3*p1(i) + 3*p2(i) + p3(i));
    }

    return rMid;

}

Vector3d GenerateBezierPatch::calcReduceNearMarge(Vector3d p0, Vector3d p1)
{
    Vector3d rNearMarge;

    for (int i = 0; i < 3; ++i) {
        rNearMarge(i) = (0.5)*(p0(i) + p1(i));
    }

    return rNearMarge;
}

Vector3d GenerateBezierPatch::calcReduceNearMidPoint(Vector3d p0, Vector3d p1, Vector3d p2)
{
    Vector3d rNearMid;

    for (int i = 0; i < 3; ++i) {
        rNearMid(i) = (0.25)*(p0(i) + 2*p1(i) + p2(i));
    }

    return rNearMid;
}
