{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit imagefxfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d;

type
  TForm11 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    SetupLayer: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    HudTrackBar1: TD2HudTrackBar;
    Label1: TD2Label;
    Label2: TD2Label;
    HudTrackBar2: TD2HudTrackBar;
    Label3: TD2Label;
    HudTrackBar3: TD2HudTrackBar;
    HudButton1: TD2HudButton;
    PreviewLayer: TD3ObjectLayer;
    Root4: TD2Layout;
    PreviewLayout: TD2Layout;
    Preview: TD2Rectangle;
    ReflectionEffect1: TD2ReflectionEffect;
    Label4: TD2Label;
    CheckBox1: TD2CheckBox;
    CheckBox2: TD2CheckBox;
    CheckBox3: TD2CheckBox;
    CheckBox4: TD2CheckBox;
    HudButton2: TD2HudButton;
    SaveDialog1: TSaveDialog;
    procedure HudTrackBar1Change(Sender: TObject);
    procedure HudTrackBar2Change(Sender: TObject);
    procedure HudTrackBar3Change(Sender: TObject);
    procedure HudButton1Click(Sender: TObject);
    procedure CheckBox1Change(Sender: TObject);
    procedure HudButton2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form11: TForm11;

implementation

{$R *.lfm}


procedure TForm11.HudTrackBar1Change(Sender: TObject);
begin
  Preview.xRadius := HudTrackBar1.Value;
  Preview.yRadius := HudTrackBar1.Value;
end;

procedure TForm11.HudTrackBar2Change(Sender: TObject);
begin
  Preview.StrokeThickness := HudTrackBar2.Value;
end;

procedure TForm11.HudTrackBar3Change(Sender: TObject);
begin
  PreviewLayer.RotateAngle.Z := HudTrackBar3.Value;
end;

procedure TForm11.HudButton1Click(Sender: TObject);
begin
  GvarD2BitmapEditor := TD2BitmapEditor.Create(Self);
  GvarD2BitmapEditor.AssignFromBitmap(Preview.Fill.Bitmap.Bitmap);
  if GvarD2BitmapEditor.ShowModal = mrOk then
  begin
    GvarD2BitmapEditor.AssignToBitmap(Preview.Fill.Bitmap.Bitmap);
    // call SetBounds need to corect fit Preview in Layout1 - because Preview use vaFit align
    Preview.SetBounds(0, 0, Preview.Fill.Bitmap.Bitmap.Width, Preview.Fill.Bitmap.Bitmap.Height);
    // Update reflection
    Preview.UpdateEffects;
  end;
  GvarD2BitmapEditor.Free;
end;

procedure TForm11.CheckBox1Change(Sender: TObject);
begin
  Preview.Corners := [];
  if CheckBox1.IsChecked then
    Preview.Corners := Preview.Corners + [d2CornerTopLeft];
  if CheckBox2.IsChecked then
    Preview.Corners := Preview.Corners + [d2CornerTopRight];
  if CheckBox3.IsChecked then
    Preview.Corners := Preview.Corners + [d2CornerBottomLeft];
  if CheckBox4.IsChecked then
    Preview.Corners := Preview.Corners + [d2CornerBottomRight];
  Preview.UpdateEffects;
end;

procedure TForm11.HudButton2Click(Sender: TObject);
var
  B: TD2Bitmap;
  R: TD2Rect;
begin
  if SaveDialog1.Execute then
  begin
    // get screen size of 3D Object
    R := PreviewLayer.ScreenBounds;
    // create TD2Bitmap - TD2Bitmap is a best for export because it not create any hardware resoruces
    B := TD2Bitmap.Create(trunc(d2RectWidth(R)), trunc(d2RectHeight(R)));
    // render to TD2Bitmap
    PreviewLayer.PaintToVxBitmap(B, B.Width, B.Height, $00000000{ Clear Color });
    // Save to file
    B.SaveToFile(SaveDialog1.FileName);
    // free resoruces
    B.Free;
  end;
end;

end.
