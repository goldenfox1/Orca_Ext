{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Panel ===========================================

constructor TD2Panel.Create(AOwner:TComponent);
begin
  inherited;
  Width:=120;
  Height:=100;
end;  


//=============== TD2CalloutPanel =====================================

constructor TD2CalloutPanel.Create(AOwner:TComponent);
begin
  inherited;
  FCalloutWidth:=23;
  FCalloutLength:=11;
end;

procedure TD2CalloutPanel.ApplyStyle;
var
  Back:TD2Object;
begin
  inherited;
  Back:=FindResource('Backgound');
  if (Back=nil) and (FResourceLink is TD2CalloutRectangle) then
    Back:=FResourceLink;
  if (Back <> nil) and (Back is TD2CalloutRectangle) then
  begin
    TD2CalloutRectangle(Back).CalloutWidth:=FCalloutWidth;
    TD2CalloutRectangle(Back).CalloutLength:=FCalloutLength;
    TD2CalloutRectangle(Back).CalloutPosition:=FCalloutPosition;
    TD2CalloutRectangle(Back).CalloutOffset:=FCalloutOffset;
  end;
end;

procedure TD2CalloutPanel.SetCalloutLength(const Value:single);
begin
  if FCalloutLength <> Value then
  begin
    FCalloutLength:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CalloutPanel.SetCalloutPosition(
  const Value:TD2CalloutPosition);
begin
  if FCalloutPosition <> Value then
  begin
    FCalloutPosition:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CalloutPanel.SetCalloutWidth(const Value:single);
begin
  if FCalloutWidth <> Value then
  begin
    FCalloutWidth:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CalloutPanel.SetCalloutOffset(const Value:single);
begin
  if FCalloutOffset <> Value then
  begin
    FCalloutOffset:=Value;
    ApplyStyle;
  end;
end;  

//================== TD2ColorPanel ================================

constructor TD2ColorPanel.Create(AOwner:TComponent);
begin
  inherited;
  FUseAlpha:=true;
  Width:=150;
  Height:=150;
  FAlphaTrack:=TD2AlphaTrackBar.Create(Self);
  FAlphaTrack.Parent:=Self;
  FAlphaTrack.Align:=vaBottom;
  FAlphaTrack.Stored:=false;
  FAlphaTrack.Locked:=true;
  FAlphaTrack.Padding.Rect:=d2Rect(0,0,15,0);
  FAlphaTrack.Height:=15;
  FAlphaTrack.DisableFocusEffect:=true;
  FAlphaTrack.OnChange:=DoAlphaChange;

  FHueTrack:=TD2HueTrackBar.Create(Self);
  FHueTrack.Parent:=Self;
  FHueTrack.Align:=vaRight;
  FHueTrack.Stored:=false;
  FHueTrack.Locked:=true;
  FHueTrack.Padding.Rect:=d2Rect(0,0,0,0);
  FHueTrack.Orientation:=d2Vertical;
  FHueTrack.Width:=15;
  FHueTrack.DisableFocusEffect:=true;
  FHueTrack.OnChange:=DoHueChange;

  FColorQuad:=TD2ColorQuad.Create(Self);
  FColorQuad.Parent:=Self;
  FColorQuad.Align:=vaClient;
  FColorQuad.Stored:=false;
  FColorQuad.Locked:=true;
  FColorQuad.Padding.Rect:=d2Rect(5,5,3,3);
  FColorQuad.OnChange:=DoQuadChange;

  Color:=vcWhite;
end;

destructor TD2ColorPanel.Destroy;
begin
  inherited;
end;

procedure TD2ColorPanel.DoAlphaChange(Sender:TObject);
begin
  FColorQuad.Alpha:=FAlphaTrack.Value;
end;

procedure TD2ColorPanel.DoHueChange(Sender:TObject);
begin
  FColorQuad.Hue:=FHueTrack.Value;
end;

procedure TD2ColorPanel.DoQuadChange(Sender:TObject);
begin
  if FColorBox <> nil then
    FColorBox.Color:=d2StrToColor(Color);
  if Assigned(OnChange) then
    OnChange(Self);
end;

function TD2ColorPanel.GetColor:string;
begin
  Result:=d2ColorToStr(d2Opacity(d2HSLtoRGB(FColorQuad.Hue,FColorQuad.Sat,FColorQuad.Lum),FColorQuad.Alpha));
end;

procedure TD2ColorPanel.Loaded;
begin
  inherited;
  Color:=Color;
end;

procedure TD2ColorPanel.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FColorBox) then
    ColorBox:=nil;
end;

procedure TD2ColorPanel.SetColor(const Value:string);
var
  H,S,L:single;
  C:TD2Color;
  SaveOnChange:TNotifyEvent;
begin
  SaveOnChange:=FOnChange;
  FOnChange:=nil;

  C:=d2StrToColor(Value);
  d2RGBtoHSL(C,H,S,L);
  FColorQuad.Lum:=L;
  FColorQuad.Sat:=S;
  FHueTrack.Value:=H;
  FAlphaTrack.Value:=TD2ColorRec(C).A / $FF;

  FOnChange:=SaveOnChange;

  if not (csLoading in ComponentState) then
    DoQuadChange(Self);
end;

procedure TD2ColorPanel.SetColorBox(const Value:TD2ColorBox);
begin
  if FColorBox <> Value then
  begin
    FColorBox:=Value;
    if FColorBox <> nil then
      FColorBox.Color:=d2StrToColor(Color);
  end;
end;

procedure TD2ColorPanel.SetUseAlpha(const Value:boolean);
begin
  if FUseAlpha <> Value then
  begin
    FUseAlpha:=Value;
    FAlphaTrack.Visible:=FUseAlpha;
  end;
end;                    
