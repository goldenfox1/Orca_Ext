{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit aniform;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, orca_scene2d;

type
  TForm4 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Ellipse1: TD2Ellipse;
    FloatAnimation1: TD2FloatAnimation;
    Rectangle1: TD2Rectangle;
    Text1: TD2Text;
    ColorAnimation1: TD2ColorAnimation;
    Text2: TD2Text;
    Rectangle2: TD2Rectangle;
    Text3: TD2Text;
    FloatAnimation2: TD2FloatAnimation;
    Image1: TD2Image;
    BitmapAnimation1: TD2BitmapAnimation;
    Text4: TD2Text;
    Rectangle3: TD2Rectangle;
    FloatAnimation4: TD2FloatAnimation;
    Text5: TD2Text;
    AniIndicator1: TD2AniIndicator;
    Label1: TD2Label;
    AniIndicator2: TD2AniIndicator;
    AniIndicator3: TD2AniIndicator;
    RoundRect1: TD2RoundRect;
    PathAnimation1: TD2PathAnimation;
    Text6: TD2Text;
    Path1: TD2Path;
    Text7: TD2Text;
    PathAnimation2: TD2PathAnimation;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form4: TForm4;

implementation

{$R *.lfm}

end.
