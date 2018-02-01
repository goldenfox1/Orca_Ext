{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Shape ===============================================

constructor TD2Shape.Create(AOwner:TComponent);
begin
  inherited;
  FFill:=TD2Brush.Create(d2BrushSolid,$FFE0E0E0);
  FFill.OnChanged:=FillChanged;
  FStroke:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FStroke.SolidColor:=$FF000000;
  FStroke.OnChanged:=StrokeChanged;
  FStrokeThickness:=1;
end;

destructor TD2Shape.Destroy;
begin
  FStroke.Free;
  FFill.Free;
  inherited;
end;

function TD2Shape.GetShapeRect:TD2Rect;
begin
  Result:=LocalRect;
  if FStroke.Style <> d2BrushNone then
    if Odd(Round(FStrokeThickness)) then
    begin
      d2InflateRect(Result,-(FStrokeThickness / 2),-(FStrokeThickness / 2));
    end
    else
      d2InflateRect(Result,-(FStrokeThickness / 2),-(FStrokeThickness / 2));
end;

procedure TD2Shape.FillChanged(Sender:TObject);
begin
  if FUpdating=0 then
    Repaint;
  FUpdateEffects:=true;
end;

procedure TD2Shape.StrokeChanged(Sender:TObject);
begin
  if FUpdating=0 then
    Repaint;
  FUpdateEffects:=true;
end;

procedure TD2Shape.BeforePaint;
begin
  inherited;
  Canvas.Fill.Assign(FFill);
  Canvas.Stroke.Assign(FStroke);
  Canvas.StrokeThickness:=FStrokeThickness;
  Canvas.StrokeCap:=FStrokeCap;
  Canvas.StrokeJoin:=FStrokeJoin;
  Canvas.StrokeDash:=FStrokeDash;
end;

procedure TD2Shape.AfterPaint;
begin
  inherited AfterPaint;
  Canvas.StrokeDash:=d2DashSolid;
  Canvas.StrokeThickness:=1;
end;

function TD2Shape.isStrokeThicknessStored:boolean;
begin
  Result:=StrokeThickness <> 1;
end;

procedure TD2Shape.SetFill(const Value:TD2Brush);
begin
  FFill.Assign(Value);
end;

procedure TD2Shape.SetStroke(const Value:TD2Brush);
begin
  FStroke.Assign(Value);
end;

procedure TD2Shape.SetStrokeThickness(const Value:single);
begin
  if FStrokeThickness <> Value then
  begin
    FStrokeThickness:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Shape.SetStrokeCap(const Value:TD2StrokeCap);
begin
  if FStrokeCap <> Value then
  begin
    FStrokeCap:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Shape.SetStrokeJoin(const Value:TD2StrokeJoin);
begin
  if FStrokeJoin <> Value then
  begin
    FStrokeJoin:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Shape.SetStrokeDash(const Value:TD2StrokeDash);
begin
  if FStrokeDash <> Value then
  begin
    FStrokeDash:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;


//=============== TD2Line ================================

constructor TD2Line.Create(AOwner:TComponent);
begin
  inherited;
end;

procedure TD2Line.Paint;
begin
  case FLineType of
    d2LineHorizontal:
      Canvas.DrawLine(GetShapeRect.TopLeft,d2Point(GetShapeRect.Right,GetShapeRect.Top),AbsoluteOpacity);
    d2LineVertical:
      Canvas.DrawLine(GetShapeRect.TopLeft,d2Point(GetShapeRect.Left,GetShapeRect.Bottom),AbsoluteOpacity);
  else
    Canvas.DrawLine(GetShapeRect.TopLeft,GetShapeRect.BottomRight,AbsoluteOpacity);
  end;
end;

procedure TD2Line.SetLineType(const Value:TD2LineType);
begin
  if FLineType <> Value then
  begin
    FLineType:=Value;
    Repaint;
  end;
end;

//=============== TD2Ellipse =================================

procedure TD2Ellipse.Paint;
begin
  Canvas.FillEllipse(GetShapeRect,AbsoluteOpacity);
  Canvas.DrawEllipse(GetShapeRect,AbsoluteOpacity);
end;

function TD2Ellipse.PointInObject(X,Y:single):boolean;
var
  P:TD2Point;
begin
  Result:=false;
  P:=AbsoluteToLocal(d2Point(X,Y));
  if Width * Height=0 then Exit;
  if (Sqr((P.X*2 - Width) / Width) + Sqr((P.Y*2 - Height) / Height) <= 1) then
  begin
    Result:=true;
  end;
end;

//=============== TD2Circle ====================================

procedure TD2Circle.Paint;
var
  R:TD2Rect;
begin
  R:=d2Rect(0,0,d2MaxFloat(Width,Height),d2MaxFloat(Width,Height));
  d2FitRect(R,GetShapeRect);
  Canvas.FillEllipse(R,AbsoluteOpacity);
  Canvas.DrawEllipse(R,AbsoluteOpacity);
end;

//=============== TD2Pie ==============================================

constructor TD2Pie.Create(AOwner:TComponent);
begin
  inherited;
  FStartAngle:=0;
  FEndAngle:=-90;
end;

procedure TD2Pie.Paint;
var
  P:TD2PathData;
begin
  P:=TD2PathData.Create;
  P.MoveTo(d2Point(Width / 2,Height / 2));
  P.AddArc(d2Point(Width / 2,Height / 2),d2Point((Width - StrokeThickness) / 2,(Height - StrokeThickness) / 2),FStartAngle,FEndAngle - FStartAngle);
  P.LineTo(d2Point(Width / 2,Height / 2));
  P.ClosePath;
  Canvas.FillPath(P,P.GetBounds,AbsoluteOpacity);
  Canvas.DrawPath(P,P.GetBounds,AbsoluteOpacity);
  P.Free;
end;

function TD2Pie.PointInObject(X,Y:single):boolean;
var
  P:TD2PathData;
begin
  if (Canvas <> nil) then
  begin
    P:=TD2PathData.Create;
    P.MoveTo(d2Point(Width / 2,Height / 2));
    P.AddArc(d2Point(Width / 2,Height / 2),d2Point((Width - StrokeThickness) / 2,(Height - StrokeThickness) / 2),FStartAngle,FEndAngle - FStartAngle);
    P.LineTo(d2Point(Width / 2,Height / 2));
    P.ClosePath;
    Result:=Canvas.PtInPath(AbsoluteToLocal(d2Point(X,Y)),P.GetBounds,P);
    P.Free;
  end
  else
    Result:=inherited PointInObject(X,Y);
end;

procedure TD2Pie.SetEndAngle(const Value:single);
begin
  if FEndAngle <> Value then
  begin
    FEndAngle:=Value;
    Repaint;
  end;
end;

procedure TD2Pie.SetStartAngle(const Value:single);
begin
  if FStartAngle <> Value then
  begin
    FStartAngle:=Value;
    Repaint;
  end;
end;

//=============== TD2Arc =========================================

constructor TD2Arc.Create(AOwner:TComponent);
begin
  inherited;
  Fill.Style:=d2BrushNone;
  Fill.DefaultStyle:=d2BrushNone;
  FStartAngle:=0;
  FEndAngle:=-90;
end;

procedure TD2Arc.Paint;
begin
  Canvas.FillArc(d2Point(Width / 2,Height / 2),d2Point((Width - StrokeThickness) / 2,((Height - StrokeThickness) / 2)),FStartAngle,FEndAngle,AbsoluteOpacity);
  Canvas.DrawArc(d2Point(Width / 2,Height / 2),d2Point((Width - StrokeThickness) / 2,((Height - StrokeThickness) / 2)),FStartAngle,FEndAngle,AbsoluteOpacity);
end;

function TD2Arc.PointInObject(X,Y:single):boolean;
begin
  Result:=inherited PointInObject(X,Y);
end;

procedure TD2Arc.SetEndAngle(const Value:single);
begin
  if FEndAngle <> Value then
  begin
    FEndAngle:=Value;
    Repaint;
  end;
end;

procedure TD2Arc.SetStartAngle(const Value:single);
begin
  if FStartAngle <> Value then
  begin
    FStartAngle:=Value;
    Repaint;
  end;
end;

//=============== TD2Rectangle =============================================

constructor TD2Rectangle.Create(AOwner:TComponent);
begin
  inherited;
  FCorners:=AllCorners;
  FxRadius:=0;
  FyRadius:=0;
  FSides:=AllSides;
end;

function TD2Rectangle.IsCornersStored:boolean;
begin
  Result:=FCorners <> AllCorners;
end;

function TD2Rectangle.IsSidesStored:boolean;
begin
  Result:=FSides * AllSides <> []
end;

procedure TD2Rectangle.Paint;
var
  R:TD2Rect;
  Off:single;
begin
  R:=GetShapeRect;
  if Sides <> AllSides then
  begin
    Off:=R.Left;
    if not (d2SideTop in FSides) then
      R.Top:=R.Top - Off;
    if not (d2SideLeft in FSides) then
      R.Left:=R.Left - Off;
    if not (d2SideBottom in FSides) then
      R.Bottom:=R.Bottom + Off;
    if not (d2SideRight in FSides) then
      R.Right:=R.Right + Off;
    Canvas.FillRect(R,xRadius,yRadius,FCorners,AbsoluteOpacity,CornerType);
    Canvas.DrawRectSides(GetShapeRect,xRadius,yRadius,FCorners,AbsoluteOpacity,Sides,CornerType);
  end
  else
  begin
    Canvas.FillRect(R,xRadius,yRadius,FCorners,AbsoluteOpacity,CornerType);
    Canvas.DrawRect(R,xRadius,yRadius,FCorners,AbsoluteOpacity,CornerType);
  end;
end;

procedure TD2Rectangle.SetCorners(const Value:TD2Corners);
begin
  if FCorners <> Value then
  begin
    FCorners:=Value;
    Repaint;
  end;
end;

procedure TD2Rectangle.SetCornerType(const Value:TD2CornerType);
begin
  if FCornerType <> Value then
  begin
    FCornerType:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Rectangle.SetxRadius(const Value:single);
begin
  if FxRadius <> Value then
  begin
    FxRadius:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Rectangle.SetyRadius(const Value:single);
begin
  if FyRadius <> Value then
  begin
    FyRadius:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Rectangle.SetSides(const Value:TD2Sides);
begin
  if FSides <> Value then
  begin
    FSides:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

//=============== TD2BlurRectangle ====================================================

constructor TD2BlurRectangle.Create(AOwner:TComponent);
begin
  inherited;
  Stroke.Style:=d2BrushNone;
  FSoftness:=0.3;
  FRecreate:=true;
end;

destructor TD2BlurRectangle.Destroy;
begin
  if FBuffer <> nil then
    FreeAndNil(FBuffer);
  inherited;
end;

procedure TD2BlurRectangle.Paint;
var
  R:TD2Rect;
  Radius:integer;
begin
  Radius:=Trunc(FSoftness * 15);
  if FBuffer=nil then
  begin
    FBuffer:=TD2Bitmap.Create(Round(Width),Round(Height));
    FRecreate:=true;
  end;
  if (FBuffer.Width <> Round(Width)) or (FBuffer.Height <> Round(Height)) then
  begin
    FBuffer.SetSize(Round(Width),Round(Height));
    FRecreate:=true;
  end;

  if FRecreate then
  begin
    R:=d2Rect(Radius,Radius,Width - Radius,Height - Radius);
    FBuffer.Clear(0);
    FBuffer.Canvas.Fill.Assign(Fill);
    FBuffer.Canvas.FillRect(R,xRadius,yRadius,FCorners,AbsoluteOpacity,CornerType);
    FBuffer.Canvas.Stroke.Assign(Stroke);
    FBuffer.Canvas.DrawRect(R,xRadius,yRadius,FCorners,AbsoluteOpacity,CornerType);
    Blur(nil,FBuffer,Radius);
    FRecreate:=false;
  end;
  Canvas.DrawBitmap(FBuffer,LocalRect,LocalRect,AbsoluteOpacity,true);
end;

procedure TD2BlurRectangle.SetSoftness(const Value:single);
begin
  if FSoftness <> Value then
  begin
    FSoftness:=Value;
    FRecreate:=true;
    Repaint;
  end;
end;

procedure TD2BlurRectangle.SetCorners(const Value:TD2Corners);
begin
  if Value <> Corners then
  begin
    inherited;
    FRecreate:=true;
  end;
end;

procedure TD2BlurRectangle.SetCornerType(const Value:TD2CornerType);
begin
  if Value <> CornerType then
  begin
    inherited;
    FRecreate:=true;
  end;
end;

procedure TD2BlurRectangle.SetxRadius(const Value:single);
begin
  if Value <> xRadius then
  begin
    inherited;
    FRecreate:=true;
  end;
end;

procedure TD2BlurRectangle.SetyRadius(const Value:single);
begin
  if Value <> yRadius then
  begin
    inherited;
    FRecreate:=true;
  end;
end;

procedure TD2BlurRectangle.FillChanged(Sender:TObject);
begin
  inherited;
  FRecreate:=true;
end;

//=============== TD2RoundRect ========================================

constructor TD2RoundRect.Create(AOwner:TComponent);
begin
  inherited;
  FCorners:=AllCorners;
end;

function TD2RoundRect.IsCornersStored:boolean;
begin
  Result:=FCorners <> AllCorners;
end;

procedure TD2RoundRect.Paint;
var
  Radius:single;
begin
  if Height < Width then
    Radius:=d2RectHeight(GetShapeRect) / 2
  else
    Radius:=d2RectWidth(GetShapeRect) / 2;
  Canvas.FillRect(GetShapeRect,Radius,Radius,FCorners,AbsoluteOpacity);
  Canvas.DrawRect(GetShapeRect,Radius,Radius,FCorners,AbsoluteOpacity);
end;

procedure TD2RoundRect.SetCorners(const Value:TD2Corners);
begin
  if FCorners <> Value then
  begin
    FCorners:=Value;
    Repaint;
  end;
end;

//=============== TD2BlurRoundRect ==========================================

constructor TD2BlurRoundRect.Create(AOwner:TComponent);
begin
  inherited;
  Stroke.Style:=d2BrushNone;
  FSoftness:=0.3;
  FRecreate:=true;
end;

destructor TD2BlurRoundRect.Destroy;
begin
  if FBuffer <> nil then
    FreeAndNil(FBuffer);
  inherited;
end;

procedure TD2BlurRoundRect.Paint;
var
  R:TD2Rect;
  Radius:integer;
  rectRadius:single;
begin
  Radius:=Trunc(FSoftness * 15);
  if FBuffer=nil then
  begin
    FBuffer:=TD2Bitmap.Create(Round(Width),Round(Height));
    FRecreate:=true;
  end;
  if (FBuffer.Width <> Round(Width)) or (FBuffer.Height <> Round(Height)) then
  begin
    FBuffer.SetSize(Round(Width),Round(Height));
    FRecreate:=true;
  end;

  if FRecreate then
  begin
    R:=d2Rect(Radius,Radius,Width - Radius,Height - Radius);
    rectRadius:=d2RectHeight(R) / 2;
    FBuffer.Clear(0);
    FBuffer.Canvas.Fill.Assign(Fill);
    FBuffer.Canvas.FillRect(R,rectRadius,rectRadius,FCorners,AbsoluteOpacity);
    FBuffer.Canvas.Stroke.Assign(Stroke);
    FBuffer.Canvas.DrawRect(R,rectRadius,rectRadius,FCorners,AbsoluteOpacity);
    Blur(nil,FBuffer,Radius);
    FRecreate:=false;
  end;
  Canvas.DrawBitmap(FBuffer,LocalRect,LocalRect,AbsoluteOpacity,true);
end;

procedure TD2BlurRoundRect.SetSoftness(const Value:single);
begin
  if FSoftness <> Value then
  begin
    FSoftness:=Value;
    FRecreate:=true;
    Repaint;
  end;
end;

procedure TD2BlurRoundRect.SetCorners(const Value:TD2Corners);
begin
  if Value <> Corners then
  begin
    inherited;
    FRecreate:=true;
  end;
end;

procedure TD2BlurRoundRect.FillChanged(Sender:TObject);
begin
  inherited;
  FRecreate:=true;
end;

{ TD2DirectionRectangle }

procedure TD2DirectionRectangle.SetDirectionLength(const Value: single);
begin
  if FDirectionLength <> Value then
  begin
    FDirectionLength:=Value;
    CreatePath;
    Repaint;
  end
end;

procedure TD2DirectionRectangle.SetDirectionLine(Value: boolean);
begin
  if FDirectionLine=Value then Exit;
  FDirectionLine:=Value;
  Repaint;
end;

procedure TD2DirectionRectangle.SetDirectionMark(Value: boolean);
begin
  if FDirectionMark=Value then Exit;
  FDirectionMark:=Value;
  Repaint;
end;

procedure TD2DirectionRectangle.SetDirectionPosition(const Value: TD2DirectionPosition);
begin
  if FDirectionPosition <> Value then
  begin
    FDirectionPosition:=Value;
    CreatePath;
    Repaint;
  end;
end;

procedure TD2DirectionRectangle.CreatePath;
var
  x1,x2,y1,y2:single;
  R:TD2Rect;
  Off:single;
  p1,p2,p3: TD2Point;
begin
  R:=GetShapeRect;
  if DirectionLength > 0 then
    case DirectionPosition of
      d2DirectionTop:
        R.Top:=R.Top + FDirectionLength;
      d2DirectionLeft:
        R.Left:=R.Left + FDirectionLength;
      d2DirectionBottom:
        R.Bottom:=R.Bottom - FDirectionLength;
      d2DirectionRight:
        R.Right:=R.Right - FDirectionLength;
      d2DirectionVertical:
        begin
          R.Top:=R.Top + FDirectionLength;
          R.Bottom:=R.Bottom - FDirectionLength;
        end;
      d2DirectionHorizontal:
        begin
          R.Left:=R.Left + FDirectionLength;
          R.Right:=R.Right - FDirectionLength;
        end;
    end;
  if Sides <> AllSides then
  begin
    Off:=R.Left;
    if not (d2SideTop in FSides) then
      R.Top:=R.Top - Off;
    if not (d2SideLeft in FSides) then
      R.Left:=R.Left - Off;
    if not (d2SideBottom in FSides) then
      R.Bottom:=R.Bottom + Off;
    if not (d2SideRight in FSides) then
      R.Right:=R.Right + Off;
  end;
  x1:=xRadius;
  if d2RectWidth(R) - (x1 * 2) < 0 then
    x1:=(xRadius * (d2RectWidth(R) / (x1 * 2)));
  x2:=x1 / 2;
  y1:=yRadius;
  if d2RectHeight(R) - (y1 * 2) < 0 then
    y1:=(yRadius * (d2RectHeight(R) / (y1 * 2)));
  y2:=y1 / 2;
  FPath.Clear;
  FMarkPath.Clear;
  FPath.MoveTo(d2Point(R.Left,R.Top + y1));
  if d2CornerTopLeft in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:
        FPath.LineTo(d2Point(R.Left + x1,R.Top));
      d2CornerInnerRound:
        FPath.CurveTo(d2Point(R.Left + x2,R.Top + y1),d2Point(R.Left + x1,R.Top + y2),d2Point(R.Left + x1,R.Top));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Left + x2,R.Top + y1));
          FPath.LineTo(d2Point(R.Left + x1,R.Top + y2));
          FPath.LineTo(d2Point(R.Left + x1,R.Top));
        end;
    else
      FPath.CurveTo(d2Point(R.Left,R.Top + (y2)),d2Point(R.Left + x2,R.Top),d2Point(R.Left + x1,R.Top))
    end;
  end
  else
  begin
    if d2SideLeft in FSides then
      FPath.LineTo(d2Point(R.Left,R.Top))
    else
      FPath.MoveTo(d2Point(R.Left,R.Top));
    if d2SideTop in FSides then
      FPath.LineTo(d2Point(R.Left + x1,R.Top))
    else
      FPath.MoveTo(d2Point(R.Left + x1,R.Top));
  end;
  if not (d2SideTop in FSides) then
    FPath.MoveTo(d2Point(R.Right - x1,R.Top))
  else
  begin
    if (FDirectionPosition=d2DirectionTop) or
       (FDirectionPosition=d2DirectionVertical)  then
    begin
      if FDirectionMark and (FDirectionLength>0) then
      begin
        p2:=d2Point((R.Right - R.Left) / 2,R.Top - FDirectionLength); //вершина треугольника
        p3:=d2Point(R.Right - x1,R.Top);
        p3:=d2Point(p2.X + (p3.X - p2.X)/2, p2.Y + (p3.Y - p2.Y)/2);
        p1:=d2Point(p2.X - (p3.X - p2.X), p3.Y);
        FMarkPath.MoveTo(p1);
        FMarkPath.LineTo(p2);
        FMarkPath.LineTo(p3);
        FMarkPath.LineTo(p1);
      end;
      FPath.LineTo(d2Point((R.Right - R.Left) / 2,R.Top - FDirectionLength));
    end;
    FPath.LineTo(d2Point(R.Right - x1,R.Top));
  end;
  if d2CornerTopRight in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Right,R.Top + y1));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Right - x1,R.Top + y2),d2Point(R.Right - x2,R.Top + y1),d2Point(R.Right,R.Top + y1));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Right - x1,R.Top + y2));
          FPath.LineTo(d2Point(R.Right - x2,R.Top + y1));
          FPath.LineTo(d2Point(R.Right,R.Top + y1));
        end;
    else
      FPath.CurveTo(d2Point(R.Right - x2,R.Top),d2Point(R.Right,R.Top + (y2)),d2Point(R.Right,R.Top + y1))
    end;
  end
  else
  begin
    if d2SideTop in FSides then
      FPath.LineTo(d2Point(R.Right,R.Top))
    else
      FPath.MoveTo(d2Point(R.Right,R.Top));
    if d2SideRight in FSides then
      FPath.LineTo(d2Point(R.Right,R.Top + y1))
    else
      FPath.MoveTo(d2Point(R.Right,R.Top + y1));
  end;
  if not (d2SideRight in FSides) then
    FPath.MoveTo(d2Point(R.Right,R.Bottom - y1))
  else
  begin
    if (FDirectionPosition=d2DirectionRight) or
       (FDirectionPosition=d2DirectionHorizontal) then
    begin
      if FDirectionMark and (FDirectionLength>0) then
      begin
        p2:=d2Point(R.Right + FDirectionLength,(R.Bottom - R.Top) / 2); //вершина треугольника
        p3:=d2Point(R.Right,R.Bottom - y1);
        p3:=d2Point(p2.X - (p2.X - p3.X)/2, p2.Y + (p3.Y - p2.Y)/2);
        p1:=d2Point(p3.X, p2.Y-(p3.Y-p2.Y));

        FMarkPath.MoveTo(p1);
        FMarkPath.LineTo(p2);
        FMarkPath.LineTo(p3);
        FMarkPath.LineTo(p1);
      end;
      FPath.LineTo(d2Point(R.Right + FDirectionLength,(R.Bottom - R.Top) / 2));
    end;
    FPath.LineTo(d2Point(R.Right,R.Bottom - y1));
  end;
  if d2CornerBottomRight in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Right - x2,R.Bottom - y1),d2Point(R.Right - x1,R.Bottom - y2),d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Right - x2,R.Bottom - y1));
          FPath.LineTo(d2Point(R.Right - x1,R.Bottom - y2));
          FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
        end;
    else
      FPath.CurveTo(d2Point(R.Right,R.Bottom - (y2)),d2Point(R.Right - x2,R.Bottom),d2Point(R.Right - x1,R.Bottom))
    end;
  end
  else
  begin
    if d2SideRight in FSides then
      FPath.LineTo(d2Point(R.Right,R.Bottom))
    else
      FPath.MoveTo(d2Point(R.Right,R.Bottom));
    if d2SideBottom in FSides then
      FPath.LineTo(d2Point(R.Right - x1,R.Bottom))
    else
      FPath.MoveTo(d2Point(R.Right - x1,R.Bottom));
  end;
  if not (d2SideBottom in FSides) then
    FPath.MoveTo(d2Point(R.Left + x1,R.Bottom))
  else
  begin
    if (FDirectionPosition=d2DirectionBottom) or
       (FDirectionPosition=d2DirectionVertical) then
    begin
      if FDirectionMark and (FDirectionLength>0) then
      begin
        p2:=d2Point((R.Right - R.Left) / 2,R.Bottom + FDirectionLength); //вершина треугольника
        p3:=d2Point(R.Left + x1,R.Bottom);
        p3:=d2Point(p2.X - (p2.X - p3.X)/2, p2.Y - (p2.Y - p3.Y)/2);
        p1:=d2Point(p2.X + (p2.X - p3.X), p3.Y);

        FMarkPath.MoveTo(p1);
        FMarkPath.LineTo(p2);
        FMarkPath.LineTo(p3);
        FMarkPath.LineTo(p1);
      end;
      FPath.LineTo(d2Point((R.Right - R.Left) / 2,R.Bottom + FDirectionLength));
    end;
    FPath.LineTo(d2Point(R.Left + x1,R.Bottom));
  end;
  if d2CornerBottomLeft in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Left + x1,R.Bottom - y2),d2Point(R.Left + x2,R.Bottom - y1),d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Left + x1,R.Bottom - y2));
          FPath.LineTo(d2Point(R.Left + x2,R.Bottom - y1));
          FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
        end;
    else
      FPath.CurveTo(d2Point(R.Left + x2,R.Bottom),d2Point(R.Left,R.Bottom - (y2)),d2Point(R.Left,R.Bottom - y1))
    end;
  end
  else
  begin
    if d2SideBottom in FSides then
      FPath.LineTo(d2Point(R.Left,R.Bottom))
    else
      FPath.MoveTo(d2Point(R.Left,R.Bottom));
    if d2SideLeft in FSides then
      FPath.LineTo(d2Point(R.Left,R.Bottom - y1))
    else
      FPath.MoveTo(d2Point(R.Left,R.Bottom - y1));
  end;
  if (d2SideLeft in FSides) then
  begin
    if (FDirectionPosition=d2DirectionLeft) or
       (FDirectionPosition=d2DirectionHorizontal) then
    begin
      if FDirectionMark and (FDirectionLength > 0) then
      begin
        p2:=d2Point(R.Left - FDirectionLength,(R.Bottom - R.Top) / 2); //вершина треугольника
        p3:=d2Point(R.Left,R.Top + y1);
        p3:=d2Point(p2.X + (p3.X - p2.X)/2, p2.Y - (p2.Y - p3.Y)/2);
        p1:=d2Point(p3.X, p2.Y - (p3.Y - p2.Y));
        FMarkPath.MoveTo(p1);
        FMarkPath.LineTo(p2);
        FMarkPath.LineTo(p3);
        FMarkPath.LineTo(p1);
      end;
      FPath.LineTo(d2Point(R.Left - FDirectionLength,(R.Bottom - R.Top) / 2));
    end;
    FPath.LineTo(d2Point(R.Left,R.Top + y1));
  end;
end;

procedure TD2DirectionRectangle.Paint;
var
  R:TD2Rect;
begin
  CreatePath;
  R:=FPath.GetBounds;
  Canvas.FillPath(FPath,R,AbsoluteOpacity);
  Canvas.DrawPath(FPath,R,AbsoluteOpacity);

  //рисуем линию
  if FDirectionLine then
    case FDirectionPosition of
      d2DirectionBottom:
        Canvas.DrawLine(d2Point(R.Left, R.Bottom-FDirectionLength),d2Point(R.Right, R.Bottom-FDirectionLength),AbsoluteOpacity);
      d2DirectionTop:
        Canvas.DrawLine(d2Point(R.Left, R.Top+FDirectionLength),d2Point(R.Right, R.Top+FDirectionLength),AbsoluteOpacity);
      d2DirectionLeft:
        Canvas.DrawLine(d2Point(R.Left+FDirectionLength, R.Top),d2Point(R.Left+FDirectionLength, R.Bottom),AbsoluteOpacity);
      d2DirectionRight:
        Canvas.DrawLine(d2Point(R.Right-FDirectionLength, R.Top),d2Point(R.Right-FDirectionLength, R.Bottom),AbsoluteOpacity);
      d2DirectionVertical:
        begin
          Canvas.DrawLine(d2Point(R.Left, R.Bottom-FDirectionLength),d2Point(R.Right, R.Bottom-FDirectionLength),AbsoluteOpacity);
          Canvas.DrawLine(d2Point(R.Left, R.Top+FDirectionLength),d2Point(R.Right, R.Top+FDirectionLength),AbsoluteOpacity);
        end;
      d2DirectionHorizontal:
        begin
          Canvas.DrawLine(d2Point(R.Left+FDirectionLength, R.Top),d2Point(R.Left+FDirectionLength, R.Bottom),AbsoluteOpacity);
          Canvas.DrawLine(d2Point(R.Right-FDirectionLength, R.Top),d2Point(R.Right-FDirectionLength, R.Bottom),AbsoluteOpacity);
        end;
    end;
  //рисуем маркер
  if FDirectionMark and (FDirectionLength > 0) then
  begin
    Canvas.Fill.Assign(FStroke);
    Canvas.FillPath(FMarkPath,FMarkPath.GetBounds,AbsoluteOpacity);
  end;

end;

constructor TD2DirectionRectangle.Create(AOwner: TComponent);
begin
  inherited;
  FDirectionLength:=15;
  FPath:=TD2PathData.Create;
  FMarkPath:=TD2PathData.Create;
end;

destructor TD2DirectionRectangle.Destroy;
begin
  FreeAndNil(FMarkPath);
  FreeAndNil(FPath);
  inherited;
end;

//=============== TD2Parallelogram ====================================

procedure TD2Parallelogram.SetShiftSize(const Value: single);
begin
  if FShiftSize=Value then exit;
  FShiftSize:=Value;
  CreatePath;
  Repaint;
end;

procedure TD2Parallelogram.SetShiftDirection(const Value: TD2ShiftDirection);
begin
  if FShiftDirection=Value then Exit;
  FShiftDirection:=Value;
  CreatePath;
  Repaint;
end;

procedure TD2Parallelogram.SetDoubleLine(Value: boolean);
begin
  if FDoubleLine=Value then Exit;
  FDoubleLine:=Value;
  CreatePath;
  Repaint;
end;

procedure TD2Parallelogram.SetDoubleLineIndent(Value: single);
begin
  if FDoubleLineIndent=Value then Exit;
  FDoubleLineIndent:=Value;
  Repaint;
end;

procedure TD2Parallelogram.CreatePath;
var
  x1,x2,y1,y2,x3,y3:single;
  R:TD2Rect;
  Off:single;
begin
  R:=GetShapeRect;
  x1:=xRadius;
  if d2RectWidth(R) - (x1 * 2) < 0 then
    x1:=(xRadius * (d2RectWidth(R) / (x1 * 2)));
  x2:=x1 / 2;
  y1:=yRadius;
  if d2RectHeight(R) - (y1 * 2) < 0 then
    y1:=(yRadius * (d2RectHeight(R) / (y1 * 2)));
  y2:=y1 / 2;
  FPath.Clear;

  case ShiftDirection of
    d2ShiftHorizontal:

      //наклон вправо
      if FShiftSize > 0
        then  begin
                //верхниий левый угол
                FPath.MoveTo(d2Point(R.Left+FShiftSize, R.Top + y1));
                if d2CornerTopLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left+FShiftSize + x1,R.Top));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left+FShiftSize + x2, R.Top + y1),
                                            d2Point(R.Left+FShiftSize + x1, R.Top + y2),
                                            d2Point(R.Left+FShiftSize + x1, R.Top));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left+FShiftSize + x2, R.Top + y1));
                                FPath.LineTo(d2Point(R.Left+FShiftSize + x1, R.Top + y2));
                                FPath.LineTo(d2Point(R.Left+FShiftSize + x1, R.Top));
                              end;
                          else  FPath.CurveTo(d2Point(R.Left+FShiftSize, R.Top + (y2)),
                                              d2Point(R.Left+FShiftSize + x2,R.Top),
                                              d2Point(R.Left+FShiftSize + x1,R.Top))
                          end;
                        end
                  else  begin
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left+FShiftSize,R.Top))
                            else FPath.MoveTo(d2Point(R.Left+FShiftSize,R.Top));
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Left+FShiftSize + x1,R.Top))
                            else FPath.MoveTo(d2Point(R.Left+FShiftSize + x1,R.Top));
                        end;

                //верхняя сторона
                if d2SideTop in FSides
                  then FPath.LineTo(d2Point(R.Right - x1,R.Top))
                  else FPath.MoveTo(d2Point(R.Right - x1,R.Top));

                //верхниий правый угол
                if d2CornerTopRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right,R.Top + y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right - x1,R.Top + y2),
                                            d2Point(R.Right - x2,R.Top + y1),
                                            d2Point(R.Right,R.Top + y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right - x1,R.Top + y2));
                                FPath.LineTo(d2Point(R.Right - x2,R.Top + y1));
                                FPath.LineTo(d2Point(R.Right,R.Top + y1));
                              end;
                          else FPath.CurveTo(d2Point(R.Right - x2,R.Top),
                                             d2Point(R.Right,R.Top + (y2)),
                                             d2Point(R.Right,R.Top + y1))
                          end;
                        end
                  else  begin
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Top))
                            else FPath.MoveTo(d2Point(R.Right,R.Top));
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Top + y1))
                            else FPath.MoveTo(d2Point(R.Right,R.Top + y1));
                        end;

                //правая сторона
                if d2SideRight in FSides
                  then FPath.LineTo(d2Point(R.Right-FShiftSize, R.Bottom - y1))
                  else FPath.MoveTo(d2Point(R.Right-FShiftSize, R.Bottom - y1));

                //нижний правый угол
                if d2CornerBottomRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right-FShiftSize - x1,R.Bottom));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right-FShiftSize - x2,R.Bottom - y1),
                                            d2Point(R.Right-FShiftSize - x1,R.Bottom - y2),
                                            d2Point(R.Right-FShiftSize - x1,R.Bottom));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right-FShiftSize - x2,R.Bottom - y1));
                                FPath.LineTo(d2Point(R.Right-FShiftSize - x1,R.Bottom - y2));
                                FPath.LineTo(d2Point(R.Right-FShiftSize - x1,R.Bottom));
                              end;
                            else FPath.CurveTo(d2Point(R.Right-FShiftSize,R.Bottom - (y2)),
                                               d2Point(R.Right-FShiftSize - x2,R.Bottom),
                                               d2Point(R.Right-FShiftSize - x1,R.Bottom))
                          end;
                        end
                  else  begin
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right-FShiftSize,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Right-FShiftSize,R.Bottom));
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Right-FShiftSize - x1,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Right-FShiftSize - x1,R.Bottom));
                        end;
                //нижняя сторона
                if d2SideBottom in FSides
                  then FPath.LineTo(d2Point(R.Left + x1, R.Bottom))
                  else FPath.MoveTo(d2Point(R.Left + x1, R.Bottom));

                //нижний левый угол
                if d2CornerBottomLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left + x1,R.Bottom - y2),
                                            d2Point(R.Left + x2,R.Bottom - y1),
                                            d2Point(R.Left,R.Bottom - y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left + x1,R.Bottom - y2));
                                FPath.LineTo(d2Point(R.Left + x2,R.Bottom - y1));
                                FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
                              end;
                            else FPath.CurveTo(d2Point(R.Left + x2,R.Bottom),
                                               d2Point(R.Left,R.Bottom - (y2)),
                                               d2Point(R.Left,R.Bottom - y1))
                          end;
                        end
                  else  begin
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Left,R.Bottom));
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Bottom - y1))
                            else FPath.MoveTo(d2Point(R.Left,R.Bottom - y1));
                        end;

                //левая сторона
                if d2SideLeft in FSides
                  then FPath.LineTo(d2Point(R.Left+FShiftSize, R.Top + y1));
              end

        //наклон влево
        else  begin
                //верхниий левый угол
                FPath.MoveTo(d2Point(R.Left, R.Top + y1));
                if d2CornerTopLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left + x1,R.Top));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left + x2, R.Top + y1),
                                            d2Point(R.Left + x1, R.Top + y2),
                                            d2Point(R.Left + x1, R.Top));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left + x2, R.Top + y1));
                                FPath.LineTo(d2Point(R.Left + x1, R.Top + y2));
                                FPath.LineTo(d2Point(R.Left + x1, R.Top));
                              end;
                          else  FPath.CurveTo(d2Point(R.Left, R.Top + (y2)),
                                              d2Point(R.Left + x2,R.Top),
                                              d2Point(R.Left + x1,R.Top))
                          end;
                        end
                  else  begin
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Top))
                            else FPath.MoveTo(d2Point(R.Left,R.Top));
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Left + x1,R.Top))
                            else FPath.MoveTo(d2Point(R.Left+ x1,R.Top));
                        end;

                //верхняя сторона
                if d2SideTop in FSides
                  then FPath.LineTo(d2Point(R.Right+FShiftSize - x1,R.Top))
                  else FPath.MoveTo(d2Point(R.Right+FShiftSize - x1,R.Top));

                //верхниий правый угол
                if d2CornerTopRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right+FShiftSize,R.Top + y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right+FShiftSize - x1,R.Top + y2),
                                            d2Point(R.Right+FShiftSize - x2,R.Top + y1),
                                            d2Point(R.Right+FShiftSize,R.Top + y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right+FShiftSize - x1,R.Top + y2));
                                FPath.LineTo(d2Point(R.Right+FShiftSize - x2,R.Top + y1));
                                FPath.LineTo(d2Point(R.Right+FShiftSize,R.Top + y1));
                              end;
                          else FPath.CurveTo(d2Point(R.Right+FShiftSize - x2,R.Top),
                                             d2Point(R.Right+FShiftSize,R.Top + (y2)),
                                             d2Point(R.Right+FShiftSize,R.Top + y1))
                          end;
                        end
                  else  begin
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Right+FShiftSize,R.Top))
                            else FPath.MoveTo(d2Point(R.Right+FShiftSize,R.Top));
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right+FShiftSize,R.Top + y1))
                            else FPath.MoveTo(d2Point(R.Right+FShiftSize,R.Top + y1));
                        end;

                //правая сторона
                if d2SideRight in FSides
                  then FPath.LineTo(d2Point(R.Right, R.Bottom - y1))
                  else FPath.MoveTo(d2Point(R.Right, R.Bottom - y1));

                //нижний правый угол
                if d2CornerBottomRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right - x2,R.Bottom - y1),
                                            d2Point(R.Right - x1,R.Bottom - y2),
                                            d2Point(R.Right - x1,R.Bottom));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right - x2,R.Bottom - y1));
                                FPath.LineTo(d2Point(R.Right - x1,R.Bottom - y2));
                                FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
                              end;
                            else FPath.CurveTo(d2Point(R.Right,R.Bottom - (y2)),
                                               d2Point(R.Right - x2,R.Bottom),
                                               d2Point(R.Right - x1,R.Bottom))
                          end;
                        end
                  else  begin
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Right,R.Bottom));
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Right - x1,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Right - x1,R.Bottom));
                        end;
                //нижняя сторона
                if d2SideBottom in FSides
                  then FPath.LineTo(d2Point(R.Left-FShiftSize + x1, R.Bottom))
                  else FPath.MoveTo(d2Point(R.Left-FShiftSize + x1, R.Bottom));

                //нижний левый угол
                if d2CornerBottomLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left-FShiftSize,R.Bottom - y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left-FShiftSize + x1,R.Bottom - y2),
                                            d2Point(R.Left-FShiftSize + x2,R.Bottom - y1),
                                            d2Point(R.Left-FShiftSize,R.Bottom - y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left-FShiftSize + x1,R.Bottom - y2));
                                FPath.LineTo(d2Point(R.Left-FShiftSize + x2,R.Bottom - y1));
                                FPath.LineTo(d2Point(R.Left-FShiftSize,R.Bottom - y1));
                              end;
                            else FPath.CurveTo(d2Point(R.Left-FShiftSize + x2,R.Bottom),
                                               d2Point(R.Left-FShiftSize,R.Bottom - (y2)),
                                               d2Point(R.Left-FShiftSize,R.Bottom - y1))
                          end;
                        end
                  else  begin
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Left-FShiftSize,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Left-FShiftSize,R.Bottom));
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left-FShiftSize,R.Bottom - y1))
                            else FPath.MoveTo(d2Point(R.Left-FShiftSize,R.Bottom - y1));
                        end;

                //левая сторона
                if d2SideLeft in FSides
                  then FPath.LineTo(d2Point(R.Left, R.Top + y1));
              end;

    // Вертикальный сдвиг
    d2ShiftVertical:
      //правая сторона сдвиг вниз
      if FShiftSize > 0
        then  begin
                //верхниий левый угол
                FPath.MoveTo(d2Point(R.Left, R.Top + y1));
                if d2CornerTopLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left + x1,R.Top));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left + x2, R.Top + y1),
                                            d2Point(R.Left + x1, R.Top + y2),
                                            d2Point(R.Left + x1, R.Top));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left + x2, R.Top + y1));
                                FPath.LineTo(d2Point(R.Left + x1, R.Top + y2));
                                FPath.LineTo(d2Point(R.Left + x1, R.Top));
                              end;
                          else  FPath.CurveTo(d2Point(R.Left, R.Top + (y2)),
                                              d2Point(R.Left + x2,R.Top),
                                              d2Point(R.Left + x1,R.Top))
                          end;
                        end
                  else  begin
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Top))
                            else FPath.MoveTo(d2Point(R.Left,R.Top));
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Left + x1,R.Top))
                            else FPath.MoveTo(d2Point(R.Left+ x1,R.Top));
                        end;

                //верхняя сторона
                if d2SideTop in FSides
                  then FPath.LineTo(d2Point(R.Right - x1,R.Top+FShiftSize))
                  else FPath.MoveTo(d2Point(R.Right - x1,R.Top+FShiftSize));

                //верхниий правый угол
                if d2CornerTopRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right,R.Top+FShiftSize + y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right - x1,R.Top+FShiftSize + y2),
                                            d2Point(R.Right - x2,R.Top+FShiftSize + y1),
                                            d2Point(R.Right,R.Top+FShiftSize + y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right - x1,R.Top+FShiftSize + y2));
                                FPath.LineTo(d2Point(R.Right - x2,R.Top+FShiftSize + y1));
                                FPath.LineTo(d2Point(R.Right,R.Top+FShiftSize + y1));
                              end;
                          else FPath.CurveTo(d2Point(R.Right - x2,R.Top+FShiftSize),
                                             d2Point(R.Right,R.Top+FShiftSize + y2),
                                             d2Point(R.Right,R.Top+FShiftSize + y1))
                          end;
                        end
                  else  begin
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Top+FShiftSize))
                            else FPath.MoveTo(d2Point(R.Right,R.Top+FShiftSize));
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Top+FShiftSize + y1))
                            else FPath.MoveTo(d2Point(R.Right,R.Top+FShiftSize + y1));
                        end;

                //правая сторона
                if d2SideRight in FSides
                  then FPath.LineTo(d2Point(R.Right, R.Bottom - y1))
                  else FPath.MoveTo(d2Point(R.Right, R.Bottom - y1));

                //нижний правый угол
                if d2CornerBottomRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right - x2,R.Bottom - y1),
                                            d2Point(R.Right - x1,R.Bottom - y2),
                                            d2Point(R.Right - x1,R.Bottom));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right - x2,R.Bottom - y1));
                                FPath.LineTo(d2Point(R.Right - x1,R.Bottom - y2));
                                FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
                              end;
                            else FPath.CurveTo(d2Point(R.Right,R.Bottom - (y2)),
                                               d2Point(R.Right - x2,R.Bottom),
                                               d2Point(R.Right - x1,R.Bottom))
                          end;
                        end
                  else  begin
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Right,R.Bottom));
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Right - x1,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Right - x1,R.Bottom));
                        end;
                //нижняя сторона
                if d2SideBottom in FSides
                  then FPath.LineTo(d2Point(R.Left + x1, R.Bottom-FShiftSize))
                  else FPath.MoveTo(d2Point(R.Left + x1, R.Bottom-FShiftSize));

                //нижний левый угол
                if d2CornerBottomLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left,R.Bottom-FShiftSize - y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left + x1,R.Bottom-FShiftSize - y2),
                                            d2Point(R.Left + x2,R.Bottom-FShiftSize - y1),
                                            d2Point(R.Left,R.Bottom-FShiftSize - y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left + x1,R.Bottom-FShiftSize - y2));
                                FPath.LineTo(d2Point(R.Left + x2,R.Bottom-FShiftSize - y1));
                                FPath.LineTo(d2Point(R.Left,R.Bottom-FShiftSize - y1));
                              end;
                            else FPath.CurveTo(d2Point(R.Left + x2,R.Bottom-FShiftSize),
                                               d2Point(R.Left,R.Bottom-FShiftSize - y2),
                                               d2Point(R.Left,R.Bottom-FShiftSize - y1))
                          end;
                        end
                  else  begin
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Bottom-FShiftSize))
                            else FPath.MoveTo(d2Point(R.Left,R.Bottom-FShiftSize));
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Bottom-FShiftSize - y1))
                            else FPath.MoveTo(d2Point(R.Left,R.Bottom-FShiftSize - y1));
                        end;

                //левая сторона
                if d2SideLeft in FSides
                  then FPath.LineTo(d2Point(R.Left, R.Top + y1));
              end

        //левая сторона сдвиг вниз
        else  begin
                //верхниий левый угол
                FPath.MoveTo(d2Point(R.Left, R.Top-FShiftSize + y1));
                if d2CornerTopLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left + x1,R.Top-FShiftSize));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left + x2, R.Top-FShiftSize + y1),
                                            d2Point(R.Left + x1, R.Top-FShiftSize + y2),
                                            d2Point(R.Left + x1, R.Top-FShiftSize));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left + x2, R.Top-FShiftSize + y1));
                                FPath.LineTo(d2Point(R.Left + x1, R.Top-FShiftSize + y2));
                                FPath.LineTo(d2Point(R.Left + x1, R.Top-FShiftSize));
                              end;
                          else  FPath.CurveTo(d2Point(R.Left, R.Top-FShiftSize + y2),
                                              d2Point(R.Left + x2,R.Top-FShiftSize),
                                              d2Point(R.Left + x1,R.Top-FShiftSize))
                          end;
                        end
                  else  begin
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Top-FShiftSize))
                            else FPath.MoveTo(d2Point(R.Left,R.Top-FShiftSize));
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Left + x1,R.Top-FShiftSize))
                            else FPath.MoveTo(d2Point(R.Left + x1,R.Top-FShiftSize));
                        end;

                //верхняя сторона
                if d2SideTop in FSides
                  then FPath.LineTo(d2Point(R.Right - x1,R.Top))
                  else FPath.MoveTo(d2Point(R.Right - x1,R.Top));

                //верхниий правый угол
                if d2CornerTopRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right,R.Top + y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right - x1,R.Top + y2),
                                            d2Point(R.Right - x2,R.Top + y1),
                                            d2Point(R.Right,R.Top + y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right - x1,R.Top + y2));
                                FPath.LineTo(d2Point(R.Right - x2,R.Top + y1));
                                FPath.LineTo(d2Point(R.Right,R.Top + y1));
                              end;
                          else FPath.CurveTo(d2Point(R.Right - x2,R.Top),
                                             d2Point(R.Right,R.Top + (y2)),
                                             d2Point(R.Right,R.Top + y1))
                          end;
                        end
                  else  begin
                          if d2SideTop in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Top))
                            else FPath.MoveTo(d2Point(R.Right,R.Top));
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Top + y1))
                            else FPath.MoveTo(d2Point(R.Right,R.Top + y1));
                        end;

                //правая сторона
                if d2SideRight in FSides
                  then FPath.LineTo(d2Point(R.Right, R.Bottom+FShiftSize - y1))
                  else FPath.MoveTo(d2Point(R.Right, R.Bottom+FShiftSize - y1));

                //нижний правый угол
                if d2CornerBottomRight in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Right - x1,R.Bottom+FShiftSize));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Right - x2,R.Bottom+FShiftSize - y1),
                                            d2Point(R.Right - x1,R.Bottom+FShiftSize - y2),
                                            d2Point(R.Right - x1,R.Bottom+FShiftSize));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Right - x2,R.Bottom+FShiftSize - y1));
                                FPath.LineTo(d2Point(R.Right - x1,R.Bottom+FShiftSize - y2));
                                FPath.LineTo(d2Point(R.Right - x1,R.Bottom+FShiftSize));
                              end;
                            else FPath.CurveTo(d2Point(R.Right,R.Bottom+FShiftSize - y2),
                                               d2Point(R.Right - x2,R.Bottom+FShiftSize),
                                               d2Point(R.Right - x1,R.Bottom+FShiftSize))
                          end;
                        end
                  else  begin
                          if d2SideRight in FSides
                            then FPath.LineTo(d2Point(R.Right,R.Bottom+FShiftSize))
                            else FPath.MoveTo(d2Point(R.Right,R.Bottom+FShiftSize));
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Right - x1,R.Bottom+FShiftSize))
                            else FPath.MoveTo(d2Point(R.Right - x1,R.Bottom+FShiftSize));
                        end;
                //нижняя сторона
                if d2SideBottom in FSides
                  then FPath.LineTo(d2Point(R.Left + x1, R.Bottom))
                  else FPath.MoveTo(d2Point(R.Left + x1, R.Bottom));

                //нижний левый угол
                if d2CornerBottomLeft in FCorners
                  then  begin
                          case FCornerType of
                            // d2CornetRound - default
                            d2CornerBevel:
                              FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
                            d2CornerInnerRound:
                              FPath.CurveTo(d2Point(R.Left + x1,R.Bottom - y2),
                                            d2Point(R.Left + x2,R.Bottom - y1),
                                            d2Point(R.Left,R.Bottom - y1));
                            d2CornerInnerLine:
                              begin
                                FPath.LineTo(d2Point(R.Left + x1,R.Bottom - y2));
                                FPath.LineTo(d2Point(R.Left + x2,R.Bottom - y1));
                                FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
                              end;
                            else FPath.CurveTo(d2Point(R.Left + x2,R.Bottom),
                                               d2Point(R.Left,R.Bottom - (y2)),
                                               d2Point(R.Left,R.Bottom - y1))
                          end;
                        end
                  else  begin
                          if d2SideBottom in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Bottom))
                            else FPath.MoveTo(d2Point(R.Left,R.Bottom));
                          if d2SideLeft in FSides
                            then FPath.LineTo(d2Point(R.Left,R.Bottom - y1))
                            else FPath.MoveTo(d2Point(R.Left,R.Bottom - y1));
                        end;

                //левая сторона
                if d2SideLeft in FSides
                  then FPath.LineTo(d2Point(R.Left, R.Top-FShiftSize + y1));
              end;
  end;

  //рисуем линии
  if FDoubleLine then
    case FShiftDirection of
      d2ShiftHorizontal:
        begin
          if ShiftSize>0
            then begin
                   x1:=DoubleLineIndent+ShiftSize;
                   x2:=DoubleLineIndent;
                 end
            else begin
                   x1:=DoubleLineIndent;
                   x2:=DoubleLineIndent+ShiftSize;
                 end;
          FPath.MoveTo(d2Point(R.Left+x1, R.Top));
          FPath.LineTo(d2Point(R.Left+x2, R.Bottom));
          FPath.MoveTo(d2Point(R.Right-x2, R.Top));
          FPath.LineTo(d2Point(R.Right-x1, R.Bottom));
        end;
      d2ShiftVertical:
        begin
          if ShiftSize>0
            then begin
                   x1:=DoubleLineIndent+ShiftSize;
                   x2:=DoubleLineIndent;
                 end
            else begin
                   x1:=DoubleLineIndent;
                   x2:=DoubleLineIndent+ShiftSize;
                 end;
          FPath.MoveTo(d2Point(R.Left, R.Top+x2));
          FPath.LineTo(d2Point(R.Right, R.Top+x1));
          FPath.MoveTo(d2Point(R.Left, R.Bottom-x1));
          FPath.LineTo(d2Point(R.Right, R.Bottom-x2));
        end;
    end;

end;

procedure TD2Parallelogram.Paint;
var
  R:TD2Rect;
  x1,x2: single;
begin
  CreatePath;
  R:=FPath.GetBounds;
  Canvas.FillPath(FPath,R,AbsoluteOpacity);
  Canvas.DrawPath(FPath,R,AbsoluteOpacity);
end;

constructor TD2Parallelogram.Create(AOwner: TComponent);
begin
  inherited;
  FShiftDirection:=d2ShiftHorizontal;
  FShiftSize:=10;
  FDoubleLineIndent:=5;
  FPath:=TD2PathData.Create;
end;

destructor TD2Parallelogram.Destroy;
begin
  FreeAndNil(FPath);
  inherited;
end;

//=============== TD2CalloutRectangle ====================================

constructor TD2CalloutRectangle.Create(AOwner:TComponent);
begin
  inherited;
  FCalloutWidth:=23;
  FCalloutLength:=11;
  FPath:=TD2PathData.Create;
end;

destructor TD2CalloutRectangle.Destroy;
begin
  FreeAndNil(FPath);
  inherited;
end;

procedure TD2CalloutRectangle.CreatePath;
var
  x1,x2,y1,y2:single;
  R:TD2Rect;
  Off:single;
begin
  R:=GetShapeRect;
  case CalloutPosition of
    d2CalloutTop:R.Top:=R.Top + FCalloutLength;
    d2CalloutLeft:R.Left:=R.Left + FCalloutLength;
    d2CalloutBottom:R.Bottom:=R.Bottom - FCalloutLength;
    d2CalloutRight:R.Right:=R.Right - FCalloutLength;
  end;
  if Sides <> AllSides then
  begin
    Off:=R.Left;
    if not (d2SideTop in FSides) then
      R.Top:=R.Top - Off;
    if not (d2SideLeft in FSides) then
      R.Left:=R.Left - Off;
    if not (d2SideBottom in FSides) then
      R.Bottom:=R.Bottom + Off;
    if not (d2SideRight in FSides) then
      R.Right:=R.Right + Off;
  end;
  x1:=xRadius;
  if d2RectWidth(R) - (x1 * 2) < 0 then
    x1:=(xRadius * (d2RectWidth(R) / (x1 * 2)));
  x2:=x1 / 2;
  y1:=yRadius;
  if d2RectHeight(R) - (y1 * 2) < 0 then
    y1:=(yRadius * (d2RectHeight(R) / (y1 * 2)));
  y2:=y1 / 2;
  FPath.Clear;
  FPath.MoveTo(d2Point(R.Left,R.Top + y1));
  if d2CornerTopLeft in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Left + x1,R.Top));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Left + x2,R.Top + y1),d2Point(R.Left + x1,R.Top + y2),d2Point(R.Left + x1,R.Top));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Left + x2,R.Top + y1));
          FPath.LineTo(d2Point(R.Left + x1,R.Top + y2));
          FPath.LineTo(d2Point(R.Left + x1,R.Top));
        end;
    else
      FPath.CurveTo(d2Point(R.Left,R.Top + (y2)),d2Point(R.Left + x2,R.Top),d2Point(R.Left + x1,R.Top))
    end;
  end
  else
  begin
    if d2SideLeft in FSides then
      FPath.LineTo(d2Point(R.Left,R.Top))
    else
      FPath.MoveTo(d2Point(R.Left,R.Top));
    if d2SideTop in FSides then
      FPath.LineTo(d2Point(R.Left + x1,R.Top))
    else
      FPath.MoveTo(d2Point(R.Left + x1,R.Top));
  end;
  if not (d2SideTop in FSides) then
    FPath.MoveTo(d2Point(R.Right - x1,R.Top))
  else
  begin
    if (FCalloutPosition=d2CalloutTop) then
    begin
      if CalloutOffset=0 then
      begin
        FPath.LineTo(d2Point((R.Right - R.Left) / 2 - (CalloutWidth / 2),R.Top));
        FPath.LineTo(d2Point((R.Right - R.Left) / 2,R.Top - FCalloutLength));
        FPath.LineTo(d2Point((R.Right - R.Left) / 2 + (CalloutWidth / 2),R.Top));
        FPath.LineTo(d2Point(R.Right - x1,R.Top));
      end
      else if CalloutOffset > 0 then
      begin
        FPath.LineTo(d2Point(R.Left + FCalloutOffset,R.Top));
        FPath.LineTo(d2Point(R.Left + FCalloutOffset + (CalloutWidth / 2),R.Top - FCalloutLength));
        FPath.LineTo(d2Point(R.Left + FCalloutOffset + CalloutWidth,R.Top));
        FPath.LineTo(d2Point(R.Right - x1,R.Top));
      end
      else
      begin
        FPath.LineTo(d2Point(R.Right - Abs(FCalloutOffset) - CalloutWidth,R.Top));
        FPath.LineTo(d2Point(R.Right - Abs(FCalloutOffset) - (CallOutWidth / 2),R.Top - FCalloutLength));
        FPath.LineTo(d2Point(R.Right - Abs(FCalloutOffset),R.Top));
        FPath.LineTo(d2Point(R.Right - x1,R.Top));
      end;
    end
    else
      FPath.LineTo(d2Point(R.Right - x1,R.Top));
  end;
  if d2CornerTopRight in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Right,R.Top + y1));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Right - x1,R.Top + y2),d2Point(R.Right - x2,R.Top + y1),d2Point(R.Right,R.Top + y1));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Right - x1,R.Top + y2));
          FPath.LineTo(d2Point(R.Right - x2,R.Top + y1));
          FPath.LineTo(d2Point(R.Right,R.Top + y1));
        end;
    else
      FPath.CurveTo(d2Point(R.Right - x2,R.Top),d2Point(R.Right,R.Top + (y2)),d2Point(R.Right,R.Top + y1))
    end;
  end
  else
  begin
    if d2SideTop in FSides then
      FPath.LineTo(d2Point(R.Right,R.Top))
    else
      FPath.MoveTo(d2Point(R.Right,R.Top));
    if d2SideRight in FSides then
      FPath.LineTo(d2Point(R.Right,R.Top + y1))
    else
      FPath.MoveTo(d2Point(R.Right,R.Top + y1));
  end;
  if not (d2SideRight in FSides) then
    FPath.MoveTo(d2Point(R.Right,R.Bottom - y1))
  else
  begin
    if (FCalloutPosition=d2CalloutRight) then
    begin
      if CalloutOffset=0 then
      begin
        FPath.LineTo(d2Point(R.Right,(R.Bottom - R.Top) / 2 - (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Right + FCalloutLength,(R.Bottom - R.Top) / 2));
        FPath.LineTo(d2Point(R.Right,(R.Bottom - R.Top) / 2 + (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Right,R.Bottom - y1));
      end
      else if CalloutOffset > 0 then
      begin
        FPath.LineTo(d2Point(R.Right,R.Top + CalloutOffset));
        FPath.LineTo(d2Point(R.Right + FCalloutLength,R.Top + CalloutOffset + (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Right,R.Top + CalloutOffset + CalloutWidth));
        FPath.LineTo(d2Point(R.Right,R.Bottom - y1));
      end
      else
      begin
        FPath.LineTo(d2Point(R.Right,R.Bottom + CalloutOffset));
        FPath.LineTo(d2Point(R.Right + FCalloutLength,R.Bottom + CalloutOffset + (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Right,R.Bottom + CalloutOffset + CalloutWidth));
        FPath.LineTo(d2Point(R.Right,R.Bottom - y1));
      end;
    end
    else
      FPath.LineTo(d2Point(R.Right,R.Bottom - y1));
  end;
  if d2CornerBottomRight in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Right - x2,R.Bottom - y1),d2Point(R.Right - x1,R.Bottom - y2),d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Right - x2,R.Bottom - y1));
          FPath.LineTo(d2Point(R.Right - x1,R.Bottom - y2));
          FPath.LineTo(d2Point(R.Right - x1,R.Bottom));
        end;
    else
      FPath.CurveTo(d2Point(R.Right,R.Bottom - (y2)),d2Point(R.Right - x2,R.Bottom),d2Point(R.Right - x1,R.Bottom))
    end;
  end
  else
  begin
    if d2SideRight in FSides then
      FPath.LineTo(d2Point(R.Right,R.Bottom))
    else
      FPath.MoveTo(d2Point(R.Right,R.Bottom));
    if d2SideBottom in FSides then
      FPath.LineTo(d2Point(R.Right - x1,R.Bottom))
    else
      FPath.MoveTo(d2Point(R.Right - x1,R.Bottom));
  end;
  if not (d2SideBottom in FSides) then
    FPath.MoveTo(d2Point(R.Left + x1,R.Bottom))
  else
  begin
    if (FCalloutPosition=d2CalloutBottom) then
    begin
      if CalloutOffset=0 then
      begin
        FPath.LineTo(d2Point((R.Right - R.Left) / 2 + (CalloutWidth / 2),R.Bottom));
        FPath.LineTo(d2Point((R.Right - R.Left) / 2,R.Bottom + FCalloutLength));
        FPath.LineTo(d2Point((R.Right - R.Left) / 2 - (CalloutWidth / 2),R.Bottom));
        FPath.LineTo(d2Point(R.Left + x1,R.Bottom));
      end
      else if CalloutOffset > 0 then
      begin
        FPath.LineTo(d2Point(R.Left + FCalloutOffset + CalloutWidth,R.Bottom));
        FPath.LineTo(d2Point(R.Left + FCalloutOffset + (CalloutWidth / 2),R.Bottom + FCalloutLength));
        FPath.LineTo(d2Point(R.Left + FCalloutOffset,R.Bottom));
        FPath.LineTo(d2Point(R.Left + x1,R.Bottom));
      end
      else
      begin
        FPath.LineTo(d2Point(R.Right - Abs(FCalloutOffset),R.Bottom));
        FPath.LineTo(d2Point(R.Right - Abs(FCalloutOffset) - (CalloutWidth / 2),R.Bottom + FCalloutLength));
        FPath.LineTo(d2Point(R.Right - Abs(FCalloutOffset) - CalloutWidth,R.Bottom));
        FPath.LineTo(d2Point(R.Left + x1,R.Bottom));
      end;
    end
    else
      FPath.LineTo(d2Point(R.Left + x1,R.Bottom));
  end;
  if d2CornerBottomLeft in FCorners then
  begin
    case FCornerType of
      // d2CornetRound - default
      d2CornerBevel:FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerRound:FPath.CurveTo(d2Point(R.Left + x1,R.Bottom - y2),d2Point(R.Left + x2,R.Bottom - y1),d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerLine:
        begin
          FPath.LineTo(d2Point(R.Left + x1,R.Bottom - y2));
          FPath.LineTo(d2Point(R.Left + x2,R.Bottom - y1));
          FPath.LineTo(d2Point(R.Left,R.Bottom - y1));
        end;
    else
      FPath.CurveTo(d2Point(R.Left + x2,R.Bottom),d2Point(R.Left,R.Bottom - (y2)),d2Point(R.Left,R.Bottom - y1))
    end;
  end
  else
  begin
    if d2SideBottom in FSides then
      FPath.LineTo(d2Point(R.Left,R.Bottom))
    else
      FPath.MoveTo(d2Point(R.Left,R.Bottom));
    if d2SideLeft in FSides then
      FPath.LineTo(d2Point(R.Left,R.Bottom - y1))
    else
      FPath.MoveTo(d2Point(R.Left,R.Bottom - y1));
  end;
  if (d2SideLeft in FSides) then
  begin
    if (FCalloutPosition=d2CalloutLeft) then
    begin
      if CalloutOffset=0 then
      begin
        FPath.LineTo(d2Point(R.Left,(R.Bottom - R.Top) / 2 + (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Left - FCalloutLength,(R.Bottom - R.Top) / 2));
        FPath.LineTo(d2Point(R.Left,(R.Bottom - R.Top) / 2 - (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Left,R.Top + y1));
      end
      else if CalloutOffset > 0 then
      begin
        FPath.LineTo(d2Point(R.Left,R.Top + CalloutOffset + CalloutWidth));
        FPath.LineTo(d2Point(R.Left - FCalloutLength,R.Top + CalloutOffset + (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Left,R.Top + CalloutOffset));
        FPath.LineTo(d2Point(R.Left,R.Top + y1));
      end
      else
      begin
        FPath.LineTo(d2Point(R.Left,R.Bottom + CalloutOffset + CalloutWidth));
        FPath.LineTo(d2Point(R.Left - FCalloutLength,R.Bottom + CalloutOffset + (CalloutWidth / 2)));
        FPath.LineTo(d2Point(R.Left,R.Bottom + CalloutOffset));
        FPath.LineTo(d2Point(R.Left,R.Top + y1));
      end;
    end
    else
      FPath.LineTo(d2Point(R.Left,R.Top + y1));
  end;
end;

procedure TD2CalloutRectangle.Paint;
var
  R:TD2Rect;
begin
  CreatePath;
  R:=FPath.GetBounds;
  Canvas.FillPath(FPath,R,AbsoluteOpacity);
  Canvas.DrawPath(FPath,R,AbsoluteOpacity);
end;

procedure TD2CalloutRectangle.SetCalloutWidth(const Value:single);
begin
  if FCalloutWidth <> Value then
  begin
    FCalloutWidth:=Value;
    CreatePath;
    Repaint;
  end;
end;

procedure TD2CalloutRectangle.SetCalloutLength(const Value:single);
begin
  if FCalloutLength <> Value then
  begin
    FCalloutLength:=Value;
    CreatePath;
    Repaint;
  end;
end;

procedure TD2CalloutRectangle.SetCalloutPosition(const Value:TD2CalloutPosition);
begin
  if FCalloutPosition <> Value then
  begin
    FCalloutPosition:=Value;
    CreatePath;
    Repaint;
  end;
end;

procedure TD2CalloutRectangle.SetCalloutOffset(const Value:single);
begin
  if FCalloutOffset <> Value then
  begin
    FCalloutOffset:=Value;
    CreatePath;
    Repaint;
  end;
end;


//=============== TD2Text =====================================================

constructor TD2Text.Create(AOwner:TComponent);
begin
  inherited;
  FHorzTextAlign:=d2TextAlignCenter;
  FVertTextAlign:=d2TextAlignCenter;
  FWordWrap:=true;
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
  Fill.DefaultColor:=$FF000000;
  Fill.SolidColor:=$FF000000;
  Stroke.DefaultStyle:=d2BrushNone;
  Stroke.Style:=d2BrushNone;
end;

destructor TD2Text.Destroy;
begin
  FFont.Free;
  inherited;
end;

function TD2Text.GetData:Variant;
begin
  Result:=TextW;
end;

procedure TD2Text.SetData(const Value:Variant);
begin
  TextW:=Value;
end;

procedure TD2Text.FontChanged(Sender:TObject);
begin
  FUpdateEffects:=true;
  if FAutoSize then AdjustSize;
  Repaint;
end;

procedure TD2Text.Paint;
var
  R:TD2Rect;
  M:TD2Matrix;
begin
  Canvas.Font.Assign(FFont);
  if FStretch then
  begin
    R:=d2Rect(0,0,1000,1000);
    Canvas.MeasureText(R,R,FText,false,FHorzTextAlign,FVertTextAlign);
    d2OffsetRect(R,-R.Left,-R.Top);
    M:=IdentityMatrix;
    if not d2IsRectEmpty(R) then
    begin
      M.m11:=d2RectWidth(LocalRect) / d2RectWidth(R);
      M.m22:=d2RectHeight(LocalRect) / d2RectHeight(R);
    end;
    Canvas.MultyMatrix(M);
    d2InflateRect(R,Font.Size / 3,Font.Size / 3);
    Canvas.FillText(R,R,FText,false,AbsoluteOpacity,FHorzTextAlign,FVertTextAlign);
  end
  else
    Canvas.FillText(LocalRect,LocalRect,FText,FWordWrap,AbsoluteOpacity,FHorzTextAlign,FVertTextAlign);
end;

procedure TD2Text.SetWordWrap(const Value:boolean);
begin
  if FWordWrap <> Value then
  begin
    FWordWrap:=Value;
    Repaint;
  end;
end;

procedure TD2Text.SetAutoSize(const Value:boolean);
begin
  if FAutoSize <> Value then
  begin
    FAutoSize:=Value;
    AdjustSize;
  end;
end;

procedure TD2Text.SetStretch(const Value:boolean);
begin
  if FStretch <> Value then
  begin
    FUpdateEffects:=true;
    FStretch:=Value;
    if Stretch then
      AutoSize:=false;
    Repaint;
  end;
end;

procedure TD2Text.AdjustSize;
var
  R:TD2Rect;
begin
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  if FAutoSize and (Canvas <> nil) and (FText <> '') then
  begin
    Canvas.Font.Assign(FFont);
    if WordWrap then
      R:=d2Rect(0,0,Width,1000)
    else
      R:=d2Rect(0,0,1000,1000);
    Canvas.MeasureText(R,R,FText,WordWrap,d2TextAlignNear,d2TextAlignNear);
    if not WordWrap then
      Width:=R.Right + (Font.Size / 2);
    if VertTextAlign <> d2TextAlignCenter then
      Height:=R.Bottom;
  end;
  FDisableAlign:=false;
  if FAutoSize and (Canvas <> nil) and (FText <> '') then
    if (Parent <> nil) and (Parent.IsVisual) then
      Parent.Visual.Realign;
end;

procedure TD2Text.Realign;
begin
  inherited;
  if FAutoSize then
    AdjustSize;
end;

procedure TD2Text.SetFont(const Value:TD2Font);
begin
  FFont.Assign(Value);
  if FAutoSize then AdjustSize;
end;

procedure TD2Text.SetHorzTextAlign(const Value:TD2TextAlign);
begin
  if FHorzTextAlign <> Value then
  begin
    FHorzTextAlign:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Text.SetVertTextAlign(const Value:TD2TextAlign);
begin
  if FVertTextAlign <> Value then
  begin
    FVertTextAlign:=Value;
    FUpdateEffects:=true;
    Repaint;
  end;
end;

procedure TD2Text.SetText(const Value:WideString);
begin
  if FText <> Value then
  begin
    FText:=Value;
    FUpdateEffects:=true;
    if FAutoSize then
      AdjustSize;
    Repaint;
  end;
end;

procedure TD2Text.SetText(const Value:String);
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  SetText(ss);
end;

Function TD2Text.GetText:WideString;
begin
  Result:=Ftext;
end;

Function TD2Text.GetText:string;
begin
  Result:=UTF8Encode(Ftext);
end;

//=============== TD2Path =====================================================

constructor TD2CustomPath.Create(AOwner:TComponent);
begin
  inherited;
  FWrapMode:=d2PathStretch;
  FData:=TD2PathData.Create;
  FData.OnChanged:=DoChanged;
end;

destructor TD2CustomPath.Destroy;
begin
  FreeAndNil(FData);
  inherited;
end;

procedure TD2CustomPath.DoChanged(Sender:TObject);
begin
  Repaint;
end;

function TD2CustomPath.PointInObject(X,Y:single):boolean;
begin
  if Assigned(Scene) and Scene.GetDesignTime and not FLocked and not FInPaintTo then
  begin
    Result:=inherited PointInObject(X,Y);
  end
  else
  if (Canvas <> nil) and (FData.ResourcePath <> nil) and (Length(FData.ResourcePath.PathData) > 1) then
  begin
    Result:=Canvas.PtInPath(AbsoluteToLocal(d2Point(X,Y)),GetShapeRect,FData.ResourcePath)
  end
  else
  if (Canvas <> nil) and (Length(FData.PathData) > 1) then
  begin
    Result:=Canvas.PtInPath(AbsoluteToLocal(d2Point(X,Y)),GetShapeRect,FData)
  end
  else
    Result:=inherited PointInObject(X,Y);
end;

procedure TD2CustomPath.Paint;
var
  R:TD2Rect;
  i,j:integer;
  State:integer;
  P:TD2PathData;
begin
  P:=FData;
  if FData.ResourcePath <> nil then
    P:=FData.ResourcePath;

  if not P.IsEmpty then
  begin
    case FWrapMode of
      d2PathOriginal:
        begin
          State:=Canvas.SaveCanvas;
          Canvas.IntersectClipRect(LocalRect);

          R:=P.GetBounds;
          d2OffsetRect(R,shapeRect.Left,shapeRect.Top);
          Canvas.FillPath(P,R,AbsoluteOpacity);
          Canvas.DrawPath(P,R,AbsoluteOpacity);

          Canvas.RestoreCanvas(State);
        end;
      d2PathFit:
        begin
          R:=P.GetBounds;
          d2FitRect(R,ShapeRect);
          Canvas.FillPath(P,R,AbsoluteOpacity);
          Canvas.DrawPath(P,R,AbsoluteOpacity);
        end;
      d2PathStretch:
        begin
          Canvas.FillPath(P,ShapeRect,AbsoluteOpacity);
          Canvas.DrawPath(P,ShapeRect,AbsoluteOpacity);
        end;
      d2PathTile:
        begin
          State:=Canvas.SaveCanvas;
          Canvas.IntersectClipRect(LocalRect);

          R:=P.GetBounds;
          for i:=0 to round(Width / d2RectWidth(R)) do
            for j:=0 to round(Height / d2RectHeight(R)) do
            begin
              R:=P.GetBounds;
              d2OffsetRect(R,shapeRect.Left,shapeRect.Top);
              d2OffsetRect(R,i * (d2RectWidth(R) + shapeRect.Left * 2),j * (d2RectHeight(R) + shapeRect.Top * 2));
              Canvas.FillPath(P,R,AbsoluteOpacity);
              Canvas.DrawPath(P,R,AbsoluteOpacity);
            end;

          Canvas.RestoreCanvas(State);
        end;
    end;
  end;
  if Assigned(Scene) and Scene.GetDesignTime and not FLocked and not FInPaintTo then
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

procedure TD2CustomPath.SetWrapMode(const Value:TD2PathWrap);
begin
  if FWrapMode <> Value then
  begin
    FWrapMode:=Value;
    Repaint;
  end;
end;

procedure TD2CustomPath.SetData(const Value:TD2PathData);
begin
  FData.Assign(Value);
end;
              
        
           
        
        
         

            
  
