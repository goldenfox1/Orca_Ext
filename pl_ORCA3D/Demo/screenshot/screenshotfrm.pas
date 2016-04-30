{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit screenshotfrm;

{$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d, ExtCtrls;

type
  TfrmScreenshot = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    RoundCube1: TD3RoundCube;
    FloatAnimation1: TD3FloatAnimation;
    Light1: TD3Light;
    Root2: TD2Layout;
    HudButton1: TD2HudButton;
    checkBitmap: TD2HudRadioButton;
    checkd3Bitmap: TD2HudRadioButton;
    checkPicture: TD2HudRadioButton;
    Panel1: TPanel;
    Image1: TImage;
    GUIImage1: TD3GUIImage;
    GUIText1: TD3GUIText;
    SaveDialog1: TSaveDialog;
    procedure HudButton1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmScreenshot: TfrmScreenshot;

implementation

{$R *.lfm}

procedure TfrmScreenshot.HudButton1Click(Sender: TObject);
var
  Bmp: TBitmap;
begin
  if checkBitmap.IsChecked then
  begin
    Bmp := TBitmap.Create;
    Bmp.Assign(d3Scene1.Canvas);  // <-- This is real screenshot creation - one line
    if SaveDialog1.Execute then
    begin
      Bmp.SaveToFile(SaveDialog1.FileName);
    end;
    Bmp.Free;
  end;
  if checkd3Bitmap.IsChecked then
  begin
    GUIImage1.Bitmap.Assign(d3Scene1.Canvas);  // <-- This is real screenshot creation - one line
  end;
  if checkPicture.IsChecked then
  begin
    Image1.Picture.Assign(d3Scene1.Canvas);  // <-- This is real screenshot creation - one line
  end;
end;

initialization
end.
