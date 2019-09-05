unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TD2MyCustomDrakonEditor }

  TD2MyCustomDrakonEditor = class(TD2CustomDrakonEditor)
  protected
    procedure CreateSilhouette; override;
  public
    function SetCommand(ACommand: TD2DrakonIcon): boolean; override;
  end;

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DrakonEditor1: TD2MyCustomDrakonEditor;
    DrakonNode1: TD2DrakonNode;
    DrakonPallet1: TD2DrakonPallet;
    Line1: TD2Line;
    Resources1: TD2Resources;
    Root1: TD2Background;

  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }


{ TD2MyCustomDrakonEditor }

procedure TD2MyCustomDrakonEditor.CreateSilhouette;
begin
  inherited CreateSilhouette;
end;

function TD2MyCustomDrakonEditor.SetCommand(ACommand: TD2DrakonIcon): boolean;
begin
  Result:=inherited SetCommand(ACommand);
end;

end.

