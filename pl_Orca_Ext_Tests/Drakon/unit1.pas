unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DrakonNode1: TD2DrakonNode;
    DrakonPallet1: TD2DrakonPallet;
    Root1: TD2Background;
    ValueLabel1: TD2ValueLabel;
    procedure Root1Click(Sender: TObject);
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.Root1Click(Sender: TObject);
begin

end;

end.

