{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit bindingfrm;

{$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TForm6 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    TextBox1: TD2TextBox;
    Label1: TD2Label;
    TrackBar1: TD2TrackBar;
    TextBox2: TD2TextBox;
    Label2: TD2Label;
    NumberBox1: TD2NumberBox;
    Label3: TD2Label;
    t1: TD2TrackBar;
    t2: TD2TrackBar;
    ToolBar1: TD2ToolBar;
    Grid1: TD2GridLayout;
    ValueLabel1: TD2ValueLabel;
    Panel1: TD2Panel;
    Panel2: TD2Panel;
    Panel3: TD2Panel;
    Panel4: TD2Panel;
    Label4: TD2Label;
    ValueLabel2: TD2ValueLabel;
    TextBox3: TD2TextBox;
    TextBox4: TD2TextBox;
    Panel5: TD2Panel;
    StringListBox1: TD2StringListBox;
    TextBox5: TD2TextBox;
    Panel6: TD2Panel;
    StringComboBox1: TD2StringComboBox;
    Label5: TD2Label;
    Panel7: TD2Panel;
    Panel8: TD2Panel;
    TrackBar2: TD2TrackBar;
    ProgressBar1: TD2ProgressBar;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form6: TForm6;

implementation

{$R *.lfm}

end.
