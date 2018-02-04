unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DrakonAction1: TD2DrakonAction;
    DrakonNode1: TD2DrakonNode;
    Rectangle1: TD2Rectangle;
    Root1: TD2Background;
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

