{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit cubefrm1;

{$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d;

type
  TfrmCube = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    ScreenDummy1: TD3ScreenDummy;
    layerFront: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Background1: TD2Background;
    layerRight: TD3GUIScene2DLayer;
    Root3: TD2Layout;
    Rectangle1: TD2Rectangle;
    Toolbar: TD3GUIScene2DLayer;
    Root4: TD2Layout;
    TrackBar1: TD2TrackBar;
    Button2: TD2Button;
    TrackBar2: TD2TrackBar;
    layerBack: TD3GUIScene2DLayer;
    layerLeft: TD3GUIScene2DLayer;
    Root5: TD2Layout;
    Rectangle2: TD2Rectangle;
    TextBox1: TD2TextBox;
    Root6: TD2Layout;
    Rectangle3: TD2Rectangle;
    ScrollBar1: TD2ScrollBar;
    Label1: TD2Label;
    GUIText1: TD3GUIText;
    GUIImage1: TD3GUIImage;
    procedure TrackBar1Change(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmCube: TfrmCube;

implementation

{$R *.lfm}

procedure TfrmCube.TrackBar1Change(Sender: TObject);
begin
  ScreenDummy1.RotateAngle.Y := TrackBar1.Value;
end;

procedure TfrmCube.FormResize(Sender: TObject);
begin
  ScreenDummy1.Position.X := ClientWidth div 2;
  ScreenDummy1.Position.Y := ClientHeight div 2;
end;

procedure TfrmCube.d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
begin
  TrackBar1.Value := TrackBar1.Value + (WheelDelta / 60)
end;

end.
