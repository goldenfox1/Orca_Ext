{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit stylesfrm;

  {$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmStyles = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Label1: TD2Label;
    Button1: TD2Button;
    Resources1: TD2Resources;
    SpeedButton1: TD2SpeedButton;
    TextBox1: TD2TextBox;
    ListBox1: TD2ListBox;
    ListBoxItem1: TD2ListBoxItem;
    ListBoxItem2: TD2ListBoxItem;
    ListBoxItem3: TD2ListBoxItem;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmStyles: TfrmStyles;

implementation

{$R *.lfm}

end.
