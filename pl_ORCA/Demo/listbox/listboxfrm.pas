{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit listboxfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources,
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmListBoxDemo = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    TabControl1: TD2TabControl;
    TabItem1: TD2TabItem;
    Layout1: TD2Layout;
    StringListBox1: TD2StringListBox;
    TrackBar1: TD2TrackBar;
    TabItem2: TD2TabItem;
    Layout2: TD2Layout;
    HorzListBox1: TD2HorzListBox;
    Layout3: TD2Layout;
    Splitter1: TD2Splitter;
    Label1: TD2Label;
    Label2: TD2Label;
    TrackBar2: TD2TrackBar;
    TrackBar3: TD2TrackBar;
    Label3: TD2Label;
    Label4: TD2Label;
    Layout4: TD2Layout;
    ListBox1: TD2ListBox;
    HorzListBox2: TD2HorzListBox;
    Splitter2: TD2Splitter;
    TabItem3: TD2TabItem;
    Layout5: TD2Layout;
    ImageListBox1: TD2ImageListBox;
    Button1: TD2Button;
    OpenPictureDialog1: TOpenDialog;
    TrackBar4: TD2TrackBar;
    CheckBox1: TD2CheckBox;
    TabItem4: TD2TabItem;
    Layout6: TD2Layout;
    StringListBox2: TD2StringListBox;
    procedure FormCreate(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
    procedure TrackBar2Change(Sender: TObject);
    procedure TrackBar3Change(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure TrackBar4Change(Sender: TObject);
    procedure CheckBox1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmListBoxDemo: TfrmListBoxDemo;

implementation

{$R *.lfm}

procedure TfrmListBoxDemo.FormCreate(Sender: TObject);
var
  i: integer;
  Item: TD2ListboxItem;
begin
  StringListBox1.BeginUpdate;
  for i := 0 to 200 do
    StringListBox1.Items.add('Item ' + IntToStr(i));
  StringListBox1.EndUpdate;

  HorzListBox1.BeginUpdate;
  for i := 0 to 200 do
  begin
    Item := TD2ListboxItem.Create(Self);
    with Item do
    begin
      Parent := HorzListBox1;
      Width := 48;
    end;
    with TD2Rectangle.Create(Self) do
    begin
      Parent := Item;
      Padding.Rect := d2Rect(5,5,5,5);
      xRadius := 4;
      yRadius := 4;
      Opacity := 0.5;
      HitTest := false;
      Align := vaClient;
    end;
  end;
  HorzListBox1.EndUpdate;

  HorzListBox2.BeginUpdate;
  for i := 0 to 60 do
  begin
    Item := TD2ListboxItem.Create(Self);
    with Item do
    begin
      Parent := HorzListBox2;
      Width := 48;
    end;
    with TD2Rectangle.Create(Self) do
    begin
      Parent := Item;
      Padding.Rect := d2Rect(5,5,5,5);
      xRadius := 4;
      yRadius := 4;
      Opacity := 0.5;
      HitTest := false;
      Align := vaClient;
    end;
  end;
  HorzListBox2.EndUpdate;

  ListBox1.BeginUpdate;
  for i := 0 to 60 do
  begin
    Item := TD2ListboxItem.Create(Self);
    with Item do
    begin
      Parent := ListBox1;
    end;
    with TD2Rectangle.Create(Self) do
    begin
      Parent := Item;
      Padding.Rect := d2Rect(5,5,5,5);
      xRadius := 4;
      yRadius := 4;
      Opacity := 0.5;
      HitTest := false;
      Align := vaClient;
    end;
  end;
  ListBox1.EndUpdate;
end;

procedure TfrmListBoxDemo.TrackBar1Change(Sender: TObject);
begin
  StringListBox1.Columns := Round(TrackBar1.Value);
  HorzListBox1.Columns := Round(TrackBar1.Value);
end;

procedure TfrmListBoxDemo.TrackBar2Change(Sender: TObject);
begin
  ListBox1.ItemWidth := Round(TrackBar2.Value);
  HorzListBox2.ItemWidth := Round(TrackBar2.Value);
end;

procedure TfrmListBoxDemo.TrackBar3Change(Sender: TObject);
begin
  ListBox1.ItemHeight := Round(TrackBar3.Value);
  HorzListBox2.ItemHeight := Round(TrackBar3.Value);
end;

procedure TfrmListBoxDemo.Button1Click(Sender: TObject);
var
  Dir: string;
begin
  { add folder }
  OpenPictureDialog1.Filter := 'Image files|' + GVarD2DefaultFilterClass.GetFileTypes;
  if OpenPictureDialog1.Execute then
  begin
    Dir := ExtractFilePath(OpenPictureDialog1.FileName);
    ImageListBox1.AddFolder(Dir);
  end;
end;

procedure TfrmListBoxDemo.TrackBar4Change(Sender: TObject);
begin
  ImageListBox1.BeginUpdate;
  ImageListBox1.ItemHeight := Round(TrackBar4.Value);
  ImageListBox1.ItemWidth := Round(TrackBar4.Value);
  ImageListBox1.EndUpdate;
end;

procedure TfrmListBoxDemo.CheckBox1Change(Sender: TObject);
begin
  StringListBox1.MouseTracking := CheckBox1.IsChecked;
  HorzListBox1.MouseTracking := CheckBox1.IsChecked;
end;

end.
