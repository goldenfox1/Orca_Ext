{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2RadioButton ==========================================

constructor TD2RadioButton.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  AutoCapture:=true;
  CanFocused:=true;
  TextAlign:=d2TextAlignNear;
  Width:=120;
  Height:=19;
end;

destructor TD2RadioButton.Destroy;
begin
  inherited;
end;

function TD2RadioButton.GetData:Variant;
begin
  Result:=IsChecked;
end;

procedure TD2RadioButton.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    if VarIsType(Value,varBoolean) then
      IsChecked:=Value
    else
      IsChecked:=false;
end;

procedure TD2RadioButton.ApplyStyle;
begin
  inherited;
  StartTriggerAnimation(Self,'IsChecked');
end;

procedure TD2RadioButton.EnterFocus;
begin
  inherited;
end;

procedure TD2RadioButton.MouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    FPressing:=true;
    FIsPressed:=true;
    StartTriggerAnimation(Self,'IsPressed');
  end;
end;

procedure TD2RadioButton.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
  if (ssLeft in Shift) and (FPressing) then
  begin
    if FIsPressed <> d2PtInRect(d2Point(X,Y),LocalRect) then
    begin
      FIsPressed:=d2PtInRect(d2Point(X,Y),LocalRect);
      StartTriggerAnimation(Self,'IsPressed');
    end;
  end;
end;

procedure TD2RadioButton.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if FPressing then
  begin
    FPressing:=false;
    FIsPressed:=false;
    if d2PtInRect(d2Point(X,Y),LocalRect) then
    begin
      IsChecked:=not IsChecked;
    end
  end;
end;

procedure TD2RadioButton.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited;
  if (Key=VK_RETURN) or (Key=VK_SPACE) then
  begin
    IsChecked:=not IsChecked;
  end;
end;

procedure TD2RadioButton.SetIsChecked(const Value:boolean);
var
  List:TList;
  i,c,cc:integer;
begin
  if FIsChecked <> Value then
  begin
    if Value then
      FIsChecked:=Value;
    { group }
    c:=0;
    cc:=0;
    if Assigned(FScene) and (FScene.GetRoot <> nil) then
    begin
      List:=TList.Create;
      FScene.GetRoot.AddControlsToList(List);
      for i:=0 to List.Count - 1 do
        if (TD2Object(List[i]) is TD2RadioButton) and (TD2Object(List[i]) <> Self) and
           (TD2RadioButton(List[i]).GroupName=GroupName) then
        begin
          if TD2RadioButton(List[i]).IsChecked then
            cc:=cc + 1;
          if Value then
            TD2RadioButton(List[i]).IsChecked:=false;
          c:=c + 1;
        end;
      List.Free;
    end;
    { check }
    if not Value and (c=0) then Exit;
    if not Value and (cc=0) then Exit;
    FIsChecked:=Value;
    StartTriggerAnimation(Self,'IsChecked');
    { event }
    if Assigned(FOnChange) then
    begin
      FOnChange(Self);
    end;
  end;
end;
                      
