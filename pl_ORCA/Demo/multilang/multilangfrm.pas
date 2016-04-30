{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit multilangfrm;

  {$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, StdCtrls;

type
  TfrmMultilang = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    d2Lang1: TD2Lang;
    Button1: TD2Button;
    ToolBar1: TD2ToolBar;
    HudButton1: TD2HudButton;
    HudLabel1: TD2HudLabel;
    HudRoundButton1: TD2HudRoundButton;
    Label1: TD2Label;
    HudCheckBox1: TD2HudCheckBox;
    StringListBox1: TD2StringListBox;
    Label2: TD2Label;
    Label3: TD2Label;
    Label4: TD2Label;
    StringListBox2: TD2StringListBox;
    StatusBar1: TD2StatusBar;
    Label5: TD2Label;
    ApplicationEvents1: TApplicationProperties;
    procedure Button1Click(Sender: TObject);
    procedure HudRoundButton1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure StringListBox1Change(Sender: TObject);
    procedure ApplicationEvents1Hint(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMultilang: TfrmMultilang;

implementation

{$R *.lfm}

procedure TfrmMultilang.Button1Click(Sender: TObject);
begin
  MessagePopup(Translate('Caption'),
    Translate('This is a message text.'),
    d2MessageInformation,
    [d2ButtonOK, d2ButtonCancel], d2Scene1, Toolbar1);
end;

procedure TfrmMultilang.HudRoundButton1Click(Sender: TObject);
begin
  ShowDsgnLang(d2Lang1);
end;

procedure TfrmMultilang.FormCreate(Sender: TObject);
begin
  StringListBox1.Items.Assign(d2Lang1.Resources);
  StringListBox1.Items.Add('en');
  StringListBox1.ItemIndex := 0;//StringListBox1.Items.IndexOf(d2Lang1.Lang);
end;

procedure TfrmMultilang.StringListBox1Change(Sender: TObject);
begin
  if StringListBox1.Selected <> nil then
    d2Lang1.Lang := StringListBox1.Selected.Text;
end;

procedure TfrmMultilang.ApplicationEvents1Hint(Sender: TObject);
begin
  Label5.Text := Application.Hint;
end;

end.
