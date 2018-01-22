unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    AniIndicator1: TD2AniIndicator;
    CalloutPanel1: TD2CalloutPanel;
    CalloutRectangle1: TD2CalloutRectangle;
    D2Scene1: TD2Scene;
    Ellipse1: TD2Ellipse;
    FramedScrollBox1: TD2FramedScrollBox;
    Root1: TD2Background;
    RoundRect1: TD2RoundRect;
    Text1: TD2Text;
    ValueLabel1: TD2ValueLabel;
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

end.

