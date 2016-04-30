{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}



const
  VBSize=$FFFF * SizeOf(TD3ColorTexVertexNormal);
  IBSize=$FFFF * 2 * 2;

type

  TD3FilterGdiPlus=class(TD3Filter)
  private
  public
  published
    class function GetFileTypes:string;  override;
    class function GetImageSize(const AFileName:string):TD3Point;  override;
    function LoadFromFile(const AFileName:string; const Rotate:single; var Bitmap:TD3Bitmap):boolean;  override;
    function SaveToFile(const AFileName:string; var Bitmap:TD3Bitmap; const Params:string=''):boolean;  override;
    function LoadThumbnailFromFile(const AFileName:string; const AFitWidth,AFitHeight:single; const UseEmbedded:boolean;
                                   var Bitmap:TD3Bitmap):boolean;  override;
    function LoadFromStream(const AStream:TStream; var Bitmap:TD3Bitmap):boolean;  override;
    function SaveToStream(const AStream:TStream; var Bitmap:TD3Bitmap; const Format:string;
                           const Params:string=''):boolean;  override;
  end;

  TCharCoord=packed record
    Exists:longword;
    Coord:array [0..3] of single;
    a,c :longint;
  end;

  TD3InternalFont=record
    Family:string;
    Size:single;
    Style:TD3FontStyle;
    CharCoords:array of TCharCoord;
    Bitmap:TD3Bitmap;
  end;

type

  TD3DirectXCanvas=class(TD3Canvas)
  private
    FPresentParams:TD3DPresentParameters;
    FDev:IDirect3DDevice9;
    FFonts:array of TD3InternalFont;
    Caps:TD3DCaps9;
    VB:IDirect3DVertexBuffer9;
    IB:IDirect3DIndexBuffer9;
    VBLockPos,IBLockPos:integer;
    AllowSample,AllowSample4:boolean;
    SysMemBuf,ColorBuf,ColorBuf2,DepthBuf:IDirect3DSurface9;
    function GetPresentParameters(Wnd,DC:cardinal):TD3DPresentParameters;
    function UpdateFont(const AText:WideString; Font:TD3Font):integer;
  protected
    procedure MaterialChanged(Sender:TObject);  override;
    procedure CreateRenderStateList(AState:TD3RenderState);  override;
    procedure ApplyRenderState(AState:TD3RenderState);  override;
    procedure UpdateBitmap(Bitmap:TD3Bitmap);  override;
  public
    constructor Create(const AWnd:cardinal; const AWidth,AHeight:integer; const ALockable:boolean; const Quality:TD3Quality);  override;
    constructor CreateFromBitmap(const ABitmap:TD3Bitmap);  override;
    destructor Destroy;  override;
    procedure SetQuality(const Quality:TD3Quality);  override;

    function  BeginScene:boolean;  override;
    procedure Clear(const AColor:TD3Color);  override;
    procedure ClearARGB(const AWidth,AHeight:integer; const Bits:PD3ColorArray);  override;
    procedure FlushBuffer;  override;
    procedure FreeBuffer;  override;
    procedure ResizeBuffer(const AWidth,AHeight:integer);  override;
    procedure CopyBits(const Bits:PD3ColorArray);  override;
    procedure DrawLine(const StartPoint,EndPoint:TD3Vector; const Opacity:single);  override;
    procedure DrawRect(const StartPoint,EndPoint:TD3Vector; const Opacity:single);  override;
    procedure DrawCube(const Center,Size:TD3Vector; const Opacity:single);  override;
    procedure FillCube(const Center,Size:TD3Vector; const Opacity:single);  override;
    procedure FillMesh(const Center,Size:TD3Vector; const MeshData:TD3MeshData; const Opacity:single);  override;
    procedure FillColorTexVertexNormal(const Vertices:array of TD3ColorTexVertexNormal;
                                       const Indices:array of word; const Opacity:single);  override;
    procedure FillPolygon(const Center,Size:TD3Vector; const Rect:TD2Rect; const Points:TD2Polygon; const Opacity:single;
                          Front:boolean=true; Back:boolean=true; Left:boolean=true);  override;
    procedure DrawText(const AX,AY:single; const AText:WideString; const Opacity:single);  override;
    function  MeasureText(const AText:WideString):TD3Point;  override;
    procedure DestroyBitmap(const Bitmap:TD3Bitmap);  override;
    procedure SetMatrix(const M:TD3Matrix);  override;
    Function  IsVista:boolean;
  published
  end;

var
  Enum:IDirect3D9=nil;
  VarIsVista:boolean;


function ColorToD3DColor(const AColor:TD3Color):TD3DColorValue;
begin
  Result.r:=TD3ColorRec(AColor).R / $FF;
  Result.g:=TD3ColorRec(AColor).G / $FF;
  Result.b:=TD3ColorRec(AColor).B / $FF;
  Result.a:=TD3ColorRec(AColor).A / $FF;
end;

function makeD3DVector(const V:TD3Vector):TD3DVector;
begin
  Result.X:=V.X;
  Result.Y:=V.Y;
  Result.Z:=V.Z;
end;

procedure SAFE_RELEASE(var i);
begin
  if IUnknown(i) <> nil then
  begin
    IUnknown(i):= nil;
  end;
end;

function Checkd3Error(Res:HResult):boolean;
begin
  Result:=FAILED(Res);
end;

function CheckVistaDevice:boolean;
var
  L:THandle;
begin
  result:=false;

 {$IFDEF USE_DIRECT3D_VERSION_9_VISTA}
  L:=LoadLibrary('d3d9.dll');
  try
    Result:=GetProcAddress(L,'Direct3DCreate9Ex') <> nil;
  finally
    FreeLibrary(L);
  end;
 {$ENDIF}

end;

//------------------ TD3FilterGdiPlus ----------------------------------

class function TD3FilterGdiPlus.GetFileTypes:string;
begin
  Result:='*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff;*.gif'
end;

class function TD3FilterGdiPlus.GetImageSize(const AFileName:string):TD3Point;
begin
  with orca_scene2d.GvarD2DefaultFilterClass.GetImageSize(AFileName) do
    Result:=d3Point(X,Y,0);
end;

function TD3FilterGdiPlus.LoadFromFile(const AFileName:string; const Rotate:single; var Bitmap:TD3Bitmap):boolean;
var
  B:TD2Bitmap;
  bits:PD3ColorArray;
begin
  B:=TD2Bitmap.Create(1,1);
  try
    B.LoadFromFile(AFileName,Rotate);

    Bitmap.SetSize(B.Width,B.Height);
    if Bitmap.LockBitmapBits(bits,true) then
    begin
      Move(B.StartLine^,bits^,Bitmap.Width * Bitmap.Height * 4);
      Bitmap.UnlockBitmapBits;
    end;
  finally
    B.Free;
  end;
end;

function TD3FilterGdiPlus.LoadThumbnailFromFile(const AFileName:string;
  const AFitWidth,AFitHeight:single; const UseEmbedded:boolean; var Bitmap:TD3Bitmap):boolean;
var
  B:TD2Bitmap;
  bits:PD3ColorArray;
begin
  B:=TD2Bitmap.Create(1,1);
  try
    B.LoadThumbnailFromFile(AFileName,AFitWidth,AFitHeight,UseEmbedded);

    Bitmap.SetSize(B.Width,B.Height);
    if Bitmap.LockBitmapBits(bits,true) then
    begin
      Move(B.StartLine^,bits^,Bitmap.Width * Bitmap.Height * 4);
      Bitmap.UnlockBitmapBits;
    end;
  finally
    B.Free;
  end;
end;

function TD3FilterGdiPlus.LoadFromStream(const AStream:TStream;
  var Bitmap:TD3Bitmap):boolean;
var
  B:TD2Bitmap;
  F:TD2Filter;
  bits:PD3ColorArray;
begin
  B:=TD2Bitmap.Create(1,1);
  try
    F:=orca_scene2d.GvarD2DefaultFilterClass.Create;
    F.LoadFromStream(AStream,B);
    F.Free;

    Bitmap.SetSize(B.Width,B.Height);
    if Bitmap.LockBitmapBits(bits,true) then
    begin
      Move(B.StartLine^,bits^,Bitmap.Width * Bitmap.Height * 4);
      Bitmap.UnlockBitmapBits;
    end;
  finally
    B.Free;
  end;
end;

function TD3FilterGdiPlus.SaveToStream(const AStream:TStream;
  var Bitmap:TD3Bitmap; const Format:string;
  const Params:string=''):boolean;
var
  B:TD2Bitmap;
  F:TD2Filter;
  bits:PD3ColorArray;
begin
  B:=TD2Bitmap.Create(Bitmap.Width,Bitmap.Height);
  try
    if Bitmap.LockBitmapBits(bits,false) then
    begin
      Move(bits^,B.StartLine^,Bitmap.Width * Bitmap.Height * 4);
      Bitmap.UnlockBitmapBits;
    end;

    F:=orca_scene2d.GvarD2DefaultFilterClass.Create;
    F.SaveToStream(AStream,B,Format,Params);
    F.Free;
  finally
    B.Free;
  end;
end;

function TD3FilterGdiPlus.SaveToFile(const AFileName:string;
  var Bitmap:TD3Bitmap; const Params:string=''):boolean;
var
  B:TD2Bitmap;
  F:TD2Filter;
  bits:PD3ColorArray;
begin
  B:=TD2Bitmap.Create(Bitmap.Width,Bitmap.Height);
  try
    if Bitmap.LockBitmapBits(bits,false) then
    begin
      Move(bits^,B.StartLine^,Bitmap.Width * Bitmap.Height * 4);
      Bitmap.UnlockBitmapBits;
    end;

    F:=orca_scene2d.GvarD2DefaultFilterClass.Create;
    F.SaveToFile(AFileName,B,Params);
    F.Free;
  finally
    B.Free;
  end;
end;

//------------------ TD3DirectXCanvas --------------------------------

procedure CalcMaxBitmapSize;
var
  Caps:TD3DCAPS9;
begin

  VarIsVista:=CheckVistaDevice;

{$IFDEF USE_DIRECT3D_VERSION_9_VISTA}
  if VarIsVista then
  Direct3DCreate9Ex(D3D_SDK_VERSION,IDirect3D9Ex(Enum))
  else
{$ENDIF}
  Enum:=Direct3DCreate9(D3D_SDK_VERSION);

  if (Enum=nil) then Exit;
  Enum.GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Caps);
  cnMaxBitmapSize:=Caps.MaxTextureWidth;
end;

Function  TD3DirectXCanvas.IsVista:boolean;
begin
  result:=VarIsVista;
end;

constructor TD3DirectXCanvas.Create(const AWnd:cardinal; const AWidth,AHeight:integer; const ALockable:boolean; const Quality:TD3Quality);
var
  DC:HDC;
  Res:HResult;
  VP:Cardinal;
  Caps:TD3DCAPS9;
  BackBuffer:IDirect3DSurface9;
  i,ModeCount:integer;
  Mode,Mode1:TD3DDisplayMode;
  S:TD3RenderState;
begin
  inherited;
  { Create device }
  if (Enum=nil) then Exit;

  Enum.GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Caps);
  if (Caps.DevCaps and D3DDEVCAPS_HWTRANSFORMANDLIGHT=D3DDEVCAPS_HWTRANSFORMANDLIGHT) then
  begin
    if Caps.DevCaps and D3DDEVCAPS_PUREDEVICE=D3DDEVCAPS_PUREDEVICE then
    begin
      VP:=D3DCREATE_PUREDEVICE or D3DCREATE_HARDWARE_VERTEXPROCESSING;
    end
    else
    begin
      VP:=D3DCREATE_HARDWARE_VERTEXPROCESSING;
    end;
  end
  else
  begin
    VP:=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  end;
  VP:=VP or D3DCREATE_MULTITHREADED or D3DCREATE_FPU_PRESERVE;

  if Checkd3Error(Enum.CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,D3DFMT_A8R8G8B8,true,D3DMULTISAMPLE_2_SAMPLES,nil)) then
    AllowSample:=false
  else
    AllowSample:=true;
  if Checkd3Error(Enum.CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,D3DFMT_A8R8G8B8,true,D3DMULTISAMPLE_4_SAMPLES,nil)) then
    AllowSample4:=false
  else
    AllowSample4:=true;

  { Create device }
  DC:=GetDC(FWnd);
  FPresentParams:=GetPresentParameters(FWnd,DC);

{$IFDEF USE_DIRECT3D_VERSION_9_VISTA}
  if VarIsVista then
  Res:=IDirect3D9Ex(Enum).CreateDeviceEX(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,FWnd,VP,@FPresentParams,nil,IDirect3DDevice9Ex(FDev))
  else
{$ENDIF}
  Res:=Enum.CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,FWnd,VP,@FPresentParams,FDev);

  ReleaseDC(FWnd,DC);
  if CheckD3Error(Res) then Exit;
  { Create SysMem }
  if ALockable then
  begin
    Res:=FDev.CreateOffscreenPlainSurface(FWidth,FHeight,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,SysMemBuf,nil);
    if (Quality in [d3HighQuality,d3SuperHighQuality]) and (AllowSample or AllowSample4) then
      Res:=FDev.CreateRenderTarget(FWidth,FHeight,D3DFMT_A8R8G8B8,D3DMULTISAMPLE_NONE,0,false,ColorBuf2,nil);
  end;

  FDev.GetDeviceCaps(Caps);

  { Prepare object }
  if CheckD3Error(FDev.CreateVertexBuffer(VBSize,D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC,D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE,D3DPOOL_DEFAULT,VB,nil)) then
  begin
  end;
  if CheckD3Error(FDev.CreateIndexBuffer(IBSize,D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC,
     D3DFMT_INDEX16,D3DPOOL_DEFAULT,IB,nil)) then
  begin
  end;

  { states }
  for S:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(S);
end;

constructor TD3DirectXCanvas.CreateFromBitmap(const ABitmap:TD3Bitmap);
var
  Res:HResult;
  VP:Cardinal;
  Caps:TD3DCAPS9;
  BackBuffer:IDirect3DSurface9;
  i,ModeCount:integer;
  Mode,Mode1:TD3DDisplayMode;
  S:TD3RenderState;
begin
  inherited;
  { Create device }
  if (Enum=nil) then Exit;

  Enum.GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Caps);
  if (Caps.DevCaps and D3DDEVCAPS_HWTRANSFORMANDLIGHT=D3DDEVCAPS_HWTRANSFORMANDLIGHT) then
  begin
    if Caps.DevCaps and D3DDEVCAPS_PUREDEVICE=D3DDEVCAPS_PUREDEVICE then
    begin
      VP:=D3DCREATE_PUREDEVICE or D3DCREATE_HARDWARE_VERTEXPROCESSING;
    end
    else
    begin
      VP:=D3DCREATE_HARDWARE_VERTEXPROCESSING;
    end;
  end
  else
  begin
    VP:=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  end;
  VP:=VP or D3DCREATE_MULTITHREADED or D3DCREATE_FPU_PRESERVE;

  { Create device }
  FPresentParams:=GetPresentParameters(GetDesktopWindow,GetDC(GetDesktopWindow));

{$IFDEF USE_DIRECT3D_VERSION_9_VISTA}
  if VarIsVista then
  Res:=IDirect3D9Ex(Enum).CreateDeviceEX(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,FWnd,VP,@FPresentParams,nil,IDirect3DDevice9Ex(FDev))
  else
{$ENDIF}
  Res:=Enum.CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,FWnd,VP,@FPresentParams,FDev);


  if CheckD3Error(Res) then Exit;

  FDev.GetDeviceCaps(Caps);

  { Create RenderTarget }
  Res:=FDev.CreateRenderTarget(ABitmap.Width,ABitmap.Height,D3DFMT_A8R8G8B8,
    D3DMULTISAMPLE_NONE,0,false,ColorBuf,nil);
  if CheckD3Error(Res) then ;
  Res:=FDev.CreateDepthStencilSurface(ABitmap.Width,ABitmap.Height,D3DFMT_D24S8,
    D3DMULTISAMPLE_NONE,0,true,DepthBuf,nil);
  if CheckD3Error(Res) then ;
  CheckD3Error(FDev.SetRenderTarget(0,ColorBuf));
  CheckD3Error(FDev.SetDepthStencilSurface(DepthBuf));

  Res:=FDev.CreateOffscreenPlainSurface(ABitmap.Width,ABitmap.Height,D3DFMT_A8R8G8B8,
    D3DPOOL_SYSTEMMEM,SysMemBuf,nil);
  if CheckD3Error(Res) then ;

  { Prepare object }
  if CheckD3Error(FDev.CreateVertexBuffer(VBSize,D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC,D3DFVF_XYZ,D3DPOOL_DEFAULT,VB,nil)) then
  begin
  end;
  if CheckD3Error(FDev.CreateIndexBuffer(IBSize,D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC,
     D3DFMT_INDEX16,D3DPOOL_DEFAULT,IB,nil)) then
  begin
  end;

  { states }
  for S:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(S);
end;

procedure TD3DirectXCanvas.ResizeBuffer(const AWidth,AHeight:integer);
var
  DC:cardinal;
  i:integer;
  S:TD3RenderState;
  Res:HResult;
  Viewport:TD3DViewport9;
  Tex:IDirect3DTexture9;
begin
  inherited ;
  if Assigned(FDev) then
  begin

      if FLockable then
      begin
        SAFE_RELEASE(SysMemBuf);
        SAFE_RELEASE(ColorBuf2);
      end;
      { reset textures }
      if FBitmaps <> nil then
      begin
        for i:=0 to FHandles.Count - 1 do
        begin
          Tex:=IDirect3DTexture9(FHandles[i]);
          Tex._Release;
          SAFE_RELEASE(Tex);
        end;
        FBitmaps.Clear;
        FHandles.Clear;
      end;
      { Release}
      SAFE_RELEASE(IB);
      SAFE_RELEASE(VB);
      VBLockPos:=0;
      IBLockPos:=0;

      for S:=TD3RenderState(0) to rsGouraud do
        if FRenderStates[S] <> 0 then
          SAFE_RELEASE(IDirect3DStateBlock9(FRenderStates[S]));

      DC:=GetDC(FWnd);
      FPresentParams:=GetPresentParameters(FWnd,DC);
      ReleaseDC(FWnd,DC);
      Res:=FDev.Reset(FPresentParams);

      if FLockable then
      begin
        CheckD3Error(FDev.CreateOffscreenPlainSurface(FWidth,FHeight,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,SysMemBuf,nil));
        if (FQuality in [d3HighQuality,d3SuperHighQuality]) and (AllowSample or AllowSample4) then
          CheckD3Error(FDev.CreateRenderTarget(FWidth,FHeight,D3DFMT_A8R8G8B8,D3DMULTISAMPLE_NONE,0,false,ColorBuf2,nil));
      end;

      if CheckD3Error(FDev.CreateVertexBuffer(VBSize,D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC,
         D3DFVF_XYZ,D3DPOOL_DEFAULT,VB,nil)) then
      begin
      end;
      if CheckD3Error(FDev.CreateIndexBuffer(IBSize,D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC,
         D3DFMT_INDEX16,D3DPOOL_DEFAULT,IB,nil)) then
      begin
      end;

      for S:=TD3RenderState(0) to rsGouraud do
        CreateRenderStateList(S);

  end;
end;

procedure TD3DirectXCanvas.SetQuality(const Quality:TD3Quality);
begin
  inherited ;
  ResizeBuffer(FWidth,FHeight);
end;

procedure TD3DirectXCanvas.CopyBits(const Bits:PD3ColorArray);
var
  Res:HResult;
  Surface:TD3DLockedRect;
  Buf:IDirect3DSurface9;
  R:TRect;
begin
  if Assigned(FDev) then
  begin
    if FBitmap <> nil then
    begin
      d2MoveLongword(FBitmap.Bits,Bits,FWidth * FHeight);
    end;
    if FLockable and (FBufferDC <> 0) then
    begin
      if not CheckD3Error(FDev.GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,Buf)) then
      begin
        if (Assigned(ColorBuf2)) then
        begin
          R:=Rect(0,0,FWidth,FHeight);
          if not CheckD3Error(FDev.StretchRect(Buf,@R,ColorBuf2,@R,D3DTEXF_LINEAR)) then
          begin
            if not CheckD3Error(FDev.GetRenderTargetData(ColorBuf2,SysMemBuf)) then
            begin
              if not CheckD3Error(SysMemBuf.LockRect(Surface,nil,0)) then
              begin
                d2MoveLongword(Surface.pBits,Bits,FWidth * FHeight);
                SysMemBuf.UnlockRect;
              end;
            end;
          end;
        end
        else
        begin
          { normal }
          if not CheckD3Error(FDev.GetRenderTargetData(Buf,SysMemBuf)) then
          begin
            if not CheckD3Error(SysMemBuf.LockRect(Surface,nil,0)) then
            begin
              d2MoveLongword(Surface.pBits,Bits,FWidth * FHeight);
              SysMemBuf.UnlockRect;
            end;
          end;
        end;
      end;
    end;
    if not FLockable then
    begin
      Res:=FDev.CreateOffscreenPlainSurface(FWidth,FHeight,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,SysMemBuf,nil);
      if (FQuality in [d3HighQuality,d3SuperHighQuality]) and (AllowSample or AllowSample4) then
        Res:=FDev.CreateRenderTarget(FWidth,FHeight,D3DFMT_A8R8G8B8,D3DMULTISAMPLE_NONE,0,false,ColorBuf2,nil);
      if not CheckD3Error(FDev.GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,Buf)) then
      begin
        if (Assigned(ColorBuf2)) then
        begin
          R:=Rect(0,0,FWidth,FHeight);
          if not CheckD3Error(FDev.StretchRect(Buf,@R,ColorBuf2,@R,D3DTEXF_LINEAR)) then
          begin
            if not CheckD3Error(FDev.GetRenderTargetData(ColorBuf2,SysMemBuf)) then
            begin
              if not CheckD3Error(SysMemBuf.LockRect(Surface,nil,0)) then
              begin
                d2MoveLongword(Surface.pBits,Bits,FWidth * FHeight);
                SysMemBuf.UnlockRect;
              end;
            end;
          end;
        end
        else
        begin
          { normal }
          if not CheckD3Error(FDev.GetRenderTargetData(Buf,SysMemBuf)) then
          begin
            if not CheckD3Error(SysMemBuf.LockRect(Surface,nil,0)) then
            begin
              d2MoveLongword(Surface.pBits,Bits,FWidth * FHeight);
              SysMemBuf.UnlockRect;
            end;
          end;
        end;
      end;
      SAFE_RELEASE(SysMemBuf);
      SAFE_RELEASE(ColorBuf2);
    end;
  end;
end;

function TD3DirectXCanvas.GetPresentParameters(Wnd,DC:cardinal):TD3DPresentParameters;
begin
  FillChar(Result,SizeOf(Result),0);
  Result.hDeviceWindow:=Wnd;
  Result.Windowed:=true;
  Result.BackBufferWidth:=FWidth;
  Result.BackBufferHeight:=FHeight;
  Result.BackBufferFormat:=D3DFMT_A8R8G8B8;

  Result.BackBufferCount:=0;
  Result.EnableAutoDepthStencil:=true;
  Result.AutoDepthStencilFormat:=D3DFMT_D24S8; //D3DFMT_D16;

  Result.SwapEffect:=D3DSWAPEFFECT_DISCARD;

  Result.PresentationInterval:=D3DPRESENT_INTERVAL_IMMEDIATE; //D3DPRESENT_INTERVAL_DEFAULT;

  if FBitmap <> nil then Exit;

  if (FQuality=d3SuperHighQuality) and (AllowSample4) then
    Result.MultiSampleType:=D3DMULTISAMPLE_4_SAMPLES
  else
    if (FQuality in [d3HighQuality,d3SuperHighQuality]) and (AllowSample) then
      Result.MultiSampleType:=D3DMULTISAMPLE_2_SAMPLES;
end;

destructor TD3DirectXCanvas.Destroy;
var
  i:integer;
begin
  inherited;
  for i:=0 to High(FFonts) do
    if FFonts[i].Bitmap <> nil then
      FFonts[i].Bitmap.Free;
end;

procedure TD3DirectXCanvas.Clear(const AColor:TD3Color);
begin
  if Assigned(FDev) then
    CheckD3Error(FDev.Clear(0,nil,D3DCLEAR_ZBUFFER or D3DCLEAR_TARGET,AColor,1.0,0));
end;

procedure TD3DirectXCanvas.ClearARGB(const AWidth,AHeight:integer; const Bits:PD3ColorArray);
var
  i:integer;
  Buf:IDirect3DSurface9;
  R:TRect;
  Surface:TD3DLockedRect;
begin
  if not Assigned(FDev) then Exit;

  FDev.Clear(0,nil,D3DCLEAR_ZBUFFER,0,1.0,0);
  if Assigned(FDev) then
  begin
    if not CheckD3Error(SysMemBuf.LockRect(Surface,nil,0)) then
    begin
      d2MoveLongword(Bits,Surface.pBits,FWidth * FHeight);
      if not CheckD3Error(FDev.GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,Buf)) then
      begin
        if not CheckD3Error(FDev.GetRenderTargetData(SysMemBuf,Buf)) then
        begin
          d2MoveLongword(Bits,Surface.pBits,FWidth * FHeight);
        end;
      end;
      SysMemBuf.UnlockRect;
    end;
  end;
end;

function TD3DirectXCanvas.BeginScene:boolean;
var
  i:integer;
  Res:HResult;
  VP:TD3DViewport9;
  Surface:TD3DLockedRect;
  R:Windows.TRect;
  Light:TD3DLight9;
begin
  Result:=false;
  if not Assigned(FDev) then Exit;

  Res:=FDev.TestCooperativeLevel;

  if CheckD3Error(Res) then
  begin
    if (D3DERR_DEVICELOST=Res) then
    begin
      Exit;
    end;
    if (D3DERR_DEVICENOTRESET=Res) then
    begin
      // If we are windowed,read the desktop mode and use the same format for
      // the back buffer
      ResizeBuffer(FWidth,FHeight);

      Res:=FDev.TestCooperativeLevel;

      if CheckD3Error(Res) then Exit;
    end;
  end;
  Result:=true;
  { Render }
  if CheckD3Error(FDev.BeginScene) then Exit;
  if not Assigned(VB) then Exit;
  if not Assigned(IB) then Exit;
  { Common states }
  FDev.SetRenderState(D3DRS_STENCILENABLE,iFALSE);

  FDev.SetRenderState(D3DRS_ALPHABLENDENABLE,iTRUE);
  FDev.SetRenderState(D3DRS_ALPHATESTENABLE, iTRUE);
  FDev.SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
  FDev.SetRenderState(D3DRS_ALPHAREF,1);

  FDev.SetRenderState(D3DRS_SPECULARENABLE,iFALSE);

  FDev.SetRenderState(D3DRS_COLORVERTEX,iTRUE);
  FDev.SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);
  FDev.SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);
  FDev.SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_MATERIAL);

  FDev.SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  FDev.SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
  { Separate alpha }
  if FLockable then
  begin
    FDev.SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,iTRUE);
    FDev.SetRenderState(D3DRS_SRCBLENDALPHA,D3DBLEND_ONE);
    FDev.SetRenderState(D3DRS_DESTBLENDALPHA,D3DBLEND_INVSRCALPHA);
  end;

  FDev.SetRenderState(D3DRS_CLIPPING,iTRUE);
  FDev.SetRenderState(D3DRS_CLIPPLANEENABLE,iFALSE);

  FDev.SetRenderState(D3DRS_FOGENABLE,iFALSE);

  FDev.SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);

  FDev.SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
//  FDev.SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

  FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
//  FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

  FDev.SetRenderState(D3DRS_NORMALIZENORMALS,iTRUE);

  { Non Pow 2 }
{  FDev.SetRenderState(D3DRS_WRAP0,iFALSE);
  FDev.SetRenderState(D3DRS_WRAP1,iFALSE);}

  { default light }
  if Lighting then
  begin
    FDev.SetRenderState(D3DRS_LIGHTING,iTRUE);
    FDev.SetRenderState(D3DRS_AMBIENT,Ambient);

    for i:=0 to FCurrentLights.Count - 1 do
    begin
      ZeroMemory(@Light,SizeOf(Light));
      Light.Ambient:=ColorToD3DColor($FF696969);
      Light.Diffuse:=ColorToD3DColor($FFFFFFFF);
      Light.Specular:=ColorToD3DColor($FFFFFFFF);
      Light.Range:=100.0;
      { all }
      case TD3Light(FCurrentLights[i]).LightType of
        d3LightDirectional:
          begin
            Light._Type:=D3DLIGHT_DIRECTIONAL;
            Light.Direction:=makeD3DVector(TD3Light(FCurrentLights[i]).AbsoluteDirection);
            Light.Position:=makeD3DVector(TD3Light(FCurrentLights[i]).AbsolutePosition);
          end;
        d3LightPoint:
          begin
            Light._Type:=D3DLIGHT_POINT;
            Light.Range:=100.0;
            Light.Direction:=makeD3DVector(TD3Light(FCurrentLights[i]).AbsoluteDirection);
            Light.Position:=makeD3DVector(TD3Light(FCurrentLights[i]).AbsolutePosition);
            Light.Attenuation1:=0.2;
          end;
        d3LightSpot:
          begin
            Light._Type:=D3DLIGHT_SPOT;
            Light.Direction:=makeD3DVector(d3VectorScale(TD3Light(FCurrentLights[i]).AbsoluteDirection,1));
            Light.Position:=makeD3DVector(TD3Light(FCurrentLights[i]).AbsolutePosition);
            Light.Theta       :=0.5;
            Light.Phi         :=1.0;
            Light.Falloff     :=1.0;
            Light.Attenuation0:=1.0;
          end;
      end;
      FDev.SetLight(i,Light);
      FDev.LightEnable(i,TRUE);
    end;
  end
  else
  begin
    FDev.SetRenderState(D3DRS_LIGHTING,iFALSE);
  end;

  SetRenderState(rs3DScene);
end;

procedure TD3DirectXCanvas.FlushBuffer;
var
  Res:HResult;
  Surface:TD3DLockedRect;
  Buf:IDirect3DSurface9;
  R:TRect;
begin
  if Assigned(FDev) then
  begin
    if not CheckD3Error(FDev.TestCooperativeLevel) then
    begin
      FDev.EndScene;
      if FBitmap=nil then
      begin
        FDev.Present(nil,nil,0,nil);
      end;
    end;
    if FBitmap <> nil then
    begin
      if not Checkd3Error(FDev.GetRenderTargetData(ColorBuf,SysMemBuf)) then
      begin
        if not Checkd3Error(SysMemBuf.LockRect(Surface,nil,0)) then
        begin
          d2MoveLongword(Surface.pBits,FBitmap.Bits,FBitmap.Width * FBitmap.Height);
          SysMemBuf.UnlockRect;
        end
      end
    end;
    if FLockable and (FBufferDC <> 0) then
    begin
      if not Checkd3Error(FDev.GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,Buf)) then
      begin
        if (Assigned(ColorBuf2)) then
        begin
          R:=Rect(0,0,FWidth,FHeight);
          if not Checkd3Error(FDev.StretchRect(Buf,@R,ColorBuf2,@R,D3DTEXF_LINEAR)) then
          begin
            if not Checkd3Error(FDev.GetRenderTargetData(ColorBuf2,SysMemBuf)) then
            begin
              if not Checkd3Error(SysMemBuf.LockRect(Surface,nil,0)) then
              begin
                d2MoveLongword(Surface.pBits,FBufferBits,FWidth * FHeight);
                SysMemBuf.UnlockRect;
              end;
            end;
          end;
        end
        else
        begin
          { normal }
          if not Checkd3Error(FDev.GetRenderTargetData(Buf,SysMemBuf)) then
          begin
            if not Checkd3Error(SysMemBuf.LockRect(Surface,nil,0)) then
            begin
              d2MoveLongword(Surface.pBits,FBufferBits,FWidth * FHeight);
              SysMemBuf.UnlockRect;
            end;
          end;
        end;
      end;
    end;
  end;
end;

procedure TD3DirectXCanvas.CreateRenderStateList(AState:TD3RenderState);
var
  i:integer;
  matProj,scaleMatrix,transMatrix,orthoProj:TD3Matrix;
begin
  if FDev=nil then Exit;

  if FRenderStates[AState] <> 0 then
     SAFE_RELEASE(IDirect3DStateBlock9(FRenderStates[AState]));
  FDev.BeginStateBlock;
  { States }
  case AState of
    rs2DScene:
      begin
        FDev.SetTransform(D3DTS_PROJECTION,TD3DMatrix(GetScreenMatrix));
        FDev.SetTransform(D3DTS_WORLD,TD3DMatrix(IdentityMatrix));
        FDev.SetTransform(D3DTS_VIEW,TD3DMatrix(IdentityMatrix));
      end;
    rs3DScene:
      begin
        { End Applet Scale }
        FDev.SetTransform(D3DTS_PROJECTION,TD3DMatrix(GetProjectionMatrix));
        FDev.SetTransform(D3DTS_WORLD,TD3DMatrix(IdentityMatrix));
        FDev.SetTransform(D3DTS_VIEW,TD3DMatrix(IdentityMatrix));
      end;
    rsLightOn:
      begin
        FDev.SetRenderState(D3DRS_LIGHTING,iTRUE);
      end;
    rsLightOff:
      begin
        FDev.SetRenderState(D3DRS_LIGHTING,iFALSE);
      end;
    rsZTestOn:
      begin
        FDev.SetRenderState(D3DRS_ZENABLE,iTRUE);
        FDev.SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
      end;
    rsZTestOff:
      begin
        FDev.SetRenderState(D3DRS_ZENABLE,iFALSE);
      end;
    rsZWriteOn:
      begin
        FDev.SetRenderState(D3DRS_ZWRITEENABLE,iTRUE);
      end;
    rsZWriteOff:
      begin
        FDev.SetRenderState(D3DRS_ZWRITEENABLE,iFALSE);
      end;
    rsFrontFace:
      begin
        FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
      end;
    rsBackFace:
      begin
        FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
      end;
    rsAllFace:
      begin
        FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
      end;
    rsBlendAdditive:
      begin
        FDev.SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
        FDev.SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
      end;
    rsBlendNormal:
      begin
        FDev.SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
        FDev.SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
      end;
    { Tex stretch }
    rsTexNearest:
      begin
        FDev.SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
        FDev.SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
        FDev.SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
      end;
    rsTexLinear:
      begin
        FDev.SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
        FDev.SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
        FDev.SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
      end;
    { Tex modulation }
    rsTexReplace:
      begin
        FDev.SetTextureStageState(0,D3DTSS_COLOROP,  D3DTOP_SELECTARG1);
        FDev.SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
        FDev.SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
      end;
    rsTexModulate:
      begin
        FDev.SetTextureStageState(0,D3DTSS_COLOROP,  D3DTOP_MODULATE);
        FDev.SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
        FDev.SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
      end;
    rsTexDisable:
      begin
        FDev.SetTextureStageState(0,D3DTSS_COLOROP,  D3DTOP_SELECTARG2);
        FDev.SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_DIFFUSE);
        FDev.SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAOP,  D3DTOP_SELECTARG2);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
        FDev.SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
      end;
    rsFrame:
      begin
        FDev.SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
      end;
    rsSolid:
      begin
        FDev.SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
      end;
    rsFlat:FDev.SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);
    rsGouraud:FDev.SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
  end;
  FDev.EndStateBlock(IDirect3DStateBlock9(FRenderStates[AState]));
end;

procedure TD3DirectXCanvas.ApplyRenderState(AState:TD3RenderState);
var
  M:TD3Matrix;
begin
  if FDev=nil then Exit;

  if FRenderStates[AState] <> 0 then
    IDirect3DStateBlock9(FRenderStates[AState]).Apply;
  case AState of
    rs2DScene:
      begin
        FDev.SetTransform(D3DTS_VIEW,TD3DMatrix(IdentityMatrix));
      end;
    rs3DScene:
      begin
        FDev.SetTransform(D3DTS_VIEW,TD3DMatrix(FCurrentCameraMatrix));
      end;
  end;
end;

procedure FreeTex(AInfo,AItem,AData:Pointer; out AContinue:boolean);
begin
  TList(AInfo).Add(AItem);
end;

procedure TD3DirectXCanvas.FreeBuffer;
var
  S:TD3RenderState;
begin
  { states }
  for S:=TD3RenderState(0) to rsGouraud do
    if FRenderStates[S] <> 0 then
      SAFE_RELEASE(IDirect3DStateBlock9(FRenderStates[S]));
  SAFE_RELEASE(SysMemBuf);
  SAFE_RELEASE(ColorBuf2);
  SAFE_RELEASE(ColorBuf);
  SAFE_RELEASE(DepthBuf);
  SAFE_RELEASE(IB);
  SAFE_RELEASE(VB);
  VBLockPos:=0;
  IBLockPos:=0;
  SAFE_RELEASE(FDev);
  inherited;
end;

procedure TD3DirectXCanvas.MaterialChanged(Sender:TObject);
var
  Material:TD3DMATERIAL9;
  M:TD3Matrix;
begin
  inherited;
  if not Assigned(FDev) then Exit;
  if Self.Material <> nil then
  begin
    Material.Ambient:=ColorToD3dColor(Self.Material.NativeDiffuse);
    Material.Diffuse:=ColorToD3dColor(Self.Material.NativeDiffuse);
    Material.Specular:=ColorToD3DColor($FFFFFFFF);
    Material.Emissive:=ColorToD3DColor(0);
    Material.Power:=0;
    FDev.SetMaterial(Material);
    UpdateBitmap(Self.Material.NativeBitmap);
    if (Self.Material.NativeBitmap <> nil) and (GetHandle(Self.Material.NativeBitmap) <> 0) then
    begin
      { texture matrix }
      if (Self.Material.BitmapTileX <> 1) or (Self.Material.BitmapTileY <> 1) or (not Self.Material.BitmapRect.empty) then
      begin
        FDev.SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
        M:=IdentityMatrix;
        M.m11:=Self.Material.BitmapTileX;
        M.m22:=Self.Material.BitmapTileY;
        if not Self.Material.BitmapRect.empty then
        begin
          M.m11:=(Self.Material.BitmapRect.Right - Self.Material.BitmapRect.Left) / Self.Material.NativeBitmap.Width;
          M.m22:=(Self.Material.BitmapRect.Bottom - Self.Material.BitmapRect.Top) / Self.Material.NativeBitmap.Height;
          M.m31:=Self.Material.BitmapRect.Left / Self.Material.NativeBitmap.Width;
          M.m32:=Self.Material.BitmapRect.Top / Self.Material.NativeBitmap.Height;
        end;
        FDev.SetTransform(D3DTS_TEXTURE0,TD3DMatrix(M));
      end
      else
        FDev.SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
      { }

      if Self.Material.BitmapMode=d3TexReplace then
        SetRenderState(rsTexReplace)
      else
        SetRenderState(rsTexModulate);
      FDev.SetTexture(0,IDirect3DTexture9(GetHandle(Self.Material.NativeBitmap)));
    end
    else
    begin
      FDev.SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
      SetRenderState(rsTexDisable);
    end;

    if Self.Material.ShadeMode=d3Flat then
      SetRenderState(rsFlat)
    else
      SetRenderState(rsGouraud);

    if Self.Material.FillMode=d3Solid then
      SetRenderState(rsSolid)
    else
      SetRenderState(rsFrame);

    if Self.Material.Lighting then
      SetRenderState(rsLightOn)
    else
      SetRenderState(rsLightOff);
  end;
end;

procedure TD3DirectXCanvas.SetMatrix(const M:TD3Matrix);
begin
  FCurrentMatrix:=M;
  if Assigned(FDev) then
    FDev.SetTransform(D3DTS_WORLD,TD3DMatrix(FCurrentMatrix));
end;

procedure TD3DirectXCanvas.DrawLine(const StartPoint,EndPoint:TD3Vector; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormal;
  Id3:^word;
  i:integer;
  Flags:cardinal;
begin
  if FDev=nil then Exit;
  if not Assigned(VB) then Exit;
  if not Assigned(IB) then Exit;

  if VBLockPos + (2 * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
  begin
    VBLockPos:=0;
    Flags:=D3DLOCK_DISCARD;
  end
  else
  begin
    Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};;;
  end;
  if not Checkd3Error(VB.Lock(VBLockPos,2 * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
  begin
    { front }
    Ver^:=ColorTexVertexNormal(StartPoint.X,StartPoint.Y,StartPoint.Z,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),0,0); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(EndPoint.X,EndPoint.Y,EndPoint.Z,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),1,1); System.Inc(Ver);
    { indexs }
    if IBLockPos + (2 * 2) > IBSize then
    begin
      IBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};;;
    end;
    if not Checkd3Error(IB.Lock(IBLockPos,2 * 2,Pointer(Id3),Flags)) then
    begin
      for i:=0 to 0 do
      begin
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 2) + 0; System.Inc(Id3);
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 2) + 1; System.Inc(Id3);
      end;
      IB.Unlock;
    end;
    { unlock }
    VB.Unlock;

//    //FDev.SetPixelShader(0);
    FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
    FDev.SetIndices(IB);
    FDev.SetStreamSource(0,VB,0,SizeOf(TD3ColorTexVertexNormal));
    FDev.DrawIndexedPrimitive(D3DPT_LINELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
      2,IBLockPos div 2,1);
    VBLockPos:=VBLockPos + (2 * SizeOf(TD3ColorTexVertexNormal));
    IBLockPos:=IBLockPos + (2 * 2);
  end;
end;

procedure TD3DirectXCanvas.DrawRect(const StartPoint,EndPoint:TD3Vector;
  const Opacity:single);
var
  Ver:PD3ColorTexVertexNormal;
  Id3:^word;
  i:integer;
  Flags:cardinal;
begin
end;

procedure TD3DirectXCanvas.DrawCube(const Center,Size:TD3Vector; const Opacity:single);
var
  i:integer;
  A,B:TD3Vector;
  Pts:array [0..24] of TD3Vector;
begin
  A:=d3VectorAdd(Center,d3VectorScale(Size,-0.5));
  B:=d3VectorAdd(Center,d3VectorScale(Size,0.5));
  begin
    Pts[0]:=d3Vector(A.X,A.Y,B.Z);
    Pts[1]:=d3Vector(B.X,A.Y,B.Z);
    Pts[2]:=d3Vector(A.X,A.Y,A.Z);
    Pts[3]:=d3Vector(B.X,A.Y,A.Z);
    Pts[4]:=d3Vector(A.X,B.Y,B.Z);
    Pts[5]:=d3Vector(B.X,B.Y,B.Z);
    Pts[6]:=d3Vector(A.X,B.Y,A.Z);
    Pts[7]:=d3Vector(B.X,B.Y,A.Z);

    Pts[8]:=d3Vector(A.X,A.Y,A.Z);
    Pts[9]:=d3Vector(A.X,B.Y,A.Z);
    Pts[10]:=d3Vector(A.X,A.Y,B.Z);
    Pts[11]:=d3Vector(A.X,B.Y,B.Z);
    Pts[12]:=d3Vector(B.X,A.Y,A.Z);
    Pts[13]:=d3Vector(B.X,B.Y,A.Z);
    Pts[14]:=d3Vector(B.X,A.Y,B.Z);
    Pts[15]:=d3Vector(B.X,B.Y,B.Z);

    Pts[16]:=d3Vector(A.X,A.Y,A.Z);
    Pts[17]:=d3Vector(A.X,A.Y,B.Z);
    Pts[18]:=d3Vector(B.X,A.Y,A.Z);
    Pts[19]:=d3Vector(B.X,A.Y,B.Z);
    Pts[20]:=d3Vector(A.X,B.Y,A.Z);
    Pts[21]:=d3Vector(A.X,B.Y,B.Z);
    Pts[22]:=d3Vector(B.X,B.Y,A.Z);
    Pts[23]:=d3Vector(B.X,B.Y,B.Z);
  end;
  for i:=0 to 11 do
    DrawLine(Pts[i * 2],Pts[i * 2 + 1],Opacity);
end;

procedure TD3DirectXCanvas.FillCube(const Center,Size:TD3Vector; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormal;
  Id3:^word;
  tx1,ty1,tx2,ty2:single;
  A,B,n:TD3Vector;
  i:integer;
  Flags:cardinal;
begin
  if FDev=nil then Exit;
  if not Assigned(VB) then Exit;
  if not Assigned(IB) then Exit;

  if VBLockPos + (24 * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
  begin
    VBLockPos:=0;
    Flags:=D3DLOCK_DISCARD;
  end
  else
  begin
    Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};;;
  end;

  if not Checkd3Error(VB.Lock(VBLockPos,24 * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
  begin
    A:=d3VectorAdd(Center,d3VectorScale(Size,-0.5));
    B:=d3VectorAdd(Center,d3VectorScale(Size,0.5));
    tx1:=0;
    ty1:=0;
    tx2:=1;
    ty2:=1;
    { front }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,A.Y,B.Z),d3Vector(B.X,A.Y,B.Z)),
      d3VectorSubtract(d3Vector(A.X,A.Y,B.Z),d3Vector(B.X,A.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver^:=ColorTexVertexNormal(A.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(Ver);
    { right }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(B.X,A.Y,B.Z),d3Vector(B.X,B.Y,B.Z)),
      d3VectorSubtract(d3Vector(B.X,A.Y,B.Z),d3Vector(B.X,B.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver^:=ColorTexVertexNormal(B.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(Ver);
    { left }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(A.X,A.Y,B.Z)),
      d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(A.X,A.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver^:=ColorTexVertexNormal(A.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(Ver);
    { back }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(B.X,B.Y,B.Z),d3Vector(A.X,B.Y,B.Z)),
      d3VectorSubtract(d3Vector(B.X,B.Y,B.Z),d3Vector(A.X,B.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver^:=ColorTexVertexNormal(B.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(Ver);
    { top }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(B.X,B.Y,B.Z)),
      d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(B.X,A.Y,B.Z)));
    n:=d3VectorScale(n,-1);
    Ver^:=ColorTexVertexNormal(A.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(Ver);
    { bottom }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,A.Y,A.Z),d3Vector(B.X,A.Y,A.Z)),
      d3VectorSubtract(d3Vector(A.X,A.Y,A.Z),d3Vector(B.X,B.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver^:=ColorTexVertexNormal(A.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(B.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(Ver);
    Ver^:=ColorTexVertexNormal(A.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(Ver);

    { indexs }
    if IBLockPos + (36 * 2) > IBSize then
    begin
      IBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};;;
    end;
    if not Checkd3Error(IB.Lock(IBLockPos,36 * 2,Pointer(Id3),Flags)) then
    begin
      for i:=0 to 5 do
      begin
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} (i * 4) + 0; System.Inc(Id3);
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} (i * 4) + 1; System.Inc(Id3);
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} (i * 4) + 3; System.Inc(Id3);
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} (i * 4) + 3; System.Inc(Id3);
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} (i * 4) + 1; System.Inc(Id3);
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} (i * 4) + 2; System.Inc(Id3);
      end;
      IB.Unlock;
    end;
    { unlock }
    VB.Unlock;

    //FDev.SetPixelShader(0);
    FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
    FDev.SetIndices(IB);
    FDev.SetStreamSource(0,VB,0,SizeOf(TD3ColorTexVertexNormal));
    FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
      24,IBLockPos div 2,36 div 3);
    VBLockPos:=VBLockPos + (24 * SizeOf(TD3ColorTexVertexNormal));
    IBLockPos:=IBLockPos + (36 * 2);
  end;
end;

procedure TD3DirectXCanvas.FillMesh(const Center,Size:TD3Vector;
  const MeshData:TD3MeshData; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormal;
  Id3:^word;
  A,B,n:TD3Vector;
  C:TD3Color;
  i:integer;
  Flags:cardinal;
begin
  if FDev=nil then Exit;
  if not Assigned(VB) then Exit;
  if not Assigned(IB) then Exit;
  if Length(MeshData.MeshVertices)=0 then Exit;
  if Length(MeshData.MeshIndices)=0 then Exit;

  if Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal) > VBSize then Exit;
  if (Length(MeshData.MeshIndices) * 2) > IBSize then Exit;

  if VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
  begin
    VBLockPos:=0;
    Flags:=D3DLOCK_DISCARD;
  end
  else
  begin
    Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
  end;

  if not Checkd3Error(VB.Lock(VBLockPos,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
  begin
    C:=d3Opacity(Material.NativeDiffuse,Opacity);
    for i:=0 to Length(MeshData.MeshVertices) - 1 do
      with MeshData.MeshVertices[i] do
      begin
        if FCurrentStates[rs2DScene] then
          Ver^:=ColorTexVertexNormal(trunc(x * Size.X + Center.X) - 0.5,trunc(y * Size.Y + Center.Y) - 0.5,z * Size.Z + Center.Z,nx,ny,nz,C,tu,tv)
        else
          Ver^:=ColorTexVertexNormal(x * Size.X + Center.X,y * Size.Y + Center.Y,z * Size.Z + Center.Z,nx,ny,nz,C,tu,tv);
        System.Inc(Ver);
      end;
    { indexs }
    if IBLockPos + (Length(MeshData.MeshIndices) * 2) > IBSize then
    begin
      IBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
    end;
    if not Checkd3Error(IB.Lock(IBLockPos,Length(MeshData.MeshIndices) * 2,Pointer(Id3),Flags)) then
    begin
      for i:=0 to Length(MeshData.MeshIndices) - 1 do
      begin
        Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }MeshData.MeshIndices[i];
        System.Inc(Id3);
      end;
      IB.Unlock;
    end;
    { unlock }
    VB.Unlock;

    //FDev.SetPixelShader(0);
    FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
    FDev.SetIndices(IB);
    FDev.SetStreamSource(0,VB,0{VBLockPos},SizeOf(TD3ColorTexVertexNormal));
    FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,VBLockPos div SizeOf(TD3ColorTexVertexNormal),0,
      Length(MeshData.MeshVertices),IBLockPos div 2,Length(MeshData.MeshIndices) div 3);

    VBLockPos:=VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
    IBLockPos:=IBLockPos + (Length(MeshData.MeshIndices) * 2);
  end;
end;

procedure TD3DirectXCanvas.FillColorTexVertexNormal(const Vertices:array of TD3ColorTexVertexNormal;
   const Indices:array of word; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormal;
  Id3:^word;
  A,B,n:TD3Vector;
  i:integer;
  Flags:cardinal;
begin
  if FDev=nil then Exit;
  if not Assigned(VB) then Exit;
  if not Assigned(IB) then Exit;

  if Length(Vertices) * SizeOf(TD3ColorTexVertexNormal) > VBSize then Exit;
  if (Length(Indices) * 2) > IBSize then Exit;

  if VBLockPos + (Length(Vertices) * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
  begin
    VBLockPos:=0;
    Flags:=D3DLOCK_DISCARD;
  end
  else
  begin
    Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
  end;

  if not Checkd3Error(VB.Lock(VBLockPos,Length(Vertices) * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
  begin
    Move(Vertices[0],Ver^,Length(Vertices) * SizeOf(TD3ColorTexVertexNormal));
    { indexs }
    if IBLockPos + (Length(Indices) * 2) > IBSize then
    begin
      IBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
    end;
    if not Checkd3Error(IB.Lock(IBLockPos,Length(Indices) * 2,Pointer(Id3),Flags)) then
    begin
      Move(Indices[0],Id3^,Length(Indices) * 2);
      IB.Unlock;
    end;
    { unlock }
    VB.Unlock;

    //FDev.SetPixelShader(0);
    FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
    FDev.SetIndices(IB);
    FDev.SetStreamSource(0,VB,0{VBLockPos},SizeOf(TD3ColorTexVertexNormal));
    FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,VBLockPos div SizeOf(TD3ColorTexVertexNormal),0,
      Length(Vertices),IBLockPos div 2,Length(Indices) div 3);

    VBLockPos:=VBLockPos + (Length(Vertices) * SizeOf(TD3ColorTexVertexNormal));
    IBLockPos:=IBLockPos + (Length(Indices) * 2);
  end;
end;

procedure TD3DirectXCanvas.FillPolygon(const Center,Size:TD3Vector;
  const Rect:TD2Rect; const Points:TD2Polygon; const Opacity:single;
  Front:boolean=true; Back:boolean=true; Left:boolean=true);
var
  Ver:PD3ColorTexVertexNormal;
  Id3:^word;
  A,B,n,V1,V2:TD3Vector;
  C:TD3Color;
  i,j:integer;
  Flags:cardinal;
  MeshData:TD3MeshData;
  startIndex:integer;
  leftLen,curPos:single;
begin
  if FDev=nil then Exit;
  if not Assigned(VB) then Exit;
  if not Assigned(IB) then Exit;
  if Length(Points)=0 then Exit;

  { calc bounds }
  A:=d3Vector($FFFF,$FFFF,0);
  B:=d3Vector(-$FFFF,-$FFFF,0);
  leftLen:=0;
  for i:=0 to High(Points) do
  begin
    if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then continue;
    with Points[i] do
    begin
      if x < A.X then A.X:=x;
      if y < A.Y then A.Y:=y;
      if x > B.X then B.X:=x;
      if y > B.Y then B.Y:=y;
      if Left and (i > 0) then
      begin
        if Points[i - 1].X >= $FFFF then
        begin
          leftLen:=leftLen + d2VectorLength(d2Vector(x - Points[i - 2].x,y - Points[i - 2].y));
        end
        else
        begin
          leftLen:=leftLen + d2VectorLength(d2Vector(x - Points[i - 1].x,y - Points[i - 1].y));
        end;
      end;
    end;
  end;
  if not d2IsRectEmpty(Rect) then
  begin
    if Rect.Left < A.X then A.X:=Rect.Left;
    if Rect.Top < A.Y then A.Y:=Rect.Top;
    if Rect.Right > B.X then B.X:=Rect.Right;
    if Rect.Bottom > B.Y then B.Y:=Rect.bottom;
  end;

  MeshData:=TD3MeshData.Create;

  { 2. front }
  if Front then
  begin
    SetLength(MeshData.MeshVertices,Length(Points));
    { set vertices }
    for i:=0 to High(Points) do
    begin
      if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then
        continue;
      with MeshData.MeshVertices[i] do
      begin
        x:=(Points[i].x - A.X) / Abs(B.X - A.X);
        y:=(Points[i].y - A.Y) / Abs(B.Y - A.Y);
        z:=1;
        nx:=0;
        ny:=0;
        nz:=1;
        tu:=0.02 + (x * 0.96);
        tv:=0.02 + (y * 0.96);
      end;
    end;
    { Set indices }
    SetLength(MeshData.MeshIndices,(High(Points)) * 3);
    startIndex:=0;
    j:=0;
    for i:=0 to High(Points) - 1 do
    begin
      if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then
      begin
        startIndex:=i + 1;
        continue;
      end;
      MeshData.MeshIndices[(j * 3) + 0]:=startIndex;
      MeshData.MeshIndices[(j * 3) + 1]:=i + 1;
      MeshData.MeshIndices[(j * 3) + 2]:=i;
      Inc(j);
    end;
    SetLength(MeshData.MeshIndices,(j - 1) * 6);

    if Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal) > VBSize then Exit;
    if (Length(MeshData.MeshIndices) * 2) > IBSize then Exit;

    if VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
    begin
      VBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
    end;

    if not Checkd3Error(VB.Lock(VBLockPos,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
    begin
      C:=d3Opacity(Material.NativeDiffuse,Opacity);
      for i:=0 to Length(MeshData.MeshVertices) - 1 do
        with MeshData.MeshVertices[i] do
        begin
          if FCurrentStates[rs2DScene] then
            Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(x * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(y * Size.Y) + 0.5,Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv)
          else
            Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + (x * Size.X),Center.Y + (Size.Y / 2) - (y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv);
          System.Inc(Ver);
        end;
      { indexs }
      if IBLockPos + (Length(MeshData.MeshIndices) * 2) > IBSize then
      begin
        IBLockPos:=0;
        Flags:=D3DLOCK_DISCARD;
      end
      else
      begin
        Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
      end;
      if not Checkd3Error(IB.Lock(IBLockPos,Length(MeshData.MeshIndices) * 2,Pointer(Id3),Flags)) then
      begin
        for i:=0 to Length(MeshData.MeshIndices) - 1 do
        begin
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }MeshData.MeshIndices[i]; System.Inc(Id3);
        end;
        IB.Unlock;
      end;
      { unlock }
      VB.Unlock;

      //FDev.SetPixelShader(0);
      FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
      FDev.SetIndices(IB);
      FDev.SetStreamSource(0,VB,0,SizeOf(TD3ColorTexVertexNormal));
      { write to stencil }
      FDev.SetRenderState(D3DRS_STENCILENABLE,iTRUE);
      FDev.Clear(0,nil,D3DCLEAR_STENCIL,0,0,0);
      FDev.SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
      FDev.SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_INVERT);
      FDev.SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
      FDev.SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
      FDev.SetRenderState(D3DRS_STENCILMASK,$FFFFFFFF);

      FDEv.SetRenderState(D3DRS_COLORWRITEENABLE,0);
      FDEv.SetRenderState(D3DRS_ZWRITEENABLE,iFALSE);
//      FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
      FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
        Length(MeshData.MeshVertices),IBLockPos div 2,Length(MeshData.MeshIndices) div 3);
//      FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
      FDEv.SetRenderState(D3DRS_COLORWRITEENABLE,$FFFFFFFF);

      VBLockPos:=VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
      IBLockPos:=IBLockPos + (Length(MeshData.MeshIndices) * 2);
      { just paint rect using stencil }
      if VBLockPos + (4 * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
      begin
        VBLockPos:=0;
        Flags:=D3DLOCK_DISCARD;
      end
      else
      begin
        Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
      end;
      if not Checkd3Error(VB.Lock(VBLockPos,4 * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
      begin
        if FCurrentStates[rs2DScene] then
        begin
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,0); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,0); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,1); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,1); System.Inc(Ver);
        end
        else
        begin
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,0,1); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,1,1); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,1,0); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,0,0); System.Inc(Ver);
        end;
        { indexs }
        if IBLockPos + (6 * 2) > IBSize then
        begin
          IBLockPos:=0;
          Flags:=D3DLOCK_DISCARD;
        end
        else
        begin
          Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
        end;
        if not Checkd3Error(IB.Lock(IBLockPos,6 * 2,Pointer(Id3),Flags)) then
        begin
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 0; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 3; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 1; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 1; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 3; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 2; System.Inc(Id3);
          IB.Unlock;
        end;
        { unlock }
        VB.Unlock;
        FDev.SetRenderState(D3DRS_STENCILFUNC,D3DCMP_NOTEQUAL);
        FDev.SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
        FDEv.SetRenderState(D3DRS_ZWRITEENABLE,iTRUE);
        FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
          4,IBLockPos div 2,6 div 3);
        FDev.SetRenderState(D3DRS_STENCILENABLE,iFALSE);

        VBLockPos:=VBLockPos + (4 * SizeOf(TD3ColorTexVertexNormal));
        IBLockPos:=IBLockPos + (6 * 2);
      end;
    end;
  end;

  { 3. back }
  if Back then
  begin
    SetLength(MeshData.MeshVertices,Length(Points));
    { set vertices }
    for i:=0 to High(Points) do
    begin
      if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then continue;
      with MeshData.MeshVertices[i] do
      begin
        x:=(Points[i].x - A.X) / Abs(B.X - A.X);
        y:=(Points[i].y - A.Y) / Abs(B.Y - A.Y);
        z:=0;
        tu:=x;
        tv:=y;
      end;
    end;
    { Set indices }
    SetLength(MeshData.MeshIndices,(High(Points)) * 3);
    startIndex:=0;
    j:=0;
    for i:=0 to High(Points) - 1 do
    begin
      if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then
      begin
        startIndex:=i + 1;
        continue;
      end;
      MeshData.MeshIndices[(j * 3) + 0]:=startIndex;
      MeshData.MeshIndices[(j * 3) + 1]:=i + 1;
      MeshData.MeshIndices[(j * 3) + 2]:=i;
      Inc(j);
    end;
    SetLength(MeshData.MeshIndices,(j - 1) * 6);

    if Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal) > VBSize then Exit;
    if (Length(MeshData.MeshIndices) * 2) > IBSize then Exit;

    if VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
    begin
      VBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
    end;

    if not Checkd3Error(VB.Lock(VBLockPos,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
    begin
      C:=d3Opacity(Material.NativeDiffuse,Opacity);
      for i:=0 to Length(MeshData.MeshVertices) - 1 do
        with MeshData.MeshVertices[i] do
        begin
          if FCurrentStates[rs2DScene] then
            Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(x * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(y * Size.Y) + 0.5,Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv)
          else
            Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + (x * Size.X),Center.Y + (Size.Y / 2) - (y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv);
          System.Inc(Ver);
        end;
      { indexs }
      if IBLockPos + (Length(MeshData.MeshIndices) * 2) > IBSize then
      begin
        IBLockPos:=0;
        Flags:=D3DLOCK_DISCARD;
      end
      else
      begin
        Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
      end;
      if not Checkd3Error(IB.Lock(IBLockPos,Length(MeshData.MeshIndices) * 2,Pointer(Id3),Flags)) then
      begin
        for i:=0 to Length(MeshData.MeshIndices) - 1 do
        begin
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} MeshData.MeshIndices[i]; System.Inc(Id3);
        end;
        IB.Unlock;
      end;
      { unlock }
      VB.Unlock;

      //FDev.SetPixelShader(0);
      FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
      FDev.SetIndices(IB);
      FDev.SetStreamSource(0,VB,0,SizeOf(TD3ColorTexVertexNormal));
      { write to stencil }
      FDev.SetRenderState(D3DRS_STENCILENABLE,iTRUE);
      FDev.Clear(0,nil,D3DCLEAR_STENCIL,0,0,0);
      FDev.SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
      FDev.SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_INVERT);
      FDev.SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
      FDev.SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
      FDev.SetRenderState(D3DRS_STENCILMASK,$FFFFFFFF);

      FDEv.SetRenderState(D3DRS_COLORWRITEENABLE,0);
      FDEv.SetRenderState(D3DRS_ZWRITEENABLE,iFALSE);
//      FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
      FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
        Length(MeshData.MeshVertices),IBLockPos div 2,Length(MeshData.MeshIndices) div 3);
//      FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
      FDEv.SetRenderState(D3DRS_COLORWRITEENABLE,$FFFFFFFF);

      VBLockPos:=VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
      IBLockPos:=IBLockPos + (Length(MeshData.MeshIndices) * 2);
      { just paint rect using stencil }
      if VBLockPos + (4 * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
      begin
        VBLockPos:=0;
        Flags:=D3DLOCK_DISCARD;
      end
      else
      begin
        Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
      end;
      if not Checkd3Error(VB.Lock(VBLockPos,4 * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
      begin
        if FCurrentStates[rs2DScene] then
        begin
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,0); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,0); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,1); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,1); System.Inc(Ver);
        end
        else
        begin
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,0,1); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,1,1); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,1,0); System.Inc(Ver);
          Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,0,0); System.Inc(Ver);
        end;
        { indexs }
        if IBLockPos + (6 * 2) > IBSize then
        begin
          IBLockPos:=0;
          Flags:=D3DLOCK_DISCARD;
        end
        else
        begin
          Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
        end;
        if not Checkd3Error(IB.Lock(IBLockPos,6 * 2,Pointer(Id3),Flags)) then
        begin
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 0; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 1; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 3; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 1; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 2; System.Inc(Id3);
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) +} 3; System.Inc(Id3);
          IB.Unlock;
        end;
        { unlock }
        VB.Unlock;
        FDev.SetRenderState(D3DRS_STENCILFUNC,D3DCMP_NOTEQUAL);
        FDev.SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
        FDEv.SetRenderState(D3DRS_ZWRITEENABLE,iTRUE);
        FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
          4,IBLockPos div 2,6 div 3);
        FDev.SetRenderState(D3DRS_STENCILENABLE,iFALSE);

        VBLockPos:=VBLockPos + (4 * SizeOf(TD3ColorTexVertexNormal));
        IBLockPos:=IBLockPos + (6 * 2);
      end;
    end;
  end;

  { 1. sides }
  if Left and (leftLen > 0) then
  begin
    SetLength(MeshData.MeshVertices,Length(Points) * 2);
    { set vertices }
    curPos:=0;
    for i:=0 to High(Points) do
    begin
      if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then continue;
      if (i > 0) then
      begin
        if Points[i - 1].X >= $FFFF then
          curPos:=curPos + d2VectorLength(d2Vector(Points[i].x - Points[i - 2].x,Points[i].y - Points[i - 2].y))
        else
          curPos:=curPos + d2VectorLength(d2Vector(Points[i].x - Points[i - 1].x,Points[i].y - Points[i - 1].y));
      end;
      with MeshData.MeshVertices[i] do
      begin
        x:=(Points[i].x - A.X) / Abs(B.X - A.X);
        y:=1 - ((Points[i].y - A.Y) / Abs(B.Y - A.Y));
        z:=1;
        tv:=0;
        tu:=curPos / leftLen;
{        tu:=0.02 + (x * 0.96);
        tv:=0.02 + (y * 0.96);}
      end;
      with MeshData.MeshVertices[Length(Points) + i] do
      begin
        x:=(Points[i].x - A.X) / Abs(B.X - A.X);
        y:=1 - ((Points[i].y - A.Y) / Abs(B.Y - A.Y));
        z:=0;
        tv:=1;
        tu:=curPos / leftLen;
{        tu:=0.02 + (x * 0.96);
        tv:=0.02 + (y * 0.96);}
      end;
    end;
    { set indices }
    SetLength(MeshData.MeshIndices,(High(Points)) * 6);
    j:=0;
    for i:=0 to High(Points) - 1 do
    begin
      if (Points[i].X >= $FFFF) and (Points[i].Y >= $FFFF) then
      begin
        continue;
      end;
      if (Points[i + 1].X >= $FFFF) and (Points[i + 1].X >= $FFFF) then
      begin
        continue;
      end;
      MeshData.MeshIndices[(j * 6) + 0]:=i;
      MeshData.MeshIndices[(j * 6) + 1]:=Length(Points) + i;
      MeshData.MeshIndices[(j * 6) + 2]:=Length(Points) + i + 1;
      MeshData.MeshIndices[(j * 6) + 3]:=Length(Points) + i + 1;
      MeshData.MeshIndices[(j * 6) + 4]:=i + 1;
      MeshData.MeshIndices[(j * 6) + 5]:=i;
      Inc(j);
    end;
    SetLength(MeshData.MeshIndices,(j - 0) * 6);
    MeshData.CalcNormals;

    if Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal) > VBSize then Exit;
    if (Length(MeshData.MeshIndices) * 2) > IBSize then Exit;

    if VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
    begin
      VBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
    end;

    if not Checkd3Error(VB.Lock(VBLockPos,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal),Pointer(Ver),Flags)) then
    begin
      C:=d3Opacity(Material.NativeDiffuse,Opacity);
      for i:=0 to Length(MeshData.MeshVertices) - 1 do
        with MeshData.MeshVertices[i] do
        begin
          if FCurrentStates[rs2DScene] then
            Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(x * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(y * Size.Y) + 0.5,Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv)
          else
            Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + (x * Size.X),Center.Y - (Size.Y / 2) + (y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv);
          System.Inc(Ver);
        end;
      { indexs }
      if IBLockPos + (Length(MeshData.MeshIndices) * 2) > IBSize then
      begin
        IBLockPos:=0;
        Flags:=D3DLOCK_DISCARD;
      end
      else
      begin
        Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
      end;
      if not Checkd3Error(IB.Lock(IBLockPos,Length(MeshData.MeshIndices) * 2,Pointer(Id3),Flags)) then
      begin
        for i:=0 to Length(MeshData.MeshIndices) - 1 do
        begin
          Id3^:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }MeshData.MeshIndices[i]; System.Inc(Id3);
        end;
        IB.Unlock;
      end;
      { unlock }
      VB.Unlock;

      //FDev.SetPixelShader(0);
      FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
      FDev.SetIndices(IB);
      FDev.SetStreamSource(0,VB,0,SizeOf(TD3ColorTexVertexNormal));
//      FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
      FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
        Length(MeshData.MeshVertices),IBLockPos div 2,Length(MeshData.MeshIndices) div 3);
//      FDev.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

      VBLockPos:=VBLockPos + (Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
      IBLockPos:=IBLockPos + (Length(MeshData.MeshIndices) * 2);
    end;
  end;

  { free }
  MeshData.Free;
end;

var
  H:Pointer;

procedure TD3DirectXCanvas.UpdateBitmap(Bitmap:TD3Bitmap);
var
  Tex:IDirect3DTexture9;
  Surface:TD3DLockedRect;
  Desc:TD3DSurfaceDesc;
  P:^PD3ColorArray;
  R:TRect;
  i:integer;
begin
  inherited ;
  if Bitmap=nil then Exit;
  if Bitmap.Width * Bitmap.Height=0 then
  begin
    Bitmap.NeedUpdate:=false;
    Exit;
  end;
  if not Assigned(FDev) then Exit;

  Tex:=IDirect3DTexture9(GetHandle(Bitmap));
  { create - if need }
  if Tex=nil then
  begin

    if Checkd3Error(FDev.CreateTexture(Bitmap.Width,Bitmap.Height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,Tex,nil)) then Exit;
      Tex._AddRef;

    Bitmap.NeedUpdate:=true;
    AddHandle(Bitmap,Cardinal(Tex));
  end;

  if Bitmap.NeedUpdate then
  begin
    if not Checkd3Error(Tex.LockRect(0,Surface,nil,D3DLOCK_DISCARD)) then
    begin
      for i:=0 to Bitmap.Height - 1 do
      begin
        Move(Bitmap.Bits[i * Bitmap.Width],PByteArray(Surface.pBits)[i * Surface.Pitch],Bitmap.Width * 4);
      end;
      Tex.UnlockRect(0);
    end;
  end;
  Bitmap.NeedUpdate:=false;
end;

procedure TD3DirectXCanvas.DestroyBitmap(const Bitmap:TD3Bitmap);
var
  Tex:IDirect3DTexture9;
begin
  if (Bitmap <> nil) then
    Bitmap.CanvasList.Remove(Self);
  if Assigned(FDev) then
  begin
    if GetHandle(Bitmap) <> 0 then
    begin
      Tex:=IDirect3DTexture9(GetHandle(Bitmap));
      RemoveHandle(Bitmap);
      Tex._Release;
      SAFE_RELEASE(Tex);
    end;
  end;
end;

procedure TD3DirectXCanvas.DrawText(const AX,AY:single; const AText:WideString; const Opacity:single);
var
  charCount,i:integer;
  fStartX:single;
  pVertices:PD3ColorTexVertexNormal;
  pIndexs:PWord;
  c:Word;
  tx1,ty1,tx2,ty2:single;
  scale,x,y,w,h:single;
  Vertex:word;
  Flags:cardinal;
  CurFont:TD3InternalFont;
begin
  if Length(AText)=0 then Exit;
  x:=AX;
  y:=AY;
  if FCurrentStates[rs2DScene] then
  begin
    x:=Round(FCurrentMatrix.m41 + x) - FCurrentMatrix.m41;
    y:=Round(FCurrentMatrix.m42 + y) - FCurrentMatrix.m42;
  end;
  { Update font }
  i:=UpdateFont(AText,Font);
  if i < 0 then Exit;
  CurFont:=FFonts[i];
  { Calc real vertex count }
  charCount:=0;
  for i:=1 to Length(AText) do
  begin
    c:=Word(AText[i]);
    if c <= 32 then Continue;
    if ((c - 32) < 0) or ((c - 32) > Length(CurFont.CharCoords)) then
      Continue;
    charCount:=charCount + 1;
  end;
  { Draw text }
  if VBLockPos + (charCount * 4 * SizeOf(TD3ColorTexVertexNormal)) > VBSize then
  begin
    VBLockPos:=0;
    Flags:=D3DLOCK_DISCARD;
  end
  else
  begin
    Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
  end;
  if not Checkd3Error(VB.Lock(VBLockPos,charCount * 4 * SizeOf(TD3ColorTexVertexNormal),Pointer(pVertices),Flags)) then
  begin
    if IBLockPos + (charCount * 6 * 2) > IBSize then
    begin
      IBLockPos:=0;
      Flags:=D3DLOCK_DISCARD;
    end
    else
    begin
      Flags:=D3DLOCK_NOOVERWRITE{ or D3DLOCK_DISCARD};
    end;
    if not Checkd3Error(IB.Lock(IBLockPos,charCount * 6 * 2,Pointer(pIndexs),Flags)) then
    begin
      { Draw }
      Vertex:=0; //(VBLockPos div SizeOf(TD3ColorTexVertexNormal));
      for i:=1 to Length(AText) do
      begin
        c:=Word(AText[i]);
        if c < 32 then Continue;
        if ((c - 32) < 0) or ((c - 32) > Length(CurFont.CharCoords)) then Continue;

        tx1:=CurFont.CharCoords[c-32].Coord[0];
        ty1:=CurFont.CharCoords[c-32].Coord[1];
        tx2:=CurFont.CharCoords[c-32].Coord[2];
        ty2:=CurFont.CharCoords[c-32].Coord[3];

        w:=(tx2-tx1) * CurFont.Bitmap.Width;
        h:=(ty2-ty1) * CurFont.Bitmap.Height;

        if (c <> 32) then
        begin
          x:=x + CurFont.CharCoords[c-32].a;

          if FCurrentStates[rs2DScene] then
          begin
            pVertices^:=ColorTexVertexNormal(x + 0 + 0.5,y + 0 + 0.5,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(pVertices);
            pVertices^:=ColorTexVertexNormal(x + w + 0.5,y + 0 + 0.5,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(pVertices);
            pVertices^:=ColorTexVertexNormal(x + w + 0.5,y + h + 0.5,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(pVertices);
            pVertices^:=ColorTexVertexNormal(x + 0 + 0.5,y + h + 0.5,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(pVertices);
          end
          else
          begin
            scale:=0.02;
            pVertices^:=ColorTexVertexNormal((x + 0) * scale,0,(y + h) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(pVertices);
            pVertices^:=ColorTexVertexNormal((x + w) * scale,0,(y + h) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(pVertices);
            pVertices^:=ColorTexVertexNormal((x + w) * scale,0,(y + 0) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(pVertices);
            pVertices^:=ColorTexVertexNormal((x + 0) * scale,0,(y + 0) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(pVertices);
          end;

          pIndexs^:=Vertex + 0; System.Inc(pIndexs);
          pIndexs^:=Vertex + 1; System.Inc(pIndexs);
          pIndexs^:=Vertex + 3; System.Inc(pIndexs);
          pIndexs^:=Vertex + 3; System.Inc(pIndexs);
          pIndexs^:=Vertex + 1; System.Inc(pIndexs);
          pIndexs^:=Vertex + 2; System.Inc(pIndexs);
          x:=x + CurFont.CharCoords[c-32].c;
          Vertex:=Vertex + 4;
        end;
        x:=x + (w);
      end;

      IB.Unlock;
    end;
    VB.Unlock;

    UpdateBitmap(CurFont.Bitmap);
    if GetHandle(CurFont.Bitmap) <> 0 then
    begin
      SetRenderState(rsTexModulate);
      FDev.SetTexture(0,IDirect3DTexture9(GetHandle(CurFont.Bitmap)));
      FDev.SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

      //FDev.SetPixelShader(0);
      FDev.SetFVF(D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1 or D3DFVF_DIFFUSE);
      FDev.SetIndices(IB);
      FDev.SetStreamSource(0,VB,0,SizeOf(TD3ColorTexVertexNormal));
      FDev.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),0,
        charCount * 4,IBLockPos div 2,(charCount * 6) div 3);
    end;
    VBLockPos:=VBLockPos + (charCount * 4 * SizeOf(TD3ColorTexVertexNormal));
    IBLockPos:=IBLockPos + (charCount * 6 * 2);
  end;
end;

function TD3DirectXCanvas.MeasureText(const AText:WideString):TD3Point;
var
  i:integer;
  CurFont:TD3InternalFont;
  fRowWidth,fRowHeight,fWidth,fHeight:single;
  c:Word;
  tx1,tx2:single;
begin
  Result:=d3Point(0,0,0);
  if Length(AText)=0 then Exit;
  { Update font }
  i:=UpdateFont(AText,Font);
  if i < 0 then Exit;
  CurFont:=FFonts[i];

  fRowWidth:=0.0;
  fRowHeight:= (CurFont.CharCoords[0].Coord[3] - CurFont.CharCoords[0].Coord[1]) * CurFont.Bitmap.Height;
  fWidth   :=0.0;
  fHeight  :=fRowHeight;
  for i:=1 to Length(AText) do
  begin
    c:=Word(AText[i]);
    if c < 32 then Continue;

    if (c=0010) then
    begin
      fRowWidth:=0.0;
      fHeight :=fHeight + fRowHeight;
    end;
    if (c < 0032) then Continue;

    if ((c - 32) < 0) or ((c - 32) > Length(CurFont.CharCoords) - 1) then
    begin
      Continue;
    end;

    tx1:=CurFont.CharCoords[c - 32].Coord[0];
    tx2:=CurFont.CharCoords[c - 32].Coord[2];

//    if (fRowWidth + (tx2-tx1) * CurFont.Bitmap.Width > ClipWidth) then Break;

    fRowWidth:=fRowWidth + CurFont.CharCoords[c - 32].a + ((tx2-tx1) * CurFont.Bitmap.Width) + CurFont.CharCoords[c - 32].c;
    if (fRowWidth > fWidth) then fWidth:=fRowWidth;

{    if i <= Length(AText) then
      CalcStrLen:=i;}
  end;

  Result.x:=fWidth;
  Result.y:=fHeight;
end;

function TD3DirectXCanvas.UpdateFont(const AText:WideString; Font:TD3Font):integer;
const
  MaxPixmapSize=1024;
type
  pBit=array[0..0] of Cardinal;
  PpBit=^pBit;
var
  bmi:TBitmapInfo;
  hDC_:HDC;
  hbmBitmap:HBITMAP;
  nHeight:integer;
  dwBold,dwItalic:Cardinal;
  hFont_:HFONT;
  x,y:Cardinal;
  str:array[0..1] of WideChar;
  size_:TSize;
  c:word;
  pBitmapBits:PD3ColorArray;
  pDstRow:PD3ColorArray;
  Res:HResult;
  Calc:boolean;
  Pix:Longword;
  abc:TABCFLOAT;
  tm:TTextMetric;
  i,j:integer;
  NewFont:TD3InternalFont;
  FTexWidth,FTexHeight:integer;
  NewFontId3:integer;
  NeedUpdate:boolean;
begin
  Result:=-1;
  if not Assigned(FDev) then Exit;
  { check ready font }
  NeedUpdate:=false;
  NewFontId3:=-1;
  Fillchar(NewFont,SizeOf(Newfont),0);
  for i:=0 to High(FFonts) do
    if (CompareText(FFonts[i].Family,Font.Family)=0) and (FFonts[i].Size=Font.Size) and
       (FFonts[i].Style=Font.Style) then
    begin
      { check chars }
      NewFont:=FFonts[i];
      NewFontId3:=i;
      if Length(NewFont.CharCoords) > 0 then
      begin
        for j:=1 to Length(AText) do
        begin
          if Ord(AText[j]) < 32 then Continue;
          if (Length(NewFont.CharCoords) <= Ord(AText[j]) - 32) then
          begin
            SetLength(NewFont.CharCoords,Ord(AText[j]) - 32 + 1);
            NewFont.CharCoords[Ord(AText[j]) - 32].Exists:=1;
            NeedUpdate:=true;
            Continue;
          end;
          if (NewFont.CharCoords[Ord(AText[j]) - 32].Exists=0) then
          begin
            NewFont.CharCoords[Ord(AText[j]) - 32].Exists:=1;
            NeedUpdate:=true;
          end;
        end;
      end;
      {}
      if not NeedUpdate then
      begin
        Result:=i;
        Exit;
      end
      else
        Break;
    end;

  if not NeedUpdate then
  begin
    SetLength(NewFont.CharCoords,127 - 32 + 1);
    FillChar(NewFont.CharCoords[0],SizeOf(NewFont.CharCoords[0]) * Length(NewFont.CharCoords),0);
    for i:=0 to 127 - 32 do
      NewFont.CharCoords[i].Exists:=1;
  end;
  { Build font }
  NewFont.Family:=Font.Family;
  NewFont.Size:=Font.Size;
  NewFont.Style:=Font.Style;
  { Calc texture size for base charset }
  hDC_:=CreateCompatibleDC(0);
  // Create a font.  By specifying ANTIALIASED_QUALITY,we might get an
  // antialiased font,but this is not guaranteed.
  nHeight:= -Round(Font.Size);
  dwBold:=0;
  dwItalic:=0;
  if Font.Style=d3FontBoldItalic then
  begin
    dwBold:=FW_BOLD;
    dwItalic:=1;
  end;
  if Font.Style=d3FontBold then
    dwBold:=FW_BOLD;
  if Font.Style=d3FontItalic then
    dwItalic:=1;
  hFont_:=CreateFont(nHeight,0,0,0,dwBold,dwItalic,
                       0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,
                       DEFAULT_PITCH or FF_DONTCARE,PChar(Font.Family));
  if (0=hFont_) then
  begin
    Exit;
  end;
  SelectObject(hDC_,hFont_);
  GetTextMetrics(hDC_,tm);

  str[0]:='x';
  str[1]:=#0;
  FTexWidth:=64;
  FTexHeight:=64;
  Calc:=true;
  while Calc do
  begin
    Calc:=false;
    x:=0;
    y:=0;
    for c:=0 to High(NewFont.CharCoords) do
    begin
      if NewFont.CharCoords[c].Exists=0 then Continue;

      str[0]:=WideChar(32 + c);
      GetTextExtentPoint32W(hDC_,str,1,size_);
      size_.cx:=size_.cx + 2;
      size_.cy:=size_.cy + 2;

      if (x + Longword(size_.cx) + 1 > FTexWidth) then
      begin
        x:=0;
        y:=y + Longword(size_.cy) + 2;
      end;
      x:=x + Longword(size_.cx) + 2;
      if y + Longword(size_.cy) + 1  > FTexHeight then
      begin
        if FTexWidth=FTexHeight then
        begin
          // first time expand by x
          FTexWidth:=FTexWidth * 2;
        end
        else
        begin
          // second time expand by y
          FTexHeight:=FTexHeight * 2;
        end;
        Calc:=true;
        Break;
      end;
    end;
    if FTexHeight > MaxPixmapSize then
    begin
      Exit;
    end;
  end;
  DeleteDC(hDC_);

  // Create a new texture for the font
  if NewFont.Bitmap <> nil then
    NewFont.Bitmap.Free;
  NewFont.Bitmap:=TD3Bitmap.Create(FTexWidth,FTexHeight);

  // Prepare to create a bitmap
  ZeroMemory(@(bmi.bmiHeader),SizeOf(TBitmapInfoHeader));
  bmi.bmiHeader.biSize       :=SizeOf(TBitmapInfoHeader);
  bmi.bmiHeader.biWidth      := FTexWidth;
  bmi.bmiHeader.biHeight     :=-FTexHeight;
  bmi.bmiHeader.biPlanes     :=1;
  bmi.bmiHeader.biCompression:=BI_RGB;
  bmi.bmiHeader.biBitCount   :=32;

  // Create a DC and a bitmap for the font
  hDC_:=CreateCompatibleDC(0);
  hbmBitmap:=CreateDIBSection(hDC_,bmi,DIB_RGB_COLORS,Pointer(pBitmapBits),0,0);
  SetMapMode(hDC_,MM_TEXT);

  // Create a font.  By specifying ANTIALIASED_QUALITY,we might get an
  // antialiased font,but this is not guaranteed.

  SelectObject(hDC_,hFont_);
  SelectObject(hDC_,hbmBitmap);

  // Set text properties
  SetTextColor(hDC_,RGB(255,255,255));
  SetBkColor(hDC_,0);
  SetTextAlign(hDC_,TA_TOP);
	SetMapMode(hDC_,MM_TEXT);

  // Loop through all printable character and output them to the bitmap..
  // Meanwhile,keep track of the corresponding tex coords for each character.
  x:=1;
  y:=1;
  str[0]:='x';
  str[1]:=#0;

  { Make texture }
  for c:=0 to High(NewFont.CharCoords) do
  begin
    if NewFont.CharCoords[c].Exists=0 then Continue;

    str[0]:=WideChar(32 + c);
    GetCharABCWidthsFloatW(hDC_,32 + c,32 + c,abc);
    size_.cx:=trunc(abc.abcfB) + 2;
    size_.cy:=tm.tmHeight;

    if (x + Longword(size_.cx) + 1 > FTexWidth) then
    begin
      x:=1;
      y:=y + size_.cy + 1;
    end;

    NewFont.CharCoords[c].Coord[0]:=((x)) / FTexWidth;
    NewFont.CharCoords[c].Coord[1]:=((y)) / FTexHeight;
    NewFont.CharCoords[c].Coord[2]:=((x + size_.cx)) / FTexWidth;
    NewFont.CharCoords[c].Coord[3]:=((y + size_.cy)) / FTexHeight;
    NewFont.CharCoords[c].a:=trunc(abc.abcfA) - 1;
    NewFont.CharCoords[c].c:=trunc(abc.abcfC) - 1;

//    Rectangle(hDC_,x,y,x + size_.cx,y + size_.cy);
    TextOutW(hDC_,x - trunc(NewFont.CharCoords[c].a),y,str,1);


    x:=x + size_.cx + 1;
  end;

  // Lock the surface and write the alpha values for the set pixels
  NewFont.Bitmap.LockBitmapBits(pDstRow,true);
  FillChar(pDstRow^,FTexWidth * FTexHeight * 4,0);

  for x:=0 to FTexHeight - 1 do
  begin
    for y:=0 to FTexWidth - 1 do
    begin
      if pBitmapBits[y + (x * FTexWidth)] > 0 then
      begin
        Pix:=((pBitmapBits[y + (x * FTexWidth)] and $FF) shl 24) or $FFFFFF;
        pDstRow[y + (x * FTexWidth)]:=Pix;
      end
      else
      begin
        Pix:=$00FFFFFF;
        pDstRow[y + (x * FTexWidth)]:=Pix;
      end;
    end;
  end;
  // Done updating texture,so clean up used objects
  Newfont.Bitmap.UnlockBitmapBits;

  DeleteObject(hbmBitmap);
  DeleteDC(hDC_);
  DeleteObject(hFont_);

  if (NewFontId3 >= 0) and NeedUpdate then
  begin
    FFonts[NewFontId3]:=NewFont;
    Result:=NewFontId3;
  end
  else
  begin
    SetLength(FFonts,Length(FFonts) + 1);
    FFonts[High(FFonts)]:=NewFont;
    Result:=High(FFonts);
  end;
end;

var
  Direct3D9Lib:THandle;






