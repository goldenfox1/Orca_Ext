unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DrakonEditor1: TD2DrakonEditor;
    DrakonNode1: TD2DrakonNode;
    DrakonPallet1: TD2DrakonPallet;
    Root1: TD2Background;
  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}


end.

