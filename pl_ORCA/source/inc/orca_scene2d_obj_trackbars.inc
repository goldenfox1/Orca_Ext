{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2CustomTrack ==============================================

constructor TD2CustomTrack.Create(AOwner:TComponent);
begin
  inherited;
  FViewportSize:=0;
  FMax:=100;
  FTracking:=true;
  Width:=100;
  Height:=15;
end;

destructor TD2CustomTrack.Destroy;
begin
  inherited;
end;

function TD2CustomTrack.GetData:Variant;
begin
  Result:=Value;
end;

procedure TD2CustomTrack.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
   if VarIsNumeric(Value) then
     Self.Value:=Value
   else
     Self.Value:=Min
end;

function TD2CustomTrack.GetThumbRect:TD2Rect;
begin
  Result:=LocalRect;
  if (FMax - FMin) > 0 then
  begin
    if Orientation=d2Horizontal then
    begin
      Result:=d2Rect(0,0,(FViewportSize / (FMax - FMin)) * Width,Height);
      d2OffsetRect(Result,Round((((FValue - FMin) / (FMax - FMin))) * Width),0);
      if Result.Right - Result.Left < Height then
      begin
        Result.Right:=Result.Left + trunc(Height / 2);
        Result.Left:=Result.Left - trunc(Height / 2);
      end;
      if Result.Right > Width then
        d2OffsetRect(Result,Width - Result.Right,0);
      if Result.Left < 0 then
        d2OffsetRect(Result,- Result.Left,0);
    end
    else
    begin
      Result:=d2Rect(0,0,Width,(FViewportSize / (FMax - FMin)) * Height);
      if Result.Bottom - Result.Top < Width then
      begin
        Result.Bottom:=Result.Top + trunc(Width / 2);
        Result.Top:=Result.Top - trunc(Width / 2);
      end;
      d2OffsetRect(Result,0,Round((((FValue - FMin) / (FMax - FMin))) * Height));
      if Result.Bottom > Height then
        d2OffsetRect(Result,0,Height - Result.Bottom);
      if Result.Top < 0 then
        d2OffsetRect(Result,0,- Result.Top);
    end;
  end;
  if (Thumb <> nil) and (Thumb.Parent <> nil) and (Thumb.Parent.IsVisual) then
  begin
    if d2RectWidth(Result) > TD2VisualObject(Thumb.Parent).Margins.Left + Thumb.Padding.Left + TD2VisualObject(Thumb.Parent).Margins.Right - Thumb.Padding.Right then
    begin
      Result.Left:=Result.Left + TD2VisualObject(Thumb.Parent).Margins.Left + Thumb.Padding.Left;
      Result.Right:=Result.Right - TD2VisualObject(Thumb.Parent).Margins.Right - Thumb.Padding.Right;
    end;
    Result.Top:=Result.Top + TD2VisualObject(Thumb.Parent).Margins.Top + Thumb.Padding.Top;
    Result.Bottom:=Result.Bottom - TD2VisualObject(Thumb.Parent).Margins.Bottom - Thumb.Padding.Bottom;
  end;
end;

function TD2CustomTrack.GetThumb:TD2Thumb;
var
  T:TD2Object;
begin
  T:=FindResource('thumb');
  if (T <> nil) and (T is TD2Thumb) then
  begin
    Result:=TD2Thumb(T);
    Result.FTrack:=Self;
  end
  else
    Result:=nil;
end;

procedure TD2CustomTrack.Realign;
begin
  inherited ;
  if Thumb <> nil then
  begin
    with GetThumbRect do
    begin
      Thumb.Position.X:=Left;
      Thumb.Position.Y:=Top;
      if Round(Right - Left) > 0 then
        Thumb.Width:=Round(Right - Left);
      if Round(Bottom - Top) > 0 then
        Thumb.Height:=Round(Bottom - Top);
    end;
  end;
end;

procedure TD2CustomTrack.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    if Orientation=d2Horizontal then
    begin
      if FViewportSize=0 then
        Value:=Min + ((X / Width) * (FMax - FMin))
      else
      begin
        if Min + ((X / Width) * (FMax - FMin)) > Value then
          Value:=Value + FViewportSize
        else
          Value:=Value - FViewportSize
      end;
    end
    else
    begin
      if FViewportSize=0 then
        Value:=Min + ((Y / Height) * (FMax - FMin))
      else
      begin
        if Min + ((Y / Height) * (FMax - FMin)) > Value then
          Value:=Value + FViewportSize
        else
          Value:=Value - FViewportSize
      end;
    end;
  end;
end;

procedure TD2CustomTrack.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
end;

procedure TD2CustomTrack.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
end;

procedure TD2CustomTrack.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
var
  inc:single;
begin
  inc:=Frequency;
  if inc=0 then inc:=1;
  inherited;
  case Key of
    VK_HOME:Value:=Min;
    VK_END:Value:=Max;
    VK_UP:Value:=Value - inc;
    VK_DOWN:Value:=Value + inc;
    VK_LEFT:Value:=Value - inc;
    VK_RIGHT:Value:=Value + inc;
  else
    Exit;
  end;
  if not Tracking and Assigned(FOnChange) then
    FOnChange(Self);
  Key:=0;
end;

procedure TD2CustomTrack.SetMax(const Value:single);
begin
  if FMax <> Value then
  begin
    FMax:=Value;
    if FMax < FMin then
      FMax:=FMin + 0.001;
    if FViewportSize > (FMax - FMin) then
      FViewportSize:=FMax - FMin;
    if FValue > FMax - FViewportSize then
      FValue:=FMax - FViewportSize;
    Realign;
  end;
end;

procedure TD2CustomTrack.SetMin(const Value:single);
begin
  if FMin <> Value then
  begin
    FMin:=Value;
    Realign;
  end;
end;

procedure TD2CustomTrack.SetOrientation(const Value:TD2Orientation);
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

function TD2CustomTrack.GetIsTracking:boolean;
begin
  Result:=(Thumb <> nil) and Thumb.FPressed;
end;

procedure TD2CustomTrack.SetFrequency(const Value:single);
begin
  if FFrequency <> Value then
  begin
    FFrequency:=Value;
    if FFrequency <> 0 then
      Self.Value:=Round(Self.Value / Frequency) * Frequency;
  end;
end;

procedure TD2CustomTrack.SetValue(Value:single);
begin
  if FFrequency <> 0 then
    Value:=Round(Value / Frequency) * Frequency;
  if FValue <> Value then
  begin
    FValue:=Value;
    if FValue > FMax - FViewportSize then FValue:=FMax - FViewportSize;
    if FValue < FMin then FValue:=FMin;
    if GetIsTracking and Assigned(FOnTracking) then
      FOnTracking(Self);
    if Tracking and Assigned(FBindingObjects) then
      ToBindingObjects;
    if GetIsTracking and Tracking and Assigned(FOnChange) then
      FOnChange(Self)
    else
      if not GetIsTracking and Assigned(FOnChange) then
        FOnChange(Self);
    Realign;
  end;
end;

procedure TD2CustomTrack.SetViewportSize(const Value:single);
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

//=============== TD2TrackBar ==================================

constructor TD2TrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FViewportSize:=0;
  CanFocused:=true;
end;
  
//============ TD2BitmapTrackBar =================================

constructor TD2BitmapTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='trackbarstyle';
end;

destructor TD2BitmapTrackBar.Destroy;
begin
  if FBitmap <> nil then
    FreeAndNil(FBitmap);
  inherited;
end;

procedure TD2BitmapTrackBar.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('background');
  if (T <> nil) and (T is TD2Shape) then
  begin
    FBackground:=TD2Shape(T);
    UpdateBitmap;
  end;
end;

procedure TD2BitmapTrackBar.FreeStyle;
begin
  FBackground:=nil;
  inherited;
end;

procedure TD2BitmapTrackBar.Realign;
begin
  inherited;
  UpdateBitmap;
end;

procedure TD2BitmapTrackBar.UpdateBitmap;
begin
  if FBackground=nil then Exit;

  if FBitmap <> nil then
    if (FBitmap.Width <> Trunc(FBackground.Width)) or (FBitmap.Height <> Trunc(FBackground.Height)) then
    begin
      FreeAndNil(FBitmap);
    end;

  if FBitmap=nil then
  begin
    FBitmap:=TD2Bitmap.Create(Trunc(FBackground.Width),Trunc(FBackground.Height));
    FillBitmap;
  end;
  FBackground.Fill.Style:=d2BrushBitmap;
  FBackground.Fill.Bitmap.Bitmap:=FBitmap;

  Repaint;
end;

procedure TD2BitmapTrackBar.FillBitmap;
begin
end;

//============  TD2HueTrackBar ==============================

constructor TD2HueTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  Max:=1;
  Value:=0.5;
end;

procedure TD2HueTrackBar.FillBitmap;
var
  i,j:integer;
begin
  for j:=0 to FBitmap.Height - 1 do
  begin
    for i:=0 to FBitmap.Width - 1 do
    begin
      if Orientation=d2Horizontal then
        FBitmap.Scanline[j][i]:=d2CorrectColor(d2HSLtoRGB(i / FBitmap.Width,0.9,0.5))
      else
        FBitmap.Scanline[j][i]:=d2CorrectColor(d2HSLtoRGB(j / FBitmap.Height,0.9,0.5));
    end;
  end;
end;

//============  TD2AlphaTrackBar =====================================

constructor TD2AlphaTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  Max:=1;
  Value:=1;
end;

procedure TD2AlphaTrackBar.FillBitmap;
var
  i,j:integer;
begin
  for j:=0 to FBitmap.Height - 1 do
  begin
    for i:=0 to FBitmap.Width - 1 do
    begin
      if odd(i div 3) and not odd(j div 3) then
        FBitmap.Scanline[j][i]:=d2CorrectColor($FFA0A0A0)
      else
      if not odd(i div 3) and odd(j div 3) then
        FBitmap.Scanline[j][i]:=d2CorrectColor($FFA0A0A0)
      else
        FBitmap.Scanline[j][i]:=d2CorrectColor($FFFFFFFF)
    end;
  end;
  if FBitmap.Canvas.BeginScene then
  begin
    FBitmap.Canvas.Fill.Style:=d2BrushGradient;
    FBitmap.Canvas.Fill.Gradient.Points[0].Color:='$00FFFFFF';
    FBitmap.Canvas.Fill.Gradient.Points[1].Color:='$FFFFFFFF';
    FBitmap.Canvas.Fill.Gradient.StopPosition.Point:=d2Point(1,0);
    FBitmap.Canvas.FillRect(d2Rect(0,0,FBitmap.Width,FBitmap.Height),0,0,[],1);
    FBitmap.Canvas.EndScene;
  end;
end;

//============  TD2BWTrackBar =================

constructor TD2BWTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  Max:=1;
  Value:=0.5;
end;

procedure TD2BWTrackBar.FillBitmap;
var
  i,j:integer;
  a:byte;
begin
  for j:=0 to FBitmap.Height - 1 do
  begin
    for i:=0 to FBitmap.Width - 1 do
    begin
      if Orientation=d2Horizontal then
        a:=round((i / FBitmap.Width) * $FF)
      else
        a:=round((j / FBitmap.Height) * $FF);
      FBitmap.Scanline[j][i]:=d2CorrectColor(d2Color(a,a,a));
    end;
  end;
end;

//=============== TD2HudHueTrackBar ===========================

constructor TD2HudHueTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='hudtrackbarstyle';
end;

//=============== TD2HudAlphaTrackBar ==========================

constructor TD2HudAlphaTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='hudtrackbarstyle';
end;

//=============== TD2HudBWTrackBar =============================

constructor TD2HudBWTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='hudtrackbarstyle';
end;      

//=============== TD2CompoundTrackBar ==================================================

constructor TD2CompoundTrackBar.Create(AOwner:TComponent);
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
    Name:='TextLabel';
    TextW:='Caption';
    Width:=70;
    Padding.right:=5;
    WordWrap:=false;
    TextAlign:=d2TextAlignFar;
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
  FTrackBar:=TD2Trackbar.Create(Self);
  with FTrackBar do
  begin
    Parent:=C;
    Stored:=false;
    Locked:=true;
    Align:=vaVertCenter;
    Name:='TrackBar';
    SetSubComponent(True);
    OnChange:=DoTrack;
    OnTracking:=DoTracking;
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

procedure TD2CompoundTrackBar.UpdateLabel;
begin
  if (FTrackBar.Frequency <> 0) and (frac(FTrackBar.Frequency)=0) then
    FValueLabel.TextW:=IntToStr(Trunc(Value)) + Suffix
  else
    FValueLabel.TextW:=Format('%.' + IntToStr(FDecimalDigits) + 'f',[Self.Value]) + Suffix;
end;

procedure TD2CompoundTrackBar.DoTrack(Sender:TObject);
begin
  if Assigned(FOnChange) then FOnChange(Sender);
end;

procedure TD2CompoundTrackBar.DoTracking(Sender:TObject);
begin
  UpdateLabel;
end;

function TD2CompoundTrackBar.GetValue:single;
begin
  Result:=FTrackBar.Value;
end;

procedure TD2CompoundTrackBar.SetDecimalDigits(const Value:integer);
begin
  if FDecimalDigits <> Value then
  begin
    FDecimalDigits:=Value;
    UpdateLabel;
  end;
end;

procedure TD2CompoundTrackBar.SetValue(const Value:single);
begin
  FTrackBar.Value:=Value;
  UpdateLabel;
end;

procedure TD2CompoundTrackBar.SetSuffix(const Value:WideString);
begin
  if FSuffix <> Value then
  begin
    FSuffix:=Value;
    UpdateLabel;
  end;
end;
                            
                    
