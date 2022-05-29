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


Material material_0_ {
 1.000000;1.000000;1.000000;1.000000;;
 32.000000;
 0.100000;0.100000;0.100000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Stage_3th_soul_valley_b.dds";
 }
}

Frame mesh56 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-205.823091,74.000595,179.129382,1.000000;;
 }

 Mesh mesh56 {
  8;
  -216.267597;-118.084186;62.857197;,
  -179.396992;-118.084186;115.326605;,
  -155.289600;-118.084186;69.238803;,
  -194.996403;-118.084186;112.490394;,
  -246.756582;-118.084186;120.290394;,
  -208.468198;-118.084186;130.926016;,
  -157.690189;-118.084186;134.155200;,
  -125.945408;-118.084186;92.953197;;
  6;
  3;0,1,2;,
  3;3,1,0;,
  3;3,4,5;,
  3;0,4,3;,
  3;2,6,7;,
  3;1,6,2;;

  MeshNormals {
   8;
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;;
   6;
   3;0,1,2;,
   3;3,1,0;,
   3;3,4,5;,
   3;0,4,3;,
   3;2,6,7;,
   3;1,6,2;;
  }

  MeshMaterialList {
   1;
   6;
   0,
   0,
   0,
   0,
   0,
   0;
   { material_0_ }
  }

  MeshTextureCoords c1 {
   8;
   -6.671390;-0.825106;,
   -5.778074;0.501265;,
   -5.193984;-0.663790;,
   -6.156015;0.429569;,
   -7.410093;0.626732;,
   -6.482419;0.895592;,
   -5.252140;0.977221;,
   -4.483019;-0.064311;;
  }
 }
}