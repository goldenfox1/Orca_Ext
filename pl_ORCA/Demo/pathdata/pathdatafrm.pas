{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit pathdatafrm;

  {$mode objfpc}{$H+}

interface

uses
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TForm15 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    PaintRect: TD2Rectangle;
    CompoundTrackBar1: TD2CompoundTrackBar;
    Label1: TD2Label;
    procedure PaintRectMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure PaintRectMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure PaintRectMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y, Dx, Dy: Single);
    procedure PaintRectPaint(Sender: TObject; const aCanvas: TD2Canvas;
      const ARect: TD2Rect);
  private
    { Private declarations }
    FPress: boolean;
    FSegment: TD2Polygon;
    FLastPoint: TD2Point;
  public
    { Public declarations }
  end;

var
  Form15: TForm15;

implementation

{$R *.lfm}

procedure CurveFromPoint(P0, P1, P2, P3: TD2Point; var CP1, CP2: TD2Point);
const
  smooth_value = 1.0;
var
  xc1, yc1, xc2, yc2, xc3, yc3: single;
  len1, len2, len3: single;
  k1, k2: single;
  xm1, ym1, xm2, ym2: single;
begin
  xc1 := (P0.x + P1.x) / 2.0;
  yc1 := (P0.y + P1.y) / 2.0;
  xc2 := (P1.x + P2.x) / 2.0;
  yc2 := (P1.y + P2.y) / 2.0;
  xc3 := (P2.x + P3.x) / 2.0;
  yc3 := (P2.y + P3.y) / 2.0;

  len1 := sqrt((P1.x - P0.x) * (P1.x - P0.x) + (P1.Y - P0.y) * (P1.y - P0.y));
  len2 := sqrt((P2.x - P1.x) * (P2.X - P1.x) + (P2.Y - P1.y) * (P2.y - P1.y));
  len3 := sqrt((P3.x - P2.x) * (P3.x - P2.x) + (P3.Y - P2.y) * (P3.y - P2.y));

  k1 := len1 / (len1 + len2);
  k2 := len2 / (len2 + len3);

  xm1 := xc1 + (xc2 - xc1) * k1;
  ym1 := yc1 + (yc2 - yc1) * k1;

  xm2 := xc2 + (xc3 - xc2) * k2;
  ym2 := yc2 + (yc3 - yc2) * k2;

  // Resulting control points. Here smooth_value is mentioned
  // above coefficient K whose value should be in range [0...1].
  CP1.x := xm1 + (xc2 - xm1) * smooth_value + P1.x - xm1;
  CP1.y := ym1 + (yc2 - ym1) * smooth_value + P1.y - ym1;
  CP2.x := xm2 + (xc2 - xm2) * smooth_value + P2.x - xm2;
  CP2.y := ym2 + (yc2 - ym2) * smooth_value + P2.y - ym2;
end;

procedure TForm15.PaintRectMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Single);
begin
  FPress := true;
  PaintRect.AutoCapture := true;
  SetLength(FSegment, 0);
  FLastPoint := d2Point(X, Y);
end;

procedure TForm15.PaintRectMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
var
  P: TD2Path;
  CP1, CP2, Pt: TD2Point;
  i: integer;
begin
  if FPress and (Length(FSegment) > 3) then
  begin
    P := TD2Path.Create(Self);
    P.Parent := PaintRect;
    P.WrapMode := d2PathOriginal;
    P.Align := vaClient;
    P.StrokeThickness := CompoundTrackBar1.Value;
    P.StrokeCap := d2CapRound;
    P.StrokeJoin := d2JoinRound;
    P.Stroke.SolidColor := $FF000000 or random($FFFFFF);
    P.Fill.Style := d2BrushNone;
    P.HitTest := false;
    // add LastPoint
    SetLength(FSegment, Length(FSegment) + 1);
    FSegment[High(FSegment)] := FLAstPoint;
    // correct points to StrokeThickness
    for i := 0 to High(FSegment) do
    begin
      FSegment[i].X := FSegment[i].X - (CompoundTrackBar1.Value / 2);
      FSegment[i].Y := FSegment[i].Y - (CompoundTrackBar1.Value / 2);
    end;
    // add first
    P.Data.MoveTo(FSegment[0]);
    P.Data.LineTo(FSegment[1]);
    // create Path
    i := 2;
    while i < High(FSegment) - 1 do
    begin
      // interpolate Curve
      CurveFromPoint(FSegment[i - 1], FSegment[i], FSegment[i + 1], FSegment[i + 2], CP1, CP2);
      P.Data.CurveTo(CP1, CP2, FSegment[i + 1]);
      Inc(i, 2);
    end;
    // add last
    P.Data.LineTo(FSegment[High(FSegment) - 1]);
//    P.Data.LineTo(FSegment[High(FSegment)]);

    SetLength(FSegment, 0);
  end;
  FPress := false;
end;

procedure TForm15.PaintRectMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y, Dx, Dy: Single);
begin
  if FPress then
  begin
    if (Length(FSegment) > 0) and (d2VectorLength(d2VectorSubtract(d2Vector(X, Y), d2Vector(FSegment[High(FSegment)]))) < 10) then
    begin
      FLastPoint := d2Point(X, Y);
    end
    else
    begin
      SetLength(FSegment, Length(FSegment) + 1);
      FSegment[High(FSegment)] := d2Point(X, Y);
    end;
    // Forece Update
    PaintRect.Repaint;
    // Force Repaint
    Application.ProcessMessages;
  end;
end;

procedure TForm15.PaintRectPaint(Sender: TObject; const aCanvas: TD2Canvas;
  const ARect: TD2Rect);
var
  S: integer;
begin
  if Length(FSegment) = 0 then Exit;
  { Edit polygon drawing }
  S := aCanvas.SaveCanvas;
  aCanvas.IntersectClipRect(d2Rect(1, 1, PaintRect.Width - 1, PaintRect.Height - 1));

  aCanvas.StrokeThickness := 1;
  aCanvas.Stroke.Style := d2BrushSolid;
  aCanvas.Stroke.Color := '#FF1d7e96';
  aCanvas.StrokeCap := d2CapRound;
  aCanvas.StrokeJoin := d2JoinRound;
  aCanvas.DrawPolygon(FSegment, 1);

  aCanvas.RestoreCanvas(S);
end;

end.
