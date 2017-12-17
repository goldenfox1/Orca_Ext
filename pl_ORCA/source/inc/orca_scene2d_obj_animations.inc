{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2AniThread ===================================

constructor TD2AniThread.Create;
begin
  inherited Create(nil);
  Interval:=Trunc(1000 / 30);
  OnTimer:=DoSyncTimer;

  FAniList:=TList.Create;
  FStartTime:=D2GetTickCount / 1000;
end;

destructor TD2AniThread.Destroy;
begin
  FAniList.Free;
  inherited ;
end;

procedure TD2AniThread.DoSyncTimer(Sender:TObject);
{begin
  OneStep;
end;

procedure TD2AniThread.OneStep;}
var
  i:integer;
  NewTime:single;
begin
  NewTime:=(D2GetTickCount / 1000) - FStartTime;
  if NewTime <= FTime then Exit;
  FDeltaTime:=NewTime - FTime;
  FTime:=NewTime;
  //if FAniList.Count > 0 then
  //begin
  //  i:=FAniList.Count - 1;
  //  while i >= 0 do
  i := 0;
  while i < FAniList.Count do
  begin
    if TD2Animation(FAniList[i]).FRunning then
    begin
      if (TD2Animation(FAniList[i]).BindingName <> '') and
         (CompareText(TD2Animation(FAniList[i]).BindingName,'caret')=0)
        then  begin
                TD2Animation(FAniList[i]).Tag:=TD2Animation(FAniList[i]).Tag + 1;
                if TD2Animation(FAniList[i]).Tag mod 3=0 then
                begin
                  TD2Animation(FAniList[i]).ProcessTick(FTime,FDeltaTime);
                end;
              end
        else TD2Animation(FAniList[i]).ProcessTick(FTime,FDeltaTime);
    end;
    //Dec(i);
    //if i >= FAniList.Count then
    //  i:=FAniList.Count - 1;
    Inc(i);
  end;
  //end;
end;

//================== TD2Animation ==================================

constructor TD2Animation.Create(AOwner:TComponent);
begin
  inherited;
  FEnabled:=false;
  Duration:=0.2;
end;

destructor TD2Animation.Destroy;
begin
  if GvarD2aniThread <> nil then
  begin
    TD2AniThread(GvarD2aniThread).FAniList.Remove(Self);
  end;
  inherited;
end;

procedure TD2Animation.Loaded;
begin
  inherited ;
  if not(Assigned(FScene) and (FScene.GetDesignTime)) and Enabled then
    Start;
end;

procedure TD2Animation.DoFinish;                                   // *** DB ***
begin                                                              // *** DB ***
  if ( Parent <> nil   ) and                                       // *** DB ***
     ( Parent.IsVisual ) and                                       // *** DB ***
     ( FHideOnFinish   )     then                                  // *** DB ***
     TD2VisualObject(Parent).Visible:=False;                     // *** DB ***
  Finish;                                                          // *** DB ***
end;                                                               // *** DB ***

procedure TD2Animation.Finish;
begin                                                              // *** DB ***
   if Assigned(FOnFinish) then FOnFinish(Self);                    // *** DB ***
end;                                                                 // *** DB ***

procedure TD2Animation.SetEnabled(const Value:boolean);
begin
  if FEnabled <> Value then
  begin
    FEnabled:=Value;
    if not (Assigned(Scene) and Scene.GetDesignTime) and not (csLoading in ComponentState) then
    begin
      if FEnabled then
        Start
      else
        Stop;
    end;
  end;
end;

function TD2Animation.NormalizedTime:single;
begin
  if (FDuration > 0) and (FDelayTime <= 0) then
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

procedure TD2Animation.ProcessAnimation;
begin
end;

procedure TD2Animation.ProcessTick(time,deltaTime:single);
begin
  inherited;
  if Assigned(FScene) and (FScene.GetDesignTime) then Exit;
  if csDestroying in ComponentState then Exit;

  if (Parent <> nil) and (Parent.IsVisual) and (not TD2VisualObject(Parent).Visible) then
    Stop;

  if not FRunning then Exit;
  if FPause then Exit;

  if (FDelay > 0) and (FDelayTime <> 0) then
  begin
    if FDelayTime > 0 then
    begin
      FDelayTime:=FDelayTime - deltaTime;
      if FDelayTime <= 0 then
      begin
        Start;
        FDelayTime:=0;
      end;
    end;
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
      FRunning:=false;
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
      FRunning:=false;
  end;

  ProcessAnimation;
  if Assigned(FOnProcess) then FOnProcess(Self);

  if (FScene <> nil) then
    if not FRunning then
    begin
      if GvarD2aniThread <> nil then
        TD2AniThread(GvarD2aniThread).FAniList.Remove(Self);
      DoFinish;                                                    // *** DB ***
//    if Assigned(FOnFinish) then FOnFinish(Self);                 // *** DB ***
    end;
end;

procedure TD2Animation.Start;
begin
  if (Parent <> nil) and (Parent.IsVisual) and (not TD2VisualObject(Parent).Visible) then Exit;
  if (Abs(FDuration) < 0.001) or (FScene=nil) or (Assigned(FScene) and (FScene.GetDesignTime)) then
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
    FDuration:=0.00001;
    DoFinish;                                                      // *** DB ***
//  if Assigned(FOnFinish) then FOnFinish(Self);                   // *** DB ***
    FEnabled:=false;
  end
  else
  begin
    FDelayTime:=FDelay;
    FRunning:=true;
    if FInverse then
      FTime:=FDuration
    else
      FTime:=0;
    if FDelay=0 then
      ProcessAnimation;

    if (FScene <> nil) then
    begin
      if GvarD2aniThread=nil then
        GvarD2aniThread:=TD2AniThread.Create;

      if TD2AniThread(GvarD2aniThread).FAniList.IndexOf(Self) < 0 then
        TD2AniThread(GvarD2aniThread).FAniList.Add(Self);
    end;
    FEnabled:=true;
  end;
end;

procedure TD2Animation.Stop;
begin
  if not FRunning then Exit;

  if GvarD2aniThread <> nil then
  begin
    TD2AniThread(GvarD2aniThread).FAniList.Remove(Self);
  end;

  if FInverse then
    FTime:=0
  else
    FTime:=FDuration;
  ProcessAnimation;
  FRunning:=false;
  FEnabled:=false;
  DoFinish;                                                        // *** DB ***
//  if Assigned(FOnFinish) then FOnFinish(Self);                   // *** DB ***
end;

procedure TD2Animation.StopAtCurrent;
begin
  if not FRunning then Exit;

  if GvarD2aniThread <> nil then
  begin
    TD2AniThread(GvarD2aniThread).FAniList.Remove(Self);
  end;

  if FInverse then
    FTime:=0
  else
    FTime:=FDuration;
  FRunning:=false;
  FEnabled:=false;
  if Assigned(FOnFinish) then FOnFinish(Self);
end;

procedure TD2Animation.StartTrigger(AInstance:TD2Object; ATrigger:string);
var
  StartValue:boolean;
  Line,Setter,Prop,Value:AnsiString;
begin
  if AInstance=nil then Exit;
  if (FTriggerInverse <> '') and (Pos(LowerCase(ATrigger),LowerCase(FTriggerInverse)) > 0) then
  begin
    Line:=FTriggerInverse;
    Setter:=d2GetToken(Line,';');
    StartValue:=false;
    while Setter <> '' do
    begin
      Prop:=d2GetToken(Setter,'=');
      Value:=Setter;
      if GetPropInfo(AInstance,Prop,[tkBool]) <> nil then
      begin
        StartValue:=false;
        if (CompareText(Value,'true')=0) and (GetOrdProp(AInstance,Prop) > 0) then
          StartValue:=true;
        if (CompareText(Value,'false')=0) and (GetOrdProp(AInstance,Prop)=0) then
          StartValue:=true;

        if not StartValue then Break;
      end;
      Setter:=d2GetToken(Line,';');
    end;
    if StartValue then
    begin
      Inverse:=true;
      Start;
      Exit;
    end;
  end;
  if (FTrigger <> '') and (Pos(LowerCase(ATrigger),LowerCase(FTrigger)) > 0) then
  begin
    Line:=FTrigger;
    Setter:=d2GetToken(Line,';');
    StartValue:=false;
    while Setter <> '' do
    begin
      Prop:=d2GetToken(Setter,'=');
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
      Setter:=d2GetToken(Line,';');
    end;
    if StartValue then
    begin
      if FTriggerInverse <> '' then
        Inverse:=false;
      Start;
    end;
  end;
end;

         
//================== TD2ColorAnimation =======================================

constructor TD2ColorAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartColor:=$FFFFFFFF;
  FStartColor:=$FFFFFFFF;
end;

destructor TD2ColorAnimation.Destroy;
begin
  inherited;
end;

procedure TD2ColorAnimation.Start;
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
        Persistent:=d2GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
  end;
  if (FInstance <> nil) and StartFromCurrent then
  begin
    { is string prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkString,tkLString,tkWString,tkAString{$IFDEF KS_COMPILER11_UP},tkUString{$ENDIF}]) <> nil then
      StartValue:=GetStrProp(FInstance,FPath);
    { is int prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkInteger]) <> nil then
      StartValue:=d2ColorToStr(GetOrdProp(FInstance,FPath));
  end;
  inherited;
end;

procedure TD2ColorAnimation.ProcessAnimation;
begin
  if FInstance <> nil then
  begin
    { is string prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkString,tkLString,tkWString,tkAString{$IFDEF KS_COMPILER11_UP},tkUString{$ENDIF}]) <> nil then
      SetStrProp(FInstance,FPath,d2ColorToStr(d2InterpolateColor(FStartColor,FStopColor,NormalizedTime)));
    { is int prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkInteger]) <> nil then
      SetOrdProp(FInstance,FPath,d2InterpolateColor(FStartColor,FStopColor,NormalizedTime));
  end;
end;

function TD2ColorAnimation.GetStartColor:string;
begin
  Result:=d2ColorToStr(FStartColor);
end;

function TD2ColorAnimation.GetStopColor:string;
begin
  Result:=d2ColorToStr(FStopColor);
end;

procedure TD2ColorAnimation.SetStartColor(const Value:string);
begin
  FStartColor:=d2StrToColor(Value);
end;

procedure TD2ColorAnimation.SetStopColor(const Value:string);
begin
  FStopColor:=d2StrToColor(Value);
end;

{ TD2GradientAnimation ===================================================================}

constructor TD2GradientAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartGradient:=TD2Gradient.Create;
  FStopGradient:=TD2Gradient.Create;
end;

destructor TD2GradientAnimation.Destroy;
begin
  FStartGradient.Free;
  FStopGradient.Free;
  inherited;
end;

procedure TD2GradientAnimation.Start;
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
        Persistent:=d2GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
  end;
  if (FInstance <> nil) and StartFromCurrent then
  begin
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkClass]) <> nil then
    begin
      StartValue:=TD2Gradient(GetObjectProp(FInstance,FPath,TD2Gradient));
    end;
  end;
  inherited;
end;

procedure TD2GradientAnimation.ProcessAnimation;
var
  i:integer;
  //G:TD2Gradient;
begin
  if FInstance <> nil then
  begin
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkClass]) <> nil then
    begin
      with TD2Gradient(GetObjectProp(FInstance,FPath,TD2Gradient)) do
      begin
        for i:=0 to Points.Count - 1 do
        begin
          if (i < FStopGradient.Points.Count) or (i < FStartGradient.Points.Count) then
            Points[i].Color:=d2ColorToStr(d2InterpolateColor(d2StrToColor(FStartGradient.Points[i].Color),
              d2StrToColor(FStopGradient.Points[i].Color),NormalizedTime));
        end;
        Change;
      end;
    end;
  end;
end;

procedure TD2GradientAnimation.SetStartGradient(const Value:TD2Gradient);
begin
  FStartGradient.Assign(Value);
end;

procedure TD2GradientAnimation.SetStopGradient(const Value:TD2Gradient);
begin
  FStopGradient.Assign(Value);
end;

{ TD2FloatAnimation ===================================================================}

constructor TD2FloatAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartFloat:=0;
  FStopFloat:=0;
end;

destructor TD2FloatAnimation.Destroy;
begin
  inherited;
end;

procedure TD2FloatAnimation.Start;
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
        Persistent:=d2GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
  end;
  if (FInstance <> nil) and StartFromCurrent then
  begin
    { is float prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkFloat]) <> nil then
    begin
      StartValue:=GetFloatProp(FInstance,FPath);
    end;
  end;
  inherited;
end;

procedure TD2FloatAnimation.Stop;
begin
  inherited;
  FInstance:=nil;
end;

procedure TD2FloatAnimation.ProcessAnimation;
begin
  if FInstance <> nil then
  begin
    { is float prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkFloat]) <> nil then
    begin
      SetFloatProp(FInstance,FPath,d2InterpolateSingle(FStartFloat,FStopFloat,NormalizedTime));
    end;
  end;
end;

{ TD2RectAnimation ===================================================================}

constructor TD2RectAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartRect:=TD2Bounds.Create(d2Rect(0,0,0,0));
  FStopRect:=TD2Bounds.Create(d2Rect(0,0,0,0));
  FCurrent:=TD2Bounds.Create(d2Rect(0,0,0,0));
end;

destructor TD2RectAnimation.Destroy;
begin
  FCurrent.Free;
  FStartRect.Free;
  FStopRect.Free;
  inherited;
end;

procedure TD2RectAnimation.Start;
var
  Persistent:string;
  Value:TObject;
begin
  if (Parent <> nil) and (FPropertyName <> '') then
  begin
    if FInstance=nil then
    begin
      FInstance:=Parent;
      FPath:=FPropertyName;
      while Pos('.',FPath) > 0 do
      begin
        Persistent:=d2GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
  end;
  if (FInstance <> nil) and StartFromCurrent then
  begin
    { is Rect prop }
    if GetPropInfo(FInstance.ClassInfo,FPropertyName,[tkClass]) <> nil then
    begin
      Value:=GetObjectProp(FInstance,FPropertyName);
      if (Value <> nil) and (Value is TPersistent) then
        FStartRect.Assign(TPersistent(Value));
    end;
  end;
  inherited;
end;

procedure TD2RectAnimation.ProcessAnimation;
var
  Value:TObject;
begin
  if FInstance <> nil then
  begin
    { calc value }
    FCurrent.Left:=d2InterpolateSingle(FStartRect.Left,FStopRect.Left,NormalizedTime);
    FCurrent.Top:=d2InterpolateSingle(FStartRect.Top,FStopRect.Top,NormalizedTime);
    FCurrent.Right:=d2InterpolateSingle(FStartRect.Right,FStopRect.Right,NormalizedTime);
    FCurrent.Bottom:=d2InterpolateSingle(FStartRect.Bottom,FStopRect.Bottom,NormalizedTime);

    { is Rect prop }
    if GetPropInfo(FInstance.ClassInfo,FPath,[tkClass]) <> nil then
    begin
      Value:=GetObjectProp(FInstance,FPath);
      if (Value <> nil) and (Value is TPersistent) then
        TPersistent(Value).Assign(FCurrent);
    end;
  end;
end;

{ TD2BitmapAnimation ===================================================================}

constructor TD2BitmapAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FStartBitmap:=TD2Bitmap.Create(1,1);
  FStopBitmap:=TD2Bitmap.Create(1,1);
  FCurrent:=TD2Bitmap.Create(1,1);
end;

destructor TD2BitmapAnimation.Destroy;
begin
  FCurrent.Free;
  FStartBitmap.Free;
  FStopBitmap.Free;
  inherited;
end;

procedure TD2BitmapAnimation.ProcessAnimation;
var
  Persistent{,Path}:string;
  Value:TObject;
begin
  if (Parent <> nil) and (FPropertyName <> '')  then
  begin
    if FInstance=nil then
    begin
      FInstance:=Parent;
      //Path:=FPropertyName;
      while Pos('.',FPropertyName) > 0 do
      begin
        Persistent:=d2GetToken(FPropertyName,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;

    if FInstance <> nil then
    begin
      { is Bitmap prop }
      if GetPropInfo(FInstance.ClassInfo,FPropertyName,[tkClass]) <> nil then
      begin
        { calc new value }
        Value:=GetObjectProp(FInstance,FPropertyName);
        if (Value <> nil) and (Value is TPersistent) then
        begin
          if Inverse then
          begin
            { assign to start }
            FCurrent.Assign(FStopBitmap);
            { draw final with alpha }
            if FCurrent.Canvas.BeginScene then
            begin
              FCurrent.Canvas.Clear(0);
              FCurrent.Canvas.DrawBitmap(FStartBitmap,d2Rect(0,0,FCurrent.Width,FCurrent.Height),
                d2Rect(0,0,FStartBitmap.Width,FStartBitmap.Height),1 - NormalizedTime);
              FCurrent.Canvas.EndScene;
            end;
          end
          else
          begin
            { assign to start }
            FCurrent.Assign(FStartBitmap);
            { draw final with alpha }
            if FCurrent.Canvas.BeginScene then
            begin
              FCurrent.Canvas.Clear(0);
              FCurrent.Canvas.DrawBitmap(FStopBitmap,d2Rect(0,0,FCurrent.Width,FCurrent.Height),
                d2Rect(0,0,FStopBitmap.Width,FStopBitmap.Height),NormalizedTime);
              FCurrent.Canvas.EndScene;
            end;
          end;
          { assign }
          TPersistent(Value).Assign(FCurrent);
        end;
      end;
    end;
  end;
end;

{ TD2BitmapListAnimation ===================================================================}

constructor TD2BitmapListAnimation.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.2;
  FCurrent:=TD2Bitmap.Create(0,0);
  FAnimationBitmap:=TD2Bitmap.Create(0,0);
  FAnimationCount:=1;
  FLastAnimationStep:=0;
end;

destructor TD2BitmapListAnimation.Destroy;
begin
  FCurrent.Free;
  FAnimationBitmap.Free;
  inherited;
end;

procedure TD2BitmapListAnimation.ProcessAnimation;
var
  Persistent{,Path}:string;
  Value:TObject;
  LeftPos,CurrentIndex:integer;
  NowValue:single;
begin
  if (Parent <> nil) and (FPropertyName <> '')  then
  begin
    if FInstance=nil then
    begin
      FInstance:=Parent;
      //Path:=FPropertyName;
      while Pos('.',FPropertyName) > 0 do
      begin
        Persistent:=d2GetToken(FPropertyName,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;

    if FInstance <> nil then
    begin
      { is Bitmap prop }
      if GetPropInfo(FInstance.ClassInfo,FPropertyName,[tkClass]) <> nil then
      begin
        { calc new value }
        Value:=GetObjectProp(FInstance,FPropertyName);
        if (Value <> nil) and (Value is TD2Bitmap) then
        begin
          NowValue:=d2InterpolateSingle(0,FAnimationCount - 1,NormalizedTime);

          FCurrent.SetSize(TD2Bitmap(Value).Height,TD2Bitmap(Value).Width);

          CurrentIndex:=Trunc(NowValue);

          LeftPos:=CurrentIndex * (FAnimationBitmap.Width div FAnimationCount) + CurrentIndex;

          if FCurrent.Canvas.BeginScene then
          begin
            FCurrent.Canvas.Clear(0);
            FCurrent.Canvas.DrawBitmap(FAnimationBitmap,
              d2Rect(LeftPos,0,LeftPos + FCurrent.Width,FCurrent.Height),
              d2Rect(0,0,FAnimationBitmap.Width div FAnimationCount,FAnimationBitmap.Height),1);
            FCurrent.Canvas.EndScene;
          end;

          TPersistent(Value).Assign(FCurrent);
        end;
      end;
    end;
  end;
end;

{ Key Animation ===============================================================}

{ TD2Key }

procedure TD2Key.SetKey(const Value:single);
begin
  FKey:=Value;
  if FKey < 0 then FKey:=0;
  if FKey > 1 then FKey:=1;
end;

{ TD2Keys }

function TD2Keys.FindKeys(const Time:single; var Key1,Key2:TD2Key):boolean;
var
  i:integer;
begin
  Result:=false;
  if Count < 2 then Exit;
  for i:=0 to Count - 2 do
    if ((Time >= TD2Key(Items[i]).Key) and (Time <= TD2Key(Items[i + 1]).Key)) then
    begin
      Result:=true;
      Key1:=TD2Key(Items[i]);
      Key2:=TD2Key(Items[i + 1]);
      Exit;
    end;
end;

{ TD2ColorKeyAnimation ========================================================}

constructor TD2ColorKeyAnimation.Create(AOwner:TComponent);
begin
  inherited;
  FKeys:=TD2Keys.Create(TD2ColorKey);
end;

destructor TD2ColorKeyAnimation.Destroy;
begin
  FKeys.Free;
  inherited;
end;

procedure TD2ColorKeyAnimation.Start;
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
        Persistent:=d2GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
  end;
  if (FInstance <> nil) and StartFromCurrent then
  begin
    if Keys.Count > 0 then
    begin
      { is string prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkString,tkLString,tkWString{$IFDEF FPC},tkAString{$ENDIF}{$IFDEF KS_COMPILER11_UP},tkUString{$ENDIF}]) <> nil then
        TD2ColorKey(Keys.Items[0]).Value:=GetStrProp(FInstance,FPath);
      { is int prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkInteger]) <> nil then
        TD2ColorKey(Keys.Items[0]).Value:=d2ColorToStr(GetOrdProp(FInstance,FPath));
    end;
  end;
  inherited;
end;

procedure TD2ColorKeyAnimation.ProcessAnimation;
var
  Key1,Key2:TD2Key;
begin
  if FInstance <> nil then
  begin
    if FKeys.FindKeys(NormalizedTime,Key1,Key2) then
    begin
      if (TD2FloatKey(Key2).Key - TD2FloatKey(Key1).Key)=0 then Exit;
      { is string prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkString,tkLString,tkWString,tkAString]) <> nil then
        SetStrProp(FInstance,FPath,d2ColorToStr(d2InterpolateColor(d2StrToColor(TD2ColorKey(Key1).Value),d2StrToColor(TD2ColorKey(Key2).Value),(NormalizedTime - TD2FloatKey(Key1).Key) / (TD2FloatKey(Key2).Key - TD2FloatKey(Key1).Key))));
      { is int prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkInteger]) <> nil then
        SetOrdProp(FInstance,FPath,d2InterpolateColor(d2StrToColor(TD2ColorKey(Key1).Value),d2StrToColor(TD2ColorKey(Key2).Value),(NormalizedTime - TD2FloatKey(Key1).Key) / (TD2FloatKey(Key2).Key - TD2FloatKey(Key1).Key)));
    end;
  end;
end;

{ TD2FloatKeyAnimation ========================================================}

constructor TD2FloatKeyAnimation.Create(AOwner:TComponent);
begin
  inherited;
  FKeys:=TD2Keys.Create(TD2FloatKey);
end;

destructor TD2FloatKeyAnimation.Destroy;
begin
  FKeys.Free;
  inherited;
end;

procedure TD2FloatKeyAnimation.Start;
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
        Persistent:=d2GetToken(FPath,'.');
        if GetPropInfo(FInstance.ClassInfo,Persistent,[tkClass]) <> nil then
          FInstance:=GetObjectProp(FInstance,Persistent);
      end;
    end;
  end;
  if (FInstance <> nil) and StartFromCurrent then
  begin
    if Keys.Count > 0 then
    begin
      { is string prop }
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkFloat]) <> nil then
        TD2FloatKey(Keys.Items[0]).Value:=GetFloatProp(FInstance,FPath);
    end;
  end;
  inherited;
end;

procedure TD2FloatKeyAnimation.ProcessAnimation;
var
  Key1,Key2:TD2Key;
begin
  if FInstance <> nil then
  begin
    if FKeys.FindKeys(NormalizedTime,Key1,Key2) then
    begin
      if (TD2FloatKey(Key2).Key - TD2FloatKey(Key1).Key)=0 then Exit;
      if GetPropInfo(FInstance.ClassInfo,FPath,[tkFloat]) <> nil then
        SetFloatProp(FInstance,FPath,d2InterpolateSingle(TD2FloatKey(Key1).Value,TD2FloatKey(Key2).Value,(NormalizedTime - TD2FloatKey(Key1).Key) / (TD2FloatKey(Key2).Key - TD2FloatKey(Key1).Key)));
    end;
  end;
end;

{ TD2PathAnimation ========================================================}

constructor TD2PathAnimation.Create(AOwner:TComponent);
begin
  inherited;
  FPath:=TD2PathData.Create;
end;

destructor TD2PathAnimation.Destroy;
begin
  if FSpline <> nil then FreeAndNil(FSpline);
  FPath.Free;
  inherited;
end;

procedure TD2PathAnimation.ProcessAnimation;
var
  OldP,P1:TD2Point;
begin
  if (Length(FPolygon) > 0) and (FObj <> nil) then
  begin
    OldP:=FObj.Position.Point;
    FSpline.SplineXY(NormalizedTime * Length(FPolygon),P1.X,P1.Y);
    FObj.Position.X:=FStart.X + P1.X;
    FObj.Position.Y:=FStart.Y + P1.Y;
    if FRotate and (NormalizedTime <> 0) and (NormalizedTime <> 1) and ((OldP.X <> FObj.Position.X) and (OldP.Y <> FObj.Position.Y)) then
    begin
      if Inverse then
      begin
        if d2VectorCrossProductZ(d2Vector(FObj.Position.X - OldP.X,FObj.Position.Y - OldP.Y),d2Vector(0,1)) < 0 then
          FObj.RotateAngle:=180 + d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(FObj.Position.X - OldP.X,FObj.Position.Y - OldP.Y),d2Vector(0,1))))
        else
          FObj.RotateAngle:=180 - d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(FObj.Position.X - OldP.X,FObj.Position.Y - OldP.Y),d2Vector(0,1))))
      end
      else
      begin
        if d2VectorCrossProductZ(d2Vector(FObj.Position.X - OldP.X,FObj.Position.Y - OldP.Y),d2Vector(0,1)) < 0 then
          FObj.RotateAngle:=d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(FObj.Position.X - OldP.X,FObj.Position.Y - OldP.Y),d2Vector(0,1))))
        else
          FObj.RotateAngle:=-d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(FObj.Position.X - OldP.X,FObj.Position.Y - OldP.Y),d2Vector(0,1))))
      end;
    end;
  end;
end;

procedure TD2PathAnimation.SetPath(const Value:TD2PathData);
begin
  FPath.Assign(Value);
end;

procedure TD2PathAnimation.Start;
var
  i:integer;
begin
  inherited;
  if FSpline <> nil then FreeAndNil(FSpline);
  SetLength(FPolygon,0);
  if Assigned(Scene) and (Scene.GetCanvas <> nil) then
  begin
    FPath.FlattenToPolygon(FPolygon);
    if Length(FPolygon) > 1 then
      for i:=1 to High(FPolygon) do
        if (FPolygon[i].X=ClosePolygon.X) and (FPolygon[i].Y=ClosePolygon.Y) then
          FPolygon[i]:=FPolygon[i - 1];
    FSpline:=TD2Spline.Create(FPolygon);
  end;
  if (Parent <> nil) and (Parent.IsVisual) then
    FObj:=Parent.Visual
  else
    FObj:=nil;
  if FObj <> nil then
    FStart:=FObj.Position.Point;
end;

{ TD2PathSwitcher =============================================================}

constructor TD2PathSwitcher.Create(AOwner:TComponent);
begin
  inherited;
  Duration:=0.001;
end;

destructor TD2PathSwitcher.Destroy;
begin
  inherited;
end;

procedure TD2PathSwitcher.Start;
begin
  inherited;
end;

procedure TD2PathSwitcher.ProcessAnimation;
begin
  inherited;
end;

procedure TD2PathSwitcher.SetPathFalse(const Value:string);
begin
  FPathFalse:=Value;
end;

procedure TD2PathSwitcher.SetPathTrue(const Value:string);
begin
  FPathTrue:=Value;
end;

     
                 
