unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TD2MyCustomDrakonEditor }

  TD2MyDrakonEditor = class(TD2CustomDrakonEditor)
    public
      procedure Realign;  override;
  end;

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DockingPanel1: TD2DockingPanel;
    DrakonEditor1: TD2MyDrakonEditor;
    DrakonPallet1: TD2DrakonPallet;
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

procedure TD2MyDrakonEditor.Realign;
begin
  inherited;
end;

end.

