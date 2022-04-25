{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=================== TD3CustomLayer ======================================

constructor TD3CustomLayer.Create(AOwner:TComponent);
begin
  inherited;
  TwoSide:=true;

  Width:=4;
  Depth:=3;
  Height:=1;
end;

destructor TD3CustomLayer.Destroy;
begin
  inherited;
end;

procedure TD3CustomLayer.BeforePaint;
begin
  inherited;
end;

procedure TD3CustomLayer.Paint;
begin
end;

function TD3CustomLayer.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
var
  ip:TD3Vector;
  p:array [0..5] of TD3Vector;
  CubeSize:TD3Vector;
  r:TD3Vector;
  i:integer;
  t,e:single;
  eSize:TD3Vector;
begin
  Result:=false;
  e:=0.5 + 0.003; // Small value for floating point imprecisions
  CubeSize.V[0]:=Width;
  CubeSize.V[1]:=Height;
  CubeSize.V[2]:=Depth;
  eSize.V[0]:=Width * e;
  eSize.V[1]:=Height * e;
  eSize.V[2]:=Depth * e;
  p[0]:=XHmgVector;
  p[1]:=YHmgVector;
  p[2]:=ZHmgVector;
  p[3]:=d3Vector(-1, 0, 0);
  p[4]:=d3Vector(0,-1, 0);
  p[5]:=d3Vector(0, 0,-1);
  for i:=0 to 5 do
  begin
    if d3VectorDotProduct(p[i],RayDir) > 0 then
    begin
      t:=- (p[i].V[0]*RayPos.V[0] + p[i].V[1]*RayPos.V[1] + p[i].V[2]*RayPos.V[2] + 0.5 * CubeSize.V[i mod 3])
           / (p[i].V[0]*RayDir.V[0] + p[i].V[1]*RayDir.V[1] + p[i].V[2]*RayDir.V[2]);
      r:=d3Vector(RayPos.V[0] + t*RayDir.V[0],RayPos.V[1] + t*RayDir.V[1],RayPos.V[2] + t*RayDir.V[2]);
      if (Abs(r.V[0]) <= eSize.V[0]) and (Abs(r.V[1]) <= eSize.V[1]) and (Abs(r.V[2]) <= eSize.V[2]) and
         (d3VectorDotProduct(d3VectorSubtract(r,RayPos),RayDir) > 0) then
      begin
        Intersection:=d3Vector(LocalToAbsolute(d3Point(r)));
        Result:=true;
        Break;
      end;
    end;
  end;
  // calc for captured - bacause mouse can be inside layer
  if not Result and (Scene <> nil) and (Scene.FCaptured=Self) then
  begin
    e:=0.5 + 0.003; // Small value for floating point imprecisions
    CubeSize.V[0]:=Width * 10;
    CubeSize.V[1]:=Height * 10;
    CubeSize.V[2]:=Depth * 10;
    eSize.V[0]:=Width * 10 * e;
    eSize.V[1]:=Height * 10 * e;
    eSize.V[2]:=Depth * 10 * e;
    p[0]:=XHmgVector;
    p[1]:=YHmgVector;
    p[2]:=ZHmgVector;
    p[3]:=d3Vector(-1, 0, 0);
    p[4]:=d3Vector(0,-1, 0);
    p[5]:=d3Vector(0, 0,-1);
    for i:=0 to 5 do
    begin
      if d3VectorDotProduct(p[i],RayDir) > 0 then
      begin
        t:=- (p[i].V[0]*RayPos.V[0] + p[i].V[1]*RayPos.V[1] + p[i].V[2]*RayPos.V[2] + 0.5 * CubeSize.V[i mod 3])
             / (p[i].V[0]*RayDir.V[0] + p[i].V[1]*RayDir.V[1] + p[i].V[2]*RayDir.V[2]);
        r:=d3Vector(RayPos.V[0] + t*RayDir.V[0],RayPos.V[1] + t*RayDir.V[1],RayPos.V[2] + t*RayDir.V[2]);
        if (Abs(r.V[0]) <= eSize.V[0]) and (Abs(r.V[1]) <= eSize.V[1]) and (Abs(r.V[2]) <= eSize.V[2]) and
           (d3VectorDotProduct(d3VectorSubtract(r,RayPos),RayDir) > 0) then
        begin
          Intersection:=d3Vector(LocalToAbsolute(d3Point(r)));
          Result:=true;
          Exit;
        end;
      end;
    end;
  end;
end;

procedure TD3CustomLayer.MouseMove(Shift:TShiftState; X,Y,d3,
  Dy:single; rayPos,rayDir:TD3Vector);
var
  P3,rPos,rDir:TD3Vector;
begin
  inherited;
  if FDisableLayerEvent then Exit;
  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * Width);
      Y:=(((-P3.z + (Depth / 2)) / Depth) * Depth);
    end
    else
      Exit;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * Width);
      Y:=(((P3.y + (Height / 2)) / Height) * Height);
    end
    else
      Exit;
  end;
  LayerMouseMove(Shift,X,Y);
end;

procedure TD3CustomLayer.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single; rayPos,rayDir:TD3Vector);
var
  P3,rPos,rDir:TD3Vector;
begin
  inherited;
  if FDisableLayerEvent then Exit;
  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * Width);
      Y:=(((-P3.z + (Depth / 2)) / Depth) * Depth);
    end
    else
      Exit;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * Width);
      Y:=(((P3.y + (Height / 2)) / Height) * Height);
    end
    else
      Exit;
  end;
  LayerMouseDown(Button,Shift,X,Y);
end;

procedure TD3CustomLayer.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single; rayPos,rayDir:TD3Vector);
var
  P3,rPos,rDir:TD3Vector;
begin
  inherited;
  if FDisableLayerEvent then Exit;
  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * Width);
      Y:=(((-P3.z + (Depth / 2)) / Depth) * Depth);
    end;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * Width);
      Y:=(((P3.y + (Height / 2)) / Height) * Height);
    end;
  end;
  LayerMouseUp(Button,Shift,X,Y);
end;

procedure TD3CustomLayer.LayerMouseMove(Shift:TShiftState; X,Y:single);
begin
  if Assigned(FOnLayerMouseMove) then
      FOnLayerMouseMove(Self,Shift,trunc(X),trunc(Y));
end;

procedure TD3CustomLayer.LayerMouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  if Assigned(FOnLayerMouseDown) then
      FOnLayerMouseDown(Self,Button,Shift,trunc(X),trunc(Y));
end;

procedure TD3CustomLayer.LayerMouseUp(Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  if Assigned(FOnLayerMouseUp) then
      FOnLayerMouseUp(Self,Button,Shift,trunc(X),trunc(Y));
end;

procedure TD3CustomLayer.SetWidth(const Value:single);
begin
  inherited SetWidth(Value);
end;

procedure TD3CustomLayer.SetDepth(const Value:single);
begin
  if Projection=d3ProjectionCamera then
  begin
    inherited SetDepth(Value);
  end
  else
    inherited SetDepth(Value);
end;

procedure TD3CustomLayer.SetHeight(const Value:single);
begin
  if Projection=d3ProjectionCamera then
    inherited SetHeight(Value)
  else
  begin
    inherited SetHeight(Value);
  end;
end;

procedure TD3CustomLayer.SetLayerAlign(const Value:TD3LayerAlign);
begin
  if FLayerAlign <> Value then
  begin
    FLayerAlign:=Value;
    if (Parent <> nil) and (Parent.IsVisual) then
      Parent.Visual.Realign;
  end;
end;

//=================== TD3CustomBufferLayer =======================================

constructor TD3CustomBufferLayer.Create(AOwner:TComponent);
const
  Res=2;
var
  x,y:integer;
  s,xx,yy:single;
begin
  inherited;
  TwoSide:=true;

  FResolution:=60;
  FModulationColor:=$FFFFFFFF;
  Width:=4;
  Depth:=3;
  Height:=1;

  FPlane:=TD3MeshData.Create;
  SetLength(FPlane.MeshVertices,Res * Res);
  for y:=0 to Res - 1 do
    for x:=0 to Res - 1 do
    begin
{      xx:=(-0.5 + (x / (Res - 1))) * 4;
      yy:=(-0.5 + (y / (Res - 1))) * 4;
      s:=-sin(xx * xx + yy * yy) * 0.5;}
      s:=0;
      FPlane.MeshVertices[x + (y * Res)]:=TexVertexNormal(-0.5 + (x / (Res - 1)),s,-0.5 + (y / (Res - 1)),0,-1,0,(x / (Res - 1)),1 - (y / (Res - 1)));
    end;
  SetLength(FPlane.MeshIndices,Res * Res * 6);
  for y:=0 to Res - 2 do
    for x:=0 to Res - 2 do
    begin
      FPlane.MeshIndices[(x + (y * Res)) * 6 + 0]:=x + (y * Res);
      FPlane.MeshIndices[(x + (y * Res)) * 6 + 2]:=x + 1 + (y * Res);
      FPlane.MeshIndices[(x + (y * Res)) * 6 + 1]:=x + ((y + 1) * Res);
      FPlane.MeshIndices[(x + (y * Res)) * 6 + 3]:=x + ((y + 1) * Res);
      FPlane.MeshIndices[(x + (y * Res)) * 6 + 5]:=x + 1 + (y * Res);
      FPlane.MeshIndices[(x + (y * Res)) * 6 + 4]:=x + 1 + ((y + 1) * Res);
    end;
  FPlane.CalcNormals;

  FPlaneScreen:=TD3MeshData.Create;
  SetLength(FPlaneScreen.MeshVertices,Res * Res);
  for y:=0 to Res - 1 do
    for x:=0 to Res - 1 do
    begin
      s:=0; //sin((y / (Res - 1)) * (Pi * 4));
      FPlaneScreen .MeshVertices[x + (y * Res)]:=TexVertexNormal(-0.5 + (x / (Res - 1)),-0.5 + (y / (Res - 1)),s,0,0,1,(x / (Res - 1)),(y / (Res - 1)));
    end;

  SetLength(FPlaneScreen.MeshIndices,Res * Res * 6);
  for y:=0 to Res - 2 do
    for x:=0 to Res - 2 do
    begin
      FPlaneScreen.MeshIndices[(x + (y * Res)) * 6 + 0]:=x + (y * Res);
      FPlaneScreen.MeshIndices[(x + (y * Res)) * 6 + 1]:=x + 1 + (y * Res);
      FPlaneScreen.MeshIndices[(x + (y * Res)) * 6 + 2]:=x + ((y + 1) * Res);
      FPlaneScreen.MeshIndices[(x + (y * Res)) * 6 + 3]:=x + ((y + 1) * Res);
      FPlaneScreen.MeshIndices[(x + (y * Res)) * 6 + 4]:=x + 1 + (y * Res);
      FPlaneScreen.MeshIndices[(x + (y * Res)) * 6 + 5]:=x + 1 + ((y + 1) * Res);
    end;
  FBuffer:=TD3Bitmap.Create(FLayerWidth,FLayerHeight);
  if GvarD3BitmapList=nil then
    GvarD3BitmapList:=TStringList.Create;
  GvarD3BitmapList.AddObject(Name,Self);
end;

procedure TD3CustomBufferLayer.SetName(const NewName:TComponentName);
var
  Id3:integer;
begin
  if Name <> NewName then
  begin
    inherited SetName(NewName);
    Id3:=GvarD3BitmapList.IndexOfObject(Self);
    if Id3 >= 0 then
      GvarD3BitmapList[Id3]:=Name;
  end;
end;

destructor TD3CustomBufferLayer.Destroy;
begin
  if GvarD3BitmapList <> nil then
    GvarD3BitmapList.Delete(GvarD3BitmapList.IndexOfObject(Self));
  FreeAndNil(FPlane);
  FreeAndNil(FPlaneScreen);
  FreeAndNil(FBuffer);
  inherited;
end;

procedure TD3CustomBufferLayer.BeforePaint;
begin
  if FBuffer=nil then
    FBuffer:=TD3Bitmap.Create(FLayerWidth,FLayerHeight);
  if (FBuffer.Width <> FLayerWidth) or (FBuffer.Height <> FLayerHeight) then
  begin
    FBuffer.SetSize(FLayerWidth,FLayerHeight);
  end;
  inherited;
end;

procedure TD3CustomBufferLayer.Paint;
var
  Offset:TD3Point;
  M:TD3Matrix;
begin
  if (FBuffer <> nil) and Assigned(FOnUpdateBuffer) then
    FOnUpdateBuffer(Self);
  Canvas.Material.FBitmap:='';
  Canvas.Material.FBitmapTile:=1;
  Canvas.Material.FBitmapTileY:=1;
  Canvas.Material.FBitmapRect.left:=0;
  Canvas.Material.FBitmapRect.top:=0;
  Canvas.Material.FBitmapRect.right:=0;
  Canvas.Material.FBitmapRect.bottom:=0;
  Canvas.Material.FDiffuse:=d3Opacity(FModulationColor,AbsoluteOpacity);
  Canvas.Material.FLighting:=false;
  Canvas.Material.FModulation:=d3TexModulate;
  Canvas.Material.FShadeMode:=d3Gouraud;
  Canvas.Material.FFillMode:=d3Solid;
  Canvas.Material.FTempBitmap:=GetBitmap;
  Canvas.SetMaterial(Canvas.Material);
  Canvas.SetRenderState(rsTexModulate);
  Canvas.SetRenderState(rsTexLinear);
  if Projection=d3ProjectionCamera then
    Canvas.FillMesh(d3Vector(0,0,0),d3Vector(Width,Height,Depth),FPlane,AbsoluteOpacity)
  else
  begin
    M:=AbsoluteMatrix;
    Offset:=d3Point(0,0,0);
    M.m41:=trunc(M.m41);
    M.m42:=trunc(M.m42);
    Canvas.SetMatrix(M);
    Canvas.FillMesh(d3Vector(Offset.X + frac(FLayerWidth / 2),Offset.Y + frac(FLayerHeight / 2),0),d3Vector(FLayerWidth,FLayerHeight,Depth),FPlaneScreen,AbsoluteOpacity)
  end;
  Canvas.Material.FTempBitmap:=nil;
  Canvas.SetRenderState(rsTexDisable);
end;

function TD3CustomBufferLayer.GetBitmap:TD3Bitmap;
begin
  Result:=FBuffer;
end;

function TD3CustomBufferLayer.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
var
  ip:TD3Vector;
  p:array [0..5] of TD3Vector;
  p1,p2,p3:TD3Vector;
  CubeSize:TD3Vector;
  r:TD3Vector;
  i,j:integer;
  t,e:single;
  eSize:TD3Vector;
  FData:TD3MeshData;
begin
  Result:=false;
  e:=0.5 + 0.003; //Small value for floating point imprecisions
  CubeSize.V[0]:=Width;
  CubeSize.V[1]:=Height;
  CubeSize.V[2]:=Depth;
  eSize.V[0]:=Width * e;
  eSize.V[1]:=Height * e;
  eSize.V[2]:=Depth * e;
  p[0]:=XHmgVector;
  p[1]:=YHmgVector;
  p[2]:=ZHmgVector;
  p[3]:=d3Vector(-1, 0, 0);
  p[4]:=d3Vector(0,-1, 0);
  p[5]:=d3Vector(0, 0,-1);
  if Projection=d3ProjectionScreen then
    FData:=FPlaneScreen
  else
    FData:=FPlane;
  for i:=0 to 5 do
  begin
    if d3VectorDotProduct(p[i],RayDir) > 0 then
    begin
      t:=- (p[i].V[0]*RayPos.V[0] + p[i].V[1]*RayPos.V[1] + p[i].V[2]*RayPos.V[2] + 0.5 * CubeSize.V[i mod 3])
           / (p[i].V[0]*RayDir.V[0] + p[i].V[1]*RayDir.V[1] + p[i].V[2]*RayDir.V[2]);
      r:=d3Vector(RayPos.V[0] + t*RayDir.V[0],RayPos.V[1] + t*RayDir.V[1],RayPos.V[2] + t*RayDir.V[2]);
      if (Abs(r.V[0]) <= eSize.V[0]) and (Abs(r.V[1]) <= eSize.V[1]) and (Abs(r.V[2]) <= eSize.V[2]) and
         (d3VectorDotProduct(d3VectorSubtract(r,RayPos),RayDir) > 0) then
      begin
        { check geomerty }
        if (Length(FData.MeshVertices) > 0) and (Length(FData.MeshIndices) > 0) then
        begin
          for j:=0 to High(FData.MeshIndices) div 3 do
          begin
            if (FData.MeshIndices[(j * 3) + 0] <= High(FData.MeshVertices)) and
               (FData.MeshIndices[(j * 3) + 1] <= High(FData.MeshVertices)) and
               (FData.MeshIndices[(j * 3) + 2] <= High(FData.MeshVertices)) then
            begin
              with FData.MeshVertices[FData.MeshIndices[(j * 3) + 0]] do
                p1:=d3Vector(x * Width,y * Height,z * Depth);
              with FData.MeshVertices[FData.MeshIndices[(j * 3) + 1]] do
                p2:=d3Vector(x * Width,y * Height,z * Depth);
              with FData.MeshVertices[FData.MeshIndices[(j * 3) + 2]] do
                p3:=d3Vector(x * Width,y * Height,z * Depth);
              if RayCastTriangleIntersect(RayPos,RayDir,p1,p2,p3,@ip,nil) then
              begin
                Intersection:=LocalToAbsoluteVector(ip);
                Result:=true;
                Break;
              end;
            end;
          end;
        end
        else
          if (csDesigning in ComponentState) {or (FScene.DesignTime) }then
          begin
            Result:=true;
          end;
        Break;
      end;
    end;
  end;
  // calc for captured - bacause mouse can be inside layer
  if not Result and (Scene <> nil) and (Scene.FCaptured=Self) then
  begin
    e:=0.5 + 0.003; // Small value for floating point imprecisions
    CubeSize.V[0]:=Width * 10;
    CubeSize.V[1]:=Height * 10;
    CubeSize.V[2]:=Depth * 10;
    eSize.V[0]:=Width * 10 * e;
    eSize.V[1]:=Height * 10 * e;
    eSize.V[2]:=Depth * 10 * e;
    p[0]:=XHmgVector;
    p[1]:=YHmgVector;
    p[2]:=ZHmgVector;
    p[3]:=d3Vector(-1, 0, 0);
    p[4]:=d3Vector(0,-1, 0);
    p[5]:=d3Vector(0, 0,-1);
    for i:=0 to 5 do
    begin
      if d3VectorDotProduct(p[i],RayDir) > 0 then
      begin
        t:=- (p[i].V[0]*RayPos.V[0] + p[i].V[1]*RayPos.V[1] + p[i].V[2]*RayPos.V[2] + 0.5 * CubeSize.V[i mod 3])
             / (p[i].V[0]*RayDir.V[0] + p[i].V[1]*RayDir.V[1] + p[i].V[2]*RayDir.V[2]);
        r:=d3Vector(RayPos.V[0] + t*RayDir.V[0],RayPos.V[1] + t*RayDir.V[1],RayPos.V[2] + t*RayDir.V[2]);
        if (Abs(r.V[0]) <= eSize.V[0]) and (Abs(r.V[1]) <= eSize.V[1]) and (Abs(r.V[2]) <= eSize.V[2]) and
           (d3VectorDotProduct(d3VectorSubtract(r,RayPos),RayDir) > 0) then
        begin
          Intersection:=d3Vector(LocalToAbsolute(d3Point(r)));
          Result:=true;
          Exit;
        end;
      end;
    end;
  end;
end;

procedure TD3CustomBufferLayer.MouseMove(Shift:TShiftState; X,Y,d3,
  Dy:single; rayPos,rayDir:TD3Vector);
var
  P3,rPos,rDir:TD3Vector;
begin
  FDisableLayerEvent:=true;
  try
    inherited;
  finally
    FDisableLayerEvent:=false;
  end;
  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((-P3.z + (Depth / 2)) / Depth) * FLayerHeight);
    end
    else
      Exit;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((P3.y + (Height / 2)) / Height) * FLayerHeight);
    end
    else
      Exit;
  end;
  LayerMouseMove(Shift,X,Y);
end;

procedure TD3CustomBufferLayer.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single; rayPos,rayDir:TD3Vector);
var
  P3,rPos,rDir:TD3Vector;
begin
  FDisableLayerEvent:=true;
  try
    inherited;
  finally
    FDisableLayerEvent:=false;
  end;
  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((-P3.z + (Depth / 2)) / Depth) * FLayerHeight);
    end
    else
      Exit;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((P3.y + (Height / 2)) / Height) * FLayerHeight);
    end
    else
      Exit;
  end;
  LayerMouseDown(Button,Shift,X,Y);
end;

procedure TD3CustomBufferLayer.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single; rayPos,rayDir:TD3Vector);
var
  P3,rPos,rDir:TD3Vector;
begin
  FDisableLayerEvent:=true;
  try
    inherited;
  finally
    FDisableLayerEvent:=false;
  end;
  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((-P3.z + (Depth / 2)) / Depth) * FLayerHeight);
    end;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((P3.y + (Height / 2)) / Height) * FLayerHeight);
    end;
  end;
  LayerMouseUp(Button,Shift,X,Y);
end;

procedure TD3CustomBufferLayer.SetResolution(const Value:integer);
begin
  if FResolution <> Value then
  begin
    FResolution:=Value;
    if Projection=d3ProjectionCamera then
    begin
      FLayerWidth:=Round(Width * FResolution);
      FLayerHeight:=Round(Depth * FResolution);
    end;
    Repaint;
  end;
end;

procedure TD3CustomBufferLayer.SetWidth(const Value:single);
begin
  inherited SetWidth(Value);
  if Projection=d3ProjectionCamera then
  begin
    if (Round(Width * FResolution) <> FLayerWidth) then
    begin
      FLayerWidth:=Round(Width * FResolution);
    end;
  end
  else
    FLayerWidth:=Round(Width);
end;

procedure TD3CustomBufferLayer.SetDepth(const Value:single);
begin
  if Projection=d3ProjectionCamera then
  begin
    inherited SetDepth(Value);
    if (Round(Depth * FResolution) <> FLayerHeight) then
    begin
      FLayerHeight:=Round(Depth * FResolution);
    end;
  end
  else
    inherited SetDepth(Value);
end;

procedure TD3CustomBufferLayer.SetHeight(const Value:single);
begin
  if Projection=d3ProjectionCamera then
    inherited SetHeight(Value)
  else
  begin
    inherited SetHeight(Value);
    FLayerHeight:=Round(Height);
  end;
end;

function TD3CustomBufferLayer.GetModulationColor:string;
begin
  Result:=d3ColorToStr(FModulationColor);
end;

procedure TD3CustomBufferLayer.SetModulationColor(const Value:string);
begin
  if FModulationColor <> d3StrToColor(Value) then
  begin
    FModulationColor:=d3StrToColor(Value);
    Repaint;
  end;
end;
         
function BigEndianColor(C:cardinal):cardinal;
begin
  Result:=C;
  d2ReverseBytes(@Result,4);
end;


//============== TD3CustomObjectLayer ===================================

constructor TD3CustomObjectLayer.Create(AOwner:TComponent);
begin
  inherited;
  AddScene(Self);
  CanFocused:=true;
  FAnimatedCaret:=true;
  FDesignTime:=csDesigning in ComponentState;
  FCanvas:=orca_scene2d.GvarD2DefaultCanvasClass.Create(FLayerWidth,FLayerHeight);
  ShowHint:=true;
  DragDisableHighlight:=true;
  Width:=8;
  Depth:=8;
  AutoCapture:=true;
  FDesignInteract:=true;
  FSnapToLines:=true;
  FAlignRoot:=true;
  TxxHackCanvas(FCanvas).FBuffered:=true;
  FFill:=TD2Brush.Create(d2BrushSolid,$FF505050);
  FFill.OnChanged:=FillChanged;
  DesignPopupEnabled:=true;
  FSnapGridSize:=1;
  Fill.Style:=d2BrushNone;
  if GvarD2Designer <> nil then
    GvarD2Designer.AddScene(Self);
  GvarD2SceneCount:=GvarD2SceneCount + 1;
end;

destructor TD3CustomObjectLayer.Destroy;
begin
  if FHovered <> nil then
  begin
    TD2VisualObject(FHovered).RemoveFreeNotify(Self);
    FHovered:=nil;
  end;
  if FFocused <> nil then
  begin
    TD2VisualObject(FFocused).RemoveFreeNotify(Self);
    FFocused:=nil;
  end;
  if GvarD2Designer <> nil then
    GvarD2Designer.RemoveScene(Self);
  GvarD2SceneCount:=GvarD2SceneCount - 1;
  if GvarD2SceneCount=0 then
  begin
    if orca_scene2d.GvarD2aniThread <> nil then
    begin
      orca_scene2d.GvarD2aniThread.Free;
    end;
    orca_scene2d.GvarD2aniThread:=nil;
  end;
  if Assigned(FSaveIdle) then
    Application.OnIdle:=FSaveIdle;
  if FOpenInFrame <> nil then
    FOpenInFrame.SceneObject:=nil;
  DeleteChildren;
  if FChildren <> nil then
    FreeAndNil(FChildren);
  FreeAndNil(FFill);
  FreeAndNil(FCanvas);
  RemoveScene(Self);
  inherited;
end;

procedure TD3CustomObjectLayer.Loaded;
begin
  inherited;
  FLoadCursor:=Cursor;
  if FSnapToLines then
    FSnapToGrid:=false;
  FShowTimer:=TTimer.Create(Self);
  FShowTimer.Interval:=1;
  FShowTimer.OnTimer:=DoShowTimer;
end;

procedure TD3CustomObjectLayer.DoShowTimer(Sender:TObject);
begin
  FShowTimer.Enabled:=false;
  AddUpdateRect(d2Rect(0,0,$FFFF,$FFFF));
  FShowTimer.Free;
end;

procedure TD3CustomObjectLayer.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  i:integer;
begin
  inherited;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if (TObject(FChildren[i]) is TD2Object) and (TD2Object(FChildren[i]).Stored) then
        Proc(FChildren[i]);
end;

procedure TD3CustomObjectLayer.AddUpdateRect(R:TD2Rect);
begin
  if FDisableUpdate then Exit;
  if csDestroying in ComponentState then Exit;
  if FCanvas=nil then Exit;
  if FCanvas.BufferBits=nil then Exit;

  R:=d2Rect(Trunc(R.Left),Trunc(R.Top),Trunc(R.Right) + 1,Trunc(R.Bottom) + 1);
  if not d2IntersectRect(R,d2Rect(0,0,FLayerWidth,FLayerHeight)) then Exit;

  SetLength(FUpdateRects,Length(FUpdateRects) + 1);
  FUpdateRects[High(FUpdateRects)]:=R;
  Repaint;
end;

procedure TD3CustomObjectLayer.Draw;
var
  i,j:integer;
  R:TD2Rect;
  Rgn,NRgn:Cardinal;
  ScaleMatrix:TD2Matrix;
begin
  if not (FDesignTime) and (FOpenInFrame <> nil) then Exit;
  if FDrawing then Exit;
  if Length(FUpdateRects) > 0 then
  begin
    FDrawing:=true;
    try
      { Split rects if rects too more }
      if (Length(FUpdateRects) > 20) then
      begin
        for i:=1 to High(FUpdateRects) do
          FUpdateRects[0]:=d2UnionRect(FUpdateRects[0],FUpdateRects[i]);
        SetLength(FUpdateRects,1);
      end;
      if Canvas.BeginScene then
      begin
        { draw back }
        Canvas.ResetClipRect;
        ScaleMatrix:=orca_scene2d.IdentityMatrix;
        Canvas.SetMatrix(ScaleMatrix);
        Canvas.SetClipRects(FUpdateRects);

        if (FFill.Style=d2BrushNone) or ((FFill.SolidColor and $FF000000=0) and (FFill.Style=d2BrushSolid)) then
        begin
          for i:=0 to High(FUpdateRects) do
          begin
            if FTransparency then
              Canvas.ClearRect(FUpdateRects[i],0)
            else
              {$ifdef FPC_BIG_ENDIAN}
              Canvas.ClearRect(FUpdateRects[i],BigEndianColor(FFill.SolidColor and $FFFFFF));
              {$ELSE}
              Canvas.ClearRect(FUpdateRects[i],FFill.SolidColor and $FFFFFF);
              {$ENDIF}
          end;
        end
        else
        begin
          Canvas.Fill.Assign(FFill);
          Canvas.FillRect(d2Rect(-1,-1,FLayerWidth + 1,FLayerHeight + 1),0,0,AllCorners,1);
        end;
        { reset }
        Canvas.StrokeThickness:=1;
        Canvas.StrokeCap:=d2CapFlat;
        Canvas.StrokeJoin:=d2JoinMiter;
        Canvas.StrokeDash:=d2DashSolid;
        Canvas.Stroke.Style:=d2BrushSolid;
        Canvas.Fill.Style:=d2BrushSolid;
        if FChildren <> nil then
          for i:=0 to FChildren.Count - 1 do
          begin
            if not (TObject(FChildren[i]) is TD2VisualObject) then Continue;
            if not TD2VisualObject(FChildren[i]).Visible then Continue;

            ScaleMatrix:=orca_scene2d.IdentityMatrix;
            for j:=0 to High(FUpdateRects) do
              if d2IntersectRect(FUpdateRects[j],TD2VisualObject(FChildren[i]).UpdateRect) then
              begin
                Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,TD2VisualObject(FChildren[i]).AbsoluteMatrix));
                TD2HackVisualObject(FChildren[i]).BeforePaint;
                TD2HackVisualObject(FChildren[i]).Paint;
                TD2HackVisualObject(FChildren[i]).PaintChildren;
                Break;
              end;
          end;
        { grid }
        if FSnapGridShow and (FSnapGridSize <> 0) then
        begin
          ScaleMatrix:=orca_scene2d.IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);
          Canvas.Stroke.Style:=d2BrushSolid;
          Canvas.StrokeThickness:=1;
  (*        for i:=Trunc((-FDesignScroll.X) / (FSnapGridSize)) - 1 to Trunc((-FDesignScroll.X + Width) / (FSnapGridSize)) + 1 do
          begin
            if i mod 5=0 then
              Canvas.Stroke.SolidColor:=$50505050
            else
              Canvas.Stroke.SolidColor:=$50303030;
            Canvas.DrawLine(d2Point(i * FSnapGridSize + 0.5,-FDesignScroll.Y + 0.5),d2Point(i * FSnapGridSize + 0.5,-FDesignScroll.Y + Height + 0.5),1);
          end;
          for j:=Trunc((-FDesignScroll.Y) / (FSnapGridSize)) - 1 to Trunc((-FDesignScroll.Y + Height) / (FSnapGridSize)) + 1 do
          begin
            if j mod 5=0 then
              Canvas.Stroke.SolidColor:=$50505050
            else
              Canvas.Stroke.SolidColor:=$50303030;
            Canvas.DrawLine(d2Point(-FDesignScroll.X + 0.5,j * FSnapGridSize + 0.5),d2Point(-FDesignScroll.X + Width + 0.5,j * FSnapGridSize + 0.5),1)
          end; *)
        end;
        { design }
        if (FSelected <> nil) and not FSelected.DisableDesignResize then
        begin
          Canvas.Fill.Style:=d2BrushSolid;
          Canvas.Fill.SolidColor:=$FFFFFFFF;
          Canvas.StrokeThickness:=1;
          Canvas.Stroke.Style:=d2BrushSolid;
          Canvas.Stroke.SolidColor:=$FF1072C5;
          ScaleMatrix:=orca_scene2d.IdentityMatrix;
          Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,FSelected.AbsoluteMatrix));
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.StrokeDash:=d2DashDash;
          Canvas.DrawRect(R,0,0,AllCorners,1);
          Canvas.StrokeDash:=d2DashSolid;
          begin
          { rotate }
          if FRotateHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.DrawLine(d2Point((R.Left + R.Right) / 2,R.Top),d2Point((R.Left + R.Right) / 2,R.Top - cnRotSize),1);
          Canvas.Fillellipse(d2Rect((R.Left + R.Right) / 2 - (GripSize),R.Top - cnRotSize - (GripSize),
            (R.Left + R.Right) / 2 +(GripSize),R.Top - cnRotSize + (GripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect((R.Left + R.Right) / 2 - (GripSize),R.Top - cnRotSize - (GripSize),
            (R.Left + R.Right) / 2 +(GripSize),R.Top - cnRotSize + (GripSize)),Opaque);
          { angles }
          if FLeftTopHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.Fillellipse(d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize)),Opaque);

          if FRightTopHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.Fillellipse(d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize)),Opaque);

          if FLeftBottomHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.Fillellipse(d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize)),Opaque);

          if FRightBottomHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.FillEllipse(d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize)),Opaque);
          { lines }
          if FSelected.Width > GripSize * 4 then
          begin
            if FTopHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Left + d2RectWidth(R)/2 - (GripSize),R.Top - (GripSize),R.Left + d2RectWidth(R)/2 + (GripSize),R.Top + (GripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Left + d2RectWidth(R)/2 - (GripSize),R.Top - (GripSize),R.Left + d2RectWidth(R)/2 + (GripSize),R.Top + (GripSize)),0,0,[],Opaque);
            if FBottomHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Left + d2RectWidth(R)/2 - (GripSize),R.Bottom - (GripSize),R.Left + d2RectWidth(R)/2 + (GripSize),R.Bottom + (GripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Left + d2RectWidth(R)/2 - (GripSize),R.Bottom - (GripSize),R.Left + d2RectWidth(R)/2 + (GripSize),R.Bottom + (GripSize)),0,0,[],Opaque);
          end;
          if FSelected.Height > GripSize * 4 then
          begin
            if FLeftHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Left - (GripSize),R.Top + d2RectHeight(R)/2 - (GripSize),R.Left + (GripSize),R.Top + d2RectHeight(R)/2 + (GripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Left - (GripSize),R.Top + d2RectHeight(R)/2 - (GripSize),R.Left + (GripSize),R.Top + d2RectHeight(R)/2 + (GripSize)),0,0,[],Opaque);
            if FRightHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Right - (GripSize),R.Top + d2RectHeight(R)/2 - (GripSize),R.Right + (GripSize),R.Top + d2RectHeight(R)/2 + (GripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Right - (GripSize),R.Top + d2RectHeight(R)/2 - (GripSize),R.Right + (GripSize),R.Top + d2RectHeight(R)/2 + (GripSize)),0,0,[],Opaque);
          end;
          { grid lines }
          if (FMoving or FLeftTop or FRightTop or FLeftBottom or FRightBottom or FTop or FBottom or FLeft or FRight) and
             (Length(FDesignGridLines) > 0) and (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
          begin
            ScaleMatrix:=orca_scene2d.IdentityMatrix;
            Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,TD2VisualObject(FSelected.Parent).AbsoluteMatrix));
            Canvas.StrokeDash:=d2DashDash;
            for i:=0 to High(FDesignGridLines) do
            begin
              if (FDesignGridLines[i].Position.Y + round(FDesignGridLines[i].Height / 2))=(FSelected.Position.Y + round(FSelected.Height / 2)) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,Trunc(FSelected.Position.Y + (FSelected.Height / 2)) + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + 0.5,Trunc(FSelected.Position.Y + (FSelected.Height / 2)) + 0.5),1);
              end;
              if (FDesignGridLines[i].Position.X + round(FDesignGridLines[i].Width / 2))=(FSelected.Position.X + round(FSelected.Width / 2)) then
              begin
                Canvas.DrawLine(d2Point(Trunc(FSelected.Position.X + (FSelected.Width / 2)) + 0.5,FSelected.Position.Y + 0.5),
                  d2Point(Trunc(FDesignGridLines[i].Position.X + (FDesignGridLines[i].Width / 2)) + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.X=FDesignGridLines[i].Position.X) or (FSelected.Position.Y=FDesignGridLines[i].Position.Y) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FSelected.Position.Y + 0.5),d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.X + FSelected.Width=FDesignGridLines[i].Position.X) then
              begin
                Canvas.DrawLine(d2Point(FDesignGridLines[i].Position.X + 0.5,FSelected.Position.Y + 0.5),d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.Y + FSelected.Height=FDesignGridLines[i].Position.Y) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.X=FDesignGridLines[i].Position.X + FDesignGridLines[i].Width) then
              begin
                Canvas.DrawLine(d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FSelected.Position.Y + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.Y=FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),1);
              end;
              if (FSelected.Position.X + FSelected.Width=FDesignGridLines[i].Position.X + FDesignGridLines[i].Width) then
              begin
                Canvas.DrawLine(d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FSelected.Position.Y + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.Y + FSelected.Height=FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),1);
              end;
            end;
            Canvas.StrokeDash:=d2DashSolid;
          end;
          end;
          { place message }
          if FDesignPlaceObject <> nil then
          begin
            ScaleMatrix:=orca_scene2d.IdentityMatrix;
            Canvas.SetMatrix(ScaleMatrix);

            R:=FDesignPlaceObject.AbsoluteRect;
            Canvas.Stroke.SolidColor:=$FF5B91DE;
            Canvas.DrawRect(R,0,0,AllCorners,1);
            Canvas.Font.Family:='Tahoma';
            Canvas.Font.Style:=d2FontRegular;
            Canvas.Font.Size:=9;
            R.Bottom:=R.Top;
            R.Top:=R.Bottom - 19;
            R.Right:=R.Left + 160;
            Canvas.Fill.SolidColor:=$FF5B91DE;
            Canvas.FillRect(R,0,0,AllCorners,1);
            Canvas.Fill.SolidColor:=$FFFFFFFF;
            d2InflateRect(R,-2,-2);
            if FDesignPlaceObject.Name <> '' then
              Canvas.FillText(R,R,'ALT-drag to place into [' + FDesignPlaceObject.Name + ']',false,1,d2TextAlignNear)
            else
              Canvas.FillText(R,R,'ALT-drag to place into [' + FDesignPlaceObject.ClassName + ']',false,1,d2TextAlignNear);
          end;
        end;
        { design modes }
        if FDesignTime then
        begin
          ScaleMatrix:=orca_scene2d.IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);

          Canvas.Stroke.SolidColor:=$FF5B91DE;
          Canvas.Font.Family:='Tahoma';
          Canvas.Font.Style:=d2FontRegular;
          Canvas.Font.Size:=9;
          R:=d2Rect(0,0,200,17);
        end;
        { debug }
        {$IFDEF UPDATERECT}
        with Canvas do
        begin
          ResetClipRect;
          ScaleMatrix:=orca_d2core.IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);
          Stroke.Style:=d2BrushSolid;
          Stroke.Color:='#90FF0000';
          StrokeThickness:=1;
          Fill.Style:=d2BrushNone;
          for i:=0 to High(FUpdateRects) do
          begin
            R:=FUpdateRects[i];
            DrawRect(FUpdateRects[i],0,0,AllCorners,0.5);
          end;
        end;
        {$ENDIF}
        Canvas.EndScene;
      end;
    finally
      setLength(FUpdateRects,0);
      FDrawing:=false;
    end;
  end;
end;

procedure TD3CustomObjectLayer.BeforePaint;
begin
  if (FBuffer.Width <> FLayerWidth) or (FBuffer.Height <> FLayerHeight) then
  begin
    AddUpdateRect(d2Rect(0,0,FLayerWidth,FLayerHeight));
  end;
  if (FCanvas <> nil) and ((FCanvas.Width <> FLayerWidth) or (FCanvas.Height <> FLayerHeight)) then
  begin
    FCanvas.ResizeBuffer(FLayerWidth,FLayerHeight);
    RealignRoot;
  end;
  inherited;
end;

procedure TD3CustomObjectLayer.Paint;
var
  B:PD3colorArray;
  i,j:integer;
begin
  { Update d3 texture using d2Scene canvas }
  if FBuffer <> nil then
  begin
    if Length(FUpdateRects) > 0 then
    begin
      Draw;
      if FBuffer.LockBitmapBits(B,true) then
      begin
        {$IFDEF DARWIN}
        {$IFDEF FPC_BIG_ENDIAN}
        for j:=0 to FBuffer.Height - 1 do
          for i:=0 to FBuffer.Width - 1 do
          begin
            B[i + (j * FBuffer.Width)]:=PD3ColorArray(FCanvas.FBufferBits)[i + j * FBuffer.Width];
            ReverseBytes(@B[i + (j * FBuffer.Width)],4);
          end;
        {$ELSE}
        FCanvas.SaveToBits(B);
        {$ENDIF}
        {$ELSE}
        FCanvas.SaveToBits(B);
        {$ENDIF}
        FBuffer.UnlockBitmapBits;
      end;
    end;
  end;
  inherited ;
end;

function TD3CustomObjectLayer.ObjectByPoint(X,Y:single):TD2VisualObject;
var
  i:integer;
  Obj,NewObj:TD2Object;
begin
  Result:=nil;
  for i:=Count - 1 downto 0 do
  begin
    Obj:=Children[i];
    if not (Obj is TD2VisualObject) then Exit;
    if not TD2VisualObject(Obj).Visible and not (FDesignTime) then Continue;

    NewObj:=TD2VisualObject(Obj).ObjectByPoint(X,Y);
    if NewObj <> nil then
    begin
      Result:=TD2VisualObject(NewObj);
      Exit;
    end;
  end;
end;

procedure TD3CustomObjectLayer.KeyDown(var Key: word;
  var char: System.widechar; Shift: TShiftState);
begin
  if FFocused <> nil then
  begin
    TD2HackVisualObject(FFocused).KeyDown(Key,Char,Shift);
  end;
end;

procedure TD3CustomObjectLayer.KeyUp(var Key: word; var char: System.widechar;
  Shift: TShiftState);
begin
  if FDesignTime or (FDesignTime) then
  begin
    if (Key=VK_DELETE) and (FSelected <> nil) then
    begin
      FSelected.Free;
      FSelected:=nil;
    end;
  end;
  { focused handler }
  if FFocused <> nil then
  begin
    TD2HackVisualObject(FFocused).KeyUp(Key,Char,Shift);
  end;
end;

procedure TD3CustomObjectLayer.DeleteChildren;
var
  Child:TD2Object;
begin
  if Assigned(FChildren) then
  begin
    while FChildren.Count > 0 do
    begin
      Child:=TD2Object(FChildren[0]);
      FChildren.Delete(0);
      TD2HackObject(Child).FParent:=nil;
      TD2HackObject(Child).FScene:=nil;
      Child.Free;
    end;
    FreeAndNil(FChildren);
  end;
end;

procedure TD3CustomObjectLayer.GetTabOrderList(const AList:TList);
begin
  if Assigned(Root) then
    TD2VisualObject(Root).GetTabOrderList(AList,true);
end;

procedure TD3CustomObjectLayer.AddObject(AObject:TD2Object);
begin
  if AObject.Parent <> nil then
    AObject.Parent:=nil;
  if FChildren=nil then
    FChildren:=TList.Create;
  FChildren.Add(AObject);
  TD2HackObject(AObject).FScene:=Self;
  if AObject.IsVisual and not (csDestroying in ComponentState) then
    TD2VisualObject(AObject).Repaint;
  RealignRoot;
end;

procedure TD3CustomObjectLayer.RemoveObject(AObject:TD2Object);
begin
  if FChildren <> nil then
  begin
    Repaint;
    FChildren.Remove(AObject);
  end;
end;

function TD3CustomObjectLayer.GetCount:integer;
begin
  if FChildren <> nil then
    Result:=FChildren.Count
  else
    Result:=0;
end;

function TD3CustomObjectLayer.GetChildrenObject(Index:integer):TD2Object;
begin
  if FChildren <> nil then
    Result:=TD2Object(FChildren[Index])
  else
    Result:=nil;
end;

procedure TD3CustomObjectLayer.SetChildren(Index:integer; const Value:TD2Object);
begin
end;

function TD3CustomObjectLayer.DoHintShow(var Message: TLMessage ):boolean;
var
  Obj:TD2VisualObject;
  P:TD2Point;
  i:integer;
  P3,rayPos,rayDir:TD3Vector;
  X,Y:single;
begin
  Result:=inherited DoHintShow(Message);
  with TCMHintShow(Message).HintInfo^ do
  begin
    inherited Canvas.Pick(CursorPos.X,CursorPos.Y,Projection,rayPos,rayDir);
    rayPos:=AbsoluteToLocalVector(rayPos);
    rayDir:=AbsoluteToLocalVector(rayDir);
    if Projection=d3ProjectionCamera then
    begin
      if RayCastIntersect(rayPos,rayDir,P3) then
      begin
        P3:=AbsoluteToLocalVector(P3);
        X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
        Y:=(((-P3.z + (Depth / 2)) / Depth) * FLayerHeight);
      end
      else
        Exit;
    end
    else
    begin
      if RayCastIntersect(rayPos,rayDir,P3) then
      begin
        P3:=AbsoluteToLocalVector(P3);
        X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
        Y:=(((P3.y + (Height / 2)) / Height) * FLayerHeight);
      end
      else
        Exit;
    end;

    Obj:=ObjectByPoint(X,Y);
    if (Obj <> nil) and (Obj.ShowHint) then
    begin
      HintStr:=Obj.Hint;
      with LocalToAbsolute(d3Point(Obj.Position.X,Obj.Position.Y,0)) do
        CursorRect:=types.Rect(Trunc(X),Trunc(Y),Trunc(X + Obj.Width),Trunc(Y + Obj.Height));
      Result:=true;
    end;
  end;
end;

function TD3CustomObjectLayer.SnapToGridValue(Value:single):single;
begin
  if (DesignSnapToGrid) and (DesignSnapGridSize <> 0) then
    Result:=Trunc(Value / DesignSnapGridSize) * DesignSnapGridSize
  else
    Result:=Value;
end;

procedure TD3CustomObjectLayer.AddUpdateRectsFromGridLines;
  procedure IntAddUpdateRect(const R:TD2Rect);
  var
    i:integer;
  begin
    for i:=0 to High(FUpdateRects) do
      with FUpdateRects[i] do
        if (R.Left=Left) and (R.Top=Top) and (R.Right=Right) and (R.Bottom=Bottom) then
        begin
          Exit;
        end;
    AddUpdateRect(R);
  end;
var
  i:integer;
begin
  { Add grip lines }
  if FDesignTime and (FSelected <> nil) and not FSelected.DisableDesignResize and
     (FMoving or FLeftTop or FRightTop or FLeftBottom or FRightBottom or FTop or FBottom or FLeft or FRight) and
     (Length(FDesignGridLines) > 0) and (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
  begin
    for i:=0 to High(FDesignGridLines) do
    begin
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FSelected.Position.Y + (FSelected.Height / 2) - 1,
        FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + (FDesignGridLines[i].Height / 2) + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X + (FSelected.Width / 2) - 1,FSelected.Position.Y - 1,
        FDesignGridLines[i].Position.X + (FDesignGridLines[i].Width / 2) + 1,FDesignGridLines[i].Position.Y + 1)));

      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FSelected.Position.Y - 1,FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FDesignGridLines[i].Position.X - 1,FSelected.Position.Y - 1,FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FDesignGridLines[i].Position.Y - 1,FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width - 1,FSelected.Position.Y - 1,
          FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height - 1,
          FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width - 1,FSelected.Position.Y - 1,
          FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height - 1,
          FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 1)));
    end;
  end;
end;

procedure TD3CustomObjectLayer.SnapToGridLines(AllowChangePosition:boolean);
  procedure AddGridLine(const Obj:TD2VisualObject);
  var
    i:integer;
  begin
    for i:=0 to High(FDesignGridLines) do
      if FDesignGridLines[i]=Obj then Exit;
    SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
    FDesignGridLines[High(FDesignGridLines)]:=Obj;
  end;
const
  SnapLineSize=2;
var
  i:integer;
begin
  if (DesignSnapToLines) and (FSelected.Parent <> nil) then
    for i:=0 to FSelected.Parent.ChildrenCount - 1 do
    begin
      if TD2Object(FSelected.Parent.Children[i])=FSelected then Continue;
      if not TD2Object(FSelected.Parent.Children[i]).isVisual then Continue;
      with TD2VisualObject(FSelected.Parent.Children[i]) do
      begin
        if (Abs((Position.Y + round(Height / 2)) - (FSelected.Position.Y + round(FSelected.Height / 2))) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + ((Position.Y + round(Height / 2)) - (FSelected.Position.Y + round(FSelected.Height / 2)));
            FSelected.Position.Y:=(Position.Y + round(Height / 2) - round(FSelected.Height / 2));
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.X + round(Width / 2)) - (FSelected.Position.X + round(FSelected.Width / 2))) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + ((Position.X + round(Width / 2)) - (FSelected.Position.X + round(FSelected.Width / 2)));
            FSelected.Position.X:=(Position.X + round(Width / 2) - round(FSelected.Width / 2));
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.X - FSelected.Position.X) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + (Position.X - FSelected.Position.X);
            FSelected.Position.X:=Position.X;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.Y - FSelected.Position.Y) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + (Position.Y - FSelected.Position.Y);
            FSelected.Position.Y:=Position.Y;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.X - (FSelected.Position.X + FSelected.Width)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + (Position.X - (FSelected.Position.X + FSelected.Width));
            FSelected.Position.X:=Position.X - FSelected.Width;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.Y - (FSelected.Position.Y + FSelected.Height)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + (Position.Y - (FSelected.Position.Y + FSelected.Height));
            FSelected.Position.Y:=Position.Y - FSelected.Height;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.X + Width) - FSelected.Position.X) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + ((Position.X + Width) - FSelected.Position.X);
            FSelected.Position.X:=Position.X + Width;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.Y + Height) - FSelected.Position.Y) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + ((Position.Y + Height) - FSelected.Position.Y);
            FSelected.Position.Y:=Position.Y + Height;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.X + Width) - (FSelected.Position.X + FSelected.Width)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + ((Position.X + Width) - (FSelected.Position.X + FSelected.Width));
            FSelected.Position.X:=Position.X + Width - FSelected.Width;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.Y + Height) - (FSelected.Position.Y + FSelected.Height)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + ((Position.Y + Height) - (FSelected.Position.Y + FSelected.Height));
            FSelected.Position.Y:=Position.Y + Height - FSelected.Height;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
      end;
    end;
  AddUpdateRectsFromGridLines;
end;

function TD3CustomObjectLayer.SnapPointToGridLines(const APoint:TD2Point):TD2Point;
var
  i:integer;
begin
  Result:=APoint;
  if not DesignSnapToLines then Exit;
  if FSelected=nil then Exit;
  if FSelected.Parent=nil then Exit;
  SnapToGridLines(false);
  if Length(FDesignGridLines) > 0 then
  begin
    Result:=FSelected.LocalToAbsolute(APoint);
    Result:=TD2VisualObject(FSelected.Parent).AbsoluteToLocal(Result);
    for i:=0 to High(FDesignGridLines) do
    begin
      if Abs(Result.X - FDesignGridLines[i].Position.X) < (4) then
        Result.X:=FDesignGridLines[i].Position.X;
      if Abs(Result.Y - FDesignGridLines[i].Position.Y) < (4) then
        Result.Y:=FDesignGridLines[i].Position.Y;
      if Abs(Result.X - (FDesignGridLines[i].Position.X + FDesignGridLines[i].Width)) < (4) then
        Result.X:=FDesignGridLines[i].Position.X + FDesignGridLines[i].Width;
      if Abs(Result.Y - (FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height)) < (4) then
        Result.Y:=FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height;
    end;
    Result:=TD2VisualObject(FSelected.Parent).LocalToAbsolute(Result);
    Result:=FSelected.AbsolutetoLocal(Result);
  end;
end;

function TD3CustomObjectLayer.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
var
  V:TD3Vector;
  x,y:single;
begin
  Result:=inherited RayCastIntersect(RayPos,RayDir,Intersection);
  if Result and not FDesignTime and (Scene <> nil) and (Scene.Captured <> Self) and not AllowDrag then
  begin
    V:=AbsoluteToLocalVector(Intersection);
    if Projection=d3ProjectionCamera then
    begin
      X:=(((V.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((-V.z + (Depth / 2)) / Depth) * FLayerHeight);
    end
    else
    begin
      X:=(((V.x + (Width / 2)) / Width) * FLayerWidth);
      Y:=(((V.y + (Height / 2)) / Height) * FLayerHeight);
    end;
    Result:=ObjectByPoint(X,Y) <> nil;
  end;
end;

procedure TD3CustomObjectLayer.LayerMouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
var
  P:TD2Point;
  R:TD2Rect;
  Obj:TD2VisualObject;
  InsertObject:TD2Object;
  SG:Id2SizeGrip;
  i:integer;
begin
  inherited;
  if (Assigned(Scene) and not (Scene.DesignTime)) and not (FDesignTime) then
    SetFocus;
  { translate coord }
  FUnsnapMousePos:=d2Point(x,y);
  FMousePos:=d2Point(SnapToGridValue(x),SnapToGridValue(y));
  FDownPos:=FMousePos;
  SetLength(FDesignGridLines,0);
  { design }
  if FDesignTime then
  begin
    { Create root }
    if (FChildren=nil) or (FChildren.Count=0) then
    begin
      Obj:=TD2Layout.Create(Owner);
      if GvarD2Designer <> nil then
        Obj.Name:=GvarD2Designer.UniqueName(Owner,'Root');
      AddObject(Obj);
      RealignRoot;
    end;
    { Popup }
    if Button=mbRight then
    begin
      FPopupPos:=FMousePos;
      OpenDesignPopup;
    end;
    { Resize }
    if (FSelected <> nil) then
    begin
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(FSelected.Width / 2 - (GripSize),- cnRotSize - (GripSize),
          (FSelected.Width) / 2 +(GripSize),- cnRotSize + (GripSize));
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRotate:=true;
        FMoving:=false;
        Exit;
      end;
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(-GripSize,-GripSize,GripSize,GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FLeftTop:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right -GripSize,-GripSize,R.Right + GripSize,GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRightTop:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-GripSize,R.Bottom - GripSize,GripSize,R.Bottom + GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FLeftBottom:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right - GripSize,R.Bottom - GripSize,R.Right + GripSize,R.Bottom + GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRightBottom:=true;
        FMoving:=false;
        Exit;
      end;

      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - GripSize,-GripSize,d2RectWidth(R)/2 + GripSize,GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FTop:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - GripSize,R.Bottom - GripSize,d2RectWidth(R)/2 + GripSize,R.Bottom + GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FBottom:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-GripSize,d2RectHeight(R)/2 - GripSize,GripSize,d2RectHeight(R)/2 + GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FLeft:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right-GripSize,d2RectHeight(R)/2 - GripSize,R.Right + GripSize,d2RectHeight(R)/2 + GripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRight:=true;
        FMoving:=false;
        Exit;
      end;
    end;
    { Change Selected }
    Obj:=ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y);
    if (Obj <> nil) and (Obj=FSelected) and (ssDouble in Shift) then
    begin
      TD2HackVisualObject(Obj).DesignClick;
    end;
    if (Obj <> nil) then
    begin
      if (ssCtrl in Shift) and (Obj <> Selected) then
      begin
        { check is exists }
        for i:=0 to High(FSelection) do
          if FSelection[i]=Obj then
          begin
            FSelection[i]:=Selected;
            Obj:=nil;
          end;
        if Obj <> nil then
        begin
          SetLength(FSelection,Length(FSelection) + 1);
          FSelection[High(FSelection)]:=Obj;
        end;
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,Selected,FSelection);
        Exit;
      end;
      SetLength(FSelection,0);

      if FSelected <> nil then
        FSelected.Repaint;
      FSelected:=Obj;
      TD2HackVisualObject(FSelected).DesignSelect;
      { Select in IDE }
      if GvarD2Designer <> nil then
      begin
        GvarD2Designer.SelectObject(Owner,Obj,[]);
      end;
      if Assigned(FDesignChangeSelection) then
        FDesignChangeSelection(Self);
      { }
      FSelected.Repaint;
      if (Obj=FSelected) then
        FMoving:=true;
    end;
    Exit;
  end;
  { event }
  if not FDesignTime then
  begin
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) then
    begin
      if (TD2HackObject(Obj).QueryInterface(Id2SizeGrip,SG)=0) and (Assigned(SG)) then
      begin
        BeginResize;
      end
      else
      begin
        P:=Obj.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
        TD2HackVisualObject(Obj).MouseDown(Button,Shift,P.X,P.Y);
        if (TD2HackVisualObject(Obj).DragMode=d2DragAutomatic) then
          TD2HackVisualObject(Obj).BeginAutoDrag;
      end;
    end
    else
      if AllowDrag then
        BeginDrag;
  end;
end;

procedure TD3CustomObjectLayer.LayerMouseMove(Shift:TShiftState; X,Y:single);
var
  R:TD2Rect;
  P,P1:TD2Point;
  Obj:TD2VisualObject;
  SG:Id2SizeGrip;
  downP:TD3Vector;
  P3:TD3Vector;
  NewCursor:TCursor;
begin
  inherited;
  NewCursor:=FLoadCursor;
  if FDragging then
  begin
    P3:=LocalToAbsoluteVector(d3Vector(X - FDownPos.X,Y - FDownPos.Y,0,0));
    P3:=TD3VisualObject(Parent).AbsoluteToLocalVector(P3);
    Position.Point:=d3Point(Position.X + P3.X,Position.Y + P3.Y,Position.Z);
    Exit;
  end;
  if FResizing then
  begin
    FResizePos.X:=FResizePos.X + (X - FMousePos.X) / 2;
    FResizePos.Y:=FResizePos.Y + (Y - FMousePos.Y) / 2;
    FResizeSize.X:=FResizeSize.X + (X - FMousePos.X);
    FResizeSize.Y:=FResizeSize.Y + (Y - FMousePos.Y);
    Width:=round(FResizeSize.X);
    Height:=round(FResizeSize.Y);
    if (FResizeStartPos.X <> Position.X - Width / 2) then
    begin
      Position.X:=FResizeStartPos.X + (Width / 2);
    end;
    if (FResizeStartPos.Y <> Position.Y - Height / 2) then
    begin
      Position.Y:=FResizeStartPos.Y + (Height / 2);
    end;
    FMousePos:=d2Point(x,y);
    Exit;
  end;
  { translate coord }
  FMousePos:=d2Point(SnapToGridValue(x),SnapToGridValue(y));
  FUnsnapMousePos:=d2Point(x,y);
  { design }
  if FDesignTime then
  begin
    { change cursor }
    if (FSelected <> nil) then
    begin
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(FSelected.Width / 2 - (GripSize),- cnRotSize - (GripSize),
        (FSelected.Width) / 2 +(GripSize),- cnRotSize + (GripSize));
      if FRotateHot <> d2PtInRect(P,R) then
      begin
        FRotateHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(-GripSize,-GripSize,GripSize,GripSize);
      if FLeftTopHot <> d2PtInRect(P,R) then
      begin
        FLeftTopHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right -GripSize,-GripSize,R.Right + GripSize,GripSize);
      if FRightTopHot <> d2PtInRect(P,R) then
      begin
        FRightTopHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-GripSize,R.Bottom - GripSize,GripSize,R.Bottom + GripSize);
      if FLeftBottomHot <> d2PtInRect(P,R) then
      begin
        FLeftBottomHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right - GripSize,R.Bottom - GripSize,R.Right + GripSize,R.Bottom + GripSize);
      if FRightBottomHot <> d2PtInRect(P,R) then
      begin
        FRightBottomHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;

      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - GripSize,-GripSize,d2RectWidth(R)/2 + GripSize,GripSize);
      if FTopHot <> d2PtInRect(P,R) then
      begin
        FTopHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - GripSize,R.Bottom - GripSize,d2RectWidth(R)/2 + GripSize,R.Bottom + GripSize);
      if FBottomHot <> d2PtInRect(P,R) then
      begin
        FBottomHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-GripSize,d2RectHeight(R)/2 - GripSize,GripSize,d2RectHeight(R)/2 + GripSize);
      if FLeftHot <> d2PtInRect(P,R) then
      begin
        FLeftHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right-GripSize,d2RectHeight(R)/2 - GripSize,R.Right + GripSize,d2RectHeight(R)/2 + GripSize);
      if FRightHot <> d2PtInRect(P,R) then
      begin
        FRightHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
    end;
    { resize and move }
    if (ssLeft in Shift) and (FSelected <> nil) and (FMoving) then
    begin
      P:=d2Point(FUnsnapMousePos.X - FDownPos.X,FUnsnapMousePos.Y - FDownPos.Y);
      if (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
      begin
        with TD2VisualObject(FSelected.Parent).AbsoluteToLocalVector(d2Vector(P.X,P.Y)) do
          P:=d2Point(X,Y);
      end
      else
      begin
        with FSelected.AbsoluteToLocalVector(d2Vector(P.X,P.Y)) do
          P:=d2Point(X,Y);
      end;
      FSelected.Position.X:=SnapToGridValue(FSelected.Position.X + P.X);
      FSelected.Position.Y:=SnapToGridValue(FSelected.Position.Y + P.Y);
      { lines grid }
      SnapToGridLines(true);
      { check place }
      TD2HackVisualObject(FSelected).FLocked:=true;
      Obj:=TD2VisualObject(ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      TD2HackVisualObject(FSelected).FLocked:=false;
      { select }
      if (Obj <> nil) and (Obj <> FSelected.Parent) and (Obj <> FSelected) then
      begin
        FDesignPlaceObject:=Obj;
        if (ssAlt in Shift) then
        begin
          P:=FSelected.LocalToAbsolute(d2Point(0,0));
          FSelected.Parent:=FDesignPlaceObject;
          P:=FDesignPlaceObject.AbsoluteToLocal(P);
          FSelected.Position.X:=P.X;
          FSelected.Position.Y:=P.Y;
        end;
      end
      else
        FDesignPlaceObject:=nil;
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FRotate) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P1:=FSelected.AbsoluteToLocal(FDownPos);
      if d2VectorCrossProductZ(d2Vector(P.X - (FSelected.Width / 2),P.Y - (FSelected.Height / 2)),
        d2Vector(P1.X - (FSelected.Width / 2),P1.Y - (FSelected.Height / 2))) < 0
      then
        FSelected.RotateAngle:=FSelected.RotateAngle + d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(P.X - (FSelected.Width / 2),P.Y - (FSelected.Height / 2)),
          d2Vector(P1.X - (FSelected.Width / 2),P1.Y - (FSelected.Height / 2)))))
      else
        FSelected.RotateAngle:=FSelected.RotateAngle - d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(P.X - (FSelected.Width / 2),P.Y - (FSelected.Height / 2)),
          d2Vector(P1.X - (FSelected.Width / 2),P1.Y - (FSelected.Height / 2)))));
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FLeftTop) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(P.X,P.Y,
        R.Right,R.Bottom);
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FRightTop) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(R.Left,P.Y,
        P.X,R.Bottom);
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FLeftBottom) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(P.X,R.Top,
        R.Right,P.Y);
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FRightBottom) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(R.Left,R.Top,P.X,P.Y);
    end;

    if (ssLeft in Shift) and (FSelected <> nil) and (FTop) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(R.Left,P.Y,R.Right,R.Bottom);
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FBottom) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(R.Left,R.Top,R.Right,P.Y);
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FLeft) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(P.X,R.Top,R.Right,R.Bottom);
    end;
    if (ssLeft in Shift) and (FSelected <> nil) and (FRight) then
    begin
      R:=FSelected.BoundsRect;
      P:=FSelected.AbsoluteToLocal(FMousePos);
      P:=SnapPointToGridLines(P);
      FSelected.BoundsRect:=d2Rect(R.Left,R.Top,P.X,R.Bottom);
    end;
    FDownPos:=FMousePos;
    Exit;
  end;
  { event }
  if not FDesignTime then
  begin
    if (FCaptured <> nil) then
    begin
      P:=FCaptured.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2HackVisualObject(FCaptured).MouseMove(Shift,P.X,P.Y,0,0);
      Exit;
    end;
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) then
    begin
{      if (Obj.QueryInterface(Id2SizeGrip,SG)=0) and Assigned(SG) then
        NewCursor:=crSizeNWSE
      else}
        NewCursor:=Obj.Cursor;

      if (Obj <> FHovered) then
      begin
        if FHovered <> nil then
        begin
          TD2HackVisualObject(FHovered).MouseInObject:=false;
          TD2HackVisualObject(FHovered).MouseLeave;
          TD2VisualObject(FHovered).RemoveFreeNotify(Self);
        end;
        FHovered:=Obj;
        TD2HackVisualObject(FHovered).MouseInObject:=true;
        TD2HackVisualObject(FHovered).MouseEnter;
        TD2VisualObject(FHovered).AddFreeNotify(Self);
      end;

      P:=Obj.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2HackVisualObject(Obj).MouseMove(Shift,P.X,P.Y,0,0);
    end
    else
      if FHovered <> nil then
      begin
        TD2HackVisualObject(FHovered).MouseInObject:=false;
        TD2HackVisualObject(FHovered).MouseLeave;
        TD2VisualObject(FHovered).RemoveFreeNotify(Self);
        FHovered:=nil;
      end;
  end;
  FDownPos:=FMousePos;
  Cursor:=NewCursor;
end;

procedure TD3CustomObjectLayer.LayerMouseUp(Button:TMouseButton; Shift:TShiftState; X,Y:single);
var
  P:TD2Point;
  Obj:TD2VisualObject;
begin
  inherited;
  { design }
  FDesignPlaceObject:=nil;
  SetLength(FDesignGridLines,0);
  if (FSelected <> nil) and FMoving then
  begin
    if (FSelected.Parent <> nil) and (TD2Object(FSelected.Parent).IsVisual) then
      TD2VisualObject(FSelected.Parent).Realign;
    if (GvarD2Designer <> nil) then
      GvarD2Designer.Modified(Owner);
    if FSelected.Parent=nil then
      RealignRoot;
  end;
  if (FSelected <> nil) and (FLeftTop or FLeftBottom or FLeftBottom or FRightBottom) then
  begin
    if (FSelected.Parent <> nil) and (TD2Object(FSelected.Parent).IsVisual) then
      TD2VisualObject(FSelected.Parent).Realign;
    if (GvarD2Designer <> nil) then
      GvarD2Designer.Modified(Owner);
    if FSelected.Parent=nil then
      RealignRoot;
  end;
  FMoving:=false;
  FLeftTop:=false;
  FLeftBottom:=false;
  FRightTop:=false;
  FRightBottom:=false;
  FTop:=false;
  FBottom:=false;
  FLeft:=false;
  FRight:=false;
  FRotate:=false;
  { drag }
  if FDragging then
  begin
    FDragging:=false;
//    ReleaseCapture;
  end;
  if FResizing then
  begin
    FResizing:=false;
//    ReleaseCapture;
  end;
  { event }
  if not FDesignTime then
  begin
    if (FCaptured <> nil) then
    begin
      P:=FCaptured.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2HackVisualObject(FCaptured).MouseUp(Button,Shift,P.X,P.Y);
      Exit;
    end;
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) then
    begin
      P:=Obj.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2HackVisualObject(Obj).MouseUp(Button,Shift,P.X,P.Y);
    end;
  end;
end;

procedure TD3CustomObjectLayer.MouseWheel(Shift:TShiftState; WheelDelta:integer; var Handled:boolean);
var
  Obj:TD2VisualObject;
begin
  inherited ;
  { event }
  if not FDesignTime then
  begin
    {$IFDEF DARWIN}
    WheelDelta:=WheelDelta * 40;
    {$ENDIF}
    if (FCaptured <> nil) then
    begin
      TD2HackVisualObject(FCaptured).MouseWheel(Shift,WheelDelta,Handled);
      Exit;
    end;
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    while (Obj <> nil) do
    begin
      TD2HackVisualObject(Obj).MouseWheel(Shift,WheelDelta,Handled);
      if Handled then Break;
      if (Obj.Parent <> nil) and (Obj.Parent.IsVisual) then
        Obj:=TD2VisualObject(Obj.Parent)
      else
        Obj:=nil;
    end;
  end;
end;

procedure TD3CustomObjectLayer.RealignRoot;
begin
  if (FChildren <> nil) and (FChildren.Count > 0) and (TD2Object(FChildren[0]).isVisual) then
    with TD2VisualObject(FChildren[0]) do
    begin
      if (RotateAngle=90) or (RotateAngle=-90) or (RotateAngle=-270) or (RotateAngle=270) then
        SetBounds(round((FLayerWidth - FLayerHeight) / 2),-round((FLayerWidth - FLayerHeight) / 2),
          round(FLayerHeight / Scale.Y),round(FLayerWidth / Scale.X))
      else
        SetBounds(0,0,Round(FLayerWidth / Scale.X),Round(FLayerHeight / Scale.Y));
    end;
end;

procedure TD3CustomObjectLayer.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FCaptured) then
    FCaptured:=nil;
  if (Operation=opRemove) and (AComponent=FSelected) then
    FSelected:=nil;
  if (Operation=opRemove) and (AComponent=FHovered) then
    FHovered:=nil;
  if (Operation=opRemove) and (AComponent=FFocused) then
    FFocused:=nil;
  if (Operation=opRemove) and (AComponent=FDesignPlaceObject) then
    FDesignPlaceObject:=nil;
  if (Operation=opRemove) and (AComponent=FStyle) then
    Style:=nil;
end;

procedure TD3CustomObjectLayer.SetFill(const Value:TD2Brush);
begin
  FFill.Assign(Value);
end;

procedure TD3CustomObjectLayer.FillChanged(Sender:TObject);
begin
  SetLength(FUpdateRects,0);
  AddUpdateRect(d2Rect(0,0,FLayerWidth,FLayerHeight));
end;

procedure TD3CustomObjectLayer.SetSnapGridShow(const Value:boolean);
begin
  if FSnapGridShow <> Value then
  begin
    FSnapGridShow:=Value;
    SetLength(FUpdateRects,0);
    AddUpdateRect(d2Rect(0,0,FLayerWidth,FLayerHeight));
  end;
end;

procedure TD3CustomObjectLayer.SetSnapGridSize(const Value:single);
begin
  if FSnapGridSize <> Value then
  begin
    FSnapGridSize:=Value;
    if FSnapGridSize < 0.01 then
      FSnapGridSize:=0.01;
    if FsnapGridShow then
      Repaint;
  end;
end;

procedure TD3CustomObjectLayer.InsertObject(const ClassName:string);  // 7777
var
  P:TPoint;
  Obj:TD2Object;
  OldSel:TD2VisualObject;
  InsertPos:TD2Point;
begin
  if GetClass(ClassName) <> nil then
  begin
    if GetClass(ClassName).InheritsFrom(TD2Object) then
    begin
      if FSelected <> nil then
      begin
        try
          Obj:=TD2ObjectClass(GetClass(ClassName)).Create(Owner);
          OldSel:=FSelected;
          FSelected.AddObject(Obj);

          if GvarD2Designer <> nil then
           begin
            Obj.Name:=orca_scene2d.GvarD2Designer.UniqueName(Owner,Obj.ClassName);
            GvarD3Designer.AddObject2D(Obj);
           end;

          if GvarD2Designer <> nil then
          begin
            GvarD2Designer.SelectObject(Owner,Obj,[]);
            if Assigned(FDesignChangeSelection) then   FDesignChangeSelection(Self);
            GvarD2Designer.Modified(Owner);
          end;

          if Obj.IsVisual then
          begin
            if GetPropInfo(Obj.ClassInfo,'Text',[tkString,tkLString,tkWString]) <> nil then
              SetStrProp(Obj,'Text',Copy(Obj.ClassName,4,Length(Obj.ClassName)));
            if (Owner is TWinControl) and (FPopupPos.X > 0) then
            begin
              if Pos('Item',Obj.ClassName)=0 then
              begin
                P:=Scene.ScreenToClient(Point(trunc(FPopupPos.X),trunc(FPopupPos.Y)));
                InsertPos:=OldSel.AbsoluteToLocal(d2Point(P.X,P.Y));

                if d2PtInRect(d2Point(P.X,P.Y),d2Rect(0,0,Width,Height)) then
                begin
                  TD2VisualObject(Obj).Position.X:=InsertPos.X;
                  TD2VisualObject(Obj).Position.Y:=InsertPos.Y;
                end;

              end;
            end;
            FPopupPos:=d2Point(-1,-1);
            FSelected:=TD2VisualObject(Obj);
            TD2HackVisualObject(FSelected).DesignSelect;
            TD2HackVisualObject(FSelected).DesignInsert;
          end;
        finally
        end;
      end
      else
        if (FChildren=nil) or (FChildren.Count=0) then
        begin
          { insert root object }
          try
            Obj:=TD2ObjectClass(GetClass(ClassName)).Create(Owner);
            if GvarD2Designer <> nil then
              Obj.Name:=GvarD2Designer.UniqueName(Owner,Obj.ClassName);
            AddObject(Obj);
            if GvarD2Designer <> nil then
            begin
              GvarD2Designer.SelectObject(Owner,Obj,[]);
              if Assigned(FDesignChangeSelection) then
                FDesignChangeSelection(Self);
              GvarD2Designer.Modified(Owner);
            end;
            if Obj.IsVisual then
            begin
              InsertPos:=FPopupPos;
              FPopupPos:=d2Point(-1,-1);
              FSelected:=TD2VisualObject(Obj);
              TD2HackVisualObject(FSelected).DesignSelect;
            end;
          except
          end;
        end;
    end
    else
    begin
      FInsertObject:=ClassName;
    end;
  end
  else
    FInsertObject:='';
end;

procedure TD3CustomObjectLayer.doDesignPopupDesignHide(Sender:TObject);
begin
  if (FSelected <> nil) and (FSelected <> Root) then
  begin
    FSelected.DesignHide:=not FSelected.DesignHide;
    if FSelected.DesignHide and (FSelected.Parent.IsVisual) then
    begin
      FSelected:=TD2VisualObject(FSelected.Parent);
    end;
    AddUpdateRect(d2Rect(0,0,FLayerWidth,FLayerHeight));
  end;
end;

procedure TD3CustomObjectLayer.doDesignPopupDel(Sender:TObject);
var
  Obj:TD2VisualObject;
begin
  if (FSelected <> nil) and (FSelected <> Root) then
  begin
    Obj:=FSelected;
    if (Obj.Parent <> nil) and (Obj.Parent.IsVisual) then
    begin
      FSelected:=TD2VisualObject(Obj.Parent);
      TD2HackvisualObject(FSelected).DesignSelect;
    end
    else
    begin
      FSelected:=TD2VisualObject(Root);
      if FSelected <> nil then
        TD2HackvisualObject(FSelected).DesignSelect;
    end;
    Obj.Free;
  end;
end;

procedure TD3CustomObjectLayer.doDesignPopupAddItem(Sender:TObject);
begin
  if FSelected <> nil then
  begin
    InsertObject(TMenuItem(Sender).Hint);
  end;
end;

procedure TD3CustomObjectLayer.doDesignPopupAdd(Sender:TObject);
var
  S:string;
begin
  S:=StripHotkey(TMenuItem(Sender).Caption);
  if (S <> '') then
    S:='TD2' + S;
  InsertObject(S);
end;

procedure TD3CustomObjectLayer.doDesignPopupReorder(Sender:TObject);
begin
  if FSelected=nil then Exit;

  if StripHotkey(TMenuItem(Sender).Caption)='Bring to front' then
    FSelected.BringToFront;
  if StripHotkey(TMenuItem(Sender).Caption)='Send to back' then
    FSelected.SendToBack;
end;

procedure TD3CustomObjectLayer.doDesignPopupGrid(Sender:TObject);
begin
  case TMenuItem(Sender).Tag of
    1:FSnapGridShow:=TMenuItem(Sender).Checked;
    2:
      begin
        FSnapToGrid:=TMenuItem(Sender).Checked;
        FSnapToLines:=not FSnapToGrid;
      end;
    3:
      begin
        FSnapToLines:=TMenuItem(Sender).Checked;
        FSnapToGrid:=not FSnapToLines;
      end;
    4:
      begin
        FSnapToLines:=false;
        FSnapToGrid:=false;
      end;
  end;
  SetLength(FUpdateRects,0);
  AddUpdateRect(d2Rect(0,0,FLayerWidth,FLayerHeight));
end;

var
  Clipboard:AnsiString;

procedure TD3CustomObjectLayer.doDesignPopupCopy(Sender:TObject);
var
  S:TStringStream;
begin
  if FSelected <> nil then
  begin
    S:=TStringStream.Create('');
    FSelected.SaveToStream(S);
    Clipboard:=S.DataString;
    S.Free;
  end;
end;

procedure TD3CustomObjectLayer.doDesignPopupPaste(Sender:TObject);
var
  S:TStringStream;
  Result:TD2Object;
begin
  if Clipboard <> '' then
  begin
    S:=TStringStream.Create(Clipboard);
    Result:=CreateObjectFromStream(Owner,S);
    if FSelected <> nil then
      Result.Parent:=FSelected
    else
      Result.Parent:=Root;
    if GvarD2Designer <> nil then
      Result.Name:=GvarD2Designer.UniqueName(Owner,Result.ClassName);
    S.Free;
  end;
end;

procedure TD3CustomObjectLayer.doDesignPopupLoadFromFile(Sender:TObject);
var
  S:TStream;
  Result:TD2Object;
  Open:TOpenDialog;
begin
  Open:=TOpenDialog.Create(nil);
  Open.Filter:='d2Scene Files|*.d2scene';
  if Open.Execute then
  begin
    S:=TFileStream.Create(Open.FileName,fmOpenRead);
{    Result:=CreateObjectFromStream(Owner,S);
    if FSelected <> nil then
      Result.Parent:=FSelected
    else
      Result.Parent:=Root;
    if GvarD2Designer <> nil then
      Result.Name:=GvarD2Designer.UniqueName(Owner,Result.ClassName);}
    S.Free;
  end;
  Open.Free;
end;

procedure TD3CustomObjectLayer.doDesignPopupCreateStyle(Sender:TObject);
var
  R:TD2Resources;
  CurrentRes,Link:TD2VisualObject;
  LinkedList:TList;
  i:integer;
begin
  { Edit Style }
  if FSelected=nil then Exit;
  if not Assigned(FSelected.Scene) then Exit;

  if (FSelected.Scene.GetStyle=nil) then
  begin
    // Create style
    R:=TD2Resources.Create(FSelected.Scene.GetOwner);
    R.Root:=TD2Background.Create(nil);
    if GvarD2Designer <> nil then
      R.Name:=GvarD2Designer.UniqueName(Owner,R.ClassName);
    FSelected.Scene.SetStyle(R);
  end
  else
    R:=FSelected.Scene.GetStyle;

  if (TD2Control(FSelected).Resource='') and (GvarD2Designer <> nil) then
  begin
    TD2Control(FSelected).Resource:=GvarD2Designer.UniqueName(Owner,FSelected.Name + 'Style');
  end;
  if (R.Root.FindResource(TD2Control(FSelected).Resource)=nil) then
  begin
    // Clone Resource from Current
    CurrentRes:=TD2HackControl(FSelected).GetResourceObject;
    CurrentRes.Align:=vaNone;
    CurrentRes.Parent:=R.Root;
    if GvarD2Designer <> nil then
    begin
      CurrentRes.ResourceName:=GvarD2Designer.UniqueName(Owner,FSelected.Name + 'Style');
      TD2Control(FSelected).Resource:=CurrentRes.ResourceName;
    end
    else
    begin
      CurrentRes.ResourceName:=FSelected.Name;
      TD2Control(FSelected).Resource:=CurrentRes.ResourceName;
    end;

    // Add linked object to resource - like ListBox with ScrollBar
    LinkedList:=TList.Create;
    TD2Control(CurrentRes).AddControlsToList(LinkedList);
    for i:=0 to LinkedList.Count - 1 do
    begin
      if R.Root.FindResource(TD2Control(LinkedList[i]).Resource)=nil then
      begin
        Link:=TD2HackControl(LinkedList[i]).GetResourceObject;
        Link.Align:=vaNone;
        Link.Parent:=R.Root;
        if GvarD2Designer <> nil then
        begin
          Link.ResourceName:=GvarD2Designer.UniqueName(Owner,FSelected.Name + TD2Control(LinkedList[i]).ResourceName);
          TD2Control(LinkedList[i]).Resource:=Link.ResourceName;
        end;
      end;
    end;
    LinkedList.Free;
  end;

  if GvarD2Designer <> nil then
  begin
    // Edit Style
    GvarD2Designer.EditStyle(R,TD2Control(FSelected).Resource);
  end;
end;

procedure TD3CustomObjectLayer.doDesignPopupEditStyle(Sender:TObject);
var
  R:TD2Resources;
  CurrentRes,Link:TD2VisualObject;
  LinkedList:TList;
  i:integer;
  StyleName:string;
begin
  { Edit Style }
  if FSelected=nil then Exit;
  if not Assigned(FSelected.Scene) then Exit;

  if (FSelected.Scene.GetStyle=nil) then
  begin
    // Create style
    R:=TD2Resources.Create(FSelected.Scene.GetOwner);
    R.Root:=TD2Background.Create(nil);
    if GvarD2Designer <> nil then
      R.Name:=GvarD2Designer.UniqueName(Owner,R.ClassName);
    FSelected.Scene.SetStyle(R);
  end
  else
    R:=FSelected.Scene.GetStyle;

  // Clone Resource from Current
  StyleName:=TD2Control(FSelected).Resource;
  if StyleName='' then
  begin
    StyleName:=FSelected.ClassName + 'Style';
    Delete(StyleName,1,3);
  end;
  if (R.Root.FindResource(StyleName)=nil) then
  begin
    CurrentRes:=TD2HackControl(FSelected).GetResourceObject;
    CurrentRes.Align:=vaNone;
    CurrentRes.Parent:=R.Root;

    // Add linked object to resource - like ListBox with ScrollBar
    LinkedList:=TList.Create;
    TD2Control(CurrentRes).AddControlsToList(LinkedList);
    for i:=0 to LinkedList.Count - 1 do
    begin
      StyleName:=TD2Control(LinkedList[i]).Resource;
      if StyleName='' then
      begin
        StyleName:=TD2Control(LinkedList[i]).ClassName + 'Style';
        Delete(StyleName,1,3);
      end;
      if R.Root.FindResource(StyleName)=nil then
      begin
        Link:=TD2HackControl(LinkedList[i]).GetResourceObject;
        Link.Align:=vaNone;
        Link.ResourceName:=StyleName;
        Link.Parent:=R.Root;
      end;
    end;
    LinkedList.Free;
  end;

  if GvarD2Designer <> nil then
  begin
    // Edit Style
    StyleName:=TD2Control(FSelected).Resource;
    if StyleName='' then
    begin
      StyleName:=FSelected.ClassName + 'Style';
      Delete(StyleName,1,3);
    end;
    GvarD2Designer.EditStyle(R,StyleName);
  end;
end;

procedure TD3CustomObjectLayer.doDesignTabOrderBtnClick(Sender:TObject);
var
  Obj:TD2VisualObject;
begin
  if TD2Button(Sender).TextW='Up' then
  begin
    Obj:=TD2VisualObject(TD2Listbox(TD2Button(Sender).TagObject).Selected.TagObject);
    Obj.TabOrder:=Obj.TabOrder - 1;
  end;
  if TD2Button(Sender).TextW='Down' then
  begin
    Obj:=TD2VisualObject(TD2Listbox(TD2Button(Sender).TagObject).Selected.TagObject);
    Obj.TabOrder:=Obj.TabOrder + 1;
  end;
  doDesignTabOrderRebuildList(TD2Listbox(TD2Button(Sender).TagObject));
end;

procedure TD3CustomObjectLayer.doDesignTabOrderRebuildList(ListBox:TD2VisualObject);
var
  Item:TD2ListboxItem;
  List:TD2ListBox;
  Id3,i:integer;
  L:TList;
begin
  if FSelected=nil then Exit;
  if FSelected.Parent=nil then Exit;
  if not FSelected.Parent.IsVisual then Exit;
  List:=TD2ListBox(Listbox);
  if List.Selected <> nil then
    Id3:=TD2VisualObject(List.Selected.TagObject).TabOrder
  else
    Id3:=0;
  { add taborder list }
  List.BeginUpdate;
  List.Clear;
  L:=TList.Create;
  TD2VisualObject(FSelected.Parent).GetTabOrderList(L,false);
  for i:=0 to L.Count - 1 do
  begin
    Item:=TD2ListboxItem.Create(List.Owner);
    Item.Parent:=List;
    Item.TextW:=TD2VisualObject(L[i]).Name;
    if Item.TextW='' then
      Item.TextW:=TD2VisualObject(L[i]).ClassName;
    Item.TagObject:=TD2VisualObject(L[i]);
  end;
  L.Free;
  List.EndUpdate;
  List.ItemIndex:=Id3;
end;

procedure TD3CustomObjectLayer.doDesignPopupTabOrder(Sender:TObject);
var
  Form:TForm;
  Scene:TD2Scene;
  Wnd:TD2HudWindow;
  List:TD2Listbox;
  BtnUp,BtnDown:TD2Button;
begin
  if FSelected=nil then Exit;
  if FSelected.Parent=nil then Exit;
  if not FSelected.Parent.IsVisual then Exit;

  Form:=TForm.CreateNew(Application,0);
  Form.BorderStyle:=bsNone;
  Form.Position:=poScreenCenter;
  Form.SetBounds(0,0,300,400);
  Scene:=TD2Scene.Create(Form);
  Scene.Parent:=Form;
  Scene.Align:=alClient;
  Scene.Transparency:=true;
  Wnd:=TD2HudWindow.Create(Form);
  Wnd.ShowSizeGrip:=false;
  Wnd.TextW:='TabOrder Editor';
  Scene.AddObject(Wnd);

  BtnUp:=TD2HudButton.Create(Form);
  BtnUp.Parent:=Wnd;
  BtnUp.SetBounds(Form.Width - 70,50,50,26);
  BtnUp.TextW:='Up';
  BtnUp.OnClick:=doDesignTabOrderBtnClick;
  BtnDown:=TD2HudButton.Create(Form);
  BtnDown.Parent:=Wnd;
  BtnDown.SetBounds(Form.Width - 70,90,50,26);
  BtnDown.TextW:='Down';
  BtnDown.OnClick:=doDesignTabOrderBtnClick;

  List:=TD2HudListbox.Create(Form);
  List.Parent:=Wnd;
  List.Align:=vaClient;
  List.Padding.Rect:=d2Rect(26,45,86,33);
  List.HideSelectionUnfocused:=false;
  List.ItemHeight:=20;

  Form.SetBounds(0,0,300,400);
  BtnUp.TagObject:=List;
  BtnDown.TagObject:=List;
  doDesignTabOrderRebuildList(List);
  List.ItemIndex:=FSelected.TabOrder;
  Form.ShowModal;
  Form.Free;
end;


procedure TD3CustomObjectLayer.OpenDesignPopup;
var
  i,j:integer;
  S,T:AnsiString;
  OItem,SItem,Item:TMenuItem;
  CatList:TStringList;
  P:TPoint;
begin
  if not DesignPopupEnabled then Exit;
  if FDesignPopup <> nil then
  begin
    FDesignPopup.Free;
    FDesignPopup:=nil;
  end;
  if FDesignPopup=nil then
  begin
    FDesignPopup:=TPopupMenu.Create(nil);

    if orca_scene2d.GVarD2ObjectList <> nil then
    begin
      // item
      if (FSelected <> nil) and (FSelected.ItemClass <> '') then
      begin
        S:=FSelected.ItemClass;
        T:=d2GetToken(S,';');
        while T <> '' do
        begin
          if Pos('TD2',T)=1 then Delete(T,1,3);
          Item:=NewItem('Add ' + T,0,false,true,doDesignPopupAddItem,0,'');
          Item.Hint:='TD2' + T;
          FDesignPopup.Items.Add(Item);
          T:=d2GetToken(S,';');
        end;
      end;
      // add
      Item:=NewItem('Add 2D Object',0,false,true,nil,0,'');
      // add categories
      for i:=0 to orca_scene2d.GVarD2ObjectList.Count - 1 do
      begin
        if Item.Find(orca_scene2d.GVarD2ObjectList[i])=nil then
        begin
          SItem:=NewItem(orca_scene2d.GVarD2ObjectList[i],0,false,true,nil,0,'');
          Item.Add(SItem);
        end;
      end;

      //................................................
      //add controls to list and sort
      CatList:=TStringList.Create;
      CatList.Sorted:=true;

      for i:=0 to orca_scene2d.GVarD2ObjectList.Count - 1 do
      begin
        S:=TD2ObjectClass(orca_scene2d.GVarD2ObjectList.Objects[i]).ClassName;
        if Pos('TD2',S)=1 then  Delete(S,1,3);
        CatList.Add(S);
      end;

      // add objects
      for i:=0 to CatList.Count - 1 do
      begin
        OItem:=NewItem(CatList[i],0,false,true,doDesignPopupAdd,0,'');
        OItem.Tag:=$FF;
        for j:=0 to orca_scene2d.GVarD2ObjectList.Count - 1 do
        begin
          S:=TD2ObjectClass(orca_scene2d.GVarD2ObjectList.Objects[j]).ClassName;
          if Pos('TD2',S)=1 then Delete(S,1,3);

          if sametext(S,CatList[i]) then
          begin
            SItem:=Item.Find(orca_scene2d.GVarD2ObjectList[j]);
            Break;
          end;

        end;
        if SItem <> nil then  SItem.Add(OItem);
      end;

      CatList.Free;

      //................................................

      FDesignPopup.Items.Add(Item);
      { Delete }
      Item:=NewItem('Delete object',0,false,true,doDesignPopupDel,0,'');
      FDesignPopup.Items.Add(Item);
      { Delete }
      Item:=NewItem('Load From File...',0,false,true,doDesignPopupLoadFromFile,0,'');
      FDesignPopup.Items.Add(Item);
      { Design Hide }
      if FSelected <> nil then
      begin
        Item:=NewItem('Hide in Design-time',0,FSelected.DesignHide,true,doDesignPopupDesignHide,0,'');
        Item.AutoCheck:=true;
        FDesignPopup.Items.Add(Item);

        { Edit Style }
        if (FSelected is TD2Control) then
        begin
          Item:=NewItem('Edit Default Style...',0,false,true,doDesignPopupEditStyle,0,'');
          FDesignPopup.Items.Add(Item);
          Item:=NewItem('Edit Custom Style...',0,false,true,doDesignPopupCreateStyle,0,'');
          FDesignPopup.Items.Add(Item);
        end;
        { TabOrder }
        if (FSelected.IsVisual) and (FSelected.CanFocused) then
        begin
          Item:=NewItem('Tab Order...',0,false,true,doDesignPopupTabOrder,0,'');
          FDesignPopup.Items.Add(Item);
        end;
      end;
      { Reorder }
      Item:=NewItem('Order',0,false,true,nil,0,'');
      SItem:=NewItem('Bring to front',0,false,true,doDesignPopupReorder,0,'');
      Item.Add(SItem);
      SItem:=NewItem('Send to back',0,false,true,doDesignPopupReorder,0,'');
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
      { Edit }
      Item:=NewItem('Edit',0,false,true,nil,0,'');
      SItem:=NewItem('Copy to clipboard',0,false,true,doDesignPopupCopy,0,'');
      Item.Add(SItem);
      SItem:=NewItem('Paste from clipboard',0,false,true,doDesignPopupPaste,0,'');
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
      { Grid }
      Item:=NewItem('Grid',0,false,true,nil,0,'');
      SItem:=NewItem('Show snap grid',0,FSnapGridShow,true,doDesignPopupGrid,0,'');
      SItem.Tag:=1;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      SItem:=NewItem('Snap to grid',0,FSnapToGrid,true,doDesignPopupGrid,0,'');
      SItem.Tag:=2;
      SItem.RadioItem:=true;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      SItem:=NewItem('Snap to lines',0,FSnapToLines,true,doDesignPopupGrid,0,'');
      SItem.Tag:=3;
      SItem.RadioItem:=true;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      SItem:=NewItem('Snap disabled',0,not FSnapToLines or not FSnapToLines,true,doDesignPopupGrid,0,'');
      SItem.Tag:=4;
      SItem.RadioItem:=true;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
    end;
  end;
  GetCursorPos(P);
  FDesignPopup.Popup(P.X,P.Y);
end;

procedure TD3CustomObjectLayer.DefineProperties(Filer:TFiler);
begin
  inherited DefineProperties(Filer);
  Filer.DefineProperty('DesignSnapGridShow',ReadDesignSnapGridShow,WriteDesignSnapGridShow,true);
  Filer.DefineProperty('DesignSnapToGrid',ReadDesignSnapToGrid,WriteDesignSnapToGrid,true);
  Filer.DefineProperty('DesignSnapToLines',ReadDesignSnapToLines,WriteDesignSnapToLines,true);
end;

procedure TD3CustomObjectLayer.ReadDesignSnapGridShow(Reader:TReader);
begin
  FSnapGridShow:=Reader.ReadBoolean;
  if not (FDesignTime) then
    FSnapGridShow:=false;
end;

procedure TD3CustomObjectLayer.ReadDesignSnapToGrid(Reader:TReader);
begin
  FSnapToGrid:=Reader.ReadBoolean;
end;

procedure TD3CustomObjectLayer.ReadDesignSnapToLines(Reader:TReader);
begin
  FSnapToLines:=Reader.ReadBoolean;
end;

procedure TD3CustomObjectLayer.WriteDesignSnapGridShow(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapGridShow);
end;

procedure TD3CustomObjectLayer.WriteDesignSnapToGrid(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapToGrid);
end;

procedure TD3CustomObjectLayer.WriteDesignSnapToLines(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapToLines);
end;

function TD3CustomObjectLayer.GetRoot:TD2Object;
begin
  if (FChildren <> nil) and (FChildren.Count > 0) then
    Result:=TD2Object(FChildren[0])
  else
    Result:=nil;
end;

procedure TD3CustomObjectLayer.SetFocused(const Value:TD2VisualObject);
begin
  if FFocused <> Value then
  begin
    if FFocused <> nil then
    begin
      TD2HackvisualObject(FFocused).KillFocus;
      TD2VisualObject(FFocused).RemoveFreeNotify(Self);
    end;
    FFocused:=Value;
    if FFocused <> nil then
    begin
      TD2HackvisualObject(FFocused).EnterFocus;
      TD2VisualObject(FFocused).AddFreeNotify(Self);
      SetFocus;
    end;
  end;
end;

procedure TD3CustomObjectLayer.DoDesignSelect(AObject:TObject);
begin
  if (AObject <> nil) and (AObject is TD2VisualObject) and (TD2VisualObject(AObject).Scene <> nil) and (TD2VisualObject(AObject).Scene.GetComponent=Self) then
  begin
    FSelected:=TD2VisualObject(AObject);
    TD2HackvisualObject(FSelected).DesignSelect;
    FSelected.Repaint;
  end;
end;

procedure TD3CustomObjectLayer.ProcessTick;
begin
end;

procedure TD3CustomObjectLayer.SetSelected(const Value:TD2VisualObject);
begin
  if FSelected <> Value then
  begin
    if FSelected <> nil then
      FSelected.Repaint;
    FSelected:=Value;
    if FSelected <> nil then
      FSelected.Repaint;
    if Assigned(FDesignChangeSelection) then
      FDesignChangeSelection(Self);
    AddUpdateRect(d2Rect(0,0,1000,1000));
    Draw;
  end;
end;

function TD3CustomObjectLayer.GetActiveControl:TD2Control;
begin
  Result:=FActiveControl;
end;

function TD3CustomObjectLayer.GetDisableUpdate:boolean;
begin
  Result:=FDisableUpdate;
end;

function TD3CustomObjectLayer.GetDesignTime:boolean;
begin
  Result:=FDesignTime;
end;

function TD3CustomObjectLayer.GetCanvas:TD2Canvas;
begin
  Result:=FCanvas;
end;

function TD3CustomObjectLayer.GetOwner:TComponent;
begin
  Result:=Owner;
end;

function TD3CustomObjectLayer.GetRealTime:boolean;
begin
  Result:=Scene.RealTime;
end;

function TD3CustomObjectLayer.GetComponent:TComponent;
begin
  Result:=Self;
end;

function TD3CustomObjectLayer.GetSelected:TD2VisualObject;
begin
  Result:=FSelected;
end;

function TD3CustomObjectLayer.GetDeltaTime:single;
begin
  Result:=Scene.DeltaTime;
end;

procedure TD3CustomObjectLayer.SetDisableUpdate(Value:boolean);
begin
  FDisableUpdate:=Value;
end;

function TD3CustomObjectLayer.GetUpdateRectsCount:integer;
begin
  Result:=Length(FUpdateRects);
end;

function TD3CustomObjectLayer.GetUpdateRect(const Index:integer):TD2Rect;
begin
  Result:=FUpdateRects[Index];
end;

function TD3CustomObjectLayer.GetCaptured:TD2VisualObject;
begin
  Result:=FCaptured;
end;

procedure TD3CustomObjectLayer.SetCaptured(const Value:TD2VisualObject);
begin
  FCaptured:=Value;
  if Value <> nil then
    Capture
  else
    ReleaseCapture;
end;

function TD3CustomObjectLayer.GetFocused:TD2VisualObject;
begin
  Result:=FFocused;
end;

procedure TD3CustomObjectLayer.SetDesignRoot(const Value:TD2VisualObject);
begin
  FDesignRoot:=Value;
end;

function TD3CustomObjectLayer.GetMousePos:TD2Point;
begin
  Result:=FMousePos;
end;

procedure TD3CustomObjectLayer.BeginDrag;
begin
  if (LayerAlign=d3LayerContents) and (Scene.AllowDrag) then
  begin
    Scene.BeginDrag;
  end;
  FDragging:=true;
  FDownPos:=FMousePos;
  Capture;
end;

procedure TD3CustomObjectLayer.BeginResize;
begin
  if (LayerAlign in [d3LayerContents,d3LayerBottom,d3LayerRight,d3LayerMostBottom,d3LayerMostRight]) then
  begin
    Scene.BeginResize;
  end;
  FResizing:=true;
  FDownPos:=FMousePos;
  FResizePos:=d2Point(Position.X,Position.Y);
  FResizeStartPos:=d2Point(Round(Position.X - Width / 2),Round(Position.Y - Height / 2));
  if Projection=d3ProjectionScreen then
    FResizeSize:=d2Point(Width,Height)
  else
    FResizeSize:=d2Point(Width,Depth);
  FDownSize:=FResizeSize;
  Capture;
end;

function TD3CustomObjectLayer.GetStyle:TD2Resources;
begin
  Result:=FStyle;
end;

function TD3CustomObjectLayer.LocalToScreen(const Point:TD2Point):TD2Point;
begin
  with LocalToAbsolute(d3Point(Point.x - (Width / 2),Point.y - (Height / 2),0)) do
    with Scene.ClientToScreen(Classes.Point(round(x),round(y))) do
      Result:=d2Point(X,Y);
end;

function TD3CustomObjectLayer.GetTransparency:boolean;
begin
  Result:=FTransparency;
end;

function TD3CustomObjectLayer.GetDesignPlaceObject:TD2VisualObject;
begin
  Result:=FDesignPlaceObject;
end;

procedure TD3CustomObjectLayer.UpdateResource;
begin
  if Root <> nil then
    Root.UpdateResource;
end;

procedure TD3CustomObjectLayer.SetStyle(const Value:TD2Resources);
begin
  if FStyle <> Value then
  begin
    if FStyle <> nil then
      FStyle.RemoveSceneUpdater(Self);
    FStyle:=Value;
    if FStyle <> nil then
      FStyle.AddSceneUpdater(Self);

    UpdateResource;
  end;
end;

procedure TD3CustomObjectLayer.BeginVCLDrag(Source:TObject; ABitmap:TD2Bitmap);
begin
  if Scene <> nil then
    TD3HackScene(Scene).BeginVCLDrag(Source);
end;

procedure TD3CustomObjectLayer.DragEnter(const Data:TD3DragObject;
  const Point:TD3Point);
begin
  inherited;
end;

procedure TD3CustomObjectLayer.DragLeave;
begin
  inherited;
end;

var
  FTarget:TD2VisualObject=nil;

//procedure TD3CustomObjectLayer.DragOver(const Data:TD3DragObject;	//Deleted by GoldenFox
  procedure TD3CustomObjectLayer.DragOver(const Data:TD3DragObject; Shift: TShiftState;	//Added by GoldenFox
  const Point:TD3Point; var Accept:boolean);
var
  P:TD2Point;
  i:integer;
  NewTarget:TD2VisualObject;
  d2Data:TD2DragObject;
  P3,rayPos,rayDir:TD3Vector;
begin
  inherited;
  if Accept then Exit; // Self accept

  with LocalToAbsolute(Point) do
    inherited Canvas.Pick(X,Y,Projection,rayPos,rayDir);
  rayPos:=AbsoluteToLocalVector(rayPos);
  rayDir:=AbsoluteToLocalVector(rayDir);

  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      P.X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      P.Y:=(((-P3.z + (Depth / 2)) / Depth) * FLayerHeight);
    end
    else
      Exit;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      P.X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      P.Y:=(((P3.y + (Height / 2)) / Height) * FLayerHeight);
    end
    else
      Exit;
  end;

  Fillchar(d2Data,SizeOf(d2Data),0);
  SetLength(d2Data.Files,Length(Data.Files));
  for i:=0 to Length(Data.Files) - 1 do
    d2Data.Files[i]:=Data.Files[i];
  d2Data.Data:=Data.Data;
  d2Data.Source:=Data.Source;
  //NewTarget:=Root.Visual.FindTarget(P,d2Data);	//Deleted by GoldenFox
  NewTarget:=Root.Visual.FindTarget(Shift,P,d2Data);	//Added by GoldenFox
  if (NewTarget <> FTarget) then
  begin
    if FTarget <> nil then
      TD2HackVisualObject(FTarget).DragLeave;
    FTarget:=NewTarget;
    if FTarget <> nil then
    begin
      TD2HackVisualObject(FTarget).DragEnter(d2Data,P);
    end;
  end;
  Accept:=NewTarget <> nil;
end;

procedure TD3CustomObjectLayer.DragDrop(const Data: TD3DragObject;
//  const Point:TD3Point);	//Deleted by GoldenFox
  Shift: TShiftState; const Point: TD3Point);	//Added by GoldenFox
var
  P:TD2Point;
  i:integer;
  NewTarget:TD2VisualObject;
  d2Data:TD2DragObject;
  P3,rayPos,rayDir:TD3Vector;
begin
  inherited;

  with LocaltoAbsolute(Point) do
    inherited Canvas.Pick(X,Y,Projection,rayPos,rayDir);
  rayPos:=AbsoluteToLocalVector(rayPos);
  rayDir:=AbsoluteToLocalVector(rayDir);

  if Projection=d3ProjectionCamera then
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      P.X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      P.Y:=(((-P3.z + (Depth / 2)) / Depth) * FLayerHeight);
    end
    else
      Exit;
  end
  else
  begin
    if RayCastIntersect(rayPos,rayDir,P3) then
    begin
      P3:=AbsoluteToLocalVector(P3);
      P.X:=(((P3.x + (Width / 2)) / Width) * FLayerWidth);
      P.Y:=(((P3.y + (Height / 2)) / Height) * FLayerHeight);
    end
    else
      Exit;
  end;

  Fillchar(d2Data,SizeOf(d2Data),0);
  SetLength(d2Data.Files,Length(Data.Files));
  for i:=0 to Length(Data.Files) - 1 do
    d2Data.Files[i]:=Data.Files[i];
  d2Data.Data:=Data.Data;
  d2Data.Source:=Data.Source;
  if (FTarget <> nil) then
  begin
    TD2HackVisualObject(FTarget).DragLeave;
    //TD2HackVisualObject(FTarget).DragDrop(d2Data,P);	//Deleted by GoldenFox
	TD2HackVisualObject(FTarget).DragDrop(d2Data,Shift,P);	//Added by GoldenFox
  end;
  FTarget:=nil;
end;

procedure TD3CustomObjectLayer.DragEnd;
begin
end;

procedure TD3CustomObjectLayer.SetVisible(const Value:boolean);
begin
  inherited SetVisible(Value);
  if Visible then
    AddUpdateRect(d2Rect(0,0,FLayerWidth,FLayerHeight));
end;

procedure TD3CustomObjectLayer.EnterFocus;
var
  List:TList;
  i,CurId3:integer;
begin
  inherited ;
  List:=TList.Create;
  TD2VisualObject(Root).GetTabOrderList(List,true);
  for i:=0 to List.Count - 1 do
    if TD2Object(List[i]).isVisual and (TD2HackVisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) then
    begin
      TD2VisualObject(List[i]).SetFocus;
      Break;
    end;
  List.Free;
end;

procedure TD3CustomObjectLayer.KillFocus;
begin
  inherited ;
  SetFocused(nil);
end;

procedure TD3CustomObjectLayer.MouseLeave;
begin
  inherited ;
  if FHovered <> nil then
  begin
    TD2HackVisualObject(FHovered).MouseInObject:=false;
    TD2HackVisualObject(FHovered).MouseLeave;
    TD2VisualObject(FHovered).RemoveFreeNotify(Self);
  end;
  FHovered:=nil;
end;

procedure TD3CustomObjectLayer.SetActiveControl(AControl:TD2Control);
begin
  if AControl <> FActiveControl then
  begin
    FActiveControl:=AControl;
    if (FActiveControl <> nil) and not (csLoading in ComponentState) then
      FActiveControl.SetFocus;
  end;
end;

function TD3CustomObjectLayer.GetAnimatedCaret:boolean;
begin
  Result:=FAnimatedCaret;
end;

function TD3CustomObjectLayer.ShowKeyboardForControl(AObject:TD2Object):boolean;
begin
  Result:=false;
end;

function TD3CustomObjectLayer.HideKeyboardForControl(AObject:TD2Object):boolean;
begin
  Result:=false;
end;

//=================== TD3GUIScene2DLayer ========================================

constructor TD3GUIScene2DLayer.Create(AOwner:TComponent);
begin
  inherited;
  Projection:=d3ProjectionScreen;
  Width:=256;
  Height:=256;
  ZWrite:=false;
end;

           
