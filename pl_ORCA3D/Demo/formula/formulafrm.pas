{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit formulafrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d,  orca_scene2d;

type
  TfrmFomula = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Camera1: TD3Camera;
    Light1: TD3Light;
    Mesh1: TD3Mesh;
    cameraDummyZ: TD3Dummy;
    cameraDummyX: TD3Dummy;
    Screend2Layer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    CheckBox1: TD2CheckBox;
    procedure FormCreate(Sender: TObject);
    procedure d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure CheckBox1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    Down: TD3Point;
  end;

var
  frmFomula: TfrmFomula;

implementation

uses math;

{$R *.lfm}

procedure TfrmFomula.FormCreate(Sender: TObject);
const
  Res = 80;
var
  x, y: integer;
  invRes, h, xx, yy: single;
begin
  { Create mesh at runtime }

  invRes := 20;

  { calc vertices }
  SetLength(Mesh1.Data.MeshVertices, Res * Res);
  for y := 0 to Res - 1 do
    for x := 0 to Res - 1 do
    begin
      // calc x,y in (-1..1)
      xx := (-0.5 + (x / (Res - 1)));
      yy := (-0.5 + (y / (Res - 1)));
      // calc h - using formula
      h := -sin(Pi + (xx * xx + yy * yy) * invRes) * 0.5;
      // add Vertex - all coords must be in -0.5..0.5
      Mesh1.Data.MeshVertices[x + (y * Res)] := TexVertexNormal(xx, yy, h, 0, 0, 1, xx, 1 - yy);
    end;
  SetLength(Mesh1.Data.MeshIndices, Res * Res * 6);
  { create triangles }
  for y := 0 to Res - 2 do
    for x := 0 to Res - 2 do
    begin
      Mesh1.Data.MeshIndices[(x + (y * Res)) * 6 + 0] := x + (y * Res);
      Mesh1.Data.MeshIndices[(x + (y * Res)) * 6 + 2] := x + 1 + (y * Res);
      Mesh1.Data.MeshIndices[(x + (y * Res)) * 6 + 1] := x + ((y + 1) * Res);
      Mesh1.Data.MeshIndices[(x + (y * Res)) * 6 + 3] := x + ((y + 1) * Res);
      Mesh1.Data.MeshIndices[(x + (y * Res)) * 6 + 5] := x + 1 + (y * Res);
      Mesh1.Data.MeshIndices[(x + (y * Res)) * 6 + 4] := x + 1 + ((y + 1) * Res);
    end;
  { calc normals }
  Mesh1.Data.CalcNormals;
end;

procedure TfrmFomula.d3Scene1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  Down := d3Point(X, Y, 0);
end;

procedure TfrmFomula.d3Scene1MouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  if ssRight in Shift then
  begin
    cameraDummyZ.RotateAngle.Z := cameraDummyZ.RotateAngle.Z + (X - Down.X);
    cameraDummyX.RotateAngle.X := cameraDummyX.RotateAngle.X + (Y - Down.Y);
    Down := d3Point(X, Y, 0);
  end;
end;

procedure TfrmFomula.d3Scene1MouseWheel(Sender: TObject;
  Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint;
  var Handled: Boolean);
begin
  Camera1.Position.Vector := d3VectorAdd(Camera1.Position.Vector, d3VectorScale(d3Vector(0, 1, 0),
    (WheelDelta{$ifndef FPC}/30{$endif}) * 0.3));
end;

procedure TfrmFomula.CheckBox1Change(Sender: TObject);
begin
  if CheckBox1.IsChecked then
    Mesh1.Material.FillMode := d3Wireframe
  else
    Mesh1.Material.FillMode := d3Solid;
end;

end.
