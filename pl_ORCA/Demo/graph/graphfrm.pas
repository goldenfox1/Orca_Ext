{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit graphfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs,orca_scene2d,
  Menus;

type
  TForm14 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    TrackBar1: TD2TrackBar;
    PlotGrid1: TD2PlotGrid;
    Label1: TD2Label;
    Rectangle1: TD2Rectangle;
    procedure PlotGrid1Paint(Sender: TObject; const aCanvas: TD2Canvas;
      const ARect: TD2Rect);
    procedure TrackBar1Change(Sender: TObject);
    procedure PlotGrid1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TD2Point; var Handled: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form14: TForm14;

implementation

uses Math;

{$R *.lfm}

procedure TForm14.PlotGrid1Paint(Sender: TObject; const aCanvas: TD2Canvas;
  const ARect: TD2Rect);
var
  i: integer;
  x, y: single;
  P: TD2Polygon;
  SaveIndex: cardinal;
begin
  SaveIndex := aCanvas.SaveCanvas;
  aCanvas.SetClipRects([ARect]);
  { Paint sin }
  SetLength(P, 200);
  for i := 0 to High(P) do
  begin
    // calc only in PlotGrid area
    x := -(PlotGrid1.Width / 2) + ((i / High(P)) * PlotGrid1.Width);
    x := x / PlotGrid1.Frequency;
    // formula here
    y := sin(x);
    P[i] := d2Point(TD2VisualObject(Sender).Width / 2 + x*PlotGrid1.Frequency, TD2VisualObject(Sender).Height / 2 - y * PlotGrid1.Frequency);
  end;
  aCanvas.Stroke.Style := d2BrushSolid;
  aCanvas.StrokeThickness := 2;
  aCanvas.Stroke.Color := '#FFFF0000';
  aCanvas.DrawPolygon(P, 1);
  { Paint cos * x }
  SetLength(P, 500);
  for i := 0 to High(P) do
  begin
    // calc only in PlotGrid area
    x := -(PlotGrid1.Width / 2) + ((i / High(P)) * PlotGrid1.Width);
    x := x / PlotGrid1.Frequency;
    // formula here
    y := cos(x) * x;
    P[i] := d2Point(TD2VisualObject(Sender).Width / 2 + x*PlotGrid1.Frequency, TD2VisualObject(Sender).Height / 2 - y * PlotGrid1.Frequency);
  end;
  aCanvas.Stroke.Style := d2BrushSolid;
  aCanvas.StrokeThickness := 2;
  aCanvas.Stroke.Color := '#FF00FF00';
  aCanvas.DrawPolygon(P, 1);
  { Paint x * x }
  SetLength(P, 100);
  for i := 0 to High(P) do
  begin
    // calc only in PlotGrid area
    x := -(PlotGrid1.Width / 2) + ((i / High(P)) * PlotGrid1.Width);
    x := x / PlotGrid1.Frequency;
    // formula here
    y := x * x;
    P[i] := d2Point(TD2VisualObject(Sender).Width / 2 + x*PlotGrid1.Frequency, TD2VisualObject(Sender).Height / 2 - y * PlotGrid1.Frequency);
  end;
  aCanvas.Stroke.Style := d2BrushSolid;
  aCanvas.StrokeThickness := 2;
  aCanvas.Stroke.Color := '#FF0000FF';
  aCanvas.DrawPolygon(P, 1);
  { End Paint }
  aCanvas.RestoreCanvas(SaveIndex);
end;

procedure TForm14.TrackBar1Change(Sender: TObject);
begin
  PlotGrid1.Frequency := Round(TrackBar1.Value);
end;

procedure TForm14.PlotGrid1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TD2Point; var Handled: Boolean);
begin
  TrackBar1.Value := TrackBar1.Value + (WheelDelta / 120);
  Handled := true;
end;

end.
