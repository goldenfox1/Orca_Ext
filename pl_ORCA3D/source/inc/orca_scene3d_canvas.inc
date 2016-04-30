{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//====================  TD3Canvas ===============================

constructor TD3Canvas.Create(const AWnd:cardinal; const AWidth,AHeight:integer; const ALockable:boolean; const Quality:TD3Quality);
begin
  FLockable:=ALockable;
  FQuality:=Quality;
  FWnd:=AWnd;
  FWidth:=AWidth;
  FHeight:=AHeight;
  inherited Create;
  FPaintToMatrix:=IdentityMatrix;
  FCurrentLights:=TList.Create;
  FMaterial:=TD3Material.Create;
  FMaterial.OnChanged:=MaterialChanged;
  FFont:=TD3Font.Create;

  if FBuffered or FLockable then
  begin
{$IFDEF WINDOWS}
  { Initialization }
  with FBitmapInfo.bmiHeader do
  begin
    biSize:=SizeOf(TBitmapInfoHeader);
    biPlanes:=1;
    biBitCount:=32;
    biCompression:=BI_RGB;
    biWidth:=AWidth;
    if biWidth <= 0 then biWidth:=1;
    biHeight:=-AHeight;
    if biHeight >= 0 then biHeight:=-1;
  end;

  { Create new DIB }
  FBufferHandle:=CreateDIBSection(0,FBitmapInfo,DIB_RGB_COLORS,Pointer(FBufferBits),0,0);
  if FBufferBits=nil then
    raise Exception.Create('Can''t allocate the DIB handle');

  FBufferDC:=CreateCompatibleDC(0);
  if FBufferDC=0 then
  begin
    DeleteObject(FBufferHandle);
    FBufferDC:=0;
    FBufferBits:=nil;
    raise Exception.Create('Can''t create compatible DC');
  end;

  if SelectObject(FBufferDC,FBufferHandle)=0 then
  begin
    DeleteDC(FBufferDC);
    DeleteObject(FBufferHandle);
    FBufferDC:=0;
    FBufferHandle:=0;
    FBufferBits:=nil;
    raise Exception.Create('Can''t select an object into DC');
  end;
{$ENDIF}
{$IFDEF DARWIN}
  GetMem(FBufferBits,FWidth * FHeight * 4);
  FBufferHandle:=CGBitmapContextCreate(FBufferBits,FWidth,FHeight,8,
    FWidth * 4,CGColorSpaceCreateDeviceRGB,kCGImageAlphaPremultipliedLast);
  CGContextTranslateCTM(FBufferHandle,0,FHeight);
  CGContextScaleCTM(FBufferHandle,1,-1);
{$ENDIF}
  end;
end;

constructor TD3Canvas.CreateFromBitmap(const ABitmap:TD3Bitmap);
begin
  FLockable:=true;
  FQuality:=d3LowQuality;
  inherited Create;
  FPaintToMatrix:=IdentityMatrix;
  FBitmap:=ABitmap;
  FWidth:=FBitmap.Width;
  FHeight:=FBitmap.Height;
  FCurrentLights:=TList.Create;
  FMaterial:=TD3Material.Create;
  FMaterial.OnChanged:=MaterialChanged;
  FFont:=TD3Font.Create;
end;

destructor TD3Canvas.Destroy;
var
  i:integer;
begin
  if FBitmaps <> nil then
    for i:=FBitmaps.Count - 1 downto 0 do
      DestroyBitmap(TD3Bitmap(FBitmaps[i]));
  FreeBuffer;
  if FBitmaps <> nil then FreeAndNil(FBitmaps);
  if FHandles <> nil then FreeAndNil(FHandles);
  FCurrentLights.Free;
  FFont.Free;
  FMaterial.Free;
  inherited;
end;

procedure TD3Canvas.AssignTo(Dest:TPersistent);
begin
    inherited ;
end;

procedure TD3Canvas.UpdateBitmap(Bitmap:TD3Bitmap);
begin
  if Bitmap=nil then Exit;
  if Bitmap.Width * Bitmap.Height=0 then
  begin
    Bitmap.NeedUpdate:=false;
    Exit;
  end;
  { add to free notification }
  if Bitmap.CanvasList.IndexOf(Self) < 0 then
    Bitmap.CanvasList.Add(Self);
end;

procedure TD3Canvas.AddHandle(const Bitmap:TD3Bitmap; const Handle:cardinal);
begin
  if FBitmaps=nil then
  begin
    FBitmaps:=TList.Create;
    FHandles:=TList.Create;
  end;
  if FBitmaps.IndexOf(Bitmap) < 0 then
  begin
    FBitmaps.Add(Bitmap);
    FHandles.Add(Pointer(Handle));
  end;
end;

procedure TD3Canvas.ChangeHandle(const Bitmap:TD3Bitmap; const Handle:cardinal);
var
  Id3:integer;
begin
  if FBitmaps=nil then Exit;
  Id3:=FBitmaps.IndexOf(Bitmap);
  if Id3 >= 0 then
  begin
    FHandles[Id3]:=Pointer(Handle);
  end;
end;

procedure TD3Canvas.RemoveHandle(const Bitmap:TD3Bitmap);
var
  Id3:integer;
begin
  if FBitmaps=nil then Exit;
  Id3:=FBitmaps.IndexOf(Bitmap);
  if (Id3 >= 0) then
  begin
    FBitmaps.Delete(Id3);
    FHandles.Delete(Id3);
  end;
end;

function TD3Canvas.GetHandle(const Bitmap:TD3Bitmap):cardinal;
var
  Id3:integer;
begin
  Result:=0;
  if FBitmaps=nil then Exit;
  Id3:=FBitmaps.IndexOf(Bitmap);
  if (Id3 >= 0) then
  begin
    Result:=cardinal(FHandles[Id3]);
  end;
end;

function TD3Canvas.GetScreenMatrix:TD3Matrix;
var
  matProj,scaleMatrix,transMatrix,orthoProj:TD3Matrix;
begin
  OrthoProj:=MatrixOrthoOffCenterRH(0,FWidth,0,FHeight,1,1000);
  matProj:=MatrixPerspectiveFovRH(cPI / 6,FWidth / FHeight,1,1000);

  transMatrix:=IdentityMatrix;
  transMatrix.m41:=0;
  transMatrix.m42:=0;
  transMatrix.m43:=-2;
  matProj:=d3MatrixMultiply(transMatrix,matProj);

  scaleMatrix:=IdentityMatrix;
  scaleMatrix.m11:=(OrthoProj.m11 / matProj.m11) * 2;
  scaleMatrix.m22:=-(OrthoProj.m11 / matProj.m11) * 2;
  scaleMatrix.m33:=-(OrthoProj.m11 / matProj.m11) * 2;
  matProj:=d3MatrixMultiply(scaleMatrix,matProj);

  transMatrix:=IdentityMatrix;
  transMatrix.m41:=-FWidth / 2;
  transMatrix.m42:=-FHeight / 2;
  transMatrix.m43:=0;
  matProj:=d3MatrixMultiply(transMatrix,matProj);

  Result:=matProj;
end;

function TD3Canvas.GetProjectionMatrix:TD3Matrix;
begin
  Result:=MatrixPerspectiveFovRH(cPI / 4{ + cPI / 6},FWidth / FHeight,1.0,1000.0);
  if (FPaintToMatrix.m41 <> 0) or (FPaintToMatrix.m11 <> 1) then
  begin
{    FPaintToMatrix:=IdentityMatrix;
    FPaintToMatrix.m11:=2;
    FPaintToMatrix.m22:=2;
    FPaintToMatrix.m41:=-1;
    FPaintToMatrix.m42:=1;}
    Result:=d3MatrixMultiply(Result,FPaintToMatrix);
  end;
end;

procedure TD3Canvas.SaveToStream(S:TStream);
var
  B:TD3Bitmap;
  Bits:PD3ColorArray;
begin
  {$ifdef WINDOWS}
  if FBufferBits <> nil then
  begin
    B:=TD3Bitmap.Create(FWidth,FHeight);
    B.LockBitmapBits(Bits,false);
    d2MoveLongword(FBufferBits,Bits,FWidth * FHeight);
    B.UnlockBitmapBits;
    B.SaveToStream(S);
    B.Free;
  end;
  {$endif}
end;

procedure TD3Canvas.SaveToBits(Bits:Pointer);
var
  j:integer;
begin
  if FBufferBits <> nil then
  begin
    {$ifdef DARWIN}
    for j:=0 to FHeight - 1 do
      d2MoveLongword(@PD3ColorArray(FBufferBits)[j * FWidth],@PD3ColorArray(Bits)[(FHeight - j - 1) * FWidth],FWidth);
    {$else}
    d2MoveLongword(FBufferBits,Bits,FWidth * FHeight);
    {$endif}
  end;
end;

procedure TD3Canvas.SetQuality(const Quality:TD3Quality);
begin
  FQuality:=Quality;
end;

procedure TD3Canvas.ResizeBuffer(const AWidth,AHeight:integer);
begin
  if (AWidth=FWidth) and (AHeight=FHeight) then Exit;
{$IFDEF WINDOWS}
  if FBuffered or FLockable then
  begin
    if FBufferDC <> 0 then DeleteDC(FBufferDC);
    if FBufferHandle <> 0 then DeleteObject(FBufferHandle);
    FBufferDC:=0;
    FBufferHandle:=0;
    FBufferBits:=nil;
    FWidth:=AWidth;
    FHeight:=AHeight;
    if FWidth <= 0 then FWidth:=1;
    if FHeight <= 0 then FHeight:=1;
    { Initialization }
    with FBitmapInfo.bmiHeader do
    begin
      biSize:=SizeOf(TBitmapInfoHeader);
      biPlanes:=1;
      biBitCount:=32;
      biCompression:=BI_RGB;
      biWidth:=AWidth;
      if biWidth <= 0 then biWidth:=1;
      biHeight:=-AHeight;
      if biHeight >= 0 then biHeight:=-1;
    end;

    { Create new DIB }
    FBufferHandle:=CreateDIBSection(0,FBitmapInfo,DIB_RGB_COLORS,Pointer(FBufferBits),0,0);
    if FBufferBits=nil then
      raise Exception.Create('Can''t allocate the DIB handle');

    FBufferDC:=CreateCompatibleDC(0);
    if FBufferDC=0 then
    begin
      DeleteObject(FBufferHandle);
      FBufferDC:=0;
      FBufferBits:=nil;
      raise Exception.Create('Can''t create compatible DC');
    end;

    if SelectObject(FBufferDC,FBufferHandle)=0 then
    begin
      DeleteDC(FBufferDC);
      DeleteObject(FBufferHandle);
      FBufferDC:=0;
      FBufferHandle:=0;
      FBufferBits:=nil;
      raise Exception.Create('Can''t select an object into DC');
    end;
  end
  else
  begin
    FWidth:=AWidth;
    FHeight:=AHeight;
    if FWidth <= 0 then FWidth:=1;
    if FHeight <= 0 then FHeight:=1;
  end;
{$ENDIF}
{$IFDEF DARWIN}
  if FBuffered or FLockable then
  begin
    if FBufferBits <> nil then
      System.FreeMem(FBufferBits,FWidth * FHeight * 4);
    FBufferBits:=nil;
    FWidth:=AWidth;
    FHeight:=AHeight;
    if FWidth <= 0 then FWidth:=1;
    if FHeight <= 0 then FHeight:=1;

    GetMem(FBufferBits,FWidth * FHeight * 4);
    FBufferHandle:=CGBitmapContextCreate(FBufferBits,FWidth,FHeight,8,
      FWidth * 4,CGColorSpaceCreateDeviceRGB,kCGImageAlphaPremultipliedLast);
    CGContextTranslateCTM(FBufferHandle,0,FHeight);
    CGContextScaleCTM(FBufferHandle,1,-1);
  end
  else
  begin
    FWidth:=AWidth;
    FHeight:=AHeight;
    if FWidth <= 0 then FWidth:=1;
    if FHeight <= 0 then FHeight:=1;
  end;
{$ENDIF}
end;

procedure TD3Canvas.FreeBuffer;
begin
{$IFDEF WINDOWS}
  if FBufferHandle=0 then Exit;
  if FBufferDC <> 0 then DeleteDC(FBufferDC);
  FBufferDC:=0;
  if FBufferHandle <> 0 then DeleteObject(FBufferHandle);
  FBufferHandle:=0;
{$ENDIF}
{$ifdef DARWIN}
  if FBufferHandle <> nil then
    CGContextRelease(FBufferHandle);
  if FBufferBits <> nil then
    System.FreeMem(FBufferBits,FWidth * FHeight * 4);
{$endif}
  FBufferBits:=nil;
end;

procedure TD3Canvas.Reset;
begin
  { reset }
  FCurrentVertexFormat:=vfNone;
  FChangeStateCount:=0;
  FillChar(FCurrentStates,SizeOf(FCurrentStates),0);
//  FCurrentMesh:=nil;
//  FCurrentPixmap[0]:=nil;
//  FCurrentPixmap[1]:=nil;
//  FCurrentPixmap[2]:=nil;
//  FCurrentPixmap[3]:=nil;
//  FCurrentMaterial.lighting:=false;
//  FCurrentMaterial.diffuse:=0;
//  FCurrentMaterial.specular:=0;
//  FCurrentMaterial.ambient:=0;
  FCurrentCameraMatrix:=IdentityMatrix;
  SetMatrix(IdentityMatrix);
  FCurrentLights.Clear;
  FMaterial.FDiffuse:=0;
  FMaterial.FAmbient:=0;
  FMaterial.FBitmap:='';
end;

function TD3Canvas.BeginScene:boolean;
begin
  Result:=false;
end;

procedure TD3Canvas.FlushBuffer;
begin
end;

procedure TD3Canvas.MaterialChanged(Sender:TObject);
begin

end;

procedure TD3Canvas.SetMatrix(const M:TD3Matrix);
begin
  FCurrentMatrix:=M;
end;

procedure TD3Canvas.SetLight(const Light:TD3Light);
begin
  FCurrentLights.Add(Light);
end;

procedure TD3Canvas.SetCamera(const Camera:TD3Camera);
begin
  FCurrentCamera:=Camera;
  if FCurrentCamera.Target <> nil then
  begin
    FCurrentCameraMatrix:=MatrixLookAtDirRH(Camera.AbsoluteMatrix.M[3],d3VectorSubtract(Camera.Target.AbsolutePosition,Camera.AbsolutePosition),Camera.AbsoluteMatrix.M[2]);
  end
  else
    FCurrentCameraMatrix:=MatrixLookAtDirRH(Camera.AbsoluteMatrix.M[3],Camera.AbsoluteMatrix.M[1],Camera.AbsoluteMatrix.M[2]);
  FCurrentCameraInvMatrix:=FCurrentCameraMatrix;
  InvertMatrix(FCurrentCameraInvMatrix);
end;

procedure TD3Canvas.SetMaterialWithOpacity(M:TD3Material; Opacity:single);
var
  SaveD:cardinal;
begin
  SaveD:=M.NativeDiffuse;
  M.FDiffuse:=d3Opacity(M.NativeDiffuse,Opacity);
  FMaterial.Assign(M);
  M.FDiffuse:=SaveD;
end;

procedure TD3Canvas.SetMaterial(const M:TD3Material);
begin
  FMaterial.Assign(M);
end;

procedure TD3Canvas.SetRenderState(const State:TD3RenderState);
begin
  if not FCurrentStates[State] then
  begin
    FCurrentStates[State]:=true;
    case State of
      rs2DScene:FCurrentStates[rs3DScene]:=false;
      rs3DScene:FCurrentStates[rs2DScene]:=false;
      rsLightOn:FCurrentStates[rsLightOff]:=false;
      rsLightOff:FCurrentStates[rsLightOn]:=false;
      rsZTestOn:FCurrentStates[rsZTestOff]:=false;
      rsZTestOff:FCurrentStates[rsZTestOn]:=false;
      rsZWriteOn:FCurrentStates[rsZWriteOff]:=false;
      rsZWriteOff:FCurrentStates[rsZWriteOn]:=false;
      rsFrontFace:
        begin
          FCurrentStates[rsBackFace]:=false;
          FCurrentStates[rsAllFace]:=false;
        end;
      rsBackFace:
        begin
          FCurrentStates[rsAllFace]:=false;
          FCurrentStates[rsFrontFace]:=false;
        end;
      rsAllFace:
        begin
          FCurrentStates[rsBackFace]:=false;
          FCurrentStates[rsFrontFace]:=false;
        end;
      { Blending }
      rsBlendAdditive:FCurrentStates[rsBlendNormal]:=false;
      rsBlendNormal:FCurrentStates[rsBlendAdditive]:=false;
      { Tex stretch }
      rsTexNearest:FCurrentStates[rsTexLinear]:=false;
      rsTexLinear:FCurrentStates[rsTexNearest]:=false;
      { Tex modulation }
      rsTexDisable:
        begin
          FCurrentStates[rsTexModulate]:=false;
          FCurrentStates[rsTexReplace]:=false;
        end;
      rsTexReplace:
        begin
          FCurrentStates[rsTexModulate]:=false;
          FCurrentStates[rsTexDisable]:=false;
        end;
      rsTexModulate:
        begin
          FCurrentStates[rsTexDisable]:=false;
          FCurrentStates[rsTexReplace]:=false;
        end;
      { Fill }
      rsFrame:FCurrentStates[rsSolid]:=false;
      rsSolid:FCurrentStates[rsFrame]:=false;
      { Shade }
      rsFlat:FCurrentStates[rsGouraud]:=false;
      rsGouraud:FCurrentStates[rsFlat]:=false;
    end;
    ApplyRenderState(State);
  end;
end;

procedure TD3Canvas.Pick(x,y:single; const AProj:TD3Projection; var RayPos,RayDir:TD3Vector);
var
  matProj:TD3Matrix;
  vPos,vNear:TD3Vector;
begin
  if AProj=d3ProjectionCamera then
  begin
    { camera }
    matProj:=GetProjectionMatrix;
    // Compute the vector of the pick ray in screen space
    vPos:=d3Vector(0,0,0);
    vNear:=d3Vector((1.0 - (2.0 * (x / FWidth))) / matProj.m11,
      -(1.0 - (2.0 * (y / FHeight))) / matProj.m22,1);
    // Get the inverse view matrix
    if FCurrentCamera <> nil then
    begin
      // Transform the screen space pick ray into 3D space
      vPos:=d3VectorTransform(vPos,FCurrentCameraInvMatrix);
      vNear:=d3VectorTransform(vNear,FCurrentCameraInvMatrix);
    end;
    RayPos:=vPos;
    RayDir:=d3VectorNormalize(d3VectorSubtract(vPos,vNear));
  end
  else
  begin
    { screen }
    matProj:=GetScreenMatrix;
    InvertMatrix(matProj);
    vPos:=d3Vector(0,0,0);
    vPos:=d3VectorTransform(vPos,matProj);
    // Old behavior
    vPos:=d3Vector(FWidth / 2,FHeight / 2,vPos.Z * 2);
    vNear:=d3Vector(x,y,0);
    RayPos:=vPos;
    RayDir:=d3VectorNormalize(d3VectorSubtract(vNear,vPos));
  end;
end;

function TD3Canvas.WorldToScreen(const AProj:TD3Projection; const P:TD3Point):TD3Point;
var
  matProj:TD3Matrix;
begin
  if AProj=d3ProjectionCamera then
  begin
    { camera }
    matProj:=FCurrentCameraMatrix;
    Result:=d3Point(d3VectorTransform(d3Vector(P),matProj));

    matProj:=GetProjectionMatrix;
    if Result.z <> 0 then
    begin
      Result.x:=-((Result.x / Result.z) * matProj.m11 - 1) * FWidth / 2;
      Result.y:=((Result.y / Result.z) * matProj.m22 + 1) * FHeight / 2;
    end;
  end
  else
  begin
    { screen }
    matProj:=GetScreenMatrix;
    Result:=P;
  end;
end;

procedure TD3Canvas.FillRect(const Rect:TD2Rect; const Depth,Opacity:single);
var
  P:TD2Polygon;
begin
  SetLength(P,5);
  P[0]:=Rect.TopLeft;
  P[1]:=d2Point(Rect.Right,Rect.Top);
  P[2]:=Rect.BottomRight;
  P[3]:=d2Point(Rect.Left,Rect.Bottom);
  P[4]:=Rect.TopLeft;
  FillPolygon(d3Vector((Rect.Left + Rect.Right) / 2,(Rect.Top + Rect.Bottom) / 2,0),
    d3Vector(d2RectWidth(Rect),d2RectHeight(Rect),Depth),Rect,P,Opacity);
end;   

//===================== TD3CustomCanvasLayer ==========================

constructor TD3CustomCanvasLayer.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD3CustomCanvasLayer.Destroy;
begin
  if FBitmap <> nil then FreeAndNil(FBitmap);
  inherited;
end;

function TD3CustomCanvasLayer.GetLayerCanvas:TCanvas;
begin
  if FBitmap=nil then
  begin
    FBitmap:=TBitmap.Create;
    FBitmap.HandleType:=bmDIB;
    FBitmap.PixelFormat:=pf32bit;
  end;
  if (FBitmap.Width <> FLayerWidth) then
    FBitmap.Width:=FLayerWidth;
  if (FBitmap.Height <> FLayerHeight) then
    FBitmap.Height:=-FLayerHeight;
  Result:=FBitmap.Canvas;
end;

procedure TD3CustomCanvasLayer.BeforePaint;
var
  Bits:PD3ColorArray;
  Update:boolean;
  //xbmp:TLazIntfImage;
begin

  if Assigned(FOnPaint) then
  begin
    Update:=false;
    FOnPaint(Self,LayerCanvas,types.Rect(0,0,FLayerWidth,FLayerHeight),Update);

    if Update and FBuffer.LockBitmapBits(Bits,true) then
    begin
     // xbmp:=FBitmap.CreateIntfImage;

      d2MoveLongword(FBitmap.ScanLine[FBitmap.Height - 1],Bits,FBuffer.Width * FBuffer.Height);

      d2FillAlpha(Bits,FBuffer.Width * FBuffer.Height,$FF);

      d2FillLongword(Bits,FBuffer.Width,$707070);
      d2FillLongword(@Bits[FBuffer.Width * (1)],FBuffer.Width,$707070);
      d2FillLongword(@Bits[FBuffer.Width * (FBuffer.Height - 1)],FBuffer.Width,$707070);
      d2FillLongword(@Bits[FBuffer.Width * (FBuffer.Height - 2)],FBuffer.Width,$707070);

     // FBitmap.LoadFromIntfImage(xbmp);
     // xbmp.free;

      FBuffer.UnlockBitmapBits;
    end;
  end
  else
    FBuffer.Clear($FFFFFFFF);

  inherited;
end;

procedure TD3CustomCanvasLayer.UpdateCanvas;
var
  Bits:PD3ColorArray;
 // xbmp:TLazIntfImage;
begin

  if FBuffer.LockBitmapBits(Bits,true) then
  begin
   // xbmp:=FBitmap.CreateIntfImage;
    d2MoveLongword(FBitmap.ScanLine[FBitmap.Height - 1],Bits,FBuffer.Width * FBuffer.Height);

    d2FillAlpha(Bits,FBuffer.Width * FBuffer.Height,$FF);

    d2FillLongword(Bits,FBuffer.Width,$707070);
    d2FillLongword(@Bits[FBuffer.Width * (1)],FBuffer.Width,$707070);
    d2FillLongword(@Bits[FBuffer.Width * (FBuffer.Height - 1)],FBuffer.Width,$707070);
    d2FillLongword(@Bits[FBuffer.Width * (FBuffer.Height - 2)],FBuffer.Width,$707070);

 //   FBitmap.LoadFromIntfImage(xbmp);
 //   xbmp.free;

    FBuffer.UnlockBitmapBits;
  end;

end;

//=============== TD3GUICanvasLayer ===================================

constructor TD3GUICanvasLayer.Create(AOwner:TComponent);
begin
  inherited;
  Projection:=d3ProjectionScreen;
  Width:=256;
  Height:=256;
end;
             
