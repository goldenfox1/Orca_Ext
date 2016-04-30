{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2ScrollBar =====================================

constructor TD2ScrollBar.Create(AOwner:TComponent);
begin
  inherited;
  FMax:=100;
  FViewportSize:=0;
  FSmallChange:=1;
  Width:=150;
  Height:=18;
end;

destructor TD2ScrollBar.Destroy;
begin
  inherited;
end;

function TD2ScrollBar.GetData:Variant;
begin
  Result:=Value;
end;

procedure TD2ScrollBar.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    if VarIsNumeric(Value) then
      Self.Value:=Value
    else
      Self.Value:=Min
end;

function TD2ScrollBar.Track:TD2CustomTrack;
var
  T:TD2Object;
  HT,VT:TD2CustomTrack;
begin
  HT:=nil; VT:=nil;
  T:=FindResource('htrack');
  if (T <> nil) and (T is TD2CustomTrack) then
  begin
    HT:=TD2CustomTrack(T);
    HT.FOrientation:=d2Horizontal;
    HT.FMax:=Max;
    HT.FMin:=Min;
    HT.FValue:=Value;
    HT.ViewportSize:=ViewportSize;
    HT.Visible:=Orientation=d2Horizontal;
    HT.OnChange:=DoTrackChanged;
    HT.CanFocused:=false;
    if HT.visible then HT.Realign;
  end;
  T:=FindResource('vtrack');
  if (T <> nil) and (T is TD2CustomTrack) then
  begin
    VT:=TD2CustomTrack(T);
    VT.FOrientation:=d2Vertical;
    VT.FMax:=Max;
    VT.FMin:=Min;
    VT.FValue:=Value;
    VT.ViewportSize:=ViewportSize;
    VT.Visible:=Orientation=d2Vertical;
    VT.OnChange:=DoTrackChanged;
    VT.CanFocused:=false;
    if VT.visible then VT.Realign;
  end;
  if Orientation=d2Vertical then
    Result:=VT
  else
    Result:=HT;
end;

function TD2ScrollBar.MinButton:TD2CustomButton;
var
  T:TD2Object;
  LB,TB:TD2CustomButton;
begin
  TB:=nil; LB:=nil;
  T:=FindResource('leftbutton');
  if (T <> nil) and (T is TD2CustomButton) then
  begin
    LB:=TD2CustomButton(T);
    LB.OnClick:=DoMinButtonClick;
    LB.Visible:=Orientation=d2Horizontal;
    LB.CanFocused:=false;
  end;

  T:=FindResource('topbutton');
  if (T <> nil) and (T is TD2CustomButton) then
  begin
    TB:=TD2CustomButton(T);
    TB.OnClick:=DoMinButtonClick;
    TB.Visible:=Orientation=d2Vertical;
    TB.CanFocused:=false;
  end;

  if Orientation=d2Vertical then
    Result:=TB
  else
    Result:=LB;
end;

function TD2ScrollBar.MaxButton:TD2CustomButton;
var
  T:TD2Object;
  RB,BB:TD2CustomButton;
begin
  RB:=nil; BB:=nil;
  T:=FindResource('rightbutton');
  if (T <> nil) and (T is TD2CustomButton) then
  begin
    RB:=TD2CustomButton(T);
    RB.OnClick:=DoMaxButtonClick;
    RB.Visible:=Orientation=d2Horizontal;
    RB.CanFocused:=false;
  end;

  T:=FindResource('bottombutton');
  if (T <> nil) and (T is TD2CustomButton) then
  begin
    BB:=TD2CustomButton(T);
    BB.OnClick:=DoMaxButtonClick;
    BB.Visible:=Orientation=d2Vertical;
    RB.CanFocused:=false;
  end;

  if Orientation=d2Vertical then
    Result:=BB
  else
    Result:=RB;
end;

procedure TD2ScrollBar.DoTrackChanged(Sender:TObject);
begin
  Value:=TD2CustomTrack(Sender).Value;
end;

procedure TD2ScrollBar.DoMinButtonClick(Sender:TObject);
begin
  Value:=Value - SmallChange;
end;

procedure TD2ScrollBar.DoMaxButtonClick(Sender:TObject);
begin
  Value:=Value + SmallChange;
end;

procedure TD2ScrollBar.Realign;
begin
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  Track;
  MinButton;
  MaxButton;
  FDisableAlign:=false;
  inherited;
end;

procedure TD2ScrollBar.SetMax(const Value:single);
begin
  if FMax <> Value then
  begin
    FMax:=Value;
    if FMax < FMin then
      FMax:=FMin + 0.001;
    if FValue > FMax - FViewportSize then
      Self.Value:=FMax - FViewportSize;
    if FViewportSize > (FMax - FMin) then
      FViewportSize:=FMax - FMin;
    Realign;
  end;
end;

procedure TD2ScrollBar.SetMin(const Value:single);
begin
  if FMin <> Value then
  begin
    FMin:=Value;
    if FValue < FMin then
      Self.Value:=FMin;
    Realign;
  end;
end;

procedure TD2ScrollBar.SetOrientation(const Value:TD2Orientation);
var
  T:single;
begin
  if FOrientation <> Value then
  begin
    FOrientation:=Value;
    // swap
    if Assigned(FScene) and (FScene.GetDesignTime) and not (csLoading in ComponentState) then
    begin
      T:=Width;
      Width:=Height;
      Height:=T;
    end;
    Realign;
  end;
end;

procedure TD2ScrollBar.SetValue(const Value:single);
begin
  if FValue <> Value then
  begin
    FValue:=Value;
    if FValue > FMax - FViewportSize then
      FValue:=FMax - FViewportSize;
    if FValue < FMin then
      FValue:=FMin;
    if Assigned(FBindingObjects) then
      ToBindingObjects;
    if Assigned(FOnChange) then
      FOnChange(Self);
    Realign;
  end;
end;

procedure TD2ScrollBar.SetViewportSize(const Value:single);
begin
  if FViewportSize <> Value then
  begin
    FViewportSize:=Value;
    if FViewportSize > (FMax - FMin) then
      FViewportSize:=FMax - FMin;
    if FValue > FMax - FViewportSize then
      Self.Value:=FMax - FViewportSize;
    Realign;
  end;
end;

//=============== TD2SmallScrollBar =================================================

constructor TD2SmallScrollBar.Create(AOwner:TComponent);
begin
  inherited;
  Height:=8;
end;


//=============== TD2ScrollArrowLeft =================================

constructor TD2ScrollArrowLeft.Create(AOwner:TComponent);
begin
  inherited;
  Data.Data:='M 1,0 L 1,1 L 0,0.500 L 1,0 Z';
  Data.Scale(100,100);
  WrapMode:=d2PathFit;
end;

//=============== TD2ScrollArrowRight =================================

constructor TD2ScrollArrowRight.Create(AOwner:TComponent);
begin
  inherited;
  Data.Data:='M 0,0 L 0,1 L 1,0.500 Z';
  Data.Scale(100,100);
  WrapMode:=d2PathFit;
end;
        
           
                    
