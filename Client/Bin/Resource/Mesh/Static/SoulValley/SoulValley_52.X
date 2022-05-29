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


Material material_1_ {
 1.000000;1.000000;1.000000;1.000000;;
 32.000000;
 0.100000;0.100000;0.100000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Stage_3th_soul_valley_e.dds";
 }
}

Frame mesh47 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-205.823091,74.000595,179.129382,1.000000;;
 }

 Mesh mesh47 {
  12;
  -112.033190;-75.549600;-40.862579;,
  -101.856595;-34.203658;-53.510579;,
  -110.243994;-34.203658;-39.712442;,
  -103.645798;-75.549600;-54.660659;,
  -102.194400;105.837594;-53.718240;,
  -110.555392;105.837594;-39.963840;,
  -114.131994;-75.549600;-40.796278;,
  -110.243994;-34.203658;-39.712442;,
  -114.131994;-34.203658;-40.796278;,
  -112.033190;-75.549600;-40.862579;,
  -110.555392;105.837594;-39.963840;,
  -112.625395;105.837594;-39.945302;;
  8;
  3;0,1,2;,
  3;3,1,0;,
  3;2,4,5;,
  3;1,4,2;,
  3;6,7,8;,
  3;9,7,6;,
  3;8,10,11;,
  3;7,10,8;;

  MeshNormals {
   12;
   0.853385;-0.051359;0.518744;,
   0.854262;-0.024284;0.519276;,
   0.854262;-0.024265;0.519276;,
   0.853385;-0.051359;0.518745;,
   0.854506;0.002832;0.519434;,
   0.854505;0.002833;0.519435;,
   0.013058;-0.027430;0.999538;,
   -0.263820;0.000098;0.964572;,
   -0.267266;0.000530;0.963623;,
   0.031562;-0.029158;0.999076;,
   0.003880;-0.006043;0.999974;,
   0.008957;-0.006173;0.999941;;
   8;
   3;0,1,2;,
   3;3,1,0;,
   3;2,4,5;,
   3;1,4,2;,
   3;6,7,8;,
   3;9,7,6;,
   3;8,10,11;,
   3;7,10,8;;
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
   { material_1_ }
  }

  MeshTextureCoords c1 {
   12;
   0.066631;1.176327;,
   0.467964;0.296379;,
   0.066947;0.296379;,
   0.468759;1.176327;,
   0.474000;-2.684079;,
   0.074253;-2.684079;,
   -0.029716;1.176327;,
   0.066947;0.296379;,
   -0.029716;0.296379;,
   0.066631;1.176327;,
   0.074253;-2.684079;,
   -0.032310;-2.684079;;
  }
 }
}