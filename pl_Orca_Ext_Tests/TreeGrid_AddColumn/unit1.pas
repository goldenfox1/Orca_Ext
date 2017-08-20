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
    TreeGrid1: TD2TreeGrid;
    TreeTextColumn1: TD2TreeTextColumn;
    VirtualDrawTree1: TVirtualDrawTree;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

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

type
  PPhoneNode = ^TPhoneNode;
  TPhoneNode = record
    Name, // Имя контакта
    Phone: WideString; // Телефон
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
  TreeGrid1.NodeDataSize:= SizeOf(TPhoneNode);
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
  end;
end;

end.

