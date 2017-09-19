unit Unit1;

{$mode objfpc}{$H+}

//{$APPTYPE console}

interface

uses
  Classes, SysUtils, FileUtil, DBStringTree, RTTICtrls, SynEdit,
  SynHighlighterPas, SynCompletion, SynMacroRecorder, SynPluginSyncroEdit,
  VirtualTrees, VTHeaderPopup, orca_scene2d, Forms, Controls, Graphics, Dialogs;

type

  { TForm1 }

  TForm1 = class(TForm)
    D2Scene1: TD2Scene;
    DBStringTree1: TDBStringTree;
    Grid1: TD2Grid;
    Line1: TD2Line;
    Path1: TD2Path;
    Root1: TD2Background;
    SpinBox1: TD2SpinBox;
    StringColumn1: TD2StringColumn;
    StringColumn2: TD2StringColumn;
    StringGrid1: TD2StringGrid;
    TreeGrid1: TD2TreeGrid;
    TreeTextColumn1: TD2TreeTextColumn;
    TreeTextColumn2: TD2TreeTextColumn;
    VT: TVirtualStringTree;
    procedure FormCreate(Sender: TObject);
    procedure SpinBox1Change(Sender: TObject);
    procedure TreeGrid1GetValue(Sender: TObject; Node: PD2TreeNode;
      const Column: integer; var Value: Variant);
    procedure CreateTreeGreed;
    procedure CreateVT;
    procedure TreeGrid1Paint(Sender: TObject; const ACanvas: TD2Canvas;
      const ARect: TD2Rect);
    procedure TreeGrid1Scroll(Sender: TD2CustomScrollBox; DeltaX, DeltaY: single
      );
    procedure VTGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
      Column: TColumnIndex; TextType: TVSTTextType; var CellText: String);
    procedure VTScroll(Sender: TBaseVirtualTree; DeltaX, DeltaY: Integer);
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

procedure TForm1.FormCreate(Sender: TObject);
begin
  CreateTreeGreed;
  CreateVT;
end;

procedure TForm1.SpinBox1Change(Sender: TObject);
begin
  TreeGrid1.VScrollBar.Value:=SpinBox1.Value;
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
  NewNode,NewNode1,NewNode2: PD2TreeNode;
  NewPhone: PPhoneNode;
  i,t,k: integer;
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
        Phone := IntToStr(i+1) + IntToStr(i+1) + Phones[i];
      end;
    for t:=0 to High(Names) do
    begin
      NewNode1 := TreeGrid1.AddChild(NewNode);
      NewPhone := TreeGrid1.GetNodeData(NewNode1);
      if Assigned(NewPhone) then
        with NewPhone^ do
        begin
          Name := Names[t];
          Phone := IntToStr(t+1) + Phones[t];
        end;
      if t<=1 then
        for k:=0 to High(Names) do
        begin
          NewNode2 := TreeGrid1.AddChild(NewNode1);
          NewPhone := TreeGrid1.GetNodeData(NewNode2);
          if Assigned(NewPhone) then
            with NewPhone^ do
            begin
              Name := Names[k];
              Phone := Phones[k];
            end;
        end;

    end;
    if i=0 then TreeGrid1.ToggleNode(NewNode);
  end;
  TreeGrid1.EndUpdate;
end;

procedure TForm1.CreateVT;
var
  NewNode,NewNode1, NewNode2: PVirtualNode;
  NewPhone: PPhoneNode;
  i,t,k: integer;
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
        Phone := IntToStr(i+1) + IntToStr(i+1) + Phones[i];
      end;
    for t:=0 to High(Names) do
    begin
      NewNode1 := VT.AddChild(NewNode);
      NewNode1^.CheckType:=VirtualTrees.ctRadioButton;
      NewPhone := VT.GetNodeData(NewNode1);
      if Assigned(NewPhone) then
        with NewPhone^ do
        begin
          Name := Names[t];
          Phone := IntToStr(t+1) + Phones[t];
        end;
      if t<=1 then
        for k:=0 to High(Names) do
        begin
          NewNode2 := VT.AddChild(NewNode1);
          NewNode2^.CheckType:=VirtualTrees.ctTriStateCheckBox;
          NewPhone := VT.GetNodeData(NewNode2);
          if Assigned(NewPhone) then
            with NewPhone^ do
            begin
              Name := Names[k];
              Phone := Phones[k];
            end;
        end
    end;
    if i=0 then VT.ToggleNode(NewNode);
  end;
  VT.EndUpdate;
end;

procedure TForm1.TreeGrid1Paint(Sender: TObject; const ACanvas: TD2Canvas;
  const ARect: TD2Rect);
begin

end;

procedure TForm1.TreeGrid1Scroll(Sender: TD2CustomScrollBox; DeltaX,
  DeltaY: single);
begin
  SpinBox1.Value:=TreeGrid1.VScrollBar.Value;
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

procedure TForm1.VTScroll(Sender: TBaseVirtualTree; DeltaX, DeltaY: Integer);
begin

end;

end.

