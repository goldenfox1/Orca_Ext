{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit aerofrm;

{$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, orca_scene2d;

type
  TfrmAeroGlass = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Label1: TLabel;
    Image1: TD2Image;
    Rectangle1: TD2Rectangle;
    SpeedButton1: TD2SpeedButton;
    d2Resources1: TD2Resources;
    SpeedButton2: TD2SpeedButton;
    ShadowEffect1: TD2ShadowEffect;
    ShadowEffect2: TD2ShadowEffect;
    Text1: TD2Text;
    ImageListBox1: TD2HorzImageListBox;
    Text2: TD2Text;
    Text3: TD2Text;
    OpenDialog1: TOpenDialog;
    procedure SpeedButton2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAeroGlass: TfrmAeroGlass;

implementation

{$R *.lfm}

procedure TfrmAeroGlass.SpeedButton2Click(Sender: TObject);
begin
  { add folder }
  OpenDialog1.Filter := 'Image files|' + GVarD2DefaultFilterClass.GetFileTypes;
  if OpenDialog1.Execute then
  begin
    ImageListBox1.AddFolder(ExtractFilePath(OpenDialog1.FileName));
  end;
end;

end.
