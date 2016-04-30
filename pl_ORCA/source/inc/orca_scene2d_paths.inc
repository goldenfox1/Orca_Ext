{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Path ===================================================

constructor TD2PathData.Create;
begin
  inherited Create;
end;

destructor TD2PathData.Destroy;
begin
  if FResource <> nil then
  begin
    FResource.RemoveFreeNotify(Self);
    FResource:=nil;
  end;
  inherited;
end;

procedure TD2PathData.Assign(Source:TPersistent);
begin
  if Source is TD2PathData then
  begin
    if TD2PathData(Source).ResourcePath <> nil then
    begin
      Resource:=TD2PathData(Source).Resource;
      FResourceName:=TD2PathData(Source).ResourceName;
      if Assigned(FOnChanged) then
        FOnChanged(Self);
    end
    else
    begin
      SetLength(PathData,Length(TD2PathData(Source).PathData));
      System.Move(TD2PathData(Source).PathData[0],PathData[0],SizeOf(TD2PathPoint) * Length(PathData));
      if Assigned(FOnChanged) then
        FOnChanged(Self);
    end;
  end
  else
    inherited
end;

procedure TD2PathData.SetResource(const Value:TD2PathObject);
begin
  if FResource <> Value then
  begin
    if FResource <> nil then
      FResource.RemoveFreeNotify(Self);
    FResource:=Value;
    if FResource <> nil then
    begin
      FResourceName:=FResource.ResourceName;
      FResource.AddFreeNotify(Self);
    end
    else
      FResourceName:='';
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

function TD2PathData.GetResourceName:string;
begin
  Result:=FResourceName;
end;

procedure TD2PathData.SetResourceName(const Value:string);
begin
  if Value <> FResourceName then
  begin
    FResourceName:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

function TD2PathData.GetPath:TD2PathData;
var
  O:TD2Object;
begin
  Result:=nil;
  if FResource <> nil then
  begin
    Result:=TD2PathObject(FResource).Path;
  end
  else
  if FResourceName <> '' then
  begin
    O:=FindResource(FResourceName);
    if O is TD2PathObject then
      Resource:=TD2PathObject(O);
    if FResource <> nil then
      Result:=TD2PathObject(FResource).Path;
  end;
end;

procedure TD2PathData.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineBinaryProperty('Path',ReadPath,WritePath,Length(PathData) > 0);
end;

procedure TD2PathData.ReadPath(Stream:TStream);
var
  S:longint;
  i:integer;
  k:byte;
  p:TD2Point;
begin
  Stream.Read(S,SizeOf(S));
  {$ifdef FPC_BIG_ENDIAN}
  ReverseBytes(@S,4);
  {$endif}
  SetLength(PathData,S);
  if S > 0 then
  begin
    if (Stream.Size  - 4) div S=9 then
    begin
      for i:=0 to S - 1 do
      begin
        Stream.Read(k,1);
        Stream.Read(p,SizeOf(p));
        {$ifdef FPC_BIG_ENDIAN}
        ReverseBytes(@p.x,4);
        ReverseBytes(@p.y,4);
        {$endif}
        PathData[i].Kind:=TD2PathPointKind(k);
        PathData[i].Point:=p;
      end;
    end
    else
    begin
      Stream.Read(PathData[0],S * SizeOf(TD2PathPoint));
      {$ifdef FPC_BIG_ENDIAN}
      for i:=0 to S * 3 - 1 do
        ReverseBytes(@PD2ColorArray(PathData)[i],4);
      {$endif}
    end;
  end;
end;

procedure TD2PathData.WritePath(Stream:TStream);
var
  S:longint;
begin
  S:=Length(PathData);
  Stream.Write(S,SizeOf(S));
  if S > 0 then
    Stream.Write(PathData[0],S * SizeOf(TD2PathPoint));
end;

function TD2PathData.LastPoint:TD2Point;
begin
  if Length(PathData) > 0 then
    Result:=PathData[High(PathData)].Point
  else
    Result:=d2Point(0,0);
end;

procedure TD2PathData.MoveTo(const P:TD2Point);
begin
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointMoveTo;
  PathData[High(PathData)].Point:=P;
  FStartPoint:=PathData[High(PathData)].Point;
end;

procedure TD2PathData.MoveToRel(const P:TD2Point);
begin
  with LastPoint do
  begin
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointMoveTo;
    PathData[High(PathData)].Point:=d2Point(x + P.x,y + P.y);
  end;
  FStartPoint:=PathData[High(PathData)].Point;
end;

procedure TD2PathData.LineTo(const P:TD2Point);
begin
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointLineTo;
  PathData[High(PathData)].Point:=P;
end;

procedure TD2PathData.LineToRel(const P:TD2Point);
begin
  with LastPoint do
  begin
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointLineTo;
    PathData[High(PathData)].Point:=d2Point(x + P.x,y + P.y);
  end;
end;

procedure TD2PathData.HLineTo(const x:single);
begin
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointLineTo;
  PathData[High(PathData)].Point:=d2Point(x,PathData[High(PathData) - 1].Point.y);
end;

procedure TD2PathData.HLineToRel(const x:single);
var
  LP:TD2Point;
begin
  LP:=LastPoint;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointLineTo;
  PathData[High(PathData)].Point:=d2Point(LP.x + x,LP.y);
end;

procedure TD2PathData.VLineTo(const y:single);
begin
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointLineTo;
  PathData[High(PathData)].Point:=d2Point(PathData[High(PathData) - 1].Point.x,y);
end;

procedure TD2PathData.VLineToRel(const y:single);
var
  LP:TD2Point;
begin
  LP:=LastPoint;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointLineTo;
  PathData[High(PathData)].Point:=d2Point(LP.x,LP.y + y);
end;

procedure TD2PathData.CurveTo(const ControlPoint1,ControlPoint2,EndPoint:TD2Point);
begin
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointCurveTo;
  PathData[High(PathData)].Point:=ControlPoint1;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointCurveTo;
  PathData[High(PathData)].Point:=ControlPoint2;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointCurveTo;
  PathData[High(PathData)].Point:=EndPoint;
end;

procedure TD2PathData.CurveToRel(const ControlPoint1,ControlPoint2,EndPoint:TD2Point);
begin
  with LastPoint do
  begin
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointCurveTo;
    PathData[High(PathData)].Point:=d2Point(x + ControlPoint1.x,y + ControlPoint1.y);
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointCurveTo;
    PathData[High(PathData)].Point:=d2Point(x + ControlPoint2.x,y + ControlPoint2.y);;
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointCurveTo;
    PathData[High(PathData)].Point:=d2Point(x + EndPoint.x,y + EndPoint.y);;
  end;
end;

procedure TD2PathData.SmoothCurveTo(const ControlPoint2,EndPoint:TD2Point);
var
  ControlPoint1:TD2Point;
begin
  if Length(PathData) > 2 then
  begin
    ControlPoint1.x:=LastPoint.x + (LastPoint.x - PathData[High(PathData)].Point.x);
    ControlPoint1.y:=LastPoint.y + (LastPoint.y - PathData[High(PathData)].Point.y);
  end
  else
    ControlPoint1:=ControlPoint2;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointCurveTo;
  PathData[High(PathData)].Point:=ControlPoint1;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointCurveTo;
  PathData[High(PathData)].Point:=ControlPoint2;
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointCurveTo;
  PathData[High(PathData)].Point:=EndPoint;
end;

procedure TD2PathData.SmoothCurveToRel(const ControlPoint2,EndPoint:TD2Point);
var
  ControlPoint1:TD2Point;
begin
  if Length(PathData) > 2 then
  begin
    ControlPoint1.x:=LastPoint.x + (LastPoint.x - PathData[High(PathData)].Point.x);
    ControlPoint1.y:=LastPoint.y + (LastPoint.y - PathData[High(PathData)].Point.y);
  end
  else
    ControlPoint1:=ControlPoint2;
  with LastPoint do
  begin
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointCurveTo;
    PathData[High(PathData)].Point:=d2Point(ControlPoint1.x,ControlPoint1.y);
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointCurveTo;
    PathData[High(PathData)].Point:=d2Point(x + ControlPoint2.x,y + ControlPoint2.y);;
    SetLength(PathData,Length(PathData) + 1);
    PathData[High(PathData)].Kind:=d2PathPointCurveTo;
    PathData[High(PathData)].Point:=d2Point(x + EndPoint.x,y + EndPoint.y);;
  end;
end;

procedure TD2PathData.ClosePath;
begin
  SetLength(PathData,Length(PathData) + 1);
  PathData[High(PathData)].Kind:=d2PathPointClose;
  PathData[High(PathData)].Point:=FStartPoint;
end;

procedure TD2PathData.Clear;
begin
  SetLength(PathData,0);
  if Assigned(FOnChanged) then
    FOnChanged(Self);
end;

function TD2PathData.GetBounds:TD2Rect;
var
  i:integer;
begin
  if Length(PathData)=0 then
  begin
    Result:=d2Rect(0,0,0,0);
    Exit;
  end;
  Result:=d2Rect($FFFF,$FFFF,-$FFFF,-$FFFF);
  for i:=0 to High(PathData) do
  begin
    if PathData[i].Kind=d2PathPointClose then Continue;

    if PathData[i].Point.X < Result.Left then Result.Left:=PathData[i].Point.X;
    if PathData[i].Point.X > Result.Right then Result.Right:=PathData[i].Point.X;
    if PathData[i].Point.Y < Result.Top then Result.Top:=PathData[i].Point.Y;
    if PathData[i].Point.Y > Result.Bottom then Result.Bottom:=PathData[i].Point.Y;
  end;
  // add small amount
  if d2RectWidth(Result)=0 then Result.Right:=Result.Left + 0.001;
  if d2RectHeight(Result)=0 then Result.Bottom:=Result.Top + 0.001;
end;

procedure TD2PathData.Scale(const scaleX,scaleY:single);
var
  i:integer;
begin
  if Length(PathData) > 0 then
  begin
    for i:=0 to High(PathData) do
      case PathData[i].Kind of
        d2PathPointMoveTo,d2PathPointLineTo,d2PathPointCurveTo:
          begin
            PathData[i].Point.x:=PathData[i].Point.x * scaleX;
            PathData[i].Point.y:=PathData[i].Point.y * scaleY;
          end;
        d2PathPointClose:
          begin
          end;
      end;
  end;
end;

procedure TD2PathData.Offset(const dX,dY:single);
var
  i:integer;
begin
  if Length(PathData) > 0 then
  begin
    for i:=0 to High(PathData) do
      case PathData[i].Kind of
        d2PathPointMoveTo,d2PathPointLineTo,d2PathPointCurveTo:
          begin
            PathData[i].Point.x:=PathData[i].Point.x + dX;
            PathData[i].Point.y:=PathData[i].Point.y + dY;
          end;
        d2PathPointClose:
          begin
          end;
      end;
  end;
end;

procedure TD2PathData.ApplyMatrix(const M:TD2Matrix);
var
  i:integer;
begin
  if Length(PathData) > 0 then
  begin
    for i:=0 to High(PathData) do
      case PathData[i].Kind of
        d2PathPointMoveTo,d2PathPointLineTo,d2PathPointCurveTo:
          begin
            with d2VectorTransform(d2Vector(PathData[i].Point),M) do
              PathData[i].Point:=d2Point(x,y);
          end;
        d2PathPointClose:
          begin
          end;
      end;
  end;
end;

procedure TD2PathData.Flatten(const Flatness:single=0.25);

  procedure CalculateBezierCoefficients(const Bezier:TD2CubicBezier; out ax,bx,cx,ay,by,cy:single);
  begin
    cx:=3.0 * (Bezier[1].x - Bezier[0].x);
    cy:=3.0 * (Bezier[1].y - Bezier[0].y);
    bx:=3.0 * (Bezier[2].x - Bezier[1].x) - cx;
    by:=3.0 * (Bezier[2].y - Bezier[1].y) - cy;
    ax:=Bezier[3].x - Bezier[0].x - cx - bx;
    ay:=Bezier[3].y - Bezier[0].y - cy - by;
  end;

  function PointOnBezier(const StartPoint:TD2Point; const ax,bx,cx,ay,by,cy,T:single):TD2Point;
  var
    tSqr  :single;
    tCube :single;
  begin
    tSqr    :=t * t;
    tCube   :=tSqr * t;
    Result.x:=(ax * tCube) + (bx * tSqr) + (cx * t) + StartPoint.x;
    Result.y:=(ay * tCube) + (by * tSqr) + (cy * t) + StartPoint.y;
  end;

  function CreateBezier(const Bezier:TD2CubicBezier; const PointCount:integer):TD2Polygon;
  var
    ax :single;
    bx :single;
    cx :single;
    ay :single;
    by :single;
    cy :single;
    dT :single;
    T  :single;
    i  :integer;
  begin
    if PointCount=0 then exit;
    dT:=1.0 / (1.0 * PointCount - 1.0);
    T :=0.0;
    SetLength(Result,PointCount);
    CalculateBezierCoefficients(Bezier,ax,bx,cx,ay,by,cy);
    for i:=0 to PointCount - 1 do
    begin
      Result[i]:=PointOnBezier(Bezier[0],ax,bx,cx,ay,by,cy,T);
      T:=T + dT;
    end;
  end;

var
  i,j:integer;
  BPts:TD2Polygon;
  B:TD2CubicBezier;
  Len:single;
  SegCount:integer;
  OldPathData:array of TD2PathPoint;
  CurPoint:TD2Point;
  f,s:single;
  Bounds,R:TD2Rect;
begin
  { scale }
  if Length(PathData) > 0 then
  begin
    Bounds:=GetBounds;
    R:=Bounds;
    d2FitRect(R,d2Rect(0,0,100,100));
    s:=d2MinFloat(d2RectWidth(Bounds) / 100,d2RectHeight(Bounds) / 100);
    f:=Flatness * s;
    if f < 0.05 then f:=0.05;

    { copy data }
    SetLength(OldPathData,Length(PathData));
    System.Move(PathData[0],OldPathData[0],Length(PathData) * SizeOf(PathData[0]));
    SetLength(PathData,0);

    i:=0;
    while i < Length(OldPathData) do
    begin
      case OldPathData[i].Kind of
        d2PathPointMoveTo:
          begin
            MoveTo(OldPathData[i].Point);
            CurPoint:=OldPathData[i].Point;
          end;
        d2PathPointLineTo:
          begin
            LineTo(OldPathData[i].Point);
            CurPoint:=OldPathData[i].Point;
          end;
        d2PathPointCurveTo:
          begin
            B[0]:=CurPoint;
            B[1]:=OldPathData[i].Point;
            Inc(i);
            B[2]:=OldPathData[i].Point;
            Inc(i);
            B[3]:=OldPathData[i].Point;
            Len:=d2VectorLength(d2VectorSubtract(d2Vector(B[1]),d2Vector(B[3])));
            SegCount:=round(Len / f);
            if SegCount < 2 then
              SegCount:=2;
            BPts:=CreateBezier(B,SegCount);
            for j:=0 to High(BPts) do
            begin
              LineTo(BPts[j]);
            end;
            CurPoint:=OldPathData[i].Point;
          end;
        d2PathPointClose:
          begin
            ClosePath;
          end;
      end;
      Inc(i);
    end;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

function TD2PathData.FlattenToPolygon(var Polygon:TD2Polygon; const Flatness:single=0.25):TD2Point;

  procedure CalculateBezierCoefficients(const Bezier:TD2CubicBezier; out ax,bx,cx,ay,by,cy:single);
  begin
    cx:=3.0 * (Bezier[1].x - Bezier[0].x);
    cy:=3.0 * (Bezier[1].y - Bezier[0].y);
    bx:=3.0 * (Bezier[2].x - Bezier[1].x) - cx;
    by:=3.0 * (Bezier[2].y - Bezier[1].y) - cy;
    ax:=Bezier[3].x - Bezier[0].x - cx - bx;
    ay:=Bezier[3].y - Bezier[0].y - cy - by;
  end;

  function PointOnBezier(const StartPoint:TD2Point; const ax,bx,cx,ay,by,cy,T:single):TD2Point;
  var
    tSqr  :single;
    tCube :single;
  begin
    tSqr    :=t * t;
    tCube   :=tSqr * t;
    Result.x:=(ax * tCube) + (bx * tSqr) + (cx * t) + StartPoint.x;
    Result.y:=(ay * tCube) + (by * tSqr) + (cy * t) + StartPoint.y;
  end;

  function CreateBezier(const Bezier:TD2CubicBezier; const PointCount:integer):TD2Polygon;
  var
    ax :single;
    bx :single;
    cx :single;
    ay :single;
    by :single;
    cy :single;
    dT :single;
    T  :single;
    i  :integer;
  begin
    if PointCount=0 then exit;
    dT:=1.0 / (1.0 * PointCount - 1.0);
    T :=0.0;
    SetLength(Result,PointCount);
    CalculateBezierCoefficients(Bezier,ax,bx,cx,ay,by,cy);
    for i:=0 to PointCount - 1 do
    begin
      Result[i]:=PointOnBezier(Bezier[0],ax,bx,cx,ay,by,cy,T);
      T:=T + dT;
    end;
  end;

var
  i,j:integer;
  BPts:TD2Polygon;
  B:TD2CubicBezier;
  SP,CurPoint:TD2Point;
  Len:single;
  SegCount:integer;
  f,s:single;
  Bounds,R:TD2Rect;
begin
  Result:=d2Point(0,0);
  SetLength(Polygon,0);
  if Length(PathData) > 0 then
  begin
    Bounds:=GetBounds;
    R:=Bounds;
    d2FitRect(R,d2Rect(0,0,100,100));
    s:=d2MinFloat(d2RectWidth(Bounds) / 100,d2RectHeight(Bounds) / 100);
    f:=Flatness * s;
    if f < 0.05 then f:=0.05;

    i:=0;
    while i < Length(PathData) do
    begin
      case PathData[i].Kind of
        d2PathPointMoveTo:
          begin
            SetLength(Polygon,Length(Polygon) + 1);
            Polygon[High(Polygon)]:=PathData[i].Point;
            CurPoint:=PathData[i].Point;
            SP:=CurPoint;
          end;
        d2PathPointLineTo:
          begin
            SetLength(Polygon,Length(Polygon) + 1);
            Polygon[High(Polygon)]:=PathData[i].Point;
            CurPoint:=PathData[i].Point;
          end;
        d2PathPointCurveTo:
          begin
            B[0]:=CurPoint;
            B[1]:=PathData[i].Point;
            Inc(i);
            B[2]:=PathData[i].Point;
            Inc(i);
            B[3]:=PathData[i].Point;
            Len:=d2VectorLength(d2VectorSubtract(d2Vector(B[1]),d2Vector(B[3])));
            SegCount:=round(Len / f);
            if SegCount < 2 then
              SegCount:=2;
            BPts:=CreateBezier(B,SegCount);
            for j:=0 to High(BPts) do
            begin
              SetLength(Polygon,Length(Polygon) + 1);
              Polygon[High(Polygon)]:=BPts[j];
            end;
            CurPoint:=PathData[i].Point;
          end;
        d2PathPointClose:
          begin
            SetLength(Polygon,Length(Polygon) + 1);
            Polygon[High(Polygon)]:=SP;
            SetLength(Polygon,Length(Polygon) + 1);
            Polygon[High(Polygon)]:=ClosePolygon;
          end;
      end;
      Inc(i);
    end;
    with GetBounds do
      Result:=d2Point(Abs(Right - Left),Abs(Bottom - Top));
  end;
end;

procedure TD2PathData.AddEllipse(const ARect:TD2Rect);
var
  cx,cy:single;
  px,py:single;
begin
  cx:=(ARect.Left + ARect.Right) / 2;
  cy:=(ARect.Top + ARect.Bottom) / 2;
  px:=CurveKappa * (d2RectWidth(ARect) / 2);
  py:=CurveKappa * (d2RectHeight(ARect) / 2);
  MoveTo(d2Point(ARect.Left,cy));
  CurveTo(d2Point(ARect.Left,cy - py),d2Point(cx - px,ARect.Top),d2Point(cx,ARect.Top));
  CurveTo(d2Point(cx + px,ARect.Top),d2Point(ARect.Right,cy - py),d2Point(ARect.Right,cy));
  CurveTo(d2Point(ARect.Right,cy + py),d2Point(cx + px,ARect.Bottom),d2Point(cx,ARect.Bottom));
  CurveTo(d2Point(cx - px,ARect.Bottom),d2Point(ARect.Left,cy + py),d2Point(ARect.Left,cy));
end;

procedure TD2PathData.AddRectangle(const ARect:TD2Rect; const xRadius,yRadius:single; const ACorners:TD2Corners;
      const ACornerType:TD2CornerType=d2CornerRound);
var
  R:TD2Rect;
  x1,x2,y1,y2:single;
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

  MoveTo(d2Point(R.Left,R.Top + y1));
  if d2CornerTopLeft in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:LineTo(d2Point(R.Left + x1,R.Top));
      d2CornerInnerRound:CurveTo(d2Point(R.Left + x2,R.Top + y1),d2Point(R.Left + x1,R.Top + y2),
        d2Point(R.Left + x1,R.Top));
      d2CornerInnerLine:
        begin
          LineTo(d2Point(R.Left + x2,R.Top + y1));
          LineTo(d2Point(R.Left + x1,R.Top + y2));
          LineTo(d2Point(R.Left + x1,R.Top));
        end;
    else
      CurveTo(d2Point(R.Left,R.Top + (y2)),d2Point(R.Left + x2,R.Top),d2Point(R.Left + x1,R.Top))
    end;
  end
  else
  begin
    LineTo(d2Point(R.Left,R.Top));
    LineTo(d2Point(R.Left + x1,R.Top));
  end;
  LineTo(d2Point(R.Right - x1,R.Top));
  if d2CornerTopRight in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:LineTo(d2Point(R.Right,R.Top + y1));
      d2CornerInnerRound:CurveTo(d2Point(R.Right - x1,R.Top + y2),d2Point(R.Right - x2,R.Top + y1),
        d2Point(R.Right,R.Top + y1));
      d2CornerInnerLine:
        begin
          LineTo(d2Point(R.Right - x1,R.Top + y2));
          LineTo(d2Point(R.Right - x2,R.Top + y1));
          LineTo(d2Point(R.Right,R.Top + y1));
        end;
    else
      CurveTo(d2Point(R.Right - x2,R.Top),d2Point(R.Right,R.Top + (y2)),d2Point(R.Right,R.Top + y1))
    end;
  end
  else
  begin
    LineTo(d2Point(R.Right,R.Top));
    LineTo(d2Point(R.Right,R.Top + y1));
  end;
  LineTo(d2Point(R.Right,R.Bottom - y1));
  if d2CornerBottomRight in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:LineTo(d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerRound:CurveTo(d2Point(R.Right - x2,R.Bottom - y1),d2Point(R.Right - x1,R.Bottom - y2),
        d2Point(R.Right - x1,R.Bottom));
      d2CornerInnerLine:
        begin
          LineTo(d2Point(R.Right - x2,R.Bottom - y1));
          LineTo(d2Point(R.Right - x1,R.Bottom - y2));
          LineTo(d2Point(R.Right - x1,R.Bottom));
        end;
    else
      CurveTo(d2Point(R.Right,R.Bottom - (y2)),d2Point(R.Right - x2,R.Bottom),d2Point(R.Right - x1,R.Bottom))
    end;
  end
  else
  begin
    LineTo(d2Point(R.Right,R.Bottom));
    LineTo(d2Point(R.Right - x1,R.Bottom));
  end;
  LineTo(d2Point(R.Left + x1,R.Bottom));
  if d2CornerBottomLeft in ACorners then
  begin
    case ACornerType of
      // d2CornetRound - default
      d2CornerBevel:LineTo(d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerRound:CurveTo(d2Point(R.Left + x1,R.Bottom - y2),d2Point(R.Left + x2,R.Bottom - y1),
        d2Point(R.Left,R.Bottom - y1));
      d2CornerInnerLine:
        begin
          LineTo(d2Point(R.Left + x1,R.Bottom - y2));
          LineTo(d2Point(R.Left + x2,R.Bottom - y1));
          LineTo(d2Point(R.Left,R.Bottom - y1));
        end;
    else
      CurveTo(d2Point(R.Left + x2,R.Bottom),d2Point(R.Left,R.Bottom - (y2)),d2Point(R.Left,R.Bottom - y1))
    end;
  end
  else
  begin
    LineTo(d2Point(R.Left,R.Bottom));
    LineTo(d2Point(R.Left,R.Bottom - y1));
  end;
  ClosePath;
end;

procedure DrawArcWithBezier(Path:TD2PathData; CenterX,CenterY,RadiusX,RadiusY,StartAngle,SweepRange:single; UseMoveTo:boolean);
var
  Coord:array[0..3] of TD2Point;
  pts:array[0..3] of TD2Point;
  a,b,c,x,y:Double;
  ss,cc:Double;
  i:integer;
begin
  if SweepRange=0 then
  begin
    if UseMoveTo then
    begin
      if (Length(Path.PathData)=0) then
        Path.MoveTo(d2Point(CenterX + RadiusX * cos(StartAngle),CenterY - RadiusY * sin(StartAngle)))
      else
        Path.LineTo(d2Point(CenterX + RadiusX * cos(StartAngle),CenterY - RadiusY * sin(StartAngle)));
    end;
    Path.LineTo(d2Point(CenterX + RadiusX * cos(StartAngle),CenterY - RadiusY * sin(StartAngle)));
    Exit;
  end;
  b:=sin(SweepRange / 2);
  c:=cos(SweepRange / 2);
  a:=1 - c;
  x:=a * 4.0 / 3.0;
  y:=b - x * c / b;
  ss:=sin(StartAngle + SweepRange / 2);
  cc:=cos(StartAngle + SweepRange / 2);
  Coord[0]:=d2Point(c,-b);
  Coord[1]:=d2Point(c + x,-y);
  Coord[2]:=d2Point(c + x,y);
  Coord[3]:=d2Point(c,b);
  for i:=0 to 3 do
  begin
    pts[i]:=d2Point(CenterX + RadiusX * (Coord[i].x * cc - Coord[i].y * ss),Centery + RadiusY * (Coord[i].x * ss + Coord[i].y * cc));
  end;
  if UseMoveTo then
  begin
    if (Length(Path.PathData)=0) then
      Path.MoveTo(pts[0])
    else
      Path.LineTo(pts[0]);
  end;
  Path.CurveTo(pts[1],pts[2],pts[3]);
end;

procedure TD2PathData.AddArc(const Center,Radius:TD2Point; StartAngle,SweepAngle:single);
const
  bezier_arc_angle_epsilon=0.01;
var
  UseMoveTo:boolean;
  i:integer;
  f:single;
  total_sweep ,
  local_sweep ,
  prev_sweep  :single;
  done:boolean;
begin
  StartAngle:=d2DegToRad(StartAngle);
  SweepAngle:=d2DegToRad(SweepAngle);

  i:=trunc(StartAngle / (2.0 * cPi));
  f:=StartAngle - (i * 2.0 * cPi);

  StartAngle:=f;

  if SweepAngle >= 2.0 * cPi then
    SweepAngle:=2.0 * cPi;
  if SweepAngle <= -2.0 * cPi then
    SweepAngle:=-2.0 * cPi;

  if Abs(SweepAngle ) < 1e-10 then
  begin
    exit;
  end;

  total_sweep:=0.0;

  done:=false;
  UseMoveTo:=true;
  repeat
    if SweepAngle < 0.0 then
    begin
      prev_sweep:=total_sweep;
      local_sweep:= -cPi * 0.5;
      total_sweep:= total_sweep - (cPi * 0.5 );
      if total_sweep <= SweepAngle + bezier_arc_angle_epsilon  then
      begin
        local_sweep:=SweepAngle - prev_sweep;
        done:=true;
      end;
    end
    else
    begin
      prev_sweep:=total_sweep;
      local_sweep:= cPi * 0.5;
      total_sweep:= total_sweep + (pi * 0.5 );
      if total_sweep >= SweepAngle - bezier_arc_angle_epsilon then
      begin
        local_sweep:=SweepAngle - prev_sweep;
        done:=true;
      end;
    end;
    DrawArcWithBezier(Self,Center.x,Center.y,Radius.x,Radius.y,StartAngle,local_sweep,UseMoveTo);
    UseMoveTo:=false;
    StartAngle:=StartAngle + local_sweep;
  until done;
end;

procedure TD2PathData.AddArcSd2Part(const Center,Radius:TD2Point; StartAngle,SweepAngle:single);
const
  bezier_arc_angle_epsilon=0.01;
var
  UseMoveTo:boolean;
  i:integer;
  f:single;
  total_sweep ,
  local_sweep ,
  prev_sweep  :single;
  done:boolean;
begin
  StartAngle:=d2DegToRad(StartAngle);
  SweepAngle:=d2DegToRad(SweepAngle);

  i:=trunc(StartAngle / (2.0 * cPi));
  f:=StartAngle - (i * 2.0 * cPi);

  StartAngle:=f;

  if SweepAngle >= 2.0 * cPi then
    SweepAngle:=2.0 * cPi;
  if SweepAngle <= -2.0 * cPi then
    SweepAngle:=-2.0 * cPi;

  if Abs(SweepAngle ) < 1e-10 then
  begin
    exit;
  end;

  total_sweep:=0.0;

  done:=false;
  UseMoveTo:=false;
  repeat
    if SweepAngle < 0.0 then
    begin
      prev_sweep:=total_sweep;
      local_sweep:= -cPi * 0.5;
      total_sweep:= total_sweep - (cPi * 0.5 );
      if total_sweep <= SweepAngle + bezier_arc_angle_epsilon  then
      begin
        local_sweep:=SweepAngle - prev_sweep;
        done:=true;
      end;
    end
    else
    begin
      prev_sweep:=total_sweep;
      local_sweep:= cPi * 0.5;
      total_sweep:= total_sweep + (pi * 0.5 );
      if total_sweep >= SweepAngle - bezier_arc_angle_epsilon then
      begin
        local_sweep:=SweepAngle - prev_sweep;
        done:=true;
      end;
    end;
    DrawArcWithBezier(Self,Center.x,Center.y,Radius.x,Radius.y,StartAngle,local_sweep,UseMoveTo);
    UseMoveTo:=false;
    StartAngle:=StartAngle + local_sweep;
  until done;
end;

procedure TD2PathData.AddArcSd2(const P1,Radius:TD2Point; Angle:single; const LargeFlag,SweepFlag:boolean; const P2:TD2Point);
var
  i:integer;
  //m_radii_ok:boolean;
  v ,p ,n ,sq ,
  rx ,ry ,
  x0 ,y0 ,
  x1 ,y1 ,
  x2 ,y2 ,
  cx ,cy ,
  ux ,uy ,
  vx ,vy ,

  dx2 ,dy2 ,
  prx ,pry ,
  px1 ,py1 ,
  cx1 ,cy1 ,
  sx2 ,sy2 ,

  sign ,coef ,

  radii_check ,
  start_angle ,
  sweep_angle ,

  cos_a ,sin_a:single;
  tm:TD2Matrix;
  len:integer;
begin
  rx:=Radius.X;
  ry:=Radius.Y;
  x0:=P1.X;
  y0:=P1.Y;
  x2:=P2.X;
  y2:=P2.Y;
  angle:=d2DegToRad(Angle);

  //m_radii_ok:=true;

  if rx < 0.0 then
   rx:=-rx;

  if ry < 0.0 then
   ry:=-rx;

  // Calculate the middle point between
  // the current and the final points
  dx2:=(x0 - x2 ) / 2.0;
  dy2:=(y0 - y2 ) / 2.0;

  // Convert angle from degrees to radians
  cos_a:=cos(angle);
  sin_a:=sin(angle);

  // Calculate (x1,y1)
  x1:=cos_a * dx2 + sin_a * dy2;
  y1:=-sin_a * dx2 + cos_a * dy2;

  // Ensure radii are large enough
  prx :=rx * rx;
  pry :=ry * ry;
  px1 :=x1 * x1;
  py1 :=y1 * y1;

  // Check that radii are large enough
  radii_check:=px1 / prx + py1 / pry;

  if radii_check > 1.0 then
  begin
    rx :=sqrt(radii_check ) * rx;
    ry :=sqrt(radii_check ) * ry;
    prx:=rx * rx;
    pry:=ry * ry;

    //if radii_check > 10.0 then
    //  m_radii_ok:=false;

  end;

  // Calculate (cx1,cy1)
  if LargeFlag=SweepFlag then
    sign:=-1.0
  else
    sign:=1.0;

  sq:=(prx * pry - prx * py1 - pry * px1 ) / (prx * py1 + pry * px1 );

  if sq < 0 then
    coef:=sign * sqrt(0 )
  else
    coef:=sign * sqrt(sq );

  cx1:=coef *  ((rx * y1 ) / ry );
  cy1:=coef * -((ry * x1 ) / rx );

  // Calculate (cx,cy) from (cx1,cy1)
  sx2:=(x0 + x2 ) / 2.0;
  sy2:=(y0 + y2 ) / 2.0;
  cx:=sx2 + (cos_a * cx1 - sin_a * cy1 );
  cy:=sy2 + (sin_a * cx1 + cos_a * cy1 );

  // Calculate the start_angle (angle1) and the sweep_angle (dangle)
  ux:= (x1 - cx1 ) / rx;
  uy:= (y1 - cy1 ) / ry;
  vx:=(-x1 - cx1 ) / rx;
  vy:=(-y1 - cy1 ) / ry;

  // Calculate the angle start
  n:=sqrt(ux * ux + uy * uy );
  p:=ux; // (1 * ux ) + (0 * uy )

  if uy < 0 then
    sign:=-1.0
  else
    sign:=1.0;

  v:=p / n;

  if v < -1.0 then
    v:=-1.0;

  if v > 1.0 then
    v:=1.0;

  start_angle:=sign * ArcCos(v );

  // Calculate the sweep angle
  n:=sqrt((ux * ux + uy * uy ) * (vx * vx + vy * vy ) );
  p:=ux * vx + uy * vy;

  if ux * vy - uy * vx < 0 then
    sign:=-1.0
  else
    sign:=1.0;

   v:=p / n;

  if v < -1.0 then
    v:=-1.0;

  if v > 1.0 then
    v:=1.0;

  sweep_angle:=sign * ArcCos(v );

  if (not SweepFlag ) and (sweep_angle > 0 ) then
    sweep_angle:=sweep_angle - pi * 2.0
  else
    if SweepFlag and (sweep_angle < 0 ) then
      sweep_angle:=sweep_angle + pi * 2.0;

  len:=Length(PathData);
  AddArcSd2Part(d2Point(0,0),d2Point(rx,ry),d2RadToDeg(start_angle),d2RadToDeg(sweep_angle));

  tm:=IdentityMatrix;
  tm.m31:=cx;
  tm.m32:=cy;
  tm:=d2MatrixMultiply(d2CreateRotationMatrix(Angle),tm);

  i:=len;
  while i < Length(PathData) do
  begin
    with d2VectorTransform(d2Vector(PathData[i].Point),tm) do
      PathData[i].Point:=d2Point(x,y);
    inc(i);
  end;
end;

function TD2PathData.IsEmpty:boolean;
begin
  Result:=Length(PathData)=0;
end;

function TD2PathData.GetPathString:AnsiString;
var
  i:integer;
begin
  Result:='';
  i:=0;
  while i < Length(PathData) do
  begin
    case PathData[i].Kind of
      d2PathPointMoveTo:
        Result:=Result + 'M ' + d2FloatToStr(PathData[i].Point.X) + ',' + d2FloatToStr(PathData[i].Point.Y) + ' ';
      d2PathPointLineTo:
        Result:=Result + 'L ' + d2FloatToStr(PathData[i].Point.X) + ',' + d2FloatToStr(PathData[i].Point.Y) + ' ';
      d2PathPointCurveTo:
        begin
          Result:=Result + 'C ' + d2FloatToStr(PathData[i].Point.X) + ',' + d2FloatToStr(PathData[i].Point.Y) + ' ' +
            d2FloatToStr(PathData[i + 1].Point.X) + ',' + d2FloatToStr(PathData[i + 1].Point.Y) + ' ' +
            d2FloatToStr(PathData[i + 2].Point.X) + ',' + d2FloatToStr(PathData[i + 2].Point.Y) + ' ';
          Inc(i,2);
        end;
      d2PathPointClose:
        Result:=Result + 'Z ';
    end;
    Inc(i);
  end;
end;

function GetTok(const S:AnsiString; var Pos:integer):AnsiString;
var
  i:integer;
begin
  Result:='';
  if Pos > Length(S) then Exit;
  while (Pos <= Length(S)) and (S[Pos] in [' ']) do
    Inc(Pos);
  for i:=Pos to Length(S) do
  begin
    if System.Pos(S[i],'zmlchvsqtaZMLCHVSQTA')=0 then Break;
    Result:=Result + S[i];
  end;
  Pos:=i;
end;

function GetNum(const S:AnsiString; var Pos:integer):AnsiString;
var
  i:integer;
begin
  Result:='';
  if Pos > Length(S) then Exit;
  while (Pos <= Length(S)) and (S[Pos] in [' ']) do
    Inc(Pos);
  for i:=Pos to Length(S) do
  begin
    if (S[i]='e') then
    begin
      Result:=Result + S[i];
      Continue;
    end;
    if (S[i]='-') and (Length(Result) > 0) and (Result[Length(Result)]='e') then
    begin
      Result:=Result + S[i];
      Continue;
    end;
    if (System.Pos(S[i],'0123456789.')=0) and not ((i=Pos) and (S[i]='-')) then Break;
    Result:=Result + S[i];
  end;
  while S[Pos] in [' '] do
    Inc(Pos);
  Pos:=i;
end;

function GetPoint(const S:AnsiString; var Pos:integer):TD2Point;
var
  x,y:AnsiString;
begin
  Result:=d2Point(0,0);
  if Pos > Length(S) then Exit;
  while (Pos <= Length(S)) and (S[Pos] in [',',' ']) do
    Inc(Pos);
  x:=GetNum(S,Pos);
  while (Pos <= Length(S)) and (S[Pos] in [',',' ']) do
    Inc(Pos);
  y:=GetNum(S,Pos);
  while (Pos <= Length(S)) and (S[Pos] in [',',' ']) do
    Inc(Pos);

  Result:=d2Point(d2StrToFloat(x),d2StrToFloat(y));
end;

procedure TD2PathData.SetPathString(const Value:AnsiString);
const
  TokSep=' '#10#13;
  TokStop='0123456789-';
  NumSep=' ,'#10#13;
  NumStop='zmlchvsqtaZMLCHVSQTA';
var
  S,toks:AnsiString;
  tok:AnsiChar;
  R,CP1,CP2:TD2Point;
  angle:single;
  large,sweet:boolean;
  lastlen,pos,i:integer;
begin
  { remove #10#13 }
  for i:=1 to Length(Value) do
  begin
    if Value[i] in [#9,#10,#13] then Continue;
    S:=S + Value[i];
  end;
  { }
  SetLength(PathData,0);
  pos:=1;
  while S <> '' do
  begin
    lastlen:=pos;
    toks:=GetTok(S,pos);
    while toks <> '' do
    begin
      tok:=toks[1];
      Delete(toks,1,1);
      try
        if (tok in ['z','Z']) then
        begin
          ClosePath;
        end;
        if (tok in ['M']) then
        begin
          MoveTo(GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            LineTo(GetPoint(S,pos));
          end;
        end;
        if (tok in ['m']) then
        begin
          MoveToRel(GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            LineToRel(GetPoint(S,pos));
          end;
        end;
        if (tok='L') then
        begin
          LineTo(GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            LineTo(GetPoint(S,pos));
          end;
        end;
        if (tok='l') then
        begin
          LineToRel(GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            LineToRel(GetPoint(S,pos));
          end;
        end;
        if (tok='C') then
        begin
          CP1:=GetPoint(S,pos);
          CP2:=GetPoint(S,pos);
          CurveTo(CP1,CP2,GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            CP1:=GetPoint(S,pos);
            CP2:=GetPoint(S,pos);
            CurveTo(CP1,CP2,
              GetPoint(S,pos)
            );
          end;
        end;
        if (tok='c') then
        begin
          CP1:=GetPoint(S,pos);
          CP2:=GetPoint(S,pos);
          CurveToRel(CP1,CP2,GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            CP1:=GetPoint(S,pos);
            CP2:=GetPoint(S,pos);
            CurveToRel(CP1,CP2,GetPoint(S,pos));
          end;
        end;
        if (tok='S') then
        begin
          CP2:=GetPoint(S,pos);
          SmoothCurveTo(CP2,GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            CP2:=GetPoint(S,pos);
            SmoothCurveTo(CP2,GetPoint(S,pos));
          end;
        end;
        if (tok='s') then
        begin
          CP2:=GetPoint(S,pos);
          SmoothCurveToRel(CP2,GetPoint(S,pos));
          while (S <> '') and (S[pos] in ['0','1','2','3','4','5','6','7','8','9','-']) do
          begin
            { next points }
            CP2:=GetPoint(S,pos);
            SmoothCurveToRel(CP2,GetPoint(S,pos));
          end;
        end;
        if (tok='H') then
        begin
          // skip horizontal line
          HLineTo(d2StrToFloat(GetNum(S,Pos)));
        end;
        if (tok='h') then
        begin
          // skip horizontal line
          HLineToRel(d2StrToFloat(GetNum(S,Pos)));
        end;
        if (tok='V') then
        begin
          // skip vertical line
          VLineTo(d2StrToFloat(GetNum(S,Pos)));
        end;
        if (tok='v') then
        begin
          // skip vertical line
          VLineToRel(d2StrToFloat(GetNum(S,Pos)));
        end;
        if (tok='Q') then
        begin
          // skip quadratic bezier
          GetPoint(S,pos);
          GetPoint(S,pos);
        end;
        if (tok='q') then
        begin
          // skip quadratic bezier
          GetPoint(S,pos);
          GetPoint(S,pos);
        end;
        if (tok='T') then
        begin
          // skip show qudratic bezier
          GetPoint(S,pos);
        end;
        if (tok='t') then
        begin
          // skip show qudratic bezier
          GetPoint(S,pos);
        end;
        if (tok='A') then
        begin
          // arc
          if Length(PathData) > 0 then
            CP1:=PathData[High(PathData)].Point
          else
            CP1:=d2Point(0,0);
          R:=GetPoint(S,pos);
          angle:=d2StrToFloat(GetNum(S,Pos));
          with GetPoint(S,pos) do
          begin
            large:=X=1;
            sweet:=Y=1;
          end;
          CP2:=GetPoint(S,pos);
          AddArcSd2(CP1,R,angle,large,sweet,CP2);
        end;
        if (tok='a') then
        begin
          // arc rel
          if Length(PathData) > 0 then
            CP1:=PathData[High(PathData)].Point
          else
            CP1:=d2Point(0,0);
          R:=GetPoint(S,pos);
          angle:=d2StrToFloat(GetNum(S,Pos));
          with GetPoint(S,pos) do
          begin
            large:=X=1;
            sweet:=Y=1;
          end;
          CP2:=GetPoint(S,pos);
          CP2.x:=CP1.x + CP2.x;
          CP2.y:=CP1.y + CP2.y;
          AddArcSd2(CP1,R,angle,large,sweet,CP2);
        end;
      except
      end;
    end;
    if lastlen=pos then
    begin
      Pos :=0;
      Break;
    end;
  end;
  if Assigned(FOnChanged) then
    FOnChanged(Self);
end;


//================== Spline ======================================

constructor TD2Spline.Create(const Polygon:TD2Polygon);
var
  i:integer;
  X,Y:array of single;
begin
  inherited Create;
  len:=Length(Polygon);
  SetLength(X,len);
  SetLength(Y,len);
  for i:=0 to len - 1 do
  begin
    X[i]:=Polygon[i].X;
    Y[i]:=Polygon[i].Y;
  end;
  CalcHermiteFactors(X,matX);
  CalcHermiteFactors(Y,matY);
end;

destructor TD2Spline.Destroy;
begin
  inherited;
end;

procedure TD2Spline.SplineXY(const t:single; var X,Y :single);
begin
  X:=HermitInterpolate(MatX,t,len);
  Y:=HermitInterpolate(MatY,t,len);
end;    
                      
