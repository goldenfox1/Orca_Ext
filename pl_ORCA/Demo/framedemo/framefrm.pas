{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


unit framefrm;

  {$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, orca_scene2d;

type
  TfrmMain = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Label1: TD2Label;
    Panel1: TD2Panel;
    Frame1: TD2Frame;
    Panel2: TD2Panel;
    Frame2: TD2Frame;
    Splitter1: TD2Splitter;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses secondfrm;

{$R *.lfm}

end.
