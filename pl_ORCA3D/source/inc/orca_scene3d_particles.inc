{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

procedure RndVector(var v:TD3Vector; var f :single;
  dispersionRange:TD3Position);
var
  fsq,f2:single;
  p:TD3Vector;
begin
  f2:=2 * f;
  if Assigned(dispersionRange) then
    p:=d3VectorScale(dispersionRange.Vector,f2)
  else
    p:=d3VectorScale(XYZWHmgVector,f2);
  if true {fast} then
  begin
    v.V[0]:=(Random-0.5) * p.V[0];
    v.V[1]:=(Random-0.5) * p.V[1];
    v.V[2]:=(Random-0.5) * p.V[2];
    v.W:=1;
  end
  else
  begin
    fsq:=Sqr(0.5);
    repeat
       v.V[0]:=(Random-0.5);
       v.V[1]:=(Random-0.5);
       v.V[2]:=(Random-0.5);
    until d3VectorNorm(v) <= fsq;
    v.V[0]:=v.V[0] * p.V[0];
    v.V[1]:=v.V[1] * p.V[1];
    v.V[2]:=v.V[2] * p.V[2];
    v.W:=1;
  end;
end;

//=================== TD3Particle ===================================================

constructor TD3Particle.Create;
begin
  inherited;
end;

destructor TD3Particle.Destroy;
begin
  inherited;
end;

//================== TD3ParticleList =================================================

constructor TD3ParticleList.Create;
begin
  inherited;
  FItemList:=TList.Create;
  FitemList.Capacity:=64;
  FDirectList:=nil;
end;

destructor TD3ParticleList.Destroy;
var
  i:integer;
begin
  for i:=0 to FItemList.Count - 1 do
    TObject(FItemList[i]).Free;
  FItemList.Free;
  inherited;
end;

function TD3ParticleList.GetItems(index:integer):TD3Particle;
begin
  Result:=TD3Particle(FItemList[index]);
end;

procedure TD3ParticleList.SetItems(index:integer; Value:TD3Particle);
begin
  FItemList[index]:=Value;
end;

function TD3ParticleList.ItemCount:integer;
begin
  Result:=FItemList.Count;
end;

procedure TD3ParticleList.RemoveAndFreeItem(AItem:TD3Particle);
var
  i:integer;
begin
  i:=FItemList.IndexOf(AItem);
  if i >= 0 then
  begin
    if AItem.Owner=Self then
      AItem.Owner:=nil;
    AItem.Free;
    FItemList.List[i]:=nil;
  end;
end;

procedure TD3ParticleList.Pack;
begin
  FItemList.Pack;
  FDirectList:=Pd3ParticleArray(FItemList.List);
end;

function TD3ParticleList.IndexOfItem(AItem:TD3Particle):integer;
begin
   Result:=FItemList.IndexOf(AItem);
end;

function TD3ParticleList.AddItem(AItem:TD3Particle):integer;
begin
  AItem.Owner:=Self;
  Result:=FItemList.Add(AItem);
  FDirectList:=Pd3ParticleArray(FItemList.List);
end;

procedure TD3ParticleList.AfterItemCreated(Sender:TObject);
begin
  TD3Particle(Sender).Owner:=Self;
end;

//============== TD3ParticleKey ===========================================

constructor TD3ParticleKey.Create(Collection:TCollection);
begin
  inherited;
end;

destructor TD3ParticleKey.Destroy;
begin
  inherited;
end;

function TD3ParticleKey.GetColor:AnsiString;
begin
  Result:=d3ColorToStr(FColor);
end;

procedure TD3ParticleKey.SetColor(const Value:AnsiString);
begin
  FColor:=d3StrToColor(Value);
end;

procedure TD3ParticleKey.SetKey(const Value:longword);
begin
  FKey:=Value;
end;

function TD3ParticleKey.Time:single;
begin
  Result:=(Key * cnKeyTime);
end;

//====================== TD3ParticleKeys =====================================

constructor TD3ParticleKeys.Create;
begin
  inherited Create(TD3ParticleKey);
end;

function TD3ParticleKeys.GetKey(Index:integer):TD3ParticleKey;
begin
  Result:=TD3ParticleKey(Items[Index]);
end;

//==================== TD3ParticleAnimation ====================================

type

  TD3ParticleAnimation=class(TD3Animation)
  private
    FEmitter:TD3ParticleEmitter;
  protected
  public
    procedure ProcessTick(time,deltaTime:single);  override;
  end;

procedure TD3ParticleAnimation.ProcessTick(time,deltaTime:single);
begin
  inherited ;
  if not FRunning then Exit;

  if FEmitter <> nil then
  begin
    FEmitter.ProcessEffect(deltaTime,Time);
  end;
end;

//==================== TTD3ParticleEmitter ======================================

constructor TD3ParticleEmitter.Create(AOwner:TComponent);
var
  k:TD3ParticleKey;
begin
  inherited Create(AOwner);
  FCanResize:=false;
  FCanRotate:=false;
  zWrite:=false;
  TwoSide:=true;
  Width:=0.3;
  Height:=0.3;
  Depth:=0.3;

  FKeys:=TD3ParticleKeys.Create;
  FKeys.FEmitter:=Self;
  k:=TD3ParticleKey.Create(FKeys);
  k.spin:=0;
  k.scale:=1;
  k.color:='#FFFFFFFF';
  k:=TD3ParticleKey.Create(FKeys);
  k.spin:=0;
  k.scale:=0.1;
  k.color:='#00000000';
  k.key:=10;
  FRect:=TD2Bounds.Create(d2Rect(0,0,1,1));
  FGravity:=TD3Position.Create(d3Point(WHmgVector));
  FPositionDispersion:=TD3Position.Create(d3Point(WHmgVector));
  FParticles:=TD3ParticleList.Create;
  FParticles.Owner:=Self;
  FParticleInterval:=0.05;
  FVelocityMode:=svmAbsolute;
  FDispersionMode:=sdmFast;
  FFriction:=1;
  FZTest:=false;
  FSpreadAngle:=360;
  FVelocityMin:=0.5;
  FVelocityMax:=0.8;
  FLifeTime:=2;

  FTimer:=TD3ParticleAnimation.Create(Self);
  TD3ParticleAnimation(FTimer).FEmitter:=Self;
  FTimer.Parent:=Self;
  FTimer.Stored:=false;
  FTimer.Duration:=Maxsingle;
  FTimer.Enabled:=true;
end;

destructor TD3ParticleEmitter.Destroy;
begin
  FTimer.Free;
  FKeys.Free;
  FRect.Free;
  FPositionDispersion.Free;
  FGravity.Free;
  FParticles.Free;
  inherited;
end;

procedure TD3ParticleEmitter.Assign(Source:TPersistent);
begin
  if Source is TD3ParticleEmitter then
  begin
    FBitmap:=TD3ParticleEmitter(Source).FBitmap;
    FBlendingMode:=TD3ParticleEmitter(Source).BlendingMode;
    FGravity.Assign(TD3ParticleEmitter(Source).Gravity);
    FPositionDispersion.Assign(TD3ParticleEmitter(Source).PositionDispersion);
    FParticleInterval:=TD3ParticleEmitter(Source).FParticleInterval;
    FVelocityMode:=TD3ParticleEmitter(Source).VelocityMode;
    FFollowToOwner:=TD3ParticleEmitter(Source).FollowToOwner;
    FDispersionMode:=TD3ParticleEmitter(Source).DispersionMode;
    FDirectionAngle:=TD3ParticleEmitter(Source).DirectionAngle;
    FSpreadAngle:=TD3ParticleEmitter(Source).SpreadAngle;
    FFriction:=TD3ParticleEmitter(Source).Friction;
    FRect.Assign(TD3ParticleEmitter(Source).Rect);
    FCentrifugalVelMin:=TD3ParticleEmitter(Source).CentrifugalVelMin;
    FCentrifugalVelMax:=TD3ParticleEmitter(Source).CentrifugalVelMax;
    FTangentVelMin:=TD3ParticleEmitter(Source).TangentVelMin;
    FTangentVelMax:=TD3ParticleEmitter(Source).TangentVelMax;
    FVelocityMin:=TD3ParticleEmitter(Source).VelocityMin;
    FVelocityMax:=TD3ParticleEmitter(Source).VelocityMax;
    
    LifeTime:=TD3ParticleEmitter(Source).LifeTime;

    ColorBegin:=TD3ParticleEmitter(Source).ColorBegin;
    ScaleBegin:=TD3ParticleEmitter(Source).ScaleBegin;
    SpinBegin:=TD3ParticleEmitter(Source).SpinBegin;

    ColorEnd:=TD3ParticleEmitter(Source).ColorEnd;
    ScaleEnd:=TD3ParticleEmitter(Source).ScaleEnd;
    SpinEnd:=TD3ParticleEmitter(Source).SpinEnd;
  end
  else
    inherited
end;

procedure TD3ParticleEmitter.ProcessEffect(DeltaTime,Time:single);
var
  n :integer;
  i :integer;
  curParticle :TD3Particle;
  dt:single;
  list:Pd3ParticleArray;
  doFriction,doPack :boolean;
  frictionScale :single;
  p1,p2,axis:TD3Vector;
  OwnerPositionDelta:TD3Vector;
  NeedRepaint:boolean;
  NeedFinish:boolean;
begin
  FCurrentTime:=FCurrentTime + deltaTime;
  if (FParticleInterval > 0) then
  begin
    if FTimeRemainder >= FParticleInterval then
    begin
      n:=Trunc(FTimeRemainder / FParticleInterval);
      Burst(FCurrentTime,n);
      FTimeRemainder:=0;
      NeedFinish:=true;
    end
    else
    begin
      FTimeRemainder:=FTimeRemainder + deltaTime;
      NeedFinish:=true;
    end;
  end;

  dt:=deltaTime;

  OwnerPositionDelta:=d3VectorSubtract(AbsolutePosition,FOldPosition);
  FOldPosition:=AbsolutePosition;


  NeedRepaint:=false;
  doPack:=false;
  list:=Particles.List;
  for i:=0 to Particles.ItemCount-1 do
  begin
    curParticle:=list[i];

    doFriction:=false;
    if curParticle.Emitter <> nil then
    begin
      NeedRepaint:=true;
      doFriction:=(curParticle.FEmitter.FFriction<>1);
      if doFriction then
      begin
        frictionScale:=Power(curParticle.FEmitter.FFriction,dt)
      end
      else
        frictionScale:=1;

      if curParticle.Emitter.FFollowToOwner then
      begin
        with curParticle do
        begin
          FPosition:=d3VectorAdd(FPosition,OwnerPositionDelta);
          if (not Visible) then
          begin
            curParticle.Free;
            list[i]:=nil;
            doPack:=True;
            Continue;
          end;
        end;
      end;
    end
    else
      frictionScale:=1;

    if curParticle.FCurrentTime < (LifeTime - dt) then
    begin

      with curParticle do
      begin

        FVelocity.V[0]:=FVelocity.V[0] - 0.001 + random * 0.002;
        FVelocity.V[1]:=FVelocity.V[1] - 0.001 + random * 0.002;
        FVelocity.V[2]:=FVelocity.V[2] - 0.001 + random * 0.002;

        if (FEmitter <> nil) then
        begin
          Combined3Vector(FVelocity,Gravity.Vector,dt);
        end;

        if (FCentrifugalVel <> 0) and
           ((FPosition.X <> FRotationCenter.X) or (FPosition.Y <> FRotationCenter.Y) or (FPosition.Z <> FRotationCenter.Z)) then
        begin
          Combined3Vector(FVelocity,d3VectorScale(d3VectorNormalize(d3VectorSubtract(FPosition,FRotationCenter)),FCentrifugalVel),dt);
        end;
        Combined3Vector(FPosition,FVelocity,dt);

        if (FTangentVel <> 0) and
           ((FPosition.X <> FRotationCenter.X) or (FPosition.Y <> FRotationCenter.Y) or (FPosition.Z <> FRotationCenter.Z)) then
        begin
          if (Projection=d3ProjectionCamera) and (Canvas <> nil) and (Canvas.CurrentCamera <> nil) then
            axis:=AbsoluteToLocalVector(Canvas.CurrentCamera.AbsoluteDirection);
          if (Projection=d3ProjectionScreen) then
            axis:=d3Vector(0,0,1);
          p1:=AbsoluteToLocalVector(FVelocity);
          RotateVector(p1,axis,DegToRad(FTangentVel));
          FVelocity:=LocalToAbsoluteVector(p1);
          p1:=AbsoluteToLocalVector(FPosition);
          RotateVector(p1,axis,DegToRad(FTangentVel));
          FPosition:=LocalToAbsoluteVector(p1);
        end;

        if doFriction then
          d3VectorScale(FVelocity,frictionScale);

      end;

      curParticle.FCurrentTime:=curParticle.FCurrentTime + dt;
    end
    else
    begin

      curParticle.Free;
      list[i]:=nil;
      doPack:=true;
      NeedRepaint:=true;
    end;
  end;
  if doPack then
  begin
    Particles.Pack;
    if Particles.ItemCount=0 then
      NeedFinish:=false;
    NeedRepaint:=true;
  end;

  if NeedRepaint then
    Repaint;
end;

procedure TD3ParticleEmitter.Paint;
var
  i:integer;
begin
  inherited;
  if Assigned(Scene) and (not Scene.DesignTime) and (FParticles.ItemCount > 0) then
  begin
    Canvas.Material.Lighting:=false;
    Canvas.Material.Diffuse:='#FFFFFFFF';
    Canvas.Material.BitmapMode:=d3TexModulate;
    Canvas.Material.Bitmap:=FBitmap;
    SetLength(MeshVertices,FParticles.ItemCount * 4);
    SetLength(MeshIndices,FParticles.ItemCount * 6);
    case BlendingMode of
      d3BlendAdditive:Canvas.SetRenderState(rsBlendAdditive);
      d3BlendBlend:Canvas.SetRenderState(rsBlendNormal);
    end;
    for i:=0 to FParticles.ItemCount - 1 do
      RenderParticle(FParticles.Items[i],i,MeshVertices,MeshIndices);

//    Canvas.SetRenderState(rsZTestOff);
    Canvas.FillColorTexVertexNormal(MeshVertices,MeshIndices,AbsoluteOpacity);
//    Canvas.SetRenderState(rsZTestOn);
    Canvas.SetRenderState(rsBlendNormal);
  end;
end;

procedure TD3ParticleEmitter.RenderParticle(const APart:TD3Particle; const Id3:integer; var Vertices:array of TD3ColorTexVertexNormal;
  var Indices:array of word);
var
  i:integer;
  x,y,z:single;
  SizeScale:single;
  RotateAngle:single;
  Color:TD3Color;
  V:TD3Vector;
  Right,Up:TD3Vector;
  rotMatrix:TD3Matrix;
  lifeTime:single;
  partPos,partDir:TD3Vector;
  curIndex,curVertex:integer;
  M:TD3Matrix;
begin
  if Canvas.CurrentCamera=nil then Exit;

  lifeTime:=APart.FCurrentTime;

  curVertex:=Id3 * 4;
  curIndex:=Id3 * 6;


  ComputeSizeScale(lifeTime,sizeScale);

  ComputeCurColor(lifeTime,Color);

  partPos:=d3Vector(APart.FPosition.X,APart.FPosition.Y,APart.FPosition.Z);
  partPos:=AbsoluteToLocalVector(partPos);
  x:=partPos.x;
  y:=partPos.y;
  z:=partPos.z;

  partDir:=AbsoluteToLocalVector(Canvas.CurrentCamera.AbsoluteDirection);
  Normalized3Vector(partDir);
  Up:=AbsoluteToLocalVector(Canvas.CurrentCamera.AbsoluteUp);
  Normalized3Vector(Up);
  Right:=AbsoluteToLocalVector(Canvas.CurrentCamera.AbsoluteRight);
  Normalized3Vector(Right);

  Right:=d3VectorScale(Right,sizeScale / 2);
  Up:=d3VectorScale(Up,sizeScale / 2);

  if ComputeSpinAngle(lifeTime,RotateAngle) then
  begin

    Vertices[curVertex + 0]:=ColorTexVertexNormal(x + (-Right.x - Up.x),y + (-Right.y - Up.y),z + (-Right.z - Up.z),0,0,0,Color,FRect.Left,FRect.Top);
    Vertices[curVertex + 1]:=ColorTexVertexNormal(x + ( Right.x - Up.x),y + ( Right.y - Up.y),z + ( Right.z - Up.z),0,0,0,Color,FRect.Right,FRect.Top);
    Vertices[curVertex + 2]:=ColorTexVertexNormal(x + ( Right.x + Up.x),y + ( Right.y + Up.y),z + ( Right.z + Up.z),0,0,0,Color,FRect.Right,FRect.Bottom);
    Vertices[curVertex + 3]:=ColorTexVertexNormal(x + (-Right.x + Up.x),y + (-Right.y + Up.y),z + (-Right.z + Up.z),0,0,0,Color,FRect.Left,FRect.Bottom);

    RotateAngle:=DegToRad(RotateAngle);
    rotMatrix:=d3CreateRotationMatrix(partDir,RotateAngle);
    for i:=0 to 3 do
    begin
      V:=d3Vector(Vertices[curVertex + i].X - x,Vertices[curVertex + i].Y - y,Vertices[curVertex + i].Z - z);
      V:=d3VectorTransform(V,rotMatrix);
      Vertices[curVertex + i].X:=V.X + x;
      Vertices[curVertex + i].Y:=V.Y + y;
      Vertices[curVertex + i].Z:=V.Z + z;
    end;
  end
  else
  begin

    Vertices[curVertex + 0]:=ColorTexVertexNormal(x + (-Right.x - Up.x),y + (-Right.y - Up.y),z + (-Right.z - Up.z),0,0,0,Color,FRect.Left,FRect.Top);
    Vertices[curVertex + 1]:=ColorTexVertexNormal(x + ( Right.x - Up.x),y + ( Right.y - Up.y),z + ( Right.z - Up.z),0,0,0,Color,FRect.Right,FRect.Top);
    Vertices[curVertex + 2]:=ColorTexVertexNormal(x + ( Right.x + Up.x),y + ( Right.y + Up.y),z + ( Right.z + Up.z),0,0,0,Color,FRect.Right,FRect.Bottom);
    Vertices[curVertex + 3]:=ColorTexVertexNormal(x + (-Right.x + Up.x),y + (-Right.y + Up.y),z + (-Right.z + Up.z),0,0,0,Color,FRect.Left,FRect.Bottom);
  end;

  Indices[curIndex + 0]:=curVertex + 0;
  Indices[curIndex + 1]:=curVertex + 1;
  Indices[curIndex + 2]:=curVertex + 2;
  Indices[curIndex + 3]:=curVertex + 0;
  Indices[curIndex + 4]:=curVertex + 2;
  Indices[curIndex + 5]:=curVertex + 3;
end;

function TD3ParticleEmitter.CreateParticle:TD3Particle;
begin
  Result:=TD3Particle.Create;
  Result.FCreationTime:=FCurrentTime;
  Result.FCurrentTime:=0;
  FParticles.AddItem(Result);
  if Assigned(FOnCreateParticle) then
    FOnCreateParticle(Self,Result);
end;

function TD3ParticleEmitter.FindKeys(const ATime:single; var Key1,
  Key2:TD3ParticleKey):boolean;
var
  i:integer;
begin
  Result:=false;
  if Keys.Count < 2 then Exit;

  if ATime > Keys[Keys.Count - 1].Time then
  begin
    Result:=true;
    Key1:=Keys[Keys.Count - 2];
    Key2:=Keys[Keys.Count - 1];
    Exit;
  end;

  for i:=0 to Keys.Count - 2 do
    if ((ATime >= Keys[i].Time) and (ATime <= Keys[i + 1].Time)) then
    begin
      Result:=true;
      Key1:=Keys[i];
      Key2:=Keys[i + 1];
      Exit;
    end;
end;

function TD3ParticleEmitter.ComputeSpinAngle(lifeTime:single; var spinAngle:single):boolean;
var
  k1,k2:TD3ParticleKey;
  f:single;
begin
  if FindKeys(lifeTime,k1,k2) then
  begin
    f:=(lifeTime - k1.Time) / (k2.Time - k1.Time);
    spinAngle:=d3Interpolatesingle(k1.spin,k2.spin,f);
  end
  else
    spinAngle:=0;
  Result:=spinAngle <> 0.0;
end;

procedure TD3ParticleEmitter.ComputeCurColor(lifeTime:single; var curColor:TD3Color);
var
  k1,k2:TD3ParticleKey;
  f:single;
begin
  if FindKeys(lifeTime,k1,k2) then
  begin
    f:=(lifeTime - k1.Time) / (k2.Time - k1.Time);
    curColor:=d3InterpolateColor(k1.FColor,k2.FColor,f);
  end
  else
    curColor:=0;
end;

function TD3ParticleEmitter.ComputeSizeScale(lifeTime:single; var sizeScale:single):boolean;
var
  k1,k2:TD3ParticleKey;
  f:single;
begin
  if FindKeys(lifeTime,k1,k2) then
  begin
    f:=(lifeTime - k1.Time) / (k2.Time - k1.Time);
    sizeScale:=d3Interpolatesingle(k1.scale,k2.scale,f);
  end
  else
    sizeScale:=0;
  Result:=sizeScale <> 1.0;
end;

procedure TD3ParticleEmitter.KillAll;
var
  i :integer;
  curParticle :TD3Particle;
  list :Pd3ParticleArray;
begin
  list:=Particles.List;
  for i:=0 to Particles.ItemCount-1 do
  begin
    curParticle:=list[i];
    curParticle.Free;
    list[i]:=nil;
  end;
  Particles.Pack;
end;

procedure TD3ParticleEmitter.SetPositionDispersion(const Value:TD3Position);
begin
  FPositionDispersion.Assign(Value);
end;

procedure TD3ParticleEmitter.SetRect(const Value:TD2Bounds);
begin
  FRect.Assign(Value);
end;

procedure TD3ParticleEmitter.SetZTest(const Value:boolean);
begin
  if FZTest <> Value then
  begin
    FZTest:=Value;
  end;
end;

procedure TD3ParticleEmitter.SetBlendingMode(const Value:TD3BlendingMode);
begin
  if FblendingMode <> Value then
  begin
    FblendingMode:=Value;
  end;
end;

procedure TD3ParticleEmitter.Burst(aTime:single; ParticlesCount:integer);
var
   particle :TD3Particle;
   av,pos:TD3Vector;
   axis:TD3Vector;
   f:single;
begin
  while ParticlesCount > 0 do
  begin
    particle:=CreateParticle;
    particle.Emitter:=Self;

    pos:=AbsolutePosition;
    f:=1;
    RndVector(av,f,FPositionDispersion);
    pos:=d3VectorAdd(pos,av);

    particle.FRotationCenter:=pos;
    particle.Position:=pos;

    if (Canvas <> nil) and (Canvas.CurrentCamera <> nil) then
    begin
      axis:=Canvas.CurrentCamera.AbsoluteDirection;
      av:=Canvas.CurrentCamera.AbsoluteRight;
      Normalized3Vector(av);
    end
    else
    begin
      axis:=d3Vector(0,1,0);
      av:=d3Vector(1,0,0);
    end;
    RotateVector(av,axis,DegToRad(FDirectionAngle - (FSpreadAngle / 2) + (FSpreadAngle * random)));

    av:=d3VectorScale(av,FVelocityMin + Random * (FVelocityMax - FVelocityMin));
    particle.FVelocity:=av;

    if VelocityMode=svmRelative then
    begin
      particle.FVelocity.W:=0;
      particle.FVelocity:=LocalToAbsoluteVector(particle.FVelocity);
      particle.FVelocity.W:=1;
    end;

    particle.FCentrifugalVel:=FCentrifugalVelMin + Random * (FCentrifugalVelMax - FCentrifugalVelMin);
    particle.FTangentVel:=FTangentVelMin + Random * (FTangentVelMax - FTangentVelMin);

    particle.FCreationTime:=FCurrentTime;
    particle.FCurrentTime:=0;
    System.Dec(ParticlesCount);
  end;
end;


procedure TD3ParticleEmitter.SetLifeTime(const Value:single);
begin
  FLifeTime:=Value;
  if FKeys.Count=2 then
  begin
    FKeys[1].key:=round(value / cnKeyTime);
    FLifeTime:=FKeys[1].key * cnKeyTime;
  end;
end;

procedure TD3ParticleEmitter.SetGravity(const Value:TD3Position);
begin
  FGravity.Assign(Value);
end;

procedure TD3ParticleEmitter.SetParticles(const Value:TD3ParticleList);
begin
  FParticles:=Value;
end;

function TD3ParticleEmitter.GetParticleInterval:single;
begin
  if FParticleInterval <> 0 then
    Result:=1 / FParticleInterval
  else
    Result:=1;
end;

procedure TD3ParticleEmitter.SetParticleInterval(const Value:single);
begin
  if Value <> 0 then
  begin
    FParticleInterval:=1 / Value;
    if FParticleInterval < 0 then FParticleInterval:=0;
    if FTimeRemainder > FParticleInterval then
      FTimeRemainder:=FParticleInterval;
  end;
end;

procedure TD3ParticleEmitter.SetCentrifugalVelMax(const Value:single);
begin
  FCentrifugalVelMax:=Value;
end;

procedure TD3ParticleEmitter.SetCentrifugalVelMin(const Value:single);
begin
  FCentrifugalVelMin:=Value;
end;

procedure TD3ParticleEmitter.SetTangentVelMax(const Value:single);
begin
  FTangentVelMax:=Value;
end;

procedure TD3ParticleEmitter.SetTangentVelMin(const Value:single);
begin
  FTangentVelMin:=Value;
end;

procedure TD3ParticleEmitter.SetVelocityMax(const Value:single);
begin
  FVelocityMax:=Value;
end;

procedure TD3ParticleEmitter.SetVelocityMin(const Value:single);
begin
  FVelocityMin:=Value;
end;

procedure TD3ParticleEmitter.SetBitmap(const Value:AnsiString);
begin
  if FBitmap <> Value then
  begin
    FBitmap:=Value;
    Repaint;
  end;
end;

function TD3ParticleEmitter.GetColorMax:AnsiString;
begin
  Result:=FKeys[1].color;
end;

function TD3ParticleEmitter.GetColorMin:AnsiString;
begin
  Result:=FKeys[0].color;
end;

function TD3ParticleEmitter.GetScaleMax:single;
begin
  Result:=FKeys[1].scale;
end;

function TD3ParticleEmitter.GetScaleMin:single;
begin
  Result:=FKeys[0].scale;
end;

function TD3ParticleEmitter.GetSpinMax:single;
begin
  Result:=FKeys[1].spin;
end;

function TD3ParticleEmitter.GetSpinMin:single;
begin
  Result:=FKeys[0].spin;
end;

procedure TD3ParticleEmitter.SetColorMax(const Value:AnsiString);
begin
  FKeys[1].color:=Value;
end;

procedure TD3ParticleEmitter.SetColorMin(const Value:AnsiString);
begin
  FKeys[0].color:=Value;
end;

procedure TD3ParticleEmitter.SetScaleMax(const Value:single);
begin
  FKeys[1].scale:=Value;
end;

procedure TD3ParticleEmitter.SetScaleMin(const Value:single);
begin
  FKeys[0].scale:=Value;
end;

procedure TD3ParticleEmitter.SetSpinMax(const Value:single);
begin
  FKeys[1].spin:=Value;
end;

procedure TD3ParticleEmitter.SetSpinMin(const Value:single);
begin
  FKeys[0].spin:=Value;
end;
