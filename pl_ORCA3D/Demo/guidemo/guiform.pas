{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


unit guiform;

{$mode objfpc}{$H+}

interface

uses

  LResources, SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d,
  ExtCtrls;

type
  TfrmGuiDemo = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Layer: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    HudButton1: TD2Button;
    frontRotate: TD3FloatAnimation;
    frontZ: TD3FloatAnimation;
    Front: TD2Background;
    Back: TD2Background;
    HudButton2: TD2Button;
    GroupBox1: TD2GroupBox;
    CheckBox1: TD2CheckBox;
    RadioButton1: TD2RadioButton;
    HudTextBox1: TD2TextBox;
    HudTrackBar1: TD2TrackBar;
    HudAngleButton1: TD2AngleButton;
    Text1: TD2Text;
    ShadowEffect1: TD2ShadowEffect;
    modernStyle: TD2Resources;
    Panel1: TD2Panel;
    AniIndicator1: TD2AniIndicator;
    PopupBox1: TD2PopupBox;
    Label1: TD2Label;
    vistaStyle: TD2Resources;
    Button1: TD2Button;
    BitmapButton1: TD2BitmapButton;
    CheckBox2: TD2CheckBox;
    Expander1: TD2Expander;
    GroupBox2: TD2GroupBox;
    Memo1: TD2Memo;
    NumberBox1: TD2NumberBox;
    ProgressBar1: TD2ProgressBar;
    TrackBar1: TD2TrackBar;
    TextBox1: TD2TextBox;
    SpeedButton1: TD2SpeedButton;
    ScrollBar1: TD2ScrollBar;
    Label2: TD2Label;
    airStyle: TD2Resources;
    Image1: TD2Image;
    Timer1: TTimer;
    CompoundAngleBar1: TD2CompoundAngleBar;
    CompoundTrackBar1: TD2CompoundTrackBar;
    procedure HudButton1Click(Sender: TObject);
    procedure HudButton2Click(Sender: TObject);
    procedure PopupBox1Change(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmGuiDemo: TfrmGuiDemo;

implementation

{$R *.lfm}

procedure TfrmGuiDemo.HudButton1Click(Sender: TObject);
begin
  Layer.AnimateFloat('Opacity', 0.5, 0.5);
  Layer.AnimateFloat('RotateAngle.Y', -90, 0.5);
  Layer.AnimateFloatWait('Position.Z', 300, 0.5);
  Front.Visible := false;

  Back.Visible := true;
  Layer.AnimateFloat('Opacity', 1, 0.5);
  Layer.RotateAngle.Y := 90;
  Layer.AnimateFloat('RotateAngle.Y', 0, 0.5);
  Layer.AnimateFloatWait('Position.Z', 0, 0.5);
end;

procedure TfrmGuiDemo.HudButton2Click(Sender: TObject);
begin
  Layer.AnimateFloat('RotateAngle.X', 90, 0.5);
  Layer.AnimateFloat('Opacity', 0.5, 0.5);
  Layer.AnimateFloatWait('Position.Z', 300, 0.5);
  Back.Visible := false;

  Front.Visible := true;
  Layer.RotateAngle.X := -90;
  Layer.AnimateFloat('RotateAngle.X', 0, 0.5);
  Layer.AnimateFloat('Opacity', 1, 0.5);
  Layer.AnimateFloatWait('Position.Z', 0, 0.5);
end;

procedure TfrmGuiDemo.PopupBox1Change(Sender: TObject);
begin
  case PopupBox1.ItemIndex of
    0: Layer.Style := nil;
    1: Layer.Style := modernStyle;
    2: Layer.Style := vistaStyle;
    3: Layer.Style := airStyle;
  end;
end;

procedure TfrmGuiDemo.TrackBar1Change(Sender: TObject);
begin
  ProgressBar1.Value := TrackBar1.Value * 100;
end;

end.
