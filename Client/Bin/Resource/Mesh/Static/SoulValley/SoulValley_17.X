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


Material material_3_ {
 1.000000;1.000000;1.000000;1.000000;;
 32.000000;
 0.100000;0.100000;0.100000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Stage_3th_soul_valley_d.dds";
 }
}

Frame mesh82 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-205.823091,74.000595,179.129382,1.000000;;
 }

 Mesh mesh82 {
  16;
  188.214579;-76.794598;-94.953600;,
  189.646195;-94.768800;-92.048998;,
  191.437210;-76.924800;-92.816390;,
  186.538790;-94.767590;-94.378797;,
  157.741786;-94.256390;-171.905395;,
  147.497677;-75.575994;-200.606067;,
  147.940800;-93.884397;-198.746394;,
  153.820723;-75.575994;-170.342528;,
  161.346598;-94.528794;-153.677990;,
  153.820723;-75.575994;-170.342528;,
  159.235930;-75.575994;-152.837466;,
  164.096998;-75.575994;-138.190182;,
  159.235930;-75.575994;-152.837466;,
  167.318387;-94.758003;-138.834598;,
  181.461005;-76.243194;-114.808794;,
  180.570598;-94.763398;-114.431398;;
  12;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,4;,
  3;9,8,10;,
  3;8,11,12;,
  3;11,8,13;,
  3;14,3,0;,
  3;3,14,15;,
  3;11,15,14;,
  3;15,11,13;;

  MeshNormals {
   16;
   -0.788907;0.098926;0.606497;,
   -0.591432;0.082515;0.802122;,
   -0.547863;0.090754;0.831631;,
   -0.814449;0.082863;0.574288;,
   -0.959276;-0.178562;0.218870;,
   -0.953713;-0.075959;0.290969;,
   -0.939213;0.012121;0.343122;,
   -0.955482;-0.217261;0.199631;,
   -0.943351;-0.157984;0.291771;,
   -0.955889;-0.167282;0.241438;,
   -0.948584;-0.118653;0.293445;,
   -0.896531;-0.112546;0.428446;,
   -0.942374;-0.118818;0.312751;,
   -0.892262;-0.163978;0.420690;,
   -0.886998;0.062476;0.457528;,
   -0.905385;0.011608;0.424432;;
   12;
   3;0,1,2;,
   3;1,0,3;,
   3;4,5,6;,
   3;5,4,7;,
   3;8,9,4;,
   3;9,8,10;,
   3;8,11,12;,
   3;11,8,13;,
   3;14,3,0;,
   3;3,14,15;,
   3;11,15,14;,
   3;15,11,13;;
  }

  MeshMaterialList {
   1;
   12;
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
   { material_3_ }
  }

  MeshTextureCoords c1 {
   16;
   1.087342;1.620028;,
   0.983930;2.095923;,
   0.998227;1.623474;,
   1.076640;2.095895;,
   2.483450;2.082357;,
   3.018406;1.573234;,
   2.983415;2.097373;,
   2.481031;1.588476;,
   2.144005;2.089565;,
   2.469338;1.587762;,
   2.149776;1.593671;,
   1.855591;1.587762;,
   2.123955;1.601575;,
   1.867588;2.095638;,
   1.420162;1.605428;,
   1.413130;2.095779;;
  }
 }
}