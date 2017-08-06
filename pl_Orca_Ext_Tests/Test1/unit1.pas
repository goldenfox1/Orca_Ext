unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, DBStringTree, orca_scene2d, Forms, Controls,
  Graphics, Dialogs, StdCtrls;

type
  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DBGrid1: TD2DBGrid;
    DBStringTree1: TDBStringTree;
    DBTextColumn1: TD2DBTextColumn;
    Grid1: TD2Grid;
    Root1: TD2Background;
    TextColumn1: TD2TextColumn;
    TextColumn2: TD2TextColumn;
    VT: TD2TreeGrid;
    TreeTextColumn1: TD2TreeTextColumn;
    TreeView1: TD2TreeView;
    TreeViewItem1: TD2TreeViewItem;
    TreeViewItem2: TD2TreeViewItem;
    TreeViewItem3: TD2TreeViewItem;
    TreeViewItem4: TD2TreeViewItem;
    TreeViewItem5: TD2TreeViewItem;
    TreeViewItem6: TD2TreeViewItem;
    procedure FormCreate(Sender: TObject);

  private
    { private declarations }
  public
    { public declarations }
  end;


  PPhoneNode = ^TPhoneNode;
  TPhoneNode = record
    Name,              // Имя контакта
    Phone: WideString; // Телефон
  end;

var
  Form1: TForm1;

const
  Names: array[0..4] of WideString = (
  'Вася',
  'Петя',
  'Маша',
  'Костя',
  'Дима'
);
Phones: array[0..4] of WideString = (
  '433-56-49',
  '545-67-79',
  '777-50-50',
  '911-03-05',
  '02'
);

implementation

{$R *.lfm}

{ TForm1 }

       procedure TForm1.FormCreate(Sender: TObject);
var
  i: Integer;
  NewNode, Node: PD2TreeNode;
  NewPhone: PPhoneNode;
begin
  VT.NodeDataSize := SizeOf(TPhoneNode);
  VT.BeginUpdate;
  for i := 0 to Length(Names) - 1 do
  begin
    NewNode := VT.AddChild(VT.RootNode);
    NewPhone := VT.GetNodeData(NewNode);
    if Assigned(NewPhone) then
      with NewPhone^ do
      begin
        Name := Names[i];
        Phone := Phones[i];
      end;
  end;
  Node:=VT.GetFirstChild(VT.RootNode);
  while Node<>nil do
  begin
    for i := 0 to Length(Names) - 1 do
    begin
      NewNode := VT.AddChild(Node);
      NewPhone := VT.GetNodeData(NewNode);
      if Assigned(NewPhone) then
        with NewPhone^ do
        begin
          Name := Names[i];
          Phone := Phones[i];
        end;
    end;
    Node:=VT.GetNextSibling(Node)
  end;
  VT.EndUpdate;
end;

end.

