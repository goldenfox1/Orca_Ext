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
    DrakonAction1: TD2DrakonAction;
    DrakonNode1: TD2DrakonNode;
    Ellipse1: TD2Ellipse;
    FramedScrollBox1: TD2FramedScrollBox;
    Memo1: TD2Memo;
    Rectangle1: TD2Rectangle;
    Root1: TD2Background;
    RoundRect1: TD2RoundRect;
    RoundRect2: TD2RoundRect;
    Text1: TD2Text;
    TextBox1: TD2TextBox;
    ValueLabel1: TD2ValueLabel;
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

end.

