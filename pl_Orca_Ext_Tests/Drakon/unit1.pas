unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DropTarget1: TD2DropTarget;
    Line1: TD2Line;
    Line2: TD2Line;
    Rectangle1: TD2Rectangle;
    Root1: TD2Background;
    RoundRect1: TD2RoundRect;
    ScrollArrowLeft1: TD2ScrollArrowLeft;
    SidesRectangle1: TD2SidesRectangle;
    ValueLabel1: TD2ValueLabel;
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

end.

