{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit chartfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d;

type
  TForm9 = class(TForm)
    Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Camera1: TD3Camera;
    Light1: TD3Light;
    Grid1: TD3Grid;
    Cube1: TD3Cube;
    Cube2: TD3Cube;
    Cube3: TD3Cube;
    Cube4: TD3Cube;
    Grid2: TD3Grid;
    Grid3: TD3Grid;
    cameraz: TD3Dummy;
    camerax: TD3Dummy;
    Cylinder1: TD3Cylinder;
    Cylinder2: TD3Cylinder;
    Cylinder3: TD3Cylinder;
    Cylinder4: TD3Cylinder;
    Cone1: TD3Cone;
    Cone2: TD3Cone;
    Cone3: TD3Cone;
    Cone4: TD3Cone;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    HudButton1: TD2HudButton;
    procedure Scene1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure FormCreate(Sender: TObject);
    procedure HudButton1Click(Sender: TObject);
  private
    { Private declarations }
    Down: TD3Point;
  public
    { Public declarations }
  end;

var
  Form9: TForm9;

implementation

{$R *.lfm}

procedure TForm9.FormCreate(Sender: TObject);
begin
  HudButton1Click(Self);
end;

procedure TForm9.Scene1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  Down := d3Point(X, Y, 0);
end;

procedure TForm9.Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if ssRight in Shift then
  begin
    cameraz.RotateAngle.Z := cameraz.RotateAngle.Z + (X - Down.X);
    camerax.RotateAngle.X := camerax.RotateAngle.X + (Y - Down.Y);
    Down := d3Point(X, Y, 0);
  end;
end;

procedure TForm9.Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
begin
  Camera1.Position.Vector := d3VectorAdd(Camera1.Position.Vector, d3VectorScale(d3Vector(0, 1, 0),
    (WheelDelta{$ifndef FPC}/30{$endif}) * 0.3));
end;

procedure TForm9.HudButton1Click(Sender: TObject);
var
  NewSize: single;
begin
  NewSize := 1 + random(8);
  Cube1.AnimateFloat('Depth', NewSize);
  Cube1.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cube2.AnimateFloat('Depth', NewSize);
  Cube2.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cube3.AnimateFloat('Depth', NewSize);
  Cube3.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cube4.AnimateFloat('Depth', NewSize);
  Cube4.AnimateFloat('Position.Z', NewSize / 2);

  NewSize := 1 + random(8);
  Cylinder1.AnimateFloat('Width', NewSize);
  Cylinder1.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cylinder2.AnimateFloat('Width', NewSize);
  Cylinder2.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cylinder3.AnimateFloat('Width', NewSize);
  Cylinder3.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cylinder4.AnimateFloat('Width', NewSize);
  Cylinder4.AnimateFloat('Position.Z', NewSize / 2);

  NewSize := 1 + random(8);
  Cone1.AnimateFloat('Width', NewSize);
  Cone1.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cone2.AnimateFloat('Width', NewSize);
  Cone2.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cone3.AnimateFloat('Width', NewSize);
  Cone3.AnimateFloat('Position.Z', NewSize / 2);
  NewSize := 1 + random(8);
  Cone4.AnimateFloat('Width', NewSize);
  Cone4.AnimateFloat('Position.Z', NewSize / 2);
end;

end.
