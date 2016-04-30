{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

type

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

  { TD3MesaCanvas }

  TD3MesaCanvas=class(TD3Canvas)
  private
    FFonts:array of TD3InternalFont;
    Context:cardinal;
    VBLockPos,IBLockPos:integer;
    AllowSample:boolean;
    FCurrentInvMatrix:TD3Matrix;
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
   
    function BeginScene:boolean;  override;
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
    function MeasureText(const AText:WideString):TD3Point;  override;
   
    procedure SetMatrix(const M:TD3Matrix);  override;
  published
  end;

function ColorToGlColor(AColor:cardinal):TD3Vector;
begin
  Result.X:=TD3ColorRec(AColor).B / $FF;
  Result.Y:=TD3ColorRec(AColor).G / $FF;
  Result.Z:=TD3ColorRec(AColor).R / $FF;
  Result.W:=TD3ColorRec(AColor).A / $FF;
end;

{ TD3MesaCanvas }

constructor TD3MesaCanvas.Create(const AWnd:cardinal; const AWidth,AHeight:integer; const ALockable:boolean; const Quality:TD3Quality);
var
  St:TD3RenderState;
begin
  FBuffered:=true;
  inherited;
  Context:=OSMesaCreateContext(OSMESA_RGBA,nil);
  if Context=0 then Exit;

  if not OSMesaMakeCurrent(Context,FBufferBits,GL_UNSIGNED_BYTE,AWidth,AHeight) then Exit;

  ReadImplementationProperties;

  glGetIntegerv(GL_MAX_TEXTURE_SIZE,@MaxBitmapSize);

  {$ifdef WINDOWS}
  OSMesaPixelStore(OSMESA_Y_UP,0);
  {$endif}

  { states }
  for St:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(St);
end;

constructor TD3MesaCanvas.CreateFromBitmap(const ABitmap:TD3Bitmap);
var
  St:TD3RenderState;
begin
  inherited;
  Context:=OSMesaCreateContext(OSMESA_RGBA,nil);
  if Context=0 then Exit;

  if not OSMesaMakeCurrent(Context,FBitmap.Bits,GL_UNSIGNED_BYTE,FWidth,FHeight) then Exit;

  ReadImplementationProperties;

  glGetIntegerv(GL_MAX_TEXTURE_SIZE,@MaxBitmapSize);

  {$ifdef WINDOWS}
  OSMesaPixelStore(OSMESA_Y_UP,0);
  {$endif}

  { states }
  for St:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(St);
end;

procedure TD3MesaCanvas.CopyBits(const Bits:PD3ColorArray);
begin
  if (FBitmap <> nil) then
  begin
    d2MoveLongword(FBitmap.Bits,Bits,FWidth * FHeight);
  end
  else
  begin
    glReadPixels(0,0,FWidth,FHeight,GL_RGBA,GL_UNSIGNED_BYTE,Bits);
  end
end;

procedure TD3MesaCanvas.ResizeBuffer(const AWidth,AHeight:integer);
var
  S:TD3RenderState;
begin
  inherited ;
  if FBitmap <> nil then
  begin
    OSMesaMakeCurrent(Context,FBitmap.Bits,GL_UNSIGNED_BYTE,AWidth,AHeight);
    glViewport(0,0,AWidth,AHeight);
  end
  else
  if Context <> 0 then
  begin
    OSMesaMakeCurrent(Context,FBufferBits,GL_UNSIGNED_BYTE,AWidth,AHeight);
    glViewport(0,0,AWidth,AHeight);
  end;
  // states
  for S:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(S);
end;

procedure TD3MesaCanvas.SetQuality(const Quality:TD3Quality);
var
  S:TD3RenderState;
begin
  inherited ;
  // states
  for S:=TD3RenderState(0) to rsGouraud do
    CreateRenderStateList(S);
end;

destructor TD3MesaCanvas.Destroy;
var
  i:integer;
begin
  for i:=0 to High(FFonts) do
    if FFonts[i].Bitmap <> nil then
      FFonts[i].Bitmap.Free;
  inherited;
end;

procedure TD3MesaCanvas.FreeBuffer;
begin
  OSMesaDestroyContext(Context);
  Context:=0;
  inherited;
end;

procedure TD3MesaCanvas.Clear(const AColor:TD3Color);
begin
  glClearDepth(1.0);
  glClearStencil(0);
  with ColorToGlColor(AColor) do
    glClearColor(X,Y,Z,W);
  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT);
end;
  
procedure TD3MesaCanvas.ClearARGB(const AWidth,AHeight:integer;
  const Bits:PD3ColorArray);
begin
end;

procedure TD3MesaCanvas.ApplyLights;
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

function TD3MesaCanvas.BeginScene:boolean;
var
  M:TD3Matrix;
begin
  Result:=false;
  if not (Context <> 0) then Exit;

  if FBitmap <> nil then
    OSMesaMakeCurrent(Context,FBitmap.Bits,GL_UNSIGNED_BYTE,FWidth,FHeight)
  else
    OSMesaMakeCurrent(Context,FBufferBits,GL_UNSIGNED_BYTE,FWidth,FHeight);

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
  glDepthMask(true);
  glEnable(GL_TEXTURE_2D);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glDisable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT,GL_DIFFUSE);

  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);

  glEnable(GL_NORMALIZE);

end;

procedure TD3MesaCanvas.FlushBuffer;
var
  DC:cardinal;
  B:TBitmap;
  i:integer;
begin
  if FLockable and (FBitmap <> nil) then
  begin
    glFlush();
    FBitmap.NeedUpdate:=true;
  end
  else
  if FLockable {and (FBufferHandle <> nil) }then
  begin
    glFlush();
  end
  else
  if (Context <> 0) and not FLockable then
  begin
    {$ifdef WINDOWS}
    DC:= GetDC(FWnd);
    BitBlt(DC,0,0,FWidth,FHeight,FBufferDC,0,0,SRCCOPY);
    ReleaseDC(FWnd,DC);
    {$ENDIF}
  end;
end;

procedure TD3MesaCanvas.CreateRenderStateList(AState:TD3RenderState);
var
  i:integer;
  matProj,scaleMatrix,transMatrix,orthoProj:TD3Matrix;
begin
  if Context=0 then Exit;
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
        glDepthMask(true);
      end;
    rsZWriteOff:
      begin
        glDepthMask(false);
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
      end;
    rsBlendNormal:
      begin
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

procedure TD3MesaCanvas.ApplyRenderState(AState:TD3RenderState);
var
  M:TD3Matrix;
begin
  if Context=0 then Exit;

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
        if GL_EXT_blend_func_separate and (FLockable) then
          glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_ONE,GL_ONE_MINUS_SRC_ALPHA)
        else
          glBlendFunc(GL_SRC_ALPHA,GL_ONE);
      end;
    rsBlendNormal:
      begin
        if GL_EXT_blend_func_separate and FLockable then
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

procedure TD3MesaCanvas.MaterialChanged(Sender:TObject);
var
  C:TD3Vector;
  M:TD3Matrix;
begin
  inherited;
  if not (Context <> 0) then Exit;
  if Self.Material <> nil then
  begin
    C:=ColorToGlColor(Self.Material.NativeDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,@C);
    C:=ColorToGlColor(Self.Material.NativeDiffuse);
    glColor4f(C.X,C.Y,C.Z,C.W);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,@C);
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
          M.m41:=Self.Material.BitmapRect.Left;
          M.m42:=Self.Material.BitmapRect.Top;
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

procedure TD3MesaCanvas.SetMatrix(const M:TD3Matrix);
begin
  FCurrentMatrix:=M;
  FCurrentInvMatrix:=M;
  InvertMatrix(FCurrentInvMatrix);
  if (Context <> 0) then
  begin
  end;
end;

procedure TD3MesaCanvas.DrawLine(const StartPoint,EndPoint:TD3Vector; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  A,B,n:TD3Vector;
  C:TD3Color;
  i:integer;
  Flags:cardinal;
begin
  if Context=0 then Exit;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);
//  ApplyLights;

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

procedure TD3MesaCanvas.DrawRect(const StartPoint,EndPoint:TD3Vector;
  const Opacity:single);
begin
  if Context=0 then Exit;
end;

procedure TD3MesaCanvas.DrawCube(const Center,Size:TD3Vector; const Opacity:single);
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

procedure TD3MesaCanvas.FillCube(const Center,Size:TD3Vector; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  tx1,ty1,tx2,ty2:single;
  A,B,n:TD3Vector;
  i:integer;
begin
  if Context=0 then Exit;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix;
    glMultMatrixf(@FCurrentMatrix);
//    ApplyLights;

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
          glEnableClientState(GL_TEXTURE_COORD_ARRAY);

          glInterleavedArrays(GL_T2F_N3F_V3F,0,Ver);

          glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Ver);
          glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
          glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3 + 3 + 1) * 4)));

          glEnableClientState(GL_INDEX_ARRAY);
          glIndexPointer(GL_UNSIGNED_SHORT,0,Id3);
          glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,Id3);

  FreeMem(Id3,36 * 2);
  FreeMem(Ver,24 * SizeOf(TD3ColorTexVertexNormal));

  glPopMatrix;
end;

procedure TD3MesaCanvas.FillMesh(const Center,Size:TD3Vector;
  const MeshData:TD3MeshData; const Opacity:single);
var
  Ver:PD3ColorTexVertexNormalArray;
  Id3:PWordArray;
  A,B,n:TD3Vector;
  C:TD3Color;
  i:integer;
  Flags:cardinal;
begin
  if Context=0 then Exit;
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

procedure TD3MesaCanvas.FillColorTexVertexNormal(const Vertices:array of TD3ColorTexVertexNormal;
      const Indices:array of word; const Opacity:single);
begin
  if Context=0 then Exit;
  if Length(Vertices)=0 then Exit;
  if Length(Indices)=0 then Exit;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
//          glnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glInterleavedArrays(GL_T2F_N3F_V3F,0,@Vertices[0]);
  glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),@Vertices[0]);
//          glColorPointer(4,GL_UNSIGNED_BYTE,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(Ver) + ((3) * 4)));
  glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(@Vertices[0]) + ((3) * 4)));
  glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(@Vertices[0]) + ((3 + 3 + 1) * 4)));

  glEnableClientState(GL_INDEX_ARRAY);
  glIndexPointer(GL_UNSIGNED_SHORT,0,@Indices[0]);
  glDrawElements(GL_TRIANGLES,Length(Indices),GL_UNSIGNED_SHORT,@Indices[0]);

  glPopMatrix;
end;

procedure TD3MesaCanvas.FillPolygon(const Center,Size:TD3Vector;
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
  if Context=0 then Exit;
  if Length(Points)=0 then Exit;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix;
  glMultMatrixf(@FCurrentMatrix);
//  ApplyLights;
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

//    glDisable(GL_CULL_FACE);
    glColorMask(false,false,false,false);
    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);
    glColorMask(true,true,true,true);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
//    glEnable(GL_CULL_FACE);

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

//    glDisable(GL_CULL_FACE);
    glColorMask(false,false,false,false);
    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);
    glColorMask(true,true,true,true);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
//    glEnable(GL_CULL_FACE);

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
//    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES,Length(MeshData.MeshIndices),GL_UNSIGNED_SHORT,Id3);
//    glEnable(GL_CULL_FACE);
    FreeMem(Id3,Length(MeshData.MeshIndices) * 2);
    FreeMem(Ver,Length(MeshData.MeshVertices) * SizeOf(TD3ColorTexVertexNormal));
  end;

  { free }
  MeshData.Free;
  glPopMatrix;
end;

procedure TD3MesaCanvas.UpdateBitmap(Bitmap:TD3Bitmap);
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
  if not (Context <> 0) then Exit;

  { create - if need }
  if GetHandle(Bitmap)=0 then
  begin
    glGenTextures(1,@Tex);
    AddHandle(Bitmap,Tex);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT,Tex);
    glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_GENERATE_MIPMAP_SGIS,GL_FALSE);
    glTexImage2D(GL_TEXTURE_RECTANGLE_EXT,0,GL_RGBA,Bitmap.Width,Bitmap.Height,0,GL_RGBA{GL_BGRA_EXT},GL_UNSIGNED_BYTE,Bitmap.Bits);
  end
  else
  begin
    Tex:=GetHandle(Bitmap);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT,Tex);
    if Bitmap.NeedUpdate then
    begin
      glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_GENERATE_MIPMAP_SGIS,GL_FALSE);
      glTexImage2D(GL_TEXTURE_RECTANGLE_EXT,0,GL_RGBA,Bitmap.Width,Bitmap.Height,0,GL_RGBA{GL_BGRA_EXT},GL_UNSIGNED_BYTE,Bitmap.Bits);
    end;
  end;
  Bitmap.NeedUpdate:=false;
end;

procedure TD3MesaCanvas.DestroyBitmap(const Bitmap:TD3Bitmap);
var
  Tex:cardinal;
begin
  if (Bitmap <> nil) then
    Bitmap.CanvasList.Remove(Self);
  if (Context <> 0) and (Bitmap <> nil) then
  begin
    Tex:=GetHandle(Bitmap);
    if Tex <> 0 then
      glDeleteTextures(1,@Tex);
    RemoveHandle(Bitmap);
  end;
end;

procedure TD3MesaCanvas.DrawText(const AX,AY:single; const AText:WideString; const Opacity:single);
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

    tx1:=CurFont.CharCoords[c-32].Coord[0] * CurFont.Bitmap.Width;
    ty1:=CurFont.CharCoords[c-32].Coord[1] * CurFont.Bitmap.Height;
    tx2:=CurFont.CharCoords[c-32].Coord[2] * CurFont.Bitmap.Width;
    ty2:=CurFont.CharCoords[c-32].Coord[3] * CurFont.Bitmap.Height;

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

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glInterleavedArrays(GL_T2F_N3F_V3F,0,pVertices);

    glVertexPointer(3,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),pVertices);
    glNormalPointer(GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(pVertices) + ((3) * 4)));
    glTexCoordPointer(2,GL_FLOAT,SizeOf(TD3ColorTexVertexNormal),Pointer(PtrInt(pVertices) + ((3 + 3 + 1) * 4)));

    glEnableClientState(GL_INDEX_ARRAY);
    glIndexPointer(GL_UNSIGNED_SHORT,0,pIndexs);
    glDrawElements(GL_TRIANGLES,charCount * 6,GL_UNSIGNED_SHORT,pIndexs);
  end;

  FreeMem(pIndexs,charCount * 6 * 2);
  FreeMem(pVertices,charCount * 4 * SizeOf(TD3ColorTexVertexNormal));

  glPopMatrix;
end;

function TD3MesaCanvas.MeasureText(const AText:WideString):TD3Point;
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

function TD3MesaCanvas.UpdateFont(const AText:WideString; Font:TD3Font):integer;
{$ifdef DARWIN}
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
  W:WideString;
  BoundsCount:ItemCount;
  BoundsRects:ATSTrapezoid;
  CLeft,CRight,CTop,CBottom:Fixed;
  LHeight,LBottom:integer;
  NewFont:TD3InternalFont;
  FTexWidth,FTexHeight:integer;
  S:string;
  i,j:integer;
  NewFontId3:integer;
  NeedUpdate:boolean;}
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
  LHeight:=Font.Size;
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
    if FTexHeight > MaxTextureSize then
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
  CGContextFillRect(CtxRef,CGRectFromRect(d3Rect(-1,-1,10000,10000)));
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
      CGContextStrokeRect(CtxRef,CGRectFromRect(d3Rect(x - 0.5,FTexHeight - P.Y - y - 0.5,x + Round(P.X) + 0.5,FTexHeight - y + 0.5)));}
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
{$endif}

{$ifdef WINDOWS}
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
  nHeight:= -Trunc(Font.Size);
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
{$endif}
{$ifdef UNIX}
begin
end;
{$endif}



