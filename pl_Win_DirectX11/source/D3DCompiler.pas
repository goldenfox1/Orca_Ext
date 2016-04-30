{**********************************************************************
 Package pl_Win_DirectX11.pkg
 this unit is part of CodeTyphon Studio (http://www.pilotlogic.com/)
***********************************************************************}

unit D3DCompiler;

interface

{$Z4}

uses
  Windows, SysUtils, DXGITypes, D3DCommon, D3D10;

// =============== D3Dcompiler.h =====================

const
  D3DCOMPILER_DLL='d3dcompiler_43.dll';

const
  D3DCOMPILE_DEBUG=(1 shl 0);
  D3DCOMPILE_SKIP_VALIDATION=(1 shl 1);
  D3DCOMPILE_SKIP_OPTIMIZATION=(1 shl 2);
  D3DCOMPILE_PACK_MATRIX_ROW_MAJOR=(1 shl 3);
  D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR=(1 shl 4);
  D3DCOMPILE_PARTIAL_PRECISION=(1 shl 5);
  D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT=(1 shl 6);
  D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT=(1 shl 7);
  D3DCOMPILE_NO_PRESHADER=(1 shl 8);
  D3DCOMPILE_AVOID_FLOW_CONTROL=(1 shl 9);
  D3DCOMPILE_PREFER_FLOW_CONTROL=(1 shl 10);
  D3DCOMPILE_ENABLE_STRICTNESS=(1 shl 11);
  D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY=(1 shl 12);
  D3DCOMPILE_IEEE_STRICTNESS=(1 shl 13);
  D3DCOMPILE_OPTIMIZATION_LEVEL0=(1 shl 14);
  D3DCOMPILE_OPTIMIZATION_LEVEL1=0;
  D3DCOMPILE_OPTIMIZATION_LEVEL2=((1 shl 14) or (1 shl 15));
  D3DCOMPILE_OPTIMIZATION_LEVEL3=(1 shl 15);
  D3DCOMPILE_RESERVED16=(1 shl 16);
  D3DCOMPILE_RESERVED17=(1 shl 17);
  D3DCOMPILE_WARNINGS_ARE_ERRORS=(1 shl 18);
  D3DCOMPILE_EFFECT_CHILD_EFFECT=(1 shl 0);
  D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS=(1 shl 1);
  D3D_DISASM_ENABLE_COLOR_CODE=$00000001;
  D3D_DISASM_ENABLE_DEFAULT_VALUE_PRINTS=$00000002;
  D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING=$00000004;
  D3D_DISASM_ENABLE_INSTRUCTION_CYCLE=$00000008;
  D3D_DISASM_DISABLE_DEBUG_INFO=$00000010;
  D3D_COMPRESS_SHADER_KEEP_ALL_PARTS=$00000001;

var D3DCompile: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  pSourceName:PAnsiChar;  
  pDefines:PTD3D_ShaderMacro;
  Include:ID3DInclude;  
  pEntrypoint:PAnsiChar;  
  pTarget:PAnsiChar;  
  Flags1:LongWord;  
  Flags2:LongWord;  
  out Code:ID3DBlob;  
  {$IFDEF UsePointersForOptionalOutputInterfaces}pErrorMsgs:PID3DBlob{$ELSE}out ErrorMsgs:ID3DBlob{$ENDIF}
):HResult; stdcall;  

var D3DPreprocess: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  pSourceName:PAnsiChar;  
  pDefines:PTD3D_ShaderMacro;  
  Include:ID3DInclude;  
  out CodeText:ID3DBlob;  
  {$IFDEF UsePointersForOptionalOutputInterfaces}pErrorMsgs:PID3DBlob{$ELSE}out ErrorMsgs:ID3DBlob{$ENDIF}
):HResult; stdcall;  

var D3DGetDebugInfo: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  out DebugInfo:ID3DBlob  
):HResult; stdcall;  

var D3DReflect: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  const pInterface:TGUID;  
  out pReflector
):HResult; stdcall;  

var D3DDisassemble: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  Flags:LongWord;  
  Comments:PAnsiChar;  
  out Disassembly:ID3DBlob  
):HResult; stdcall;  

var D3DDisassemble10Effect: function
(
  Effect:ID3D10Effect;  
  Flags:LongWord;  
  out Disassembly:ID3DBlob  
):HResult; stdcall;  

var D3DGetInputSignatureBlob: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  out SignatureBlob:ID3DBlob  
):HResult; stdcall;  

var D3DGetOutputSignatureBlob: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  out SignatureBlob:ID3DBlob  
):HResult; stdcall;  

var D3DGetInputAndOutputSignatureBlob: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  out SignatureBlob:ID3DBlob  
):HResult; stdcall;  

type
  TD3DCOMPILER_STRIP_FLAGS=
  (
    D3DCOMPILER_STRIP_REFLECTION_DATA=1,
    D3DCOMPILER_STRIP_DEBUG_INFO=2,
    D3DCOMPILER_STRIP_TEST_BLOBS=4
  );
  PTD3DCOMPILER_STRIP_FLAGS=^TD3DCOMPILER_STRIP_FLAGS;
  D3DCOMPILER_STRIP_FLAGS=TD3DCOMPILER_STRIP_FLAGS;
  PD3DCOMPILER_STRIP_FLAGS=^TD3DCOMPILER_STRIP_FLAGS;

var D3DStripShader: function
(
  pShaderBytecode:Pointer;
  BytecodeLength:SIZE_T;  
  StripFlags:LongWord;  
  out StrippedBlob:ID3DBlob  
):HResult; stdcall;  

type
  TD3D_BlobPart=
  (
    D3D_BLOB_INPUT_SIGNATURE_BLOB,
    D3D_BLOB_OUTPUT_SIGNATURE_BLOB,
    D3D_BLOB_INPUT_AND_OUTPUT_SIGNATURE_BLOB,
    D3D_BLOB_PATCH_CONSTANT_SIGNATURE_BLOB,
    D3D_BLOB_ALL_SIGNATURE_BLOB,
    D3D_BLOB_DEBUG_INFO,
    D3D_BLOB_LEGACY_SHADER,
    D3D_BLOB_XNA_PREPASS_SHADER,
    D3D_BLOB_XNA_SHADER,
    D3D_BLOB_TEST_ALTERNATE_SHADER=$8000,
    D3D_BLOB_TEST_COMPILE_DETAILS,
    D3D_BLOB_TEST_COMPILE_PERF
  );
  PTD3D_BlobPart=^TD3D_BlobPart;
  D3D_BLOB_PART=TD3D_BlobPart;
  PD3D_BLOB_PART=^TD3D_BlobPart;

var D3DGetBlobPart: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  Part:TD3D_BlobPart;  
  Flags:LongWord;  
  out o_Part:ID3DBlob  
):HResult; stdcall;  

type
  TD3D_ShaderData=record
    pBytecode:Pointer;
    BytecodeLength:SIZE_T;
  end;
  PTD3D_ShaderData=^TD3D_ShaderData;
  D3D_SHADER_DATA=TD3D_ShaderData;
  PD3D_SHADER_DATA=^TD3D_ShaderData;

var D3DCompressShaders: function
(
  NumShaders:LongWord;  
  pShaderData:PTD3D_ShaderData;
  Flags:LongWord;  
  out CompressedData:ID3DBlob  
):HResult; stdcall;  

var D3DDecompressShaders: function
(
  pSrcData:Pointer;  
  SrcDataSize:SIZE_T;  
  NumShaders:LongWord;  
  StartIndex:LongWord;  
  pIndices:PLongWord;
  Flags:LongWord;  
  pShaders:PID3DBlob;
  pTotalShaders:PLongWord
):HResult; stdcall;  

var D3DCreateBlob: function
(
  Size:SIZE_T;  
  out Blob:ID3DBlob  
):HResult; stdcall;  



//============================================================
//============================================================
//============================================================

procedure Link;

implementation

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
  hD3DCOMPILER_DLL:HModule;
begin
  hD3DCOMPILER_DLL:=LoadDLL(D3DCOMPILER_DLL);

  D3DCompile:=LinkMethod(hD3DCOMPILER_DLL,'D3DCompile',D3DCOMPILER_DLL);
  D3DPreprocess:=LinkMethod(hD3DCOMPILER_DLL,'D3DPreprocess',D3DCOMPILER_DLL);
  D3DGetDebugInfo:=LinkMethod(hD3DCOMPILER_DLL,'D3DGetDebugInfo',D3DCOMPILER_DLL);
  D3DReflect:=LinkMethod(hD3DCOMPILER_DLL,'D3DReflect',D3DCOMPILER_DLL);
  D3DDisassemble:=LinkMethod(hD3DCOMPILER_DLL,'D3DDisassemble',D3DCOMPILER_DLL);
  D3DDisassemble10Effect:=LinkMethod(hD3DCOMPILER_DLL,'D3DDisassemble10Effect',D3DCOMPILER_DLL);
  D3DGetInputSignatureBlob:=LinkMethod(hD3DCOMPILER_DLL,'D3DGetInputSignatureBlob',D3DCOMPILER_DLL);
  D3DGetOutputSignatureBlob:=LinkMethod(hD3DCOMPILER_DLL,'D3DGetOutputSignatureBlob',D3DCOMPILER_DLL);
  D3DGetInputAndOutputSignatureBlob:=LinkMethod(hD3DCOMPILER_DLL,'D3DGetInputAndOutputSignatureBlob',D3DCOMPILER_DLL);
  D3DStripShader:=LinkMethod(hD3DCOMPILER_DLL,'D3DStripShader',D3DCOMPILER_DLL);
  D3DGetBlobPart:=LinkMethod(hD3DCOMPILER_DLL,'D3DGetBlobPart',D3DCOMPILER_DLL);
  D3DCompressShaders:=LinkMethod(hD3DCOMPILER_DLL,'D3DCompressShaders',D3DCOMPILER_DLL);
  D3DDecompressShaders:=LinkMethod(hD3DCOMPILER_DLL,'D3DDecompressShaders',D3DCOMPILER_DLL);
  D3DCreateBlob:=LinkMethod(hD3DCOMPILER_DLL,'D3DCreateBlob',D3DCOMPILER_DLL);
end;


end.
