{**********************************************************************
 Package pl_Win_DirectX11.pkg
 this unit is part of CodeTyphon Studio (http://www.pilotlogic.com/)
***********************************************************************}

unit D3DX10;

interface

{$Z4}

uses
  Windows, SysUtils,
  DXGITypes, DXGI, D3DCommon, D3D10, D3D10_1;

const
  DLL_D3DX10={$IFDEF DEBUG}'d3dx10d_41.dll'{$ELSE}'d3dx10_41.dll'{$ENDIF};

// =============== D3DX10Math.h =====================

const
  D3DX_PI=(3.14159265358979323846);
  D3DX_1BYPI=1.0 / D3DX_PI;
  D3DX_16F_DIG=3;
  D3DX_16F_EPSILON=4.8875809e-4;
  D3DX_16F_MANT_DIG=11;
  D3DX_16F_MAX=6.550400e+004;
  D3DX_16F_MAX_10_EXP=4;
  D3DX_16F_MAX_EXP=15;
  D3DX_16F_MIN=6.1035156e-5;
  D3DX_16F_MIN_10_EXP=(-4);
  D3DX_16F_MIN_EXP=(-14);
  D3DX_16F_RADIX=2;
  D3DX_16F_ROUNDS=1;
  D3DX_16F_SIGN_MASK=$8000;
  D3DX_16F_EXP_MASK=$7C00;
  D3DX_16F_FRAC_MASK=$03FF;
  D3DXSH_MINORDER=2;
  D3DXSH_MAXORDER=6;

type
  TD3DVector=record
    X:Single;
    Y:Single;
    Z:Single;
  end;
  PTD3DVector=^TD3DVector;
  D3DVECTOR=TD3DVector;
  PD3DVECTOR=^TD3DVector;

  TD3DXFloat16=record
    Value:Word;
  end;
  PTD3DXFloat16=^TD3DXFloat16;
  D3DXFLOAT16=TD3DXFloat16;
  PD3DXFLOAT16=^TD3DXFloat16;

  TD3DXVector2=record
    X:Single;
    Y:Single;
  end;
  PTD3DXVector2=^TD3DXVector2;

function D3DXVector2(X,Y:Single):TD3DXVector2;

type
  TD3DXVector2_16F=record
    X:TD3DXFloat16;
    Y:TD3DXFloat16;
  end;
  PD3DXVector2_16F=^TD3DXVector2_16F;
  PTD3DXVector2_16F=^TD3DXVector2_16F;

  TD3DXVector3=TD3DVECTOR;
  PD3DXVector3=^TD3DXVector3;
  PTD3DXVector3=^TD3DXVector3;

function D3DXVector3(X,Y,Z:Single):TD3DXVector3;

type
  TD3DXVector3_16F=record
    X:TD3DXFloat16;
    Y:TD3DXFloat16;
    Z:TD3DXFloat16;
  end;
  PD3DXVector3_16F=^TD3DXVector3_16F;
  PTD3DXVector3_16F=^TD3DXVector3_16F;

  TD3DXVector4=record
    X:Single;
    Y:Single;
    Z:Single;
    W:Single;
  end;
  PD3DXVector4=^TD3DXVector4;
  PTD3DXVector4=^TD3DXVector4;

function D3DXVector4(X,Y,Z,W:Single):TD3DXVector4;

type
  TD3DXVector4_16F=record
    X:TD3DXFloat16;
    Y:TD3DXFloat16;
    Z:TD3DXFloat16;
    W:TD3DXFloat16;
  end;
  PD3DXVector4_16F=^TD3DXVector4_16F;
  PTD3DXVector4_16F=^TD3DXVector4_16F;

  TD3DXMatrix=packed record
    case Byte of
      0:
      (
        _11,_12,_13,_14:Single;
        _21,_22,_23,_24:Single;
        _31,_32,_33,_34:Single;
        _41,_42,_43,_44:Single
      );

      1:
      (
        m : array [0..3, 0..3] of Single
      );
  end;
  PD3DXMatrix=^TD3DXMatrix;
  PTD3DXMatrix=^TD3DXMatrix;

  TD3DXQuaternion=record
    X:Single;
    Y:Single;
    Z:Single;
    W:Single;
  end;
  PTD3DXQuaternion=^TD3DXQuaternion;
  D3DXQUATERNION=TD3DXQuaternion;
  PD3DXQUATERNION=^TD3DXQuaternion;

  TD3DXPlane=record
    A:Single;
    B:Single;
    C:Single;
    D:Single;
  end;
  PTD3DXPlane=^TD3DXPlane;
  D3DXPLANE=TD3DXPlane;
  PD3DXPLANE=^TD3DXPlane;

  D3DXCOLOR=D3DCOLORVALUE; // DXGITypes
  PD3DXCOLOR=^D3DXCOLOR;
  TD3DXColor=D3DXCOLOR;
  PTD3DXColor=^TD3DXColor;

var D3DXFloat32To16Array: function
(
  out _Out:TD3DXFloat16;  
  const _In:Single;
  N:LongWord
):PTD3DXFloat16; stdcall;  

var D3DXFloat16To32Array: function
(
  pOut:PSingle;
  pIn:PTD3DXFloat16;
  N:LongWord
):PSingle; stdcall;  

function D3DXVec2Length(const V:TD3DXVector2):Single;
function D3DXVec2LengthSq(const V:TD3DXVector2):Single;
function D3DXVec2Dot(const V1:TD3DXVector2;const V2:TD3DXVector2):Single;
function D3DXVec2CCW(const V1:TD3DXVector2;const V2:TD3DXVector2):Single;

function D3DXVec2Add
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const V2:TD3DXVector2
):PTD3DXVector2;

function D3DXVec2Subtract
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const V2:TD3DXVector2
):PTD3DXVector2;

function D3DXVec2Minimize
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const V2:TD3DXVector2
):PTD3DXVector2;

function D3DXVec2Maximize
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const V2:TD3DXVector2
):PTD3DXVector2;

function D3DXVec2Scale
(
  out _Out:TD3DXVector2;  
  const V:TD3DXVector2;
  S:Single
):PTD3DXVector2;

function D3DXVec2Lerp
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const V2:TD3DXVector2;
  S:Single
):PTD3DXVector2;

var D3DXVec2Normalize: function
(
  out _Out:TD3DXVector2;  
  const V:TD3DXVector2
):PTD3DXVector2; stdcall;  

var D3DXVec2Hermite: function
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const T1:TD3DXVector2;
  const V2:TD3DXVector2;
  const T2:TD3DXVector2;
  S:Single
):PTD3DXVector2; stdcall;  

var D3DXVec2CatmullRom: function
(
  out _Out:TD3DXVector2;  
  const V0:TD3DXVector2;
  const V1:TD3DXVector2;
  const V2:TD3DXVector2;
  const V3:TD3DXVector2;
  S:Single
):PTD3DXVector2; stdcall;  

var D3DXVec2BaryCentric: function
(
  out _Out:TD3DXVector2;  
  const V1:TD3DXVector2;
  const V2:TD3DXVector2;
  const V3:TD3DXVector2;
  F:Single;
  G:Single
):PTD3DXVector2; stdcall;  

var D3DXVec2Transform: function
(
  out _Out:TD3DXVector4;  
  const V:TD3DXVector2;
  const M:TD3DXMatrix
):PTD3DXVector4; stdcall;  

var D3DXVec2TransformCoord: function
(
  out _Out:TD3DXVector2;  
  const V:TD3DXVector2;
  const M:TD3DXMatrix
):PTD3DXVector2; stdcall;  

var D3DXVec2TransformNormal: function
(
  out _Out:TD3DXVector2;  
  const V:TD3DXVector2;
  const M:TD3DXMatrix
):PTD3DXVector2; stdcall;  

var D3DXVec2TransformArray: function
(
  out _Out:TD3DXVector4;  
  OutStride:LongWord;
  const V:TD3DXVector2;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector4; stdcall;  

var D3DXVec2TransformCoordArray: function
(
  out _Out:TD3DXVector2;  
  OutStride:LongWord;
  const V:TD3DXVector2;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector2; stdcall;  

var D3DXVec2TransformNormalArray: function
(
  out _Out:TD3DXVector2;  
  OutStride:LongWord;
  const V:TD3DXVector2;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector2; stdcall;  

function D3DXVec3Length(const V:TD3DXVector3):Single;
function D3DXVec3LengthSq(const V:TD3DXVector3):Single;
function D3DXVec3Dot(const V1:TD3DXVector3;const V2:TD3DXVector3):Single;

function D3DXVec3Cross
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3
):PTD3DXVector3;

function D3DXVec3Add
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3
):PTD3DXVector3;

function D3DXVec3Subtract
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3
):PTD3DXVector3;

function D3DXVec3Minimize
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3
):PTD3DXVector3;

function D3DXVec3Maximize
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3
):PTD3DXVector3;

function D3DXVec3Scale
(
  out _Out:TD3DXVector3;  
  const V:TD3DXVector3;
  S:Single
):PTD3DXVector3;

function D3DXVec3Lerp
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3;
  S:Single
):PTD3DXVector3;

var D3DXVec3Normalize: function
(
  out _Out:TD3DXVector3;  
  const V:TD3DXVector3
):PTD3DXVector3; stdcall;  

var D3DXVec3Hermite: function
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const T1:TD3DXVector3;
  const V2:TD3DXVector3;
  const T2:TD3DXVector3;
  S:Single
):PTD3DXVector3; stdcall;  

var D3DXVec3CatmullRom: function
(
  out _Out:TD3DXVector3;  
  const V0:TD3DXVector3;
  const V1:TD3DXVector3;
  const V2:TD3DXVector3;
  const V3:TD3DXVector3;
  S:Single
):PTD3DXVector3; stdcall;  

var D3DXVec3BaryCentric: function
(
  out _Out:TD3DXVector3;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3;
  const V3:TD3DXVector3;
  F:Single;
  G:Single
):PTD3DXVector3; stdcall;  

var D3DXVec3Transform: function
(
  out _Out:TD3DXVector4;  
  const V:TD3DXVector3;
  const M:TD3DXMatrix
):PTD3DXVector4; stdcall;  

var D3DXVec3TransformCoord: function
(
  out _Out:TD3DXVector3;  
  const V:TD3DXVector3;
  const M:TD3DXMatrix
):PTD3DXVector3; stdcall;  

var D3DXVec3TransformNormal: function
(
  out _Out:TD3DXVector3;  
  const V:TD3DXVector3;
  const M:TD3DXMatrix
):PTD3DXVector3; stdcall;  

var D3DXVec3TransformArray: function
(
  out _Out:TD3DXVector4;  
  OutStride:LongWord;
  const V:TD3DXVector3;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector4; stdcall;  

var D3DXVec3TransformCoordArray: function
(
  out _Out:TD3DXVector3;  
  OutStride:LongWord;
  const V:TD3DXVector3;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector3; stdcall;  

var D3DXVec3TransformNormalArray: function
(
  out _Out:TD3DXVector3;  
  OutStride:LongWord;
  const V:TD3DXVector3;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector3; stdcall;  

var D3DXVec3Project: function
(
  out _Out:TD3DXVector3;  
  const V:TD3DXVector3;
  const Viewport:TD3D10_Viewport;
  const Projection:TD3DXMatrix;
  const View:TD3DXMatrix;
  const World:TD3DXMatrix
):PTD3DXVector3; stdcall;  

var D3DXVec3Unproject: function
(
  out _Out:TD3DXVector3;  
  const V:TD3DXVector3;
  const Viewport:TD3D10_Viewport;
  const Projection:TD3DXMatrix;
  const View:TD3DXMatrix;
  const World:TD3DXMatrix
):PTD3DXVector3; stdcall;  

var D3DXVec3ProjectArray: function
(
  out _Out:TD3DXVector3;  
  OutStride:LongWord;
  const V:TD3DXVector3;
  VStride:LongWord;
  const Viewport:TD3D10_Viewport;
  const Projection:TD3DXMatrix;
  const View:TD3DXMatrix;
  const World:TD3DXMatrix;
  N:LongWord
):PTD3DXVector3; stdcall;  

var D3DXVec3UnprojectArray: function
(
  out _Out:TD3DXVector3;  
  OutStride:LongWord;
  const V:TD3DXVector3;
  VStride:LongWord;
  const Viewport:TD3D10_Viewport;
  const Projection:TD3DXMatrix;
  const View:TD3DXMatrix;
  const World:TD3DXMatrix;
  N:LongWord
):PTD3DXVector3; stdcall;  

function D3DXVec4Length(const V:TD3DXVector4):Single;
function D3DXVec4LengthSq(const V:TD3DXVector4):Single;
function D3DXVec4Dot(const V1:TD3DXVector4;const V2:TD3DXVector4):Single;

function D3DXVec4Add
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4
):PTD3DXVector4;

function D3DXVec4Subtract
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4
):PTD3DXVector4;

function D3DXVec4Minimize
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4
):PTD3DXVector4;

function D3DXVec4Maximize
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4
):PTD3DXVector4;

function D3DXVec4Scale
(
  out _Out:TD3DXVector4;  
  const V:TD3DXVector4;
  S:Single
):PTD3DXVector4;

function D3DXVec4Lerp
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4;
  S:Single
):PTD3DXVector4;

var D3DXVec4Cross: function
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4;
  const V3:TD3DXVector4
):PTD3DXVector4; stdcall;  

var D3DXVec4Normalize: function
(
  out _Out:TD3DXVector4;  
  const V:TD3DXVector4
):PTD3DXVector4; stdcall;  

var D3DXVec4Hermite: function
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const T1:TD3DXVector4;
  const V2:TD3DXVector4;
  const T2:TD3DXVector4;
  S:Single
):PTD3DXVector4; stdcall;  

var D3DXVec4CatmullRom: function
(
  out _Out:TD3DXVector4;  
  const V0:TD3DXVector4;
  const V1:TD3DXVector4;
  const V2:TD3DXVector4;
  const V3:TD3DXVector4;
  S:Single
):PTD3DXVector4; stdcall;  

var D3DXVec4BaryCentric: function
(
  out _Out:TD3DXVector4;  
  const V1:TD3DXVector4;
  const V2:TD3DXVector4;
  const V3:TD3DXVector4;
  F:Single;
  G:Single
):PTD3DXVector4; stdcall;  

var D3DXVec4Transform: function
(
  out _Out:TD3DXVector4;  
  const V:TD3DXVector4;
  const M:TD3DXMatrix
):PTD3DXVector4; stdcall;  

var D3DXVec4TransformArray: function
(
  out _Out:TD3DXVector4;  
  OutStride:LongWord;
  const V:TD3DXVector4;
  VStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXVector4; stdcall;  

function D3DXMatrixIdentity:TD3DXMatrix; overload;
function D3DXMatrixIdentity(out M:TD3DXMatrix):PTD3DXMatrix; overload;
function D3DXMatrixIsIdentity(const M:TD3DXMatrix):LongBool;
var D3DXMatrixDeterminant: function(const M:TD3DXMatrix):Single; stdcall;
var D3DXMatrixDecompose: function(pOutScale:PTD3DXVector3;pOutRotation:PTD3DXQuaternion;pOutTranslation:PTD3DXVector3;const M:TD3DXMatrix):HResult; stdcall;

var D3DXMatrixTranspose: function
(
  out _Out:TD3DXMatrix;  
  const M:TD3DXMatrix
):PTD3DXMatrix; stdcall;  

var D3DXMatrixMultiply: function
(
  out _Out:TD3DXMatrix;  
  const M1:TD3DXMatrix;
  const M2:TD3DXMatrix
):PTD3DXMatrix; stdcall;  

var D3DXMatrixMultiplyTranspose: function
(
  out _Out:TD3DXMatrix;  
  const M1:TD3DXMatrix;
  const M2:TD3DXMatrix
):PTD3DXMatrix; stdcall;  

var D3DXMatrixInverse: function
(
  out _Out:TD3DXMatrix;  
  pDeterminant:PSingle;
  const M:TD3DXMatrix
):PTD3DXMatrix; stdcall;  

var D3DXMatrixScaling: function
(
  out _Out:TD3DXMatrix;  
  Sx:Single;
  Sy:Single;
  Sz:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixTranslation: function
(
  out _Out:TD3DXMatrix;  
  X:Single;
  Y:Single;
  Z:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixRotationX: function
(
  out _Out:TD3DXMatrix;  
  Angle:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixRotationY: function
(
  out _Out:TD3DXMatrix;  
  Angle:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixRotationZ: function
(
  out _Out:TD3DXMatrix;  
  Angle:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixRotationAxis: function
(
  out _Out:TD3DXMatrix;  
  const V:TD3DXVector3;
  Angle:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixRotationQuaternion: function
(
  out _Out:TD3DXMatrix;  
  const Q:TD3DXQuaternion
):PTD3DXMatrix; stdcall;  

var D3DXMatrixRotationYawPitchRoll: function
(
  out _Out:TD3DXMatrix;  
  Yaw:Single;
  Pitch:Single;
  Roll:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixTransformation: function
(
  out _Out:TD3DXMatrix;  
  const ScalingCenter:TD3DXVector3;
  const ScalingRotation:TD3DXQuaternion;
  const Scaling:TD3DXVector3;
  const RotationCenter:TD3DXVector3;
  const Rotation:TD3DXQuaternion;
  const Translation:TD3DXVector3
):PTD3DXMatrix; stdcall;  

var D3DXMatrixTransformation2D: function
(
  out _Out:TD3DXMatrix;  
  const ScalingCenter:TD3DXVector2;
  ScalingRotation:Single;
  const Scaling:TD3DXVector2;
  const RotationCenter:TD3DXVector2;
  Rotation:Single;
  const Translation:TD3DXVector2
):PTD3DXMatrix; stdcall;  

var D3DXMatrixAffineTransformation: function
(
  out _Out:TD3DXMatrix;  
  Scaling:Single;
  const RotationCenter:TD3DXVector3;
  const Rotation:TD3DXQuaternion;
  const Translation:TD3DXVector3
):PTD3DXMatrix; stdcall;  

var D3DXMatrixAffineTransformation2D: function
(
  out _Out:TD3DXMatrix;  
  Scaling:Single;
  const RotationCenter:TD3DXVector2;
  Rotation:Single;
  const Translation:TD3DXVector2
):PTD3DXMatrix; stdcall;  

var D3DXMatrixLookAtRH: function
(
  out _Out:TD3DXMatrix;  
  const Eye:TD3DXVector3;
  const At:TD3DXVector3;
  const Up:TD3DXVector3
):PTD3DXMatrix; stdcall;  

var D3DXMatrixLookAtLH: function
(
  out _Out:TD3DXMatrix;  
  const Eye:TD3DXVector3;
  const At:TD3DXVector3;
  const Up:TD3DXVector3
):PTD3DXMatrix; stdcall;  

var D3DXMatrixPerspectiveRH: function
(
  out _Out:TD3DXMatrix;  
  W:Single;
  H:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixPerspectiveLH: function
(
  out _Out:TD3DXMatrix;  
  W:Single;
  H:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixPerspectiveFovRH: function
(
  out _Out:TD3DXMatrix;  
  Fovy:Single;
  Aspect:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixPerspectiveFovLH: function
(
  out _Out:TD3DXMatrix;  
  Fovy:Single;
  Aspect:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixPerspectiveOffCenterRH: function
(
  out _Out:TD3DXMatrix;  
  L:Single;
  R:Single;
  B:Single;
  T:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixPerspectiveOffCenterLH: function
(
  out _Out:TD3DXMatrix;  
  L:Single;
  R:Single;
  B:Single;
  T:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixOrthoRH: function
(
  out _Out:TD3DXMatrix;  
  W:Single;
  H:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixOrthoLH: function
(
  out _Out:TD3DXMatrix;  
  W:Single;
  H:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixOrthoOffCenterRH: function
(
  out _Out:TD3DXMatrix;  
  L:Single;
  R:Single;
  B:Single;
  T:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixOrthoOffCenterLH: function
(
  out _Out:TD3DXMatrix;  
  L:Single;
  R:Single;
  B:Single;
  T:Single;
  Zn:Single;
  Zf:Single
):PTD3DXMatrix; stdcall;  

var D3DXMatrixShadow: function
(
  out _Out:TD3DXMatrix;  
  const Light:TD3DXVector4;
  const Plane:TD3DXPlane
):PTD3DXMatrix; stdcall;  

var D3DXMatrixReflect: function
(
  out _Out:TD3DXMatrix;  
  const Plane:TD3DXPlane
):PTD3DXMatrix; stdcall;  

function D3DXQuaternionLength(const Q:TD3DXQuaternion):Single;
function D3DXQuaternionLengthSq(const Q:TD3DXQuaternion):Single;
function D3DXQuaternionDot(const Q1:TD3DXQuaternion;const Q2:TD3DXQuaternion):Single;

function D3DXQuaternionIdentity
(
  out _Out:TD3DXQuaternion  
):PTD3DXQuaternion;

function D3DXQuaternionIsIdentity(const Q:TD3DXQuaternion):LongBool;

function D3DXQuaternionConjugate
(
  out _Out:TD3DXQuaternion;  
  const Q:TD3DXQuaternion
):PTD3DXQuaternion;

var D3DXQuaternionToAxisAngle: procedure(const Q:TD3DXQuaternion;pAxis:PTD3DXVector3;pAngle:PSingle); stdcall;

var D3DXQuaternionRotationMatrix: function
(
  out _Out:TD3DXQuaternion;  
  const M:TD3DXMatrix
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionRotationAxis: function
(
  out _Out:TD3DXQuaternion;  
  const V:TD3DXVector3;
  Angle:Single
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionRotationYawPitchRoll: function
(
  out _Out:TD3DXQuaternion;  
  Yaw:Single;
  Pitch:Single;
  Roll:Single
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionMultiply: function
(
  out _Out:TD3DXQuaternion;  
  const Q1:TD3DXQuaternion;
  const Q2:TD3DXQuaternion
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionNormalize: function
(
  out _Out:TD3DXQuaternion;  
  const Q:TD3DXQuaternion
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionInverse: function
(
  out _Out:TD3DXQuaternion;  
  const Q:TD3DXQuaternion
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionLn: function
(
  out _Out:TD3DXQuaternion;  
  const Q:TD3DXQuaternion
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionExp: function
(
  out _Out:TD3DXQuaternion;  
  const Q:TD3DXQuaternion
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionSlerp: function
(
  out _Out:TD3DXQuaternion;  
  const Q1:TD3DXQuaternion;
  const Q2:TD3DXQuaternion;
  T:Single
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionSquad: function
(
  out _Out:TD3DXQuaternion;  
  const Q1:TD3DXQuaternion;
  const A:TD3DXQuaternion;
  const B:TD3DXQuaternion;
  const C:TD3DXQuaternion;
  T:Single
):PTD3DXQuaternion; stdcall;  

var D3DXQuaternionSquadSetup: procedure
(
  out AOut:TD3DXQuaternion;  
  out BOut:TD3DXQuaternion;  
  out COut:TD3DXQuaternion;  
  const Q0:TD3DXQuaternion;
  const Q1:TD3DXQuaternion;
  const Q2:TD3DXQuaternion;
  const Q3:TD3DXQuaternion
); stdcall;  

var D3DXQuaternionBaryCentric: function
(
  out _Out:TD3DXQuaternion;  
  const Q1:TD3DXQuaternion;
  const Q2:TD3DXQuaternion;
  const Q3:TD3DXQuaternion;
  F:Single;
  G:Single
):PTD3DXQuaternion; stdcall;  

function D3DXPlaneDot(const P:TD3DXPlane;const V:TD3DXVector4):Single;
function D3DXPlaneDotCoord(const P:TD3DXPlane;const V:TD3DXVector3):Single;
function D3DXPlaneDotNormal(const P:TD3DXPlane;const V:TD3DXVector3):Single;

function D3DXPlaneScale
(
  out _Out:TD3DXPlane;  
  const P:TD3DXPlane;
  S:Single
):PTD3DXPlane;

var D3DXPlaneNormalize: function
(
  out _Out:TD3DXPlane;  
  const P:TD3DXPlane
):PTD3DXPlane; stdcall;  

var D3DXPlaneIntersectLine: function
(
  out _Out:TD3DXVector3;  
  const P:TD3DXPlane;
  const V1:TD3DXVector3;
  const V2:TD3DXVector3
):PTD3DXVector3; stdcall;  

var D3DXPlaneFromPointNormal: function
(
  out _Out:TD3DXPlane;  
  const Point:TD3DXVector3;
  const Normal:TD3DXVector3
):PTD3DXPlane; stdcall;  

var D3DXPlaneFromPoints: function
(
  out _Out:TD3DXPlane;  
  const V1:TD3DXVector3;
  const V2:TD3DXVector3;
  const V3:TD3DXVector3
):PTD3DXPlane; stdcall;  

var D3DXPlaneTransform: function
(
  out _Out:TD3DXPlane;  
  const P:TD3DXPlane;
  const M:TD3DXMatrix
):PTD3DXPlane; stdcall;  

var D3DXPlaneTransformArray: function
(
  out _Out:TD3DXPlane;  
  OutStride:LongWord;
  const P:TD3DXPlane;
  PStride:LongWord;
  const M:TD3DXMatrix;
  N:LongWord
):PTD3DXPlane; stdcall;  

function D3DXColorNegative
(
  out _Out:TD3DXColor;  
  const Color:TD3DXColor
):PTD3DXColor;

function D3DXColorAdd
(
  out _Out:TD3DXColor;  
  const Color1:TD3DXColor;
  const Color2:TD3DXColor
):PTD3DXColor;

function D3DXColorSubtract
(
  out _Out:TD3DXColor;  
  const Color1:TD3DXColor;
  const Color2:TD3DXColor
):PTD3DXColor;

function D3DXColorScale
(
  out _Out:TD3DXColor;  
  const Color:TD3DXColor;
  S:Single
):PTD3DXColor;

function D3DXColorModulate
(
  out _Out:TD3DXColor;  
  const Color1:TD3DXColor;
  const Color2:TD3DXColor
):PTD3DXColor;

function D3DXColorLerp
(
  out _Out:TD3DXColor;  
  const Color1:TD3DXColor;
  const Color2:TD3DXColor;
  S:Single
):PTD3DXColor;

var D3DXColorAdjustSaturation: function
(
  out _Out:TD3DXColor;  
  const Color:TD3DXColor;
  S:Single
):PTD3DXColor; stdcall;  

var D3DXColorAdjustContrast: function
(
  out _Out:TD3DXColor;  
  const Color:TD3DXColor;
  C:Single
):PTD3DXColor; stdcall;  

var D3DXFresnelTerm: function(CosTheta:Single;RefractionIndex:Single):Single; stdcall;

type
  ID3DXMatrixStack=interface;
  PID3DXMatrixStack=^ID3DXMatrixStack;

  ID3DXMatrixStack=interface(IUnknown)
    ['{C7885BA7-F990-4FE7-922D-8515E477DD85}']
    function Pop:HResult; stdcall;
    function Push:HResult; stdcall;
    function LoadIdentity:HResult; stdcall;
    function LoadMatrix(const M:TD3DXMatrix):HResult; stdcall;
    function MultMatrix(const M:TD3DXMatrix):HResult; stdcall;
    function MultMatrixLocal(const M:TD3DXMatrix):HResult; stdcall;
    function RotateAxis(const V:TD3DXVector3;Angle:Single):HResult; stdcall;
    function RotateAxisLocal(const V:TD3DXVector3;Angle:Single):HResult; stdcall;
    function RotateYawPitchRoll(Yaw:Single;Pitch:Single;Roll:Single):HResult; stdcall;
    function RotateYawPitchRollLocal(Yaw:Single;Pitch:Single;Roll:Single):HResult; stdcall;
    function Scale(X:Single;Y:Single;Z:Single):HResult; stdcall;
    function ScaleLocal(X:Single;Y:Single;Z:Single):HResult; stdcall;
    function Translate(X:Single;Y:Single;Z:Single):HResult; stdcall;
    function TranslateLocal(X:Single;Y:Single;Z:Single):HResult; stdcall;
    function GetTop:PTD3DXMatrix; stdcall;
  end;

var D3DXCreateMatrixStack: function
(
  Flags:LongWord;
  out Stack:ID3DXMatrixStack  
):HResult; stdcall;  

var D3DXSHEvalDirection: function(pOut:PSingle;Order:LongWord;const Dir:TD3DXVector3):PSingle; stdcall;

var D3DXSHRotate: function
(
  pOut:PSingle;
  Order:LongWord;
  const Matrix:TD3DXMatrix;
  const _In:Single
):PSingle; stdcall;  

var D3DXSHRotateZ: function
(
  out _Out:Single;  
  Order:LongWord;
  Angle:Single;
  const _In:Single
):PSingle; stdcall;  

var D3DXSHAdd: function
(
  pOut:PSingle;
  Order:LongWord;
  const A:Single;
  const B:Single
):PSingle; stdcall;  

var D3DXSHScale: function
(
  pOut:PSingle;
  Order:LongWord;
  const _In:Single;
  Scale:Single
):PSingle; stdcall;  

var D3DXSHDot: function(Order:LongWord;const A:Single;const B:Single):Single; stdcall;

var D3DXSHMultiply2: function
(
  pOut:PSingle;
  pF:PSingle;
  pG:PSingle
):PSingle; stdcall;  

var D3DXSHMultiply3: function
(
  pOut:PSingle;
  pF:PSingle;
  pG:PSingle
):PSingle; stdcall;  

var D3DXSHMultiply4: function
(
  pOut:PSingle;
  pF:PSingle;
  pG:PSingle
):PSingle; stdcall;  

var D3DXSHMultiply5: function
(
  pOut:PSingle;  
  pF:PSingle;  
  pG:PSingle  
):PSingle; stdcall;  

var D3DXSHMultiply6: function
(
  pOut:PSingle;  
  pF:PSingle;  
  pG:PSingle  
):PSingle; stdcall;  

var D3DXSHEvalDirectionalLight: function
(
  Order:LongWord;
  const Dir:TD3DXVector3;
  RIntensity:Single;
  GIntensity:Single;
  BIntensity:Single;
  pROut:PSingle;
  pGOut:PSingle;
  pBOut:PSingle
):HResult; stdcall;  

var D3DXSHEvalSphericalLight: function
(
  Order:LongWord;
  const Pos:TD3DXVector3;
  Radius:Single;
  RIntensity:Single;
  GIntensity:Single;
  BIntensity:Single;
  pROut:PSingle;
  pGOut:PSingle;
  pBOut:PSingle
):HResult; stdcall;  

var D3DXSHEvalConeLight: function
(
  Order:LongWord;
  const Dir:TD3DXVector3;
  Radius:Single;
  RIntensity:Single;
  GIntensity:Single;
  BIntensity:Single;
  pROut:PSingle;
  pGOut:PSingle;
  pBOut:PSingle
):HResult; stdcall;  

var D3DXSHEvalHemisphereLight: function
(
  Order:LongWord;
  const Dir:TD3DXVector3;
  Top:TD3DXColor;
  Bottom:TD3DXColor;
  pROut:PSingle;
  pGOut:PSingle;
  pBOut:PSingle
):HResult; stdcall;  

var D3DXIntersectTri: function (const P0:TD3DXVector3;const P1:TD3DXVector3;const P2:TD3DXVector3;const RayPos:TD3DXVector3;const RayDir:TD3DXVector3;pU:PSingle;pV:PSingle;pDist:PSingle):LongBool; stdcall;
var D3DXSphereBoundProbe: function(const Center:TD3DXVector3;Radius:Single;const RayPosition:TD3DXVector3;const RayDirection:TD3DXVector3):LongBool; stdcall;
var D3DXBoxBoundProbe: function(const Min:TD3DXVector3;const Max:TD3DXVector3;const RayPosition:TD3DXVector3;const RayDirection:TD3DXVector3):LongBool; stdcall;
var D3DXComputeBoundingSphere: function(const FirstPosition:TD3DXVector3;NumVertices:LongWord;Stride:LongWord;pCenter:PTD3DXVector3;pRadius:PSingle):HResult; stdcall;
var D3DXComputeBoundingBox: function(const FirstPosition:TD3DXVector3;NumVertices:LongWord;Stride:LongWord;pMin:PTD3DXVector3;pMax:PTD3DXVector3):HResult; stdcall;

type
  TD3DX_CPUOptimization=
  (
    D3DX_NOT_OPTIMIZED=0,
    D3DX_3DNOW_OPTIMIZED,
    D3DX_SSE2_OPTIMIZED,
    D3DX_SSE_OPTIMIZED
  );
  PTD3DX_CPUOptimization=^TD3DX_CPUOptimization;
  D3DX_CPU_OPTIMIZATION=TD3DX_CPUOptimization;
  PD3DX_CPU_OPTIMIZATION=^TD3DX_CPUOptimization;

var D3DXCpuOptimizations: function (Enable:LongBool):TD3DX_CPUOptimization; stdcall;


// =============== D3DX10Core.h =====================

const
  D3DX10_SDK_VERSION=43;
  _FACD3D=$876;
  D3D_STATUS_Base=LongWord(_FACD3D shl 16);
  D3D_HRESULT_Base=D3D_STATUS_Base or LongWord(1 shl 31);
  D3DERR_INVALIDCALL=D3D_HRESULT_Base or 2156;
  D3DERR_WASSTILLDRAWING=D3D_HRESULT_Base or 540;

var D3DX10CreateDevice: function(Adapter:IDXGIAdapter;DriverType:TD3D10_DriverType;Software:HMODULE;Flags:LongWord;out Device:ID3D10Device):HResult; stdcall;
var D3DX10CreateDeviceAndSwapChain: function(Adapter:IDXGIAdapter;DriverType:TD3D10_DriverType;Software:HMODULE;Flags:LongWord;const SwapChainDesc:TDXGI_SwapChainDesc;out SwapChain:IDXGISwapChain;out Device:ID3D10Device):HResult; stdcall;
var D3DX10GetFeatureLevel1: function(Device:ID3D10Device;out Device1:ID3D10Device1):HResult; stdcall;

{$IFDEF D3D_DIAG_DLL}

var D3DX10DebugMute: function(Mute:LongBool):LongBool; stdcall;

{$ENDIF}

var D3DX10CheckVersion: function(D3DSdkVersion:LongWord;D3DX10SdkVersion:LongWord):HResult; stdcall;

type
  TD3DX10_SpriteFlag=
  (
    D3DX10_SPRITE_SORT_TEXTURE=$01,
    D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT=$02,
    D3DX10_SPRITE_SORT_DEPTH_FRONT_TO_BACK=$04,
    D3DX10_SPRITE_SAVE_STATE=$08,
    D3DX10_SPRITE_ADDREF_TEXTURES=$10
  );
  PTD3DX10_SpriteFlag=^TD3DX10_SpriteFlag;
  D3DX10_SPRITE_FLAG=TD3DX10_SpriteFlag;
  PD3DX10_SPRITE_FLAG=^TD3DX10_SpriteFlag;

  TD3DX10_Sprite=record
    MatWorld:TD3DXMatrix;
    TexCoord:TD3DXVector2;
    TexSize:TD3DXVector2;
    ColorModulate:TD3DXColor;
    pTexture:ID3D10ShaderResourceView;
    TextureIndex:LongWord;
  end;
  PTD3DX10_Sprite=^TD3DX10_Sprite;
  D3DX10_SPRITE=TD3DX10_Sprite;
  PD3DX10_SPRITE=^TD3DX10_Sprite;

  ID3DX10Sprite=interface;
  PID3DX10Sprite=^ID3DX10Sprite;

  ID3DX10Sprite=interface(IUnknown)
    ['{BA0B762D-8D28-43EC-B9DC-2F84443B0614}']
    function _Begin(Flags:LongWord):HResult; stdcall;
    function DrawSpritesBuffered(pSprites:PTD3DX10_Sprite;NumSprites:LongWord):HResult; stdcall;
    function Flush:HResult; stdcall;
    function DrawSpritesImmediate(pSprites:PTD3DX10_Sprite;NumSprites:LongWord;cbSprite:LongWord;Flags:LongWord):HResult; stdcall;
    function _End:HResult; stdcall;
    function GetViewTransform(pViewTransform:PTD3DXMatrix):HResult; stdcall;
    function SetViewTransform(pViewTransform:PTD3DXMatrix):HResult; stdcall;
    function GetProjectionTransform(pProjectionTransform:PTD3DXMatrix):HResult; stdcall;
    function SetProjectionTransform(pProjectionTransform:PTD3DXMatrix):HResult; stdcall;
    function GetDevice(out Device:ID3D10Device):HResult; stdcall;
  end;

var D3DX10CreateSprite: function(Device:ID3D10Device;CDeviceBufferSize:LongWord;out Sprite:ID3DX10SPRITE):HResult; stdcall;

type
  ID3DX10DataLoader=class // Cannot use 'interface' as the QueryInterface, AddRef and Release methods are missing.
    function Load:HResult; virtual; stdcall; abstract;
    function Decompress(var pData:Pointer;pNumBytes:PSIZE_T):HResult; virtual; stdcall; abstract;
    function Destroy:HResult; reintroduce; virtual; stdcall; abstract;
  end;

  ID3DX10DataProcessor=class // Cannot use 'interface' as the QueryInterface, AddRef and Release methods are missing.
    function Process(pData:Pointer;NumBytes:SIZE_T):HResult; virtual; stdcall; abstract;
    function CreateDeviceObject(var pDataObject:Pointer):HResult; virtual; stdcall; abstract;
    function Destroy:HResult; reintroduce; virtual; stdcall; abstract;
  end;

  ID3DX10ThreadPump=interface(IUnknown)
    ['{C93FECFA-6967-478A-ABBC-402D90621FCB}']
    function AddWorkItem(DataLoader:ID3DX10DataLoader;DataProcessor:ID3DX10DataProcessor;pHResult:PHResult;var pDeviceObject:Pointer):HResult; stdcall;
    function GetWorkItemCount:LongWord; stdcall;
    function WaitForAllItems:HResult; stdcall;
    function ProcessDeviceWorkItems(WorkItemCount:LongWord):HResult; stdcall;
    function PurgeAllItems:HResult; stdcall;
    function GetQueueStatus(pIoQueue:PLongWord;pProcessQueue:PLongWord;pDeviceQueue:PLongWord):HResult; stdcall;
  end;

var D3DX10CreateThreadPump: function(CIoThreads:LongWord;CProcThreads:LongWord;out ThreadPump:ID3DX10ThreadPump):HResult; stdcall;

type
  TD3DX10_FontDescA=record
    Height:Integer;
    Width:LongWord;
    Weight:LongWord;
    MipLevels:LongWord;
    Italic:LongBool;
    CharSet:Byte;
    OutputPrecision:Byte;
    Quality:Byte;
    PitchAndFamily:Byte;
    FaceName:array[0..LF_FACESIZE-1] of AnsiChar;
  end;
  PTD3DX10_FontDescA=^TD3DX10_FontDescA;
  D3DX10_FONT_DESCA=TD3DX10_FontDescA;
  PD3DX10_FONT_DESCA=^TD3DX10_FontDescA;

  TD3DX10_FontDescW=record
    Height:Integer;
    Width:LongWord;
    Weight:LongWord;
    MipLevels:LongWord;
    Italic:LongBool;
    CharSet:Byte;
    OutputPrecision:Byte;
    Quality:Byte;
    PitchAndFamily:Byte;
    FaceName:array[0..LF_FACESIZE-1] of WideChar;
  end;
  PTD3DX10_FontDescW=^TD3DX10_FontDescW;
  D3DX10_FONT_DESCW=TD3DX10_FontDescW;
  PD3DX10_FONT_DESCW=^TD3DX10_FontDescW;

{$IFDEF UNICODE}

type
  TD3DX10_FontDesc=D3DX10_FONT_DESCW;
  PTD3DX10_FontDesc=^TD3DX10_FontDesc;
  D3DX10_FONT_DESC=TD3DX10_FontDesc;
  PD3DX10_FONT_DESC=^TD3DX10_FontDesc;

{$ELSE}

type
  TD3DX10_FontDesc=D3DX10_FONT_DESCA;
  PTD3DX10_FontDesc=^TD3DX10_FontDesc;
  D3DX10_FONT_DESC=TD3DX10_FontDesc;
  PD3DX10_FONT_DESC=^TD3DX10_FontDesc;

{$ENDIF}

type
  ID3DX10Font=interface;
  PID3DX10Font=^ID3DX10Font;

  ID3DX10Font=interface(IUnknown)
    ['{D79DBB70-5F21-4D36-BBC2-FF525C213CDC}']
    function GetDevice(out Device:ID3D10Device):HResult; stdcall;
    function GetDescA(pDesc:PTD3DX10_FontDescA):HResult; stdcall;
    function GetDescW(pDesc:PTD3DX10_FontDescW):HResult; stdcall;
    function GetTextMetricsA(out TextMetrics:TTextMetricA):LongBool; stdcall;
    function GetTextMetricsW(out TextMetrics:TTextMetricW):LongBool; stdcall;
    function GetDC:HDC; stdcall;
    function GetGlyphData(Glyph:LongWord;out Texture:ID3D10ShaderResourceView;pBlackBox:PTRect;pCellInc:PTPoint):HResult; stdcall;
    function PreloadCharacters(First:LongWord;Last:LongWord):HResult; stdcall;
    function PreloadGlyphs(First:LongWord;Last:LongWord):HResult; stdcall;
    function PreloadTextA(pString:PAnsiChar;Count:Integer):HResult; stdcall;
    function PreloadTextW(pString:PWideChar;Count:Integer):HResult; stdcall;
    function DrawTextA(Sprite:ID3DX10SPRITE;pString:PAnsiChar;Count:Integer;pRect:PRECT;Format:LongWord;Color:TD3DXColor):Integer; stdcall;
    function DrawTextW(Sprite:ID3DX10SPRITE;pString:PWideChar;Count:Integer;pRect:PRECT;Format:LongWord;Color:TD3DXColor):Integer; stdcall;
  end;

var D3DX10CreateFontA: function(Device:ID3D10Device;Height:Integer;Width:LongWord;Weight:LongWord;MipLevels:LongWord;Italic:LongBool;CharSet:LongWord;OutputPrecision:LongWord;Quality:LongWord;PitchAndFamily:LongWord;pFaceName:PAnsiChar;out Font:ID3DX10FONT):HResult; stdcall;
var D3DX10CreateFontW: function(Device:ID3D10Device;Height:Integer;Width:LongWord;Weight:LongWord;MipLevels:LongWord;Italic:LongBool;CharSet:LongWord;OutputPrecision:LongWord;Quality:LongWord;PitchAndFamily:LongWord;pFaceName:PWideChar;out Font:ID3DX10FONT):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateFont: function(Device:ID3D10Device;Height:Integer;Width:LongWord;Weight:LongWord;MipLevels:LongWord;Italic:LongBool;CharSet:LongWord;OutputPrecision:LongWord;Quality:LongWord;PitchAndFamily:LongWord;pFaceName:PWideChar;out Font:ID3DX10FONT):HResult; stdcall;

{$ELSE}

var D3DX10CreateFont: function(Device:ID3D10Device;Height:Integer;Width:LongWord;Weight:LongWord;MipLevels:LongWord;Italic:LongBool;CharSet:LongWord;OutputPrecision:LongWord;Quality:LongWord;PitchAndFamily:LongWord;pFaceName:PAnsiChar;out Font:ID3DX10FONT):HResult; stdcall;

{$ENDIF}

var D3DX10CreateFontIndirectA: function(Device:ID3D10Device;const Desc:TD3DX10_FontDescA;out Font:ID3DX10FONT):HResult; stdcall;
var D3DX10CreateFontIndirectW: function(Device:ID3D10Device;const Desc:TD3DX10_FontDescW;out Font:ID3DX10FONT):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateFontIndirect: function(Device:ID3D10Device;const Desc:TD3DX10_FontDescW;out Font:ID3DX10FONT):HResult; stdcall;

{$ELSE}

var D3DX10CreateFontIndirect: function(Device:ID3D10Device;const Desc:TD3DX10_FontDescA;out Font:ID3DX10FONT):HResult; stdcall;

{$ENDIF}

var D3DX10UnsetAllDeviceObjects: function(Device:ID3D10Device):HResult; stdcall;


// =============== D3DX10Tex.h =====================

type
  TD3DX10_FilterFlag=
  (
    D3DX10_FILTER_NONE=(1 shl 0),
    D3DX10_FILTER_POINT=(2 shl 0),
    D3DX10_FILTER_LINEAR=(3 shl 0),
    D3DX10_FILTER_TRIANGLE=(4 shl 0),
    D3DX10_FILTER_BOX=(5 shl 0),
    D3DX10_FILTER_MIRROR_U=(1 shl 16),
    D3DX10_FILTER_MIRROR_V=(2 shl 16),
    D3DX10_FILTER_MIRROR_W=(4 shl 16),
    D3DX10_FILTER_MIRROR=(7 shl 16),
    D3DX10_FILTER_DITHER=(1 shl 19),
    D3DX10_FILTER_DITHER_DIFFUSION=(2 shl 19),
    D3DX10_FILTER_SRGB_IN=(1 shl 21),
    D3DX10_FILTER_SRGB_OUT=(2 shl 21),
    D3DX10_FILTER_SRGB=(3 shl 21)
  );
  PTD3DX10_FilterFlag=^TD3DX10_FilterFlag;
  D3DX10_FILTER_FLAG=TD3DX10_FilterFlag;
  PD3DX10_FILTER_FLAG=^TD3DX10_FilterFlag;

  TD3DX10_NormalmapFlag=
  (
    D3DX10_NORMALMAP_MIRROR_U=(1 shl 16),
    D3DX10_NORMALMAP_MIRROR_V=(2 shl 16),
    D3DX10_NORMALMAP_MIRROR=(3 shl 16),
    D3DX10_NORMALMAP_INVERTSIGN=(8 shl 16),
    D3DX10_NORMALMAP_COMPUTE_OCCLUSION=(16 shl 16)
  );
  PTD3DX10_NormalmapFlag=^TD3DX10_NormalmapFlag;
  D3DX10_NORMALMAP_FLAG=TD3DX10_NormalmapFlag;
  PD3DX10_NORMALMAP_FLAG=^TD3DX10_NormalmapFlag;

  TD3DX10_ChannelFlag=
  (
    D3DX10_CHANNEL_RED=(1 shl 0),
    D3DX10_CHANNEL_BLUE=(1 shl 1),
    D3DX10_CHANNEL_GREEN=(1 shl 2),
    D3DX10_CHANNEL_ALPHA=(1 shl 3),
    D3DX10_CHANNEL_LUMINANCE=(1 shl 4)
  );
  PTD3DX10_ChannelFlag=^TD3DX10_ChannelFlag;
  D3DX10_CHANNEL_FLAG=TD3DX10_ChannelFlag;
  PD3DX10_CHANNEL_FLAG=^TD3DX10_ChannelFlag;

  TD3DX10_ImageFileFormat=
  (
    D3DX10_IFF_BMP=0,
    D3DX10_IFF_JPG=1,
    D3DX10_IFF_PNG=3,
    D3DX10_IFF_DDS=4,
    D3DX10_IFF_TIFF=10,
    D3DX10_IFF_GIF=11,
    D3DX10_IFF_WMP=12
  );
  PTD3DX10_ImageFileFormat=^TD3DX10_ImageFileFormat;
  D3DX10_IMAGE_FILE_FORMAT=TD3DX10_ImageFileFormat;
  PD3DX10_IMAGE_FILE_FORMAT=^TD3DX10_ImageFileFormat;

  TD3DX10_SaveTextureFlag=
  (
    D3DX10_STF_USEINPUTBLOB=$0001
  );
  PTD3DX10_SaveTextureFlag=^TD3DX10_SaveTextureFlag;
  D3DX10_SAVE_TEXTURE_FLAG=TD3DX10_SaveTextureFlag;
  PD3DX10_SAVE_TEXTURE_FLAG=^TD3DX10_SaveTextureFlag;

  TD3DX10_ImageInfo=record
    Width:LongWord;
    Height:LongWord;
    Depth:LongWord;
    ArraySize:LongWord;
    MipLevels:LongWord;
    MiscFlags:LongWord;
    Format:TDXGI_FORMAT;
    ResourceDimension:TD3D10_ResourceDimension;
    ImageFileFormat:TD3DX10_ImageFileFormat;
  end;
  PTD3DX10_ImageInfo=^TD3DX10_ImageInfo;
  D3DX10_IMAGE_INFO=TD3DX10_ImageInfo;
  PD3DX10_IMAGE_INFO=^TD3DX10_ImageInfo;

  TD3DX10_ImageLoadInfo=record
    Width:LongWord;
    Height:LongWord;
    Depth:LongWord;
    FirstMipLevel:LongWord;
    MipLevels:LongWord;
    Usage:TD3D10_Usage;
    BindFlags:LongWord;
    CpuAccessFlags:LongWord;
    MiscFlags:LongWord;
    Format:TDXGI_FORMAT;
    Filter:LongWord;
    MipFilter:LongWord;
    pSrcInfo:PTD3DX10_ImageInfo;
  end;
  PTD3DX10_ImageLoadInfo=^TD3DX10_ImageLoadInfo;
  D3DX10_IMAGE_LOAD_INFO=TD3DX10_ImageLoadInfo;
  PD3DX10_IMAGE_LOAD_INFO=^TD3DX10_ImageLoadInfo;

var D3DX10GetImageInfoFromFileA: function(pSrcFile:PAnsiChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;
var D3DX10GetImageInfoFromFileW: function(pSrcFile:PWideChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10GetImageInfoFromFile: function(pSrcFile:PWideChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10GetImageInfoFromFile: function(pSrcFile:PAnsiChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10GetImageInfoFromResourceA: function(hSrcModule:HMODULE;pSrcResource:PAnsiChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;
var D3DX10GetImageInfoFromResourceW: function(hSrcModule:HMODULE;pSrcResource:PWideChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10GetImageInfoFromResource: function(hSrcModule:HMODULE;pSrcResource:PWideChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10GetImageInfoFromResource: function(hSrcModule:HMODULE;pSrcResource:PAnsiChar;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10GetImageInfoFromMemory: function(pSrcData:Pointer;SrcDataSize:SIZE_T;Pump:ID3DX10ThreadPump;pSrcInfo:PTD3DX10_ImageInfo;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateShaderResourceViewFromFileA: function(Device:ID3D10Device;pSrcFile:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateShaderResourceViewFromFileW: function(Device:ID3D10Device;pSrcFile:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateShaderResourceViewFromFile: function(Device:ID3D10Device;pSrcFile:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CreateShaderResourceViewFromFile: function(Device:ID3D10Device;pSrcFile:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CreateTextureFromFileA: function(Device:ID3D10Device;pSrcFile:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateTextureFromFileW: function(Device:ID3D10Device;pSrcFile:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateTextureFromFile: function(Device:ID3D10Device;pSrcFile:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CreateTextureFromFile: function(Device:ID3D10Device;pSrcFile:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CreateShaderResourceViewFromResourceA: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateShaderResourceViewFromResourceW: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateShaderResourceViewFromResource: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CreateShaderResourceViewFromResource: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CreateTextureFromResourceA: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateTextureFromResourceW: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateTextureFromResource: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PWideChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CreateTextureFromResource: function(Device:ID3D10Device;hSrcModule:HMODULE;pSrcResource:PAnsiChar;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CreateShaderResourceViewFromMemory: function(Device:ID3D10Device;pSrcData:Pointer;SrcDataSize:SIZE_T;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out ShaderResourceView:ID3D10ShaderResourceView;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateTextureFromMemory: function(Device:ID3D10Device;pSrcData:Pointer;SrcDataSize:SIZE_T;pLoadInfo:PTD3DX10_ImageLoadInfo;Pump:ID3DX10ThreadPump;out Texture:ID3D10Resource;pHResult:PHResult):HResult; stdcall;

type
  TD3DX10_TextureLoadInfo=record
    pSrcBox:PTD3D10_Box;
    pDstBox:PTD3D10_Box;
    SrcFirstMip:LongWord;
    DstFirstMip:LongWord;
    NumMips:LongWord;
    SrcFirstElement:LongWord;
    DstFirstElement:LongWord;
    NumElements:LongWord;
    Filter:LongWord;
    MipFilter:LongWord;
  end;
  PTD3DX10_TextureLoadInfo=^TD3DX10_TextureLoadInfo;
  D3DX10_TEXTURE_LOAD_INFO=TD3DX10_TextureLoadInfo;
  PD3DX10_TEXTURE_LOAD_INFO=^TD3DX10_TextureLoadInfo;

var D3DX10LoadTextureFromTexture: function(SrcTexture:ID3D10Resource;pLoadInfo:PTD3DX10_TextureLoadInfo;DstTexture:ID3D10Resource):HResult; stdcall;
var D3DX10FilterTexture: function(Texture:ID3D10Resource;SrcLevel:LongWord;MipFilter:LongWord):HResult; stdcall;
var D3DX10SaveTextureToFileA: function(SrcTexture:ID3D10Resource;DestFormat:TD3DX10_ImageFileFormat;pDestFile:PAnsiChar):HResult; stdcall;
var D3DX10SaveTextureToFileW: function(SrcTexture:ID3D10Resource;DestFormat:TD3DX10_ImageFileFormat;pDestFile:PWideChar):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10SaveTextureToFile: function(SrcTexture:ID3D10Resource;DestFormat:TD3DX10_ImageFileFormat;pDestFile:PWideChar):HResult; stdcall;

{$ELSE}

var D3DX10SaveTextureToFile: function(SrcTexture:ID3D10Resource;DestFormat:TD3DX10_ImageFileFormat;pDestFile:PAnsiChar):HResult; stdcall;

{$ENDIF}

var D3DX10SaveTextureToMemory: function(SrcTexture:ID3D10Resource;DestFormat:TD3DX10_ImageFileFormat;out DestBuf:ID3D10Blob;Flags:LongWord):HResult; stdcall;
var D3DX10ComputeNormalMap: function(SrcTexture:ID3D10Texture2D;Flags:LongWord;Channel:LongWord;Amplitude:Single;DestTexture:ID3D10Texture2D):HResult; stdcall;
var D3DX10SHProjectCubeMap: function(Order:LongWord;CubeMap:ID3D10Texture2D;pROut:PSingle;pGOut:PSingle;pBOut:PSingle):HResult; stdcall;


// =============== D3DX10Mesh.h =====================

const
  D3DX10_SKININFO_NO_SCALING=0;
  D3DX10_SKININFO_SCALE_TO_1=1;
  D3DX10_SKININFO_SCALE_TO_TOTAL=2;

type
  T_D3DX10_MESH=
  (
    D3DX10_MESH_32_BIT=$001,
    D3DX10_MESH_GS_ADJACENCY=$004
  );
  PT_D3DX10_MESH=^T_D3DX10_MESH;
  _D3DX10_MESH=T_D3DX10_MESH;
  P_D3DX10_MESH=^T_D3DX10_MESH;

  TD3DX10_AttributeRange=record
    AttribId:LongWord;
    FaceStart:LongWord;
    FaceCount:LongWord;
    VertexStart:LongWord;
    VertexCount:LongWord;
  end;
  PTD3DX10_AttributeRange=^TD3DX10_AttributeRange;
  D3DX10_ATTRIBUTE_RANGE=TD3DX10_AttributeRange;
  PD3DX10_ATTRIBUTE_RANGE=^TD3DX10_AttributeRange;

  TLPD3DX10_ATTRIBUTE_RANGE=PD3DX10_ATTRIBUTE_RANGE;
  PTLPD3DX10_ATTRIBUTE_RANGE=^TLPD3DX10_ATTRIBUTE_RANGE;
  LPD3DX10_ATTRIBUTE_RANGE=TLPD3DX10_ATTRIBUTE_RANGE;
  PLPD3DX10_ATTRIBUTE_RANGE=^TLPD3DX10_ATTRIBUTE_RANGE;

  TD3DX10_MeshDiscardFlags=
  (
    D3DX10_MESH_DISCARD_ATTRIBUTE_BUFFER=$01,
    D3DX10_MESH_DISCARD_ATTRIBUTE_TABLE=$02,
    D3DX10_MESH_DISCARD_POINTREPS=$04,
    D3DX10_MESH_DISCARD_ADJACENCY=$08,
    D3DX10_MESH_DISCARD_DEVICE_BUFFERS=$10
  );
  PTD3DX10_MeshDiscardFlags=^TD3DX10_MeshDiscardFlags;
  D3DX10_MESH_DISCARD_FLAGS=TD3DX10_MeshDiscardFlags;
  PD3DX10_MESH_DISCARD_FLAGS=^TD3DX10_MeshDiscardFlags;

  TD3DX10_WeldEpsilons=record
    Position:Single;
    BlendWeights:Single;
    Normal:Single;
    PSize:Single;
    Specular:Single;
    Diffuse:Single;
    Texcoord:array[0..7] of Single;
    Tangent:Single;
    Binormal:Single;
    TessFactor:Single;
  end;
  PTD3DX10_WeldEpsilons=^TD3DX10_WeldEpsilons;
  D3DX10_WELD_EPSILONS=TD3DX10_WeldEpsilons;
  PD3DX10_WELD_EPSILONS=^TD3DX10_WeldEpsilons;

  TLPD3DX10_WELD_EPSILONS=PD3DX10_WELD_EPSILONS;
  PTLPD3DX10_WELD_EPSILONS=^TLPD3DX10_WELD_EPSILONS;
  LPD3DX10_WELD_EPSILONS=TLPD3DX10_WELD_EPSILONS;
  PLPD3DX10_WELD_EPSILONS=^TLPD3DX10_WELD_EPSILONS;

  TD3DX10_IntersectInfo=record
    FaceIndex:LongWord;
    U:Single;
    V:Single;
    Dist:Single;
  end;
  PTD3DX10_IntersectInfo=^TD3DX10_IntersectInfo;
  D3DX10_INTERSECT_INFO=TD3DX10_IntersectInfo;
  PD3DX10_INTERSECT_INFO=^TD3DX10_IntersectInfo;

  ID3DX10MeshBuffer=interface(IUnknown)
    ['{04B0D117-1041-46B1-AA8A-3952848BA22E}']
    function Map(var pData:Pointer;pSize:PSIZE_T):HResult; stdcall;
    function Unmap:HResult; stdcall;
    function GetSize:SIZE_T; stdcall;
  end;

  ID3DX10Mesh=interface(IUnknown)
    ['{4020E5C2-1403-4929-883F-E2E849FAC195}']
    function GetFaceCount:LongWord; stdcall;
    function GetVertexCount:LongWord; stdcall;
    function GetVertexBufferCount:LongWord; stdcall;
    function GetFlags:LongWord; stdcall;
    function GetVertexDescription(var pDesc:PTD3D10_InputElementDesc;pDeclCount:PLongWord):HResult; stdcall;
    function SetVertexData(BufferLength:LongWord;pData:Pointer):HResult; stdcall;
    function GetVertexBuffer(BufferLength:LongWord;out VertexBuffer:ID3DX10MeshBuffer):HResult; stdcall;
    function SetIndexData(pData:Pointer;NumIndices:LongWord):HResult; stdcall;
    function GetIndexBuffer(out IndexBuffer:ID3DX10MeshBuffer):HResult; stdcall;
    function SetAttributeData(const Data:LongWord):HResult; stdcall;
    function GetAttributeBuffer(out AttributeBuffer:ID3DX10MeshBuffer):HResult; stdcall;
    function SetAttributeTable(const AttribTable:TD3DX10_AttributeRange;AttribTableSize:LongWord):HResult; stdcall;
    function GetAttributeTable(pAttribTable:PTD3DX10_AttributeRange;pAttribTableSize:PLongWord):HResult; stdcall;
    function GenerateAdjacencyAndPointReps(Epsilon:Single):HResult; stdcall;
    function GenerateGSAdjacency:HResult; stdcall;
    function SetAdjacencyData(const Adjacency:LongWord):HResult; stdcall;
    function GetAdjacencyBuffer(out Adjacency:ID3DX10MeshBuffer):HResult; stdcall;
    function SetPointRepData(const PointReps:LongWord):HResult; stdcall;
    function GetPointRepBuffer(out PointReps:ID3DX10MeshBuffer):HResult; stdcall;
    function Discard(Discard:TD3DX10_MeshDiscardFlags):HResult; stdcall;
    function CloneMesh(Flags:LongWord;pPosSemantic:PAnsiChar;const Desc:TD3D10_InputElementDesc;DeclCount:LongWord;out CloneMesh:ID3DX10Mesh):HResult; stdcall;
    function Optimize(Flags:LongWord;pFaceRemap:PLongWord;VertexRemap:ID3D10Blob):HResult; stdcall;
    function GenerateAttributeBufferFromTable:HResult; stdcall;
    function Intersect(pRayPos:PTD3DXVector3;pRayDir:PTD3DXVector3;pHitCount:PLongWord;pFaceIndex:PLongWord;pU:PSingle;pV:PSingle;pDist:PSingle;out AllHits:ID3D10Blob):HResult; stdcall;
    function IntersectSubset(AttribId:LongWord;pRayPos:PTD3DXVector3;pRayDir:PTD3DXVector3;pHitCount:PLongWord;pFaceIndex:PLongWord;pU:PSingle;pV:PSingle;pDist:PSingle;out AllHits:ID3D10Blob):HResult; stdcall;
    function CommitToDevice:HResult; stdcall;
    function DrawSubset(AttribId:LongWord):HResult; stdcall;
    function DrawSubsetInstanced(AttribId:LongWord;InstanceCount:LongWord;StartInstanceLocation:LongWord):HResult; stdcall;
    function GetDeviceVertexBuffer(BufferLength:LongWord;out VertexBuffer:ID3D10Buffer):HResult; stdcall;
    function GetDeviceIndexBuffer(out IndexBuffer:ID3D10Buffer):HResult; stdcall;
  end;

var D3DX10CreateMesh: function(Device:ID3D10Device;const Declaration:TD3D10_InputElementDesc;DeclCount:LongWord;pPositionSemantic:PAnsiChar;VertexCount:LongWord;FaceCount:LongWord;Options:LongWord;out Mesh:ID3DX10Mesh):HResult; stdcall;

type
  T_D3DX10_MESHOPT=
  (
    D3DX10_MESHOPT_COMPACT=$01000000,
    D3DX10_MESHOPT_ATTR_SORT=$02000000,
    D3DX10_MESHOPT_VERTEX_CACHE=$04000000,
    D3DX10_MESHOPT_STRIP_REORDER=$08000000,
    D3DX10_MESHOPT_IGNORE_VERTS=$10000000,
    D3DX10_MESHOPT_DO_NOT_SPLIT=$20000000,
    D3DX10_MESHOPT_DEVICE_INDEPENDENT=$00400000
  );
  PT_D3DX10_MESHOPT=^T_D3DX10_MESHOPT;
  _D3DX10_MESHOPT=T_D3DX10_MESHOPT;
  P_D3DX10_MESHOPT=^T_D3DX10_MESHOPT;

  TD3DX10_SkinningChannel=record
    SrcOffset:LongWord;
    DestOffset:LongWord;
    IsNormal:LongBool;
  end;
  PTD3DX10_SkinningChannel=^TD3DX10_SkinningChannel;
  D3DX10_SKINNING_CHANNEL=TD3DX10_SkinningChannel;
  PD3DX10_SKINNING_CHANNEL=^TD3DX10_SkinningChannel;

  ID3DX10SkinInfo=interface(IUnknown)
    ['{420BD604-1C76-4A34-A466-E45D0658A32C}']
    function GetNumVertices:LongWord; stdcall;
    function GetNumBones:LongWord; stdcall;
    function GetMaxBoneInfluences:LongWord; stdcall;
    function AddVertices(Count:LongWord):HResult; stdcall;
    function RemapVertices(NewVertexCount:LongWord;pVertexRemap:PLongWord):HResult; stdcall;
    function AddBones(Count:LongWord):HResult; stdcall;
    function RemoveBone(Index:LongWord):HResult; stdcall;
    function RemapBones(NewBoneCount:LongWord;pBoneRemap:PLongWord):HResult; stdcall;
    function AddBoneInfluences(BoneIndex:LongWord;InfluenceCount:LongWord;pIndices:PLongWord;pWeights:PSingle):HResult; stdcall;
    function ClearBoneInfluences(BoneIndex:LongWord):HResult; stdcall;
    function GetBoneInfluenceCount(BoneIndex:LongWord):LongWord; stdcall;
    function GetBoneInfluences(BoneIndex:LongWord;Offset:LongWord;Count:LongWord;pDestIndices:PLongWord;pDestWeights:PSingle):HResult; stdcall;
    function FindBoneInfluenceIndex(BoneIndex:LongWord;VertexIndex:LongWord;pInfluenceIndex:PLongWord):HResult; stdcall;
    function SetBoneInfluence(BoneIndex:LongWord;InfluenceIndex:LongWord;Weight:Single):HResult; stdcall;
    function GetBoneInfluence(BoneIndex:LongWord;InfluenceIndex:LongWord;pWeight:PSingle):HResult; stdcall;
    function Compact(MaxPerVertexInfluences:LongWord;ScaleMode:LongWord;MinWeight:Single):HResult; stdcall;
    function DoSoftwareSkinning(StartVertex:LongWord;VertexCount:LongWord;pSrcVertices:Pointer;SrcStride:LongWord;pDestVertices:Pointer;DestStride:LongWord;pBoneMatrices:PTD3DXMatrix;pInverseTransposeBoneMatrices:PTD3DXMatrix;pChannelDescs:PTD3DX10_SkinningChannel;NumChannels:LongWord):HResult; stdcall;
  end;

var D3DX10CreateSkinInfo: function(out SkinInfo:ID3DX10SkinInfo):HResult; stdcall;

type
  TD3DX10_AttributeWeights=record
    Position:Single;
    Boundary:Single;
    Normal:Single;
    Diffuse:Single;
    Specular:Single;
    Texcoord:array[0..7] of Single;
    Tangent:Single;
    Binormal:Single;
  end;
  PTD3DX10_AttributeWeights=^TD3DX10_AttributeWeights;
  D3DX10_ATTRIBUTE_WEIGHTS=TD3DX10_AttributeWeights;
  PD3DX10_ATTRIBUTE_WEIGHTS=^TD3DX10_AttributeWeights;


// =============== D3DX10Async.h =====================

var D3DX10CompileFromFileA: function(pSrcFile:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CompileFromFileW: function(pSrcFile:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CompileFromFile: function(pSrcFile:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CompileFromFile: function(pSrcFile:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CompileFromResourceA: function(hSrcModule:HMODULE;pSrcResource:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CompileFromResourceW: function(hSrcModule:HMODULE;pSrcResource:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CompileFromResource: function(hSrcModule:HMODULE;pSrcResource:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CompileFromResource: function(hSrcModule:HMODULE;pSrcResource:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CompileFromMemory: function(pSrcData:PAnsiChar;SrcDataLen:SIZE_T;pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;Pump:ID3DX10ThreadPump;out Shader:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromFileA: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromFileW: function(pFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromMemory: function(pData:Pointer;DataLength:SIZE_T;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromResourceA: function(hModule:HMODULE;pResourceName:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromResourceW: function(hModule:HMODULE;pResourceName:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateEffectFromFile: function(pFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromResource: function(hModule:HMODULE;pResourceName:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CreateEffectFromFile: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectFromResource: function(hModule:HMODULE;pResourceName:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;EffectPool:ID3D10EffectPool;Pump:ID3DX10ThreadPump;out Effect:ID3D10Effect;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CreateEffectPoolFromFileA: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectPoolFromFileW: function(pFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectPoolFromMemory: function(pData:Pointer;DataLength:SIZE_T;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectPoolFromResourceA: function(hModule:HMODULE;pResourceName:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectPoolFromResourceW: function(hModule:HMODULE;pResourceName:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateEffectPoolFromFile: function(pFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectPoolFromResource: function(hModule:HMODULE;pResourceName:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10CreateEffectPoolFromFile: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10CreateEffectPoolFromResource: function(hModule:HMODULE;pResourceName:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;HLSLFlags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pump:ID3DX10ThreadPump;out EffectPool:ID3D10EffectPool;out Errors:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10PreprocessShaderFromFileA: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10PreprocessShaderFromFileW: function(pFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10PreprocessShaderFromMemory: function(pSrcData:PAnsiChar;SrcDataSize:SIZE_T;pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10PreprocessShaderFromResourceA: function(hModule:HMODULE;pResourceName:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10PreprocessShaderFromResourceW: function(hModule:HMODULE;pResourceName:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10PreprocessShaderFromFile: function(pFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10PreprocessShaderFromResource: function(hModule:HMODULE;pResourceName:PWideChar;pSrcFileName:PWideChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ELSE}

var D3DX10PreprocessShaderFromFile: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;
var D3DX10PreprocessShaderFromResource: function(hModule:HMODULE;pResourceName:PAnsiChar;pSrcFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;Pump:ID3DX10ThreadPump;out ShaderText:ID3D10Blob;out ErrorMsgs:ID3D10Blob;pHResult:PHResult):HResult; stdcall;

{$ENDIF}

var D3DX10CreateAsyncCompilerProcessor: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pFunctionName:PAnsiChar;pProfile:PAnsiChar;Flags1:LongWord;Flags2:LongWord;out CompiledShader:ID3D10Blob;out ErrorBuffer:ID3D10Blob;out Processor:ID3DX10DataProcessor):HResult; stdcall;
var D3DX10CreateAsyncEffectCreateProcessor: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;Flags:LongWord;FXFlags:LongWord;Device:ID3D10Device;Pool:ID3D10EffectPool;out ErrorBuffer:ID3D10Blob;out Processor:ID3DX10DataProcessor):HResult; stdcall;
var D3DX10CreateAsyncEffectPoolCreateProcessor: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;pProfile:PAnsiChar;Flags:LongWord;FXFlags:LongWord;Device:ID3D10Device;out ErrorBuffer:ID3D10Blob;out Processor:ID3DX10DataProcessor):HResult; stdcall;
var D3DX10CreateAsyncShaderPreprocessProcessor: function(pFileName:PAnsiChar;const Defines:TD3D10_ShaderMacro;Include:ID3D10Include;out ShaderText:ID3D10Blob;out ErrorBuffer:ID3D10Blob;out Processor:ID3DX10DataProcessor):HResult; stdcall;
var D3DX10CreateAsyncFileLoaderW: function(pFileName:PWideChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;
var D3DX10CreateAsyncFileLoaderA: function(pFileName:PAnsiChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;
var D3DX10CreateAsyncMemoryLoader: function(pData:Pointer;CbData:SIZE_T;out DataLoader:ID3DX10DataLoader):HResult; stdcall;
var D3DX10CreateAsyncResourceLoaderW: function(hSrcModule:HMODULE;pSrcResource:PWideChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;
var D3DX10CreateAsyncResourceLoaderA: function(hSrcModule:HMODULE;pSrcResource:PAnsiChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;

{$IFDEF UNICODE}

var D3DX10CreateAsyncFileLoader: function(pFileName:PWideChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;
var D3DX10CreateAsyncResourceLoader: function(hSrcModule:HMODULE;pSrcResource:PWideChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;

{$ELSE}

var D3DX10CreateAsyncFileLoader: function(pFileName:PAnsiChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;
var D3DX10CreateAsyncResourceLoader: function(hSrcModule:HMODULE;pSrcResource:PAnsiChar;out DataLoader:ID3DX10DataLoader):HResult; stdcall;

{$ENDIF}

var D3DX10CreateAsyncTextureProcessor: function(Device:ID3D10Device;pLoadInfo:PTD3DX10_ImageLoadInfo;out DataProcessor:ID3DX10DataProcessor):HResult; stdcall;
var D3DX10CreateAsyncTextureInfoProcessor: function(pImageInfo:PTD3DX10_ImageInfo;out DataProcessor:ID3DX10DataProcessor):HResult; stdcall;
var D3DX10CreateAsyncShaderResourceViewProcessor: function(Device:ID3D10Device;pLoadInfo:PTD3DX10_ImageLoadInfo;out DataProcessor:ID3DX10DataProcessor):HResult; stdcall;


// =============== D3DX10.h =====================

const
  D3DX10_DEFAULT=-1;
  D3DX10_FROM_FILE=-3;
  DXGI_FORMAT_FROM_FILE=TDXGI_Format(-3);
  _FACDD=$876;
  DD_STATUS_Base=UINT(_FACDD shl 16);
  DD_HRESULT_Base=DD_STATUS_Base or UINT(1 shl 31);

const
  D3DX10_ERR_CANNOT_MODIFY_INDEX_BUFFER=HResult(DD_HRESULT_Base or 2900);
  D3DX10_ERR_INVALID_MESH=HResult(DD_HRESULT_Base or 2901);
  D3DX10_ERR_CANNOT_ATTR_SORT=HResult(DD_HRESULT_Base or 2902);
  D3DX10_ERR_SKINNING_NOT_SUPPORTED=HResult(DD_HRESULT_Base or 2903);
  D3DX10_ERR_TOO_MANY_INFLUENCES=HResult(DD_HRESULT_Base or 2904);
  D3DX10_ERR_INVALID_DATA=HResult(DD_HRESULT_Base or 2905);
  D3DX10_ERR_LOADED_MESH_HAS_NO_DATA=HResult(DD_HRESULT_Base or 2906);
  D3DX10_ERR_DUPLICATE_NAMED_FRAGMENT=HResult(DD_HRESULT_Base or 2907);
  D3DX10_ERR_CANNOT_REMOVE_LAST_ITEM=HResult(DD_HRESULT_Base or 2908);


function MAKE_DD_HRESULT(i_Code:LongWord):LongWord; inline;
function MAKE_DD_STATUS(i_Code:LongWord):LongWord; inline;

function D3DXToRadian(Degree:Double):Double; overload; inline;
function D3DXToRadian(Degree:Single):Single; overload; inline;

function D3DXToDegree(Radian:Double):Double; overload; inline;
function D3DXToDegree(Radian:Single):Single; overload; inline;


//============================================================
//============================================================
//============================================================

procedure Link;

implementation

function D3DXVector2(X,Y:Single):TD3DXVector2;
begin
  Result.X:=X;
  Result.Y:=Y;
end;

function D3DXVector3(X,Y,Z:Single):TD3DXVector3;
begin
  Result.X:=X;
  Result.Y:=Y;
  Result.Z:=Z;
end;

function D3DXVector4(X,Y,Z,W:Single):TD3DXVector4;
begin
  Result.X:=X;
  Result.Y:=Y;
  Result.Z:=Z;
  Result.W:=W;
end;


function MAKE_DD_HRESULT(i_Code:LongWord):LongWord; inline;
begin
  Result:=DD_HRESULT_Base or i_Code;
end;

function MAKE_DD_STATUS(i_Code:LongWord):LongWord; inline;
begin
  Result:=DD_STATUS_Base or i_Code;
end;


function D3DXToRadian(Degree:Single):Single;
begin
  Result:=Degree*(D3DX_PI/180.0);
end;

function D3DXToRadian(Degree: Double):Double;
begin
  Result:=Degree*(D3DX_PI / 180.0);
end;

function D3DXToDegree(Radian: Single):Single;
begin
  Result:=Radian*(180.0/D3DX_PI);
end;

function D3DXToDegree(Radian: Double):Double;
begin
  Result:=Radian*(180.0/D3DX_PI);
end;


procedure NotImplemented;
begin
  raise Exception.Create('Not implemented.');
end;

{$Warnings Off} // Disable warnings for unfinished functions not returning values.

function D3DXVec2Length(const V:TD3DXVector2):Single;
begin
  Result:=Sqrt(V.X*V.X+V.Y*V.Y);
end;

function D3DXVec2LengthSq(const V:TD3DXVector2):Single;
begin
  Result:=V.X*V.X+V.Y*V.Y;
end;

function D3DXVec2Dot(const V1:TD3DXVector2;const V2:TD3DXVector2):Single;
begin
  Result:=V1.X*V2.X+V1.Y*V2.Y;
end;

function D3DXVec2CCW(const V1:TD3DXVector2;const V2:TD3DXVector2):Single;
// Z component of (V1.X,V1.Y,0) cross (V2.X,V2.Y,0).
// See MSDN.
begin
  Result:=V1.X*V2.Y-V1.Y*V2.X;
end;

function D3DXVec2Add(out _Out:TD3DXVector2;const V1:TD3DXVector2;const V2:TD3DXVector2):PTD3DXVector2;
begin
  _Out.X:=V1.X+V2.X;
  _Out.Y:=V1.Y+V2.Y;

  Result:=@_Out;
end;

function D3DXVec2Subtract(out _Out:TD3DXVector2;const V1:TD3DXVector2;const V2:TD3DXVector2):PTD3DXVector2;
begin
  _Out.X:=V1.X-V2.X;
  _Out.Y:=V1.Y-V2.Y;

  Result:=@_Out;
end;

function D3DXVec2Minimize(out _Out:TD3DXVector2;const V1:TD3DXVector2;const V2:TD3DXVector2):PTD3DXVector2;
begin
  if V1.X<V2.X then _Out.X:=V1.X else _Out.X:=V2.X;
  if V1.Y<V2.Y then _Out.Y:=V1.Y else _Out.Y:=V2.Y;

  Result:=@_Out;
end;

function D3DXVec2Maximize(out _Out:TD3DXVector2;const V1:TD3DXVector2;const V2:TD3DXVector2):PTD3DXVector2;
begin
  if V1.X>V2.X then _Out.X:=V1.X else _Out.X:=V2.X;
  if V1.Y>V2.Y then _Out.Y:=V1.Y else _Out.Y:=V2.Y;

  Result:=@_Out;
end;

function D3DXVec2Scale(out _Out:TD3DXVector2;const V:TD3DXVector2;S:Single):PTD3DXVector2;
begin
  _Out.X:=S*V.X;
  _Out.Y:=S*V.Y;

  Result:=@_Out;
end;

function D3DXVec2Lerp(out _Out:TD3DXVector2;const V1:TD3DXVector2;const V2:TD3DXVector2;S:Single):PTD3DXVector2;
begin
  _Out.X:=V1.X+S*(V2.X-V1.X);
  _Out.Y:=V1.Y+S*(V2.Y-V1.Y);

  Result:=@_Out;
end;

function D3DXVec3Length(const V:TD3DXVector3):Single;
begin
  Result:=Sqrt(V.X*V.X+V.Y*V.Y+V.Z*V.Z);
end;

function D3DXVec3LengthSq(const V:TD3DXVector3):Single;
begin
  Result:=V.X*V.X+V.Y*V.Y+V.Z*V.Z;
end;

function D3DXVec3Dot(const V1:TD3DXVector3;const V2:TD3DXVector3):Single;
begin
  Result:=V1.X*V2.X+V1.Y*V2.Y+V1.Z*V2.Z;
end;

function D3DXVec3Cross(out _Out:TD3DXVector3;const V1:TD3DXVector3;const V2:TD3DXVector3):PTD3DXVector3;
begin
  _out.X:=V1.Y*V2.Z-V1.Z*V2.Y;
  _out.Y:=V1.Z*V2.X-V1.X*V2.Z;
  _out.Z:=V1.X*V2.Y-V1.Y*V2.X;

  Result:=@_Out;
end;

function D3DXVec3Add(out _Out:TD3DXVector3;const V1:TD3DXVector3;const V2:TD3DXVector3):PTD3DXVector3;
begin
  _Out.X:=V1.X+V2.X;
  _Out.Y:=V1.Y+V2.Y;
  _Out.Z:=V1.Z+V2.Z;

  Result:=@_Out;
end;

function D3DXVec3Subtract(out _Out:TD3DXVector3;const V1:TD3DXVector3;const V2:TD3DXVector3):PTD3DXVector3;
begin
  _Out.X:=V1.X-V2.X;
  _Out.Y:=V1.Y-V2.Y;
  _Out.Z:=V1.Z-V2.Z;

  Result:=@_Out;
end;

function D3DXVec3Minimize(out _Out:TD3DXVector3;const V1:TD3DXVector3;const V2:TD3DXVector3):PTD3DXVector3;
begin
  if V1.X<V2.X then _Out.X:=V1.X else _Out.X:=V2.X;
  if V1.Y<V2.Y then _Out.Y:=V1.Y else _Out.Y:=V2.Y;
  if V1.Z<V2.Z then _Out.Z:=V1.Z else _Out.Z:=V2.Z;

  Result:=@_Out;
end;

function D3DXVec3Maximize(out _Out:TD3DXVector3;const V1:TD3DXVector3;const V2:TD3DXVector3):PTD3DXVector3;
begin
  if V1.X>V2.X then _Out.X:=V1.X else _Out.X:=V2.X;
  if V1.Y>V2.Y then _Out.Y:=V1.Y else _Out.Y:=V2.Y;
  if V1.Z>V2.Z then _Out.Z:=V1.Z else _Out.Z:=V2.Z;

  Result:=@_Out;
end;

function D3DXVec3Scale(out _Out:TD3DXVector3;const V:TD3DXVector3;S:Single):PTD3DXVector3;
begin
  _Out.X:=S*V.X;
  _Out.Y:=S*V.Y;
  _Out.Z:=S*V.Z;

  Result:=@_Out;
end;

function D3DXVec3Lerp(out _Out:TD3DXVector3;const V1:TD3DXVector3;const V2:TD3DXVector3;S:Single):PTD3DXVector3;
begin
  _Out.X:=V1.X+S*(V2.X-V1.X);
  _Out.Y:=V1.Y+S*(V2.Y-V1.Y);
  _Out.Z:=V1.Z+S*(V2.Z-V1.Z);

  Result:=@_Out;
end;

function D3DXVec4Length(const V:TD3DXVector4):Single;
begin
  Result:=Sqrt(V.X*V.X+V.Y*V.Y+V.Z*V.Z+V.W*V.W);
end;

function D3DXVec4LengthSq(const V:TD3DXVector4):Single;
begin
  Result:=V.X*V.X+V.Y*V.Y+V.Z*V.Z+V.W*V.W;
end;

function D3DXVec4Dot(const V1:TD3DXVector4;const V2:TD3DXVector4):Single;
begin
  Result:=V1.X*V2.X+V1.Y*V2.Y+V1.Z*V2.Z+V1.W*V2.W;
end;

function D3DXVec4Add(out _Out:TD3DXVector4;const V1:TD3DXVector4;const V2:TD3DXVector4):PTD3DXVector4;
begin
  _Out.X:=V1.X+V2.X;
  _Out.Y:=V1.Y+V2.Y;
  _Out.Z:=V1.Z+V2.Z;
  _Out.W:=V1.W+V2.W;

  Result:=@_Out;
end;

function D3DXVec4Subtract(out _Out:TD3DXVector4;const V1:TD3DXVector4;const V2:TD3DXVector4):PTD3DXVector4;
begin
  _Out.X:=V1.X-V2.X;
  _Out.Y:=V1.Y-V2.Y;
  _Out.Z:=V1.Z-V2.Z;
  _Out.Z:=V1.W-V2.W;
end;

function D3DXVec4Minimize(out _Out:TD3DXVector4;const V1:TD3DXVector4;const V2:TD3DXVector4):PTD3DXVector4;
begin
  if V1.X<V2.X then _Out.X:=V1.X else _Out.X:=V2.X;
  if V1.Y<V2.Y then _Out.Y:=V1.Y else _Out.Y:=V2.Y;
  if V1.Z<V2.Z then _Out.Z:=V1.Z else _Out.Z:=V2.Z;
  if V1.W<V2.W then _Out.W:=V1.W else _Out.W:=V2.W;

  Result:=@_Out;
end;

function D3DXVec4Maximize(out _Out:TD3DXVector4;const V1:TD3DXVector4;const V2:TD3DXVector4):PTD3DXVector4;
begin
  if V1.X>V2.X then _Out.X:=V1.X else _Out.X:=V2.X;
  if V1.Y>V2.Y then _Out.Y:=V1.Y else _Out.Y:=V2.Y;
  if V1.Z>V2.Z then _Out.Z:=V1.Z else _Out.Z:=V2.Z;
  if V1.W>V2.W then _Out.W:=V1.W else _Out.W:=V2.W;

  Result:=@_Out;
end;

function D3DXVec4Scale(out _Out:TD3DXVector4;const V:TD3DXVector4;S:Single):PTD3DXVector4;
begin
  _Out.X:=S*V.X;
  _Out.Y:=S*V.Y;
  _Out.Z:=S*V.Z;
  _Out.W:=S*V.W;

  Result:=@_Out;
end;

function D3DXVec4Lerp(out _Out:TD3DXVector4;const V1:TD3DXVector4;const V2:TD3DXVector4;S:Single):PTD3DXVector4;
begin
  _Out.X:=V1.X+S*(V2.X-V1.X);
  _Out.Y:=V1.Y+S*(V2.Y-V1.Y);
  _Out.Z:=V1.Z+S*(V2.Z-V1.Z);
  _Out.W:=V1.W+S*(V2.W-V1.W);

  Result:=@_Out;
end;

function D3DXMatrixIdentity:TD3DXMatrix;
begin
  ZeroMemory(@Result,sizeof(Result));

  Result._11:=1;
  Result._22:=1;
  Result._33:=1;
  Result._44:=1;
end;

function D3DXMatrixIdentity(out M:TD3DXMatrix):PTD3DXMatrix;
begin
  M:=D3DXMatrixIdentity;

  Result:=@M;
end;

function D3DXMatrixIsIdentity(const M:TD3DXMatrix):BOOL;
var
  I:TD3DXMatrix;
begin
  I:=D3DXMatrixIdentity;

  Result:=CompareMem(@M,@I,sizeof(M));
end;

function D3DXQuaternionLength(const Q:TD3DXQUATERNION):Single;
begin
  Result:=Sqrt(Q.X*Q.X+Q.Y*Q.Y+Q.Z*Q.Z+Q.W*Q.W);
end;

function D3DXQuaternionLengthSq(const Q:TD3DXQUATERNION):Single;
begin
  Result:=Q.X*Q.X+Q.Y*Q.Y+Q.Z*Q.Z+Q.W*Q.W;
end;

function D3DXQuaternionDot(const Q1:TD3DXQUATERNION;const Q2:TD3DXQUATERNION):Single;
begin
  Result:=Q1.X*Q2.X+Q1.Y*Q2.Y+Q1.Z*Q2.Z+Q1.W*Q2.W;
end;

function D3DXQuaternionIdentity(out _Out:TD3DXQUATERNION):PTD3DXQUATERNION;
begin
  _Out.X:=0.0;
  _Out.Y:=0.0;
  _Out.Z:=0.0;
  _Out.W:=1.0;

  Result:=@_Out;
end;

function D3DXQuaternionIsIdentity(const Q:TD3DXQUATERNION):BOOL;
begin
  Result:=((Q.X=0.0) and (Q.Y=0.0) and (Q.Z=0.0) and (Q.W=1.0));
end;

function D3DXQuaternionConjugate(out _Out:TD3DXQUATERNION;const Q:TD3DXQUATERNION):PTD3DXQUATERNION;
begin
  _Out.X:=-Q.X;
  _Out.Y:=-Q.Y;
  _Out.Z:=-Q.Z;
  _Out.W:=Q.W;

  Result:=@_Out;
end;

function D3DXPlaneDot(const P:TD3DXPLANE;const V:TD3DXVector4):Single;
begin
  Result:=P.A*V.X+P.B*V.Y+P.C*V.Z+P.D*V.W;
end;

function D3DXPlaneDotCoord(const P:TD3DXPLANE;const V:TD3DXVector3):Single;
begin
  Result:=P.A*V.X+P.B*V.Y+P.C*V.Z+P.D;
end;

function D3DXPlaneDotNormal(const P:TD3DXPLANE;const V:TD3DXVector3):Single;
begin
  Result:=P.A*V.X+P.B*V.Y+P.C*V.Z;
end;

function D3DXPlaneScale(out _Out:TD3DXPLANE;const P:TD3DXPLANE;S:Single):PTD3DXPLANE;
begin
  _Out.A:=S*P.A;
  _Out.B:=S*P.B;
  _Out.C:=S*P.C;
  _Out.D:=S*P.D;

  Result:=@_Out;
end;

function D3DXColorNegative(out _Out:TD3DXColor;const Color:TD3DXColor):PTD3DXColor;
begin
  _Out.R:=1.0-Color.R;
  _Out.G:=1.0-Color.G;
  _Out.B:=1.0-Color.B;
  _Out.A:=1.0-Color.A;

  Result:=@_Out;
end;

function D3DXColorAdd(out _Out:TD3DXColor;const Color1:TD3DXColor;const Color2:TD3DXColor):PTD3DXColor;
begin
  _Out.R:=Color1.R+Color2.R;
  _Out.G:=Color1.G+Color2.G;
  _Out.B:=Color1.B+Color2.B;
  _Out.A:=Color1.A+Color2.A;

  Result:=@_Out;
end;

function D3DXColorSubtract(out _Out:TD3DXColor;const Color1:TD3DXColor;const Color2:TD3DXColor):PTD3DXColor;
begin
  _Out.R:=Color1.R-Color2.R;
  _Out.G:=Color1.G-Color2.G;
  _Out.B:=Color1.B-Color2.B;
  _Out.A:=Color1.A-Color2.A;

  Result:=@_Out;
end;

function D3DXColorScale(out _Out:TD3DXColor;const Color:TD3DXColor;S:Single):PTD3DXColor;
begin
  _Out.R:=S*Color.R;
  _Out.G:=S*Color.G;
  _Out.B:=S*Color.B;
  _Out.A:=S*Color.A;

  Result:=@_Out;
end;

function D3DXColorModulate(out _Out:TD3DXColor;const Color1:TD3DXColor;const Color2:TD3DXColor):PTD3DXColor;
begin
  _Out.R:=Color1.R*Color2.R;
  _Out.G:=Color1.G*Color2.G;
  _Out.B:=Color1.B*Color2.B;
  _Out.A:=Color1.A*Color2.A;

  Result:=@_Out;
end;

function D3DXColorLerp(out _Out:TD3DXColor;const Color1:TD3DXColor;const Color2:TD3DXColor;S:Single):PTD3DXColor;
begin
  _Out.R:=Color1.R+S*(Color2.R-Color1.R);
  _Out.G:=Color1.G+S*(Color2.G-Color1.G);
  _Out.B:=Color1.B+S*(Color2.B-Color1.B);
  _Out.A:=Color1.A+S*(Color2.A-Color1.A);

  Result:=@_Out;
end;

{$Warnings On}


function LoadDLL(DLLName:String):HModule;
begin
  Result:=LoadLibrary(PChar(DLLName));
  if Result=0 then
    raise Exception.Create('Dynamic link library (DLL) '''+DLLName+''' is not available.');
end;

function LinkMethod(hDLL:HModule;MethodName,DLLName:String):Pointer;
begin
  Result:=GetProcAddress(hDLL,PChar(MethodName));
  if Result=nil then
    raise Exception.Create('Failed to link to method '''+MethodName+''' in dynamic link library (DLL) '''+DLLName+'''.');
end;

procedure Link;
var
  hDLL_D3DX10:HModule;
begin
  hDLL_D3DX10:=LoadDLL(DLL_D3DX10);

  D3DXFloat32To16Array:=LinkMethod(hDLL_D3DX10,'D3DXFloat32To16Array',DLL_D3DX10);
  D3DXFloat16To32Array:=LinkMethod(hDLL_D3DX10,'D3DXFloat16To32Array',DLL_D3DX10);
  D3DXVec2Normalize:=LinkMethod(hDLL_D3DX10,'D3DXVec2Normalize',DLL_D3DX10);
  D3DXVec2Hermite:=LinkMethod(hDLL_D3DX10,'D3DXVec2Hermite',DLL_D3DX10);
  D3DXVec2CatmullRom:=LinkMethod(hDLL_D3DX10,'D3DXVec2CatmullRom',DLL_D3DX10);
  D3DXVec2BaryCentric:=LinkMethod(hDLL_D3DX10,'D3DXVec2BaryCentric',DLL_D3DX10);
  D3DXVec2Transform:=LinkMethod(hDLL_D3DX10,'D3DXVec2Transform',DLL_D3DX10);
  D3DXVec2TransformCoord:=LinkMethod(hDLL_D3DX10,'D3DXVec2TransformCoord',DLL_D3DX10);
  D3DXVec2TransformNormal:=LinkMethod(hDLL_D3DX10,'D3DXVec2TransformNormal',DLL_D3DX10);
  D3DXVec2TransformArray:=LinkMethod(hDLL_D3DX10,'D3DXVec2TransformArray',DLL_D3DX10);
  D3DXVec2TransformCoordArray:=LinkMethod(hDLL_D3DX10,'D3DXVec2TransformCoordArray',DLL_D3DX10);
  D3DXVec2TransformNormalArray:=LinkMethod(hDLL_D3DX10,'D3DXVec2TransformNormalArray',DLL_D3DX10);
  D3DXVec3Normalize:=LinkMethod(hDLL_D3DX10,'D3DXVec3Normalize',DLL_D3DX10);
  D3DXVec3Hermite:=LinkMethod(hDLL_D3DX10,'D3DXVec3Hermite',DLL_D3DX10);
  D3DXVec3CatmullRom:=LinkMethod(hDLL_D3DX10,'D3DXVec3CatmullRom',DLL_D3DX10);
  D3DXVec3BaryCentric:=LinkMethod(hDLL_D3DX10,'D3DXVec3BaryCentric',DLL_D3DX10);
  D3DXVec3Transform:=LinkMethod(hDLL_D3DX10,'D3DXVec3Transform',DLL_D3DX10);
  D3DXVec3TransformCoord:=LinkMethod(hDLL_D3DX10,'D3DXVec3TransformCoord',DLL_D3DX10);
  D3DXVec3TransformNormal:=LinkMethod(hDLL_D3DX10,'D3DXVec3TransformNormal',DLL_D3DX10);
  D3DXVec3TransformArray:=LinkMethod(hDLL_D3DX10,'D3DXVec3TransformArray',DLL_D3DX10);
  D3DXVec3TransformCoordArray:=LinkMethod(hDLL_D3DX10,'D3DXVec3TransformCoordArray',DLL_D3DX10);
  D3DXVec3TransformNormalArray:=LinkMethod(hDLL_D3DX10,'D3DXVec3TransformNormalArray',DLL_D3DX10);
  D3DXVec3Project:=LinkMethod(hDLL_D3DX10,'D3DXVec3Project',DLL_D3DX10);
  D3DXVec3Unproject:=LinkMethod(hDLL_D3DX10,'D3DXVec3Unproject',DLL_D3DX10);
  D3DXVec3ProjectArray:=LinkMethod(hDLL_D3DX10,'D3DXVec3ProjectArray',DLL_D3DX10);
  D3DXVec3UnprojectArray:=LinkMethod(hDLL_D3DX10,'D3DXVec3UnprojectArray',DLL_D3DX10);
  D3DXVec4Cross:=LinkMethod(hDLL_D3DX10,'D3DXVec4Cross',DLL_D3DX10);
  D3DXVec4Normalize:=LinkMethod(hDLL_D3DX10,'D3DXVec4Normalize',DLL_D3DX10);
  D3DXVec4Hermite:=LinkMethod(hDLL_D3DX10,'D3DXVec4Hermite',DLL_D3DX10);
  D3DXVec4CatmullRom:=LinkMethod(hDLL_D3DX10,'D3DXVec4CatmullRom',DLL_D3DX10);
  D3DXVec4BaryCentric:=LinkMethod(hDLL_D3DX10,'D3DXVec4BaryCentric',DLL_D3DX10);
  D3DXVec4Transform:=LinkMethod(hDLL_D3DX10,'D3DXVec4Transform',DLL_D3DX10);
  D3DXVec4TransformArray:=LinkMethod(hDLL_D3DX10,'D3DXVec4TransformArray',DLL_D3DX10);
  D3DXMatrixDeterminant:=LinkMethod(hDLL_D3DX10,'D3DXMatrixDeterminant',DLL_D3DX10);
  D3DXMatrixDecompose:=LinkMethod(hDLL_D3DX10,'D3DXMatrixDecompose',DLL_D3DX10);
  D3DXMatrixTranspose:=LinkMethod(hDLL_D3DX10,'D3DXMatrixTranspose',DLL_D3DX10);
  D3DXMatrixMultiply:=LinkMethod(hDLL_D3DX10,'D3DXMatrixMultiply',DLL_D3DX10);
  D3DXMatrixMultiplyTranspose:=LinkMethod(hDLL_D3DX10,'D3DXMatrixMultiplyTranspose',DLL_D3DX10);
  D3DXMatrixInverse:=LinkMethod(hDLL_D3DX10,'D3DXMatrixInverse',DLL_D3DX10);
  D3DXMatrixScaling:=LinkMethod(hDLL_D3DX10,'D3DXMatrixScaling',DLL_D3DX10);
  D3DXMatrixTranslation:=LinkMethod(hDLL_D3DX10,'D3DXMatrixTranslation',DLL_D3DX10);
  D3DXMatrixRotationX:=LinkMethod(hDLL_D3DX10,'D3DXMatrixRotationX',DLL_D3DX10);
  D3DXMatrixRotationY:=LinkMethod(hDLL_D3DX10,'D3DXMatrixRotationY',DLL_D3DX10);
  D3DXMatrixRotationZ:=LinkMethod(hDLL_D3DX10,'D3DXMatrixRotationZ',DLL_D3DX10);
  D3DXMatrixRotationAxis:=LinkMethod(hDLL_D3DX10,'D3DXMatrixRotationAxis',DLL_D3DX10);
  D3DXMatrixRotationQuaternion:=LinkMethod(hDLL_D3DX10,'D3DXMatrixRotationQuaternion',DLL_D3DX10);
  D3DXMatrixRotationYawPitchRoll:=LinkMethod(hDLL_D3DX10,'D3DXMatrixRotationYawPitchRoll',DLL_D3DX10);
  D3DXMatrixTransformation:=LinkMethod(hDLL_D3DX10,'D3DXMatrixTransformation',DLL_D3DX10);
  D3DXMatrixTransformation2D:=LinkMethod(hDLL_D3DX10,'D3DXMatrixTransformation2D',DLL_D3DX10);
  D3DXMatrixAffineTransformation:=LinkMethod(hDLL_D3DX10,'D3DXMatrixAffineTransformation',DLL_D3DX10);
  D3DXMatrixAffineTransformation2D:=LinkMethod(hDLL_D3DX10,'D3DXMatrixAffineTransformation2D',DLL_D3DX10);
  D3DXMatrixLookAtRH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixLookAtRH',DLL_D3DX10);
  D3DXMatrixLookAtLH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixLookAtLH',DLL_D3DX10);
  D3DXMatrixPerspectiveRH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixPerspectiveRH',DLL_D3DX10);
  D3DXMatrixPerspectiveLH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixPerspectiveLH',DLL_D3DX10);
  D3DXMatrixPerspectiveFovRH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixPerspectiveFovRH',DLL_D3DX10);
  D3DXMatrixPerspectiveFovLH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixPerspectiveFovLH',DLL_D3DX10);
  D3DXMatrixPerspectiveOffCenterRH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixPerspectiveOffCenterRH',DLL_D3DX10);
  D3DXMatrixPerspectiveOffCenterLH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixPerspectiveOffCenterLH',DLL_D3DX10);
  D3DXMatrixOrthoRH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixOrthoRH',DLL_D3DX10);
  D3DXMatrixOrthoLH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixOrthoLH',DLL_D3DX10);
  D3DXMatrixOrthoOffCenterRH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixOrthoOffCenterRH',DLL_D3DX10);
  D3DXMatrixOrthoOffCenterLH:=LinkMethod(hDLL_D3DX10,'D3DXMatrixOrthoOffCenterLH',DLL_D3DX10);
  D3DXMatrixShadow:=LinkMethod(hDLL_D3DX10,'D3DXMatrixShadow',DLL_D3DX10);
  D3DXMatrixReflect:=LinkMethod(hDLL_D3DX10,'D3DXMatrixReflect',DLL_D3DX10);
  D3DXQuaternionToAxisAngle:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionToAxisAngle',DLL_D3DX10);
  D3DXQuaternionRotationMatrix:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionRotationMatrix',DLL_D3DX10);
  D3DXQuaternionRotationAxis:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionRotationAxis',DLL_D3DX10);
  D3DXQuaternionRotationYawPitchRoll:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionRotationYawPitchRoll',DLL_D3DX10);
  D3DXQuaternionMultiply:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionMultiply',DLL_D3DX10);
  D3DXQuaternionNormalize:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionNormalize',DLL_D3DX10);
  D3DXQuaternionInverse:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionInverse',DLL_D3DX10);
  D3DXQuaternionLn:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionLn',DLL_D3DX10);
  D3DXQuaternionExp:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionExp',DLL_D3DX10);
  D3DXQuaternionSlerp:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionSlerp',DLL_D3DX10);
  D3DXQuaternionSquad:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionSquad',DLL_D3DX10);
  D3DXQuaternionSquadSetup:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionSquadSetup',DLL_D3DX10);
  D3DXQuaternionBaryCentric:=LinkMethod(hDLL_D3DX10,'D3DXQuaternionBaryCentric',DLL_D3DX10);
  D3DXPlaneNormalize:=LinkMethod(hDLL_D3DX10,'D3DXPlaneNormalize',DLL_D3DX10);
  D3DXPlaneIntersectLine:=LinkMethod(hDLL_D3DX10,'D3DXPlaneIntersectLine',DLL_D3DX10);
  D3DXPlaneFromPointNormal:=LinkMethod(hDLL_D3DX10,'D3DXPlaneFromPointNormal',DLL_D3DX10);
  D3DXPlaneFromPoints:=LinkMethod(hDLL_D3DX10,'D3DXPlaneFromPoints',DLL_D3DX10);
  D3DXPlaneTransform:=LinkMethod(hDLL_D3DX10,'D3DXPlaneTransform',DLL_D3DX10);
  D3DXPlaneTransformArray:=LinkMethod(hDLL_D3DX10,'D3DXPlaneTransformArray',DLL_D3DX10);
  D3DXColorAdjustSaturation:=LinkMethod(hDLL_D3DX10,'D3DXColorAdjustSaturation',DLL_D3DX10);
  D3DXColorAdjustContrast:=LinkMethod(hDLL_D3DX10,'D3DXColorAdjustContrast',DLL_D3DX10);
  D3DXFresnelTerm:=LinkMethod(hDLL_D3DX10,'D3DXFresnelTerm',DLL_D3DX10);
  D3DXCreateMatrixStack:=LinkMethod(hDLL_D3DX10,'D3DXCreateMatrixStack',DLL_D3DX10);
  D3DXSHEvalDirection:=LinkMethod(hDLL_D3DX10,'D3DXSHEvalDirection',DLL_D3DX10);
  D3DXSHRotate:=LinkMethod(hDLL_D3DX10,'D3DXSHRotate',DLL_D3DX10);
  D3DXSHRotateZ:=LinkMethod(hDLL_D3DX10,'D3DXSHRotateZ',DLL_D3DX10);
  D3DXSHAdd:=LinkMethod(hDLL_D3DX10,'D3DXSHAdd',DLL_D3DX10);
  D3DXSHScale:=LinkMethod(hDLL_D3DX10,'D3DXSHScale',DLL_D3DX10);
  D3DXSHDot:=LinkMethod(hDLL_D3DX10,'D3DXSHDot',DLL_D3DX10);
  D3DXSHMultiply2:=LinkMethod(hDLL_D3DX10,'D3DXSHMultiply2',DLL_D3DX10);
  D3DXSHMultiply3:=LinkMethod(hDLL_D3DX10,'D3DXSHMultiply3',DLL_D3DX10);
  D3DXSHMultiply4:=LinkMethod(hDLL_D3DX10,'D3DXSHMultiply4',DLL_D3DX10);
  D3DXSHMultiply5:=LinkMethod(hDLL_D3DX10,'D3DXSHMultiply5',DLL_D3DX10);
  D3DXSHMultiply6:=LinkMethod(hDLL_D3DX10,'D3DXSHMultiply6',DLL_D3DX10);
  D3DXSHEvalDirectionalLight:=LinkMethod(hDLL_D3DX10,'D3DXSHEvalDirectionalLight',DLL_D3DX10);
  D3DXSHEvalSphericalLight:=LinkMethod(hDLL_D3DX10,'D3DXSHEvalSphericalLight',DLL_D3DX10);
  D3DXSHEvalConeLight:=LinkMethod(hDLL_D3DX10,'D3DXSHEvalConeLight',DLL_D3DX10);
  D3DXSHEvalHemisphereLight:=LinkMethod(hDLL_D3DX10,'D3DXSHEvalHemisphereLight',DLL_D3DX10);
  D3DXIntersectTri:=LinkMethod(hDLL_D3DX10,'D3DXIntersectTri',DLL_D3DX10);
  D3DXSphereBoundProbe:=LinkMethod(hDLL_D3DX10,'D3DXSphereBoundProbe',DLL_D3DX10);
  D3DXBoxBoundProbe:=LinkMethod(hDLL_D3DX10,'D3DXBoxBoundProbe',DLL_D3DX10);
  D3DXComputeBoundingSphere:=LinkMethod(hDLL_D3DX10,'D3DXComputeBoundingSphere',DLL_D3DX10);
  D3DXComputeBoundingBox:=LinkMethod(hDLL_D3DX10,'D3DXComputeBoundingBox',DLL_D3DX10);
  D3DXCpuOptimizations:=LinkMethod(hDLL_D3DX10,'D3DXCpuOptimizations',DLL_D3DX10);
  D3DX10CreateDevice:=LinkMethod(hDLL_D3DX10,'D3DX10CreateDevice',DLL_D3DX10);
  D3DX10CreateDeviceAndSwapChain:=LinkMethod(hDLL_D3DX10,'D3DX10CreateDeviceAndSwapChain',DLL_D3DX10);
  D3DX10GetFeatureLevel1:=LinkMethod(hDLL_D3DX10,'D3DX10GetFeatureLevel1',DLL_D3DX10);

{$IFDEF D3D_DIAG_DLL}

  D3DX10DebugMute:=LinkMethod(hDLL_D3DX10,'D3DX10DebugMute',DLL_D3DX10);

{$ENDIF}

  D3DX10CheckVersion:=LinkMethod(hDLL_D3DX10,'D3DX10CheckVersion',DLL_D3DX10);
  D3DX10CreateSprite:=LinkMethod(hDLL_D3DX10,'D3DX10CreateSprite',DLL_D3DX10);
  D3DX10CreateThreadPump:=LinkMethod(hDLL_D3DX10,'D3DX10CreateThreadPump',DLL_D3DX10);
  D3DX10CreateFontA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontA',DLL_D3DX10);
  D3DX10CreateFontW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateFont:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateFont:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateFontIndirectA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontIndirectA',DLL_D3DX10);
  D3DX10CreateFontIndirectW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontIndirectW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateFontIndirect:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontIndirectW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateFontIndirect:=LinkMethod(hDLL_D3DX10,'D3DX10CreateFontIndirectA',DLL_D3DX10);

{$ENDIF}

  D3DX10UnsetAllDeviceObjects:=LinkMethod(hDLL_D3DX10,'D3DX10UnsetAllDeviceObjects',DLL_D3DX10);
  D3DX10GetImageInfoFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromFileA',DLL_D3DX10);
  D3DX10GetImageInfoFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromFileW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10GetImageInfoFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromFileW',DLL_D3DX10);

{$ELSE}

  D3DX10GetImageInfoFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromFileA',DLL_D3DX10);

{$ENDIF}

  D3DX10GetImageInfoFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromResourceA',DLL_D3DX10);
  D3DX10GetImageInfoFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10GetImageInfoFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10GetImageInfoFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10GetImageInfoFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10GetImageInfoFromMemory',DLL_D3DX10);
  D3DX10CreateShaderResourceViewFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromFileA',DLL_D3DX10);
  D3DX10CreateShaderResourceViewFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromFileW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateShaderResourceViewFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromFileW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateShaderResourceViewFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromFileA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateTextureFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromFileA',DLL_D3DX10);
  D3DX10CreateTextureFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromFileW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateTextureFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromFileW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateTextureFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromFileA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateShaderResourceViewFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromResourceA',DLL_D3DX10);
  D3DX10CreateShaderResourceViewFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateShaderResourceViewFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateShaderResourceViewFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateTextureFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromResourceA',DLL_D3DX10);
  D3DX10CreateTextureFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateTextureFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateTextureFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateShaderResourceViewFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10CreateShaderResourceViewFromMemory',DLL_D3DX10);
  D3DX10CreateTextureFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10CreateTextureFromMemory',DLL_D3DX10);
  D3DX10LoadTextureFromTexture:=LinkMethod(hDLL_D3DX10,'D3DX10LoadTextureFromTexture',DLL_D3DX10);
  D3DX10FilterTexture:=LinkMethod(hDLL_D3DX10,'D3DX10FilterTexture',DLL_D3DX10);
  D3DX10SaveTextureToFileA:=LinkMethod(hDLL_D3DX10,'D3DX10SaveTextureToFileA',DLL_D3DX10);
  D3DX10SaveTextureToFileW:=LinkMethod(hDLL_D3DX10,'D3DX10SaveTextureToFileW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10SaveTextureToFile:=LinkMethod(hDLL_D3DX10,'D3DX10SaveTextureToFileW',DLL_D3DX10);

{$ELSE}

  D3DX10SaveTextureToFile:=LinkMethod(hDLL_D3DX10,'D3DX10SaveTextureToFileA',DLL_D3DX10);

{$ENDIF}

  D3DX10SaveTextureToMemory:=LinkMethod(hDLL_D3DX10,'D3DX10SaveTextureToMemory',DLL_D3DX10);
  D3DX10ComputeNormalMap:=LinkMethod(hDLL_D3DX10,'D3DX10ComputeNormalMap',DLL_D3DX10);
  D3DX10SHProjectCubeMap:=LinkMethod(hDLL_D3DX10,'D3DX10SHProjectCubeMap',DLL_D3DX10);
  D3DX10CreateMesh:=LinkMethod(hDLL_D3DX10,'D3DX10CreateMesh',DLL_D3DX10);
  D3DX10CreateSkinInfo:=LinkMethod(hDLL_D3DX10,'D3DX10CreateSkinInfo',DLL_D3DX10);
  D3DX10CompileFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromFileA',DLL_D3DX10);
  D3DX10CompileFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromFileW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CompileFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromFileW',DLL_D3DX10);

{$ELSE}

  D3DX10CompileFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromFileA',DLL_D3DX10);

{$ENDIF}

  D3DX10CompileFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromResourceA',DLL_D3DX10);
  D3DX10CompileFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CompileFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10CompileFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10CompileFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10CompileFromMemory',DLL_D3DX10);
  D3DX10CreateEffectFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromFileA',DLL_D3DX10);
  D3DX10CreateEffectFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromFileW',DLL_D3DX10);
  D3DX10CreateEffectFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromMemory',DLL_D3DX10);
  D3DX10CreateEffectFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromResourceA',DLL_D3DX10);
  D3DX10CreateEffectFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateEffectFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromFileW',DLL_D3DX10);
  D3DX10CreateEffectFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateEffectFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromFileA',DLL_D3DX10);
  D3DX10CreateEffectFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateEffectPoolFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromFileA',DLL_D3DX10);
  D3DX10CreateEffectPoolFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromFileW',DLL_D3DX10);
  D3DX10CreateEffectPoolFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromMemory',DLL_D3DX10);
  D3DX10CreateEffectPoolFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromResourceA',DLL_D3DX10);
  D3DX10CreateEffectPoolFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateEffectPoolFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromFileW',DLL_D3DX10);
  D3DX10CreateEffectPoolFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateEffectPoolFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromFileA',DLL_D3DX10);
  D3DX10CreateEffectPoolFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10CreateEffectPoolFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10PreprocessShaderFromFileA:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromFileA',DLL_D3DX10);
  D3DX10PreprocessShaderFromFileW:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromFileW',DLL_D3DX10);
  D3DX10PreprocessShaderFromMemory:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromMemory',DLL_D3DX10);
  D3DX10PreprocessShaderFromResourceA:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromResourceA',DLL_D3DX10);
  D3DX10PreprocessShaderFromResourceW:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromResourceW',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10PreprocessShaderFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromFileW',DLL_D3DX10);
  D3DX10PreprocessShaderFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromResourceW',DLL_D3DX10);

{$ELSE}

  D3DX10PreprocessShaderFromFile:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromFileA',DLL_D3DX10);
  D3DX10PreprocessShaderFromResource:=LinkMethod(hDLL_D3DX10,'D3DX10PreprocessShaderFromResourceA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateAsyncCompilerProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncCompilerProcessor',DLL_D3DX10);
  D3DX10CreateAsyncEffectCreateProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncEffectCreateProcessor',DLL_D3DX10);
  D3DX10CreateAsyncEffectPoolCreateProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncEffectPoolCreateProcessor',DLL_D3DX10);
  D3DX10CreateAsyncShaderPreprocessProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncShaderPreprocessProcessor',DLL_D3DX10);
  D3DX10CreateAsyncFileLoaderW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncFileLoaderW',DLL_D3DX10);
  D3DX10CreateAsyncFileLoaderA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncFileLoaderA',DLL_D3DX10);
  D3DX10CreateAsyncMemoryLoader:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncMemoryLoader',DLL_D3DX10);
  D3DX10CreateAsyncResourceLoaderW:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncResourceLoaderW',DLL_D3DX10);
  D3DX10CreateAsyncResourceLoaderA:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncResourceLoaderA',DLL_D3DX10);

{$IFDEF UNICODE}

  D3DX10CreateAsyncFileLoader:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncFileLoaderW',DLL_D3DX10);
  D3DX10CreateAsyncResourceLoader:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncResourceLoaderW',DLL_D3DX10);

{$ELSE}

  D3DX10CreateAsyncFileLoader:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncFileLoaderA',DLL_D3DX10);
  D3DX10CreateAsyncResourceLoader:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncResourceLoaderA',DLL_D3DX10);

{$ENDIF}

  D3DX10CreateAsyncTextureProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncTextureProcessor',DLL_D3DX10);
  D3DX10CreateAsyncTextureInfoProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncTextureInfoProcessor',DLL_D3DX10);
  D3DX10CreateAsyncShaderResourceViewProcessor:=LinkMethod(hDLL_D3DX10,'D3DX10CreateAsyncShaderResourceViewProcessor',DLL_D3DX10);
end;


end.
