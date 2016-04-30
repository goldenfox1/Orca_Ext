{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit ctrlsform;

{$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs,
  orca_scene3d, orca_scene2d;

type
  TfrmCtrlsDemo = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Light1: TD3Light;
    Camera1: TD3Camera;
    d3BitmapList1: TD3BitmapList;
    cameraRot: TD3FloatAnimation;
    GuiLayer: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Text3D1: TD3Text3D;
    FloatAnimation1: TD3FloatAnimation;
    ScaleTrack: TD2TrackBar;
    ControlRoot: TD2Layout;
    ScaleRoot: TD2Layout;
    ScrollBox1: TD2ScrollBox;
    ToolbarLayer: TD3GUIScene2DLayer;
    Root3: TD2Layout;
    ToolBar2: TD2ToolBar;
    opacityBar: TD2CompoundTrackBar;
    rotateBar: TD2CompoundAngleBar;
    depthBar: TD2CompoundTrackBar;
    StylePopup: TD2PopupBox;
    Label18: TD2Label;
    modernStyle: TD2Resources;
    airStyle: TD2Resources;
    vistaStyle: TD2Resources;
    ValueLabel1: TD2ValueLabel;
    ListTransform: TD2StringListBox;
    TrackBar2: TD2TrackBar;
    TrackBar3: TD2TrackBar;
    TextBox4: TD2TextBox;
    PathCheckBox1: TD2PathCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure ScaleTrackChange(Sender: TObject);
    procedure opacityBarChange(Sender: TObject);
    procedure rotateBarChange(Sender: TObject);
    procedure depthBarChange(Sender: TObject);
    procedure StylePopupChange(Sender: TObject);
    procedure TrackBar2Change(Sender: TObject);
    procedure TrackBar3Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmCtrlsDemo: TfrmCtrlsDemo;

implementation

{$R *.lfm}

procedure TfrmCtrlsDemo.FormCreate(Sender: TObject);
var
  i: integer;
begin
  inherited;
  for i := 0 to 50 do
    with TD2Rectangle.Create(Self) do
    begin
      parent := ScrollBox1;
      width := (30 + random(150));
      height := (30 + random(150));
      hittest := false;
      Position.x := random(1300);
      Position.y := random(1300);
      xRadius := random(20);
      yRadius := xRadius;
      hitTest := false;
      fill.SolidColor := ((50 + random(205)) shl 24) or random($FFFFFF);
    end;
end;

procedure TfrmCtrlsDemo.ScaleTrackChange(Sender: TObject);
begin
  ControlRoot.Scale.Point := d2Point(ScaleTrack.Value / 100, ScaleTrack.Value / 100);
end;

procedure TfrmCtrlsDemo.opacityBarChange(Sender: TObject);
begin
  GuiLayer.Opacity := opacityBar.Value;
end;

procedure TfrmCtrlsDemo.rotateBarChange(Sender: TObject);
begin
  GuiLayer.RotateAngle.Y := rotateBar.Value;
end;

procedure TfrmCtrlsDemo.depthBarChange(Sender: TObject);
begin
  GuiLayer.Position.Z := depthBar.Value;
end;

procedure TfrmCtrlsDemo.StylePopupChange(Sender: TObject);
begin
  case StylePopup.ItemIndex of
    0: GuiLayer.Style := nil;
    1: GuiLayer.Style := modernStyle;
    2: GuiLayer.Style := vistaStyle;
    3: GuiLayer.Style := airStyle;
  end;
end;

procedure TfrmCtrlsDemo.TrackBar2Change(Sender: TObject);
begin
  ListTransform.RotateAngle := TrackBar2.Value;
  TextBox4.RotateAngle := TrackBar2.Value;
end;

procedure TfrmCtrlsDemo.TrackBar3Change(Sender: TObject);
begin
  ListTransform.Opacity := TrackBar3.Value;
  TextBox4.Opacity := TrackBar3.Value;
end;

end.
