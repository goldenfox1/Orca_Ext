{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit layerfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d;

type
  TfrmLayered = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    ObjectLayout: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    ShadowEffect1: TD2ShadowEffect;
    Light1: TD3Light;
    CloseButton1: TD2CloseButton;
    HudTrackBar1: TD2HudTrackBar;
    HudLabel1: TD2HudLabel;
    Rectangle2: TD2Rectangle;
    Text3D1: TD3Text3D;
    FloatAnimation1: TD3FloatAnimation;
    Cube1: TD3Cube;
    Sphere1: TD3Sphere;
    FloatAnimation2: TD3FloatAnimation;
    BottomGui: TD3GUIScene2DLayer;
    Root3: TD2Layout;
    Rectangle3: TD2Rectangle;
    Text1: TD2Text;
    GlowEffect1: TD2GlowEffect;
    ColorAnimation1: TD3ColorAnimation;
    ColorAnimation2: TD3ColorAnimation;
    Path3D1: TD3Path3D;
    FloatAnimation3: TD3FloatAnimation;
    FloatAnimation4: TD3FloatAnimation;
    FloatAnimation5: TD3FloatAnimation;
    Dummy1: TD3Dummy;
    Cylinder1: TD3Cylinder;
    FloatAnimation6: TD3FloatAnimation;
    procedure Sphere1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmLayered: TfrmLayered;

implementation

{$R *.lfm}

procedure TfrmLayered.Sphere1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  Application.Terminate;
end;

end.
