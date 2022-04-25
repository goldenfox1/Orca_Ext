{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


// =================== TD3VisualObject ========================

constructor TD3VisualObject.Create(AOwner:TComponent);
begin
  inherited;
  FCursor:=crDefault;
  FShowContextMenu:=true;
  FCanResize:=true;
  FCanRotate:=true;
  FOpacity:=1;
  FZWrite:=true;
  FLocalMatrix:=IdentityMatrix;
  FQuaternion:=IdentityQuaternion;
  FPosition:=TD3Position.Create(d3Point(0,0,0));
  FPosition.OnChange:=MatrixChanged;
  FScale:=TD3Position.Create(d3Point(1,1,1));
  FScale.OnChange:=MatrixChanged;
  FSkew:=TD3Position.Create(d3Point(0,0,0));
  FSkew.OnChange:=MatrixChanged;
  FRotateAngle:=TD3Position.Create(d3Point(0,0,0));
  FRotateAngle.OnChangeX:=RotateXChanged;
  FRotateAngle.OnChangeY:=RotateYChanged;
  FRotateAngle.OnChangeZ:=RotateZChanged;
  FRotateCenter:=TD3Position.Create(d3Point(0,0,0));
  FRotateCenter.OnChange:=MatrixChanged;
  FVelocity:=TD3Position.Create(d3Point(0,0,0));
  FMargins:=TD3Bounds.Create;
  FMargins.OnChange:=MarginsChanged;
  FPadding:=TD3Bounds.Create;
  FPadding.OnChange:=PaddingChanged;
  FWidth:=1;
  FLastWidth:=FWidth;
  FHeight:=1;
  FLastHeight:=FHeight;
  FDepth:=1;
  FLastDepth:=FDepth;
  FVisible:=true;
  FHitTest:=true;
  FRecalcAbsolute:=true;
  FRecalcOpacity:=true;
end;

destructor TD3VisualObject.Destroy;
begin
  FAbsoluteOpacity:=0;
  FVisible:=false; 
  DestroyBody;
  FVelocity.Free;
  FMargins.Free;
  FPadding.Free;
  FRotateCenter.Free;
  FRotateAngle.Free;
  FScale.Free;
  FSkew.Free;
  FPosition.Free;
  inherited;
end;

procedure TD3VisualObject.Loaded;
begin
  inherited;
  FLoadCursor:=Cursor;
  MatrixChanged(Self);
  if (FChildren <> nil) and (FChildren.Count > 0) then
    Realign;
  if Assigned(FScene) and (FScene.ActiveControl=Self) then
    SetFocus;
end;

procedure TD3VisualObject.PaddingChanged(Sender:TObject);
begin
  if (FParent <> nil) and (FParent.isVisual) then
    TD3VisualObject(FParent).Realign;
end;

procedure TD3VisualObject.MarginsChanged(Sender:TObject);
begin
  Realign;
end;

procedure TD3VisualObject.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('Quanternion',ReadQuaternion,WriteQuaternion,(FQuaternion.ImagPart.X <> 0) or (FQuaternion.ImagPart.Y <> 0) or
    (FQuaternion.ImagPart.Z <> 0) or (FQuaternion.RealPart <> 0));
end;

procedure TD3VisualObject.ReadQuaternion(Reader:TReader);
var
  S:ansistring;
begin
  S:=Reader.ReadString;
  try
    GetToken(S,',()');
    FQuaternion.ImagPart.X:=d3StrToFloat(GetToken(S,',()'));
    FQuaternion.ImagPart.Y:=d3StrToFloat(GetToken(S,',()'));
    FQuaternion.ImagPart.Z:=d3StrToFloat(GetToken(S,',()'));
    FQuaternion.ImagPart.W:=0;
    FQuaternion.RealPart:=d3StrToFloat(GetToken(S,',()'));
  except
  end;
end;

procedure TD3VisualObject.WriteQuaternion(Writer:TWriter);
var
  S:string;
begin
  S:='(' + d3FloatToStr(FQuaternion.ImagPart.X) + ',' + d3FloatToStr(FQuaternion.ImagPart.Y) + ',' + d3FloatToStr(FQuaternion.ImagPart.Z) + ',' + d3FloatToStr(FQuaternion.RealPart) + ')';
  Writer.WriteString(S);
end;

{ matrix }

procedure TD3VisualObject.RotateXChanged(Sender:TObject);
var
  q:TD3Quaternion;
  a:single;
begin
  a:=NormalizeDegAngle(RotateAngle.FX - RotateAngle.FSave.X);
  if a <> 0 then
  begin
    q:=QuaternionFromAngleAxis(-a,d3Vector(1,0,0){AbsoluteRight});
    FQuaternion:=QuaternionMultiply(FQuaternion,q);
    MatrixChanged(Sender);
    RotateAngle.FX:=NormalizeDegAngle(RotateAngle.FX);
    RotateAngle.FSave.X:=RotateAngle.FX;
  end;
end;

procedure TD3VisualObject.RotateYChanged(Sender:TObject);
var
  q:TD3Quaternion;
  a:single;
begin
  a:=NormalizeDegAngle(RotateAngle.FY - RotateAngle.FSave.Y);
  if a <> 0 then
  begin
    q:=QuaternionFromAngleAxis(a,d3Vector(0,1,0){AbsoluteDirection});
    FQuaternion:=QuaternionMultiply(FQuaternion,q);
    MatrixChanged(Sender);
    RotateAngle.FY:=NormalizeDegAngle(RotateAngle.FY);
    RotateAngle.FSave.Y:=RotateAngle.FY;
  end;
end;

procedure TD3VisualObject.RotateZChanged(Sender:TObject);
var
  q:TD3Quaternion;
  a:single;
begin
  a:=NormalizeDegAngle(RotateAngle.FZ - RotateAngle.FSave.Z);
  if a <> 0 then
  begin
    q:=QuaternionFromAngleAxis(-a,d3Vector(0,0,1){AbsoluteUp});
    FQuaternion:=QuaternionMultiply(FQuaternion,q);
    MatrixChanged(Sender);
    RotateAngle.FZ:=NormalizeDegAngle(RotateAngle.FZ);
    RotateAngle.FSave.Z:=RotateAngle.FZ;
  end;
end;

procedure TD3VisualObject.ResetRotateAngle;
begin
  FQuaternion:=IdentityQuaternion;
  MatrixChanged(Self);
  RotateAngle.FZ:=0;
  RotateAngle.FSave.Z:=0;
  RotateAngle.FY:=0;
  RotateAngle.FSave.Y:=0;
  RotateAngle.FX:=0;
  RotateAngle.FSave.X:=0;
end;

procedure TD3VisualObject.MatrixChanged(Sender:TObject);
var
  RightVector,DirectionVector,UpVector:TD3Vector;
  RotMatrix:TD3Matrix;
begin
  UpVector:=d3Vector(0,0,1);
  DirectionVector:=d3Vector(0,1,0);
  if (FRotateAngle.X <> 0) or (FRotateAngle.Y <> 0) or (FRotateAngle.Z <> 0)then
  begin
    RotMatrix:=QuaternionToMatrix(FQuaternion);
    UpVector:=d3VectorTransform(UpVector,RotMatrix);
    DirectionVector:=d3VectorTransform(DirectionVector,RotMatrix);
  end
  else
  begin
    RotateAngle.FSave:=d3Vector(0,0,0);
    FQuaternion:=IdentityQuaternion;
  end;
  RightVector:=d3VectorCrossProduct(DirectionVector,UpVector);
  FLocalMatrix.M[0]:=d3VectorScale(RightVector,Scale.X);
  FLocalMatrix.m14:=0;
  FLocalMatrix.M[1]:=d3VectorScale(DirectionVector,Scale.Y);
  FLocalMatrix.m24:=0;
  FLocalMatrix.M[2]:=d3VectorScale(UpVector,Scale.Z);
  FLocalMatrix.m34:=0;
  FLocalMatrix.m41:=FPosition.X;
  FLocalMatrix.m42:=FPosition.Y;
  FLocalMatrix.m43:=FPosition.Z;
  if Projection=d3ProjectionScreen then
  begin
{    FLocalMatrix.m41:=FPosition.X + (Width / 2);
    FLocalMatrix.m42:=FPosition.Y + (Height / 2);}
  end;
  RecalcAbsolute;
  if (FDynamic) and (FBody <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
  begin
    FScene.Physics.SetBodyMatrix(FBody,AbsoluteMatrix);
  end;
  Repaint;
end;

procedure TD3VisualObject.ProcessTick(time,deltaTime:single);
var
  L:TList;
  M:TD3Matrix;
  V:TD3Vector;
begin
  inherited;
  if (FScene.FInvisibleAtRuntime) then Exit;
   
  if FDynamic and (Body <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
  begin
    { Apply dynamics }
    M:=FScene.Physics.GetBodyMatrix(Body);
    FLocalMatrix:=M;
    M.M[0]:=AbsoluteToLocalVector(d3Vector(M.M[0].X,M.M[0].Y,M.M[0].Z,0));
    M.M[1]:=AbsoluteToLocalVector(d3Vector(M.M[1].X,M.M[1].Y,M.M[1].Z,0));
    M.M[2]:=AbsoluteToLocalVector(d3Vector(M.M[2].X,M.M[2].Y,M.M[2].Z,0));
    FQuaternion:=d3QuaternionFromMatrix(M);
    FPosition.SetVectorNoChange(AbsoluteToLocalVector(d3Vector(M.M[3].X,M.M[3].Y,M.M[3].Z,0)));
    RecalcAbsolute;
  end;
  if not FVelocity.Empty then
  begin
    V:=LocalToAbsoluteVector(d3Vector(FVelocity.X,FVelocity.Y,FVelocity.Z,0));
    FPosition.Vector:=d3VectorAdd(FPosition.Vector,d3VectorScale(V,deltaTime));
    if ColliseTrack and (Body <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
    begin
      L:=TList.Create;
      AbsoluteMatrix;
      FScene.Physics.Collise(Self,L);
      L.Free;
    end;
  end;
end;

procedure TD3VisualObject.Back;
var
  V:TD3Vector;
begin
  if not FVelocity.Empty then
  begin
    V:=LocalToAbsoluteVector(FVelocity.Vector);
    FPosition.Vector:=d3VectorAdd(FPosition.Vector,d3VectorScale(LocaltoAbsoluteVector(FVelocity.Vector),-FScene.DeltaTime));
  end;
end;

function TD3VisualObject.GetAbsoluteMatrix:TD3Matrix;
var
  S:TD3Matrix;
begin
  if FRecalcAbsolute then
  begin
    if (FParent <> nil) and (FParent is TD3VisualObject) then
      FAbsoluteMatrix:=d3MatrixMultiply(FLocalMatrix,TD3VisualObject(FParent).AbsoluteMatrix)
    else
      FAbsoluteMatrix:=FLocalMatrix;

    if (FCollider) and (FBody <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
    begin
      FScene.Physics.SetBodyMatrix(FBody,FAbsoluteMatrix);
    end;

    Result:=FAbsoluteMatrix;
    FRecalcAbsolute:=false;
    Repaint;
  end
  else
  begin
    Result:=FAbsoluteMatrix;
  end;
end;

function TD3VisualObject.GetAbsoluteDirection:TD3Vector;
begin
  Result:=AbsoluteMatrix.M[1];
end;

function TD3VisualObject.GetAbsoluteRight:TD3Vector;
begin
  Result:=AbsoluteMatrix.M[0];
end;

function TD3VisualObject.GetAbsoluteUp:TD3Vector;
begin
  Result:=AbsoluteMatrix.M[2];
end;

function TD3VisualObject.GetAbsolutePosition:TD3Vector;
begin
  Result:=AbsoluteMatrix.M[3];
end;

procedure TD3VisualObject.SetAbsolutePosition(Value:TD3Vector);
begin
  if (Parent <> nil) and (Parent.IsVisual) then
    Position.Vector:=AbsoluteToLocalVector(Value)
  else
    Position.Vector:=Value;
end;

function TD3VisualObject.GetInvertAbsoluteMatrix:TD3Matrix;
begin
  Result:=AbsoluteMatrix;
  InvertMatrix(Result);
end;

function TD3VisualObject.GetScreenBounds:TD2Rect;
var
  Pts:array [0..7] of TD3Point;
  A,B:TD3Point;
  i:integer;
begin
  Pts[0]:=d3Point(Width / 2,Height / 2,Depth / 2);
  Pts[1]:=d3Point(-Width / 2,Height / 2,Depth / 2);
  Pts[2]:=d3Point(-Width / 2,-Height / 2,Depth / 2);
  Pts[3]:=d3Point(-Width / 2,-Height / 2,-Depth / 2);
  Pts[4]:=d3Point(Width / 2,-Height / 2,Depth / 2);
  Pts[5]:=d3Point(Width / 2,Height / 2,-Depth / 2);
  Pts[6]:=d3Point(Width / 2,-Height / 2,-Depth / 2);
  Pts[7]:=d3Point(-Width / 2,Height / 2,-Depth / 2);
  for i:=0 to High(Pts) do
    Pts[i]:=Canvas.WorldToScreen(Projection,LocalToAbsolute(Pts[i]));
  { normalize }
  A:=d3Point($FFFF,$FFFF,$FFFF);
  B:=d3Point(-$FFFF,-$FFFF,-$FFFF);
  for i:=0 to High(Pts) do
  begin
    If Pts[i].X < A.X then A.X:=Pts[i].X;
    If Pts[i].Y < A.Y then A.Y:=Pts[i].Y;
    If Pts[i].X > B.X then B.X:=Pts[i].X;
    If Pts[i].Y > B.Y then B.Y:=Pts[i].Y;
  end;
  Result:=d2Rect(A.X,A.Y,B.X,B.Y);
end;

procedure TD3VisualObject.RecalcAbsolute;
var
  i:integer;
  Child:TD3VisualObject;
begin
  FRecalcAbsolute:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD3Object(FChildren[i]).isVisual then Continue;
      Child:=FChildren[i];
      TD3VisualObject(Child).RecalcAbsolute;
    end;
end;

function TD3VisualObject.AbsoluteToLocalVector(P:TD3Vector):TD3Vector;
begin
  Result:=d3VectorTransform(P,InvertAbsoluteMatrix);
end;

function TD3VisualObject.LocalToAbsoluteVector(P:TD3Vector):TD3Vector;
begin
  Result:=d3VectorTransform(P,AbsoluteMatrix);
end;

function TD3VisualObject.AbsoluteToLocal(P:TD3Point):TD3Point;
var
  V:TD3Vector;
begin
  V:=d3Vector(P.X,P.Y,P.Z);
  V:=d3VectorTransform(V,InvertAbsoluteMatrix);
  Result:=d3Point(V.X,V.Y,V.Z);
end;

function TD3VisualObject.LocalToAbsolute(P:TD3Point):TD3Point;
var
  V:TD3Vector;
begin
  V:=d3Vector(P.X,P.Y,P.Z);
  V:=d3VectorTransform(V,AbsoluteMatrix);
  Result:=d3Point(V.X,V.Y,V.Z);
end;

{ Opacity }

function TD3VisualObject.GetAbsoluteOpacity:single;
begin
  if FRecalcOpacity then
  begin
    if (FParent <> nil) and (FParent is TD3VisualObject) then
      FAbsoluteOpacity:=FOpacity * TD3VisualObject(FParent).AbsoluteOpacity
    else
      FAbsoluteOpacity:=FOpacity;

    Result:=FAbsoluteOpacity;
    FRecalcOpacity:=false;
  end
  else
  begin
    Result:=FAbsoluteOpacity;
  end;
end;

procedure TD3VisualObject.RecalcOpacity;
var
  i:integer;
  Child:TD3VisualObject;
begin
  FRecalcOpacity:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD3Object(FChildren[i]).isVisual then Continue;
      Child:=TD3VisualObject(FChildren[i]);
      TD3VisualObject(Child).RecalcOpacity;
    end;
end;

{ realtime }

procedure TD3VisualObject.RecreateBody;
begin
  if not (Assigned(FScene) and (FScene.DesignTime)) and not FScene.FInvisibleAtRuntime then
  begin
    DestroyBody;
    CreateBody;
  end;
end;

procedure TD3VisualObject.Collision(CollisionObject:TD3VisualObject;
  Point,Normal:TD3Vector);
begin
  if Assigned(OnCollision) then
    OnCollision(Self,CollisionObject,Point,Normal);
end;

procedure TD3VisualObject.AddForce(const Force:TD3Vector);
begin
  if (FDynamic) and (FScene.Physics <> nil) and (FBody <> 0) and (not FScene.FInvisibleAtRuntime) then
  begin
    FScene.Physics.AddForce(Self,Force);
  end;
end;

procedure TD3VisualObject.Spawn;
var
  i:integer;
begin
  CreateBody;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual then
        TD3VisualObject(FChildren[i]).Spawn;
end;

procedure TD3VisualObject.CreateBody;
begin
  if (FScene <> nil) and (FScene.Physics=nil) and (not FScene.FInvisibleAtRuntime) then
    FScene.CreatePhysics;
  if (FCollider or FDynamic) and (FScene.Physics <> nil) and (not FScene.FInvisibleAtRuntime) then
  begin
    FBody:=FScene.Physics.CreateBox(Self,d3Vector(Width,Height,Depth));
  end;
end;

procedure TD3VisualObject.DestroyBody;
begin
  if (FBody <> 0) and (FScene.Physics <> nil) and (not FScene.FInvisibleAtRuntime) then
  begin
    FScene.Physics.DestroyBody(FBody);
  end;
  FBody:=0;
end;

{ methods }

function TD3VisualObject.RayCastIntersect(const RayPos,RayDir:TD3Vector; var Intersection:TD3Vector):boolean;
begin
  Result:=true;
  Intersection:=AbsolutePosition;
end;

function TD3VisualObject.CheckHitTest(const AHitTest:boolean):boolean;
begin
  Result:=FHitTest;
  if ((Scene <> nil) and (Scene.FDesignTime)) then
    Result:=true;
  if ((Scene <> nil) and (Scene.FDesignTime)) and FLocked then
    Result:=false;
  if ((Scene <> nil) and (Scene.FDesignTime)) and FDesignHide then
    Result:=false;
end;

function TD3VisualObject.ObjectByPoint(X,Y:single; AProjection:TD3Projection; var Distance:single):TD3VisualObject;
var
  i:integer;
  Obj,NewObj:TD3VisualObject;
  IP,rPos,rDir:TD3Vector;
begin
  Result:=nil;
  if not Visible and not (Assigned(FScene) and (FScene.DesignTime)) then Exit;

  if FChildren <> nil then
    for i:=FChildren.Count - 1 downto 0 do
    begin
      if not TD3Object(FChildren[i]).IsVisual then Continue;
      Obj:=FChildren[i];
      if (not Obj.Visible) and not (Assigned(FScene) and (FScene.DesignTime)) then Continue;
      if (Obj.Locked) and (Assigned(FScene) and (FScene.DesignTime)) then Continue;
      if FDesignHide and (Assigned(FScene) and (FScene.DesignTime)) then Continue;

      NewObj:=Obj.ObjectByPoint(X,Y,AProjection,Distance);
      if NewObj <> nil then
      begin
        Result:=NewObj;
      end;
    end;

  if Result=nil then
  begin
    Canvas.Pick(X,Y,FProjection,rPos,rDir);
    if CheckHitTest(HitTest) and RayCastIntersect(AbsoluteToLocalVector(rPos),d3VectorNormalize(AbsoluteToLocalVector(rDir)),IP) then
    begin
      if (AProjection=Projection) and (Projection=d3ProjectionScreen) and (d3VectorLength(d3VectorSubtract(IP,rPos)) < Distance) then
      begin
        Distance:=d3VectorLength(d3VectorSubtract(IP,rPos));
        Result:=Self;
      end;
      if (AProjection=Projection) and (Projection=d3ProjectionCamera) and (Canvas.FCurrentCamera <> nil) and (d3VectorLength(d3VectorSubtract(IP,Canvas.FCurrentCamera.AbsolutePosition)) < Distance) then
      begin
        Distance:=d3VectorLength(d3VectorSubtract(IP,Canvas.FCurrentCamera.AbsolutePosition));
        Result:=Self;
      end;
    end;
  end;
end;

//function TD3VisualObject.FindTarget(const APoint:TD3Point; AProjection:TD3Projection; const Data:TD3DragObject; var Distance:single):TD3VisualObject;	//Deleted by GoldenFox
function TD3VisualObject.FindTarget(Shift: TShiftState; const APoint:TD3Point; AProjection:TD3Projection; const Data:TD3DragObject; var Distance:single):TD3VisualObject;	//Added by GoldenFox
var
  i:integer;
  Obj,NewObj:TD3VisualObject;
  IP,rPos,rDir:TD3Vector;
  Accept:boolean;
begin
  Result:=nil;
  if not Visible and not (Assigned(FScene) and (FScene.DesignTime)) then Exit;

  if FChildren <> nil then
    for i:=FChildren.Count - 1 downto 0 do
    begin
      if not TD3Object(FChildren[i]).IsVisual then Continue;
      Obj:=FChildren[i];
      if (not Obj.Visible) and not (Assigned(FScene) and (FScene.DesignTime)) then Continue;
      if (Obj.Locked) and (Assigned(FScene) and (FScene.DesignTime)) then Continue;
      if FDesignHide and (Assigned(FScene) and (FScene.DesignTime)) then Continue;

      //NewObj:=Obj.FindTarget(APoint,AProjection,Data,Distance);	//Deleted by GoldenFox
	  NewObj:=Obj.FindTarget(Shift,APoint,AProjection,Data,Distance);	//Added by GoldenFox
      if NewObj <> nil then
      begin
        Result:=NewObj;
      end;
    end;

  if Result=nil then
  begin
    Canvas.Pick(APoint.X,APoint.Y,FProjection,rPos,rDir);
    if CheckHitTest(HitTest) and RayCastIntersect(AbsoluteToLocalVector(rPos),d3VectorNormalize(AbsoluteToLocalVector(rDir)),IP) then
    begin
      if (AProjection=Projection) and (Projection=d3ProjectionScreen) and (d3VectorLength(d3VectorSubtract(IP,rPos)) < Distance) then
      begin
        Distance:=d3VectorLength(d3VectorSubtract(IP,rPos));
        Accept:=false;
        //DragOver(Data,AbsoluteToLocal(APoint),Accept);	//Deleted by GoldenFox
		DragOver(Data,Shift,AbsoluteToLocal(APoint),Accept);	//Added by GoldenFox
        if Accept then
          Result:=Self;
      end;
      if (AProjection=Projection) and (Projection=d3ProjectionCamera) and (Canvas.FCurrentCamera <> nil) and (d3VectorLength(d3VectorSubtract(IP,Canvas.FCurrentCamera.AbsolutePosition)) < Distance) then
      begin
        Distance:=d3VectorLength(d3VectorSubtract(IP,Canvas.FCurrentCamera.AbsolutePosition));
        Accept:=false;
        //DragOver(Data,AbsoluteToLocal(APoint),Accept);	//Deleted by GoldenFox
		DragOver(Data,Shift,AbsoluteToLocal(APoint),Accept);	//Added by GoldenFox
        if Accept then
          Result:=Self;
      end;
    end;
  end;
end;

function TD3VisualObject.GetCanvas:TD3Canvas;
begin
  if FTempCanvas <> nil then
    Result:=FTempCanvas
  else
    if FScene <> nil then
      Result:=FScene.Canvas
    else
      Result:=nil;
end;

procedure TD3VisualObject.PreRender;
var
  i:integer;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual then
        TD3VisualObject(FChildren[i]).PreRender;
end;

procedure TD3VisualObject.BeforePaint;
begin
  if Assigned(OnBeforePaint) then
    OnBeforePaint(Self,Canvas);
end;

procedure TD3VisualObject.AfterPaint;
begin
end;

procedure TD3VisualObject.ApplyResource;
var
  i:integer;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual then
        TD3VisualObject(FChildren[i]).ApplyResource;
end;

procedure TD3VisualObject.Paint;
begin

end;

procedure TD3VisualObject.PaintToVxBitmap(ABitmap:TD2Bitmap; AWidth,AHeight:integer;
  ClearColor:TD3Color; AutoFit:boolean=false);
var
  B:TD2Bitmap;
  D:TD3Bitmap;
  FillR,R:TD2Rect;
  i,j:integer;
  ratio:single;
begin
  if AutoFit then
  begin
    { Render to bitmap }
    B:=TD2Bitmap.Create(1,1);
    PaintToVxBitmap(B,AWidth,AHeight,0,false); // - render with alpha
    { calc best size }
    R:=d2Rect(B.Width,B.Height,0,0);
    for i:=0 to B.Width - 1 do
      for j:=0 to B.Height - 1 do
      begin
        if TD2ColorRec(B.Scanline[j][i]).A > 0 then
        begin
          if i < R.Left then R.Left:=i;
          if j < R.Top then R.Top:=j;
          if i > R.Right then R.Right:=i;
          if j > R.Bottom then R.Bottom:=j;
        end;
      end;
    FillR:=R;
    ratio:=d2FitRect(R,d2Rect(0,0,AWidth,AHeight));
    if (ratio > 0) and (ratio < 1) then
    begin
      ABitmap.SetSize(AWidth,AHeight);
      // render again to better size
      PaintToVxBitmap(B,round(AWidth / ratio),round(AHeight / ratio),0,false);
      { calc again }
      R:=d2Rect(B.Width,B.Height,0,0);
      for i:=0 to B.Width - 1 do
        for j:=0 to B.Height - 1 do
        begin
          if TD2ColorRec(B.Scanline[j][i]).A > 0 then
          begin
            if i < R.Left then R.Left:=i;
            if j < R.Top then R.Top:=j;
            if i > R.Right then R.Right:=i;
            if j > R.Bottom then R.Bottom:=j;
          end;
        end;
      FillR:=R;
      ratio:=d2FitRect(FillR,d2Rect(0,0,AWidth,AHeight));
      ABitmap.Clear(d2CorrectColor(ClearColor));
      ABitmap.Canvas.DrawBitmap(B,d2Rect(R.left,R.top,R.left + d2RectWidth(R),R.top + d2RectHeight(FillR)),FillR,1,true);
    end;
    B.Free;
  end
  else
  begin
    D:=TD3Bitmap.Create(1,1);
    PaintToBitmap(D,AWidth,AHeight,ClearColor); // - render with alpha
    ABitmap.Assign(D);
    D.Free;
  end;
end;

procedure TD3VisualObject.PaintToBitmap(ABitmap:TD3Bitmap; AWidth,AHeight:integer; ClearColor:TD3Color);
var
  i:integer;
  FitR,R:TD2Rect;
  S,T:TD3Matrix;
  scale:single;
begin
  R:=ScreenBounds;
  if d2IsRectEmpty(R) then Exit;
  FitR:=R;
  if AWidth > cnMaxBitmapSize then AWidth:=cnMaxBitmapSize;
  if AHeight > cnMaxBitmapSize then AHeight:=cnMaxBitmapSize;
  scale:=1 / d2FitRect(FitR,d2Rect(0,0,AWidth,AHeight));

  ABitmap.SetSize(round(d2RectWidth(R) * scale),round(d2RectHeight(R) * scale));
  if Assigned(FScene) then
  begin
    S:=IdentityMatrix;
    S.m11:=FScene.Height / d2RectHeight(R);
    S.m22:=S.m11;

    T:=IdentityMatrix;
    T.m41:=({OffsetX + }((FScene.Width / 2) - ((R.Left + R.Right) / 2))) / d2RectWidth(R) * 2;
    T.m42:=-({OffsetY + }((FScene.Height / 2) - ((R.Top + R.Bottom) / 2))) / d2RectHeight(R) * 2;
    ABitmap.Canvas.FPaintToMatrix:=d3MatrixMultiply(S,T);
    ABitmap.Canvas.CreateRenderStateList(rs3DScene);
  end;

  TempCanvas:=ABitmap.Canvas;
  try
    if Assigned(FScene) then
    begin
      // clone camera and lights
      Canvas.Reset;
      Canvas.Lighting:=FScene.FLighting;
      Canvas.Ambient:=FScene.FAmbient;
      { set matrix and camera }
      Canvas.FCurrentCameraMatrix:=FScene.Canvas.FCurrentCameraMatrix;
      Canvas.FCurrentCameraInvMatrix:=FScene.Canvas.FCurrentCameraInvMatrix;
      { set lights }
      if FScene.FChildren <> nil then
        for i:=0 to FScene.FChildren.Count - 1 do
        begin
          if not (TObject(FScene.FChildren[i]) is TD3VisualObject) then Continue;
          if not TD3VisualObject(FScene.FChildren[i]).Visible then Continue;
          Canvas.SetMatrix(TD3VisualObject(FScene.FChildren[i]).AbsoluteMatrix);
          TD3VisualObject(FScene.FChildren[i]).TempCanvas:=Canvas;
          TD3VisualObject(FScene.FChildren[i]).PreRender;
        end;
    end;
    // render
    if Canvas.BeginScene then
    begin
      Canvas.SetRenderState(rsGouraud);
      Canvas.SetRenderState(rs3DScene);
      Canvas.SetRenderState(rsZWriteOn);
      Canvas.SetRenderState(rsLightOn);
      Canvas.SetRenderState(rsTexLinear);
      Canvas.SetRenderState(rsFrontFace);
      Canvas.SetMatrix(AbsoluteMatrix);
      Canvas.Clear(ClearColor);
      if FZWrite then
        Canvas.SetRenderState(rsZWriteOn)
      else
        Canvas.SetRenderState(rsZWriteOff);
      if Projection=d3ProjectionCamera then
        Canvas.SetRenderState(rs3DScene)
      else
        Canvas.SetRenderState(rs2DScene);
      if TwoSide then
        Canvas.SetRenderState(rsAllFace)
      else
        Canvas.SetRenderState(rsFrontFace);
      BeforePaint;
      Paint;
      if Assigned(OnPaint) then
        OnPaint(Self,Canvas);
      AfterPaint;
      PaintChildren;
      Canvas.FlushBuffer;
    end;
    if Assigned(FScene) then
    begin
      { reset lights }
      if FScene.FChildren <> nil then
        for i:=0 to FScene.FChildren.Count - 1 do
        begin
          if not (TObject(FScene.FChildren[i]) is TD3VisualObject) then Continue;
          if not TD3VisualObject(FScene.FChildren[i]).Visible then Continue;
          TD3VisualObject(FScene.FChildren[i]).TempCanvas:=nil;
        end;
    end;
  finally
    TempCanvas:=nil;
  end;
end;

procedure TD3VisualObject.CreateTileSnapshot(ABitmap:TD3Bitmap; AWidth,AHeight,OffsetX,OffsetY:integer; Scale:single; ClearColor:TD3Color);
var
  i:integer;
  FitR,R:TD2Rect;
  S,T:TD3Matrix;
begin
  R:=ScreenBounds;
  if d2IsRectEmpty(R) then Exit;
  FitR:=d2Rect(R.left * Scale,R.Top * Scale,R.Right * Scale,R.Bottom * Scale);
  if AWidth > cnMaxBitmapSize then AWidth:=cnMaxBitmapSize;
  if AHeight > cnMaxBitmapSize then AHeight:=cnMaxBitmapSize;
  d2RectCenter(FitR,d2Rect(0,0,AWidth,AHeight));

  ABitmap.SetSize(AWidth,AHeight);
  if Assigned(FScene) then
  begin
    S:=IdentityMatrix;
    S.m11:=Min(FScene.Height / AHeight,(FScene.Width / AWidth)) * Scale;
    S.m22:=S.m11;

    T:=IdentityMatrix;
    T.m41:=(((-FitR.Left - offsetx) / Scale) + ((FScene.Width / 2) - ((R.Left + R.Right) / 2))) / AWidth * 2 * Scale;
    T.m42:=-(((-FitR.Top - offsety) / Scale) + ((FScene.Height / 2) - ((R.Top + R.Bottom) / 2))) / AHeight * 2 * Scale;
    ABitmap.Canvas.FPaintToMatrix:=d3MatrixMultiply(S,T);
    ABitmap.Canvas.CreateRenderStateList(rs3DScene);
  end;

  TempCanvas:=ABitmap.Canvas;
  try
    if Assigned(FScene) then
    begin
      // clone camera and lights
      Canvas.Reset;
      Canvas.Lighting:=FScene.FLighting;
      Canvas.Ambient:=FScene.FAmbient;
      { set matrix and camera }
      Canvas.FCurrentCameraMatrix:=FScene.Canvas.FCurrentCameraMatrix;
      Canvas.FCurrentCameraInvMatrix:=FScene.Canvas.FCurrentCameraInvMatrix;
      { set lights }
      if FScene.FChildren <> nil then
        for i:=0 to FScene.FChildren.Count - 1 do
        begin
          if not (TObject(FScene.FChildren[i]) is TD3VisualObject) then Continue;
          if not TD3VisualObject(FScene.FChildren[i]).Visible then Continue;
          Canvas.SetMatrix(TD3VisualObject(FScene.FChildren[i]).AbsoluteMatrix);
          TD3VisualObject(FScene.FChildren[i]).TempCanvas:=Canvas;
          TD3VisualObject(FScene.FChildren[i]).PreRender;
        end;
    end;
    // render
    if Canvas.BeginScene then
    begin
      Canvas.SetRenderState(rsGouraud);
      Canvas.SetRenderState(rs3DScene);
      Canvas.SetRenderState(rsZWriteOn);
      Canvas.SetRenderState(rsLightOn);
      Canvas.SetRenderState(rsTexLinear);
      Canvas.SetRenderState(rsFrontFace);
      Canvas.SetMatrix(AbsoluteMatrix);
      Canvas.Clear(ClearColor);
      if FZWrite then
        Canvas.SetRenderState(rsZWriteOn)
      else
        Canvas.SetRenderState(rsZWriteOff);
      if Projection=d3ProjectionCamera then
        Canvas.SetRenderState(rs3DScene)
      else
        Canvas.SetRenderState(rs2DScene);
      if TwoSide then
        Canvas.SetRenderState(rsAllFace)
      else
        Canvas.SetRenderState(rsFrontFace);
      BeforePaint;
      Paint;
      if Assigned(OnPaint) then
        OnPaint(Self,Canvas);
      AfterPaint;
      PaintChildren;
      Canvas.FlushBuffer;
    end;
    if Assigned(FScene) then
    begin
      { reset lights }
      if FScene.FChildren <> nil then
        for i:=0 to FScene.FChildren.Count - 1 do
        begin
          if not (TObject(FScene.FChildren[i]) is TD3VisualObject) then Continue;
          if not TD3VisualObject(FScene.FChildren[i]).Visible then Continue;
          TD3VisualObject(FScene.FChildren[i]).TempCanvas:=nil;
        end;
    end;
    ABitmap.NeedUpdate:=true;
  finally
    TempCanvas:=nil;
  end;
end;

procedure TD3VisualObject.CreateHighQualitySnapshot(ABitmap:TD2Bitmap; AWidth,AHeight:integer; ClearColor:TD3Color; Quality:integer);
const
  TileSize=512;
var
  i,j:integer;
  Sample,VTile:TD2Bitmap;
  Tile:TD3Bitmap;
  Obj:TD3VisualObject;
  R,FitR,TileR:TD2Rect;
  factor:single;
begin
  if Quality < 1 then Quality:=1;
  if Quality > 16 then Quality:=16;
  R:=ScreenBounds;
  FitR:=R;
  factor:=d2FitRect(FitR,d2Rect(0,0,AWidth,AHeight));
  if factor < 1 then
  begin
    R:=d2Rect(R.Left / factor,R.Top / factor,R.Right / factor,R.Bottom / factor);
    d2RectCenter(R,d2Rect(0,0,AWidth,AHeight));
  end
  else
    R:=FitR;
  Sample:=TD2Bitmap.Create(round(d2RectWidth(R) * Quality),round(d2RectHeight(R) * Quality));
  Tile:=TD3Bitmap.Create(TileSize,TileSize);
  VTile:=TD2Bitmap.Create(TileSize,TileSize);
  for i:=0 to Sample.Width div TileSize do
    for j:=0 to Sample.Height div TileSize do
    begin
      CreateTileSnapshot(Tile,TileSize,TileSize,i * TileSize,j * TileSize,Quality / factor,ClearColor);
      VTile.Assign(Tile);
      TileR:=d2Rect(0,0,TileSize,TileSize);
      d2OffsetRect(TileR,i * TileSize,j * TileSize);
      Sample.Canvas.DrawBitmap(VTile,d2Rect(0,0,TileSize,TileSize),TileR,1,true);
    end;

  ABitmap.SetSize(AWidth,AHeight);
  ABitmap.Canvas.DrawBitmap(Sample,d2Rect(0,0,Sample.Width,Sample.Height),R,1);

  VTile.Free;
  Tile.Free;
  Sample.Free;
end;

procedure TD3VisualObject.PaintChildren;
var
  i,j:integer;
  R:TD2Rect;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).IsVisual and
         ((TD3VisualObject(FChildren[i]).Visible) or
          (not TD3VisualObject(FChildren[i]).Visible and (Assigned(FScene) and (FScene.DesignTime)) and not TD3VisualObject(FChildren[i]).Locked)) then
      begin
        with TD3VisualObject(FChildren[i]) do
        begin
          if (Assigned(FScene) and (FScene.DesignTime)) and FDesignHide then Continue;

          FScene:=Self.FScene;
          if (Abs(Scale.X) < 0.01) or (Abs(Scale.Y) < 0.01) then Continue;

          if FZWrite then
            Canvas.SetRenderState(rsZWriteOn)
          else
            Canvas.SetRenderState(rsZWriteOff);
          if Projection=d3ProjectionCamera then
            Canvas.SetRenderState(rs3DScene)
          else
            Canvas.SetRenderState(rs2DScene);
          if TwoSide then
            Canvas.SetRenderState(rsAllFace)
          else
            Canvas.SetRenderState(rsFrontFace);

          Canvas.SetMatrix(AbsoluteMatrix);
          BeforePaint;
          Paint;
          if Assigned(FOnPaint) then
            FOnPaint(TD3VisualObject(Self.FChildren[i]),Canvas);
          AfterPaint;
          PaintChildren;

          // design selection
          if (GvarD3Designer <> nil) and (Assigned(Self.Scene)) and (Self.FChildren[i] <> Scene.Selected) and
             (GvarD3Designer.IsSelected(Self.Scene.Owner,TD3VisualObject(Self.FChildren[i]))) then
          begin
            Canvas.SetMatrix(AbsoluteMatrix);
            Canvas.Material.Diffuse:='#B200CC5A';
            Canvas.SetRenderState(rsGouraud);
            Canvas.SetRenderState(rsZWriteOn);
            Canvas.SetRenderState(rsLightOff);
            Canvas.SetRenderState(rsTexDisable);
            Canvas.DrawCube(d3Vector(0,0,0),d3Vector(Width + 0.01,Height + 0.01,Depth + 0.01),0.4);
          end;
          if not FDragDisableHighlight and (IsDragOver) then
          begin
            Canvas.SetMatrix(AbsoluteMatrix);
            Canvas.Material.Diffuse:='#B2005ACC';
            Canvas.SetRenderState(rsGouraud);
            Canvas.SetRenderState(rsZWriteOn);
            Canvas.SetRenderState(rsLightOff);
            Canvas.SetRenderState(rsTexDisable);
            Canvas.FillCube(d3Vector(0,0,0),d3Vector(Width + 0.01,Height + 0.01,Depth + 0.01),0.4);
          end;
          {$IFDEF DRAWFOCUSED}
          if CanFocused and FIsFocused then
          begin
            Canvas.SetMatrix(AbsoluteMatrix);
            Canvas.Material.Diffuse:='#CFFF0000';
            Canvas.SetRenderState(rsGouraud);
            Canvas.SetRenderState(rsZWriteOn);
            Canvas.SetRenderState(rsLightOff);
            Canvas.SetRenderState(rsTexDisable);
            Canvas.FillCube(d3Vector(0,0,0),d3Vector(Width + 0.01,Height + 0.01,Depth + 0.01),0.1);
          end;
          {$ENDIF}
        end;
      end;
end;

procedure TD3VisualObject.Repaint;
begin
  if not Visible then Exit;
  if FScene=nil then Exit;
  if not (Assigned(FScene) and (FScene.DesignTime)) and FScene.RealTime then Exit;
  if csDestroying in ComponentState then Exit;
  FScene.AddUpdateRect(d2Rect(0,0,0,0));
end;

procedure TD3VisualObject.Lock;
var
  i:integer;
begin
  Locked:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual then
        TD3VisualObject(FChildren[i]).Lock;
end;


procedure TD3VisualObject.DesignSelect;
begin

end;

procedure TD3VisualObject.DesignClick;
begin

end;

procedure TD3VisualObject.Realign;
var
  i:integer;
  R:TD3Box;
  AlignList:TList;

  function InsertBefore(C1,C2:TD3VisualObject; AAlign:TD3Align):boolean;
  begin
    Result:=False;
    case AAlign of
      d3Top:Result:=C1.Position.Y < C2.Position.Y;
      d3Bottom:Result:=(C1.Position.Y + C1.Height) >= (C2.Position.Y + C2.Height);
      d3Left,d3MostLeft:Result:=C1.Position.X < C2.Position.X;
      d3Right,d3MostRight:Result:=(C1.Position.X + C1.Width) >= (C2.Position.X + C2.Width);
    end;
  end;

  procedure DoPosition(Control:TD3VisualObject; AAlign:TD3Align; AlignInfo:TD3AlignInfo);
  var
    NewNear,NewLeft,NewTop,NewWidth,NewHeight,NewDepth:single;
    ParentSize:TPoint;
  begin
    if not Control.Visible then Exit;
    with R do
    begin
      NewWidth:=Right - Left;
      if (NewWidth < 0) or (AAlign in [d3Left,d3Right,d3Vertical,d3MostLeft,d3MostRight,d3TopLeft,d3TopRight,d3BottomLeft,d3BottomRight]) then
        NewWidth:=Control.Width + Control.Padding.Left + Control.Padding.Right;
      NewHeight:=Bottom - Top;
      if (NewHeight < 0) or (AAlign in [d3Top,d3Bottom,d3Horizontal,d3TopLeft,d3TopRight,d3BottomLeft,d3BottomRight]) then
        NewHeight:=Control.Height + Control.Padding.Top + Control.Padding.Bottom;
      NewDepth:=Far - Near;
      if (NewDepth < 0) then
        NewDepth:=Control.Depth + Control.Padding.zNear + Control.Padding.zFar;
      NewLeft:=Left + ((Control.Width) / 2);
      NewTop:=Top + ((Control.Height) / 2);
      NewNear:=Near + (Control.Depth / 2);
      if (AAlign in [d3Vertical]) then
        NewLeft:=Control.Position.X + Control.Padding.Left;
      if (AAlign in [d3Horizontal]) then
        NewTop:=Control.Position.Y + Control.Padding.Top;
      case AAlign of
        d3Top:
          begin
            Top:=Top + NewHeight;
          end;
        d3Bottom:
          begin
            Bottom:=Bottom - NewHeight;
            NewTop:=Bottom + (Control.Height / 2);
          end;
        d3Left,d3MostLeft:
          Left:=Left + NewWidth;
        d3Right,d3MostRight:
          begin
            Right:=Right - NewWidth;
            NewLeft:=Right + (Control.Width / 2);
          end;
        d3Contents:
          begin
            NewLeft:=0;
            NewTop:=0;
            NewNear:=0;
            NewWidth:=Width;
            NewHeight:=Height;
            NewDepth:=Depth;
            Control.FPosition.FX:=NewLeft + Control.Padding.Left / 2 - Control.Padding.Right / 2;
            Control.FPosition.FY:=NewTop + Control.Padding.Top / 2 - Control.Padding.Bottom / 2;
            Control.FPosition.FZ:=NewNear + Control.Padding.zNear / 2 - Control.Padding.zFar / 2;
            Control.FWidth:=NewWidth - Control.Padding.Left - Control.Padding.Right;
            Control.FHeight:=NewHeight - Control.Padding.Top - Control.Padding.Bottom;
            Control.FDepth:=NewDepth - Control.Padding.zNear - Control.Padding.zFar;
            Control.MatrixChanged(Self);
            Control.Realign;
            Exit;
          end;
        d3Center:
          begin
            NewLeft:=Left + Trunc((NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right)) / 2);
            NewWidth:=(Control.Width + Control.Padding.Left + Control.Padding.Right);
            NewTop:=Top + Trunc((NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom)) / 2);
            NewHeight:=(Control.Height + Control.Padding.Top + Control.Padding.Bottom);
            NewNear:=Near + Trunc((NewDepth - (Control.Depth + Control.Padding.zNear + Control.Padding.zFar)) / 2);
            NewDepth:=(Control.Depth + Control.Padding.zNear + Control.Padding.zFar);
          end;
        d3TopRight:
          begin
            Control.Position.X:=Control.Position.X + ((FWidth - FLastWidth) / 2);
            Control.Position.Y:=Control.Position.Y - ((FHeight - FLastHeight) / 2);
            Exit;
          end;
        d3TopLeft:
          begin
            Control.Position.X:=Control.Position.X - ((FWidth - FLastWidth) / 2);
            Control.Position.Y:=Control.Position.Y - ((FHeight - FLastHeight) / 2);
            Exit;
          end;
        d3BottomLeft:
          begin
            Control.Position.X:=Control.Position.X - ((FWidth - FLastWidth) / 2);
            Control.Position.Y:=Control.Position.Y + ((FHeight - FLastHeight) / 2);
            Exit;
          end;
        d3BottomRight:
          begin
            Control.Position.X:=Control.Position.X + ((FWidth - FLastWidth) / 2);
            Control.Position.Y:=Control.Position.Y + ((FHeight - FLastHeight) / 2);
            Exit;
          end;
      end;
    end;

    if (AALign=d3Scale) then
    begin
      if (FLastWidth > 0) and (FLastHeight > 0) and (FLastDepth > 0) and(FWidth > 0) and (FHeight > 0) and (FDepth > 0) then
      begin
        Control.FPosition.FX:=Control.FPosition.X * (FWidth / FLastWidth);
        Control.FPosition.FY:=Control.FPosition.Y * (FHeight / FLastHeight);
        Control.FPosition.FZ:=Control.FPosition.Z * (FDepth / FLastDepth);
        Control.FWidth:=Control.FWidth * (FWidth / FLastWidth);
        Control.FHeight:=Control.FHeight * (FHeight / FLastHeight);
        Control.FDepth:=Control.FDepth * (FDepth / FLastDepth);
        Control.MatrixChanged(Self);
        Control.Realign;
      end;
      Exit;
    end
    else
    begin
      Control.FPosition.FX:=NewLeft + Control.Padding.Left;
      Control.FPosition.FY:=NewTop + Control.Padding.Top;
      Control.FPosition.FZ:=NewNear + Control.Padding.zNear;
      if (Control.FWidth <> NewWidth - Control.Padding.Left - Control.Padding.Right) or
         (Control.FHeight <> NewHeight - Control.Padding.Top - Control.Padding.Bottom) or
         (Control.FDepth <> NewDepth - Control.Padding.zNear - Control.Padding.zFar) then
      begin
        Control.FWidth:=NewWidth - Control.Padding.Left - Control.Padding.Right;
        Control.FHeight:=NewHeight - Control.Padding.Top - Control.Padding.Bottom;
        Control.FDepth:=NewDepth - Control.Padding.zNear - Control.Padding.zFar;
        Control.Realign;
      end;
      Control.MatrixChanged(Self);
    end;

    if (Control.Width + Control.Padding.Left + Control.Padding.Right <> NewWidth) or
       (Control.Height + Control.Padding.Top + Control.Padding.Bottom <> NewHeight) or
       (Control.Depth + Control.Padding.zNear + Control.Padding.zFar <> NewDepth) then
      with R do
        case AAlign of
          d3Top:
            Top:=Top - (NewHeight - (Control.Height + Control.Padding.Left + Control.Padding.Right));
          d3Bottom:
            Bottom:=Bottom + (NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom));
          d3Left:
            Left:=Left - (NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right));
          d3Right:
            Right:=Right + (NewWidth - (Control.Width + Control.Padding.Top + Control.Padding.Bottom));
          d3Client:
            begin
              Right:=Right + NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right);
              Bottom:=Bottom + NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom);
              Far:=Far + NewDepth - (Control.Depth + Control.Padding.zNear + Control.Padding.zFar);
            end;
        end;
  end;

  procedure DoAlign(AAlign:TD3Align);
  var
    I,J:integer;
    Control:TD3VisualObject;
    AlignInfo:TD3AlignInfo;
  begin
    AlignList.Clear;
    for I:=0 to FChildren.Count - 1 do
    begin
      if not TD3Object(FChildren[i]).isVisual then Continue;
      Control:=TD3VisualObject(FChildren[i]);
      if (Control.Align=AAlign) and ((AAlign=d3None) or (Control.Visible) or
         (Assigned(FScene) and (FScene.DesignTime))) then
      begin
        J:=0;
        while (J < AlignList.Count) and not InsertBefore(Control,TD3VisualObject(AlignList[J]),AAlign) do Inc(J);
        AlignList.Insert(J,Control);
      end;
    end;
    for I:=0 to AlignList.Count - 1 do
    begin
      AlignInfo.AlignList:=AlignList;
      AlignInfo.ControlIndex:=I;
      AlignInfo.Align:=AAlign;
      DoPosition(TD3VisualObject(AlignList[I]),AAlign,AlignInfo);
    end;
  end;

{  function AlignWork:boolean;
  var
    I:integer;
  begin
    Result:=True;
    for I:=ControlCount - 1 downto 0 do
      if (Controls[I].Align <> alNone) or
        (Controls[I].Anchors <> [akLeft,akTop]) then Exit;
    Result:=False;
  end;}

const
  AllowAlign:boolean=true;
  NeedAlign:boolean=false;
  NeedLayerAlign:boolean=false;
begin
  if csDestroying in ComponentState then Exit;
  if ((FWidth > -0.2) and (FWidth < 0.2)) or ((FHeight > -0.2) and (FHeight < 0.2)) {or ((FDepth > -0.2) and (FDepth < 0.2)) }then Exit;
  if FDisableAlign then Exit;
  if csLoading in ComponentState then
  begin
    FLastWidth:=FWidth;
    FLastHeight:=FHeight;
    FLastDepth:=FDepth;
    Exit;
  end;
  if FChildren=nil then Exit;
  if FChildren.Count=0 then Exit;

  FDisableAlign:=true;
  try
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD3Object(FChildren[i]).isVisual and (TD3VisualObject(FChildren[i]).Align <> d3None) then
      begin
        NeedAlign:=true;
      end;
      if (TD3Object(FChildren[i]) is TD3CustomLayer) and (TD3CustomLayer(FChildren[i]).LayerAlign <> d3LayerNone) then
      begin
        NeedLayerAlign:=true;
      end;
    end;
    if NeedLayerAlign then
      RealignLayers;
    if NeedAlign then
    begin
      if AllowAlign then
      begin
        R.Left:=-FWidth / 2;
        R.Top:=-FHeight / 2;
        R.Near:=-FDepth / 2;
        R.Right:=FWidth / 2;
        R.Bottom:=FHeight / 2;
        R.Far:=FDepth / 2;
        AlignList:=TList.Create;
        try
          DoAlign(d3Scale);
          DoAlign(d3Contents);
          DoAlign(d3MostLeft);
          DoAlign(d3MostRight);
          DoAlign(d3Top);
          DoAlign(d3Bottom);
          DoAlign(d3Left);
          DoAlign(d3Right);
          DoAlign(d3Client);
          DoAlign(d3Horizontal);
          DoAlign(d3Vertical);
          DoAlign(d3Contents);
          DoAlign(d3Center);
          // Move anchored controls
          DoAlign(d3TopLeft);
          DoAlign(d3TopRight);
          DoAlign(d3BottomLeft);
          DoAlign(d3BottomRight);
        finally
          AlignList.Free;
        end;
      end;
      FLastWidth:=FWidth;
      FLastHeight:=FHeight;
      FLastDepth:=FDepth;
      Repaint;
    end;
  finally
    FDisableAlign:=false;
  end;
end;

procedure TD3VisualObject.RealignLayers;
var
  i:integer;
  R,FullR:TD2Rect;
  AlignList:TList;

  function InsertBefore(C1,C2:TD3CustomLayer; AAlign:TD3LayerAlign):boolean;
  begin
    Result:=False;
    case AAlign of
      d3LayerTop,d3LayerMostTop:Result:=(C1.Position.Y - (C1.Height / 2)) < (C2.Position.Y - (C2.Height / 2));
      d3LayerBottom,d3LayerMostBottom:Result:=(C1.Position.Y + (C1.Height / 2)) >= (C2.Position.Y + (C2.Height / 2));
      d3LayerLeft,d3LayerMostLeft:Result:=(C1.Position.X - (C1.Width / 2)) < (C2.Position.X - (C2.Width / 2));
      d3LayerRight,d3LayerMostRight:Result:=(C1.Position.X + (C1.Width / 2)) >= (C2.Position.X + (C2.Width / 2));
    end;
  end;

  procedure DoPosition(Control:TD3CustomLayer; AAlign:TD3LayerAlign);
  var
    NewLeft,NewTop,NewWidth,NewHeight:single;
    cR,mR:TD2Rect;
    fitScale:single;
  begin
    if not Control.Visible and not FScene.Designtime then Exit;
    with R do
    begin
      NewWidth:=Right - Left;
      if (NewWidth < 0) or (AAlign in [d3LayerLeft,d3LayerRight,d3LayerMostLeft,d3LayerMostRight]) then
        NewWidth:=Control.Width{ + Control.Padding.Left + Control.Padding.Right};
      NewHeight:=Bottom - Top;
      if (NewHeight < 0) or (AAlign in [d3LayerTop,d3LayerBottom,d3LayerMostTop,d3LayerMostBottom]) then
        NewHeight:=Control.Height{ + Control.Padding.Top + Control.Padding.Bottom};
      NewLeft:=Left;
      NewTop:=Top;
      case AAlign of
        d3LayerTop,d3LayerMostTop:
          Top:=Top + NewHeight;
        d3LayerBottom,d3LayerMostBottom:
          begin
            Bottom:=Bottom - NewHeight;
            NewTop:=Bottom;
          end;
        d3LayerLeft,d3LayerMostLeft:
          Left:=Left + NewWidth;
        d3LayerRight,d3LayerMostRight:
          begin
            Right:=Right - NewWidth;
            NewLeft:=Right;
          end;
        d3LayerCenter:
          begin
            NewLeft:=Left + ((NewWidth - (Control.Width)) / 2);
            NewWidth:=(Control.Width);
            NewTop:=Top + ((NewHeight - (Control.Height)) / 2);
            NewHeight:=(Control.Height);
          end;
        d3LayerContents:
          begin
            NewLeft:=0;
            NewWidth:=FullR.Right - FullR.Left;
            NewTop:=0;
            NewHeight:=FullR.Bottom - FullR.Top;
          end;
      end;
    end;

    if (Control.Width <> NewWidth) or (Control.Height <> NewHeight) then
    begin
      Control.Width:=NewWidth;
      Control.Height:=NewHeight;
    end;
    Control.Position.FX:=NewLeft + (NewWidth / 2);
    Control.Position.FY:=NewTop + (NewHeight / 2);
    Control.MatrixChanged(Self);
    if Control.FChildren <> nil then Realign;

    { Adjust client rect if control didn't resize as we expected }
    if (Control.Width <> NewWidth) or (Control.Height <> NewHeight) then
      with R do
        case AAlign of
          d3LayerTop,d3LayerMostTop:
            Top:=Top - (NewHeight - (Control.Height));
          d3LayerBottom,d3LayerMostBottom:
            Bottom:=Bottom + (NewHeight - (Control.Height));
          d3LayerLeft,d3LayerMostLeft:
            Left:=Left - (NewWidth - (Control.Width));
          d3LayerRight,d3LayerMostRight:
            Right:=Right + (NewWidth - (Control.Width));
          d3LayerClient:
            begin
              Right:=Right + NewWidth - (Control.Width);
              Bottom:=Bottom + NewHeight - (Control.Height);
            end;
        end;
  end;

  procedure DoAlign(AAlign:TD3LayerAlign);
  var
    I,J:integer;
    Control:TD3CustomLayer;
  begin
    AlignList.Clear;
    for I:=0 to ChildrenCount - 1 do
    begin
      if not (Children[i] is TD3CustomLayer) then Continue;
      Control:=TD3CustomLayer(Children[i]);
      if (Control.LayerAlign=AAlign) and ((AAlign=d3LayerNone) or (Control.Visible) or FScene.FDesignTime) and
         (Control.Projection=d3ProjectionScreen) then
      begin
        J:=0;
        while (J < AlignList.Count) and not InsertBefore(Control,TD3CustomLayer(AlignList[J]),AAlign) do Inc(J);
        AlignList.Insert(J,Control);
      end;
    end;
    for I:=0 to AlignList.Count - 1 do
    begin
      DoPosition(TD3CustomLayer(AlignList[I]),AAlign);
    end;
  end;

begin
  if FScene=nil then Exit;
  if ChildrenCount=0 then Exit;

  if Self is TD3CustomLayer then
    R:=d2Rect(-Width / 2,-Height / 2,Width / 2,Height / 2)
  else
    R:=d2Rect(0,0,FScene.Width,FScene.Height);
  FullR:=R;
  AlignList:=TList.Create;
  try
    DoAlign(d3LayerMostTop);
    DoAlign(d3LayerMostBottom);
    DoAlign(d3LayerMostLeft);
    DoAlign(d3LayerMostRight);
    DoAlign(d3LayerTop);
    DoAlign(d3LayerBottom);
    DoAlign(d3LayerLeft);
    DoAlign(d3LayerRight);
    DoAlign(d3LayerClient);
    DoAlign(d3LayerCenter);
    DoAlign(d3LayerContents);
  finally
    AlignList.Free;
  end;
end;

{ events }

//procedure TD3VisualObject.KeyDown(var Key:Word; var KeyChar:System.WideChar;	//Deleted by GoldenFox
procedure TD3VisualObject.KeyDown(var Key: word; var KeyChar: System.widechar;	//Added by GoldenFox
  Shift: TShiftState);
begin
  if Assigned(FOnKeyDown) then
    FOnKeyDown(Key,KeyChar,Shift);
end;

//procedure TD3VisualObject.KeyUp(var Key:Word; var KeyChar:System.WideChar;	//Deleted by GoldenFox
procedure TD3VisualObject.KeyUp(var Key: word; var KeyChar: System.widechar;	//Added by GoldenFox
  Shift: TShiftState);
begin
  if Assigned(FOnKeyUp) then
    FOnKeyUp(Key,KeyChar,Shift);
end;

procedure TD3VisualObject.Capture;
begin
  if (FScene <> nil) then
  begin
    FScene.FCaptured:=Self;
    FScene.MouseCapture:=true;
  end;
end;

procedure TD3VisualObject.ReleaseCapture;
begin
  if (FScene <> nil) then
  begin
    FScene.FCaptured:=nil;
    FScene.MouseCapture:=false;
  end;
end;

procedure TD3VisualObject.MouseEnter;
begin
  FIsMouseOver:=true;
  StartTriggerAnimation(Self,'IsMouseOver');
  Repaint;
end;

procedure TD3VisualObject.MouseLeave;
begin
  FIsMouseOver:=false;
  StartTriggerAnimation(Self,'IsMouseOver');
  Repaint;
end;

procedure TD3VisualObject.EnterFocus;
begin
  FIsFocused:=true;
  Repaint;
  StartTriggerAnimation(Self,'IsFocused');
end;

procedure TD3VisualObject.KillFocus;
begin
  FIsFocused:=false;
  Repaint;
  StartTriggerAnimation(Self,'IsFocused');
end;

procedure TD3VisualObject.SetFocus;
begin
  if not CanFocused then Exit;

  if not FIsFocused and (FScene <> nil) and (FScene.Focused <> Self) then
  begin
    FScene.Focused:=Self;
  end;
end;

procedure TD3VisualObject.GetTabOrderList(const AList:TList);
var
  i:integer;
begin
  if IsVisual and Visual.Visible then
    AList.Add(Self);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).IsVisual and (TD3VisualObject(FChildren[i]).Visible) then
      begin
        TD3VisualObject(FChildren[i]).GetTabOrderList(AList);
      end;
end;

procedure TD3VisualObject.ContextMenu(const ScreenPosition:TD2Point);
begin
  if FPopupMenu <> nil then
  begin
    FPopupMenu.PopupComponent:=Self;
    FPopupMenu.Popup(round(ScreenPosition.X),round(ScreenPosition.Y));
  end;
end;

procedure TD3VisualObject.Click;
begin
  if Assigned(FOnClick) then
    FOnClick(Self);
end;

procedure TD3VisualObject.DblClick;
begin
  if Assigned(FOnDblClick) then
    FOnDblClick(Self);
end;

procedure TD3VisualObject.MouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:single; rayPos,rayDir:TD3Vector);
var
  P:TPoint;
  VP:TD3Point;
begin
  if (FDragMode=d3DragAutomatic) and Assigned(FScene) then
  begin
    FScene.BeginVCLDrag(Self);
    Exit;
  end;
  if (ssDouble in Shift) and (Assigned(FScene) and (FScene.DesignTime)) then
    FScene.FDesignRoot:=Self;
  if (Assigned(FScene) and not (FScene.DesignTime)) and CanFocused and not FIsFocused and (FScene <> nil) and (FScene.Focused <> Self) then
    SetFocus;
  if Assigned(FOnMouseDown) then
    FOnMouseDown(Self,Button,Shift,X,Y,rayPos,rayDir);
  if (Button=mbRight) and FShowContextMenu then
  begin
    VP:=LocalToAbsolute(d3Point(X,Y,0));
    P:=Point(Trunc(VP.X),Trunc(VP.Y));
    P:=Scene.ClientToScreen(P);
    ContextMenu(d2Point(P.X,P.Y));
    Exit;
  end;
  if FAutoCapture then
    Capture;
  if (ssDouble in Shift) then
  begin
    DblClick;
    FDoubleClick:=true;
  end
  else
  if Button=mbLeft then
  begin
    FPressed:=true;
  end;
end;

procedure TD3VisualObject.MouseMove(Shift:TShiftState; X,Y,d3,Dy:single; rayPos,rayDir:TD3Vector);
begin
  if Assigned(FOnMouseMove) then
    FOnMouseMove(Self,Shift,X,Y,rayPos,rayDir);
end;

procedure TD3VisualObject.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single; rayPos,rayDir:TD3Vector);
begin
  if FAutoCapture then
    ReleaseCapture;
  if FPressed and not (FDoubleClick) and (FIsMouseOver) then
  begin
    FPressed:=false;
    Click;
  end;
  if Assigned(FOnMouseUp) then
    FOnMouseUp(Self,Button,Shift,X,Y,rayPos,rayDir);
end;

procedure TD3VisualObject.MouseWheel(Shift:TShiftState; WheelDelta:integer; var Handled:boolean);
begin
  if Assigned(FOnMouseWheel) then
    if FScene <> nil then
      FOnMouseWheel(Self,Shift,WheelDelta,FScene.FMousePos,Handled)
    else
      FOnMouseWheel(Self,Shift,WheelDelta,d3Point(0,0,0),Handled);
end;

procedure TD3VisualObject.DragEnter(const Data:TD3DragObject; const Point:TD3Point);
begin
  FIsDragOver:=true;
  Repaint;
  StartTriggerAnimation(Self,'IsDragOver');
  if Assigned(OnDragEnter) then
    OnDragEnter(Self,Data,Point);
end;

procedure TD3VisualObject.DragLeave;
begin
  FIsDragOver:=false;
  Repaint;
  StartTriggerAnimation(Self,'IsDragOver');
  if Assigned(OnDragLeave) then
    OnDragLeave(Self);
end;

procedure TD3VisualObject.DragEnd;
begin
  // Call mouse up - for effects - inside control
  if DragMode=d3DragAutomatic then
    MouseUp(mbLeft,[ssLeft],$FFFF,$FFFF,d3Vector($FFFF,$FFFF,0),d3Vector(1,0,0));
  if Assigned(OnDragEnd) then
    OnDragEnd(Self);
end;

function TD3VisualObject.DoHintShow(var Message:TLMessage):boolean;
begin
  Result:=false;
end;

//procedure TD3VisualObject.DragOver(const Data:TD3DragObject; const Point:TD3Point; var Accept:boolean);	//Deleted by GoldenFox
procedure TD3VisualObject.DragOver(const Data:TD3DragObject; Shift: TShiftState; const Point:TD3Point; var Accept:boolean);	//Added by GoldenFox
begin
  if Assigned(OnDragOver) then
    //OnDragOver(Self,Data,Point,Accept);	//Deleted by GoldenFox
	OnDragOver(Self,Data,Shift,Point,Accept);	//Added by GoldenFox
end;

//procedure TD3VisualObject.DragDrop(const Data:TD3DragObject; const Point:TD3Point);	//Deleted by GoldenFox
procedure TD3VisualObject.DragDrop(const Data: TD3DragObject; Shift: TShiftState; const Point: TD3Point);	//Added by GoldenFox
begin
  FIsDragOver:=false;
  Repaint;
  StartTriggerAnimation(Self,'IsDragOver');
  if Assigned(OnDragDrop) then
    OnDragDrop(Self,Data,Point);
end;

{ properties }

procedure TD3VisualObject.SetTempCanvas(const Value:TD3Canvas);
var
  i:integer;
begin
  FTempCanvas:=Value;
  if (FChildren <> nil) and (FChildren.Count > 0) then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual then
        TD3VisualObject(FChildren[i]).TempCanvas:=Value;
end;

procedure TD3VisualObject.SetHitTest(const Value:boolean);
begin
  FHitTest:=Value;
end;

procedure TD3VisualObject.SetClipChildren(const Value:boolean);
begin
  if FClipChildren <> Value then
  begin
    FClipChildren:=Value;
    Repaint;
  end;
end;

procedure TD3VisualObject.SetAlign(const Value:TD3Align);
begin
  if FAlign <> Value then
  begin
    FAlign:=Value;
    if (FParent <> nil) and (TD3Object(FParent).IsVisual) then
    begin
      TD3VisualObject(FParent).Realign;
    end;
  end;
end;

procedure TD3VisualObject.SetProjection(const Value:TD3Projection);
var
  i:integer;
begin
  if FProjection <> Value then
  begin
    FProjection:=Value;
    if FChildren <> nil then
      for i:=0 to FChildren.Count - 1 do
        if TD3Object(FChildren[i]).IsVisual then
          TD3VisualObject(FChildren[i]).Projection:=Value;
    Repaint;
  end;
end;

procedure TD3VisualObject.SetCollider(const Value:boolean);
begin
  if FCollider <> Value then
  begin
    FCollider:=Value;
    RecreateBody;
  end;
end;

procedure TD3VisualObject.SetDynamic(const Value:boolean);
begin
  if FDynamic <> Value then
  begin
    FDynamic:=Value;
    RecreateBody;
  end;
end;

procedure TD3VisualObject.SetVisible(const Value:boolean);
begin
  if FVisible <> Value then
  begin
    FVisible:=Value;
    if FVisible then
    begin
      if (Self is TD3CustomLayer) and (TD3CustomLayer(Self).LayerAlign <> d3LayerNone) and (Parent <> nil) and Parent.IsVisual and not (csLoading in ComponentState) then
        Parent.Visual.Realign;
      StartTriggerAnimation(Self,'IsVisible');
    end;
  end;
end;

procedure TD3VisualObject.SetHeight(const Value:single);
begin
  if FHeight <> Value then
  begin
    FHeight:=Value;
    if (FHeight < 0) and (Assigned(FScene) and (FScene.DesignTime)) then
    begin
      FHeight:=Abs(FHeight);
      FScale.Y:=-FScale.Y;
    end;
    if not (csLoading in ComponentState) then
    begin
      if (Parent <> nil) and (Parent.IsVisual) and (Align <> d3None) then
      begin
        TD3VisualObject(Parent).Realign;
      end;
      if FChildren <> nil then
        Realign;
      Repaint;
    end;
    if (FBody <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
      RecreateBody;
  end;
end;

procedure TD3VisualObject.SetWidth(const Value:single);
begin
  if FWidth <> Value then
  begin
    FWidth:=Value;
    if (FWidth < 0) and (Assigned(FScene) and (FScene.DesignTime)) then
    begin
      FWidth:=Abs(FWidth);
      FScale.X:=-FScale.X;
    end;
    if not (csLoading in ComponentState) then
    begin
      if (Parent <> nil) and (Parent.IsVisual) and (Align <> d3None) then
      begin
        TD3VisualObject(Parent).Realign;
      end;
      if FChildren <> nil then
        Realign;
      Repaint;
    end;
    if (FBody <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
      RecreateBody;
  end;
end;

procedure TD3VisualObject.SetDepth(const Value:single);
begin
  if FDepth <> Value then
  begin
    FDepth:=Value;
    if (FDepth < 0) and (Assigned(FScene) and (FScene.DesignTime)) then
    begin
      FDepth:=Abs(FDepth);
      FScale.Z:=-FScale.Z;
    end;
    if not (csLoading in ComponentState) then
    begin
      if (Parent <> nil) and (Parent.IsVisual) and (Align <> d3None) then
      begin
        TD3VisualObject(Parent).Realign;
      end;
      if FChildren <> nil then
        Realign;
      Repaint;
    end;
    if (FBody <> 0) and (FScene <> nil) and (FScene.Physics <> nil) then
      RecreateBody;
  end;
end;

function TD3VisualObject.isOpacityStored:boolean;
begin
  Result:=FOpacity <> 1;
  Result:=true;
end;

procedure TD3VisualObject.SetZWrite(const Value:boolean);
begin
  if FZWrite <> Value then
  begin
    FZWrite:=Value;
    Repaint;
  end;
end;

procedure TD3VisualObject.SetDesignHide(const Value:boolean);
begin
  if FDesignHide <> Value then
  begin
    FDesignHide:=Value;
    if (Assigned(FScene) and (FScene.DesignTime)) then
      Repaint;
  end;
end;

procedure TD3VisualObject.SetOpacity(const Value:single);
var
  i:integer;
begin
  if FOpacity <> Value then
  begin
    FOpacity:=Value;
    if FOpacity < 0 then FOpacity:=0;
    if FOpacity > 1 then FOpacity:=1;
    RecalcOpacity;
    Repaint;
  end;
end;

function TD3VisualObject.GetHint: string;  // 7777
begin
 Result:=UTF8Encode(FHint);
end;

procedure TD3VisualObject.SetHint(const Value: string);  // 7777
 var ss:WideString;
begin
 ss:=UTF8Decode(Value);
 if FHint <> ss then  FHint:=ss;
end;

       
           
