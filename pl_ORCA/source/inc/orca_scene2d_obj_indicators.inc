{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2AniIndicator =================================================

constructor TD2AniIndicator.Create(AOwner:TComponent);
begin
  inherited;
  FLayout:=TD2VisualObject.Create(Self);
  FLayout.Parent:=Self;
  FLayout.Align:=vaContents;
  FLayout.Locked:=true;
  FLayout.Stored:=false;
  FAni:=TD2FloatAnimation.Create(Self);
  FAni.Parent:=FLayout;
  FAni.Loop:=true;
  FAni.StartValue:=0;
  FAni.StopValue:=360;
  FAni.Duration:=10;
  FAni.PropertyName:='RotateAngle';
end;

destructor TD2AniIndicator.Destroy;
begin
  inherited;
end;

procedure TD2AniIndicator.Paint;
var
  a:integer;
  P,P2:TD2Point;
  wSize,eSize:single;
  V:single;
begin
  if Width < Height then
    wSize:=Width / 2
  else
    wSize:=Height / 2;
  eSize:=wSize / 3.7;
  wSize:=wSize - eSize;

  case FStyle of
    d2AniIndicatorLine:
      begin
        Canvas.Stroke.Style:=d2Brushsolid;
        Canvas.StrokeThickness:=eSize / 2;
        for a:=0 to 11 do
        begin
          P:=d2Point(Width / 2 + (cos(d2DegToRad(a * 30)) * wSize),Height / 2 + (sin(d2DegToRad(a * 30)) * wSize));
          P2:=d2Point(Width / 2 + (cos(d2DegToRad(a * 30)) * (wSize / 2)),Height / 2 + (sin(d2DegToRad(a * 30)) * (wSize / 2)));
          Canvas.Fill.SolidColor:=$FFBABABA;
          Canvas.Stroke.SolidColor:=$FFBABABA;
          Canvas.DrawLine(P,P2,Opacity);
          if FEnabled then
          begin
            V:=((Trunc(FLayout.RotateAngle) + (30 - Trunc((a / 12) * 30))) mod 30) / 30;
            if V > 1 then V:=Abs(V - 2);
            V:=1 - V;
            Canvas.Stroke.SolidColor:=$FF000000;
            Canvas.DrawLine(P,P2,V * Opacity);
          end;
        end;
      end;
    d2AniIndicatorCircle:
      begin
        Canvas.Stroke.Style:=d2BrushNone;
        for a:=0 to 7 do
        begin
          P:=d2Point(Width / 2 + (cos(d2DegToRad(a * 45)) * wSize),Height / 2 + (sin(d2DegToRad(a * 45)) * wSize));
          Canvas.Fill.SolidColor:=$FFBABABA;
          Canvas.FillEllipse(d2Rect(P.X - eSize,P.Y - eSize,P.X + eSize,P.Y + eSize),Opacity);
          if FEnabled then
          begin
            V:=((Trunc(FLayout.RotateAngle) + (30 - Trunc((a / 7) * 30))) mod 30) / 30;
            if V > 1 then V:=Abs(V - 2);
            V:=1 - V;
            Canvas.Fill.SolidColor:=$FF000000;
            Canvas.FillEllipse(d2Rect(P.X - eSize,P.Y - eSize,P.X + eSize,P.Y + eSize),V * Opacity);
          end;
        end;
      end;
  end;
end;

procedure TD2AniIndicator.SetEnabled(const Value:boolean);
begin
  if FEnabled <> Value then
  begin
    FEnabled:=Value;
    if FEnabled then
    begin
      FAni.Start;
    end
    else
      FAni.Stop;
  end;
end;

procedure TD2AniIndicator.SetStyle(const Value:TD2AniIndicatorStyle);
begin
  if FStyle <> Value then
  begin
    FStyle:=Value;
    Repaint;
  end;
end;    


//=============== TD2ProgressBar ============================================

constructor TD2ProgressBar.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  FMax:=100;
  Width:=100;
  Height:=20;
end;

destructor TD2ProgressBar.Destroy;
begin
  inherited;
end;

function TD2ProgressBar.GetData:Variant;
begin
  Result:=Value;
end;

procedure TD2ProgressBar.SetData(const Value:Variant);
begin
  if VarIsNumeric(Value) then
    Self.Value:=Value
  else
    Self.Value:=Min
end;

procedure TD2ProgressBar.ApplyStyle;
var
  I:TD2Object;
begin
  inherited;
  if Orientation=d2Horizontal then
  begin
    I:=FindResource('hindicator');
    if (I <> nil) and (I is TD2VisualObject) then
      TD2VisualObject(I).StartTriggerAnimation(Self,'IsVisible');
  end
  else
  begin
    I:=FindResource('vindicator');
    if (I <> nil) and (I is TD2VisualObject) then
      TD2VisualObject(I).StartTriggerAnimation(Self,'IsVisible');
  end;
  Realign;
end;

procedure TD2ProgressBar.Realign;
var
  hI,vI,T:TD2Object;
begin
  if not FDisableAlign then
  begin
    FDisableAlign:=true;
    T:=nil;
    if Orientation=d2Horizontal then
    begin
      T:=FindResource('vtrack');
      if T <> nil then
        T.Visual.Visible:=false;
      T:=FindResource('htrack')
    end
    else
    begin
      T:=FindResource('htrack');
      if T <> nil then
        T.Visual.Visible:=false;
      T:=FindResource('vtrack');
    end;
    if T=nil then
      T:=FindResource('track');
    if (T <> nil) and (T is TD2VisualObject) and (Max > Min) then
    begin
      hI:=FindResource('hindicator');
      vI:=FindResource('vindicator');
      if Orientation=d2Horizontal then
      begin
        if (hI <> nil) and (hI is TD2VisualObject) then
        begin
          TD2VisualObject(hI).Width:=((Value - Min) / (Max - Min)) * (TD2VisualObject(T).Width - TD2VisualObject(T).Margins.Left - TD2VisualObject(T).Margins.Right -
            TD2VisualObject(hI).Padding.Left - TD2VisualObject(hI).Padding.Right);
          TD2VisualObject(hI).Visible:=TD2VisualObject(hI).Width > 2;
        end;
        if (vI <> nil) and (vI is TD2VisualObject) then
          TD2VisualObject(vI).Visible:=false;
      end
      else
      begin
        if (vI <> nil) and (vI is TD2VisualObject) then
        begin
          TD2VisualObject(vI).Height:=((Value - Min) / (Max - Min)) * (TD2VisualObject(T).Height - TD2VisualObject(T).Margins.Top - TD2VisualObject(T).Margins.Bottom -
            TD2VisualObject(hI).Padding.Top - TD2VisualObject(hI).Padding.Bottom);
          TD2VisualObject(vI).Visible:=TD2VisualObject(vI).Height > 2;
        end;
        if (hI <> nil) and (hI is TD2VisualObject) then
          TD2VisualObject(hI).Visible:=false;
      end;
    end;
    FDisableAlign:=false;
  end;
  inherited;
end;

procedure TD2ProgressBar.SetMax(const Value:single);
begin
  FMax:=Value;
end;

procedure TD2ProgressBar.SetMin(const Value:single);
begin
  FMin:=Value;
end;

procedure TD2ProgressBar.SetOrientation(const Value:TD2Orientation);
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

procedure TD2ProgressBar.SetValue(const Value:single);
begin
  if FValue <> Value then
  begin
    FValue:=Value;
    Realign;
  end;
end;
      
           
                    
