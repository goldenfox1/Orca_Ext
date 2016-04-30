unit aboutboxfrm;

{$MODE Delphi}

interface

uses

  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, orca_scene2d;

type

  TfrmAbout = class(TForm)
    d2Scene: TD2Scene;
    Layout1: TD2Layout;
    Root: TD2Rectangle;
    ShadowEffect1: TD2ShadowEffect;
    Rectangle2: TD2Rectangle;
    imageLayout: TD2Layout;
    Rectangle4: TD2Rectangle;
    Button1: TD2Button;
    ColorAnimation1: TD2ColorAnimation;
    ColorAnimation2: TD2ColorAnimation;
    Rectangle5: TD2Rectangle;
    Rectangle6: TD2Rectangle;
    TitleText: TD2Text;
    ShadowEffect2: TD2ShadowEffect;
    Button2: TD2Button;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAbout: TfrmAbout;

implementation

{$R *.lfm}


procedure TfrmAbout.Button2Click(Sender: TObject);
begin
  TitleText.Text := 'It''s cool !';
end;

procedure TfrmAbout.Button1Click(Sender: TObject);
begin
  Close;
end;

end.
