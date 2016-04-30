{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit messagepopupfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type

  { TfrmMessagePopup }

  TfrmMessagePopup = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    ToolBar1: TD2ToolBar;
    Label1: TD2Label;
    msgType: TD2StringListBox;
    Label2: TD2Label;
    ResultList: TD2StringListBox;
    CheckBox1: TD2CheckBox;
    CheckBox2: TD2CheckBox;
    CheckBox3: TD2CheckBox;
    CheckBox4: TD2CheckBox;
    CheckBox5: TD2CheckBox;
    Label3: TD2Label;
    StyleBox: TD2PopupBox;
    ModernStyle: TD2Resources;
    VistaStyle: TD2Resources;
    AirStyle: TD2Resources;
    ToolButton1: TD2ToolButton;
    ToolButton2: TD2ToolButton;
    MessagePopup1: TD2MessagePopup;
    Label4: TD2Label;
    Rectangle1: TD2Rectangle;
    Button1: TD2Button;
    Panel1: TD2Panel;
    Label5: TD2Label;
    procedure FormShow(Sender: TObject);
    procedure ToolButton1Click(Sender: TObject);
    procedure StyleBoxChange(Sender: TObject);
    procedure ToolButton2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMessagePopup: TfrmMessagePopup;

implementation

{$R *.lfm}

procedure TfrmMessagePopup.ToolButton1Click(Sender: TObject);
var
  Res: TModalResult;
  Btns: TD2MessageButtons;
  Typ: TD2MessageType;
begin
  Btns := [];
  if CheckBox1.IsChecked then Btns := Btns + [d2ButtonYes];
  if CheckBox2.IsChecked then Btns := Btns + [d2ButtonNo];
  if CheckBox3.IsChecked then Btns := Btns + [d2ButtonOk];
  if CheckBox4.IsChecked then Btns := Btns + [d2ButtonCancel];
  if CheckBox5.IsChecked then Btns := Btns + [d2ButtonAbort];
  Typ := TD2MessageType(msgType.ItemIndex);

  Res := MessagePopup('This is a message popup caption',
    'Graphical editor integrated in IDE, graphical objects, simplify animation, advanced windows and controls, maximum performance, skinning engine, bitmap effects.',
    Typ,
    Btns,
    d2Scene1,        
    Toolbar1,
    true);

  ResultList.ItemIndex := Integer(Res);
end;

procedure TfrmMessagePopup.FormShow(Sender: TObject);
begin
    ModernStyle.FileName:=SetDirSeparators(ModernStyle.FileName);
    VistaStyle.FileName:=SetDirSeparators(VistaStyle.FileName);
    AirStyle.FileName:=SetDirSeparators(AirStyle.FileName);
end;

procedure TfrmMessagePopup.StyleBoxChange(Sender: TObject);
begin
  case StyleBox.ItemIndex of
    0: d2Scene1.Style := nil;
    1: d2Scene1.Style := ModernStyle;
    2: d2Scene1.Style := VistaStyle;
    3: d2Scene1.Style := AirStyle;
  end;
end;

procedure TfrmMessagePopup.ToolButton2Click(Sender: TObject);
begin
  MessagePopup1.PopupModal;
end;

end.
