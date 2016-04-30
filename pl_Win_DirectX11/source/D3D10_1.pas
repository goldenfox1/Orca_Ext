{**********************************************************************
 Package pl_Win_DirectX11.pkg
 this unit is part of CodeTyphon Studio (http://www.pilotlogic.com/)
***********************************************************************}

unit D3D10_1;

interface

{$Z4}

uses
  Windows, SysUtils,
  DXGITypes, DXGI, D3DCommon, D3D10;

const
  DLL_D3D10_1='d3d10_1.dll';

// =============== D3D10_1.h =====================

const
  D3D10_1_DEFAULT_SAMPLE_MASK=$ffffffff;
  D3D10_1_FLOAT16_FUSED_TOLERANCE_IN_ULP=0.6;
  D3D10_1_FLOAT32_TO_INTEGER_TOLERANCE_IN_ULP=0.6;
  D3D10_1_GS_INPUT_REGISTER_COUNT=32;
  D3D10_1_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT=32;
  D3D10_1_IA_VERTEX_INPUT_STRUCTURE_ELEMENTS_COMPONENTS=128;
  D3D10_1_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT=32;
  D3D10_1_PS_OUTPUT_MASK_REGISTER_COMPONENTS=1;
  D3D10_1_PS_OUTPUT_MASK_REGISTER_COMPONENT_BIT_COUNT=32;
  D3D10_1_PS_OUTPUT_MASK_REGISTER_COUNT=1;
  D3D10_1_SHADER_MAJOR_VERSION=4;
  D3D10_1_SHADER_MINOR_VERSION=1;
  D3D10_1_SO_BUFFER_MAX_STRIDE_IN_BYTES=2048;
  D3D10_1_SO_BUFFER_MAX_WRITE_WINDOW_IN_BYTES=256;
  D3D10_1_SO_BUFFER_SLOT_COUNT=4;
  D3D10_1_SO_MULTIPLE_BUFFER_ELEMENTS_PER_BUFFER=1;
  D3D10_1_SO_SINGLE_BUFFER_COMPONENT_LIMIT=64;
  D3D10_1_STANDARD_VERTEX_ELEMENT_COUNT=32;
  D3D10_1_SUBPIXEL_FRACTIONAL_BIT_COUNT=8;
  D3D10_1_VS_INPUT_REGISTER_COUNT=32;
  D3D10_1_VS_OUTPUT_REGISTER_COUNT=32;
  D3D10_1_SDK_VERSION=( 0 + $20 ) ;

type
  ID3D10BlendState1=interface;
  PID3D10BlendState1=^ID3D10BlendState1;

  ID3D10ShaderResourceView1=interface;
  PID3D10ShaderResourceView1=^ID3D10ShaderResourceView1;

  ID3D10Device1=interface;
  PID3D10Device1=^ID3D10Device1;

  TD3D10_FeatureLevel1=
  (
    D3D10_FEATURE_LEVEL_10_0=$a000,
    D3D10_FEATURE_LEVEL_10_1=$a100,
    D3D10_FEATURE_LEVEL_9_1=$9100,
    D3D10_FEATURE_LEVEL_9_2=$9200,
    D3D10_FEATURE_LEVEL_9_3=$9300
  );
  PTD3D10_FeatureLevel1=^TD3D10_FeatureLevel1;
  D3D10_FEATURE_LEVEL1=TD3D10_FeatureLevel1;
  PD3D10_FEATURE_LEVEL1=^TD3D10_FeatureLevel1;

  TD3D10_RenderTargetBlendDesc1=record
    BlendEnable:LongBool;
    SrcBlend:TD3D10_Blend;
    DestBlend:TD3D10_Blend;
    BlendOp:TD3D10_BlendOp;
    SrcBlendAlpha:TD3D10_Blend;
    DestBlendAlpha:TD3D10_Blend;
    BlendOpAlpha:TD3D10_BlendOp;
    RenderTargetWriteMask:Byte;
  end;
  PTD3D10_RenderTargetBlendDesc1=^TD3D10_RenderTargetBlendDesc1;
  D3D10_RENDER_TARGET_BLEND_DESC1=TD3D10_RenderTargetBlendDesc1;
  PD3D10_RENDER_TARGET_BLEND_DESC1=^TD3D10_RenderTargetBlendDesc1;

  TD3D10_BlendDesc1=record
    AlphaToCoverageEnable:LongBool;
    IndependentBlendEnable:LongBool;
    RenderTarget:array[0..7] of TD3D10_RenderTargetBlendDesc1;
  end;
  PTD3D10_BlendDesc1=^TD3D10_BlendDesc1;
  D3D10_BLEND_DESC1=TD3D10_BlendDesc1;
  PD3D10_BLEND_DESC1=^TD3D10_BlendDesc1;

  ID3D10BlendState1=interface(ID3D10BlendState)
    ['{EDAD8D99-8A35-4D6D-8566-2EA276CDE161}']
    procedure GetDesc1
    (
      out Desc:TD3D10_BlendDesc1  
    ); stdcall;
  end;

  TD3D10_TexcubeArraySrv1=record
    MostDetailedMip:LongWord;
    MipLevels:LongWord;
    First2DArrayFace:LongWord;
    NumCubes:LongWord;
  end;
  PTD3D10_TexcubeArraySrv1=^TD3D10_TexcubeArraySrv1;
  D3D10_TEXCUBE_ARRAY_SRV1=TD3D10_TexcubeArraySrv1;
  PD3D10_TEXCUBE_ARRAY_SRV1=^TD3D10_TexcubeArraySrv1;

  TD3D10_SrvDimension1=D3D_SRV_DIMENSION;
  PTD3D10_SrvDimension1=^TD3D10_SrvDimension1;
  D3D10_SRV_DIMENSION1=TD3D10_SrvDimension1;
  PD3D10_SRV_DIMENSION1=^TD3D10_SrvDimension1;

  TD3D10_ShaderResourceViewDesc1=record
    Format:TDXGI_FORMAT;
    ViewDimension:TD3D10_SrvDimension1;
    case Integer of
      0: (Buffer:D3D10_BUFFER_SRV);
      1: (Texture1D:D3D10_TEX1D_SRV);
      2: (Texture1DArray:D3D10_TEX1D_ARRAY_SRV);
      3: (Texture2D:D3D10_TEX2D_SRV);
      4: (Texture2DArray:D3D10_TEX2D_ARRAY_SRV);
      5: (Texture2DMS:D3D10_TEX2DMS_SRV);
      6: (Texture2DMSArray:D3D10_TEX2DMS_ARRAY_SRV);
      7: (Texture3D:D3D10_TEX3D_SRV);
      8: (TextureCube:D3D10_TEXCUBE_SRV);
      9: (TextureCubeArray:D3D10_TEXCUBE_ARRAY_SRV1);
  end;
  PTD3D10_ShaderResourceViewDesc1=^TD3D10_ShaderResourceViewDesc1;
  D3D10_SHADER_RESOURCE_VIEW_DESC1=TD3D10_ShaderResourceViewDesc1;
  PD3D10_SHADER_RESOURCE_VIEW_DESC1=^TD3D10_ShaderResourceViewDesc1;

  ID3D10ShaderResourceView1=interface(ID3D10ShaderResourceView)
    ['{9B7E4C87-342C-4106-A19F-4F2704F689F0}']
    procedure GetDesc1
    (
      out Desc:TD3D10_ShaderResourceViewDesc1  
    ); stdcall;
  end;

  TD3D10_StandardMultisampleQualityLevels=
  (
    D3D10_STANDARD_MULTISAMPLE_PATTERN=Integer($ffffffff),
    D3D10_CENTER_MULTISAMPLE_PATTERN=Integer($fffffffe)
  );
  PTD3D10_StandardMultisampleQualityLevels=^TD3D10_StandardMultisampleQualityLevels;
  D3D10_STANDARD_MULTISAMPLE_QUALITY_LEVELS=TD3D10_StandardMultisampleQualityLevels;
  PD3D10_STANDARD_MULTISAMPLE_QUALITY_LEVELS=^TD3D10_StandardMultisampleQualityLevels;

  ID3D10Device1=interface(ID3D10Device)
    ['{9B7E4C8F-342C-4106-A19F-4F2704F689F0}']
    function CreateShaderResourceView1
    (
      Resource:ID3D10Resource;  
      pDesc:PTD3D10_ShaderResourceViewDesc1;  
      {$IFDEF UsePointersForOptionalOutputInterfaces}pSRView:PID3D10ShaderResourceView1{$ELSE}out SRView:ID3D10ShaderResourceView1{$ENDIF}
    ):HResult; stdcall;

    function CreateBlendState1
    (
      const BlendStateDesc:TD3D10_BlendDesc1;  
      {$IFDEF UsePointersForOptionalOutputInterfaces}pBlendState:PID3D10BlendState1{$ELSE}out BlendState:ID3D10BlendState1{$ENDIF}
    ):HResult; stdcall;

    function GetFeatureLevel:TD3D10_FeatureLevel1; stdcall;
  end;

var D3D10CreateDevice1: function(Adapter:IDXGIAdapter;DriverType:TD3D10_DriverType;Software:HMODULE;Flags:LongWord;HardwareLevel:TD3D10_FeatureLevel1;SDKVersion:LongWord;out Device:ID3D10Device1):HResult; stdcall;

var D3D10CreateDeviceAndSwapChain1: function
(
  Adapter:IDXGIAdapter;
  DriverType:TD3D10_DriverType;
  Software:HMODULE;
  Flags:LongWord;
  HardwareLevel:TD3D10_FeatureLevel1;
  SDKVersion:LongWord;
  pSwapChainDesc:PTDXGI_SwapChainDesc;  
  out SwapChain:IDXGISwapChain;
  out Device:ID3D10Device1
):HResult; stdcall;  


// =============== D3D10_1Shader.h =====================

type
  TD3D10_ShaderDebugRegtype=
  (
    D3D10_SHADER_DEBUG_REG_INPUT,
    D3D10_SHADER_DEBUG_REG_OUTPUT,
    D3D10_SHADER_DEBUG_REG_CBUFFER,
    D3D10_SHADER_DEBUG_REG_TBUFFER,
    D3D10_SHADER_DEBUG_REG_TEMP,
    D3D10_SHADER_DEBUG_REG_TEMPARRAY,
    D3D10_SHADER_DEBUG_REG_TEXTURE,
    D3D10_SHADER_DEBUG_REG_SAMPLER,
    D3D10_SHADER_DEBUG_REG_IMMEDIATECBUFFER,
    D3D10_SHADER_DEBUG_REG_LITERAL,
    D3D10_SHADER_DEBUG_REG_UNUSED,
    D3D11_SHADER_DEBUG_REG_INTERFACE_POINTERS,
    D3D11_SHADER_DEBUG_REG_UAV
  );
  PTD3D10_ShaderDebugRegtype=^TD3D10_ShaderDebugRegtype;
  D3D10_SHADER_DEBUG_REGTYPE=TD3D10_ShaderDebugRegtype;
  PD3D10_SHADER_DEBUG_REGTYPE=^TD3D10_ShaderDebugRegtype;

  TD3D10_ShaderDebugScopetype=
  (
    D3D10_SHADER_DEBUG_SCOPE_GLOBAL,
    D3D10_SHADER_DEBUG_SCOPE_BLOCK,
    D3D10_SHADER_DEBUG_SCOPE_FORLOOP,
    D3D10_SHADER_DEBUG_SCOPE_STRUCT,
    D3D10_SHADER_DEBUG_SCOPE_FUNC_PARAMS,
    D3D10_SHADER_DEBUG_SCOPE_STATEBLOCK,
    D3D10_SHADER_DEBUG_SCOPE_NAMESPACE,
    D3D10_SHADER_DEBUG_SCOPE_ANNOTATION
  );
  PTD3D10_ShaderDebugScopetype=^TD3D10_ShaderDebugScopetype;
  D3D10_SHADER_DEBUG_SCOPETYPE=TD3D10_ShaderDebugScopetype;
  PD3D10_SHADER_DEBUG_SCOPETYPE=^TD3D10_ShaderDebugScopetype;

  TD3D10_ShaderDebugVartype=
  (
    D3D10_SHADER_DEBUG_VAR_VARIABLE,
    D3D10_SHADER_DEBUG_VAR_FUNCTION
  );
  PTD3D10_ShaderDebugVartype=^TD3D10_ShaderDebugVartype;
  D3D10_SHADER_DEBUG_VARTYPE=TD3D10_ShaderDebugVartype;
  PD3D10_SHADER_DEBUG_VARTYPE=^TD3D10_ShaderDebugVartype;

  TD3D10_ShaderDebugTokenInfo=record
    _File:LongWord;
    Line:LongWord;
    Column:LongWord;
    TokenLength:LongWord;
    TokenId:LongWord;
  end;
  PTD3D10_ShaderDebugTokenInfo=^TD3D10_ShaderDebugTokenInfo;
  D3D10_SHADER_DEBUG_TOKEN_INFO=TD3D10_ShaderDebugTokenInfo;
  PD3D10_SHADER_DEBUG_TOKEN_INFO=^TD3D10_ShaderDebugTokenInfo;

  TD3D10_ShaderDebugVarInfo=record
    TokenId:LongWord;
    _Type:TD3D10_ShaderVariableType;
    _Register:LongWord;
    Component:LongWord;
    ScopeVar:LongWord;
    ScopeVarOffset:LongWord;
  end;
  PTD3D10_ShaderDebugVarInfo=^TD3D10_ShaderDebugVarInfo;
  D3D10_SHADER_DEBUG_VAR_INFO=TD3D10_ShaderDebugVarInfo;
  PD3D10_SHADER_DEBUG_VAR_INFO=^TD3D10_ShaderDebugVarInfo;

  TD3D10_ShaderDebugInputInfo=record
    _Var:LongWord;
    InitialRegisterSet:TD3D10_ShaderDebugRegtype;
    InitialBank:LongWord;
    InitialRegister:LongWord;
    InitialComponent:LongWord;
    InitialValue:LongWord;
  end;
  PTD3D10_ShaderDebugInputInfo=^TD3D10_ShaderDebugInputInfo;
  D3D10_SHADER_DEBUG_INPUT_INFO=TD3D10_ShaderDebugInputInfo;
  PD3D10_SHADER_DEBUG_INPUT_INFO=^TD3D10_ShaderDebugInputInfo;

  TD3D10_ShaderDebugScopevarInfo=record
    TokenId:LongWord;
    VarType:TD3D10_ShaderDebugVartype;
    _Class:TD3D10_ShaderVariableClass;
    Rows:LongWord;
    Columns:LongWord;
    StructMemberScope:LongWord;
    ArrayIndices:LongWord;
    ArrayElements:LongWord;
    ArrayStrides:LongWord;
    Variables:LongWord;
    FirstVariable:LongWord;
  end;
  PTD3D10_ShaderDebugScopevarInfo=^TD3D10_ShaderDebugScopevarInfo;
  D3D10_SHADER_DEBUG_SCOPEVAR_INFO=TD3D10_ShaderDebugScopevarInfo;
  PD3D10_SHADER_DEBUG_SCOPEVAR_INFO=^TD3D10_ShaderDebugScopevarInfo;

  TD3D10_ShaderDebugScopeInfo=record
    ScopeType:TD3D10_ShaderDebugScopetype;
    Name:LongWord;
    NameLen:LongWord;
    Variables:LongWord;
    VariableData:LongWord;
  end;
  PTD3D10_ShaderDebugScopeInfo=^TD3D10_ShaderDebugScopeInfo;
  D3D10_SHADER_DEBUG_SCOPE_INFO=TD3D10_ShaderDebugScopeInfo;
  PD3D10_SHADER_DEBUG_SCOPE_INFO=^TD3D10_ShaderDebugScopeInfo;

  TD3D10_ShaderDebugOutputVar=record
    _Var:LongWord;
    uValueMin:LongWord;
    uValueMax:LongWord;
    iValueMin:Integer;
    iValueMax:Integer;
    fValueMin:Single;
    fValueMax:Single;
    bNaNPossible:LongBool;
    bInfPossible:LongBool;
  end;
  PTD3D10_ShaderDebugOutputVar=^TD3D10_ShaderDebugOutputVar;
  D3D10_SHADER_DEBUG_OUTPUTVAR=TD3D10_ShaderDebugOutputVar;
  PD3D10_SHADER_DEBUG_OUTPUTVAR=^TD3D10_ShaderDebugOutputVar;

  TD3D10_ShaderDebugOutputregInfo=record
    OutputRegisterSet:TD3D10_ShaderDebugRegtype;
    OutputReg:LongWord;
    TempArrayReg:LongWord;
    OutputComponents:array[0..3] of LongWord;
    OutputVars:array[0..3] of TD3D10_ShaderDebugOutputVar;
    IndexReg:LongWord;
    IndexComp:LongWord;
  end;
  PTD3D10_ShaderDebugOutputregInfo=^TD3D10_ShaderDebugOutputregInfo;
  D3D10_SHADER_DEBUG_OUTPUTREG_INFO=TD3D10_ShaderDebugOutputregInfo;
  PD3D10_SHADER_DEBUG_OUTPUTREG_INFO=^TD3D10_ShaderDebugOutputregInfo;

  TD3D10_ShaderDebugInstInfo=record
    ID:LongWord;
    Opcode:LongWord;
    NumOutputs:LongWord;
    pOutputs:array[0..1] of TD3D10_ShaderDebugOutputregInfo;
    TokenID:LongWord;
    NestingLevel:LongWord;
    Scopes:LongWord;
    ScopeInfo:LongWord;
    AccessedVars:LongWord;
    AccessedVarsInfo:LongWord;
  end;
  PTD3D10_ShaderDebugInstInfo=^TD3D10_ShaderDebugInstInfo;
  D3D10_SHADER_DEBUG_INST_INFO=TD3D10_ShaderDebugInstInfo;
  PD3D10_SHADER_DEBUG_INST_INFO=^TD3D10_ShaderDebugInstInfo;

  TD3D10_ShaderDebugFileInfo=record
    FileName:LongWord;
    FileNameLen:LongWord;
    FileData:LongWord;
    FileLen:LongWord;
  end;
  PTD3D10_ShaderDebugFileInfo=^TD3D10_ShaderDebugFileInfo;
  D3D10_SHADER_DEBUG_FILE_INFO=TD3D10_ShaderDebugFileInfo;
  PD3D10_SHADER_DEBUG_FILE_INFO=^TD3D10_ShaderDebugFileInfo;

  TD3D10_ShaderDebugInfo=record
    Size:LongWord;
    Creator:LongWord;
    EntrypointName:LongWord;
    ShaderTarget:LongWord;
    CompileFlags:LongWord;
    Files:LongWord;
    FileInfo:LongWord;
    Instructions:LongWord;
    InstructionInfo:LongWord;
    Variables:LongWord;
    VariableInfo:LongWord;
    InputVariables:LongWord;
    InputVariableInfo:LongWord;
    Tokens:LongWord;
    TokenInfo:LongWord;
    Scopes:LongWord;
    ScopeInfo:LongWord;
    ScopeVariables:LongWord;
    ScopeVariableInfo:LongWord;
    UintOffset:LongWord;
    StringOffset:LongWord;
  end;
  PTD3D10_ShaderDebugInfo=^TD3D10_ShaderDebugInfo;
  D3D10_SHADER_DEBUG_INFO=TD3D10_ShaderDebugInfo;
  PD3D10_SHADER_DEBUG_INFO=^TD3D10_ShaderDebugInfo;

  ID3D10ShaderReflection1=interface;
  PID3D10ShaderReflection1=^ID3D10ShaderReflection1;

  ID3D10ShaderReflection1=interface(IUnknown)
    ['{C3457783-A846-47CE-9520-CEA6F66E7447}']
    function GetDesc(pDesc:PTD3D10_ShaderDesc):HResult; stdcall;
    function GetConstantBufferByIndex(Index:LongWord):ID3D10ShaderReflectionConstantBuffer; stdcall;
    function GetConstantBufferByName(Name:PAnsiChar):ID3D10ShaderReflectionConstantBuffer; stdcall;
    function GetResourceBindingDesc(ResourceIndex:LongWord;pDesc:PTD3D10_ShaderInputBindDesc):HResult; stdcall;
    function GetInputParameterDesc(ParameterIndex:LongWord;pDesc:PTD3D10_SignatureParameterDesc):HResult; stdcall;
    function GetOutputParameterDesc(ParameterIndex:LongWord;pDesc:PTD3D10_SignatureParameterDesc):HResult; stdcall;
    function GetVariableByName(Name:PAnsiChar):ID3D10ShaderReflectionVariable; stdcall;
    function GetResourceBindingDescByName(Name:PAnsiChar;pDesc:PTD3D10_ShaderInputBindDesc):HResult; stdcall;
    function GetMovInstructionCount(pCount:PLongWord):HResult; stdcall;
    function GetMovcInstructionCount(pCount:PLongWord):HResult; stdcall;
    function GetConversionInstructionCount(pCount:PLongWord):HResult; stdcall;
    function GetBitwiseInstructionCount(pCount:PLongWord):HResult; stdcall;
    function GetGSInputPrimitive(pPrim:PTD3D10_Primitive):HResult; stdcall;
    function IsLevel9Shader(pValue:PLongBool):HResult; stdcall;
    function IsSampleFrequencyShader(pValue:PLongBool):HResult; stdcall;
  end;



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
  hDLL_D3D10_1:HModule;
begin
  hDLL_D3D10_1:=LoadDLL(DLL_D3D10_1);

  D3D10CreateDevice1:=LinkMethod(hDLL_D3D10_1,'D3D10CreateDevice1',DLL_D3D10_1);
  D3D10CreateDeviceAndSwapChain1:=LinkMethod(hDLL_D3D10_1,'D3D10CreateDeviceAndSwapChain1',DLL_D3D10_1);
end;

end.
