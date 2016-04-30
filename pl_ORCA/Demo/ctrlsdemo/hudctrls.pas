unit hudctrls;

{$MODE Delphi}

interface

uses
  {$IFDEF FPC}
  LResources,
  {$ENDIF}                                                                                          
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs,orca_scene2d;

type                                           
  TfrmLayerDemo = class(TForm)
    vgScene1: TD2Scene;
    Root1: TD2Layout;
    HudWindow1: TD2HudWindow;
    HudButton1: TD2HudButton;
    HudNumberBox1: TD2HudNumberBox;
    HudTextBox1: TD2HudTextBox;
    HudScrollBar1: TD2HudScrollBar;
    HudScrollBar2: TD2HudScrollBar;
    HudTrack1: TD2HudTrack;
    HudTrackBar1: TD2HudTrackBar;
    Label1: TD2Label;
    Label2: TD2Label;
    HudListBox1: TD2HudListBox;
    HudTabControl1: TD2HudTabControl;
    HudTabItem1: TD2HudTabItem;
    Layout1: TD2Layout;
    HudTabItem2: TD2HudTabItem;
    Layout2: TD2Layout;
    HudSpinBox1: TD2HudSpinBox;
    HudCircleButton1: TD2HudCircleButton;
    HudRoundTextBox1: TD2HudRoundTextBox;
    HudCheckBox1: TD2HudCheckBox;
    HudRadioButton1: TD2HudRadioButton;
    HudStringComboBox1: TD2HudStringComboBox;
    procedure closeButtonMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmLayerDemo: TfrmLayerDemo;

implementation

{$R *.lfm}

procedure TfrmLayerDemo.closeButtonMouseUp(Sender: TObject;  Button: TMouseButton; Shift: TShiftState; X, Y: Single);
begin
  Close;
end;

end.
