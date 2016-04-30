{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2CheckBox ===================================

constructor TD2CheckBox.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  AutoCapture:=true;
  CanFocused:=true;
  TextAlign:=d2TextAlignNear;
  Width:=120;
  Height:=19;
end;

destructor TD2CheckBox.Destroy;
begin
  inherited;
end;

function TD2CheckBox.GetData:Variant;
begin
  Result:=IsChecked;
end;

procedure TD2CheckBox.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    if VarIsType(Value,varBoolean) then
      IsChecked:=Value else
      IsChecked:=false;
end;

procedure TD2CheckBox.ApplyStyle;
begin
  inherited;
  StartTriggerAnimation(Self,'IsChecked');
end;

procedure TD2CheckBox.MouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    FPressing:=true;
    FIsPressed:=true;
    StartTriggerAnimation(Self,'IsPressed');
  end;
end;

procedure TD2CheckBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
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

procedure TD2CheckBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
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

procedure TD2CheckBox.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited;
  if (Key=VK_RETURN) or (Key=VK_SPACE) then
  begin
    IsChecked:=not IsChecked;
  end;
end;

procedure TD2CheckBox.SetIsChecked(const Value:boolean);
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    StartTriggerAnimation(Self,'IsChecked');
    if Assigned(FOnChange) then
    begin
      FOnChange(Self);
    end;
  end;
end;

//=============== TD2PathCheckBox =============================================

constructor TD2PathCheckBox.Create(AOwner:TComponent);
begin
  inherited;
  FPath:=TD2PathData.Create;
  FPath.OnChanged:=DoPathChange;
  FPath.Data:='M 341.236,311.738 L 309.372,342.676 L 291.667,325.488 L 291.667,304.863 L 309.372,321.997 L 341.236,291.113 Z';
end;

destructor TD2PathCheckBox.Destroy;
begin
  FPath.Free;
  inherited;
end;

procedure TD2PathCheckBox.ApplyStyle;
var
  Checkmark:TD2Object;
begin
  inherited;
  Checkmark:=FindResource('checkmark');
  if (Checkmark <> nil) and (Checkmark is TD2Path) then
  begin
    TD2Path(Checkmark).Data.Assign(FPath);
  end;
end;

procedure TD2PathCheckBox.DoPathChange(Sender:TObject);
begin
  ApplyStyle;
end;

procedure TD2PathCheckBox.SetPath(const Value:TD2PathData);
begin
  FPath.Assign(Value);
end;  
                    
