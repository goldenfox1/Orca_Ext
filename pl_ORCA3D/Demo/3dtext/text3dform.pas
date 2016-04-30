{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit text3dform;

{$mode objfpc}{$H+}

interface

uses
  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls,
  orca_scene3d, orca_scene2d;

type

  { TForm7 }

  TForm7 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    FontDialog1: TFontDialog;
    camerax: TD3Dummy;
    Camera1: TD3Camera;
    Text3D1: TD3Text3D;
    Light1: TD3Light;
    BitmapObject1: TD3BitmapObject;
    Timer1: TTimer;
    Layer2D: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    HudWindow1: TD2HudWindow;
    HudTextBox1: TD2HudTextBox;
    HudButton1: TD2HudButton;
    HudButton2: TD2HudButton;
    Label1: TD2Label;
    HudTrackBar1: TD2HudTrackBar;
    Label2: TD2Label;
    CheckBox1: TD2CheckBox;
    cameraz: TD3Dummy;
    Label3: TD2Label;
    HudTrackBar2: TD2HudTrackBar;
    procedure Button1Click(Sender: TObject);
    procedure d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure TextBox1Change(Sender: TObject);
    procedure d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure Button2Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure HudWindow1CloseClick(Sender: TObject);
    procedure HudTrackBar1Change(Sender: TObject);
    procedure CheckBox1Change(Sender: TObject);
    procedure HudTrackBar2Change(Sender: TObject);
  private
    { Private declarations }
    Down: TD3Point;
  public
    { Public declarations }
  end;

var
  Form7: TForm7;

implementation

uses orca_editor_material;

{$R *.lfm}

procedure TForm7.Button1Click(Sender: TObject);
begin
  FontDialog1.Font.Name := Text3D1.Font.Family;
  if Text3D1.Font.Style in [d3FontBold, d3FontBoldItalic] then
    FontDialog1.Font.Style := FontDialog1.Font.Style + [fsBold];
  if Text3D1.Font.Style in [d3FontItalic, d3FontBoldItalic] then
    FontDialog1.Font.Style := FontDialog1.Font.Style + [fsItalic];
  if FontDialog1.Execute then
  begin
    Text3D1.Font.Family := FontDialog1.Font.Name;
    if (fsBold in FontDialog1.Font.Style) and (fsItalic in FontDialog1.Font.Style) then
      Text3D1.Font.Style := d3FontBoldItalic;
    if (fsBold in FontDialog1.Font.Style) and not (fsItalic in FontDialog1.Font.Style) then
      Text3D1.Font.Style := d3FontBold;
    if not (fsBold in FontDialog1.Font.Style) and (fsItalic in FontDialog1.Font.Style) then
      Text3D1.Font.Style := d3FontItalic;
    if not (fsBold in FontDialog1.Font.Style) and not (fsItalic in FontDialog1.Font.Style) then
      Text3D1.Font.Style := d3FontRegular;
  end;
end;

procedure TForm7.d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if ssRight in Shift then
  begin
    cameraz.RotateAngle.Z := cameraz.RotateAngle.Z + (X - Down.X);
    camerax.RotateAngle.X := camerax.RotateAngle.X + (Y - Down.Y);
    Down := d3Point(X, Y, 0);
  end;
end;

procedure TForm7.d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  Down := d3Point(X, Y, 0);
end;

procedure TForm7.FormCreate(Sender: TObject);
var
  R: TD2Rectangle;
begin
  { }
  exit;
  R := TD2Rectangle.Create(Self);
  R.Parent := Root2;
  R.Align := vaClient;
  R.StrokeThickness := 3;
  R.fill.Style := d2BrushGradient;
  R.xRadius := 8;
  R.yRadius := 8;
end;

procedure TForm7.TextBox1Change(Sender: TObject);
begin
  Text3D1.Text := HudTextBox1.Text;
end;

procedure TForm7.d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
begin
  Camera1.Position.Vector := d3VectorAdd(Camera1.Position.Vector, d3VectorScale(d3Vector(0, 1, 0),
    (WheelDelta/30) * 0.3));
end;

procedure TForm7.Button2Click(Sender: TObject);
begin
  {}
  frmMaterialDesign := TfrmMaterialDesign.Create(Application);
  frmMaterialDesign.Material := Text3D1.Material;
  frmMaterialDesign.ParentScene := d3Scene1;
  frmMaterialDesign.ShowModal;
  frmMaterialDesign.Free;
end;

procedure TForm7.Timer1Timer(Sender: TObject);
begin
  Caption := FloattoStr(d3Scene1.fps);
end;

procedure TForm7.HudWindow1CloseClick(Sender: TObject);
begin
  case random(2) of
    1: Layer2D.AnimateFloat('RotateAngle.Y', Layer2D.RotateAngle.Y + 360, 1);
  else
    Layer2D.AnimateFloat('RotateAngle.X', Layer2D.RotateAngle.X + 360, 1);
  end;
end;

procedure TForm7.HudTrackBar1Change(Sender: TObject);
begin
  Text3D1.Depth := HudTrackBar1.Value;
end;

procedure TForm7.CheckBox1Change(Sender: TObject);
begin
  if CheckBox1.IsChecked then
    Text3D1.Material.FillMode := d3Wireframe
  else
    Text3D1.Material.FillMode := d3Solid;
end;

procedure TForm7.HudTrackBar2Change(Sender: TObject);
begin
  Text3D1.Flatness := HudTrackBar2.Value;
end;

end.
