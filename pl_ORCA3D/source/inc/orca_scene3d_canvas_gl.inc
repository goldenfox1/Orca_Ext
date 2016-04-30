{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

type

  TD3FilterOpenGL=class(TD3Filter)
  private
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

  { TD3CanvasOpenGL }

  TD3CanvasOpenGL=class(TD3Canvas)
  private
    FFonts:array of TD3InternalFont;
    {$IFDEF DARWIN}
    aglContext:TAGLContext;
    aglPBuffer:TAGLPbuffer;
    {$ENDIF}
    {$IFDEF UNIX}
    CurXDisplay:Pointer;
    CurXWindow:LongInt;
    FGTKWidget:PGTKWidget;
    aglContext:GLXContext;
    aglPBuffer:Pointer;
    aglDrawable:Pointer;
    {$ENDIF}
    VBLockPos,IBLockPos:integer;
    AllowSample:boolean;
    BlendSeparate:boolean;
    function UpdateFont(const AText:WideString; Font:TD3Font):integer;
    procedure ApplyLights;
  protected
    procedure UpdateBitmap(Bitmap:TD3Bitmap);  override;
    procedure DestroyBitmap(const Bitmap:TD3Bitmap);  override;
    procedure MaterialChanged(Sender:TObject);  override;
    procedure CreateRenderStateList(AState:TD3RenderState);  override;
    procedure ApplyRenderState(AState:TD3RenderState);  override;
  public
    constructor Create(const AWnd:cardinal; const AWidth,AHeight:integer; const ALockable:boolean; const Quality:TD3Quality);  override;
    constructor CreateFromBitmap(const ABitmap:TD3Bitmap);  override;
    destructor Destroy;  override;
    procedure SetQuality(const Quality:TD3Quality);  override;
    function  BeginScene:boolean;  override;
    procedure Clear(const AColor:TD3Color);  override;
    procedure FlushBuffer;  override;
    procedure FreeBuffer;  override;
    procedure ResizeBuffer(const AWidth,AHeight:integer);  override;
    procedure CopyBits(const Bits:PD3ColorArray);  override;
    procedure DrawLine(const StartPoint,EndPoint:TD3Vector; const Opacity:single);  override;
    procedure DrawRect(const StartPoint,EndPoint:TD3Vector; const Opacity:single);  override;
    procedure DrawCube(const Center,Size:TD3Vector; const Opacity:single);  override;
    procedure FillCube(const Center,Size:TD3Vector; const Opacity:single);  override;
    procedure FillMesh(const Center,Size:TD3Vector; const MeshData:TD3MeshData; const Opacity:single);  override;
    procedure FillPolygon(const Center,Size:TD3Vector; const Rect:TD2Rect; const Points:TD2Polygon; const Opacity:single;
                          Front:boolean=true; Back:boolean=true; Left:boolean=true);  override;
                          
    procedure DrawText(const AX,AY:single; const AText:WideString; const Opacity:single);  override;
    function  MeasureText(const AText:WideString):TD3Point;  override;
    
    procedure SetMatrix(const M:TD3Matrix);  override;
  published
  end;

{$IFDEF UNIX}
const
  GLX_SAMPLE_BUFFERS=100000;
  GLX_SAMPLES=       100001;
  GLX_PBUFFER_HEIGHT		= $8040;	// New for GLX 1.3
  GLX_PBUFFER_WIDTH		= $8041;	// New for GLX 1.3
  GLX_DRAWABLE_TYPE                               =$8010;
  GLX_FBCONFIG_ID                                 =$8013;
  GLX_VISUAL_ID                                   =$800B;
  GLX_WINDOW_BIT                                  =$00000001;
  GLX_PIXMAP_BIT                                  =$00000002;
  GLX_PBUFFER_BIT                                 =$00000004;
  GLX_LARGEST_PBUFFER		                  =$801C;
  GLX_RENDER_TYPE                                 =$8011;
  GLX_RGBA_BIT			                  =00000001;
  GLX_RGBA_TYPE                                   =$8014;

  opengl32='libGL.so';
type
  GLXFBConfig  =Pointer;
  PGLXFBConfig =^GLXFBConfig;
  GLXPBuffer=Pointer;

function  glXChooseFBConfig(dpy:PDisplay; screen:TGLInt; attribList:PGLInt; nitems:PGLInt):PGLXFBConfig; cdecl; external opengl32;
function  glXGetFBConfigAttrib(dpy:PDisplay; config:GLXFBConfig; attribute:TGLInt; value:PGLInt):TGLInt; cdecl; external opengl32;
function  glXGetFBConfigs(dpy:PDisplay; screen:TGLInt; nelements:PGLInt):GLXFBConfig; cdecl; external opengl32;
function  glXGetVisualFromFBConfig(dpy:PDisplay; config:GLXFBConfig):PXVisualInfo; cdecl; external opengl32;
function  glXCreatePbuffer(dpy:PDisplay; config:GLXFBConfig; attribList:PGLInt):GLXPBuffer; cdecl; external opengl32;
procedure glXDestroyPbuffer(dpy:PDisplay; pbuf:GLXPBuffer); cdecl; external opengl32;
function  glXCreateNewContext(dpy:PDisplay; config:GLXFBConfig; renderType:TGLInt; shareList:GLXContext; direct:TGLboolean):GLXContext; cdecl; external opengl32;
{$ENDIF}


procedure ORCA_InitOpenGL;
var
  LibGL:cardinal;
begin
  {$IFDEF DARWIN}
  LibGL:=LoadLibrary(PChar('/System/Library/Frameworks/AGL.framework/AGL'));
  if LibGL <> 0 then
    @aglSetHiViewRef:=GetProcAddress(LibGL,PChar('aglSetHIViewRef'))
  else
    @aglSetHiViewRef:=nil;

  GetMaxTextureSize;
  {$ENDIF}

  Load_GL_version_1_4;

end;

{$IFDEF DARWIN}
var
  aglSetHiViewRef:function (ctx:TAGLContext; view:HiViewRef):GLboolean; cdecl;

function CGRectFromRect(const R:TD2Rect):CGRect;
begin
  Result.origin.x:=R.Left;
  Result.origin.Y:=R.Top;
  Result.size.Width:=R.Right - R.Left;
  Result.size.Height:=R.Bottom - R.Top;
end;

{$ENDIF}

function ColorToGlColor(AColor:TColor):TD3Vector;
begin
  Result.X:=TD3ColorRec(AColor).R / $FF;
  Result.Y:=TD3ColorRec(AColor).G / $FF;
  Result.Z:=TD3ColorRec(AColor).B / $FF;
  Result.W:=TD3ColorRec(AColor).A / $FF;
end;

{ TD3FilterOpenGL ==================================================}

class function TD3FilterOpenGL.GetFileTypes:string;
begin
  Result:=orca_scene2d.GvarD2DefaultFilterClass.GetFileTypes;
end;

class function TD3FilterOpenGL.GetImageSize(const AFileName:string):TD3Point;
begin
  with orca_scene2d.GvarD2DefaultFilterClass.GetImageSize(AFileName) do
    Result:=d3Point(X,Y,0);
end;

function TD3FilterOpenGL.LoadFromStream(const AStream:TStream; var Bitmap:TD3Bitmap):boolean;
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
      d2MoveLongword(B.StartLine,bits,Bitmap.Width * Bitmap.Height);
      Bitmap.UnlockBitmapBits;
    end;
  finally
    B.Free;
  end;
end;

function TD3FilterOpenGL.SaveToStream(const AStream:TStream; var Bitmap:TD3Bitmap; const Format:string;
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
      d2MoveLongword(bits,B.StartLine,Bitmap.Width * Bitmap.Height);
      Bitmap.UnlockBitmapBits;
    end;

    F:=orca_scene2d.GvarD2DefaultFilterClass.Create;
    F.SaveToStream(AStream,B,Format,Params);
    F.Free;
  finally
    B.Free;
  end;
end;

function TD3FilterOpenGL.LoadFromFile(const AFileName:string; const Rotate:single;
  var Bitmap:TD3Bitmap):boolean;
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
      d2MoveLongword(B.StartLine,bits,Bitmap.Width * Bitmap.Height);
      Bitmap.UnlockBitmapBits;
    end;
  finally
    B.Free;
  end;
end;

function TD3FilterOpenGL.LoadThumbnailFromFile(const AFileName:string;
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
      d2MoveLongword(B.StartLine,bits,Bitmap.Width * Bitmap.Height);
      Bitmap.UnlockBitmapBits;
    end;
  finally
    B.Free;
  end;
end;

function TD3FilterOpenGL.SaveToFile(const AFileName:string;
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
      d2MoveLongword(bits,B.StartLine,Bitmap.Width * Bitmap.Height);
      Bitmap.UnlockBitmapBits;
    end;

    F:=orca_scene2d.GvarD2DefaultFilterClass.Create;
    F.SaveToFile(AFileName,B,Params);
    F.Free;
  finally
    B.Free;
  end;
end;

{ TD3CanvasOpenGL ==================================================}

{$IFDEF DARWIN}

procedure GetMaxTextureSize;
var
  AttrList:array [0..8] of integer=(
    AGL_WINDOW,
    AGL_RGBA,
    AGL_DOUBLEBUFFER,
    AGL_ACCELERATED,
    AGL_STENCIL_SIZE,8,
    AGL_DEPTH_SIZE,24,
    AGL_NONE
  );
var
  i,Idx:integer;
  disp:GDHandle;
  aglPixFrm:TAGLPixelFormat;
  aglContext:TAGLContext;
begin
  disp:=GetMainDevice ();

  aglPixFrm:=aglChoosePixelFormat(@disp,1,AttrList);

  aglContext:=aglCreateContext (aglPixFrm,NIL);
  if aglContext <> nil then
    aglDestroyPixelFormat(aglPixFrm);
  { set first }
  aglSetCurrentContext(aglContext);
  { get }
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,@i);
  { }
  MaxBitmapSize:=i;
  { Free }
  aglSetCurrentContext(nil);
  aglDestroyContext(aglContext);
end;

{$ENDIF}

constructor TD3CanvasOpenGL.Create(const AWnd:cardinal; const AWidth,AHeight:integer; const ALockable:boolean; const Quality:TD3Quality);
{$IFDEF UNIX}
function CreateOpenGLContextAttrList:PInteger;
var
  p:integer;

  procedure Add(i:integer);
  begin
    if Result<>nil then
      PD3ColorArray(Result)[p]:=i;
    System.inc(p);
  end;

  procedure CreateList;
  begin
    Add(GLX_DOUBLEBUFFER); Add(GL_TRUE);
    Add(GLX_RGBA); Add(GL_TRUE);
    Add(GLX_STENCIL_SIZE); Add(8);
    Add(GLX_DEPTH_SIZE); Add(24);
    if (FQuality <> d3LowQuality) then
    begin
      Add(GLX_SAMPLE_BUFFERS); Add(GL_TRUE);
      if (FQuality=d3SuperHighQuality) then
      begin
        Add(GLX_SAMPLES); Add(4);
      end
      else
        if (FQuality=d3HighQuality) then
        begin
          Add(GLX_SAMPLES); Add(2);
        end;
    end;
    Add(0);
  end;

begin
  Result:=nil;
  p:=0;
  CreateList;
  GetMem(Result,SizeOf(integer)*p);
  p:=0;
  CreateList;
end;
{$ENDIF}

{$IFDEF UNIX}
var
  AttrList:array [0..8] of integer=(
    GLX_DOUBLEBUFFER,GL_TRUE,
    GLX_RGBA,GL_TRUE,
    GLX_STENCIL_SIZE,8,
    GLX_DEPTH_SIZE,24,
    0
  );
  AttrListMultisample:array [0..12] of integer=(
    GLX_DOUBLEBUFFER,GL_TRUE,
    GLX_RGBA,GL_TRUE,
    GLX_STENCIL_SIZE,8,
    GLX_DEPTH_SIZE,24,
    GLX_SAMPLE_BUFFERS,GL_TRUE,
    GLX_SAMPLES,2,
    0
  );
{$ENDIF}
{$IFDEF DARWIN}
var
  AttrList:array [0..8] of integer=(
    AGL_WINDOW,
    AGL_DOUBLEBUFFER,
    AGL_RGBA,
    AGL_ACCELERATED,
    AGL_STENCIL_SIZE,8,
    AGL_DEPTH_SIZE,24,
    AGL_NONE
  );
{$ENDIF}
var
  i,Id3:integer;
  {$IFDEF DARWIN}
  disp:GDHandle;
  aglPixFrm:TAGLPixelFormat;
  Caps:array [0..2] of longword;
  modes:CFArrayRef;
  mode,cmode:CFDictionaryRef;
  modeIndex:integer;
  {$ENDIF}
  {$IFDEF UNIX}
  winattr:TXWindowAttributes;
  vitemp:TXVisualInfo;
  nret:integer;
  vi:PXvisualInfo;
  fb:PGLXFBConfig;
  CurScreen:integer;
  nitems:integer;
  old_context:GLXContext;
  {$ENDIF}
  SaveQuality:TD3Quality;
  Count:integer;
  S:string;
  St:TD3RenderState;
begin
  inherited;
  ORCA_InitOpenGL;
  {$IFDEF UNIX}
  fGTKWidget:=GetFixedWidget(pgtkwidget(AWnd));
  gtk_widget_realize(FGTKWidget);

  CurXDisplay:=GDK_WINDOW_XDISPLAY (PGdkDrawable(fGTKWidget^.window));
  CurXWindow:=GDK_WINDOW_XWINDOW (PGdkDrawable(fGTKWidget^.window));

  XGetWindowAttributes(CurXDisplay,CurXWindow,@winattr);
  vitemp.visual:=winattr.visual;
  vitemp.visualid:=XVisualIDFromVisual(vitemp.visual);
  vi:=XGetVisualInfo(CurXDisplay,VisualIDMask,@vitemp,@nret);
  CurScreen:=vi^.screen;
  XFree(vi);

  if FQuality <> d3LowQuality then
  begin
    if FQuality=d3HighQuality then
      AttrListMultisample[High(AttrListMultisample) - 1]:=2
    else
      AttrListMultisample[High(AttrListMultisample) - 1]:=4;
    vi:=glXChooseVisual(CurXDisplay,CurScreen,AttrListMultisample);
  end
  else
    vi:=glXChooseVisual(CurXDisplay,CurScreen,AttrList);
  aglContext:=glXCreateContext(CurXDisplay,vi,nil,true); //Last Param (Direct Draw) schoud MUST true,otherwise I get
  XFree(vi);

  glXMakeCurrent(CurXDisplay,CurXWindow,aglContext);

  if FQuality <> d3LowQuality then
    glEnable(GL_MULTISAMPLE_ARB);
  {$ENDIF}
  {$IFDEF DARWIN}
  disp:=GetMainDevice();
  aglPixFrm:=aglChoosePixelFormat(@disp,1,AttrList);

  aglContext:=aglCreateContext (aglPixFrm,NIL);
  aglDestroyPixelFormat(aglPixFrm);

  if aglContext <> nil then
  begin
    aglSetCurrentContext(aglContext);
    if Assigned(aglSetHiViewRef) then
      aglSetHiViewRef(aglContext,HiViewRef(TCarbonWidget(FWnd).Widget))
    else
    begin
      aglSetDrawable(aglContext,GetWindowPort(WindowRef(TCarbonWindow(Application.Mainform.Handle).Window)));
    end;

    i:=0;
    if FLockable then
    begin
      i:=0;
      aglSetInteger(aglContext,AGL_SURFACE_OPACITY,@i);
    end;
  end;
  {$ENDIF}
  glViewport(0,0,FWidth,FHeight);
  BlendSeparate:=Load_GL_EXT_blend_func_separate;
  { states }
  for St:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(St);
end;

constructor TD3CanvasOpenGL.CreateFromBitmap(const ABitmap:TD3Bitmap);
{$IFDEF UNIX}
var
  PBufferList:array [0..6] of integer=(
    GLX_PBUFFER_WIDTH,512,
    GLX_PBUFFER_HEIGHT,512,
    GLX_LARGEST_PBUFFER,GL_False,
    0
  );
  AttrList:array [0..18] of integer=(
    GLX_RENDER_TYPE,GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE,GLX_PBUFFER_BIT,
    GLX_DOUBLEBUFFER,GL_FALSE,
    GLX_RED_SIZE,8,
    GLX_GREEN_SIZE,8,
    GLX_BLUE_SIZE,8,
    GLX_ALPHA_SIZE,8,
    GLX_STENCIL_SIZE,8,
    GLX_DEPTH_SIZE,24,
    0
  );
{$ENDIF}
{$IFDEF DARWIN}
var
  AttrList:array [0..6] of integer=(
    AGL_RGBA,
    AGL_ACCELERATED,
    AGL_STENCIL_SIZE,8,
    AGL_DEPTH_SIZE,24,
    AGL_NONE
  );
{$ENDIF}
var
  i,Id3:integer;
  {$IFDEF DARWIN}
  disp:GDHandle;
  aglPixFrm:TAGLPixelFormat;
  Caps:array [0..2] of longword;
  modes:CFArrayRef;
  mode,cmode:CFDictionaryRef;
  modeIndex:integer;
  {$ENDIF}
  {$IFDEF UNIX}
  fb:PGLXFBConfig;
  CurScreen:integer;
  nitems:integer;
  old_context:GLXContext;
  {$ENDIF}
  Count:integer;
  S:string;
  St:TD3RenderState;
begin
  inherited;
  ORCA_InitOpenGL;
  {$IFDEF UNIX}
  old_context:=glXGetCurrentContext();

  CurXDisplay:=XOpenDisplay(nil);
  CurScreen:=DefaultScreen(CurXDisplay);

  nitems:=0;
  fb:=glXChooseFBConfig(CurXDisplay,CurScreen,AttrList,@nitems);
  if fb=nil then Exit;

  PBufferList[1]:=FWidth;
  PBufferList[3]:=FHeight;
  aglPBuffer:=glXCreatePbuffer(CurXDisplay,fb^,PBufferList);
  if aglPBuffer=nil then Exit;

  aglContext:=glXCreateNewContext(CurXDisplay,fb^,GLX_RGBA_TYPE,old_context,1); //Last Param (Direct Draw) schoud MUST true,otherwise I get

  glXMakeCurrent(CurXDisplay,GLXDrawable(aglPBuffer),aglContext);

  {$ENDIF}
  {$IFDEF DARWIN}
  disp:=GetMainDevice ();
  aglPixFrm:=aglChoosePixelFormat(@disp,1,AttrList);
  aglContext:=aglCreateContext(aglPixFrm,NIL);
  aglDestroyPixelFormat(aglPixFrm);

  if aglContext <> nil then
  begin
    aglSetCurrentContext(aglContext);
    aglCreatePBuffer(FWidth,FHeight,GL_TEXTURE_RECTANGLE_EXT,GL_RGBA,0,@aglPBuffer);
    aglSetPBuffer(aglContext,aglPBuffer,0,0,0);
  end;
  {$ENDIF}
  glViewport(0,0,FWidth,FHeight);
  BlendSeparate:=Load_GL_EXT_blend_func_separate;
  { states }
  for St:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(St);
end;

procedure TD3CanvasOpenGL.CopyBits(const Bits:PD3ColorArray);
begin
  if FLockable and (FBitmap <> nil) then
  begin
    d2MoveLongword(FBitmap.Bits,Bits,FWidth * FHeight);
  end
  else
  if FLockable and (FBufferHandle <> nil) then
  begin
    glReadPixels(0,0,FWidth,FHeight,GL_RGBA,GL_UNSIGNED_BYTE,Bits);
  end
end;

procedure TD3CanvasOpenGL.ResizeBuffer(const AWidth,AHeight:integer);
var
  S:TD3RenderState;
begin
  inherited ;
  if FBitmap <> nil then
  begin
    glViewport(0,0,AWidth,AHeight);
  end
  else
  if Assigned(aglContext) then
  begin
    {$IFDEF UNIX}
    {$ENDIF}
    {$IFDEF DARWIN}
    aglSetCurrentContext(aglContext);
    aglUpdateContext(aglContext);
    {$ENDIF}
    glViewport(0,0,AWidth,AHeight);
    // states
    for S:=TD3RenderState(0) to rsGouraud do
      CreateRenderStateList(S);
  end;
end;

procedure TD3CanvasOpenGL.SetQuality(const Quality:TD3Quality);
var
  S:TD3RenderState;
begin
  inherited ;
  if Assigned(aglContext) then
  begin
    // states
    for S:=TD3RenderState(0) to rsGouraud do
      CreateRenderStateList(S);
  end;
end;

destructor TD3CanvasOpenGL.Destroy;
var
  i:integer;
begin
  for i:=0 to High(FFonts) do
    if FFonts[i].Bitmap <> nil then
      FFonts[i].Bitmap.Free;
  inherited;
end;

procedure TD3CanvasOpenGL.FreeBuffer;
begin
  {$IFDEF UNIX}
  if (glXGetCurrentContext()=aglContext) and
     (not glXMakeCurrent(CurXDisplay,0,nil)) then Exit;
  if aglPBuffer <> nil then
    glXDestroyPbuffer(CurXDisplay,aglPBuffer);
  glXDestroyContext(CurXDisplay,aglContext);
  {$ENDIF}
  {$IFDEF DARWIN}
  aglSetCurrentContext(nil);
  if aglPBuffer <> nil then
    aglDestroyPBuffer(aglPBuffer);
  aglPBuffer:=nil;
  aglDestroyContext(aglContext);
  aglContext:=nil;
  {$ENDIF}
  inherited;
end;

procedure TD3CanvasOpenGL.Clear(const AColor:TD3Color);
begin
  glClearDepth(1.0);
  glClearStencil(0);
  glClearColor(TD3ColorRec(AColor).R / $FF,TD3ColorRec(AColor).G / $FF,TD3ColorRec(AColor).B / $FF,0.0);
  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT);
end;

procedure TD3CanvasOpenGL.ApplyLights;
var
  i:integer;
  Color,Dir:TD3Vector;
begin
  { default light }
  if not Lighting then Exit;

  for i:=0 to FCurrentLights.Count - 1 do
  begin
    if TD3Light(FCurrentLights[i]).Enabled then
      glEnable(GL_LIGHT0 + i)
    else
      glDisable(GL_LIGHT0 + i);
//      Light.Range:=100.0;
    Color:=ColorToGlColor($FF696969);
    glLightfv(GL_LIGHT0 + i,GL_AMBIENT,@Color);
    Color:=ColorToGlColor($FFFFFFFF);
    glLightfv(GL_LIGHT0 + i,GL_DIFFUSE,@Color);
    Color:=ColorToGlColor($FFFFFFFF);
    glLightfv(GL_LIGHT0 + i,GL_SPECULAR,@Color);
    { all }
    case TD3Light(FCurrentLights[i]).LightType of
      d3LightDirectional:
        begin
          glPushMatrix;

          glMatrixMode(GL_MODELVIEW);
          glLoadMatrixf(@CurrentCameraMatrix);
          Dir:=d3VectorScale(TD3Light(FCurrentLights[i]).AbsoluteDirection,-1);
          Dir.W:=0;
          glLightfv(GL_LIGHT0 + i,GL_POSITION,@Dir);
          Dir.W:=1;
	  glLightfv(GL_LIGHT0 + i,GL_SPOT_DIRECTION,@Dir);

          glLightf(GL_LIGHT0 + i,GL_SPOT_CUTOFF,180.0);
	  glLightf(GL_LIGHT0 + i,GL_SPOT_EXPONENT,0.0);

          glPopMatrix;
        end;
      d3LightPoint:
        begin
          Dir:=TD3Light(FCurrentLights[i]).AbsolutePosition;
          glLightfv(GL_LIGHT0 + i,GL_POSITION,@Dir);
          glLightf(GL_LIGHT0 + i,GL_LINEAR_ATTENUATION,0.2);
{           Light.Range:=100.0;
          Light.Attenuation1:=0.2;}
        end;
      d3LightSpot:
        begin
          Dir:=TD3Light(FCurrentLights[i]).AbsolutePosition;
          glLightfv(GL_LIGHT0 + i,GL_POSITION,@Dir);
          Dir:=d3VectorScale(TD3Light(FCurrentLights[i]).AbsoluteDirection,1);
          glLightfv(GL_LIGHT0 + i,GL_SPOT_DIRECTION,@Dir);
          glLighti(GL_LIGHT0 + i,GL_SPOT_CUTOFF,80);
          glLightf(GL_LIGHT0 + i,GL_SPOT_EXPONENT,10);
{            Light.Theta       :=0.5;
          Light.Phi         :=1.0;
          Light.Falloff     :=1.0;
          Light.Attenuation0:=1.0;}
        end;
    end;
  end;
end;

function TD3CanvasOpenGL.BeginScene:boolean;
var
  Color,Dir:TD3Vector;
  i:integer;
  S:TD3RenderState;
begin
  Result:=false;
  if not Assigned(aglContext) then Exit;
  {$IFDEF UNIX}
  if aglPBuffer <> nil then
    glXMakeCurrent(CurXDisplay,GLXDrawable(aglPBuffer),aglContext)
  else
    glXMakeCurrent(CurXDisplay,CurXWindow,aglContext);
  {$ENDIF}
  {$IFDEF DARWIN}
  aglSetCurrentContext(aglContext);
  {$ENDIF}

  ApplyLights;
  { Render }
  Result:=true;
  VBLockPos:=0;
  IBLockPos:=0;
  { }
  glEnable(GL_DEPTH_TEST);
  { Common states }
  glShadeModel(GL_SMOOTH);
  { common states }
  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);
  { }
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  SetRenderState(rsBlendNormal);
  { }
  glDepthMask(1);
  glEnable(GL_TEXTURE_2D);

  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glDisable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT,GL_DIFFUSE);

  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);

  glEnable(GL_NORMALIZE);
end;

procedure TD3CanvasOpenGL.FlushBuffer;
begin
  if FLockable and (FBitmap <> nil) then
  begin
    glFlush();
    { Read RGB }
    {$IFDEF UNIX}
    glReadPixels(0,0,FWidth,FHeight,GL_BGRA_EXT,GL_UNSIGNED_BYTE,FBitmap.Bits);
    {$ENDIF}
    {$IFDEF DARWIN}
    glReadPixels(0,0,FWidth,FHeight,GL_RGBA,GL_UNSIGNED_BYTE,FBitmap.Bits);
    {$ENDIF}
    FBitmap.NeedUpdate:=true;
  end
  else
  if FLockable and (FBufferHandle <> nil) then
  begin
    glFlush();
    { Read RGB }
    glReadPixels(0,0,FWidth,FHeight,GL_RGBA,GL_UNSIGNED_BYTE,FBufferBits);
  end
  else
  if (aglContext <> nil) and not FLockable then
    {$IFDEF UNIX}
    glXSwapBuffers(CurXDisplay,CurXWindow);
    {$ENDIF}
    {$IFDEF DARWIN}
    aglSwapBuffers(aglContext);
    {$ENDIF}
end;

procedure TD3CanvasOpenGL.CreateRenderStateList(AState:TD3RenderState);
var
  i:integer;
  matProj,scaleMatrix,transMatrix,orthoProj:TD3Matrix;
begin
  if aglContext=nil then Exit;
  if FRenderStates[AState]=0 then
    FRenderStates[AState]:=glGenLists(1);
  glNewList(FRenderStates[AState],GL_COMPILE);

  { States }
  case AState of
    rs2DScene:
      begin
        glMatrixMode(GL_PROJECTION);
        matProj:=GetScreenMatrix;
        glLoadMatrixf(@matProj);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity;
      end;
    rs3DScene:
      begin
        { End Applet Scale }
        glMatrixMode(GL_PROJECTION);
        matProj:=GetProjectionMatrix;
        glLoadMatrixf(@matProj);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity;
      end;
    rsLightOn:
      begin
        glEnable(GL_LIGHTING);
      end;
    rsLightOff:
      begin
        glDisable(GL_LIGHTING);
      end;
    rsZTestOn:
      begin
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
      end;
    rsZTestOff:
      begin
        glDisable(GL_DEPTH_TEST);
      end;
    rsZWriteOn:
      begin
        glDepthMask(1);
      end;
    rsZWriteOff:
      begin
        glDepthMask(0);
      end;
    rsFrontFace:
      begin
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
      end;
    rsBackFace:
      begin
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
      end;
    rsAllFace:
      begin
        glDisable(GL_CULL_FACE);
      end;
    rsBlendAdditive:
      begin
//        aglContext.SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//        aglContext.SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
      end;
    rsBlendNormal:
      begin
//        aglContext.SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//        aglContext.SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
      end;
    { Tex stretch }
    rsTexNearest:
      begin
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      end;
    rsTexLinear:
      begin
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      end;
    { Tex modulation }
    rsTexReplace:
      begin
        glEnable(GL_TEXTURE_RECTANGLE_EXT);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
      end;
    rsTexModulate:
      begin
        glEnable(GL_TEXTURE_RECTANGLE_EXT);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      end;
    rsTexDisable:
      begin
        glDisable(GL_TEXTURE_RECTANGLE_EXT);
      end;
  end;
  glEndList;
end;

procedure TD3CanvasOpenGL.ApplyRenderState(AState:TD3RenderState);
var
  M:TD3Matrix;
begin
  if aglContext=nil then Exit;

  if FCurrentStates[rsFrame] then
  begin
    case AState of
      rsTexReplace:Exit;
      rsTexModulate:Exit;
    end;
  end;

  case AState of
    rsBlendAdditive,rsBlendNormal:
      begin
//        if FRenderAlphaPast then Exit;
      end;
  end;

  if FRenderStates[AState] <> 0 then
    glCallList(FRenderStates[AState]);

  case AState of
    rs2DScene:
      begin
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity;
      end;
    rs3DScene:
      begin
        // Camera
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(@FCurrentCameraMatrix);
      end;
    rsFlat:glShadeModel(GL_FLAT);
    rsGouraud:glShadeModel(GL_SMOOTH);
    rsBlendAdditive:
      begin
        if BlendSeparate and FLockable then
          glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_ONE,GL_ONE_MINUS_SRC_ALPHA)
        else
          glBlendFunc(GL_SRC_ALPHA,GL_ONE);
      end;
    rsBlendNormal:
      begin
        if BlendSeparate and FLockable then
          glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA)
        else
          glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      end;
    rsFrame:
      begin
        glPolygonMode(GL_FRONT,GL_LINE);
      end;
    rsSolid:
      begin
        glPolygonMode(GL_FRONT,GL_FILL);
      end;
  end;
end;

procedure TD3CanvasOpenGL.MaterialChanged(Sender:TObject);
var
  C:TD3Vector;
  M:TD3Matrix;
begin
  inherited;
  if not Assigned(aglContext) then Exit;
  if Self.Material <> nil then
  begin
    C:=ColorToGlColor(Self.Material.NativeDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,@C);
    C:=ColorToGlColor(Self.Material.NativeDiffuse);
    glColor4f(C.X,C.Y,C.Z,C.W);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,@C);
{    C:=ColorToGlColor(Self.Material.NativeSpecular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,@C);}
{    C:=ColorToGlColor($0);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,@C);}
//    glMateriali(GL_FRONT,GL_SHININESS,Trunc(DefaultPower * 128));
    UpdateBitmap(Self.Material.NativeBitmap);
    if (Self.Material.NativeBitmap <> nil) and (GetHandle(Self.Material.NativeBitmap) <> 0) then
    begin
      { texture matrix }
      if (Self.Material.BitmapTileX <> 1) or (Self.Material.BitmapTileY <> 1) or (not Self.Material.BitmapRect.empty) then
      begin
        M:=IdentityMatrix;
        M.m11:=Self.Material.BitmapTileX * Self.Material.NativeBitmap.Width;
        M.m22:=Self.Material.BitmapTileY * Self.Material.NativeBitmap.Height;
        if not Self.Material.BitmapRect.empty then
        begin
          M.m11:=(Self.Material.BitmapRect.Right - Self.Material.BitmapRect.Left);
          M.m22:=(Self.Material.BitmapRect.Bottom - Self.Material.BitmapRect.Top);
          M.m31:=Self.Material.BitmapRect.Left;
          M.m32:=Self.Material.BitmapRect.Top;
        end;
        glMatrixMode(GL_TEXTURE);
        glLoadMatrixf(@M);
      end
      else
      begin
        M:=IdentityMatrix;
        M.m11:=Self.Material.NativeBitmap.Width;
        M.m22:=Self.Material.NativeBitmap.Height;
        glMatrixMode(GL_TEXTURE);
        glLoadMatrixf(@M);
      end;
      { }

      if Self.Material.BitmapMode=d3TexReplace then
        SetRenderState(rsTexReplace)
      else
        SetRenderState(rsTexModulate);
      glBindTexture(GL_TEXTURE_RECTANGLE_EXT,GetHandle(Self.Material.NativeBitmap));
    end
    else
    begin
      SetRenderState(rsTexDisable);
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity;
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

procedure TD3CanvasOpenGL.SetMatrix(const M:TD3Matrix);
begin
  FCurrentMatrix:=M;
  if Assigned(aglContext) then
  begin
  end;
end;

procedure TD3CanvasOpenGL.DrawLine(const StartPoint,EndPoint:TD3Vector; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  A,B,n:TD3Vector;
  C:TD3Color;
  i:integer;
  Flags:cardinal;
begin
  if aglContext=nil then Exit;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);

  GetMem(Ver,2 * SizeOf(TD3ColorTexVertexNormal));
  GetMem(Id3,2 * 2);

  Ver[0]:=ColorTexVertexNormal(StartPoint.X,StartPoint.Y,StartPoint.Z,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),0,0);
  Ver[1]:=ColorTexVertexNormal(EndPoint.X,EndPoint.Y,EndPoint.Z,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),1,1);
  Id3[0]:=0;
  Id3[1]:=1;

  glEnableClientState(GL_VERTEX_ARRAY);

  glInterleavedArrays(GL_V3F,0,Ver);

  glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);

  glEnableClientState(GL_INDEX_ARRAY);
  glIndexPointer(GL_UNSIGNED_SHORT,0,Id3);
  glDrawElements(GL_LINES,2,GL_UNSIGNED_SHORT,Id3);

  FreeMem(Id3,2 * 2);
  FreeMem(Ver,2 * SizeOf(TD3ColorTexVertexNormal));

  glPopMatrix;
end;

procedure TD3CanvasOpenGL.DrawRect(const StartPoint,EndPoint:TD3Vector;
  const Opacity:single);
begin
  if aglContext=nil then Exit;
end;

procedure TD3CanvasOpenGL.DrawCube(const Center,Size:TD3Vector; const Opacity:single);
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

procedure TD3CanvasOpenGL.FillCube(const Center,Size:TD3Vector; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  tx1,ty1,tx2,ty2:single;
  A,B,n:TD3Vector;
  i:integer;
begin
  if aglContext=nil then Exit;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix;
    glMultMatrixf(@FCurrentMatrix);

    GetMem(Ver,24 * SizeOf(TD3ColorTexVertexNormal));
    GetMem(Id3,36 * 2);

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
    Ver[0]:=ColorTexVertexNormal(A.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1);
    Ver[1]:=ColorTexVertexNormal(B.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1);
    Ver[2]:=ColorTexVertexNormal(B.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2);
    Ver[3]:=ColorTexVertexNormal(A.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2);
    { right }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(B.X,A.Y,B.Z),d3Vector(B.X,B.Y,B.Z)),
      d3VectorSubtract(d3Vector(B.X,A.Y,B.Z),d3Vector(B.X,B.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver[4]:=ColorTexVertexNormal(B.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1);
    Ver[5]:=ColorTexVertexNormal(B.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1);
    Ver[6]:=ColorTexVertexNormal(B.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2);
    Ver[7]:=ColorTexVertexNormal(B.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2);
    { left }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(A.X,A.Y,B.Z)),
      d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(A.X,A.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver[8]:=ColorTexVertexNormal(A.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1);
    Ver[9]:=ColorTexVertexNormal(A.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1);
    Ver[10]:=ColorTexVertexNormal(A.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2);
    Ver[11]:=ColorTexVertexNormal(A.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2);
    { back }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(B.X,B.Y,B.Z),d3Vector(A.X,B.Y,B.Z)),
      d3VectorSubtract(d3Vector(B.X,B.Y,B.Z),d3Vector(A.X,B.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver[12]:=ColorTexVertexNormal(B.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1);
    Ver[13]:=ColorTexVertexNormal(A.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1);
    Ver[14]:=ColorTexVertexNormal(A.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2);
    Ver[15]:=ColorTexVertexNormal(B.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2);
    { top }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(B.X,B.Y,B.Z)),
      d3VectorSubtract(d3Vector(A.X,B.Y,B.Z),d3Vector(B.X,A.Y,B.Z)));
    n:=d3VectorScale(n,-1);
    Ver[16]:=ColorTexVertexNormal(A.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1);
    Ver[17]:=ColorTexVertexNormal(B.X,B.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1);
    Ver[18]:=ColorTexVertexNormal(B.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2);
    Ver[19]:=ColorTexVertexNormal(A.X,A.Y,B.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2);
    { bottom }
    n:=d3VectorCrossProduct(d3VectorSubtract(d3Vector(A.X,A.Y,A.Z),d3Vector(B.X,A.Y,A.Z)),
      d3VectorSubtract(d3Vector(A.X,A.Y,A.Z),d3Vector(B.X,B.Y,A.Z)));
    n:=d3VectorScale(n,-1);
    Ver[20]:=ColorTexVertexNormal(A.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1);
    Ver[21]:=ColorTexVertexNormal(B.X,A.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1);
    Ver[22]:=ColorTexVertexNormal(B.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2);
    Ver[23]:=ColorTexVertexNormal(A.X,B.Y,A.Z,n.x,n.y,n.z,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2);

      for i:=0 to 5 do
      begin
        Id3[i * 6 + 0]:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 4) + 0;
        Id3[i * 6 + 1]:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 4) + 1;
        Id3[i * 6 + 2]:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 4) + 3;
        Id3[i * 6 + 3]:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 4) + 3;
        Id3[i * 6 + 4]:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 4) + 1;
        Id3[i * 6 + 5]:={(VBLockPos div SizeOf(TD3ColorTexVertexNormal)) + }(i * 4) + 2;
      end;

          glEnableClientState(GL_VERTEX_ARRAY);
          glEnableClientState(GL_NORMAL_ARRAY);
//          glnableClientState(GL_COLOR_ARRAY);
          glEnableClientState(GL_TEXTURE_COORD_ARRAY);

          glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);

          glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
//          glColorPointer(4,GL_UNSIGNED_BYTE,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
          glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
          glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));

          glEnableClientState(GL_INDEX_ARRAY);
          glIndexPointer(GL_UNSIGNED_SHORT,0,Id3);
          glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,Id3);

  FreeMem(Id3,36 * 2);
  FreeMem(Ver,24 * SizeOf(TD3ColorTexVertexNormal));

  glPopMatrix;
end;

procedure TD3CanvasOpenGL.FillMesh(const Center,Size:TD3Vector;
  const MeshData:TD3MeshData; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  A,B,n:TD3Vector;
  C:TD3Color;
  i:integer;
  Flags:cardinal;
begin
  if aglContext=nil then Exit;
  if Length(MeshData.MeshVertices)=0 then Exit;
  if Length(MeshData.MeshIndices)=0 then Exit;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);

  GetMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
  GetMem(Id3,Length(MeshData.MeshIndices) * 2);

  for i:=0 to Length(MeshData.MeshVertices) - 1 do
    with MeshData.MeshVertices[i] do
    begin
      if FCurrentStates[rs2DScene] then
        Ver[i]:=ColorTexVertexNormal(trunc(x * Size.X + Center.X),trunc(y * Size.Y + Center.Y),z * Size.Z,nx,ny,nz,C,tu,tv)
      else
        Ver[i]:=ColorTexVertexNormal(x * Size.X,y * Size.Y,z * Size.Z,nx,ny,nz,C,tu,tv);
    end;
  for i:=0 to Length(MeshData.MeshIndices) - 1 do
  begin
    Id3[i]:=MeshData.MeshIndices[i];
  end;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
//          glnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);
  glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
//          glColorPointer(4,GL_UNSIGNED_BYTE,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
  glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
  glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));

  glEnableClientState(GL_INDEX_ARRAY);
  glIndexPointer(GL_UNSIGNED_SHORT,0,Id3);
  glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);

  FreeMem(Id3,Length(MeshData.MeshIndices) * 2);
  FreeMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));

  glPopMatrix;
end;

procedure TD3CanvasOpenGL.FillPolygon(const Center,Size:TD3Vector;
  const Rect:TD2Rect; const Points:TD2Polygon; const Opacity:single;
  Front:boolean=true; Back:boolean=true; Left:boolean=true);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  A,B,n,V1,V2:TD3Vector;
  C:TD3Color;
  i,j:integer;
  Flags:cardinal;
  MeshData:TD3MeshData;
  startIndex:integer;
  leftLen,curPos:single;
begin
  if aglContext=nil then Exit;
  if Length(Points)=0 then Exit;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);
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

    GetMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
    GetMem(Id3,Length(MeshData.MeshIndices) * 2);

    for i:=0 to Length(MeshData.MeshVertices) - 1 do
      with MeshData.MeshVertices[i] do
      begin
        if FCurrentStates[rs2DScene] then
          Ver[i]:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(x * Size.X),Center.Y - (Size.Y / 2) + round(y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv)
        else
          Ver[i]:=ColorTexVertexNormal(Center.X - (Size.X / 2) + (x * Size.X),Center.Y + (Size.Y / 2) - (y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv);
      end;
    for i:=0 to Length(MeshData.MeshIndices) - 1 do
    begin
      Id3[i]:=MeshData.MeshIndices[i];
    end;

    { write to stencil }
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS,0,$FF);
    glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);

    glEnableClientState(GL_INDEX_ARRAY);
    glIndexPointer(GL_UNSIGNED_SHORT,0,Id3);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);
    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));

  //  glDisable(GL_CULL_FACE);
    glColorMask(0,0,0,0);
    glDepthMask(0);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);
    glColorMask(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
  //  glEnable(GL_CULL_FACE);

    FreeMem(Id3,Length(MeshData.MeshIndices) * 2);
    FreeMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));

    GetMem(Ver,4 * SizeOf(TD3ColorTexVertexNormal));
    GetMem(Id3,6 * 2);
    if FCurrentStates[rs2DScene] then
    begin
  {    Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,0); System.Inc(Ver);
      Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,0); System.Inc(Ver);
      Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,1); System.Inc(Ver);
      Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,1); System.Inc(Ver);}
    end
    else
    begin
      Ver[0]:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,0,1);
      Ver[1]:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,1,1);
      Ver[2]:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,1,0);
      Ver[3]:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z + (Size.Z / 2),0,0,1,C,0,0);
    end;
    { indexs }
    Id3[0]:=0;
    Id3[1]:=3;
    Id3[2]:=1;
    Id3[3]:=1;
    Id3[4]:=3;
    Id3[5]:=2;

    glStencilFunc(GL_NOTEQUAL,0,$FF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);
    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,Id3);
    glDisable(GL_STENCIL_TEST);
    FreeMem(Ver,4 * SizeOf(TD3ColorTexVertexNormal));
    FreeMem(Id3,6 * 2);
  end;

  if Back then
  begin
    { 3. back }
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

    GetMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
    GetMem(Id3,Length(MeshData.MeshIndices) * 2);

    for i:=0 to Length(MeshData.MeshVertices) - 1 do
      with MeshData.MeshVertices[i] do
      begin
        if FCurrentStates[rs2DScene] then
          Ver[i]:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(x * Size.X),Center.Y - (Size.Y / 2) + round(y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv)
        else
          Ver[i]:=ColorTexVertexNormal(Center.X - (Size.X / 2) + (x * Size.X),Center.Y + (Size.Y / 2) - (y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv);
      end;
    for i:=0 to Length(MeshData.MeshIndices) - 1 do
    begin
      Id3[i]:=MeshData.MeshIndices[i];
    end;

    { write to stencil }
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS,0,$FF);
    glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);

    glEnableClientState(GL_INDEX_ARRAY);
    glIndexPointer(GL_UNSIGNED_SHORT,0,Id3);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);
    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));

  //  glDisable(GL_CULL_FACE);
    glColorMask(0,0,0,0);
    glDepthMask(0);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);
    glColorMask(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
  //  glEnable(GL_CULL_FACE);

    FreeMem(Id3,Length(MeshData.MeshIndices) * 2);
    FreeMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));

    GetMem(Ver,4 * SizeOf(TD3ColorTexVertexNormal));
    GetMem(Id3,6 * 2);
    if FCurrentStates[rs2DScene] then
    begin
  {    Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,0); System.Inc(Ver);
      Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(A.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,0); System.Inc(Ver);
      Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(B.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,1,1); System.Inc(Ver);
      Ver^:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(A.X * Size.X) + 0.5,Center.Y - (Size.Y / 2) + round(B.Y * Size.Y) + 0.5,Size.Z,0,0,1,C,0,1); System.Inc(Ver);}
    end
    else
    begin
      Ver[0]:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,0,1);
      Ver[1]:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y - (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,1,1);
      Ver[2]:=ColorTexVertexNormal(Center.X + (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,1,0);
      Ver[3]:=ColorTexVertexNormal(Center.X - (Size.X / 2),Center.Y + (Size.Y / 2),Center.Z - (Size.Z / 2),0,0,-1,C,0,0);
    end;
    { indexs }
    Id3[0]:=0;
    Id3[1]:=1;
    Id3[2]:=3;
    Id3[3]:=1;
    Id3[4]:=2;
    Id3[5]:=3;

    glStencilFunc(GL_NOTEQUAL,0,$FF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);
    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,Id3);
    glDisable(GL_STENCIL_TEST);
    FreeMem(Ver,4 * SizeOf(TD3ColorTexVertexNormal));
    FreeMem(Id3,6 * 2);
  end;

  if Left and (leftLen > 0) then
  begin
    { 1. sides }
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
{        tu:=x;
        tv:=y;}
      end;
      with MeshData.MeshVertices[Length(Points) + i] do
      begin
        x:=(Points[i].x - A.X) / Abs(B.X - A.X);
        y:=1 - ((Points[i].y - A.Y) / Abs(B.Y - A.Y));
        z:=0;
        tv:=1;
        tu:=curPos / leftLen;
{        tu:=x;
        tv:=y;}
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

    GetMem(Id3,Length(MeshData.MeshIndices) * 2);
    GetMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
    C:=d3Opacity(Material.NativeDiffuse,Opacity);
    for i:=0 to Length(MeshData.MeshVertices) - 1 do
      with MeshData.MeshVertices[i] do
      begin
        if FCurrentStates[rs2DScene] then
          Ver[i]:=ColorTexVertexNormal(Center.X - (Size.X / 2) + round(x * Size.X),Center.Y - (Size.Y / 2) + round(y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv)
        else
          Ver[i]:=ColorTexVertexNormal(Center.X - (Size.X / 2) + (x * Size.X),Center.Y - (Size.Y / 2) + (y * Size.Y),Center.Z - (Size.Z / 2) + z * Size.Z,nx,ny,nz,C,tu,tv);
      end;
    { indexs }
    for i:=0 to Length(MeshData.MeshIndices) - 1 do
    begin
      Id3[i]:=MeshData.MeshIndices[i];
    end;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);
    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));
  //  glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);
  //  glEnable(GL_CULL_FACE);
    FreeMem(Id3,Length(MeshData.MeshIndices) * 2);
    FreeMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
  end;
  { free }
  MeshData.Free;
  glPopMatrix;
end;

procedure TD3CanvasOpenGL.UpdateBitmap(Bitmap:TD3Bitmap);
var
  W,H:integer;
  maxV:integer;
  Tex:cardinal;
begin
  inherited ;
  if Bitmap=nil then Exit;
  if Bitmap.Width * Bitmap.Height=0 then
  begin
    Bitmap.NeedUpdate:=false;
    Exit;
  end;
  if not Assigned(aglContext) then Exit;
  { create - if need }
  if GetHandle(Bitmap)=0 then
  begin
    glGenTextures(1,@Tex);
    AddHandle(Bitmap,Tex);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT,Tex);
    glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_GENERATE_MIPMAP_SGIS,GL_FALSE);
    glTexImage2D(GL_TEXTURE_RECTANGLE_EXT,0,GL_RGBA,Bitmap.Width,Bitmap.Height,0,
      {$IFDEF DARWIN}GL_RGBA{$ELSE}GL_BGRA_EXT{$ENDIF},GL_UNSIGNED_INT_8_8_8_8_REV,Bitmap.Bits);
  end
  else
  begin
    Tex:=GetHandle(Bitmap);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT,Tex);
    if Bitmap.NeedUpdate then
    begin
      glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_GENERATE_MIPMAP_SGIS,GL_FALSE);
      glTexImage2D(GL_TEXTURE_RECTANGLE_EXT,0,GL_RGBA,Bitmap.Width,Bitmap.Height,0,
        {$IFDEF DARWIN}GL_RGBA{$ELSE}GL_BGRA_EXT{$ENDIF},GL_UNSIGNED_INT_8_8_8_8_REV,Bitmap.Bits);
    end;
  end;
  Bitmap.NeedUpdate:=false;
end;

procedure TD3CanvasOpenGL.DestroyBitmap(const Bitmap:TD3Bitmap);
var
  Tex:cardinal;
begin
  if (Bitmap <> nil) then
    Bitmap.CanvasList.Remove(Self);
  if (Bitmap <> nil) then
  begin
    {$IFDEF DARWIN}
    Tex:=GetHandle(Bitmap);
    if Tex <> 0 then
      glDeleteTextures(1,@Tex);
    {$ENDIF}
    {$IFDEF UNIX}
    Tex:=GetHandle(Bitmap);
    if Tex <> 0 then
      glDeleteTextures(1,@Tex);
    {$ENDIF}
    RemoveHandle(Bitmap);
  end;
end;

procedure TD3CanvasOpenGL.DrawText(const AX,AY:single; const AText:String; const Opacity:single);
var
  charCount,i,verI,id3I:integer;
  fStartX:single;
  pVertices:PD3ColorTexVertexNormalArray;
  pIndexs:PWordArray;
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
    x:=Round(FCurrentMatrix.m41 + x) - FCurrentMatrix.m41 - 0.5;
    y:=Round(FCurrentMatrix.m42 + y) - FCurrentMatrix.m42 - 0.5;
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
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);

  GetMem(pVertices,charCount * 4 * SizeOf(TD3ColorTexVertexNormal));
  GetMem(pIndexs,charCount * 6 * 2);

  { Draw }
  Vertex:=0;
  verI:=0;
  id3I:=0;
  for i:=1 to Length(AText) do
  begin
    c:=Word(AText[i]);
    if c < 32 then Continue;
    if ((c - 32) < 0) or ((c - 32) > Length(CurFont.CharCoords)) then Continue;

    tx1:=CurFont.CharCoords[c-32].Coord[0]{ * CurFont.Bitmap.Width};
    ty1:=CurFont.CharCoords[c-32].Coord[1]{ * CurFont.Bitmap.Height};
    tx2:=CurFont.CharCoords[c-32].Coord[2]{ * CurFont.Bitmap.Width};
    ty2:=CurFont.CharCoords[c-32].Coord[3]{ * CurFont.Bitmap.Height};

    w:=(tx2-tx1);
    h:=(ty2-ty1);

    if (c <> 32) then
    begin
      x:=x + CurFont.CharCoords[c-32].a;

      if FCurrentStates[rs2DScene] then
      begin
        pVertices[verI]:=ColorTexVertexNormal(x + 0,y + 0,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(verI);
        pVertices[verI]:=ColorTexVertexNormal(x + w,y + 0,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(verI);
        pVertices[verI]:=ColorTexVertexNormal(x + w,y + h,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(verI);
        pVertices[verI]:=ColorTexVertexNormal(x + 0,y + h,0,0,0,0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(verI);
      end
      else
      begin
        scale:=0.02;
        pVertices[verI]:=ColorTexVertexNormal((x + 0) * scale,0,(y + h) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty1); System.Inc(verI);
        pVertices[verI]:=ColorTexVertexNormal((x + w) * scale,0,(y + h) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty1); System.Inc(verI);
        pVertices[verI]:=ColorTexVertexNormal((x + w) * scale,0,(y + 0) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx2,ty2); System.Inc(verI);
        pVertices[verI]:=ColorTexVertexNormal((x + 0) * scale,0,(y + 0) * scale,0,0,0.0,d3Opacity(Material.NativeDiffuse,Opacity),tx1,ty2); System.Inc(verI);
      end;

      pIndexs[id3I]:=Vertex + 0; System.Inc(id3I);
      pIndexs[id3I]:=Vertex + 1; System.Inc(id3I);
      pIndexs[id3I]:=Vertex + 3; System.Inc(id3I);
      pIndexs[id3I]:=Vertex + 3; System.Inc(id3I);
      pIndexs[id3I]:=Vertex + 1; System.Inc(id3I);
      pIndexs[id3I]:=Vertex + 2; System.Inc(id3I);
      x:=x + CurFont.CharCoords[c-32].c;
      Vertex:=Vertex + 4;
    end;
    x:=x + (w);
  end;

  UpdateBitmap(CurFont.Bitmap);
  if GetHandle(CurFont.Bitmap) <> 0 then
  begin
    SetRenderState(rsTexModulate);
//    SetRenderState(rsTexNearest);

    glBindTexture(GL_TEXTURE_RECTANGLE_EXT,GetHandle(CurFont.Bitmap));

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glInterleavedArrays(GL_T2F_N3F_V3F,0,pVertices);

    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),pVertices);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(pVertices) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(pVertices) + ((3 + 3 + 1) * 4)));

{    glMatrixMode(GL_TEXTURE);
    glLoadIdentity;}

    glEnableClientState(GL_INDEX_ARRAY);
    glIndexPointer(GL_UNSIGNED_SHORT,0,pIndexs);
    glDrawElements(GL_TRIANGLES,charCount * 6,GL_UNSIGNED_SHORT,pIndexs);

{    aglContext.SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
      aglContext.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(VBLockPos div SizeOf(TD3ColorTexVertexNormal)),
        charCount * 4,IBLockPos div 2,(charCount * 6) div 3);}
    end;

  FreeMem(pIndexs,charCount * 6 * 2);
  FreeMem(pVertices,charCount * 4 * SizeOf(TD3ColorTexVertexNormal));

  glPopMatrix;
end;

function TD3CanvasOpenGL.MeasureText(const AText:String):TD3Point;
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
  fRowHeight:= (CurFont.CharCoords[0].Coord[3] - CurFont.CharCoords[0].Coord[1]){ * CurFont.Bitmap.Height};
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

    fRowWidth:=fRowWidth + CurFont.CharCoords[c - 32].a + ((tx2-tx1){ * CurFont.Bitmap.Width}) + CurFont.CharCoords[c - 32].c;
    if (fRowWidth > fWidth) then fWidth:=fRowWidth;

{    if i <= Length(AText) then
      CalcStrLen:=i;}
  end;

  Result.x:=fWidth;
  Result.y:=fHeight;
end;

function TD3CanvasOpenGL.UpdateFont(const AText:AnsiString; Font:TD3Font):integer;
{$IFDEF DARWIN}
var
  x,y:Longword;
  c:word;
  pBitmapBits:PD3ColorArray;
  pDstRow:PD3ColorArray;
  Calc:boolean;
  Pix:Longword;
  CtxRef:CGContextRef;
  R:CGRect;
  P:CGPoint;
  Values:array [0..10] of ATSUAttributeValuePtr;
  Tags:array [0..10] of ATSUAttributeTag;
  Sizes:array [0..10] of ByteCount;
  myStyles:ATSUStyle;
  myTextLayout:ATSUTextLayout;
  myFont:ATSUFontID;
  fallbacks:ATSUFontFallbacks;
  atsuSize:Fixed;
  atsuBold:boolean;
  W:String;
  BoundsCount:ItemCount;
  BoundsRects:ATSTrapezoid;
  CLeft,CRight,CTop,CBottom:Fixed;
  LHeight,LBottom:integer;
  NewFont:TD3InternalFont;
  FTexWidth,FTexHeight:integer;
  S:string;
  i,j:integer;
  NewFontId3:integer;
  NeedUpdate:boolean;
begin
  Result:=-1;
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
  FTexWidth:=64;
  FTexHeight:=64;
  { Create ATSU }
  // style
  ATSUCreateStyle(myStyles);
  atsuSize:=Round(Font.Size * $FFFF);
  Tags[0]:=kATSUSizeTag;
  Sizes[0]:=sizeof(Fixed);
  Values[0]:=@atsuSize;
  atsuBold:=Font.Style in [d3FontBold,d3FontBoldItalic];
  Tags[1]:=kATSUQDBoldfaceTag;
  Sizes[1]:=sizeof(boolean);
  Values[1]:=@atsuBold;
  atsuBold:=Font.Style in [d3FontItalic,d3FontBoldItalic];
  Tags[2]:=kATSUQDItalicTag;
  Sizes[2]:=sizeof(boolean);
  Values[2]:=@atsuBold;
  ATSUSetAttributes(myStyles,3,Tags,Sizes,Values);
  // font
  S:=Font.Family;
  ATSUFindFontFromName(Pointer(@S[1]),Length(Font.Family),kFontFullName,kFontNoPlatform,kFontNoScript,kFontNoLanguage,myFont);
  Tags[0]:=kATSUFontTag;
  Sizes[0]:=sizeof(ATSUFontId);
  Values[0]:=@myFont;
  ATSUSetAttributes(myStyles,1,Tags,Sizes,Values);
  // layout
  myTextLayout:=nil;
  ATSUCreateTextLayout(myTextLayout);
  // fallback
  ATSUCreateFontFallbacks(fallbacks);
  ATSUSetObjFontFallbacks(fallbacks,0,nil,kATSUDefaultFontFallbacks);
  Tags[0]:=kATSULineFontFallbacksTag;
  Sizes[0]:=SizeOf(ATSUFontFallbacks);
  Values[0]:=@fallbacks;
  ATSUSetLayoutControls(myTextLayout,1,Tags,Sizes,Values);
  // set context
  Tags[0]:=kATSUCGContextTag;
  Sizes[0]:=SizeOf(CGContextRef);
  Values[0]:=@CtxRef;
  ATSUSetLayoutControls(myTextLayout,1,Tags,Sizes,Values);
  // Create temp context
  System.GetMem(pBitmapBits,1 * 1 * 4);
  CtxRef:=CGBitmapContextCreate(pBitmapBits,1,1,8,1 * 4,CGColorSpaceCreateDeviceRGB,kCGImageAlphaPremultipliedLast);
  // calculation
  Calc:=true;
  LHeight:=Trunc(Font.Size);
  while Calc do
  begin
    Calc:=false;
    x:=0;
    y:=0;
    for c:=0 to High(NewFont.CharCoords) do
    begin
      if NewFont.CharCoords[c].Exists=0 then Continue;
      // set text to layout
      W:=System.WideChar(32 + c);
      ATSUSetTextPointerLocation(myTextLayout,ConstUniCharArrayPtr(PWideChar(W)),
          kATSUFromTextBeginning,kATSUToTextEnd,Length(W));
      ATSUSetRunStyle(myTextLayout,myStyles,kATSUFromTextBeginning,
        kATSUToTextEnd);
      ATSUSetTransientFontMatching(myTextLayout,true);
      // calc size
      CLeft:=0;
      CTop:=0;
      Cright:=0;
      CBottom:=0;
      ATSUGetUnjustifiedBounds(myTextLayout,0,1,CLeft,CRight,CTop,CBottom);
      P.X:=(CRight) / $FFFF + 2;

      if (x + P.X + 1 > FTexWidth) then
      begin
        x:=0;
        y:=y + (LHeight + 2);
      end;
      x:=x + Round(P.X);
      if y > FTexHeight then
      begin
        FTexHeight:=FTexHeight * 2;
        FTexWidth:=FTexHeight;
        Calc:=true;
        Break;
      end;
    end;
    if FTexHeight > MaxBitmapSize then
    begin
      Exit;
    end;
  end;
  CGContextRelease(CtxRef);
  System.FreeMem(pBitmapBits,1 * 1 * 4);

  // Create a new texture for the font
  if NewFont.Bitmap <> nil then
    NewFont.Bitmap.Free;
  NewFont.Bitmap:=TD3Bitmap.Create(FTexWidth,FTexHeight);
  NewFont.Bitmap.LockBitmapBits(pDstRow,true);
  FillChar(pDstRow^,FTexWidth * FTexHeight * 4,0);
  CtxRef:=CGBitmapContextCreate(pDstRow,FTexWidth,FTexHeight,8,
    FTexWidth * 4,CGColorSpaceCreateDeviceRGB,kCGImageAlphaPremultipliedLast);
  // set context
  Tags[0]:=kATSUCGContextTag;
  Sizes[0]:=SizeOf(CGContextRef);
  Values[0]:=@CtxRef;
  ATSUSetLayoutControls(myTextLayout,1,Tags,Sizes,Values);
  // set color
  CGContextSetRGBFillColor(CtxRef,0,0,0,1);
  CGContextFillRect(CtxRef,CGRectFromRect(d2Rect(-1,-1,10000,10000)));
  // paint and calc texcoord
  x:=1;
  y:=1;
  LBottom:=Round(Font.Size / 4);
  for c:=0 to High(NewFont.CharCoords) do
  begin
    if NewFont.CharCoords[c].Exists <> 0 then
    begin
      W:=System.WideChar(32 + c);
      ATSUSetTextPointerLocation(myTextLayout,ConstUniCharArrayPtr(PWideChar(W)),
          kATSUFromTextBeginning,kATSUToTextEnd,Length(W));
      ATSUSetRunStyle(myTextLayout,myStyles,
        kATSUFromTextBeginning,kATSUToTextEnd);
      ATSUSetTransientFontMatching(myTextLayout,true);
      // calc size
      CLeft:=0;
      CTop:=0;
      Cright:=0;
      CBottom:=0;
      ATSUGetUnjustifiedBounds(myTextLayout,0,1,CLeft,CRight,CTop,CBottom);
      P.X:=Round((CRight) / $FFFF);
      P.Y:=LHeight;

      if (x + (P.x + 2) > FTexWidth) then
      begin
        x:=1;
        y:=y + (LHeight + 1);
      end;
      // draw bounds rect
{      CGContextSetRGBStrokeColor(CtxRef,1,0,0,1);
      CGContextStrokeRect(CtxRef,CGRectFromRect(d2Rect(x - 0.5,FTexHeight - P.Y - y - 0.5,x + Round(P.X) + 0.5,FTexHeight - y + 0.5)));}
      CGContextSetRGBFillColor(CtxRef,1,1,1,1);
      ATSUDrawText(myTextLayout,0,1,Round((x + 0) * $FFFF),Round((FTexHeight - (y + 1.5) - LHeight{ + LBottom}) * $FFFF + CBottom));

      // set coords
      NewFont.CharCoords[c].Coord[0]:=((x)){ / FTexWidth};
      NewFont.CharCoords[c].Coord[1]:=((y)){ / FTexHeight};
      NewFont.CharCoords[c].Coord[2]:=((x + P.X)){ / FTexWidth};
      NewFont.CharCoords[c].Coord[3]:=((y + P.Y)){ / FTexHeight};

      x:=x + (Round(P.X) + 1);
    end;
  end;

  // Free resource
  ATSUDisposeStyle(myStyles);
  ATSUDisposeTextLayout(myTextLayout);
  ATSUDisposeFontFallbacks(fallbacks);
  CGContextRelease(CtxRef);
  // Write the alpha values for the set pixels
  for x:=0 to FTexHeight - 1 do
  begin
    for y:=0 to FTexWidth - 1 do
    begin
      if pDstRow[y + (x * FTexWidth)] > 0 then
      begin
        CLeft:=TD3ColorRec(pDstRow[y + (x * FTexWidth)]).R;
        if CLeft > 0 then
        begin
          pDstRow[y + (x * FTexWidth)]:=$FFFFFF;
          TD3ColorRec(pDstRow[y + (x * FTexWidth)]).A:=CLeft;
        end
        else
          pDstRow[y + (x * FTexWidth)]:=0;
      end
      else
        pDstRow[y + (x * FTexWidth)]:=0;
    end;
  end;
  // Done updating texture,so clean up used objects
  NewFont.Bitmap.UnlockBitmapBits();

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
{$ENDIF}
{$IFDEF UNIX}
begin
end;
{$ENDIF}


