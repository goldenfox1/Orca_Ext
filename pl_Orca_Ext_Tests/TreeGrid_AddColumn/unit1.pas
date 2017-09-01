unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, DBStringTree, orca_scene2d, Forms, Controls,
  Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TD2Button;
    D2Scene1: TD2Scene;
    DBVT: TDBStringTree;
    Root1: TD2Background;
    StringGrid1: TD2StringGrid;
    TreeGrid1: TD2TreeGrid;
    TreeTextColumn1: TD2TreeTextColumn;
    TreeTextColumn2: TD2TreeTextColumn;
    TreeView1: TD2TreeView;
    TreeViewItem1: TD2TreeViewItem;
    TreeViewItem2: TD2TreeViewItem;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure TreeGrid1GetValue(Sender: TObject; Node: PD2TreeNode;
      const Column: integer; var Value: Variant);
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
  '433-56-49',
  '545-67-79',
  '777-50-50',
  '911-03-05',
  '02'
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
  TreeGrid1.AddObject(TD2TreeTextColumn.Create(nil));
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  NewNode,NewNode1: PD2TreeNode;
  NewPhone: PPhoneNode;
  i,t: integer;
begin
  TreeGrid1.Columns[0].Header:='Имя';
  TreeGrid1.Columns[0].Tag:= 0;
  TreeGrid1.Columns[1].Header:='Телефон';
  TreeGrid1.Columns[0].Tag:= 1;
  TreeGrid1.NodeDataSize:= SizeOf(TPhoneNode);
  TreeGrid1.BeginUpdate;
  for i:=0 to High(Names) do
  begin
    NewNode := TreeGrid1.AddChild(nil);
    NewPhone := TreeGrid1.GetNodeData(NewNode);
    if Assigned(NewPhone) then
      with NewPhone^ do
      begin
        Name := Names[i];
        Phone := Phones[i];
      end;
    for t:=0 to High(Names) do
    begin
      NewNode1 := TreeGrid1.AddChild(NewNode);
      NewPhone := TreeGrid1.GetNodeData(NewNode1);
      if Assigned(NewPhone) then
        with NewPhone^ do
        begin
          Name := Names[t];
          Phone := Phones[t];
        end;
    end;
    if i=0 then NewNode^.States:= NewNode^.States + [vsExpanded];
  end;
  TreeGrid1.EndUpdate;
end;

procedure TForm1.TreeGrid1GetValue(Sender: TObject; Node: PD2TreeNode;
  const Column: integer; var Value: Variant);
var
  Phone: PPhoneNode;
begin
  with TD2TreeGrid(Sender) do
  begin
    Phone:= GetNodeData(Node);
    case Columns[Column].Tag of
      0: Value := Phone^.Name; // Текст для колонки имени
      1: Value := Phone^.Phone; // Текст для колонки телефонного номера
    end;
  end;
end;

end.

