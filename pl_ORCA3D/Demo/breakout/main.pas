{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit main;

{$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, ExtCtrls,
  orca_scene2d;

type
  TForm4 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    SceneBitmap: TD3BitmapObject;
    Location: TD3Dummy;
    Camera1: TD3Camera;
    Light1: TD3Light;
    d3BitmapList1: TD3BitmapList;
    Cube1: TD3Cube;
    Scene: TD3Dummy;
    Cube2: TD3Cube;
    Cube3: TD3Cube;
    Bat: TD3Cube;
    Ball: TD3Sphere;
    RoundCube1: TD3RoundCube;
    Timer1: TTimer;
    Sphere1: TD3Sphere;
    Trigger: TD3Cube;
    RoundCube2: TD3RoundCube;
    RoundCube3: TD3RoundCube;
    RoundCube4: TD3RoundCube;
    RoundCube5: TD3RoundCube;
    RoundCube6: TD3RoundCube;
    RoundCube7: TD3RoundCube;
    ObjectLayer1: TD3ObjectLayer;
    Root2: TD2Layout;
    Rectangle2: TD2Rectangle;
    Text1: TD2Text;
    Text2: TD2Text;
    Rectangle3: TD2Rectangle;
    Back: TD3GUIImage;
    procedure d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure BallCollision(Sender: TObject;
      CollisionObject: TD3VisualObject; Point, Normal: TD3Vector);
    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
    Old3: single;
  public
    { Public declarations }
  end;

var
  Form4: TForm4;

implementation

{$R *.lfm}

procedure TForm4.d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if Old3 = 0 then Old3 := X;
  Bat.Position.X := Bat.Position.X + (X - Old3) * 0.05;
  Bat.Position.X := ((x - (d3Scene1.Width / 2)) / d3Scene1.Width) * 10;
  if Bat.Position.X < -5 then
    Bat.Position.X := -5;
  if Bat.Position.X > 5 then
    Bat.Position.X := 5;
  Old3 := X;
end;

procedure TForm4.d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  P: TD3Vector;
begin
  if Ball.Parent = Bat then
  begin
    P := Ball.AbsolutePosition;
    Ball.Position.Vector := Ball.AbsolutePosition;
    Ball.Parent := Scene;
    Ball.Velocity.Vector := d3Vector(0, 5, 0);
  end;
end;

procedure TForm4.BallCollision(Sender: TObject;
  CollisionObject: TD3VisualObject; Point, Normal: TD3Vector);
begin
  { ball collise }
  // first back to disable collision
  Ball.Back;
  if CollisionObject = Bat then
  begin
    // reflect velocity
    Ball.Velocity.Vector := Ball.AbsoluteToLocalVector(d3Vector((Ball.Position.X - Bat.Position.X) * 3, 4, 0, 0));
  end
  else
  if CollisionObject = Trigger then
  begin
    // ball death
    Ball.Parent := Bat;
    Ball.Position.Vector := d3Vector(0, 0.6, 0);
    Ball.Velocity.Vector := d3Vector(0, 0, 0);
  end
  else
  if CollisionObject.Tag = 1 then
  begin
    //
    if CollisionObject.Dynamic then
      CollisionObject.AddForce(d3VectorScale(Ball.LocalToAbsoluteVector(Ball.Velocity.Vector), 1))
    else
    begin
      CollisionObject.Opacity := 0;
      CollisionObject.Free;
    end;
    // reflect velocity
    Normal.W := 0;
    Normal.Z := 0;
    Normal := Ball.AbsoluteToLocalVector(Normal);
    Ball.Velocity.Vector := d3VectorReflect(Ball.Velocity.Vector, Normal);
  end
  else
  begin
    // reflect velocity
    Normal.W := 0;
    Normal.Z := 0;
    Normal := Ball.AbsoluteToLocalVector(Normal);
    Ball.Velocity.Vector := d3VectorReflect(Ball.Velocity.Vector, Normal);
  end;
end;

procedure TForm4.Timer1Timer(Sender: TObject);
begin
  Text2.Text := IntToStr(Round(d3Scene1.Fps)) + ' FPS';
end;

end.
