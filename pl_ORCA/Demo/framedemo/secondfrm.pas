{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit secondfrm;

  {$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmChild = class(TForm)
    frameScene1: TD2Scene;
    Root1: TD2Background;
    Rectangle1: TD2Rectangle;
    Label1: TD2Label;
    HudTrackBar1: TD2HudTrackBar;
    Label2: TD2Label;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmChild: TfrmChild;

implementation

{$R *.lfm}

end.
