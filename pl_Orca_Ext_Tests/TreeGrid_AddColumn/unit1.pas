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
    CheckColumn1: TD2CheckColumn;
    D2Scene1: TD2Scene;
    GradientAnimation1: TD2GradientAnimation;
    Grid1: TD2Grid;
    Label1: TD2Label;
    Label2: TD2Label;
    Label3: TD2Label;
    Root1: TD2Background;
    SpinBox1: TD2SpinBox;
    TextColumn1: TD2TextColumn;
    TextColumn2: TD2TextColumn;
    TreeGrid1: TD2TreeGrid;
    TreeGrid2: TD2TreeGrid;
    TreeTextColumn1: TD2TreeTextColumn;
    TreeTextColumn2: TD2TreeTextColumn;
    VT: TVirtualStringTree;
    procedure FormCreate(Sender: TObject);
    procedure Grid1DragOver(Sender: TObject; const Data: TD2DragObject;
      const Point: TD2Point; var Accept: Boolean);
    procedure SpinBox1Change(Sender: TObject);
    procedure TreeGrid1DragOver(Sender: TObject; const Data: TD2DragObject;
      Shift: TShiftState; const Point: TD2Point; var TargetNode: PD2TreeNode;
      var Mode: TD2DropMode; var Accept: Boolean);
    procedure TreeGrid1GetValue(Sender: TObject; Node: PD2TreeNode;
      const Column: integer; var Value: Variant);
    procedure CreateTreeGreed;
    procedure CreateVT;
    procedure TreeGrid1HeaderClick(Sender: TObject);
    procedure TreeGrid1SetValue(Sender: TObject; Node: PD2TreeNode;
      const Column: integer; const Value: Variant);
    procedure VTDragOver(Sender: TBaseVirtualTree; Source: TObject;
      Shift: TShiftState; State: TDragState; const Pt: TPoint; Mode: TDropMode;
      var Effect: LongWord; var Accept: Boolean);
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

procedure TForm1.Grid1DragOver(Sender: TObject; const Data: TD2DragObject;
  const Point: TD2Point; var Accept: Boolean);
begin
  Accept:=true;
end;

procedure TForm1.SpinBox1Change(Sender: TObject);
begin
  TreeGrid1.SortColumn:=integer(SpinBox1.Value);
end;

procedure TForm1.TreeGrid1DragOver(Sender: TObject; const Data: TD2DragObject;
  Shift: TShiftState; const Point: TD2Point; var TargetNode: PD2TreeNode;
  var Mode: TD2DropMode; var Accept: Boolean);
begin
  case Mode of
    dmNowhere: Label1.Text:='dmNowhere';
      dmAbove: Label1.Text:='dmAbove';
      dmBelow: Label1.Text:='dmBelow';
     dmOnNode: Label1.Text:='dmOnNode';
  end;

  if TargetNode<>nil then
    with PPhoneNode(TD2TreeGrid(Sender).GetNodeData(TargetNode))^ do
      label2.Text:= Phone + ' ' + Name
    else label2.Text:= '-- нет --';

  Label3.Text:='X='+inttostr(round(Point.X))+' Y='+inttostr(round(Point.Y));
  Accept:=true;
end;

procedure TForm1.VTDragOver(Sender: TBaseVirtualTree; Source: TObject;
  Shift: TShiftState; State: TDragState; const Pt: TPoint; Mode: TDropMode;
  var Effect: LongWord; var Accept: Boolean);
begin
  Accept:=true;
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

procedure TForm1.TreeGrid1SetValue(Sender: TObject; Node: PD2TreeNode;
  const Column: integer; const Value: Variant);
var
  Phone: PPhoneNode;
begin
  with TD2TreeGrid(Sender) do
  begin
    Phone:= GetNodeData(Node);
    case Columns[Column].Tag of
      0: Phone^.Name := Value; // Текст для колонки имени
      1: Phone^.Phone := Value; // Текст для колонки телефонного номера
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
      TreeGrid1.CheckType[NewNode1]:=ctRadioButton;
      //TreeGrid1.CheckType[NewNode1]:=ctTriStateCheckBox;
      if t<=1 then TreeGrid1.CheckState[NewNode1]:=csCheckedNormal;
      NewPhone := TreeGrid1.GetNodeData(NewNode1);
      if Assigned(NewPhone) then
        with NewPhone^ do
        begin
          Name := Names[t];
          Phone := IntToStr(t+1) + Phones[t];
        end;
      if t<=1 then
      begin
        for k:=0 to High(Names) do
        begin
          NewNode2 := TreeGrid1.AddChild(NewNode1);
          if k=0 then
            begin
              TreeGrid1.CheckType[NewNode2]:=ctTriStateCheckBox;
              TreeGrid1.CheckState[NewNode2]:=csMixedNormal;
            end;
          if k=1 then
            begin
              TreeGrid1.CheckType[NewNode2]:=ctCheckBox;
              TreeGrid1.CheckState[NewNode2]:=csCheckedNormal;
            end;
          if k>1 then
            begin
              TreeGrid1.CheckType[NewNode2]:=ctCheckBox;
              //TreeGrid1.CheckState[NewNode2]:=csCheckedNormal;
            end;
          NewPhone := TreeGrid1.GetNodeData(NewNode2);
          if Assigned(NewPhone) then
            with NewPhone^ do
            begin
              Name := Names[k];
              Phone := Phones[k];
            end;
        end;
      end;

    end;
    if i=0 then TreeGrid1.ToggleNode(NewNode);
  end;
  //TreeGrid1.FullExpand;
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

procedure TForm1.TreeGrid1HeaderClick(Sender: TObject);
begin
  if TreeGrid1.SortColumn=TD2HeaderItem(Sender).Index
    then  case TreeGrid1.SortDirection of
            sdAscending: TreeGrid1.SortDirection:=sdDescending;
           sdDescending: TreeGrid1.SortDirection:=sdAscending;
          end
    else  begin
            TreeGrid1.SortColumn:=TD2HeaderItem(Sender).Index;
            TreeGrid1.SortDirection:=sdAscending;
          end;
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

