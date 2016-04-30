{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit direct2dfrm;

  {$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmD2D = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Rectangle1: TD2Rectangle;
    Rectangle2: TD2Rectangle;
    FloatAnimation1: TD2FloatAnimation;
    FloatAnimation2: TD2FloatAnimation;
    Ellipse1: TD2Ellipse;
    Text1: TD2Text;
    Image1: TD2Image;
    Path1: TD2Path;
    GlowEffect1: TD2GlowEffect;
    Text2: TD2Text;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmD2D: TfrmD2D;

implementation

{$R *.lfm}

initialization
  UseDirect2DCanvas;

end.
