unit Unit1;

{$mode objfpc}{$H+}

//{$APPTYPE console}

interface

uses
  Classes, SysUtils, db, FileUtil, DBStringTree, RTTICtrls, SynEdit,
  SynHighlighterPas, SynCompletion, SynMacroRecorder, SynPluginSyncroEdit,
  ZDataset, ZConnection, VirtualTrees, VTHeaderPopup, orca_scene2d, Forms,
  Controls, Graphics, Dialogs, DBGrids;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TD2Button;
    D2Scene1: TD2Scene;
    DataSource1: TDataSource;
    DBGrid1: TDBGrid;
    DBGrid2: TD2DBGrid;
    DBTextColumn1: TD2DBTextColumn;
    DBTextColumn2: TD2DBTextColumn;
    DBTreeGrid1: TD2DBTreeGrid;
    DBTreeTextColumn1: TD2DBTreeTextColumn;
    DBTreeTextColumn2: TD2DBTreeTextColumn;
    DBTreeTextColumn3: TD2DBTreeTextColumn;
    DBTreeTextColumn4: TD2DBTreeTextColumn;
    GradientAnimation1: TD2GradientAnimation;
    Root1: TD2Background;
    ZConnection1: TZConnection;
    ZTable1: TZTable;
    procedure Button1Click(Sender: TObject);

  private
    { private declarations }
  public
    { public declarations }
  end;

const
  Names: array[0..4] of String = (
  'Вася',
  'Петя',
  'Маша',
  'Костя',
  'Дима'
);
Phones: array[0..4] of String = (
  '1',
  '2',
  '3',
  '4',
  '5'
);

type
  PPhoneNode = ^TPhoneNode;
  TPhoneNode = record
    Name, // Имя контакта
    Phone: String; // Телефон
  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }


procedure TForm1.Button1Click(Sender: TObject);
begin
  ZTable1.Active:=not ZTable1.Active;
  if ZTable1.Active
    then Button1.Text:='Зыкрыть'
    else Button1.Text:='Открыть';
end;

end.

