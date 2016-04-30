{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//================== TD3Bounds ============================

constructor TD3Bounds.Create;
begin
  inherited Create;
end;

procedure TD3Bounds.Assign(Source:TPersistent);
begin
  if Source is TD3Bounds then
  begin
    FLeft:=TD3Bounds(Source).FLeft;
    FTop:=TD3Bounds(Source).FTop;
    FRight:=TD3Bounds(Source).FRight;
    FBottom:=TD3Bounds(Source).FBottom;
    FNear:=TD3Bounds(Source).FNear;
    FFar:=TD3Bounds(Source).FFar;
    if Assigned(OnChange) then
      OnChange(Self);
  end
  else
    inherited
end;

function TD3Bounds.Width:single;
begin
  Result:=Fright - Fleft;
end;

function TD3Bounds.Height:single;
begin
  Result:=Fbottom - Ftop;
end;

function TD3Bounds.Depth:single;
begin
  Result:=FFar - FNear;
end;

function TD3Bounds.MarginEmpty:boolean;
begin
  Result:=(FLeft=0) and (FTop=0) and (FRight=0) and (FBottom=0) and (FNear=0) and (FFar=0);
end;

procedure TD3Bounds.SetBottom(const Value:single);
begin
  if FBottom <> Value then
  begin
    FBottom:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD3Bounds.SetFar(const Value:single);
begin
  if FFar <> Value then
  begin
    FFar:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD3Bounds.SetNear(const Value:single);
begin
  if FNear<> Value then
  begin
    FNear:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD3Bounds.SetLeft(const Value:single);
begin
  if FLeft <> Value then
  begin
    FLeft:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD3Bounds.SetRight(const Value:single);
begin
  if FRight <> Value then
  begin
    FRight:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD3Bounds.SetTop(const Value:single);
begin
  if FTop <> Value then
  begin
    FTop:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD3Bounds.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('Rect',ReadRect,WriteRect,false);
  Filer.DefineProperty('NewRect',ReadNewRect,WriteNewRect,(FLeft <> 0) or (FTop <> 0) or
    (FRight <> 0) or (FBottom <> 0) or (ZNear <> 0) or (ZFar <> 0));
end;

procedure TD3Bounds.ReadRect(Reader:TReader);
var
  R:TD2Rect;
begin
  R:=d2StringToRect(Reader.ReadString);
  FLeft:=R.Left;
  FTop:=R.Top;
  FRight:=R.Right;
  FBottom:=R.Bottom;
end;

procedure TD3Bounds.WriteRect(Writer:TWriter);
begin
end;

procedure TD3Bounds.ReadNewRect(Reader:TReader);
var
  S:AnsiString;
begin
  S:=Reader.ReadString;
  try
    GetToken(S,',()');
    FLeft:=d2StrToFloat(GetToken(S,',()'));
    FTop:=d2StrToFloat(GetToken(S,',()'));
    FRight:=d2StrToFloat(GetToken(S,',()'));
    FBottom:=d2StrToFloat(GetToken(S,',()'));
    FNear:=d2StrToFloat(GetToken(S,',()'));
    FFar:=d2StrToFloat(GetToken(S,',()'));
  except
  end;
end;

procedure TD3Bounds.WriteNewRect(Writer:TWriter);
var
  S:AnsiString;
begin
  S:='(' + d2FloatToStr(FLeft) + ',' + d2FloatToStr(FTop) + ',' + d2FloatToStr(FRight) + ',' +
    d2FloatToStr(FBottom) + ',' + d2FloatToStr(FNear) + ',' + d2FloatToStr(FFar) + ')';
  Writer.WriteString(S);
end;
     
