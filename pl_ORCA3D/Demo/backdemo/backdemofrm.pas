{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit backdemofrm;

{$mode objfpc}{$H+}

interface

uses
  LResources,
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d;

type

  { TfrnBackDemo }

  TfrnBackDemo = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    GUIImage1: TD3GUIImage;
    Camera1: TD3Camera;
    Light1: TD3Light;
    Dummy1: TD3Dummy;
    FloatAnimation1: TD3FloatAnimation;
    Text3D1: TD3Text3D;
    procedure Root1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure Text3D1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: single; rayPos, rayDir: TD3Vector);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frnBackDemo: TfrnBackDemo;

implementation

{$R *.lfm}

procedure TfrnBackDemo.Root1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  FloatAnimation1.Pause := not FloatAnimation1.Pause;
end;

procedure TfrnBackDemo.Text3D1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: single; rayPos, rayDir: TD3Vector);
begin
  FloatAnimation1.Pause:= not FloatAnimation1.Pause;
end;

end.
