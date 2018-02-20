{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2SelectionItem ================================

constructor TD2SelectionItem.Create(AOwner:TComponent);
begin
  inherited;
  HitTest:=false;
end;

procedure TD2SelectionItem.Paint;
begin
  inherited;
end;

procedure TD2SelectionItem.PaintChildren;
begin
  if csDesigning in ComponentState then Exit;
  inherited;
end;  

//=============== TD2Selection ===========================================

constructor TD2Selection.Create(AOwner:TComponent);
begin
  inherited;
  AutoCapture:=true;
  ParentBounds:=true;
  FMinSize:=15;
  FGripSize:=3;
  FSizingHoriz:=true;
  FSizingVert:=true;
end;

destructor TD2Selection.Destroy;
begin
  inherited;
end;

function TD2Selection.GetAbsoluteRect:TD2Rect;
begin
  Result:=inherited GetAbsoluteRect;
  d2InflateRect(Result,FGripSize + 4,FGripSize + 4);
end;

procedure TD2Selection.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
var
  R:TD2Rect;
begin
  inherited;
  FDownPos:=d2Point(X,Y);
  if Button=mbLeft then
  begin
    FRatio:=Width / Height;
    R:=LocalRect;
    R:=d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize));
    if d2PtInRect(FDownPos,R) then
    begin
      FLeftTop:=true;
      Exit;
    end;
    R:=LocalRect;
    R:=d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize));
    if d2PtInRect(FDownPos,R) then
    begin
      FRightTop:=true;
      Exit;
    end;
    R:=LocalRect;
    R:=d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize));
    if d2PtInRect(FDownPos,R) then
    begin
      FRightBottom:=true;
      Exit;
    end;
    R:=LocalRect;
    R:=d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize));
    if d2PtInRect(FDownPos,R) then
    begin
      FLeftBottom:=true;
      Exit;
    end;
    FMove:=true;
  end;
end;

procedure TD2Selection.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  P,{OldPos,}Size:TD2Point;
  {S,}SaveW:single;
  R:TD2Rect;
begin
  inherited;
  if Shift=[] then
  begin
    FMovePos:=d2Point(X,Y);
    P:=LocalToAbsolute(FMovePos);
    if Parent <> nil then
      P:=TD2VisualObject(Parent).AbsolutetoLocal(P);
    R:=LocalRect;
    R:=d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize));
    if d2PtInRect(FMovePos,R) and not FLeftTopHot then
    begin
      FLeftTopHot:=true;
      Repaint;
    end
    else
      if not d2PtInRect(FMovePos,R) and FLeftTopHot then
      begin
        FLeftTopHot:=false;
        Repaint;
      end;
    R:=LocalRect;
    R:=d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize));
    if d2PtInRect(FMovePos,R) and not FRightTopHot then
    begin
      FRightTopHot:=true;
      Repaint;
    end
    else
      if not d2PtInRect(FMovePos,R) and FRightTopHot then
      begin
        FRightTopHot:=false;
        Repaint;
      end;
    R:=LocalRect;
    R:=d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize));
    if d2PtInRect(FMovePos,R) and not FRightBottomHot then
    begin
      FRightBottomHot:=true;
      Repaint;
    end
    else
      if not d2PtInRect(FMovePos,R) and FRightBottomHot then
      begin
        FRightBottomHot:=false;
        Repaint;
      end;
    R:=LocalRect;
    R:=d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize));
    if d2PtInRect(FMovePos,R) and not FLeftBottomHot then
    begin
      FLeftBottomHot:=true;
      Repaint;
    end
    else
      if not d2PtInRect(FMovePos,R) and FLeftBottomHot then
      begin
        FLeftBottomHot:=false;
        Repaint;
      end;
  end;
  if ssLeft in Shift then
  begin
    FMovePos:=d2Point(X,Y);
    if FMove then
    begin
      Position.X:=Position.X + (FMovePos.X - FDownPos.X);
      Position.Y:=Position.Y + (FMovePos.Y - FDownPos.Y);
      if ParentBounds then
      begin
        if Position.X < 0 then Position.X:=0;
        if Position.Y < 0 then Position.Y:=0;
        if Position.X + Width > TD2VisualObject(Parent).Width then Position.X:=TD2VisualObject(Parent).Width - Width;
        if Position.Y + Height > TD2VisualObject(Parent).Height then Position.Y:=TD2VisualObject(Parent).Height - Height;
      end;
      if Assigned(FOnTrack) then FOnTrack(Self);
      Exit;
    end;
    //OldPos:=Position.Point;
    P:=LocalToAbsolute(FMovePos);
    if Parent <> nil then
      P:=TD2VisualObject(Parent).AbsolutetoLocal(P);
    if ParentBounds then
    begin
      if P.Y < 0 then P.Y:=0;
      if P.X < 0 then P.X:=0;
      if P.X > TD2VisualObject(Parent).Width then
        P.X:=TD2VisualObject(Parent).Width;
      if P.Y > TD2VisualObject(Parent).Height then
        P.Y:=TD2VisualObject(Parent).Height;
    end;
    if FLeftTop then
    begin
      Repaint;
      Size:=d2Point((Position.X - (P.X + FDownPos.X)),(Position.Y - (P.Y + FDownPos.Y)));
      if Parent <> nil then
        Size:=d2PointFromVector(TD2VisualObject(Parent).LocalToAbsoluteVector(d2Vector(Size)));
      Size:=d2PointFromVector(AbsoluteToLocalVector(d2Vector(Size)));

      if FProportional then
      begin
        Width:=Width + Size.X;
        SaveW:=Width;
        if Width < FMinSize then Width:=FMinSize;
        if Width / FRatio < FMinSize then
          Width:=Round(FMinSize * FRatio);
        Position.X:=P.X + FDownPos.X - (Width - SaveW);
        Position.Y:=Position.Y + (Height - Round(Width / FRatio));
        Height:=Round(Width / FRatio);
      end
      else
      begin
        if FSizingHoriz then
          begin
            Width:=Width + Size.X;
            Position.X:=P.X + FDownPos.X;
            if Width < FMinSize then
              begin
                Position.X:=Position.X-(FMinSize-Width);
                Width:=FMinSize;
              end;
          end;
        if FSizingVert then
          begin
            Height:=Height + Size.Y;
            Position.Y:=P.Y + FDownPos.Y;
            if Height < FMinSize then
              begin
                Position.Y:=Position.Y-(FMinSize-Height);
                Height:=FMinSize;
              end;
          end;
      end;
      if Assigned(FOnTrack) then FOnTrack(Self);
      Repaint;
    end;
    if FRightTop then
    begin
      Repaint;
      Size:=d2Point((P.X{ + FDownPos.X}) - Position.X,(Position.Y - (P.Y + FDownPos.Y)));
      if Parent <> nil then
        Size:=d2PointFromVector(TD2VisualObject(Parent).LocalToAbsoluteVector(d2Vector(Size)));
      Size:=d2PointFromVector(AbsoluteToLocalVector(d2Vector(Size)));

      if FSizingHoriz then
        Width:=Size.X;
      if FProportional then
      begin
        if Width < FMinSize then Width:=FMinSize;
        if Width / FRatio < FMinSize then
          Width:=Round(FMinSize * FRatio);
        Position.Y:=Position.Y + (Height - Round(Width / FRatio));
        Height:=Round(Width / FRatio);
      end
      else
      begin
        if FSizingHoriz then
          begin
            if Width < FMinSize then Width:=FMinSize;
          end;
        if FSizingVert then
          begin
            Position.Y:=P.Y + FDownPos.Y;
            Height:=Height + Size.Y;
            if Height < FMinSize then
              begin
                Position.Y:=Position.Y-(FMinSize-Height);
                Height:=FMinSize;
              end;
          end;
      end;
      if Assigned(FOnTrack) then FOnTrack(Self);
      Repaint;
    end;
    if FRightBottom then
    begin
      Repaint;
      Size:=d2Point((P.X{ + FDownPos.X}) - Position.X,(P.Y{ + FDownPos.Y)}) - Position.Y);
      if Parent <> nil then
        Size:=d2PointFromVector(TD2VisualObject(Parent).LocalToAbsoluteVector(d2Vector(Size)));
      Size:=d2PointFromVector(AbsoluteToLocalVector(d2Vector(Size)));

      if FSizingHoriz then
        Width:=Size.X;
      if FProportional then
      begin
        if Width < FMinSize then Width:=FMinSize;
        if Width / FRatio < FMinSize then
          Width:=Round(FMinSize * FRatio);
        Height:=Round(Width / FRatio);
      end
      else
      begin
        if FSizingHoriz then
          begin
            if Width < FMinSize then Width:=FMinSize;
          end;
        if FSizingVert then
          begin
            Height:=Size.Y;
            if Height < FMinSize then Height:=FMinSize;
          end;
      end;
      if Assigned(FOnTrack) then FOnTrack(Self);
      Repaint;
    end;
    if FLeftBottom then
    begin
      Repaint;
      Size:=d2Point((Position.X - (P.X + FDownPos.X)),(P.Y{ + FDownPos.Y)}) - Position.Y);
      if Parent <> nil then
        Size:=d2PointFromVector(TD2VisualObject(Parent).LocalToAbsoluteVector(d2Vector(Size)));
      Size:=d2PointFromVector(AbsoluteToLocalVector(d2Vector(Size)));

      if FProportional then
      begin
        Width:=Width + Size.X;
        SaveW:=Width;
        if Width < FMinSize then Width:=FMinSize;
        if Width / FRatio < FMinSize then
          Width:=Round(FMinSize * FRatio);
        Position.X:=P.X + FDownPos.X - (Width - SaveW);
        Height:=Round(Width / FRatio);
      end
      else
      begin
        if FSizingHoriz then
          begin
            Width:=Width + Size.X;
            Position.X:=P.X + FDownPos.X;
            if Width < FMinSize then
              begin
                Position.X:=Position.X-(FMinSize-Width);
                Width:=FMinSize;
              end;
          end;
        if FSizingVert then
          begin
            Height:=Size.Y;
            if Height < FMinSize then
              begin
                Height:=FMinSize;
                Position.Y:=Position.Y-(FMinSize-Height);
              end;
          end;
      end;
      if Assigned(FOnTrack) then FOnTrack(Self);
      Repaint;
    end;
  end;
end;

function TD2Selection.PointInObject(X,Y:single):boolean;
var
  R:TD2Rect;
  P:TD2Point;
begin
  Result:=inherited PointInObject(X,Y);
  if not Result then
  begin
    P:=AbsoluteToLocal(d2Point(X,Y));
    R:=LocalRect;
    R:=d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize));
    if d2PtInRect(P,R) then
    begin
      Result:=true;
      Exit;
    end;
    R:=LocalRect;
    R:=d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize));
    if d2PtInRect(P,R) then
    begin
      Result:=true;
      Exit;
    end;
    R:=LocalRect;
    R:=d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize));
    if d2PtInRect(P,R) then
    begin
      Result:=true;
      Exit;
    end;
    R:=LocalRect;
    R:=d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize));
    if d2PtInRect(P,R) then
    begin
      Result:=true;
      Exit;
    end;
  end;
end;

procedure TD2Selection.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if FMove or FLeftTop or FLeftBottom or FRightTop or FRightBottom then
  begin
     if Assigned(FOnChange) then FOnChange(Self);
  end;
  FMove:=false;
  FLeftTop:=false;
  FLeftBottom:=false;
  FRightTop:=false;
  FRightBottom:=false;
end;

procedure TD2Selection.Paint;
var
  R:TD2Rect;
begin
  if FHideSelection then Exit;
  R:=LocalRect;
  d2InflateRect(R,-0.5,-0.5);
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.StrokeThickness:=1;
  Canvas.Stroke.Style:=d2BrushSolid;
  Canvas.Stroke.SolidColor:=$FF1072C5;
  Canvas.StrokeDash:=d2DashDash;
  Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
  Canvas.StrokeDash:=d2DashSolid;
  { angles }
  R:=LocalRect;
  d2InflateRect(R,-0.5,-0.5);
  if FLeftTopHot then
    Canvas.Fill.SolidColor:=$FFFF0000
  else
    Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.Fillellipse(d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize)),AbsoluteOpacity);
  Canvas.DrawEllipse(d2Rect(R.Left - (GripSize),R.Top - (GripSize),R.Left + (GripSize),R.Top + (GripSize)),AbsoluteOpacity);
  R:=LocalRect;
  if FRightTopHot then
    Canvas.Fill.SolidColor:=$FFFF0000
  else
    Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.Fillellipse(d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize)),AbsoluteOpacity);
  Canvas.DrawEllipse(d2Rect(R.Right - (GripSize),R.Top - (GripSize),R.Right + (GripSize),R.Top + (GripSize)),AbsoluteOpacity);
  R:=LocalRect;
  if FLeftBottomHot then
    Canvas.Fill.SolidColor:=$FFFF0000
  else
    Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.Fillellipse(d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize)),AbsoluteOpacity);
  Canvas.DrawEllipse(d2Rect(R.Left - (GripSize),R.Bottom - (GripSize),R.Left + (GripSize),R.Bottom + (GripSize)),AbsoluteOpacity);
  R:=LocalRect;
  if FRightBottomHot then
    Canvas.Fill.SolidColor:=$FFFF0000
  else
    Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.FillEllipse(d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize)),AbsoluteOpacity);
  Canvas.DrawEllipse(d2Rect(R.Right - (GripSize),R.Bottom - (GripSize),R.Right + (GripSize),R.Bottom + (GripSize)),AbsoluteOpacity);
end;

procedure TD2Selection.MouseLeave;
begin
  inherited;
  FLeftTopHot:=false;
  FLeftBottomHot:=false;
  FRightTopHot:=false;
  FRightBottomHot:=false;
  Repaint;
end;

procedure TD2Selection.SetHideSelection(const Value:boolean);
begin
  if FHideSelection <> Value then
  begin
    FHideSelection:=Value;
    Repaint;
  end;
end;

procedure TD2Selection.SetMinSize(const Value:integer);
begin
  if FMinSize <> Value then
  begin
    FMinSize:=Value;
    if FMinSize < 1 then FMinSize:=1;
  end;
end;

procedure TD2Selection.SetGripSize(const Value:single);
begin
  if FGripSize <> Value then
  begin
    FGripSize:=Value;
    if FGripSize > 20 then
      FGripSize:=20;
    if FGripSize < 1 then
      FGripSize:=1;
    Repaint;
  end;
end;

//=============== TD2SelectionPoint ===============================================

constructor TD2SelectionPoint.Create(AOwner:TComponent);
begin
  inherited;
  AutoCapture:=true;
  DisableDesignResize:=true;
  ParentBounds:=true;
  FGripSize:=3;
  Width:=FGripSize * 2;
  Height:=FGripSize * 2;
end;

destructor TD2SelectionPoint.Destroy;
begin
  inherited;
end;

function TD2SelectionPoint.PointInObject(X,Y:single):boolean;
var
  P:TD2Point;
begin
  Result:=false;
  P:=AbsoluteToLocal(d2Point(X,Y));
  if (Abs(P.X) < GripSize) and (Abs(P.Y) < GripSize) then
  begin
    Result:=true;
  end;
end;

procedure TD2SelectionPoint.Paint;
begin
  inherited;
  Canvas.StrokeThickness:=1;
  Canvas.Stroke.Style:=d2BrushSolid;
  Canvas.Stroke.SolidColor:=$FF1072C5;
  Canvas.Fill.Style:=d2BrushSolid;
  if IsMouseOver then
    Canvas.Fill.SolidColor:=$FFFF0000
  else
    Canvas.Fill.SolidColor:=$FFFFFFFF;
  if Assigned(FScene) and (FScene.GetSelected=Self) then
    Canvas.Fill.SolidColor:=$FFFF0000;

  Canvas.FillEllipse(d2Rect(- (GripSize),- (GripSize),(GripSize),(GripSize)),AbsoluteOpacity);
  Canvas.DrawEllipse(d2Rect(- (GripSize),- (GripSize),(GripSize),(GripSize)),AbsoluteOpacity);
end;

procedure TD2SelectionPoint.SetHeight(const Value:single);
begin
  inherited SetHeight(FGripSize * 2);
end;

procedure TD2SelectionPoint.SetWidth(const Value:single);
begin
  inherited SetWidth(FGripSize * 2);
end;

procedure TD2SelectionPoint.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if Button=mbLeft then
    FPressed:=true;
end;

procedure TD2SelectionPoint.MouseMove(Shift:TShiftState; X,Y,Dx,
  Dy:single);
var
  P:TD2Point;
begin
  inherited;
  if FPressed and (Parent <> nil) then
  begin
    P:=LocalToAbsolute(d2Point(X,Y));
    P:=Parent.Visual.AbsoluteToLocal(P);
    if ParentBounds then
    begin
      if P.X < 0 then P.X:=0;
      if P.Y < 0 then P.Y:=0;
      if P.X > Parent.Visual.Width then P.X:=Parent.Visual.Width;
      if P.Y > Parent.Visual.Height then P.Y:=Parent.Visual.Height;
    end;
    Position.X:=P.X;
    Position.Y:=P.Y;
    if Assigned(FOnTrack) then FOnTrack(Self);
  end;
end;

procedure TD2SelectionPoint.MouseUp(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if FPressed then
  begin
    if Assigned(FOnChange) then FOnChange(Self);
  end;
  FPressed:=false;
end;

procedure TD2SelectionPoint.MouseEnter;
begin
  inherited;
  Repaint;
end;

procedure TD2SelectionPoint.MouseLeave;
begin
  inherited;
  Repaint;
end;

function TD2SelectionPoint.GetUpdateRect:TD2Rect;
begin
  Result:=inherited GetUpdateRect;
  d2InflateRect(Result,GripSize + 1,Gripsize + 1);
end;

procedure TD2SelectionPoint.SetGripSize(const Value:single);
begin
  if FGripSize <> Value then
  begin
    FGripSize:=Value;
    if FGripSize > 20 then
      FGripSize:=20;
    if FGripSize < 1 then
      FGripSize:=1;
    Repaint;
  end;
end;
            
             
                                 
