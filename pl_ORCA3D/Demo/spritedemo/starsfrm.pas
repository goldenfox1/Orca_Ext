{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit starsfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, ExtCtrls, orca_scene2d;

type
  TfrmStars = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Timer1: TTimer;
    ObjectLayer: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Text1: TD2Text;
    Rectangle1: TD2Rectangle;
    textFps: TD2Text;
    d3BitmapList1: TD3BitmapList;
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    procedure DoTick(Sender: TObject; time, deltaTime: single);
  end;

var
  frmStars: TfrmStars;

implementation

{$R *.lfm}

procedure TfrmStars.DoTick(Sender: TObject; time, deltaTime: single);
begin
  if (TD3VisualObject(Sender).Position.X < TD3VisualObject(Sender).Width / 2) then
    TD3VisualObject(Sender).Velocity.X := Abs(TD3VisualObject(Sender).Velocity.X);
  if (TD3VisualObject(Sender).Position.X > d3Scene1.Width - (TD3VisualObject(Sender).Width / 2)) then
    TD3VisualObject(Sender).Velocity.X := -Abs(TD3VisualObject(Sender).Velocity.X);
  if (TD3VisualObject(Sender).Position.Y < TD3VisualObject(Sender).Height / 2) then
    TD3VisualObject(Sender).Velocity.Y := Abs(TD3VisualObject(Sender).Velocity.Y);
  if (TD3VisualObject(Sender).Position.Y > d3Scene1.Height - (TD3VisualObject(Sender).Height / 2)) then
    TD3VisualObject(Sender).Velocity.Y := -Abs(TD3VisualObject(Sender).Velocity.Y);
end;

procedure TfrmStars.FormCreate(Sender: TObject);
var
  Star: TD3GUIScene2DLayer;
  Obj: TD3ProxyObject;
  Ellipse: TD2Shape;
  xText: TD2Text;
  i: integer;
begin
  for i := 0 to 9 do
  begin
    Star := TD3GUIScene2DLayer.Create(Self);
    Star.Fill.Style := d2BrushNone;
    Star.Parent := Root1;
    Star.Position.Point := d3Point(50 + random(d3Scene1.Width - 100), 50 + random(d3Scene1.Height - 100), 0);
    Star.Width := 32 + random(30);
    Star.Height := 32 + random(30);
    Star.Opacity := random;
    Star.Velocity.Point := d3Point(-120 + random(240), -120 + random(240), 0);
    Star.OnTick := @DoTick;
    Star.Resolution := 80;

    case random(2) of
      1: begin
        Ellipse := TD2Rectangle.Create(Self);
        TD2Rectangle(Ellipse).xRadius := random(10);
        TD2Rectangle(Ellipse).yRadius := TD2Rectangle(Ellipse).xRadius;
      end;
    else
      Ellipse := TD2Ellipse.Create(Self);
    end;

    case random(2) of
      1: begin
        Ellipse.Fill.Style := d2BrushGradient;
        Ellipse.Fill.Gradient.Style := d2RadialGradient;
        Ellipse.Fill.Gradient.Points[0].IntColor := $00FFFFFF;
        Ellipse.Fill.Gradient.Points[1].IntColor := $FF000000 or random($FFFFFF);
      end
    else
      Ellipse.Fill.Style := d2BrushSolid;
      Ellipse.Fill.SolidColor := $FF000000 or random($FFFFFF);
    end;

    Ellipse.Stroke.Style := d2BrushSolid;
    Ellipse.Stroke.SolidColor := $FF000000 or random($FFFFFF);
    Ellipse.StrokeThickness := 2;

    Star.AddObject(Ellipse);

    xText := TD2Text.Create(Self);
    xText.Parent := Ellipse;
    xText.Align := vaCenter;
    xText.Text := IntToStr(i);
  end;
  ObjectLayer.BringToFront;
end;

procedure TfrmStars.Timer1Timer(Sender: TObject);
begin
  textFps.Text := IntToStr(Trunc(d3Scene1.Fps)) + ' FPS';
end;

end.
