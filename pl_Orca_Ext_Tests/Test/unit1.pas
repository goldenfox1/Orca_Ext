unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, orca_scene2d;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TD2Button;
    D2Scene1: TD2Scene;
    DrakonEditor1: TD2DrakonEditor;
    DrakonNode1: TD2DrakonNode;
    Root1: TD2Background;
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

end.

