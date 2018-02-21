unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    Brace1: TD2Brace;
    D2Scene1: TD2Scene;
    DrakonEditor1: TD2DrakonEditor;
    DrakonPallet1: TD2DrakonPallet;
    Label1: TD2Label;
    Label2: TD2Label;
    Line1: TD2Line;
    Rectangle1: TD2Rectangle;
    Root1: TD2Background;
    Selection1: TD2Selection;
    procedure Selection1MouseMove(Sender: TObject; Shift: TShiftState; X, Y,
      Dx, Dy: single);
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.Selection1MouseMove(Sender: TObject; Shift: TShiftState; X, Y,
  Dx, Dy: single);
begin
  Label1.Text:='Dx='+floattostr(Dx);
  Label2.Text:='Dy='+floattostr(Dy);
end;

end.

