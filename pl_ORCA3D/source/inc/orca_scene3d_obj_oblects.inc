{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//================== TD3Camera =========================================

constructor TD3Camera.Create(AOwner:TComponent);
var
  i:integer;
  M:TD3VisualObject;
  Mesh:TD3Mesh;
  S:TStream;
begin
  inherited;
  HitTest:=false;
  FCanResize:=false;
  Position.Point:=d3Point(0,-10,0);

{  S:=TStringStream.Create(CameraMesh);
  try
    M:=TD3VisualObject(CreateObjectFromStream(Self,S));
    if M <> nil then
    begin
      M.Locked:=true;
      M.RotateAngle.Z:=-90;
      M.Parent:=Self;
      M.Stored:=false;
      for i:=0 to M.FChildren.Count - 1 do
        if TD3Object(M.FChildren[i]) is TD3Mesh then
          TD3Mesh(M.FChildren[i]).Material.Lighting:=false;
      M.Scale.Vector:=d3Vector(0.5,0.5,0.5);
    end;
  except
  end;
  S.Free;}
end;

destructor TD3Camera.Destroy;
begin
  inherited;
end;

procedure TD3Camera.DesignClick;
begin
  inherited;
  if FScene <> nil then
  begin
    if FSaveCamera=nil then
    begin
      FSaveCamera:=FScene.FDesignCamera;
      FScene.FDesignCamera:=Self;
      Repaint;
    end
    else
    begin
      FScene.FDesignCamera:=FSaveCamera;
      FSaveCamera:=nil;
    end;
  end;
end;

procedure TD3Camera.Paint;
begin
  if Tag=$FFFe then Exit;
  if (Assigned(FScene) and (FScene.DesignTime)) then
  begin
//    TD3VisualObject(FChildren[0]).Visible:=true;
    Canvas.SetRenderState(rsLightOff);
    Canvas.Material.Bitmap:='';
    Canvas.Material.Lighting:=false;
    Canvas.Material.NativeDiffuse:=$FF60a799;
    Canvas.FillCube(d3Vector(0,0,0),d3Vector(0.8,0.8,0.8),1);
    Canvas.Material.NativeDiffuse:=$FF9c60a7;
    Canvas.FillCube(d3Vector(0,0.5,0),d3Vector(0.3,1.4,0.3),1);
    Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3Vector(0,1,0),1000),1);
    Canvas.SetRenderState(rsLightOn);
  end
{  else
    TD3VisualObject(FChildren[0]).Visible:=false;}
end;

function TD3Camera.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
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
  if not (Assigned(FScene) and (FScene.DesignTime)) then Exit;
  Result:=RayCastIntersectsSphere(RayPos,RayDir,d3Vector(0,0,0),1);
  Intersection:=AbsolutePosition;
end;

procedure TD3Camera.Notification(AComponent:TComponent;
      Operation:TOperation);
begin
  inherited ;
  if (Operation=opRemove) and (AComponent=FTarget) then
  begin
    FTarget:=nil;
    MatrixChanged(Self);
  end;
end;

procedure TD3Camera.SetTarget(const Value:TD3VisualObject);
begin
  FTarget:=Value;
end;

//=============== TD3Light ====================================

constructor TD3Light.Create(AOwner:TComponent);
begin
  inherited;
  HitTest:=false;
  FEnabled:=true;
  FCanResize:=false;
end;

destructor TD3Light.Destroy;
begin
  inherited;
end;

procedure TD3Light.PreRender;
begin
  inherited;
  { set light }
  if Enabled and (Canvas <> nil) then
    Canvas.SetLight(Self);
end;

procedure TD3Light.Paint;
var
  i:integer;
begin
  if (Assigned(FScene) and (FScene.DesignTime)) then
  begin
    Canvas.SetRenderState(rsLightOff);
    Canvas.Material.NativeDiffuse:=$FFFFFFFF;
    Canvas.Material.Bitmap:='';
    Canvas.Material.Lighting:=false;
    Canvas.FillCube(d3Vector(0,0,0),d3Vector(0.8,0.8,0.8),1);
    case LightType of
      d3LightDirectional:
        begin
          Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3Vector(0,1,0),5),1);
        end;
      d3LightPoint:
        begin
          for i:=1 to 18 do
            Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3Vector(cos(DegToRad(i * 20)),sin(DegToRad(i * 20)),0),2),1);
          for i:=1 to 18 do
            Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3Vector(cos(DegToRad(i * 20)),0,sin(DegToRad(i * 20))),2),1);
        end;
      d3LightSpot:
        begin
          Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3VectorNormalize(d3Vector(0.2,1,-0.2)),5),1);
          Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3VectorNormalize(d3Vector(0.2,1,0.2)),5),1);
          Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3VectorNormalize(d3Vector(-0.2,1,0.2)),5),1);
          Canvas.DrawLine(d3Vector(0,0,0),d3VectorScale(d3VectorNormalize(d3Vector(-0.2,1,-0.2)),5),1);
        end;
    end;
    Canvas.SetRenderState(rsLightOn);
  end;
end;

function TD3Light.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
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
  if not (Assigned(FScene) and (FScene.DesignTime)) then Exit;
  e:=0.5 + 0.001; //Small value for floating point imprecisions
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
        Exit;
      end;
    end;
  end;
  Result:=false;
end;

procedure TD3Light.SetLightType(const Value:TD3LightType);
begin
  FLightType:=Value;
end;

//================ TD3Dummy ===============================================

constructor TD3Dummy.Create(AOwner:TComponent);
begin
  inherited;
  HitTest:=false;
end;

destructor TD3Dummy.Destroy;
begin
  inherited;
end;

procedure TD3Dummy.Paint;
begin
  if Tag=$FFFe then Exit;

  if ((FScene <> nil) and (FScene.DesignTime)) and not Locked then
  begin
    Canvas.Material.NativeDiffuse:=$8060a799;
    Canvas.DrawCube(d3Vector(0,0,0),d3Vector(Width,Height,Depth),AbsoluteOpacity);
  end;
end;

function TD3Dummy.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
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
  if not (Assigned(FScene) and (FScene.DesignTime)) then Exit;
  e:=0.5 + 0.001; //Small value for floating point imprecisions
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
        Exit;
      end;
    end;
  end;
  Result:=false;
end;

{ TD3ProxyObject }

constructor TD3ProxyObject.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD3ProxyObject.Destroy;
begin
  inherited;
end;

procedure TD3ProxyObject.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FSourceObject) then
    SourceObject:=nil;
end;

procedure TD3ProxyObject.Paint;
var
  SaveM:TD3Matrix;
  SaveS:TD3Vector;
begin
  if FSourceObject <> nil then
  begin
    SaveM:=FSourceObject.FAbsoluteMatrix;
    SaveS.X:=FSourceObject.FWidth;
    SaveS.Y:=FSourceObject.FHeight;
    SaveS.Z:=FSourceObject.FDepth;
    FSourceObject.FAbsoluteMatrix:=AbsoluteMatrix;
    FSourceObject.FWidth:=FWidth;
    FSourceObject.FHeight:=FHeight;
    FSourceObject.FDepth:=FDepth;
//    FSourceObject.RecalcAbsolute;
    FSourceObject.BeforePaint;
    FSourceObject.Paint;
    FSourceObject.AfterPaint;
    FSourceObject.PaintChildren;
    FSourceObject.FAbsoluteMatrix:=SaveM;
    FSourceObject.FWidth:=SaveS.X;
    FSourceObject.FHeight:=SaveS.Y;
    FSourceObject.FDepth:=SaveS.Z;
//    FSourceObject.RecalcAbsolute;
  end;
end;

function TD3ProxyObject.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
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
  if (Assigned(FScene) and (FScene.DesignTime)) then Exit;
  e:=0.5 + 0.001; //Small value for floating point imprecisions
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
        Exit;
      end;
    end;
  end;
  Result:=false;
end;

procedure TD3ProxyObject.SetSourceObject(const Value:TD3VisualObject);
begin
  if FSourceObject <> Value then
  begin
    FSourceObject:=Value;
    Repaint;
  end;
end;
       
           
