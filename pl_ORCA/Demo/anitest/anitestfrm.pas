{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit anitestfrm;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TForm19 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    PlotGrid1: TD2PlotGrid;
    Label1: TD2Label;
    aniBox: TD2StringListBox;
    Label2: TD2Label;
    intBox: TD2StringListBox;
    PlotSize: TD2Layout;
    Button1: TD2Button;
    Ellipse1: TD2Ellipse;
    testAni: TD2FloatAnimation;
    d2Resources1: TD2Resources;
    procedure PlotGrid1Paint(Sender: TObject; const aCanvas: TD2Canvas;  const ARect: TD2Rect);
    procedure intBoxChange(Sender: TObject);
    procedure aniBoxChange(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form19: TForm19;

implementation

{$R *.lfm}

procedure TForm19.PlotGrid1Paint(Sender: TObject; const aCanvas: TD2Canvas;
  const ARect: TD2Rect);
var
  i: integer;
  P: TD2Polygon;
  x, y: double;
begin
  SetLength(P, 200);
  for i := 0 to High(P) do
  begin
    // calc only in PlotGrid area
    x := ((i / High(P)) * PlotSize.Width);
    // formula here
    case TD2InterpolationType(intBox.ItemIndex) of
      d2InterpolationLinear: y := d2InterpolateLinear(x, 0, PlotSize.Height, PlotSize.Width);
      d2InterpolationQuadratic: y := d2InterpolateQuad(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationCubic: y := d2InterpolateCubic(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationQuartic: y := d2InterpolateQuart(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationQuintic: y := d2InterpolateQuint(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationSinusoidal: y := d2InterpolateSine(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationExponential: y := d2InterpolateExpo(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationCircular: y := d2InterpolateCirc(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationElastic: y := d2InterpolateElastic(x, 0, PlotSize.Height, PlotSize.Width, 0, 0, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationBack: y := d2InterpolateBack(x, 0, PlotSize.Height, PlotSize.Width, 0, TD2AnimationType(aniBox.ItemIndex));
      d2InterpolationBounce: y := d2InterpolateBounce(x, 0, PlotSize.Height, PlotSize.Width, TD2AnimationType(aniBox.ItemIndex));
    end;
    //
    P[i] := d2Point(PlotSize.Position.X + x, PlotSize.Position.Y + y);
  end;
  aCanvas.Stroke.Style := d2BrushSolid;
  aCanvas.StrokeThickness := 2;
  aCanvas.Stroke.Color := '#FFFF0000';
  aCanvas.DrawPolygon(P, 1);
end;

procedure TForm19.intBoxChange(Sender: TObject);
begin
  PlotGrid1.Repaint;
end;

procedure TForm19.aniBoxChange(Sender: TObject);
begin
  PlotGrid1.Repaint;
end;

procedure TForm19.Button1Click(Sender: TObject);
begin
  testAni.AnimationType := TD2AnimationType(aniBox.ItemIndex);
  testAni.Interpolation := TD2InterpolationType(intBox.ItemIndex);
  testAni.Start;
end;

end.
