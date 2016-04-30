{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit clockfrm;

{$mode objfpc}{$H+}

interface

uses

  LCLProc, LCLIntf,  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, Math,
  orca_scene2d, orca_scene3d;

type
  Tfrmd3Clock = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Face: TD3Dummy;
    Light1: TD3Light;
    Camera1: TD3Camera;
    edge: TD3Dummy;
    Dummy3: TD3Dummy;
    Dummy4: TD3Dummy;
    arrowH: TD3Dummy;
    arrowHour: TD3Dummy;
    arrowMin: TD3Dummy;
    Dummy5: TD3Dummy;
    arrowSec: TD3Dummy;
    Dummy6: TD3Dummy;
    Timer1: TTimer;
    dayText: TD3Text3D;
    cameraz: TD3Dummy;
    camerax: TD3Dummy;
    Back: TD3Dummy;
    Clock: TD3Dummy;
    Text3D1: TD3Text3D;
    Sphere1: TD3Sphere;
    ColorAnimation1: TD3ColorAnimation;
    ColorAnimation2: TD3ColorAnimation;
    Text3D2: TD3Text3D;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    HudTrackBar1: TD2HudTrackBar;
    procedure Timer1Timer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure d3Scene1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure Sphere1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure HudTrackBar1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    Down: TPoint;
  end;

var
  frmd3Clock: Tfrmd3Clock;

implementation

{$R *.lfm}

procedure Tfrmd3Clock.Timer1Timer(Sender: TObject);
var
  H, M, S, MS: word;
begin
  { set times }
  DecodeTime(Time, H, M, S, MS);
  arrowSec.RotateAngle.Z := -90 + 360 * (S / 60) + (6 * (MS / 1000));
  arrowMin.RotateAngle.Z := -90 + 360 * (M / 60);
  arrowHour.RotateAngle.Z := -90 + 360 * ((H mod 12) / 12);
  DecodeDate(Date, H, M, S);
  dayText.Text := IntToStr(S);
//  Text3D1.Text := IntToStr(Round(d3Scene1.Fps));
end;

procedure Tfrmd3Clock.FormCreate(Sender: TObject);
begin
  Timer1Timer(Self);
end;

procedure Tfrmd3Clock.d3Scene1MouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
begin
  if ssRight in Shift then
  begin
    { rotate Z }
    CameraZ.RotateAngle.Z := CameraZ.RotateAngle.Z + ((X - Down.X) * 0.3);
    { rotate X }
    CameraX.RotateAngle.X := CameraX.RotateAngle.X + ((Y - Down.Y) * 0.3);
    Down := Point(X, Y);
  end;
end;

procedure Tfrmd3Clock.d3Scene1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbRight then
  begin
    Down := Point(X, Y);
    SetCapture(d3Scene1.Handle);
  end;
end;

procedure Tfrmd3Clock.d3Scene1MouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbRight then
    ReleaseCapture;
end;

procedure Tfrmd3Clock.d3Scene1MouseWheel(Sender: TObject;
  Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint;
  var Handled: Boolean);
var
  scale: single;
begin
  scale := Clock.Scale.X + (1 / (WheelDelta * 30)) * 3;
  if scale < 0.3 then scale := 0.3;
  if scale > 1 then scale := 1;
  Clock.Scale.Point := d3Point(scale, scale, scale);
end;

procedure Tfrmd3Clock.Sphere1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  Close;
end;

procedure Tfrmd3Clock.HudTrackBar1Change(Sender: TObject);
begin
  Clock.Opacity := HudTrackBar1.Value;
end;

end.
