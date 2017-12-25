unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, ZConnection, ZDataset, Forms, Controls, Graphics,
  Dialogs, StdCtrls, DBGrids, sqldb, Sqlite3DS, db, sqlite3conn;

type

  { TForm1 }

  TForm1 = class(TForm)
    DataSource: TDataSource;
    DBGrid: TDBGrid;
    EditID: TEdit;
    EditTel: TEdit;
    EditFIO: TEdit;
    ZConnection1: TZConnection;
    ZTable1: TZTable;
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

end.

