xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material material_2_ {
 1.000000;1.000000;1.000000;1.000000;;
 32.000000;
 0.100000;0.100000;0.100000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Stage_3th_soul_valley_d.dds";
 }
}

Frame mesh81 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-205.823091,74.000595,179.129382,1.000000;;
 }

 Mesh mesh81 {
  16;
  158.122790;127.187405;-199.813798;,
  270.301210;175.210790;-170.519987;,
  243.975603;127.187405;-139.836608;,
  253.873203;175.210790;-220.988979;,
  158.122790;127.187405;-199.813798;,
  243.975603;73.443603;-139.836608;,
  151.380595;62.425203;-205.330189;,
  243.975603;127.187405;-139.836608;,
  243.975603;127.187405;-139.836608;,
  270.301210;78.763200;-170.519987;,
  243.975603;73.443603;-139.836608;,
  270.301210;175.210790;-170.519987;,
  253.873203;175.210790;-220.988979;,
  151.380595;62.425203;-205.330189;,
  251.114982;74.205600;-231.409203;,
  158.122790;127.187405;-199.813798;;
  8;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,10;,
  3;9,8,11;,
  3;12,13,14;,
  3;13,12,15;;

  MeshNormals {
   16;
   0.447675;-0.788657;-0.421435;,
   0.446579;-0.805282;-0.389984;,
   0.439009;-0.642164;-0.628408;,
   0.419803;-0.897269;-0.136649;,
   0.575577;0.006892;-0.817719;,
   0.574066;0.003269;-0.818803;,
   0.576678;0.009548;-0.816916;,
   0.572695;0.000000;-0.819769;,
   -0.758944;0.000000;-0.651156;,
   -0.758944;0.000000;-0.651156;,
   -0.758944;0.000000;-0.651156;,
   -0.758944;0.000000;-0.651156;,
   0.264390;-0.107194;0.958440;,
   0.264861;-0.107648;0.958259;,
   0.263263;-0.106111;0.958871;,
   0.266600;-0.109321;0.957587;;
   8;
   3;0,1,2;,
   3;1,0,3;,
   3;4,5,6;,
   3;5,4,7;,
   3;8,9,10;,
   3;9,8,11;,
   3;12,13,14;,
   3;13,12,15;;
  }

  MeshMaterialList {
   1;
   8;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { material_2_ }
  }

  MeshTextureCoords c1 {
   16;
   -2.780537;-0.073175;,
   -1.757384;0.215719;,
   -1.997492;0.518315;,
   -1.907218;-0.282008;,
   2.886580;1.621673;,
   3.537798;2.022746;,
   2.879350;2.104972;,
   3.537798;1.621673;,
   3.537798;1.621673;,
   3.905329;1.983048;,
   3.537798;2.022746;,
   3.905329;1.263289;,
   2.190025;1.263289;,
   2.879350;2.104972;,
   2.199404;2.017057;,
   2.886580;1.621673;;
  }
 }
}