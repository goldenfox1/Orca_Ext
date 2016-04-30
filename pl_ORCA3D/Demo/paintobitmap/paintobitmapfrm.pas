{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit paintobitmapfrm;

{$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d;

type
  TForm5 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Light1: TD3Light;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    Image1: TD2Image;
    Render: TD2Button;
    Text3D1: TD3Text3D;
    trackSize: TD2TrackBar;
    Label1: TD2Label;
    ScrollBox1: TD2ScrollBox;
    Image2: TD2Image;
    FloatAnimation1: TD3FloatAnimation;
    ValueLabel1: TD2ValueLabel;
    procedure FormCreate(Sender: TObject);
    procedure RenderClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form5: TForm5;

implementation

{$R *.lfm}

procedure TForm5.FormCreate(Sender: TObject);
begin
  trackSize.Max := cnmaxBitmapSize;
end;

procedure TForm5.RenderClick(Sender: TObject);
var
  B: TD3Bitmap;
begin
  // Render to Texture
  B := TD3Bitmap.Create(1, 1);
  Text3D1.PaintToBitmap(B, trunc(trackSize.Value), trunc(trackSize.Value), 0);
  if B <> nil then
  begin
    // copy to Image
    Image2.Bitmap.Assign(B);
    Image2.Width := Image2.Bitmap.Width;
    Image2.Height := Image2.Bitmap.Height;
    // center ScrollBox
    ScrollBox1.Centre;
    // ! Free bitmap
    B.Free;
  end;
end;

end.
