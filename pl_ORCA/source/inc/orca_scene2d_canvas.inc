{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Canvas =================================

constructor TD2Canvas.Create(const AWidth,AHeight:integer);
begin
  inherited Create;
  FStroke:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FFill:=TD2Brush.Create(d2BrushSolid,$FFFFFFFF);
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
  ResizeBuffer(AWidth,AHeight);
end;

constructor TD2Canvas.CreateFromBitmap(const ABitmap:TD2Bitmap);
begin
  inherited Create;
  FWidth:=ABitmap.Width;
  FHeight:=ABitmap.Height;
  FStroke:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FFill:=TD2Brush.Create(d2BrushSolid,$FFFFFFFF);
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
end;

destructor TD2Canvas.Destroy;
var
  i:integer;
begin
  if Length(FSaveData) > 0 then
  begin
    for i:=0 to High(FSaveData) do
    begin
      if FSaveData[i].Index=$FFFFFFFF then Continue;
      FSaveData[i].Stroke.Free;
      FSaveData[i].Fill.Free;
      FSaveData[i].Font.Free;
    end;
    SetLength(FSaveData,0);
  end;
  FFont.Free;
  FStroke.Free;
  FFill.Free;
  FreeBuffer;
  inherited;
end;

procedure TD2Canvas.AssignTo(Dest:TPersistent);
var
  i:integer;
begin
  if Dest is TD2Bitmap then
  begin
    TD2Bitmap(Dest).SetSize(FWidth,FHeight);
    if FBuffered then
      for i:=0 to FHeight - 1 do
        System.Move(PD2ColorArray(FBufferBits)[(i) * FWidth],TD2Bitmap(Dest).Scanline[i]^,FWidth * 4);
  end
  else
    inherited ;
end;

class function TD2Canvas.GetBitmapScanline(Bitmap:TD2Bitmap;
  y:integer):PD2ColorArray;
begin
  Result:=nil;
end;

procedure TD2Canvas.SaveToStream(S:TStream);
var
  B:TD2Bitmap;
begin
  if FBuffered then
  begin
    B:=TD2Bitmap.Create(FWidth,FHeight);
    d2MoveLongword(FBufferBits,B.StartLine,FWidth * FHeight);
    B.SaveToStream(S);
    B.Free;
  end;
end;

procedure TD2Canvas.SaveToBits(Bits:Pointer);
begin
  if FBuffered then
    d2MoveLongword(FBufferBits,Bits,FWidth * FHeight);
end;

function TD2Canvas.BeginScene:boolean;
begin
  Result:=true;
end;

procedure TD2Canvas.EndScene;
begin
end;

procedure TD2Canvas.SetMatrix(const M:TD2Matrix);
begin
  FMatrix:=M;
end;

procedure TD2Canvas.MultyMatrix(const M:TD2Matrix);
begin
end;

procedure TD2Canvas.FontChanged(Sender:TObject);
begin
end;

function TD2Canvas.LoadFontFromStream(AStream:TStream):boolean;
begin
end;

function TD2Canvas.TextHeight(const AText:WideString):single;
var
  R:TD2Rect;
begin
  R:=d2Rect(0,0,10000,10000);
  MeasureText(R,R,AText,false,d2TextAlignNear,d2TextAlignCenter);
  Result:=d2RectHeight(R);
end;

function TD2Canvas.TextWidth(const AText:WideString):single;
var
  R:TD2Rect;
begin
  R:=d2Rect(0,0,10000,20);
  MeasureText(R,R,AText,false,d2TextAlignNear,d2TextAlignCenter);
  Result:=d2RectWidth(R);
end;

procedure TD2Canvas.FillArc(const Center,Radius:TD2Point; StartAngle,SweepAngle:single; const AOpacity:single);
var
  P:TD2PathData;
begin
  P:=TD2PathData.Create;
  P.AddArc(Center,Radius,StartAngle,SweepAngle);
  FillPath(P,P.Getbounds,AOpacity);
  P.Free;
end;

procedure TD2Canvas.DrawArc(const Center,Radius:TD2Point; StartAngle,SweepAngle:single; const AOpacity:single);
var
  P:TD2PathData;
begin
  P:=TD2PathData.Create;
  P.AddArc(Center,Radius,StartAngle,SweepAngle);
  DrawPath(P,P.Getbounds,AOpacity);
  P.Free;
end;

procedure TD2Canvas.SetStrokeDash(const Value:TD2StrokeDash);
begin
  if Value <> FStrokeDash then
  begin
    FStrokeDash:=Value;
    case FStrokeDash of
      d2DashSolid:
        begin
          FDashOffset:=0;
          SetLength(FDash,0);
        end;
      d2DashDash:
        begin
          FDashOffset:=0;
          SetLength(FDash,2);
          FDash[0]:=1 * 3;
          FDash[1]:=1;
        end;
      d2DashDot:
        begin
          FDashOffset:=0;
          SetLength(FDash,2);
          FDash[0]:=1;
          FDash[1]:=1;
        end;
	//----------- Block added by GoldenFox -----------
      d2DashDotDash:			
        begin
          FDashOffset:=0;
          SetLength(FDash,2);
          FDash[0]:=1;
          FDash[1]:=1 * 3;
        end;
	//-------- End block added by GoldenFox ----------
      d2DashDashDot:
        begin
          FDashOffset:=0;
          SetLength(FDash,4);
          FDash[0]:=1 * 3;
          FDash[1]:=1;
          FDash[2]:=1;
          FDash[3]:=1;
        end;
      d2DashDashDotDot:
        begin
          FDashOffset:=0;
          SetLength(FDash,6);
          FDash[0]:=1 * 3;
          FDash[1]:=1;
          FDash[2]:=1;
          FDash[3]:=1;
          FDash[4]:=1;
          FDash[5]:=1;
        end;
      d2DashCustom:;
    else
      FDashOffset:=0;
      SetLength(FDash,0);
    end;
  end;
end;

procedure TD2Canvas.SetCustomDash(Dash:array of single; Offset:single);
var
  i:integer;
begin
  FStrokeDash:=d2DashCustom;
  SetLength(FDash,Length(Dash));
  for i:=0 to High(Dash) do
    FDash[i]:=Dash[i];
  FDashOffset:=Offset;
end;

procedure TD2Canvas.FillPolygon(const Points:TD2Polygon; const AOpacity:single);
var
  i:integer;
  Path:TD2PathData;
begin
  Path:=TD2PathData.Create;
  SetLength(Path.PathData,Length(Points));
  for i:=0 to High(Points) do
    with Path.PathData[i] do
    begin
      Kind:=d2PathPointLineTo;
      if i=0 then
        Kind:=d2PathPointMoveTo;
      if (i > 0) and (Path.PathData[i - 1].Kind=d2PathPointClose) then
        Kind:=d2PathPointMoveTo;
      if (Points[i].x=ClosePolygon.x) and (Points[i].y=ClosePolygon.y) then
      begin
        Kind:=d2PathPointClose;
        Continue;
      end;
      if i=High(Points) then
        Kind:=d2PathPointClose;
      Point:=Points[i];
    end;
  FillPath(Path,Path.GetBounds,AOpacity);
  Path.Free;
end;

procedure TD2Canvas.DrawPolygon(const Points:TD2Polygon; const AOpacity:single);
var
  i:integer;
  Path:TD2PathData;
begin
  Path:=TD2PathData.Create;
  SetLength(Path.PathData,Length(Points));
  for i:=0 to High(Points) do
    with Path.PathData[i] do
    begin
      Kind:=d2PathPointLineTo;
      if (i=0)  then
        Kind:=d2PathPointMoveTo;
      if (i > 0) and (Path.PathData[i - 1].Kind=d2PathPointClose) then
        Kind:=d2PathPointMoveTo;
      if (Points[i].x=ClosePolygon.x) and (Points[i].y=ClosePolygon.y) then
      begin
        Kind:=d2PathPointClose;
        Continue;
      end;
      Point:=Points[i];
    end;
  DrawPath(Path,Path.GetBounds,AOpacity);
  Path.Free;
end;

procedure TD2Canvas.DrawRectSides(const ARect:TD2Rect; const xRadius,yRadius:single; const ACorners:TD2Corners; const AOpacity:single;
  const ASides:TD2Sides;
  const ACornerType:TD2CornerType=d2CornerRound);
var
  Path:TD2PathData;
  x1,x2,y1,y2:single;
  R:TD2Rect;
begin
  R:=ARect;
  x1:=xRadius;
  if d2RectWidth(R) - (x1 * 2) < 0 then
    x1:=(xRadius * (d2RectWidth(R) / (x1 * 2)));
  x2:=x1 / 2;
  y1:=yRadius;
  if d2RectHeight(R) - (y1 * 2) < 0 then
    y1:=(yRadius * (d2RectHeight(R) / (y1 * 2)));
  y2:=y1 / 2;
  Path:=TD2PathData.Create;
  Path.MoveTo(d2Point(R.Left,R.Top + y1));
  if d2CornerTopLeft in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:Path.LineTo(d2Point(R.Left + x1,R.Top));
      d2CornerInnerRound:Path.CurveTo(d2Point(R.Left + x2,R.Top + y1),d2Point(R.Left + x1,R.Top + y2),d2Point(R.Left + x1,R.Top));
      d2CornerInnerLine:
        begin
          Path.LineTo(d2Point(R.Left + x2,R.Top + y1));
          Path.LineTo(d2Point(R.Left + x1,R.Top + y2));
          Path.LineTo(d2Point(R.Left + x1,R.Top));
        end;
    else
      Path.CurveTo(d2Point(R.Left,R.Top + (y2)),d2Point(R.Left + x2,R.Top),d2Point(R.Left + x1,R.Top))
    end;
  end
  else
  begin
    if d2SideLeft in ASides then
      Path.LineTo(d2Point(R.Left,R.Top))
    else
      Path.MoveTo(d2Point(R.Left,R.Top));
    if d2SideTop in ASides then
      Path.LineTo(d2Point(R.Left + x1,R.Top))
    else
      Path.MoveTo(d2Point(R.Left + x1,R.Top));
  end;
  if not (d2SideTop in ASides) then
    Path.MoveTo(d2Point(R.Right - x1,R.Top))
  else
    Path.LineTo(d2Point(R.Right - x1,R.Top));
  if d2CornerTopRight in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:Path.LineTo(d2Point(R.Right,R.Top + y1));
      d2CornerInnerRound:Path.CurveTo(d2Point(R.Right - x1,R.Top + y2),d2Point(R.Right - x2,R.Top + y1),d2Point(R.Right,R.Top + y1));
      d2CornerInnerLine:
        begin
          Path.LineTo(d2Point(R.Right - x1,R.Top + y2));
          Path.LineTo(d2Point(R.Right - x2,R.Top + y1));
          Path.LineTo(d2Point(R.Right,R.Top + y1));
        end;
    else
      Path.CurveTo(d2Point(R.Right - x2,R.Top),d2Point(R.Right,R.Top + (y2)),d2Point(R.Right,R.Top + y1))
    end;
  end
  else
  begin
    if d2SideTop in ASides then
      Path.LineTo(d2Point(R.Right,R.Top))
    else
      Path.MoveTo(d2Point(R.Right,R.Top));
    if d2SideRight in ASides then
      Path.LineTo(d2Point(R.Right,R.Top + y1))
    else
      Path.MoveTo(d2Point(R.Right,R.Top + y1));
  end;
  if not (d2SideRight in ASides) then
    Path.MoveTo(d2Point(R.Right,R.Bottom - y1))
  else
    Path.LineTo(d2Point(R.Right,R.Bottom - y1));
  if d2CornerBottomRight in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:Path.LineTo(d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerRound:Path.CurveTo(d2Point(R.Right - x2,R.Bottom - y1),d2Point(R.Right - x1,R.Bottom - y2),d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerLine:
        begin
          Path.LineTo(d2Point(R.Right - x2,R.Bottom - y1));
          Path.LineTo(d2Point(R.Right - x1,R.Bottom - y2));
          Path.LineTo(d2Point(R.Right - x1,R.Bottom));
        end;
    else
      Path.CurveTo(d2Point(R.Right,R.Bottom - (y2)),d2Point(R.Right - x2,R.Bottom),d2Point(R.Right - x1,R.Bottom))
    end;
  end
  else
  begin
    if d2SideRight in ASides then
      Path.LineTo(d2Point(R.Right,R.Bottom))
    else
      Path.MoveTo(d2Point(R.Right,R.Bottom));
    if d2SideBottom in ASides then
      Path.LineTo(d2Point(R.Right - x1,R.Bottom))
    else
      Path.MoveTo(d2Point(R.Right - x1,R.Bottom));
  end;
  if not (d2SideBottom in ASides) then
    Path.MoveTo(d2Point(R.Left + x1,R.Bottom))
  else
    Path.LineTo(d2Point(R.Left + x1,R.Bottom));
  if d2CornerBottomLeft in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:Path.LineTo(d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerRound:Path.CurveTo(d2Point(R.Left + x1,R.Bottom - y2),d2Point(R.Left + x2,R.Bottom - y1),d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerLine:
        begin
          Path.LineTo(d2Point(R.Left + x1,R.Bottom - y2));
          Path.LineTo(d2Point(R.Left + x2,R.Bottom - y1));
          Path.LineTo(d2Point(R.Left,R.Bottom - y1));
        end;
    else
      Path.CurveTo(d2Point(R.Left + x2,R.Bottom),d2Point(R.Left,R.Bottom - (y2)),d2Point(R.Left,R.Bottom - y1))
    end;
  end
  else
  begin
    if d2SideBottom in ASides then
      Path.LineTo(d2Point(R.Left,R.Bottom))
    else
      Path.MoveTo(d2Point(R.Left,R.Bottom));
    if d2SideLeft in ASides then
      Path.LineTo(d2Point(R.Left,R.Bottom - y1))
    else
      Path.MoveTo(d2Point(R.Left,R.Bottom - y1));
  end;
  if (d2SideLeft in ASides) then
  begin
    Path.LineTo(d2Point(R.Left,R.Top + y1));
  end;
  DrawPath(Path,ARect,AOpacity);
  Path.Free;
end;

 
                 
