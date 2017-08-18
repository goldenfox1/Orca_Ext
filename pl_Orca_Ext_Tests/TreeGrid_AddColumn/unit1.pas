unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, VirtualTrees, orca_scene2d, Forms, Controls,
  Graphics, Dialogs, StdCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TD2Button;
    D2Scene1: TD2Scene;
    Root1: TD2Background;
    StringGrid1: TD2StringGrid;
    VirtualDrawTree1: TVirtualDrawTree;
    procedure Button1Click(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.Button1Click(Sender: TObject);
begin
   //StringGrid1.AddObject(TD2StringColumn.Create(nil));
end;

end.

