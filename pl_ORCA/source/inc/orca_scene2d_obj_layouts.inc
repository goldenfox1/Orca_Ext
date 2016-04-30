{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Layout =============================================================

constructor TD2Layout.Create(AOwner:TComponent);
begin
  inherited;
  HitTest:=false;
end;

destructor TD2Layout.Destroy;
begin
  inherited;
end;

procedure TD2Layout.Paint;
var
  R:TD2Rect;
begin
  if Assigned(Scene) and Scene.GetDesignTime and not Locked and not FInPaintTo then
  begin
    R:=LocalRect;
    d2InflateRect(R,-0.5,-0.5);
    Canvas.StrokeThickness:=1;
    Canvas.StrokeDash:=d2DashDash;
    Canvas.Stroke.Style:=d2BrushSolid;
    Canvas.Stroke.SolidColor:=$A0909090;
    Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
    Canvas.StrokeDash:=d2DashSolid;
  end;
end;


//=============== TD2GridLayout =========================================================

constructor TD2GridLayout.Create(AOwner:TComponent);
begin
  inherited;
  ItemHeight:=64;
  ItemWidth:=64;
end;

procedure TD2GridLayout.Realign;
var
  i:integer;
  CurPos:TD2Point;
begin
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  CurPos:=d2Point(Margins.Left,Margins.Top);
  for i:=0 to ChildrenCount - 1 do
    if Children[i].IsVisual then
      with Children[i].Visual do
      begin
        SetBounds(CurPos.X + Padding.Left,CurPos.Y + Padding.Top,FItemWidth - Padding.Left - Padding.right,
          FItemHeight - Padding.top - Padding.bottom);
        if Orientation=d2Horizontal then
        begin
          CurPos.X:=CurPos.X + FItemWidth;
          if CurPos.X + FItemWidth > Self.Width - Self.Margins.Left - Self.Margins.Right then
          begin
            CurPos.X:=Self.Margins.Left;
            CurPos.Y:=CurPos.Y + FItemHeight;
          end;
        end else
        begin
          CurPos.Y:=CurPos.Y + FItemHeight;
          if CurPos.Y + FItemHeight > Self.Height - Self.Margins.Top - Self.Margins.Bottom then
          begin
            CurPos.Y:=Self.Margins.Top;
            CurPos.X:=CurPos.X + FItemWidth;
          end;
        end;
      end;
  FDisableAlign:=false;
end;

procedure TD2GridLayout.SetItemHeight(const Value:single);
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    Realign;
  end;
end;

procedure TD2GridLayout.SetItemWidth(const Value:single);
begin
  if FItemWidth <> Value then
  begin
    FItemWidth:=Value;
    Realign;
  end;
end;

procedure TD2GridLayout.SetOrientation(const Value:TD2Orientation);
begin
  if FOrientation <> Value then
  begin
    FOrientation:=Value;
    Realign;
  end;
end;

//=============== TD2ScaledLayout ================================================

constructor TD2ScaledLayout.Create(AOwner:TComponent);
begin
  inherited;
  FOriginalWidth:=Width;
  FOriginalHeight:=Height;
end;

destructor TD2ScaledLayout.Destroy;
begin
  inherited;
end;

procedure TD2ScaledLayout.Realign;
begin
  if (Parent <> nil) and (Parent is TD2ScrollBox) and (TD2ScrollBox(Parent).FUpdating > 0) then Exit;
  inherited;
  if not (Assigned(FScene) and (FScene.GetDesignTime)) then
  begin
    RecalcAbsolute;
    FRecalcUpdateRect:=true;
  end;
end;

function TD2ScaledLayout.GetChildrenMatrix:TD2Matrix;
begin
  if (Assigned(FScene) and (FScene.GetDesignTime)) then
  begin
    OriginalHeight:=Height;
    OriginalWidth:=Width;
  end;
  Result:=IdentityMatrix;
  Result.m11:=Width / FOriginalWidth;
  Result.m22:=Height / FOriginalHeight;
end;

procedure TD2ScaledLayout.Paint;
var
  R:TD2Rect;
begin
  if Assigned(Scene) and Scene.GetDesignTime and not Locked and not FInPaintTo then
  begin
    R:=LocalRect;
    d2InflateRect(R,-0.5,-0.5);
    Canvas.StrokeThickness:=1;
    Canvas.StrokeDash:=d2DashDash;
    Canvas.Stroke.Style:=d2BrushSolid;
    Canvas.Stroke.SolidColor:=$A0909090;
    Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
    Canvas.StrokeDash:=d2DashSolid;
  end;
  inherited ;
end;

procedure TD2ScaledLayout.SetOriginalHeight(const Value:single);
begin
  if FOriginalHeight <> Value then
  begin
    FOriginalHeight:=Value;
    if FOriginalHeight < 1 then
      FOriginalHeight:=1;
    RecalcAbsolute;
  end;
end;

procedure TD2ScaledLayout.SetOriginalWidth(const Value:single);
begin
  if FOriginalWidth <> Value then
  begin
    FOriginalWidth:=Value;
    if FOriginalWidth < 1 then
      FOriginalWidth:=1;
    RecalcAbsolute;
  end;
end;

procedure TD2ScaledLayout.SetHeight(const Value:single);
begin
  inherited;
  if (Assigned(FScene) and (FScene.GetDesignTime)) then
    OriginalHeight:=Height
  else
    RecalcAbsolute;
end;

procedure TD2ScaledLayout.SetWidth(const Value:single);
begin
  inherited;
  if (Assigned(FScene) and (FScene.GetDesignTime)) then
    OriginalWidth:=Width
  else
    RecalcAbsolute;
end;


//=============== TD2SplitLayout ==================================================

constructor TD2SplitLayout.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2SplitLayout.Destroy;
begin
  inherited;
end;

//=============== TD2Nond2Layout =================================================

constructor TD2Nond2Layout.Create(AOwner:TComponent);
begin
  inherited;
  FVisibleTimer:=TD2Timer.Create(Self);
  FVisibleTimer.Enabled:=false;
  FVisibleTimer.Interval:=100;
  FVisibleTimer.OnTimer:=DoVisibleTimer;
end;

destructor TD2Nond2Layout.Destroy;
begin
  FVisibleTimer.Free;
  inherited;
end;

procedure TD2Nond2Layout.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FControl) then
    FControl:=nil;
end;

function TD2Nond2Layout.CheckParentVisible:boolean;
begin
  Result:=inherited CheckParentVisible;
  if (FControl=nil) then Exit;
  if (Result <> FControl.Visible) then
  begin
    FControl.Visible:=Result;
    FVisibleTimer.Enabled:=FControl.Visible;
    if (FControl.Visible) then Realign;
  end;
end;

procedure TD2Nond2Layout.Realign;
var
  R:TD2Rect;
begin
  inherited;
  if (FControl <> nil) and (FControl.Visible) then
  begin
    R:=AbsoluteRect;
    FControl.SetBounds(round(R.Left),round(R.Top),round(d2RectWidth(R)),round(d2RectHeight(R)));
  end;
end;

procedure TD2Nond2Layout.MatrixChanged(Sender:TObject);
var
  R:TD2Rect;
begin
  inherited;
  if (FControl <> nil) and (FControl.Visible) then
  begin
    R:=AbsoluteRect;
    FControl.SetBounds(round(R.Left),round(R.Top),round(d2RectWidth(R)),round(d2RectHeight(R)));
  end;
end;

procedure TD2Nond2Layout.SetControl(const Value:TControl);
begin
  if FControl <> Value then
  begin
    if Assigned(FScene) and (FControl=FScene.GetComponent) then Exit;
    FControl:=Value;
    Realign;
  end;
end;

procedure TD2Nond2Layout.DoVisibleTimer(Sender:TObject);
begin
  CheckParentVisible;
end;
         


            
  
