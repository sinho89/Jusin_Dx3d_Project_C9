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


Material material_4_ {
 1.000000;1.000000;1.000000;1.000000;;
 32.000000;
 0.100000;0.100000;0.100000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Stage_3th_soul_valley_b.dds";
 }
}

Frame mesh69 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-205.823091,74.000595,179.129382,1.000000;;
 }

 Mesh mesh69 {
  28;
  -129.244800;-83.634598;337.490995;,
  -133.830003;-83.634598;328.016384;,
  -130.513203;-83.634598;333.501594;,
  -123.780595;-83.634598;339.202790;,
  -116.112589;-83.634598;342.544794;,
  -111.731392;-83.634598;342.156006;,
  -106.130394;-83.634598;338.791782;,
  -99.700205;-83.634598;337.823386;,
  -97.243795;-83.634598;329.636378;,
  -90.772794;-83.634598;324.736794;,
  -85.565402;-83.634598;319.337984;,
  -84.055802;-83.634598;312.724173;,
  -86.675398;-83.634598;308.009984;,
  -87.547795;-83.634598;301.924198;,
  -87.241798;-83.634598;297.098394;,
  -90.047398;-83.634598;292.674586;,
  -94.961997;-83.634598;290.669978;,
  -99.853197;-83.634598;290.514611;,
  -105.146394;-83.634598;288.912589;,
  -109.969792;-83.634598;285.959424;,
  -113.941197;-83.634598;282.377421;,
  -119.423398;-83.634598;283.320576;,
  -128.636992;-83.634598;287.691008;,
  -134.704205;-83.634598;292.948224;,
  -137.792998;-83.634598;298.800563;,
  -136.921203;-83.634598;313.729178;,
  -141.109798;-83.634598;322.621773;,
  -135.763200;-83.634598;306.258611;;
  26;
  3;0,1,2;,
  3;3,1,0;,
  3;4,1,3;,
  3;5,1,4;,
  3;6,1,5;,
  3;7,1,6;,
  3;8,1,7;,
  3;9,1,8;,
  3;10,1,9;,
  3;11,1,10;,
  3;12,1,11;,
  3;13,1,12;,
  3;14,1,13;,
  3;15,1,14;,
  3;16,1,15;,
  3;17,1,16;,
  3;18,1,17;,
  3;19,1,18;,
  3;20,1,19;,
  3;21,1,20;,
  3;22,1,21;,
  3;23,1,22;,
  3;24,25,26;,
  3;27,25,24;,
  3;24,26,1;,
  3;24,1,23;;

  MeshNormals {
   28;
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;-1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;-1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;-1.000000;0.000000;;
   26;
   3;0,1,2;,
   3;3,1,0;,
   3;4,1,3;,
   3;5,1,4;,
   3;6,1,5;,
   3;7,1,6;,
   3;8,1,7;,
   3;9,1,8;,
   3;10,1,9;,
   3;11,1,10;,
   3;12,1,11;,
   3;13,1,12;,
   3;14,1,13;,
   3;15,1,14;,
   3;16,1,15;,
   3;17,1,16;,
   3;18,1,17;,
   3;19,1,18;,
   3;20,1,19;,
   3;21,1,20;,
   3;22,1,21;,
   3;23,1,22;,
   3;24,25,26;,
   3;27,25,24;,
   3;24,26,1;,
   3;24,1,23;;
  }

  MeshMaterialList {
   1;
   26;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { material_4_ }
  }

  MeshTextureCoords c1 {
   28;
   0.731988;0.426067;,
   0.996910;0.685648;,
   0.822212;0.543930;,
   0.534754;0.439063;,
   0.246340;0.425646;,
   0.109983;0.495323;,
   -0.028878;0.680850;,
   -0.224090;0.796488;,
   -0.201971;1.103685;,
   -0.349862;1.352137;,
   -0.450882;1.601043;,
   -0.417722;1.843078;,
   -0.275009;1.967871;,
   -0.171629;2.161366;,
   -0.121804;2.327696;,
   0.023304;2.440306;,
   0.206466;2.444287;,
   0.365998;2.386341;,
   0.556363;2.371881;,
   0.748289;2.408942;,
   0.920574;2.478180;,
   1.085541;2.375620;,
   1.328362;2.109571;,
   1.458829;1.854323;,
   1.485960;1.617507;,
   1.273230;1.126458;,
   1.298196;0.773135;,
   1.328321;1.392773;;
  }
 }
}