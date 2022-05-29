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


Material material_5_ {
 1.000000;1.000000;1.000000;1.000000;;
 32.000000;
 0.100000;0.100000;0.100000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Stage_3th_soul_valley_f.dds";
 }
}

Frame mesh84 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-205.823091,74.000595,179.129382,1.000000;;
 }

 Mesh mesh84 {
  8;
  248.751002;61.496992;-145.461606;,
  210.483597;60.680998;-137.555994;,
  198.360000;61.496992;-176.443187;,
  221.191194;38.897760;-103.590598;,
  258.772198;41.936877;-111.256794;,
  145.333798;53.519821;-151.498790;,
  141.145190;61.496992;-205.575590;,
  183.466202;32.925658;-87.819002;;
  7;
  3;0,1,2;,
  3;3,0,4;,
  3;0,3,1;,
  3;5,6,2;,
  3;5,2,1;,
  3;7,1,3;,
  3;7,5,1;;

  MeshNormals {
   8;
   -0.037490;-0.930873;-0.363415;,
   0.033092;-0.955715;-0.292427;,
   0.083671;-0.993365;-0.078905;,
   -0.070824;-0.856152;-0.511848;,
   -0.029424;-0.871399;-0.489692;,
   0.124101;-0.969024;-0.213521;,
   0.077059;-0.985473;-0.151342;,
   0.072648;-0.887199;-0.455632;;
   7;
   3;0,1,2;,
   3;3,0,4;,
   3;0,3,1;,
   3;5,6,2;,
   3;5,2,1;,
   3;7,1,3;,
   3;7,5,1;;
  }

  MeshMaterialList {
   1;
   7;
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { material_5_ }
  }

  MeshTextureCoords c1 {
   8;
   0.775382;1.310374;,
   0.474920;1.373548;,
   0.379730;1.062796;,
   0.558995;1.644969;,
   0.854069;1.583708;,
   -0.036615;1.262130;,
   -0.069500;0.830000;,
   0.262791;1.770997;;
  }
 }
}