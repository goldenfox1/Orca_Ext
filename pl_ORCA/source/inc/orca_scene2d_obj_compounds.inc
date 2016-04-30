{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2CompoundAngleBar ===============================================

constructor TD2CompoundAngleBar.Create(AOwner:TComponent);
var
  C:TD2Control;
begin
  inherited;
  FDecimalDigits:=2;

  Width:=200;
  Height:=20;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    WordWrap:=false;
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    TextAlign:=d2TextAlignFar;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  C:=TD2Control.Create(Self);
  with C do
  begin
    Width:=40;
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaClient;
  end;
  FAngleBar:=TD2AngleButton.Create(Self);
  with FAngleBar do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaFit;
    Name:='AngleBar';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
  C:=TD2Control.Create(Self);
  with C do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaRight;
  end;
  FValueLabel:=TD2ValueLabel.Create(Self);
  with FValueLabel do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaVertCenter;
    Name:='ValueLabel';
    TextW:='0';
    Padding.left:=5;
    SetSubComponent(True);
  end;
end;

procedure TD2CompoundAngleBar.UpdateLabel;
begin
  FValueLabel.TextW:=IntToStr(Trunc(Value)) + '°';
end;

procedure TD2CompoundAngleBar.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then FOnChange(Sender);
  UpdateLabel;
end;

function TD2CompoundAngleBar.GetValue:single;
begin
  Result:=FAngleBar.Value;
end;

procedure TD2CompoundAngleBar.SetValue(const Value:single);
begin
  FAngleBar.Value:=Value;
  UpdateLabel;
end;

//=============== TD2CompoundTextBox =============================================

constructor TD2CompoundTextBox.Create(AOwner:TComponent);
var
  C:TD2Control;
begin
  inherited;
  Width:=200;
  Height:=21;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    TextAlign:=d2TextAlignFar;
    WordWrap:=false;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  C:=TD2Control.Create(Self);
  with C do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaClient;
  end;
  FTextBox:=TD2TextBox.Create(Self);
  with FTextBox do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaVertCenter;
    Name:='TextBox';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
end;

procedure TD2CompoundTextBox.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Sender);
end;

function TD2CompoundTextBox.GetText:WideString;
begin
  Result:=FTextBox.TextW;
end;

procedure TD2CompoundTextBox.SetText(const Value:WideString);
begin
  FTextBox.TextW:=Value;
end;

function TD2CompoundTextBox.GetText:String;  // 7777
begin
  Result:=UTF8Encode(GetText);
end;

procedure TD2CompoundTextBox.SetText(const Value:String); // 7777
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  SetText(ss);
end;

//=============== TD2CompoundMemo ============================================

constructor TD2CompoundMemo.Create(AOwner:TComponent);
begin
  inherited;
  Width:=200;
  Height:=60;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    TextAlign:=d2TextAlignFar;
    WordWrap:=false;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  FMemo:=TD2Memo.Create(Self);
  with FMemo do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaClient;
    Name:='Memo';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
end;

procedure TD2CompoundMemo.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Sender);
end;

function TD2CompoundMemo.GetText:WideString;
begin
  Result:=FMemo.TextW;
end;

function TD2CompoundMemo.GetText:String;
begin
  Result:=UTF8Encode(FMemo.TextW);
end;

procedure TD2CompoundMemo.SetText(const Value:WideString);
begin
  FMemo.TextW:=Value;
end;

procedure TD2CompoundMemo.SetText(const Value:String);
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  FMemo.TextW:=ss;
end;

//=============== TD2CompoundNumberBox ============================================

constructor TD2CompoundNumberBox.Create(AOwner:TComponent);
var
  C:TD2Control;
begin
  inherited;
  Width:=200;
  Height:=21;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Name:='TextLabel';
    TextW:='Caption';
    WordWrap:=false;
    Width:=70;
    TextAlign:=d2TextAlignFar;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  C:=TD2Control.Create(Self);
  with C do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaClient;
  end;
  FNumberBox:=TD2NumberBox.Create(Self);
  with FNumberBox do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaVertCenter;
    Name:='NumberBox';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
end;

procedure TD2CompoundNumberBox.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Sender);
end;

function TD2CompoundNumberBox.GetValue:single;
begin
  Result:=FNumberBox.Value;
end;

procedure TD2CompoundNumberBox.SetValue(const Value:single);
begin
  FNumberBox.Value:=Value;
end;

//=============== TD2CompoundPopupBox =================================================

constructor TD2CompoundPopupBox.Create(AOwner:TComponent);
var
  C:TD2Control;
begin
  inherited;
  Width:=200;
  Height:=21;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    TextAlign:=d2TextAlignFar;
    WordWrap:=false;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  C:=TD2Control.Create(Self);
  with C do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaClient;
  end;
  FPopupBox:=TD2PopupBox.Create(Self);
  with FPopupBox do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaVertCenter;
    Name:='PopupBox';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
end;

procedure TD2CompoundPopupBox.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Sender);
end;

function TD2CompoundPopupBox.GetItemIndex:integer;
begin
  Result:=FPopupBox.ItemIndex;
end;

procedure TD2CompoundPopupBox.SetItemIndex(const Value:integer);
begin
  FPopupBox.ItemIndex:=Value;
end;

//=============== TD2CompoundColorButton ======================================================

constructor TD2CompoundColorButton.Create(AOwner:TComponent);
var
  C:TD2Control;
begin
  inherited;
  Width:=200;
  Height:=21;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    TextAlign:=d2TextAlignFar;
    WordWrap:=false;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  C:=TD2Control.Create(Self);
  with C do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Width:=60;
  end;
  FColorButton:=TD2ColorButton.Create(Self);
  with FColorButton do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaVertCenter;
    Name:='ColorButton';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
end;

procedure TD2CompoundColorButton.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Sender);
end;

function TD2CompoundColorButton.GetValue:string;
begin
  Result:=FColorButton.Color;
end;

procedure TD2CompoundColorButton.SetValue(const Value:string);
begin
  FColorButton.Color:=Value;
end;

//=============== TD2CompoundImage ===================================================

constructor TD2CompoundImage.Create(AOwner:TComponent);
begin
  inherited;
  Width:=200;
  Height:=60;
  FTextLabel:=TD2Label.Create(Self);
  with FTextLabel do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaLeft;
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    TextAlign:=d2TextAlignFar;
    WordWrap:=false;
    Padding.right:=5;
    SetSubComponent(True);
  end;
  FImage:=TD2ImageControl.Create(Self);
  with FImage do
  begin
    Parent:=Self;
    Stored:=false;
    Locked:=true;
    Align:=vaClient;
    Name:='Image';
    SetSubComponent(True);
    OnChange:=DoChange;
  end;
end;

procedure TD2CompoundImage.DoChange(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Sender);
end;
              
      
          
                   
