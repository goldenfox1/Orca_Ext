{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit pathdemofrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d;

type
  TfrmPathDemo = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Toolbar: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    cameraz: TD3Dummy;
    camerax: TD3Dummy;
    Camera1: TD3Camera;
    Light1: TD3Light;
    Path3D: TD3Path3D;
    Rectangle1: TD2Rectangle;
    PathList: TD2HudListBox;
    HudLabel1: TD2HudLabel;
    ListBoxItem1: TD2ListBoxItem;
    Path1: TD2Path;
    Path2: TD2Path;
    ListBoxItem2: TD2ListBoxItem;
    ListBoxItem3: TD2ListBoxItem;
    ListBoxItem4: TD2ListBoxItem;
    ListBoxItem5: TD2ListBoxItem;
    ListBoxItem6: TD2ListBoxItem;
    ListBoxItem7: TD2ListBoxItem;
    ListBoxItem8: TD2ListBoxItem;
    ListBoxItem9: TD2ListBoxItem;
    ListBoxItem10: TD2ListBoxItem;
    ListBoxItem11: TD2ListBoxItem;
    ListBoxItem12: TD2ListBoxItem;
    ListBoxItem13: TD2ListBoxItem;
    ListBoxItem14: TD2ListBoxItem;
    ListBoxItem15: TD2ListBoxItem;
    ListBoxItem16: TD2ListBoxItem;
    ListBoxItem17: TD2ListBoxItem;
    ListBoxItem18: TD2ListBoxItem;
    CompoundTrackBar1: TD2CompoundTrackBar;
    ColorPicker1: TD2ColorPicker;
    ColorQuad1: TD2ColorQuad;
    ColorBox1: TD2ColorBox;
    CompoundTrackBar2: TD2CompoundTrackBar;
    procedure PathListChange(Sender: TObject);
    procedure Root1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure Root1MouseMove(Sender: TObject; Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure CompoundTrackBar1Change(Sender: TObject);
    procedure ColorQuad1Change(Sender: TObject);
    procedure Root1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TD3Point; var Handled: Boolean);
    procedure CompoundTrackBar2Change(Sender: TObject);
  private
    { Private declarations }
    Down: TD3Point;
  public
    { Public declarations }
  end;

var
  frmPathDemo: TfrmPathDemo;

implementation

{$R *.lfm}

procedure TfrmPathDemo.PathListChange(Sender: TObject);
var
  P: TD2Path;
begin
  if PathList.Selected = nil then Exit;
  P := TD2Path(PathList.Selected.FindBinding('path'));
  if P <> nil then
  begin
    Path3D.Path.Assign(P.Data);
  end;
end;

procedure TfrmPathDemo.Root1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  Down := d3Point(X, Y, 0);
end;

procedure TfrmPathDemo.Root1MouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  if ssLeft in Shift then
  begin
    cameraz.RotateAngle.Z := cameraz.RotateAngle.Z + (X - Down.X);
    camerax.RotateAngle.X := camerax.RotateAngle.X + (Y - Down.Y);
    Down := d3Point(X, Y, 0);
  end;
end;

procedure TfrmPathDemo.CompoundTrackBar1Change(Sender: TObject);
begin
  Path3D.Depth := CompoundTrackBar1.Value;
end;

procedure TfrmPathDemo.ColorQuad1Change(Sender: TObject);
begin
  Path3D.Material.Diffuse := d3ColorToStr(ColorBox1.Color);
end;

procedure TfrmPathDemo.Root1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TD3Point; var Handled: Boolean);
begin
  Camera1.Position.Vector := d3VectorAdd(Camera1.Position.Vector, d3VectorScale(d3Vector(0, 1, 0),
    (WheelDelta{$ifndef FPC}/30{$endif}) * 0.3));
end;

procedure TfrmPathDemo.CompoundTrackBar2Change(Sender: TObject);
begin
  Path3D.Flatness := CompoundTrackBar2.Value;
end;

end.
