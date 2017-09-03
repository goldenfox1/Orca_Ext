unit Unit1;

{$mode objfpc}{$H+}

//{$APPTYPE console}

interface

uses
  Classes, SysUtils, FileUtil, DBStringTree, VirtualTrees, orca_scene2d, Forms,
  Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TD2Button;
    D2Scene1: TD2Scene;
    Grid1: TD2Grid;
    Label1: TD2Label;
    Label2: TD2Label;
    Label3: TD2Label;
    Label4: TD2Label;
    Root1: TD2Background;
    StringColumn1: TD2StringColumn;
    StringColumn2: TD2StringColumn;
    StringGrid1: TD2StringGrid;
    TextBox1: TD2TextBox;
    TextBox3: TD2TextBox;
    TextBox4: TD2TextBox;
    TextBox5: TD2TextBox;
    TreeGrid1: TD2TreeGrid;
    TreeTextColumn1: TD2TreeTextColumn;
    TreeTextColumn2: TD2TreeTextColumn;
    VT: TVirtualStringTree;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure TreeGrid1GetValue(Sender: TObject; Node: PD2TreeNode;
      const Column: integer; var Value: Variant);
    procedure CreateTreeGreed;
    procedure CreateVT;
    procedure TreeGrid1Paint(Sender: TObject; const ACanvas: TD2Canvas;
      const ARect: TD2Rect);
    procedure VTGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
      Column: TColumnIndex; TextType: TVSTTextType; var CellText: String);
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
  '111',
  '222',
  '333',
  '444',
  '555'
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
begin
  CreateTreeGreed;
  CreateVT;
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

procedure TForm1.CreateTreeGreed;
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
        Phone := IntToStr(i+1) + Phones[i];
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
    if i=0 then TreeGrid1.ToggleNode(NewNode);
  end;
  TreeGrid1.EndUpdate;
end;

procedure TForm1.CreateVT;
var
  NewNode,NewNode1: PVirtualNode;
  NewPhone: PPhoneNode;
  i,t: integer;
begin
  //VT.Header.Columns.Items[0].CaptionText:='Имя';
  //VT.Header.Columns.Items[0].Tag:= 0;
  //VT.Header.Columns.Items[0].CaptionText:='Телефон';
  //VT.Header.Columns.Items[0].Tag:= 1;
  VT.NodeDataSize:= SizeOf(TPhoneNode);
  VT.BeginUpdate;
  for i:=0 to High(Names) do
  begin
    NewNode := VT.AddChild(nil);
    NewPhone := VT.GetNodeData(NewNode);
    if Assigned(NewPhone) then
      with NewPhone^ do
      begin
        Name := Names[i];
        Phone := Phones[i];
      end;
    for t:=0 to High(Names) do
    begin
      NewNode1 := VT.AddChild(NewNode);
      NewPhone := VT.GetNodeData(NewNode1);
      if Assigned(NewPhone) then
        with NewPhone^ do
        begin
          Name := Names[t];
          Phone := Phones[t];
        end;
    end;
    if i=0 then VT.ToggleNode(NewNode);
  end;
  VT.EndUpdate;
end;

procedure TForm1.TreeGrid1Paint(Sender: TObject; const ACanvas: TD2Canvas;
  const ARect: TD2Rect);
begin
   TextBox3.Text:=FloatToStr(TreeGrid1.VScrollBar.Value);
   TextBox5.Text:=FloatToStr(TreeGrid1.VScrollBar.Min);
   TextBox1.Text:=FloatToStr(TreeGrid1.VScrollBar.Max);
   TextBox4.Text:=FloatToStr(TreeGrid1.VScrollBar.ViewportSize);
end;

procedure TForm1.VTGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
  Column: TColumnIndex; TextType: TVSTTextType; var CellText: String);
var
  Phone: PPhoneNode;
begin
  with TVirtualStringTree(Sender) do
  begin
    Phone:= GetNodeData(Node);
    case Column of
      0: CellText := Phone^.Name; // Текст для колонки имени
      1: CellText := Phone^.Phone; // Текст для колонки телефонного номера
    end;
  end;
end;

end.

