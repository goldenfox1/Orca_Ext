{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit hud3dfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d;

type
  TfrmHud3D = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    GuiLayer: TD3GUIScene2DLayer;
    Root3: TD2Layout;
    HudWindow1: TD2HudWindow;
    HudButton1: TD2HudButton;
    HudButton2: TD2HudButton;
    HudButton4: TD2HudButton;
    HudButton3: TD2HudButton;
    HudButton5: TD2HudButton;
    Layout1: TD2Layout;
    procedure Root1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure HudButton1Click(Sender: TObject);
    procedure HudButton2Click(Sender: TObject);
    procedure HudButton3Click(Sender: TObject);
    procedure HudButton4Click(Sender: TObject);
    procedure HudButton5Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmHud3D: TfrmHud3D;

implementation

{$R *.lfm}

procedure TfrmHud3D.Root1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  d3Scene1.BeginResize;
end;

procedure TfrmHud3D.HudButton1Click(Sender: TObject);
begin
  Randomize;
  if GuiLayer.RotateAngle.Y <> 0 then
  begin
    GuiLayer.AnimateFloat('RotateAngle.Y', 0, 0.3);
    GuiLayer.AnimateFloatWait('Position.Z', 0, 0.3);
  end;
  { rotate }
  GuiLayer.AnimateFloat('Position.Z', 300, 0.3);
  GuiLayer.AnimateFloatWait('RotateAngle.X', 90, 0.3);
  { flip }
  GuiLayer.RotateAngle.X := 270;
  Hudwindow1.Fill.Color := d2ColorToStr((d2StrToColor(Hudwindow1.Fill.Color) and $FF000000) or random($FFFFFF));
  { restore }
  GuiLayer.AnimateFloat('Position.Z', 0, 0.3);
  GuiLayer.AnimateFloat('RotateAngle.X', 360, 0.3);
end;

procedure TfrmHud3D.HudButton2Click(Sender: TObject);
begin
  ShowBrushDialog(Hudwindow1.Fill, [d2BrushSolid, d2BrushGradient, d2BrushBitmap], false);
end;

procedure TfrmHud3D.HudButton3Click(Sender: TObject);
begin
  GuiLayer.AnimateFloat('RotateAngle.Y', 20, 0.3);
  GuiLayer.AnimateFloat('Position.Z', 50, 0.3);
end;

procedure TfrmHud3D.HudButton4Click(Sender: TObject);
begin
  GuiLayer.AnimateFloat('RotateAngle.Y', -20, 0.3);
  GuiLayer.AnimateFloat('Position.Z', 50, 0.3);
end;

procedure TfrmHud3D.HudButton5Click(Sender: TObject);
begin
  GuiLayer.AnimateFloat('RotateAngle.Y', 0, 0.3);
  GuiLayer.AnimateFloat('Position.Z', 0, 0.3);
end;

end.
