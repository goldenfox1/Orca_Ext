{%mainunit ../orca_scene3d.pas}
            
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************} 

//=== TD3AniThread ============================================

constructor TD3AniThread.Create;
begin
  inherited Create(nil);
  Interval:=round(1000 / 40);
  OnTimer:=DoSyncTimer;
  FAniList:=TList.Create;
end;

destructor TD3AniThread.Destroy;
begin
  FAniList.Free;
  inherited ;
end;

procedure TD3AniThread.DoSyncTimer(Sender:TObject);
begin
  OneStep;
end;

procedure TD3AniThread.OneStep;
var
  i:integer;
  NewTime:single;
begin
  if FTime=0 then
    FTime:=GetTickCount / 1000;
  NewTime:=GetTickCount / 1000;
  if NewTime <= FTime then Exit;
  FDeltaTime:=NewTime - FTime;
  FTime:=NewTime;
  if FAniList.Count > 0 then
  begin
    for i:=FAniList.Count - 1 downto 0 do
      if TD3Animation(FAniList[i]).FRunning then
        TD3Animation(FAniList[i]).ProcessTick(FTime,FDeltaTime);
  end;
end;

//==================== TD3Animation ============================================

constructor TD3Animation.Create(AOwner:TComponent);
begin
  inherited;
  FEnabled:=false;
  FDuration:=0.2;
  FRunning:=false;
end;

destructor TD3Animation.Destroy;
begin
  if VarD3AniThread <> nil then
    VarD3AniThread.FAniList.Remove(Self);
  inherited;
end;

procedure TD3Animation.Loaded;
begin
  inherited ;
  if not(Assigned(FScene) and (FScene.DesignTime)) and Enabled then
    Start;
end;

procedure TD3Animation.SetEnabled(const Value:boolean);
begin
  if FEnabled <> Value then
  begin
    FEnabled:=Value;
    if not (Assigned(Scene) and Scene.DesignTime) and not (csLoading in ComponentState) then
    begin
      if FEnabled then
        Start
      else
        Stop;
    end;
  end;
end;

function TD3Animation.NormalizedTime:single;
begin
  if FDuration > 0 then
  begin
    case FInterpolation of
      d2InterpolationLinear:Result:= d2InterpolateLinear(FTime,0,1,FDuration);
      d2InterpolationQuadratic:Result:= d2InterpolateQuad(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationCubic:Result:= d2InterpolateCubic(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationQuartic:Result:= d2InterpolateQuart(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationQuintic:Result:= d2InterpolateQuint(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationSinusoidal:Result:= d2InterpolateSine(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationExponential:Result:= d2InterpolateExpo(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationCircular:Result:= d2InterpolateCirc(FTime,0,1,FDuration,FAnimationType);
      d2InterpolationElastic:Result:= d2InterpolateElastic(FTime,0,1,FDuration,0,0,FAnimationType);
      d2InterpolationBack:Result:= d2InterpolateBack(FTime,0,1,FDuration,0,FAnimationType);
      d2InterpolationBounce:Result:= d2InterpolateBounce(FTime,0,1,FDuration,FAnimationType);
    end;
  end
  else
    Result:=0;
end;

procedure TD3Animation.ProcessAnimation;
begin
end;

procedure TD3Animation.ProcessTick(time,deltaTime:single);
begin
  inherited;
  if (Parent <> nil) and (Parent.IsVisual) and (not TD3VisualObject(Parent).Visible) then
    Stop;

  if not FRunning then Exit;

  if FPause then Exit;

  if FDelayTime > 0 then
  begin
    FDelayTime:=FDelayTime - deltaTime;
    Exit;
  end;

  if FInverse then
    FTime:=FTime - deltaTime
  else
    FTime:=FTime + deltaTime;
  if FTime >= FDuration then
  begin
    FTime:=FDuration;
    if FLoop then
    begin
      if FAutoReverse then
      begin
        FInverse:=true;
        FTime:=FDuration;
      end
      else
        FTime:=0;
    end
    else
    begin
      FRunning:=false;
    end;
  end
  else
  if FTime <= 0 then
  begin
    FTime:=0;
    if FLoop then
    begin
      if FAutoReverse then
      begin
        FInverse:=false;
        FTime:=0;
      end
      else
        FTime:=FDuration;
    end
    else
    begin
      FRunning:=false;
    end;
  end;

  ProcessAnimation;
  if (FScene <> nil) and (not FScene.RealTime) then
    if not FRunning then
    begin
      if VarD3AniThread <> nil then
        VarD3AniThread.FAniList.Remove(Self);
      if Assigned(FOnFinish) then FOnFinish(Self);
    end;
  if (FScene <> nil) and (FScene.RealTime) then
    if not FRunning then
    begin
      if Assigned(FOnFinish) then FOnFinish(Self);
    end;
end;

procedure TD3Animation.Start;
begin
  if (Parent <> nil) and (Parent.IsVisual) and (not TD3VisualObject(Parent).Visible) then Exit;
  if (Abs(FDuration) < 0.001) or (Assigned(FScene) and (FScene.DesignTime)) then
  begin
    { imediatly animation }
    FDelayTime:=0;
    if FInverse then
    begin
      FTime:=0;
      FDuration:=1;
    end
    else
    begin
      FTime:=1;
      FDuration:=1;
    end;
    FRunning:=true;
    ProcessAnimation;
    FRunning:=false;
    FTime:=0;
    FDuration:=0;
    FEnabled:=false;
    if Assigned(FOnFinish) then FOnFinish(Self);
  end
  else
  begin
    FDelayTime:=FDelay;
    FRunning:=true;
    if FInverse then
      FTime:=FDuration
    else
      FTime:=0;
    ProcessAnimation;

    if not ((FScene <> nil) and (FScene.RealTime)) then
    begin
      if VarD3AniThread=nil then
        VarD3AniThread:=TD3AniThread.Create;
      VarD3AniThread.FAniList.Add(Self);
    end;
    FEnabled:=true;
  end;
end;

procedure TD3Animation.Stop;
begin
  if not FRunning then Exit;

  if VarD3AniThread <> nil then
    VarD3AniThread.FAniList.Remove(Self);

  if FInverse then
    FTime:=0
  else
    FTime:=FDuration;
  ProcessAnimation;
  FRunning:=false;
  FEnabled:=false;
  if Assigned(FOnFinish) then FOnFinish(Self);
end;

procedure TD3Animation.StartTrigger(AInstance:TD3Object; ATrigger:string);
var
  StartValue:boolean;
  Line,Setter,Prop,Value:ansistring;
begin
  if FTrigger='' then Exit;
  if AInstance=nil then Exit;
  if Pos(LowerCase(ATrigger),LowerCase(FTrigger))=0 then Exit;

  Line:=FTrigger;
  Setter:=GetToken(Line,';');
  StartValue:=false;
  while Setter <> '' do
  begin
    Prop:=GetToken(Setter,'=');
    Value:=Setter;
    if GetPropInfo(AInstance,Prop,[tkBool]) <> nil then
    begin

      StartValue:=false;
      if (CompareText(Value,'true')=0) and (GetOrdProp(AInstance,Prop) > 0) then
        StartValue:=true;
      if (CompareText(Value,'false')=0) and (GetOrdProp(AInstance,Prop)=0) then
        StartValue:=true;

      if not StartValue then Exit;
    end;
    Setter:=GetToken(Line,';');
  end;
  if StartValue then
    Start;
end;
       
//============== TD3ColorAnimation =======================================

constructor TD3ColorAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartColor:=$FFFFFFFF;
  FStartColor:=$FFFFFFFF;
end;

destructor TD3ColorAnimation.Destroy;
begin
  inherited;
end;

procedure TD3ColorAnimation.Start;
var
  Persistent:string;
begin
  if (Parent <> nil) and (FPropertyName <> '') then
  begin
    if FInstance=nil then
    begin
      FInstance:=Parent;
      FPath:=FPropertyName;
      while Pos('.',FPath) > 0 do
      begin
        Persistent:=GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
    if (FInstance <> nil) and StartFromCurrent then
    begin
      { is string prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkString,tkLString,
        tkWString,tkAString]) <> nil then
        StartValue:=GetStrProp(FInstance,FPath);
      { is int prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkInteger]) <> nil then
        StartValue:=d3ColorToStr(GetOrdProp(FInstance,FPath));
    end;
  end;
  inherited;
end;

procedure TD3ColorAnimation.ProcessAnimation;
begin
  if FInstance <> nil then
  begin

    if GetPropInfo(FInstance.ClassInfo,FPath,[tkString,tkLString,
      tkWString,tkAString]) <> nil then
      SetStrProp(FInstance,FPath,
        d3ColorToStr(d3InterpolateColor(FStartColor,FStopColor,NormalizedTime)));

    if GetPropInfo(FInstance.ClassInfo,FPath,[tkInteger]) <> nil then
      SetOrdProp(FInstance,FPath,d3InterpolateColor(FStartColor,
        FStopColor,NormalizedTime));
  end;
end;

function TD3ColorAnimation.GetStartColor:string;
begin
  Result:=d3ColorToStr(FStartColor);
end;

function TD3ColorAnimation.GetStopColor:string;
begin
  Result:=d3ColorToStr(FStopColor);
end;

procedure TD3ColorAnimation.SetStartColor(const Value:string);
begin
  FStartColor:=d3StrToColor(Value);
end;

procedure TD3ColorAnimation.SetStopColor(const Value:string);
begin
  FStopColor:=d3StrToColor(Value);
end;

//========== TD3FloatAnimation ====================================

constructor TD3FloatAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartFloat:=0;
  FStopFloat:=0;
end;

destructor TD3FloatAnimation.Destroy;
begin
  PropertyName:='';
  inherited;
end;

procedure TD3FloatAnimation.Start;
var
  Persistent:string;
begin
  if (Parent <> nil) and (FPropertyName <> '') then
  begin
    if FInstance=nil then
    begin
      FInstance:=Parent;
      FPath:=FPropertyName;
      while Pos('.',FPath) > 0 do
      begin
        Persistent:=GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
    if (FInstance <> nil) and StartFromCurrent then
    begin
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkFloat]) <> nil then
      begin
        StartValue:=GetFloatProp(FInstance,FPath);
      end;
    end;
  end;
  inherited;
end;

procedure TD3FloatAnimation.ProcessAnimation;
begin
  if FInstance <> nil then
  begin
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkFloat]) <> nil then
    begin
      SetFloatProp(FInstance,FPath,d3InterpolateSingle(FStartFloat,
        FStopFloat,NormalizedTime));
    end;
  end;
end;
   
           
