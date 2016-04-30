{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit widgetfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d;

type
  TfrmWidget = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    WidgetLayout: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Front: TD2Rectangle;
    Image1: TD2Image;
    Text1: TD2Text;
    HudButton1: TD2HudButton;
    AniIndicator1: TD2AniIndicator;
    Back: TD2Rectangle;
    Label1: TD2Label;
    Label2: TD2Label;
    Label3: TD2Label;
    HudTextBox1: TD2HudTextBox;
    Label4: TD2Label;
    HudButton2: TD2HudButton;
    HudButton3: TD2HudButton;
    GlowEffect1: TD2GlowEffect;
    GlowEffect2: TD2GlowEffect;
    FloatAnimation1: TD3FloatAnimation;
    CloseButton1: TD2CloseButton;
    procedure HudButton1Click(Sender: TObject);
    procedure HudButton2Click(Sender: TObject);
    procedure HudButton3Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmWidget: TfrmWidget;

implementation

{$R *.lfm}

procedure TfrmWidget.HudButton1Click(Sender: TObject);
begin
  { rotate }
  WidgetLayout.AnimateFloat('Position.Z', 300, 0.3);
  WidgetLayout.AnimateFloatWait('RotateAngle.Y', 90, 0.3);
  { flip }
  Back.Visible := true;
  Front.Visible := false;
  // change
  WidgetLayout.RotateAngle.Y := 270;

  Height := Height + 50;
  Width := Width - 100;
  // need offet because back width <
  Left := Left + 50;
  { restore }
  WidgetLayout.AnimateFloat('Position.Z', 0, 0.3);
  WidgetLayout.AnimateFloat('RotateAngle.Y', 360, 0.3);
end;

procedure TfrmWidget.HudButton2Click(Sender: TObject);
begin
  Application.Terminate;
end;

procedure TfrmWidget.HudButton3Click(Sender: TObject);
begin
  { rotate }
  WidgetLayout.AnimateFloat('Position.Z', 300, 0.3);
  WidgetLayout.AnimateFloatWait('RotateAngle.Y', -90, 0.3);
  { flip }
  Back.Visible := false;
  Front.Visible := true;
  // change
  WidgetLayout.RotateAngle.Y := 90;
  Height := Height - 50;
  Width := Width + 100;
  // need offet because front width >
  Left := Left - 50;
  { restore }
  WidgetLayout.AnimateFloat('Position.Z', 0, 0.3);
  WidgetLayout.AnimateFloat('RotateAngle.Y', 0, 0.3);
end;

end.
