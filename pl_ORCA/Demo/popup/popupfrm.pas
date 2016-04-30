{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit popupfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmPopup = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Popup1: TD2Popup;
    Path1: TD2CalloutPanel;
    Text1: TD2Text;
    Text2: TD2Text;
    RoundRect1: TD2RoundRect;
    RoundRect2: TD2RoundRect;
    HudStringListBox1: TD2HudStringListBox;
    ShadowEffect1: TD2ShadowEffect;
    ShadowEffect2: TD2ShadowEffect;
    Text4: TD2Text;
    Text3: TD2Text;
    Text5: TD2Text;
    Popup3: TD2Popup;
    CalloutRect1: TD2CalloutPanel;
    ShadowEffect4: TD2ShadowEffect;
    Text6: TD2Text;
    ContextPopup: TD2Popup;
    CalloutRect2: TD2CalloutPanel;
    ShadowEffect6: TD2ShadowEffect;
    procedure RoundRect1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure RoundRect2MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPopup: TfrmPopup;

implementation

{$R *.lfm}

procedure TfrmPopup.RoundRect1MouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Single);
begin
  Popup1.IsOpen := not Popup1.IsOpen;
end;

procedure TfrmPopup.RoundRect2MouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Single);
begin
  Popup3.IsOpen := true;
end;

end.
